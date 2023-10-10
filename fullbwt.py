import numpy as np

class BurrowsWheeler:
    def __init__(self):
        self.first_column = []              #List representing characters in first column of sorted matrix
        self.last_column = []               #List representing characters in last column of sorted matrix
        self.last_index = {}                #Dictionary where keys are characters in first column and values are their indices
        self.first_index = {}               #Dictionary where keys are characters in last column and values are their indices
        self.last_first_index = {}          #Dictionary mapping indices from last column to index in first column with the same character
        self.first_original_mapping = {}    #Dictionary mapping indices from first column to index in original string with the same character
    
    def construct_indicies(self, transform):
        self.last_column = [*transform]
        self.first_column = [*transform]
        self.first_column.sort()

        encounters = {'$':0,'G':0,'A':0,'C':0,'T':0}

        for i in range(len(self.last_column)):
            encounters[self.last_column[i]] += 1
            concatenation = self.last_column[i] + str(encounters[self.last_column[i]])
            self.last_column[i] = concatenation
            self.last_index[concatenation] = i

        encounters = {'$':0,'G':0,'A':0,'C':0,'T':0}

        for i in range(len(self.first_column)):
            encounters[self.first_column[i]] += 1
            concatenation = self.first_column[i] + str(encounters[self.first_column[i]])
            self.first_column[i] = concatenation
            self.first_index[concatenation] = i

        for key in self.last_index:
            self.last_first_index[self.last_index[key]] = self.first_index[key]

        length = len(transform)
        self.first_original_mapping[self.first_index['$1']] = length-1

        cur = self.last_column[self.last_first_index[self.last_index['$1']]]

        self.first_original_mapping[self.first_index[cur]] = length-2

        for i in range(len(transform)-2):
            cur = self.last_column[self.last_first_index[self.last_index[cur]]]
            self.first_original_mapping[self.first_index[cur]] = length-3-i

    def reverse(self, transform):
        length = len(transform)

        original = ['$']
        cur = self.last_column[self.last_first_index[self.last_index['$1']]]

        original.append(cur[:1])
        for i in range(len(transform)-2):
            cur = self.last_column[self.last_first_index[self.last_index[cur]]]
            original.append(cur[:1]) #Slice out number
        
        original.reverse()
        return "".join(original)
    
    def query(self, query):
        length = len(self.first_column)

        top = 0
        bottom = length-1

        for i in range(1,len(query)+1):
            cur_query_char = query[-i]

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

            top = self.last_first_index[top]
            bottom = self.last_first_index[bottom]

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