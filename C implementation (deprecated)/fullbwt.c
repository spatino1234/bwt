#include "fullbwt.h"
#include <stdio.h>
#include <string.h>

int alphabeticalComp(const void *a, const void *b) { 
       return *((char *)a) - *((char *)b); 
}

void add_to_char_loc_dictionary(charint key, int loc, char_loc_dictionary *dict)
{
    // the item to add to the dict
    char_loc_dictionary *s;

    // malloc and assign values to s, can be freed by iterating over the dict
    s = (char_loc_dictionary*)malloc(sizeof *s);
    // need to set s entirely to 0 before adding this_charint
    memset(s, 0, sizeof *s);
    s->this_charint = key;
    s->loc = loc;
    // hh is UT_hash_handle, dict is the dictionary, this_charint is the key field, sizeof(charint) is key size, s is the item to add to dict
    HASH_ADD(hh, dict, this_charint, sizeof(charint), s);
}

void add_to_lf_dictionary(int last_loc, int first_loc, last_first_dictionary *lf_dict)
{
    // see more detailed implementation in add_to_char_loc_dictionary
    last_first_dictionary *s;

    s = (last_first_dictionary*)malloc(sizeof *s);
    s->last_char_loc = last_loc;
    s->first_char_loc = first_loc;
    HASH_ADD_INT(lf_dict, last_char_loc, s);
}

void constructIndices(char *transform) {
    // crete arrays for the last column
    int transform_length = strlen(transform);
    charint last_column[transform_length];
    charint first_column[transform_length];

    char sorted_transform[transform_length];
    memcpy(sorted_transform, transform, strlen(transform)+1);
    qsort(sorted_transform, strlen(transform), sizeof(char), alphabeticalComp);
    
    last_first_dictionary *lf_dict = NULL;
    char_loc_dictionary *last_index, *first_index = NULL;
    encounter_dict last_encounters;
    encounter_dict first_encounters;

    first_encounters.A = 0;
    first_encounters.C = 0;
    first_encounters.T = 0;
    first_encounters.G = 0;
    first_encounters.$ = 0;
    for (int i=0;i<transform_length;i++) {
        charint firstcharint;
        switch (*(transform+i*sizeof(char))) {
            case 'A':
                first_encounters.A++;
                firstcharint.inputchar = 'A';
                firstcharint.inputint = first_encounters.A;
                break;
            case 'C':
                first_encounters.C++;
                firstcharint.inputchar = 'C'; 
                firstcharint.inputint = first_encounters.C;
                break;
            case 'T':
                first_encounters.T++;
                firstcharint.inputchar = 'T'; 
                firstcharint.inputint = first_encounters.T;
                break;
            case 'G':
                first_encounters.G++;
                firstcharint.inputchar = 'G'; 
                firstcharint.inputint = first_encounters.G;
                break;
            case '$':
                first_encounters.$++;
                firstcharint.inputchar = '$'; 
                firstcharint.inputint = first_encounters.$;
                break;
        }
        first_column[i] = firstcharint;
        add_to_char_loc_dictionary(firstcharint, i, first_index);
    }

    last_encounters.A = 0;
    last_encounters.C = 0;
    last_encounters.T = 0;
    last_encounters.G = 0;
    last_encounters.$ = 0;
    
    for (int i=0;i<transform_length;i++) {
        charint lastcharint;
        switch (*(transform+i*sizeof(char))) {
            case 'A':
                last_encounters.A++;
                lastcharint.inputchar = 'A';
                lastcharint.inputint = last_encounters.A;
                break;
            case 'C':
                last_encounters.C++;
                lastcharint.inputchar = 'C'; 
                lastcharint.inputint = last_encounters.C;
                break;
            case 'T':
                last_encounters.T++;
                lastcharint.inputchar = 'T'; 
                lastcharint.inputint = last_encounters.T;
                break;
            case 'G':
                last_encounters.G++;
                lastcharint.inputchar = 'G'; 
                lastcharint.inputint = last_encounters.G;
                break;
            case '$':
                last_encounters.$++;
                lastcharint.inputchar = '$'; 
                lastcharint.inputint = last_encounters.$;
                break;
        }
        // Use this if necessary: https://stackoverflow.com/questions/58752824/uthash-adding-a-new-entry-to-a-struct-struct-hashmap
        last_column[i] = lastcharint;
        add_to_char_loc_dictionary(lastcharint, i, last_index);
        char_loc_dictionary *first_pair;
        char_loc_dictionary l;
        memset(&l, 0, sizeof(char_loc_dictionary));
        l.this_charint = lastcharint;
        HASH_FIND(hh, first_index, &l.this_charint, sizeof(charint), first_pair);
        int firstloc = first_pair->loc;
        add_to_lf_dictionary(i, firstloc, lf_dict);
    }

    for (int i = 0; i < transform_length; i++) {
        last_first_dictionary *output_lf;
        // this finds the corresponding index in the first column to one in the last column
        HASH_FIND_INT(lf_dict, i, lf_dict);
        
        // printf("First: %c %d \t Last: %c %d\n", first_column.inputchar, first_column.inputint, last_column.inputchar, last_column.inputint);
    }

    // printf("%d\n", encounters.A);
    // printf("%d\n", encounters.C);
    // printf("%d\n", encounters.T);
    // printf("%d\n", encounters.G);

    // to free
    char_loc_dictionary *p, *tmp = NULL;
    HASH_ITER(hh, last_index, p, tmp) {
      HASH_DEL(last_index, p);
      free(p);
    }
    HASH_ITER(hh, first_index, p, tmp) {
      HASH_DEL(first_index, p);
      free(p);
    }
    HASH_ITER(hh, last_index, p, tmp) {
      HASH_DEL(last_index, p);
      free(p);
    }
    last_first_dictionary *p, *tmp;
    HASH_ITER(hh, lf_dict, p, tmp) {
      HASH_DEL(lf_dict, p);
      free(p);
    }
}

int main(){
    char *transform = "ATTG$AA";
    
    // print string
    printf("%s\n", transform);
    constructIndices(transform);
    return 0;
}

// import numpy as np
// """
// This class contains methods and properties to perform BWT transformations
//     Reversal, and pattern matching / querong.
// """
// class BurrowsWheeler:
//     def __init__(self):
//         self.first_column = []              #List representing characters in first column of sorted matrix
//         self.last_column = []               #List representing characters in last column of sorted matrix
//         self.last_index = {}                #Dictionary where keys are characters in first column and values are their indices
//         self.first_index = {}               #Dictionary where keys are characters in last column and values are their indices
//         self.last_first_index = {}          #Dictionary mapping indices from last column to index in first column with the same character
//         self.first_original_mapping = {}    #Dictionary mapping indices from first column to index in original string with the same character
    
//     # Constructs the various columns and indixes from the transfomed string
//     def construct_indicies(self, transform):
//         # Make the first and last col, sort the first col to act as the sorted first col
//         # is BWT matrix
//         self.last_column = [*transform]
//         self.first_column = [*transform]
//         self.first_column.sort()


//         ''' track the occurance of characters in last column using the encounters dictionary, modify the character 
//             make each char itself + its count (eg. A3, $1). ensuring each char is unique and easier
//             to track for the reversal later on
//         '''
//         encounters = {'$':0,'G':0,'A':0,'C':0,'T':0}

//         for i in range(len(self.last_column)):
//             encounters[self.last_column[i]] += 1
//             concatenation = self.last_column[i] + str(encounters[self.last_column[i]])
//             self.last_column[i] = concatenation
//             self.last_index[concatenation] = i

//         '''track occurance of char in first column, track them ussing occurance dict and modify the char
//             uses the char & its count
//         '''

//         encounters = {'$':0,'G':0,'A':0,'C':0,'T':0}
//         for i in range(len(self.first_column)):
//             encounters[self.first_column[i]] += 1
//             concatenation = self.first_column[i] + str(encounters[self.first_column[i]])
//             self.first_column[i] = concatenation
//             self.first_index[concatenation] = i

//         '''create a mapping from last column to first column to act as the FM index'''
//         for key in self.last_index:
//             self.last_first_index[self.last_index[key]] = self.first_index[key]

//         '''Mapping from first column back to orignal string
//             Utilizes the BWT property that $ is last character in original string.
//             Reconstruct the string from $ backwards.
//         '''

//         length = len(transform)
//         self.first_original_mapping[self.first_index['$1']] = length-1
//         cur = self.last_column[self.last_first_index[self.last_index['$1']]]
//         self.first_original_mapping[self.first_index[cur]] = length-2
//         for i in range(len(transform)-2):
//             cur = self.last_column[self.last_first_index[self.last_index[cur]]]
//             self.first_original_mapping[self.first_index[cur]] = length-3-i

//     def reverse(self, transform):
//         length = len(transform)
//         # Hold the char of original string as constructed
//         original = ['$']
        
//         ''' get idx of $1 in last column using last index, use last_first index to map
//             this index to the corresponding index in the first column

//             retrieve the char from last column from mapped idx. The char returned is 
//             right before $ in original string
//         '''
//         cur = self.last_column[self.last_first_index[self.last_index['$1']]]

//         # appends $ instead of $1
//         original.append(cur[:1])

//         '''Reconsturct the remaining char of the orignal string. in each iteration
//             retrieve next char that precedes the current one in the original string
//             using last Column and first Columns.

//             iterate over len(transform)-2 since the $ and character preceding it have been added
//         '''
//         for i in range(len(transform)-2):
//             cur = self.last_column[self.last_first_index[self.last_index[cur]]]
//             original.append(cur[:1]) #Slice out number
        
//         ''' Reverse the reversed original string, so we get original strung
//             T$CA --> CAT$
//         '''

//         original.reverse()
//         return "".join(original)
    
//     '''Find all occurances of a given substring within the orignal string'''
//     def query(self, query):
//         '''get length of the original string + $ via first_column
//             two pointers for top and bottom of last/first index
//         '''
//         length = len(self.first_column)
//         top = 0
//         bottom = length-1

//         ''' iterate through query in reverse
//             query = NA --> Start at 'A' then do 'NA'
//         '''
//         for i in range(1,len(query)+1):
//             cur_query_char = query[-i]
//             '''adjust pointers off cur_query_char. Pointers move towards eachother till they point
//                 first and last occurance of the query. If top pointer surpasses bottom, 
//                 then we didn't find any valid substring.    
//             '''
//             for i in range(length):
//                 top_set = False
//                 if self.last_column[top][:1] != cur_query_char:
//                     top += 1
//                 else:
//                     top_set = True
//                 if self.last_column[bottom][:1] != cur_query_char:
//                     bottom -= 1
//                 else:
//                     if top_set:
//                         break
//                 if top > bottom:
//                     print("Query doesn't match anywhere\n")
//                     return
//             # update top and bot in last_first_idx, mapping last colm to first columns
//             top = self.last_first_index[top]
//             bottom = self.last_first_index[bottom]
    
//         '''Print the ranges at which the query occurs.
//             uses first original mapping to get starting positions of the query
//             in original string '''
//         for i in range(top, bottom+1):
//             print(self.first_original_mapping[i],":",self.first_original_mapping[i]+(len(query)-1))

// def bwt(input):
//     input = input + '$'
//     indices = np.arange(len(input))
//     sorted_indices = sorted(indices, key=lambda i: input[i:] + input[:i])
//     bwt_encoded = ''.join(input[i - 1] for i in sorted_indices)
//     return bwt_encoded

// def main():
//     string = "ACATGAGCAT"
//     transformed = bwt(string)
//     print(transformed)
//     burrowswheeler = BurrowsWheeler()
//     burrowswheeler.construct_indicies(transformed)
//     print(burrowswheeler.reverse(transformed))
//     burrowswheeler.query("CAT")

// if __name__ == "__main__":
//     main()