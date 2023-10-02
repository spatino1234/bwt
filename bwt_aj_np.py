'''bwt_aj_np.py'''
"A bwt implementation in python done by yours truly, this time with numpy juice from concentrate"
import numpy as np

input = input()

input = input + '$'

indices = np.arange(len(input))

# Lots of cool stuff to talk about here:
sorted_indices = sorted(indices, key=lambda i: input[i:] + input[:i])
print(sorted_indices)
# One-line key function both creates, then sorts lexicographically, the permutations

# use sorted indices to map final characters of sorted permutations to a variable
bwt_encoded = ''.join(input[i - 1] for i in sorted_indices)

print(f"Your BWT encoded codeword is: {bwt_encoded}")







