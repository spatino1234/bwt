#include "bwt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *readGenome(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("\nERROR: File %s not found.\nPlease enter the full filename including the \".txt\"\n\n", fileName);
        exit(0);
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

// Structure to store the original string and its rotations
typedef struct {
    char* str;
    int index;
} Rotation;

// Comparison function for sorting rotations
int compare_rotations(const void* a, const void* b) {
    return strcmp(((Rotation*)a)->str, ((Rotation*)b)->str);
}

void transform(char* text, int len) {
    Rotation* rotations = (Rotation*)malloc(len * sizeof(Rotation));
    if (rotations == NULL) {
        perror("Malloc failed during transform");
        exit(1);
    }

    // Create rotations and store them in the Rotation array
    for (int i = 0; i < len; i++) {
        rotations[i].index = i;
        rotations[i].str = (char*)malloc(len + 1);
        if (rotations[i].str == NULL) {
            perror("Memory allocation failed");
            exit(1);
        }
        strcpy(rotations[i].str, &text[i]);
        strncat(rotations[i].str, text, i);
    }

    // Sort the rotations
    qsort(rotations, len, sizeof(Rotation), compare_rotations);

    // Extract the last character of sorted rotations
    for (int i = 0; i < len; i++) {
        text[i] = rotations[i].str[len - 1];
        free(rotations[i].str);
    }

    free(rotations);
}

Node *newNode(int val, Node *next) {
    Node *r = malloc(sizeof(Node));
    if (r == NULL) {
        printf("Error: Malloc failed in newNode\n");
    }
    r->next = next;
    r->value = val;
    return r;
}

void displayList(Node *list) {
    Node *cur = list;
    while (cur != NULL) {
        printf("%d, ",cur->value+1);
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
        value = 0;
        break;
    case 'C':
        value = 1;
        break;
    case 'T':
        value = 2;
        break;
    case 'G':
        value = 3;
        break;
    case '$':
        value = 4; 
        break;
    }
    return value;
}

char decodeCharacter (int encoding) {
    char character;
    switch (encoding)
    {
    case 0:
        character = 'A';
        break;
    case 1:
        character = 'C';
        break;
    case 2:
        character = 'T';
        break;
    case 3:
        character = 'G';
        break;
    case 4:
        character = '$';
        break;
    }
    return character;
}

void constructIndices(BurrowsWheeler *BW, char *transform) {
    
    // create arrays for the last column
    int transform_length = strlen(transform);
    BW->length = transform_length;

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
        if ((nextchar != 'A') && (nextchar != 'C') && (nextchar != 'T') && (nextchar != 'G') && (nextchar != '$')) {
            printf("\nERROR: Invalid character in genome. Genome can only consist of As, Cs, Ts, and Gs\n\n");
            exit(0);
        }
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
    int i, len, temp;  
    len = strlen(str1);
        
    for (i = 0; i < len/2; i++)  
    {  
        temp = str1[i];  
        str1[i] = str1[len - i - 1];  
        str1[len - i - 1] = temp;  
    }  
}  

void reverse(BurrowsWheeler *BW, char* reversal) {
    
    intpair cur = BW->last_column[BW->last_index[0][4]];

    for (int i = 0; i < BW->length-1; i++) {
        cur = BW->last_column[BW->last_first_index[BW->last_index[cur.inputint][cur.inputchar]]];
        
        char character = decodeCharacter(cur.inputchar);
        strncat(reversal, &character, 1);
    }
    revstr(reversal);

    return;
}

int query(BurrowsWheeler *BW, char *query, int querrying_fastq, int output_offsets) {    
    int top = 0;
    int bottom = (BW->length)-1;
    int query_len = strlen(query);

    for (int i=0;i<(query_len); i++) {
        char cur_query_char = query[query_len-1-i];

        if ((cur_query_char != 'A') && (cur_query_char != 'C') && (cur_query_char != 'T') && (cur_query_char != 'G')) {
            printf("\nERROR: \"%s\" not a valid query. Query must be a string of As, Cs, Ts, and Gs\t\te.g. \"CATTAC\", \"GATATA\", \"AGCTTAC\", etc.\n\n", query);
            return 0;
        }

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
                printf("Query Doesn't Match Anywhere in Genome\n\n");
                return 0;
            }
        }
        top = BW->last_first_index[top];
        bottom = BW->last_first_index[bottom];
    }

    int num_matches = bottom-top+1;

    if (BW->length < 150) {
        char reversal[BW->length];
        reverse(BW, reversal);
        reversal[BW->length-1] = '\0';
        
        char query_format[BW->length];
        char query_format2[BW->length];

        for (int i=0;i<BW->length;i++) {
            *(query_format+i) = ' ';
            *(query_format2+i) = ' ';
        }

        for (int i=top;i<bottom+1;i++) {
            int match = BW->first_original_mapping[i];

            strncpy(&query_format[match], query, query_len);
            for (int j = 0;j<query_len;j++) {
                strncpy(&query_format2[match+j], "|", 1);
            }
        }

        query_format[BW->length-1] = '\0';
        query_format2[BW->length-1] = '\0';

        if (querrying_fastq) {
            printf("%s\n", reversal);
        }
        else {
            printf("\n\n%s\n", reversal);
        }
        printf("%s\n", query_format2);
        printf("%s\n\n", query_format);
    }

    if (output_offsets) {
        Node *matchList = NULL;
        for (int i=top;i<bottom+1;i++) {
            int match = BW->first_original_mapping[i];
            matchList = newNode(match, matchList);
        }
        printf("NUMBER OF MATCHES: %d\n", num_matches);
        printf("LOCATIONS/OFFSETS:\n");
        displayList(matchList);
        freeList(matchList);
    }
    else {
        printf("NUMBER OF MATCHES: %d\n\n", num_matches);
    }
    
    return num_matches;
}

void queryFASTQ(BurrowsWheeler *BW, char *fileName) {
    int total_matches = 0;
    FILE *fastq = fopen(fileName, "r");
    if (fastq == NULL) {
        printf("\nError: Unable to open fastq file.\n");
    }
    char buffer[500];
    while (fgets(buffer, 500, fastq)){
        if (buffer[0] == '@') {
            fgets(buffer, 500, fastq);
            int read_len = strlen(buffer);
            char read[read_len];
            strncpy(read, buffer, read_len-1);
            // read[read_len] = '\0';
            read[read_len-1] = '\0';

            printf("\e[1mQuery:\e[m %s\n", read);
            total_matches = total_matches + query(BW, read, 1, 0);
        }
    }
    fclose(fastq);
    printf("\nTOTAL MATCHES: %d\n\n\n", total_matches);
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


/* wraps array index within array bounds (assumes value < 2 * limit) */
#define Wrap(value, limit)      (((value) < (limit)) ? (value) : ((value) - (limit)))

int *block;     /* block being (un)transformed */
size_t blockSize;                    /* actual size of block */

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

void genBWTString(int length, char *bwt_string) {
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

int main(){
    char *genome = "GATATA$";

    char transform[strlen(genome)];
    BWTRadix(genome, transform);
    printf("%s\n",transform);

    return 0;
}




// int main()  {
//     // Getting user input
//     char genomeFile[100];
//     printf("\nEnter the name of the .txt file with your reference genome: ");
//     scanf("%100s",genomeFile);
    
//     // Reading genome from txt file input by user
//     char *genome = readGenome(genomeFile);
//     int genome_length = strlen(genome);
//     if (genome_length > 100001) {
//         printf("\nERROR: %s is too long. It can be at most 100,000 characters\n\n", genomeFile);
//         exit(0);
//     }
    
//     // Encoding genome using BWT
//     transform(genome, genome_length);
//     BurrowsWheeler *BW = initBW(genome_length+1);
//     constructIndices(BW, genome);

//     // 
//     printf("\n");
//     while (1) {
//         char query1[300];
//         printf("\n\e[4mEnter a Query\e[m \nTo exit, \t  enter the query \"exit\"\nFor more options, enter the query \"help\"\n\n\e[1mQuery\e[m: ");
//         scanf("%300s", query1);
//         if (strncmp(query1, "exit", 4) == 0) {
//             printf("\n");
//             exit(0);
//         }
//         else if (strncmp(query1, "fastq", 5) == 0) {
//             char fastqFile[100];
//             printf("\tEnter .fastq file name: ");
//             scanf("%100s",fastqFile);
//             printf("\n\n");
//             queryFASTQ(BW, fastqFile);
//             fflush(stdin);
//             continue;
//         }
//         else if (strncmp(query1, "match", 5) == 0) {
//             char query2[300];
//             printf("\e[1mActual Query:\e[m ");
//             scanf("%300s",query2);

//             if (strlen(query2) < 251) {
//                 query(BW, query2, 0, 1);
//             } else {
//                 printf("\nERROR: Query is too long. It must be at most 250 characters\n\n");
//                 fflush(stdin);
//             }
//         }
//         else if (strncmp(query1, "help", 5) == 0) {
//             printf("\n * To read queries from a .fastq file, enter the query \"fastq\"  *\n");
//             printf(" * To output the locations of matches, enter the query \"match\"  *\n");
//         }
//         else {
//             if (strlen(query1) < 251) {
//                 query(BW, query1, 0, 0);
//                 fflush(stdin);
//             } else {
//                 printf("\nERROR: Query is too long. It must be at most 250 characters\n\n");
//                 fflush(stdin);
//             }
//         }
//         printf("\n");
//     }

//     freeBW(BW);
//     return 0;
// }