#ifndef HEADERFILE_H
  #define HEADERFILE_H
  #define LENGTH 100

typedef struct {
  int inputchar;
  int inputint;
} intpair;

typedef struct {
    intpair *first_column;     // Assuming a maximum length of 100 for columns
    intpair *last_column;
    int** last_index;
    int** first_index;
    int *last_first_index;
    int *first_original_mapping;
} BurrowsWheeler;

#endif