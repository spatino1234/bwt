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

def main():
    text = "BANANA"
    query = "NA"
    print(naive(query,text))

if __name__ == "__main__":
    main()