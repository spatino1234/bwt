class transform(object):
    def bwt(self, s: str):
        original = s  # Store the original string
        a = []
        temp = s[-1] + s[:-1]  # Initial rotation
        a.append(temp)

        while temp != original:  # Stop when temp equals the original string
            print(temp)
            s = temp  # Update s with the newly rotated string
            temp = s[-1] + s[:-1]  # Perform another rotation
            a.append(temp)
        print(f"heres my unsorted array {a} \n")
        a.sort()
        print(f"heres my sorted array {a} \n")

        # take the last item col of the char of the array
        # returns the bwtTransform
        bwtResult = ""
        for s in a:
            bwtResult += s[-1]
        print(f"here is the original string {original}")

        print(f"here is the bwt transform {bwtResult}")

        print(f"here is the bwt compression {self.bwtCompression(bwtResult)}")


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

    #! https://en.wikipedia.org/wiki/FM-index#:~:text=An%20FM%2Dindex%20is%20created,rows%20have%20been%20sorted%20lexicographically.
    def create_FM_index(self, bwt:str):
    # Initializing count dictionary
        # counts num of occurances for each letter before the letter's
        # first occurance in sorted BWT transform
        counts = {}

        # total occurances of letters up to a certain position in bwt
        occurances = {}
        
        # Initialize FM index with zeros for each character in the BWT string
        # to preallocate space for the FM index
        fm_index = []
        for i in bwt:
            fm_index.append({'$': 0, 'A': 0, 'C': 0, 'G': 0, 'T': 0})
            # fm_index = [{'$': 0, 'a': 1, 'b': 0, 'n': 0},  {'$': 0, 'a': 1, 'b': 0, 'n': 1} ... 
                        # {'$': 1, 'a': 3, 'b': 1, 'n': 2}]

        # Construct the rest of the FM index
        for i in range(len(bwt)):
            char = bwt[i]
            # update count by 1 if char exists in dictionary, else it's 0
            counts[char] = counts.get(char, 0) + 1
            fm_index[i] = counts.copy()
        
        # Calculate total occurrences of each character before each position
        # we use the sorted order of BWT to find how many instances
        # of each char are before first occurance of __ char

        # TLDR: we consider how char are orderd to build the occurance dictionary
        for char, count in sorted(counts.items()):
            occurances[char] = occurances.get(char, 0) + count

        return fm_index, occurances
    
    def bwtReversal(self, bwt: str) :
        print(True)

res = transform()
a = "actgg$"
res.bwt(a)

fm_index, totals = res.create_FM_index(a)

print("FM Index:")
for dict in fm_index:
    print(dict)

print(f"\nTotals: {totals}")
