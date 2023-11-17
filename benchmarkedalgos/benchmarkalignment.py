import time
import random
import timeit
from timeit import Timer
import numpy as np

# generate random DNA sequence of specified length
def DNA(length):
    return ''.join(random.choice('CGTA') for _ in range(length))

def initializeSkipTable(string, size):

    skipTable = [-1]*128

    # Fill table with index of last occurence
    for i in range(size):
        skipTable[ord(string[i])] = i

    return skipTable

def search(text, query):
    query_len = len(query)
    text_len = len(text)

    skipTable = initializeSkipTable(query, query_len) 

    s = 0
    while(s <= text_len-query_len):
        j = query_len-1

        while j>=0 and query[j] == text[s+j]:
            j -= 1

        if j<0:
            # print(s, ":", s+query_len-1)

            s += (query_len-skipTable[ord(text[s+query_len])] if s+query_len<text_len else 1)
        else:
            s += max(1, j-skipTable[ord(text[s+j])])


"""
This class contains methods and properties to perform BWT transformations
    Reversal, and pattern matching / querong.
"""
class BurrowsWheeler:
    def __init__(self):
        self.first_column = []              #List representing characters in first column of sorted matrix
        self.last_column = []               #List representing characters in last column of sorted matrix
        self.last_index = {}                #Dictionary where keys are characters in first column and values are their indices
        self.first_index = {}               #Dictionary where keys are characters in last column and values are their indices
        self.last_first_index = {}          #Dictionary mapping indices from last column to index in first column with the same character
        self.first_original_mapping = {}    #Dictionary mapping indices from first column to index in original string with the same character
    
    # Constructs the various columns and indixes from the transfomed string
    def construct_indicies(self, transform):
        # Make the first and last col, sort the first col to act as the sorted first col
        # is BWT matrix
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
        length = len(transform)
        # Hold the char of original string as constructed
        original = ['$']
        
        ''' get idx of $1 in last column using last index, use last_first index to map
            this index to the corresponding index in the first column

            retrieve the char from last column from mapped idx. The char returned is 
            right before $ in original string
        '''
        cur = self.last_column[self.last_first_index[self.last_index['$1']]]

        # appends $ instead of $1
        original.append(cur[:1])

        '''Reconsturct the remaining char of the orignal string. in each iteration
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
    
    '''Find all occurances of a given substring within the orignal string'''
    def query(self, query):
        '''get length of the original string + $ via first_column
            two pointers for top and bottom of last/first index
        '''
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
                    # print("Query doesn't match anywhere\n")
                    return
            # update top and bot in last_first_idx, mapping last colm to first columns
            top = self.last_first_index[top]
            bottom = self.last_first_index[bottom]
    
        '''Print the ranges at which the query occurs.
            uses first original mapping to get starting positions of the query
            in original string '''
        # for i in range(top, bottom+1):
            # print(self.first_original_mapping[i],":",self.first_original_mapping[i]+(len(query)-1))
def naive(query, text):
    occurrences = []
    for i in range(len(text) - len(query) + 1):  # loop over alignments
        match = True
        for j in range(len(query)):  # loop over characters
            if text[i+j] != query[j]:  # compare characters
                match = False
                break
        if match:
            occurrences.append(i)  # all chars matched; record
    return occurrences

def bwt(input):
    input = input + '$'
    indices = np.arange(len(input))
    sorted_indices = sorted(indices, key=lambda i: input[i:] + input[:i])
    bwt_encoded = ''.join(input[i - 1] for i in sorted_indices)
    return bwt_encoded
def searchBoyerBenchmark(len, txt):
    query = DNA(len)
    search(txt, query)
def searchBWTAlignBenchmark(len, bw):
    query = DNA(len)
    bw.query(query)
def searchNaiveBenchmark(len, txt):
    query = DNA(len)
    naive(query, txt)

def main():
    txt = DNA(10**5)
    counts = [1, 2, 5, 10, 20, 50, 100, 200]
    boyertimes = []
    bwttimes = []
    naivetimes = []
    transformed = bwt(txt)
    burrowswheeler = BurrowsWheeler()
    burrowswheeler.construct_indicies(transformed)
    for count in counts:
        print(count)
        boyertimer = Timer(lambda: searchBoyerBenchmark(count, txt))
        timeboyer = boyertimer.repeat(repeat=20, number=50)
        timeboyer = min(timeboyer)
        print(f"boyer time at {count} query length: {timeboyer}")
        boyertimes.append(timeboyer)
        bwttimer = Timer(lambda: searchBWTAlignBenchmark(count, burrowswheeler))
        timebwt = bwttimer.repeat(repeat=20, number=50)
        timebwt = min(timebwt)
        print(f"bwt align time at {count} query length: {timebwt}")
        bwttimes.append(timebwt)
        naivetimer = Timer(lambda: searchNaiveBenchmark(count, txt))
        timenaive = naivetimer.repeat(repeat=20, number=50)
        timenaive = min(timenaive)
        print(f"naive align time at {count} query length: {timenaive}")
        naivetimes.append(timenaive)
    print(boyertimes)
    print(bwttimes)
    print(naivetimes)


if __name__ == '__main__':
    print("starting")
    main()