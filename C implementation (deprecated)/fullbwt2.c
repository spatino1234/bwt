#include "fullbwt2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


/* wraps array index within array bounds (assumes value < 2 * limit) */
#define Wrap(value, limit)      (((value) < (limit)) ? (value) : ((value) - (limit)))

int *block;     /* block being (un)transformed */
size_t blockSize;                    /* actual size of block */

Node *newNode(int val, Node *next) {
    Node *r = malloc(sizeof(Node));
    if (r == NULL) {
        printf("Error: Malloc failed in newRead\n");
    }
    r->next = next;
    r->value = val;
    return r;
}

static int ComparePresorted(const void *s1, const void *s2)
{
    int offset1, offset2;
    int i;

    /***********************************************************************
    * Compare 1 character at a time until there's difference or the end of
    * the block is reached.  Since we're only sorting strings that already
    * match at the first two characters, start with the third character.
    ***********************************************************************/
    offset1 = *((int *)s1) + 2;
    offset2 = *((int *)s2) + 2;

    for(i = 2; i < blockSize; i++)
    {
        int c1, c2;

        /* ensure that offsets are properly bounded */
        if (offset1 >= blockSize)
        {
            offset1 -= blockSize;
        }

        if (offset2 >= blockSize)
        {
            offset2 -= blockSize;
        }

        c1 = block[offset1];
        c2 = block[offset2];

        if (c1 > c2)
        {
            return 1;
        }
        else if (c2 > c1)
        {
            return -1;
        }

        /* strings match to here, try next character */
        offset1++;
        offset2++;
    }

    /* strings are identical */
    return 0;
}

static int ComparePresortedFull(const void *s1, const void *s2)
{
    int offset1, offset2;
    int i;

    /***********************************************************************
    * Compare 1 character at a time until there's difference or the end of
    * the block is reached.  This time start with the first character.
    ***********************************************************************/
    offset1 = *((int *)s1) + 2;
    offset2 = *((int *)s2) + 2;

    for(i = 0; i < blockSize; i++)
    {
        int c1, c2;

        /* ensure that offsets are properly bounded */
        if (offset1 >= blockSize)
        {
            offset1 -= blockSize;
        }

        if (offset2 >= blockSize)
        {
            offset2 -= blockSize;
        }

        c1 = block[offset1];
        c2 = block[offset2];

        if (c1 > c2)
        {
            return 1;
        }
        else if (c2 > c1)
        {
            return -1;
        }

        /* strings match to here, try next character */
        offset1++;
        offset2++;
    }

    /* strings are identical */
    return 0;
}

void qsort_bwt(int *last)
{
    int *rotationIdx;
    rotationIdx = (int *)malloc(blockSize * sizeof(int));
    for (int i = 0; i < blockSize; i++) {
        rotationIdx[i] = i;
    }
    int s0Idx;
    // printf("test1\n");
    qsort(&rotationIdx[0], blockSize, sizeof(int),
                    ComparePresortedFull);
        s0Idx = 0;
    for (int i = 0; i < blockSize; i++)
    {
        if (rotationIdx[i] != 0)
        {
            last[i] = block[rotationIdx[i] - 1];
        }
        else
        {
            /* unrotated string 1st character is end of string */
            s0Idx = i;
            last[i] = block[blockSize - 1];
        }
    }
    free(rotationIdx);
    return;
}

void radix_bwt(int *last)
{
    int i, j, k;
    int *rotationIdx;      /* index of first char in rotation */
    int *v;                /* index of radix sorted charaters */
    int s0Idx;                      /* index of S0 in rotations (I) */
    // int *last;            /* last characters from sorted rotations */

    /* counters and offsets used for radix sorting with characters */
    unsigned int counters[5] = {0};
    unsigned int offsetTable[5] = {0};

    /***********************************************************************
    * BLOCK_SIZE arrays are allocated on the heap, because gcc generates
    * code that throws a Segmentation fault when the large arrays are
    * allocated on the stack.
    ***********************************************************************/
    rotationIdx = (int *)malloc(blockSize * sizeof(int));

    v = (int *)malloc(blockSize * sizeof(int));

    /*******************************************************************
    * Sort the rotated strings in the block.  A radix sort is performed
    * on the first to characters of all the rotated strings (2nd
    * character then 1st).  All rotated strings with matching initial
    * characters are then quicksorted. - Q4..Q7
    *******************************************************************/

    /*** radix sort on second character in rotation ***/

    /* count number of characters for radix sort */
    for (i = 0; i < blockSize; i++)
    {
        counters[block[i]]++;
    }

    offsetTable[0] = 0;

    for(i = 1; i < 5; i++)
    {
        /* determine number of values before those sorted under i */
        offsetTable[i] = offsetTable[i - 1] + counters[i - 1];
    }

    /* sort on 2nd character */
    for (i = 0; i < blockSize - 1; i++)
    {
        j = block[i + 1];
        v[offsetTable[j]] = i;
        offsetTable[j] = offsetTable[j] + 1;
    }

    /* handle wrap around for string starting at end of block */
    j = block[0];
    v[offsetTable[j]] = i;
    offsetTable[0] = 0;

    /*** radix sort on first character in rotation ***/

    for(i = 1; i < 5; i++)
    {
        /* determine number of values before those sorted under i */
        offsetTable[i] = offsetTable[i - 1] + counters[i - 1];
    }

    for (i = 0; i < blockSize; i++)
    {
        j = v[i];
        j = block[j];
        rotationIdx[offsetTable[j]] = v[i];
        offsetTable[j] = offsetTable[j] + 1;
    }

    /*******************************************************************
    * now rotationIdx contains the sort order of all strings sorted
    * by their first 2 characters.  Use qsort to sort the strings
    * that have their first two characters matching.
    *******************************************************************/
    for (i = 0, k = 0; (i < 5 && k < (blockSize - 1)); i++)
    {
        for (j = 0; (j < 5 && k < (blockSize - 1)); j++)
        {
            unsigned int first = k;

            /* count strings starting with ij */
            while ((i == block[rotationIdx[k]]) &&
                (j == block[Wrap(rotationIdx[k] + 1,  blockSize)]))
            {
                k++;

                if (k == blockSize)
                {
                    /* we've searched the whole block */
                    break;
                }
            }

            if (k - first > 1)
            {
                /* there are at least 2 strings staring with ij, sort them */
                qsort(&rotationIdx[first], k - first, sizeof(int),
                    ComparePresorted);
            }
        }
    }

    /* find last characters of rotations (L) - C2 */
    s0Idx = 0;
    for (i = 0; i < blockSize; i++)
    {
        if (rotationIdx[i] != 0)
        {
            last[i] = block[rotationIdx[i] - 1];
        }
        else
        {
            /* unrotated string 1st character is end of string */
            s0Idx = i;
            last[i] = block[blockSize - 1];
        }
    }

    /* clean up */
    free(rotationIdx);
    free(v);
    return;
}

void displayList(Node *list) {
    Node *cur = list;
    while (cur != NULL) {
        printf("%d, ",cur->value);
        cur = cur->next;
    }
    printf("\n\n");
}

void freeList(Node* first) {
    Node *cur = first;
    while (cur != NULL) {
        Node* next = cur->next;
        free(cur);
        cur = next;
    }
}

int alphabeticalComp(const void *a, const void *b) { 
       return *((char *)a) - *((char *)b); 
}

int encodeCharacter(char character) {
    int value = 0;
    switch (character)
    {
    case 'A':
        value = 1;
        break;
    case 'C':
        value = 2;
        break;
    case 'G':
        value = 3;
        break;
    case 'T':
        value = 4;
        break;
    case '$':
        value = 0; 
        break;
    }
    return value;
}

char decodeCharacter (int encoding) {
    char character;
    switch (encoding)
    {
    case 1:
        character = 'A';
        break;
    case 2:
        character = 'C';
        break;
    case 3:
        character = 'G';
        break;
    case 4:
        character = 'T';
        break;
    case 0:
        character = '$';
        break;
    }
    return character;
}

void encodeString(char *input) {
    int input_len = strlen(input);
    int encodedCharacter;
    for (int i = 0; i < input_len; i++) {
        encodedCharacter = encodeCharacter(input[i]);
        block[i] = encodedCharacter;
    }
    return;
}

void decodeList(char laststr[], int *input) {
    char decoded_string[blockSize];
    strcpy (decoded_string,"");
    int cur;
    char character;
    for (int i = 0; i < blockSize; i++) {
        cur = input[i];
        character = decodeCharacter(cur);
        strncat(decoded_string, &character, 1);
    }
    strcpy(laststr, decoded_string);
    return;
}

void constructIndices(BurrowsWheeler *BW, char *transform) {
    
    // crete arrays for the last column
    int transform_length = strlen(transform);
    BW->length = transform_length;
    // intpair last_column[transform_length];
    // intpair first_column[transform_length];

    char sorted_transform[transform_length];
    memcpy(sorted_transform, transform, strlen(transform)+1);
    qsort(sorted_transform, strlen(transform), sizeof(char), alphabeticalComp);

    int last_index[transform_length][5], first_index[transform_length][5];
    // initialize each element of both counters to 0
    int first_encounters[5] = {0};

    int nextchar;
    int nextcharEncoded;
    intpair firstintpair;
    for (int i=0;i<transform_length;i++) {
        nextchar = sorted_transform[i];
        nextcharEncoded = encodeCharacter(nextchar);
        first_encounters[nextcharEncoded]++;

        firstintpair.inputchar = nextcharEncoded;
        firstintpair.inputint = first_encounters[nextcharEncoded]-1;

        BW->first_column[i] = firstintpair;
        BW->first_index[firstintpair.inputint][firstintpair.inputchar] = i;
    }

    int firstloc;
    intpair lastintpair;

    int last_encounters[5] = {0};

    for (int i=0;i<transform_length;i++) {
        nextchar = transform[i];
        nextcharEncoded = encodeCharacter(nextchar);
        last_encounters[nextcharEncoded]++;

        lastintpair.inputchar = nextcharEncoded;
        lastintpair.inputint = last_encounters[nextcharEncoded]-1;

        BW->last_column[i] = lastintpair;
        BW->last_index[lastintpair.inputint][lastintpair.inputchar] = i;

        
        firstloc = BW->first_index[lastintpair.inputint][lastintpair.inputchar];
        BW->last_first_index[i] = firstloc;
    }

    BW->first_original_mapping[BW->first_index[0][4]] = transform_length-1;

    intpair cur = BW->last_column[BW->last_first_index[BW->last_index[0][4]]];
    BW->first_original_mapping[BW->first_index[cur.inputint][cur.inputchar]] = transform_length-2;

    for (int i=0;i<transform_length-2;i++) {
        cur = BW->last_column[BW->last_first_index[BW->last_index[cur.inputint][cur.inputchar]]];
        BW->first_original_mapping[BW->first_index[cur.inputint][cur.inputchar]] = transform_length-3-i;
    }
}

void revstr(char *str1)  
{  
    // declare variable  
    int i, len, temp;  
    len = strlen(str1); // use strlen() to get the length of str string  
      
    // use for loop to iterate the string   
    for (i = 0; i < len/2; i++)  
    {  
        // temp variable use to temporary hold the string  
        temp = str1[i];  
        str1[i] = str1[len - i - 1];  
        str1[len - i - 1] = temp;  
    }  
}  

char *reverse(BurrowsWheeler *BW) {
    char reversal[BW->length];
    
    intpair cur = BW->last_column[BW->last_index[0][4]];

    for (int i = 0; i < BW->length-1; i++) {
        cur = BW->last_column[BW->last_first_index[BW->last_index[cur.inputint][cur.inputchar]]];
        
        char character = decodeCharacter(cur.inputchar);
        strncat(reversal, &character, 1);
    }
    revstr(reversal);

    return reversal;
}

void query(BurrowsWheeler *BW, char *query) {
    printf("Query: %s\n", query);
    printf("Matches: \n");

    int top = 0;
    int bottom = (BW->length)-1;
    int query_len = strlen(query);

    for (int i=0;i<(query_len); i++) {
        char cur_query_char = query[query_len-1-i];
        for (int j = 0; j<(BW->length); j++) {
            bool top_set = false;

            if (BW->last_column[top].inputchar != encodeCharacter(cur_query_char)) {
                top++;
            }
            else {
                top_set = true;
            }
            if (BW->last_column[bottom].inputchar != encodeCharacter(cur_query_char)) {
                bottom--;
            }
            else {
                if (top_set == true) {
                    break;
                }
            }
            if (top > bottom) {
                printf("Query Doesn't Match Anywhere\n\n");
                return;
            }
        }
        top = BW->last_first_index[top];
        bottom = BW->last_first_index[bottom];
    }

    Node *matchList = NULL;
    for (int i=top;i<bottom+1;i++) {
        int match = BW->first_original_mapping[i];
        matchList = newNode(match, matchList);
    }

    displayList(matchList);
    freeList(matchList);
}

char *readGenome(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error: Unable to open fastq file.\n");
    }
    
    fseek(file, 0, SEEK_END);
    long fileLength = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *contents = (char *)malloc(fileLength + 2);
    if (contents == NULL) {
        fclose(file);
        printf("Error: Unable to allocate memory.\n");
        return NULL;
    }
    size_t bytesRead = fread(contents, 1, fileLength, file);
    if (bytesRead != fileLength) {
        fclose(file);
        free(contents);
        perror("Error reading the file");
        return NULL;
    }

    contents[fileLength] = '$';
    contents[fileLength+1] = '\0';
    fclose(file);
    return contents;
}

void queryFASTQ(BurrowsWheeler *BW, char *fileName) {
    FILE *fastq = fopen(fileName, "r");
    if (fastq == NULL) {
        printf("Error: Unable to open fastq file.\n");
    }
    char buffer[500];
    while (fgets(buffer, 500, fastq)){
        if (buffer[0] == '@') {
            fgets(buffer, 500, fastq);
            int read_len = strlen(buffer);
            char read[read_len];
            strncpy(read, buffer, read_len-1);
            read[read_len] = '\0';

            query(BW, read);
        }
    }
    fclose(fastq);
}

BurrowsWheeler *initBW(int malloc_length) {
    BurrowsWheeler *returnstruct = malloc(sizeof(BurrowsWheeler));
    returnstruct->malloc_length = malloc_length;

    returnstruct->last_index = (int **)malloc(sizeof(int*)*malloc_length);
    returnstruct->first_index = (int **)malloc(sizeof(int*)*malloc_length);
    
    for (int i = 0; i < malloc_length; i++) {
        returnstruct->last_index[i] = malloc(sizeof(int) * malloc_length);
        returnstruct->first_index[i] = malloc(sizeof(int) * malloc_length);
    }
    
    returnstruct->last_first_index = malloc(sizeof(int) * malloc_length);
    returnstruct->first_original_mapping = malloc(sizeof(int) * malloc_length);
    return returnstruct;
}

void freeBW(BurrowsWheeler *BW) {
    free(BW->first_original_mapping);
    free(BW->last_first_index);
    
    for (int i=0;i<(BW->malloc_length);i++) {
        free(BW->last_index[i]);
        free(BW->first_index[i]);
    }
    free(BW->last_index);
    free(BW->first_index);

    free(BW);
}

double evalQsortTime(char *blockstr) {
    clock_t start_time = clock();
    blockSize = strlen(blockstr);
    block = (int*) malloc(sizeof(int)*blockSize);
    encodeString(blockstr);
    int *last;
    last = (int *)malloc(blockSize * sizeof(int));
    qsort_bwt(last);

    char laststr[blockSize];
    decodeList(laststr, last);
    free(last);
    double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    return elapsed_time;
}

double evalRadixTime(char* blockstr) {
    clock_t start_time;
    start_time = clock();
    blockSize = strlen(blockstr);
    block = (int*) malloc(sizeof(int)*blockSize);
    encodeString(blockstr);
    int *last;
    last = (int *)malloc(blockSize * sizeof(int));
    radix_bwt(last);

    char laststr[blockSize];
    decodeList(laststr, last);
    free(last);
    free(block);
    double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    return elapsed_time;
}

void BWTRadix(char *blockstr, char *transformed_string) {
    blockSize = strlen(blockstr);
    block = (int*) malloc(sizeof(int)*blockSize);
    encodeString(blockstr);
    int *last;
    last = (int *)malloc(blockSize * sizeof(int));
    radix_bwt(last);
    decodeList(transformed_string, last);
    free(last);
    free(block);
    return;
}

void BWTqsort(char *blockstr, char *transformed_string) {
    blockSize = strlen(blockstr);
    block = (int*) malloc(sizeof(int)*blockSize);
    encodeString(blockstr);
    int *last;
    last = (int *)malloc(blockSize * sizeof(int));
    qsort_bwt(last);
    decodeList(transformed_string, last);
    free(last);
    free(block);
    return;
}

void genBWTString(int length, char *bwt_string) {
    srand(time(0));
    int i;
    int bwt_intstring[length];
    for (i = 0; i < length; i++) { 
        int num = rand() % 5; 
        bwt_intstring[i] = num;
    }
    blockSize = length;
    decodeList(bwt_string, bwt_intstring);
    return;
}

void getRunTimes(bool isRadix) {
    int counts[13] = {100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000};
    double time_sum;
    double cur_best = 0;
    if (isRadix) {
        printf("Radix times\n");
    }
    else {
        printf("Full qsort times\n");
    }
    printf("[");
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 20; j++) {
            time_sum = 0;
            for (int k = 0; k < 50; k++) {
                char sort_str[counts[i]];
                genBWTString(counts[i], sort_str);
                if (isRadix) {
                    time_sum += evalRadixTime(sort_str);
                }
                else {
                    time_sum += evalQsortTime(sort_str);
                }
            }
            if (cur_best == 0) {
                cur_best = time_sum;
            }
            cur_best = MIN(cur_best, time_sum);
        }
        // printf("run %i: %f seconds\n", counts[i], cur_best);
        printf("%f, ", cur_best);
        cur_best = 0;
    }
    printf("]\n");
}

int main(){
    // char *blockstr = "GATATA$";
    // blockSize = strlen(blockstr);
    // block = (int*) malloc(sizeof(int)*blockSize);
    // encodeString(blockstr);
    // int *last;
    // last = (int *)malloc(blockSize * sizeof(int));
    // radix_bwt(last);

    // char laststr[blockSize];
    // decodeList(laststr, last);
    // printf("%s\n", laststr);
    // free(last);
    char *genome = "GATATA$";

    char transform[strlen(genome)];
    BWTRadix(genome, transform);
    printf("%s\n",transform);
}

// int main(){
//     char *genome = readGenome("genome2.txt");

//     char transform[strlen(genome)];
//     BWTqsort(genome, transform);
//     printf("%s\n",transform);
    
//     BurrowsWheeler *BW = initBW(100001);
//     constructIndices(BW, transform);

    
//     char *fastqFile = "readsshort.fastq";
//     queryFASTQ(BW, fastqFile);


//     freeBW(BW);
//     return 0;
// }