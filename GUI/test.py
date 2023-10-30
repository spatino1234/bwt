# The test file where A.J. has all of his mental breakdowns


def bwt_quick(input_string):
    input_string += '$'
    indices = list(range(len(input_string)))
    sorted_indices = sorted(indices, key=lambda i: input_string[i:] + input_string[:i])
    bwt_encoded = ''.join(input_string[i - 1] for i in sorted_indices)
    return(bwt_encoded)

def bwt_decode(input):
    input_bwt = bwt_quick(input)

    display_keyword = list(input_bwt)
    print(display_keyword)

    display_keyword.sort()

    x1 = -100
    y1 = 100
    for i in range(0,len(display_keyword)):
        print(display_keyword[i])
        # y1 -= 20


def main():
    input_string = "BANANA"
    bwt_decode(input_string)


    

if __name__ == '__main__':
    main()
