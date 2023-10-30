import timeit
from timeit import Timer
import random
import matplotlib.pyplot as plt
import numpy as np

'''bwt_aj_np.py'''
"A bwt implementation in python done by yours truly, this time with numpy juice from concentrate"

def aj_bwt(input):

    starttime = timeit.default_timer()

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

    # print(f"Your BWT encoded codeword is: {bwt_encoded}")
    return bwt_encoded
    # return(timeit.default_timer() - starttime)


def bwt(s: str):
    # starttime = timeit.default_timer()
    original = s  # Store the original string
    a = []
    temp = s[-1] + s[:-1]  # Initial rotation
    a.append(temp)

    while temp != original:  # Stop when temp equals the original string
        # print(temp)
        s = temp  # Update s with the newly rotated string
        temp = s[-1] + s[:-1]  # Perform another rotation
        a.append(temp)
    a.sort()

    # take the last item col of the char of the array
    # returns the bwtTransform
    bwtResult = ""
    for s in a:
        bwtResult += s[-1]
    return bwtResult
    # return(timeit.default_timer() - starttime)

    
def DNA(length):
    return ''.join(random.choice('CGTA') for _ in range(length))

starttime = timeit.default_timer()
timesbase = []
timesaj = []
counts = [100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000]
# counts = [100, 200, 500, 1000, 2000]
for count in counts:
    a = DNA(count)
    # timebase = res.bwt(a)
    timebaseTimer = Timer(lambda: bwt(a))
    timebase = timebaseTimer.repeat(repeat=10, number=20)
    print(timebase, count)
    timebase = min(timebase)
    timesbase.append(timebase)
    # timeaj = aj_bwt(a)
    timeajTimer = Timer(lambda: aj_bwt(a))
    timeaj = timeajTimer.repeat(repeat=10, number=20)
    print(timeaj, count)
    timeaj = min(timeaj)
    timesaj.append(timeaj)
print(timesbase)
print(timesaj)
# timesradix = [0.000046, 0.000101, 0.000298, 0.000739, 0.002430, 0.008730, 0.020783, 0.052353, 0.213522]
# timesqsort = [0.000068, 0.000163, 0.000608, 0.001665, 0.004381, 0.013280, 0.030586, 0.072626, 0.270026]
timesradix = [0.000126, 0.000209, 0.000663, 0.001554, 0.004983, 0.016651, 0.040988, 0.104231, 0.434247]
timesqsort = [0.000140, 0.000320, 0.001189, 0.003436, 0.008685, 0.026584, 0.064186, 0.147037, 0.547143]
plt.xscale("log")
plt.yscale("log")
plt.xlabel("DNA Length")
plt.ylabel("Time to complete")
plt.plot(counts, timesbase)
plt.plot(counts, timesaj)
plt.plot(counts, timesradix)
plt.plot(counts, timesqsort)
plt.title("Benchmarking for base BWT and AJ's numpy BWT, 50 runs each")
plt.legend(['base bwt', 'aj bwt', 'radix bwt', 'qsort bwt'])
plt.savefig("benchmarkgraphsall2.png")
# plt.show()