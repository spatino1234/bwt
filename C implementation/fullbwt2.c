#include "fullbwt2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

Node *addtoList(int val, Node *list) {
    Node *newNode;
    newNode->value = val;
    newNode->next = list;
    return newNode;
}

void displayList(Node *list) {
    Node *cur = list;
    while (cur != NULL) {
        printf("%d\n",cur->value);
        cur = cur->next;
    }
}

int alphabeticalComp(const void *a, const void *b) { 
       return *((char *)a) - *((char *)b); 
}

int encodeCharacter(char character) {
    int value = 0;
    switch (character)
    {
    case 'A':
        value = 0;
        break;
    case 'C':
        value = 1;
        break;
    case 'T':
        value = 2;
        break;
    case 'G':
        value = 3;
        break;
    case '$':
        value = 4; 
        break;
    }
    return value;
}

char decodeCharacter (int encoding) {
    char character;
    switch (encoding)
    {
    case 0:
        character = 'A';
        break;
    case 1:
        character = 'C';
        break;
    case 2:
        character = 'T';
        break;
    case 3:
        character = 'G';
        break;
    case 4:
        character = '$';
        break;
    }
    return character;
}

void constructIndices(BurrowsWheeler *BW, char *transform) {
    
    // crete arrays for the last column
    int transform_length = strlen(transform);
    BW->length = transform_length;
    // intpair last_column[transform_length];
    // intpair first_column[transform_length];

    char sorted_transform[transform_length];
    memcpy(sorted_transform, transform, strlen(transform)+1);
    qsort(sorted_transform, strlen(transform), sizeof(char), alphabeticalComp);

    int last_index[transform_length][5], first_index[transform_length][5];
    // initialize each element of both counters to 0
    int first_encounters[5] = {0};

    int nextchar;
    int nextcharEncoded;
    intpair firstintpair;
    for (int i=0;i<transform_length;i++) {
        nextchar = sorted_transform[i];
        nextcharEncoded = encodeCharacter(nextchar);
        first_encounters[nextcharEncoded]++;

        firstintpair.inputchar = nextcharEncoded;
        firstintpair.inputint = first_encounters[nextcharEncoded]-1;

        BW->first_column[i] = firstintpair;
        BW->first_index[firstintpair.inputint][firstintpair.inputchar] = i;
    }

    int firstloc;
    intpair lastintpair;

    int last_encounters[5] = {0};

    for (int i=0;i<transform_length;i++) {
        nextchar = transform[i];
        nextcharEncoded = encodeCharacter(nextchar);
        last_encounters[nextcharEncoded]++;

        lastintpair.inputchar = nextcharEncoded;
        lastintpair.inputint = last_encounters[nextcharEncoded]-1;

        BW->last_column[i] = lastintpair;
        BW->last_index[lastintpair.inputint][lastintpair.inputchar] = i;

        
        firstloc = BW->first_index[lastintpair.inputint][lastintpair.inputchar];
        BW->last_first_index[i] = firstloc;
    }

    BW->first_original_mapping[BW->first_index[0][4]] = transform_length-1;

    intpair cur = BW->last_column[BW->last_first_index[BW->last_index[0][4]]];
    BW->first_original_mapping[BW->first_index[cur.inputint][cur.inputchar]] = transform_length-2;

    for (int i=0;i<transform_length-2;i++) {
        cur = BW->last_column[BW->last_first_index[BW->last_index[cur.inputint][cur.inputchar]]];
        BW->first_original_mapping[BW->first_index[cur.inputint][cur.inputchar]] = transform_length-3-i;
    }
}

void revstr(char *str1)  
{  
    // declare variable  
    int i, len, temp;  
    len = strlen(str1); // use strlen() to get the length of str string  
      
    // use for loop to iterate the string   
    for (i = 0; i < len/2; i++)  
    {  
        // temp variable use to temporary hold the string  
        temp = str1[i];  
        str1[i] = str1[len - i - 1];  
        str1[len - i - 1] = temp;  
    }  
}  

char *reverse(BurrowsWheeler *BW) {
    char reversal[BW->length];
    
    intpair cur = BW->last_column[BW->last_index[0][4]];

    for (int i = 0; i < BW->length-1; i++) {
        cur = BW->last_column[BW->last_first_index[BW->last_index[cur.inputint][cur.inputchar]]];
        
        char character = decodeCharacter(cur.inputchar);
        strncat(reversal, &character, 1);
    }
    revstr(reversal);

    return reversal;
}

void query(BurrowsWheeler *BW, char *query) {
    int top = 0;
    int bottom = (BW->length)-1;
    int query_len = strlen(query);

    for (int i=0;i<(query_len); i++) {
        char cur_query_char = query[query_len-1-i];
        for (int j = 0; j<(BW->length); j++) {
            bool top_set = false;

            if (BW->last_column[top].inputchar != encodeCharacter(cur_query_char)) {
                top++;
            }
            else {
                top_set = true;
            }
            if (BW->last_column[bottom].inputchar != encodeCharacter(cur_query_char)) {
                bottom--;
            }
            else {
                if (top_set == true) {
                    break;
                }
            }
            if (top > bottom) {
                printf("Query Doesn't Match Anywhere\n");
                exit(0);
            }
        }
        top = BW->last_first_index[top];
        bottom = BW->last_first_index[bottom];
    }

    char query_format[BW->length];
    for (int i=0;i<BW->length;i++) {
        *(query_format+i) = ' ';
    }

    for (int i=top;i<bottom+1;i++) {
        int match = BW->first_original_mapping[i];

        // query_format[match] = '^';
        strncpy(&query_format[match], query, query_len);
    }
    printf("\n%s\n", reverse(BW));
    printf("%s\n\n", query_format);
}

BurrowsWheeler *initBW(int malloc_length) {
    BurrowsWheeler *returnstruct = malloc(sizeof(BurrowsWheeler));
    returnstruct->malloc_length = malloc_length;

    returnstruct->last_index = (int **)malloc(sizeof(int*)*malloc_length);
    returnstruct->first_index = (int **)malloc(sizeof(int*)*malloc_length);
    
    for (int i = 0; i < malloc_length; i++) {
        returnstruct->last_index[i] = malloc(sizeof(int) * malloc_length);
        returnstruct->first_index[i] = malloc(sizeof(int) * malloc_length);
    }
    
    returnstruct->last_first_index = malloc(sizeof(int) * malloc_length);
    returnstruct->first_original_mapping = malloc(sizeof(int) * malloc_length);
    return returnstruct;
}

void freeBW(BurrowsWheeler *BW) {
    free(BW->first_original_mapping);
    free(BW->last_first_index);
    
    for (int i=0;i<(BW->malloc_length);i++) {
        free(BW->last_index[i]);
        free(BW->first_index[i]);
    }
    free(BW->last_index);
    free(BW->first_index);

    free(BW);
}

int main(){
    char *transform = "ATTG$AA";
    char *transform2 = "TT$GGGGGGGGGGGGTCCCAGGTAAAAAAAAAAAAAATTTTTTTTTTTTTTACATACCCCCCCCCCCCCTC";
    char *transform3 = "AGTATTTACGTGTCACTGCGTTTT$CGATTTTTTTTCTCCCTACGTCTAGTGTGGGCATAAGCCGAGACA";
    char *query1;
    printf("Enter a query: ");
    scanf("%s", query1);
    BurrowsWheeler *BW = initBW(100);
    constructIndices(BW, transform3);
    query(BW, query1);

    freeBW(BW);
    return 0;
}