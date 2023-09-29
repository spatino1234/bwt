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
        print(f"here is the original string {s}")

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


res = transform()
a = "tcaagtgcatgactgatgcatgactgactgacacgtatcgcatgacgtactgactgactgatcgactgcatgactgactgcatgtccca$"
res.bwt(a)
