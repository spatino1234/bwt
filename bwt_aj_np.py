'''bwt_aj_np.py'''
"A bwt implementation in python done by yours truly, this time with numpy juice from concentrate"
import numpy as np

input = input()

input = input + '$'

indices = np.arange(len(input))

# Permute and lexicographically sort string permutations
sorted_indices = sorted(indices, key=lambda i: input[i:] + input[:i])
# The key function takes an index i and constructs a string by concatenating the substring 
# of input from index i to the end (input[i:]) with the substring from the beginning to 
# index i (input[:i]). This effectively sorts the indices based on their corresponding cyclic permutations of the input string.

# Short version: The output is a list of indexes ordered lexicographically, this is used in the following step:

# use sorted indices to map final characters of sorted permutations to a variable
bwt_encoded = ''.join(input[i - 1] for i in sorted_indices)
# Above list comprehension takes an index in sorted_indices and joins input[i - 1] into the encoded string
# This works because the bwt encoding phase is a series of rightward permutations
# This means that in the original string, if i is your first sorted index, i-1 is the final character in the permuted string
# Running through the entire list using the inline for loop builds the codeword based off the sorted indices

print(f"Your BWT encoded codeword is: {bwt_encoded}")







