import timeit
from timeit import Timer
import random
import matplotlib.pyplot as plt
import numpy as np

class InducedSorter:
    def __init__(self, arr):
        self.alphabet_size = len(set(arr))
        self.arr = [item + 1 for item in arr]
        self.arr.append(0)
        self.alphabet_size += 1

    def characterizeLetters(self):
        categories = [0 for _ in range(len(self.arr))]
        categories[len(self.arr)-1] = "*"
        for index, char in reversed(list(enumerate(self.arr[:-1]))):
            compare = index
            while (self.arr[compare] == char):
                compare +=1
            if char > self.arr[compare]:
                categories[index] = "-"
                if categories[index+1] == "+":
                    categories[index+1] = "*"
            else:
                categories[index] = "+"
        return(categories)

    def suffix(self, index):
        return ''.join(self.arr[index:])

    def createStarSubstrings(self, categories):
        current_string = []
        has_hit_star = False
        all_star_substrings = []
        star_list = []
        for index, char in enumerate(self.arr):
            if categories[index] == "*":
                star_list.append(index)
                if not has_hit_star:
                    has_hit_star = True
                else:
                    current_string.append(char)
                    all_star_substrings.append(current_string)
                current_string = [char]
            elif has_hit_star:
                current_string.append(char)
        all_star_substrings.append(current_string)
        return(all_star_substrings, star_list)
    
    def createNames(star_substrings):
        current_substring = star_substrings[0]
        current_name = 0
        names = []
        for substring in star_substrings:
            if substring != current_substring:
                current_name += 1
            names.append(current_name)

    def count_sort_letters(self, array, size, col, base, max_len):
        """ Helper routine for performing a count sort based upon column col """
        output   = [0] * size
        count    = [0] * (base + 1) # One addition cell to account for dummy letter

        for item in array: # generate Counts
            # get column letter if within string, else use dummy position of 0
            letter = item[1][col] if col < len(item[1]) else 0
            count[letter] += 1

        for i in range(len(count)-1):   # Accumulate counts
            count[i + 1] += count[i]

        for item in reversed(array):
            # Get index of current letter of item at index col in count array
            letter = item[1][col] if col < len(item[1]) else 0
            output[count[letter] - 1] = item
            count[letter] -= 1
        return output

    def radix_sort_letters(self, array, max_col = None):
        """ Main sorting routine """
        if not max_col:
            max_col = len(max([item[1] for item in array], key = len)) # edit to max length

        for col in range(max_col-1, -1, -1): # max_len-1, max_len-2, ...0
            array = self.count_sort_letters(array, len(array), col, self.alphabet_size, max_col)

        return array
    
    def numberedStarSubstrings(self, radix_sorted):
        number_array = [0 for _ in range(len(radix_sorted))]
        current_str = radix_sorted[0][1]
        current_val = 0
        for index, (original_index, substring) in enumerate(radix_sorted):
            if substring != current_str:
                current_val += 1
                current_str = substring
            number_array[original_index] = current_val
        return number_array
    def induceMinus(self, C_minus, C_star):
        C_minus[self.arr[len(self.arr) - 2]].append(len(self.arr) - 2)
        for a in range(1, self.alphabet_size):
            C = []
            while C_minus[a]:
                i = C_minus[a][0]
                C_minus[a] = C_minus[a][1:]
                C.append(i)
                if i > 0 and self.arr[i-1] >= a:
                    C_minus[self.arr[i-1]].append(i - 1)
            C_minus[a] = C
            for i in C_star[a]:
                C_minus[self.arr[i-1]].append(i - 1)

    def inducePlus(self, C_minus, C_plus):
        for a in range(self.alphabet_size-1, 0, -1):
            C = []
            while C_plus[a]:
                i = C_plus[a].pop()
                C.insert(0, i)
                if i > 0 and self.arr[i-1] <= a:
                    C_plus[self.arr[i-1]].insert(0, i - 1)
            C_plus[a] = C
            for i in C_minus[a][::-1]:
                if i > 0 and self.arr[i-1] < a:
                    C_plus[self.arr[i-1]].insert(0, i - 1)

    def makeSuffix(self, C_minus, C_plus):
        suffix = [len(self.arr) - 1]
        for minus, plus in zip(C_minus, C_plus):
            suffix = suffix + minus + plus
        return suffix

    def inducedSort(self):
        C_star = [[] for _  in range(self.alphabet_size)]
        C_minus = [[] for _  in range(self.alphabet_size)]
        C_plus = [[] for _  in range(self.alphabet_size)]
        categories = self.characterizeLetters()
        star_substrings, star_list = self.createStarSubstrings(categories)
        tuple_substrings = [(index, substring) for index, substring in zip(range(len(star_substrings)), star_substrings)]
        radix_sorted = self.radix_sort_letters(tuple_substrings)
        number_array = self.numberedStarSubstrings(radix_sorted)
        if len(number_array) == len(set(number_array)):
            recursive_suffix = [0 for _ in number_array]
            for i, num in enumerate(number_array):
                recursive_suffix[num] = i
        else:
            recursive_sorter = InducedSorter([item - 1 for item in number_array[:-1]])
            recursive_suffix = recursive_sorter.inducedSort()
        for suffix_num in recursive_suffix:
            star_index = star_list[suffix_num]
            star_char = self.arr[star_index]
            C_star[star_char].append(star_index)
        self.induceMinus(C_minus, C_star)
        self.inducePlus(C_minus, C_plus)
        suffix = self.makeSuffix(C_minus, C_plus)
        return suffix
            
def DNA(length):
    return ''.join(random.choice('CGTA') for _ in range(length))

def suffixToBwt(suffix_arr, str):
    BWT = ""
    for loc in suffix_arr:
        if loc == 0:
            BWT += "$"
        else:
            BWT += str[loc - 1]
    return BWT

if __name__ == '__main__':
    timesbase = []
    counts = [100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000]
    for count in counts:
        str = DNA(count)
        alphabet_list = list(set(str))
        str = [*str]
        alph = {char: index for index, char in enumerate(sorted(alphabet_list))}
        arr_instance = [alph[char] for char in str]

        sorter = InducedSorter(arr_instance)

        timebaseTimer = Timer(lambda: sorter.inducedSort())
        timebase = timebaseTimer.repeat(repeat=20, number=50)
        print(timebase, count)
        timebase = min(timebase)
        timesbase.append(timebase)
    print(timesbase)