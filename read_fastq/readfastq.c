#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "readfastq.h"

void displayReads(Read *reads){
    Read *cur = reads;
    int count = 1;
    while (cur != NULL) {
        printf("Read %i: %s", count, cur->read);
        cur = cur->next;
        count++;
    }
}

Read *newRead(char *read, Read *next) {
    Read *r = malloc(sizeof(Read));
    if (r == NULL) {
        printf("Error: Malloc failed in newRead\n");
    }
    r->next = next;
    r->read = read;
    return r;
}

void freeReads(Read* first) {
    Read *cur = first;
    while (cur != NULL) {
        Read* next = cur->next;
        if (cur->read != NULL) {
            free(cur->read);
        }
        free(cur);
        cur = next;
    }
}

Read *readFASTQ(char *fileName) {
    FILE *fastq = fopen(fileName, "r");
    if (fastq == NULL) {
        printf("Error: Unable to open fastq file.\n");
    }
    char buffer[500];
    Read *cur;
    Read *prev;
    while (fgets(buffer, 500, fastq)){
        //Checks first 5 characters to determine whether text represents DNA
        if (((buffer[0] == 'A') || (buffer[0] == 'C') || (buffer[0] == 'T') || (buffer[0] == 'G')) &&
            ((buffer[1] == 'A') || (buffer[1] == 'C') || (buffer[1] == 'T') || (buffer[1] == 'G')) && 
            ((buffer[2] == 'A') || (buffer[2] == 'C') || (buffer[2] == 'T') || (buffer[2] == 'G')) && 
            ((buffer[3] == 'A') || (buffer[3] == 'C') || (buffer[3] == 'T') || (buffer[3] == 'G')) && 
            ((buffer[4] == 'A') || (buffer[4] == 'C') || (buffer[4] == 'T') || (buffer[4] == 'G'))) {
                char *mallocd_buffer = malloc(sizeof(char)*500);
                strcpy(mallocd_buffer, buffer);
                cur = newRead(mallocd_buffer, prev);
                prev = cur;
        }
    }
    fclose(fastq);
    return cur;
}

int main() {
    char *fastqFile = "reads.fastq";
    Read* reads = readFASTQ(fastqFile);

    displayReads(reads);
    
    freeReads(reads);
    return 0;
}