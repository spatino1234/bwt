import numpy as np
"""
This class contains methods and properties to perform BWT transformations
    Reversal, and pattern matching / querong.

This class implemenets the Burrows Wheeler Transform (BWT) to take a string 
    and transform it w/ BWT, reversing the transformed string, and perform
    pattern matching queries on the transformed
"""
class BurrowsWheeler:
    def __init__(self):
        # Lists representing characters in the first and last columns of the BWT matrix
        self.first_column = []              
        self.last_column = []               

        # Dictionaries for mapping characters to their indices in the first and last columns
        self.last_index = {}                
        self.first_index = {}    

        # Dictionary mapping indices from the last column to the corresponding index
        # in the first column with the same character
        self.last_first_index = {}
        self.first_original_mapping = {}
    
    def construct_indicies(self, transform):

        """
        Constructs the necessary data structures from the transformed string.
        This includes the first and last columns of the BWT matrix, along with several
        mappings for indices and characters.

        @param transform: The transformed string using BWT.
        """

        self.last_column = [*transform]
        self.first_column = [*transform]
        self.first_column.sort()


        ''' track the occurance of characters in last column using the encounters dictionary, modify the character 
            make each char itself + its count (eg. A3, $1). ensuring each char is unique and easier
            to track for the reversal later on
        '''
        encounters = {'$':0,'G':0,'A':0,'C':0,'T':0}

        for i in range(len(self.last_column)):
            encounters[self.last_column[i]] += 1
            concatenation = self.last_column[i] + str(encounters[self.last_column[i]])
            self.last_column[i] = concatenation
            self.last_index[concatenation] = i

        '''track occurance of char in first column, track them ussing occurance dict and modify the char
            uses the char & its count
        '''

        encounters = {'$':0,'G':0,'A':0,'C':0,'T':0}
        for i in range(len(self.first_column)):
            encounters[self.first_column[i]] += 1
            concatenation = self.first_column[i] + str(encounters[self.first_column[i]])
            self.first_column[i] = concatenation
            self.first_index[concatenation] = i

        '''create a mapping from last column to first column to act as the FM index'''
        for key in self.last_index:
            self.last_first_index[self.last_index[key]] = self.first_index[key]

        '''Mapping from first column back to orignal string
            Utilizes the BWT property that $ is last character in original string.
            Reconstruct the string from $ backwards.
        '''

        length = len(transform)
        self.first_original_mapping[self.first_index['$1']] = length-1
        cur = self.last_column[self.last_first_index[self.last_index['$1']]]
        self.first_original_mapping[self.first_index[cur]] = length-2
        for i in range(len(transform)-2):
            cur = self.last_column[self.last_first_index[self.last_index[cur]]]
            self.first_original_mapping[self.first_index[cur]] = length-3-i

    def reverse(self, transform):
        """
        Reverses the Burrows-Wheeler Transform to reconstruct the original string.

        :param transform: The transformed string using BWT.
        :return: The original string before the BWT transformation.
        """
        length = len(transform)
        # Hold the char of original string as constructed
        original = ['$']
        
        # Reconstructing the string starting from the '$' symbol
        cur = self.last_column[self.last_first_index[self.last_index['$1']]]

        # appends $ instead of $1
        original.append(cur[:1])

        '''Reconstruct the remaining char of the orignal string. in each iteration
            retrieve next char that precedes the current one in the original string
            using last Column and first Columns.

            iterate over len(transform)-2 since the $ and character preceding it have been added
        '''
        for i in range(len(transform)-2):
            cur = self.last_column[self.last_first_index[self.last_index[cur]]]
            original.append(cur[:1]) #Slice out number
        
        ''' Reverse the reversed original string, so we get original strung
            T$CA --> CAT$
        '''

        original.reverse()
        return "".join(original)
    
    def query(self, query):
        """
        Performs a pattern matching query to find all occurrences of a given substring
        within the original string.

        :param query: The substring to search for in the original string.
        """
        length = len(self.first_column)
        top = 0
        bottom = length-1

        ''' iterate through query in reverse
            query = NA --> Start at 'A' then do 'NA'
        '''
        for i in range(1,len(query)+1):
            cur_query_char = query[-i]
            '''adjust pointers off cur_query_char. Pointers move towards eachother till they point
                first and last occurance of the query. If top pointer surpasses bottom, 
                then we didn't find any valid substring.    
            '''
            for i in range(length):
                top_set = False
                if self.last_column[top][:1] != cur_query_char:
                    top += 1
                else:
                    top_set = True
                if self.last_column[bottom][:1] != cur_query_char:
                    bottom -= 1
                else:
                    if top_set:
                        break
                if top > bottom:
                    print("Query doesn't match anywhere\n")
                    return
            # update top and bot in last_first_idx, mapping last colm to first columns
            top = self.last_first_index[top]
            bottom = self.last_first_index[bottom]
    
        '''Print the ranges at which the query occurs.
            uses first original mapping to get starting positions of the query
            in original string '''
        for i in range(top, bottom+1):
            print(self.first_original_mapping[i],":",self.first_original_mapping[i]+(len(query)-1))

def bwt(input):
    input = input + '$'
    indices = np.arange(len(input))
    sorted_indices = sorted(indices, key=lambda i: input[i:] + input[:i])
    bwt_encoded = ''.join(input[i - 1] for i in sorted_indices)
    return bwt_encoded

def main():
    string = "ACATGAGCAT"
    transformed = bwt(string)
    print(transformed)
    burrowswheeler = BurrowsWheeler()
    burrowswheeler.construct_indicies(transformed)
    print(burrowswheeler.reverse(transformed))
    burrowswheeler.query("CAT")

if __name__ == "__main__":
    main()