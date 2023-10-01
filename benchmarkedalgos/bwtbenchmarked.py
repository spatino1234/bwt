import timeit
import random
import matplotlib.pyplot as plt

class transform(object):
    def bwt(self, s: str):
        starttime = timeit.default_timer()
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
        return(timeit.default_timer() - starttime)


    def bwtCompression(self, finalString: str):
        count = 1
        prev_char = finalString[0]
        result = [] # store char and counts

        for char in finalString[1:]:
            # If current character matches previous character, increase the count
            if char == prev_char:
                count += 1
            else: #If a new character appears, append the previous character and its count to the result list
                result.append(str(count))
                result.append(prev_char)
                count = 1
                prev_char = char
        # #Append the last character and its count to the result list
        result.append(str(count))
        result.append(prev_char)

        return ("".join(result))
    
def DNA(length):
    return ''.join(random.choice('CGTA') for _ in range(length))

res = transform()

starttime = timeit.default_timer()
times = []
counts = [100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000]
for count in counts:
    a = DNA(count)
    time = res.bwt(a)
    print(time, count)
    times.append(time)
plt.xscale("log")
plt.yscale("log")
plt.xlabel("DNA Length")
plt.ylabel("Time to complete")
plt.plot(counts, times)
plt.show()