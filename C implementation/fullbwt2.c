#include "fullbwt2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/* wraps array index within array bounds (assumes value < 2 * limit) */
#define Wrap(value, limit)      (((value) < (limit)) ? (value) : ((value) - (limit)))

int *block;     /* block being (un)transformed */
size_t blockSize;                    /* actual size of block */

Node *addtoList(int val, Node *list) {
    Node *newNode;
    newNode->value = val;
    newNode->next = list;
    return newNode;
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
        printf("comparing\n");
        printf("%i\n", block[offset1]);
        printf("%i\n", block[offset2]);
        printf("%i\n", offset1);
        printf("%i\n", offset2);

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


int *bwt()
{
    int i, j, k;
    int *rotationIdx;      /* index of first char in rotation */
    int *v;                /* index of radix sorted charaters */
    int s0Idx;                      /* index of S0 in rotations (I) */
    int *last;            /* last characters from sorted rotations */

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
    last = (int *)malloc(blockSize * sizeof(int));
    printf("test1\n");

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
        printf("test1iter\n");
        printf("%i\n", block[0]);
        counters[block[i]]++;
    }
    printf("test1a\n");

    offsetTable[0] = 0;

    for(i = 1; i < 5; i++)
    {
        /* determine number of values before those sorted under i */
        offsetTable[i] = offsetTable[i - 1] + counters[i - 1];
    }
    printf("test1b\n");

    /* sort on 2nd character */
    for (i = 0; i < blockSize - 1; i++)
    {
        j = block[i + 1];
        v[offsetTable[j]] = i;
        offsetTable[j] = offsetTable[j] + 1;
    }
    printf("test2\n");

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
    printf("test3\n");

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
    printf("test4\n");

    /* find last characters of rotations (L) - C2 */
    s0Idx = 0;
    printf("%i\n", block[0]);
    for (i = 0; i < blockSize; i++)
    {
        if (rotationIdx[i] != 0)
        {
            printf("test5a\n");
            // printf("%i\n", rotationIdx[i]);
            // printf("%i\n", block[0]);
            last[i] = block[rotationIdx[i] - 1];
        }
        else
        {
            printf("test5b\n");
            /* unrotated string 1st character is end of string */
            s0Idx = i;
            last[i] = block[blockSize - 1];
        }
        printf("%i\n", last[i]);
    }
    printf("test6\n");

    /* clean up */
    free(rotationIdx);
    free(v);
    printf("test7\n");
    printf("first: %i\n", last[0]);
    printf("second: %i\n", last[1]);
    printf("third: %i\n", last[2]);
    printf("%li\n", sizeof(last)/sizeof(int));
    return last;
}

void displayList(Node *list) {
    Node *cur = list;
    while (cur != NULL) {
        printf("%d\n",cur->value);
        cur = cur->next;
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
    case 'T':
        value = 3;
        break;
    case 'G':
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
        character = 'T';
        break;
    case 4:
        character = 'G';
        break;
    case 0:
        character = '$';
        break;
    }
    return character;
}

int *encodeString(char *input) {
    int input_len = strlen(input);
    printf("%i\n", input_len);
    int *encoded_list = (int*)malloc(input_len * sizeof(int));
    printf("%li\n", sizeof(encoded_list)/sizeof(int));
    int encodedCharacter;
    for (int i = 0; i < input_len; i++) {
        encodedCharacter = encodeCharacter(input[i]);
        printf("digit: %d\n", encodedCharacter);
        encoded_list[i] = encodedCharacter;
    }
    printf("4th item: %i\n", encoded_list[3]);
    return encoded_list;
}

char *decodeList(int *input) {
    int input_len = sizeof(input)/sizeof(int);
    printf("length: %i\n", input_len);
    char *decoded_string = "";
    for (int i = 0; i < input_len; i++) {
        int cur = input[i];
        printf("%i", cur);
        char character = decodeCharacter(cur);
        strncat(decoded_string, &character, 1);
        printf("%c", character);
    }
    return decoded_string;
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

    for (int i = 0; i < BW->length; i++) {
        cur = BW->last_column[BW->last_first_index[BW->last_index[cur.inputint][cur.inputchar]]];
        
        char character = decodeCharacter(cur.inputchar);
        strncat(reversal, &character, 1);
    }
    revstr(reversal);

    return reversal;
}

void query(BurrowsWheeler *BW, char *query) {
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
                printf("Query doesn't match anywhere\n");
            }
        }
        top = BW->last_first_index[top];
        bottom = BW->last_first_index[bottom];
    }

    for (int i=top;i<bottom+1;i++) {
        printf("%d : %d \n", BW->first_original_mapping[i], BW->first_original_mapping[i]+query_len-1);
    }
}

BurrowsWheeler *initBW(int length) {
    BurrowsWheeler *returnstruct = malloc(sizeof(BurrowsWheeler));
    
    returnstruct->last_index = malloc(sizeof(int) * length);
    returnstruct->first_index = malloc(sizeof(int) * length);
    
    for (int i = 0; i < length; i++) {
        returnstruct->last_index[i] = malloc(sizeof(int) * length);
        returnstruct->first_index[i] = malloc(sizeof(int) * length);
    }
    
    returnstruct->last_first_index = malloc(sizeof(int) * length);
    returnstruct->first_original_mapping = malloc(sizeof(int) * length);
    return returnstruct;
}

int main(){
    char *transform = "ATTG$AA";
    char *blockstr = "GATATA$";
    blockSize = strlen(blockstr);
    block = encodeString(blockstr);
    printf("block size: %li\n", sizeof(block)/sizeof(int));
    int *last = bwt();
    char *laststr = decodeList(last);
    printf("%s\n", laststr);
    free(block);
    free(last);
    // BurrowsWheeler *BW = initBW(100);
    // constructIndices(BW, transform);
    // printf("String: %s\n", reverse(BW));
    // query(BW, "TA");

    return 0;
}