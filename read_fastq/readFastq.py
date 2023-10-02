# Returns list of reads as strings
def readFastq(inputFile):
    reads = []
    with open(inputFile, 'r') as fastq:
        for line in fastq:
            if (line[0] in ['A','C','G','T'] and
                line[1] in ['A','C','G','T'] and
                line[2] in ['A','C','G','T'] and
                line[3] in ['A','C','G','T'] and
                line[4] in ['A','C','G','T']):
                reads.append(line.rstrip())
    return reads

def main():
    fastq_file = 'reads.fastq'
    print(readFastq(fastq_file))

if __name__ == '__main__':
    main()