#ifndef HEADERFILE_H
  #define HEADERFILE_H
  #define LENGTH 100001

typedef struct {
  int inputchar;
  int inputint;
} intpair;

typedef struct {
    int length;
    int malloc_length;
    intpair first_column[LENGTH];     // Assuming a maximum length of 100 for columns
    intpair last_column[LENGTH];
    int** last_index;
    int** first_index;
    int *last_first_index;
    int *first_original_mapping;
} BurrowsWheeler;

typedef struct Node {
  struct Node *next;
  int value;
} Node;

#endif