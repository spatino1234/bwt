#ifndef HEADERFILE_H
  #define HEADERFILE_H
  #define LENGTH 100


typedef struct {
    char first_column[LENGTH];     // Assuming a maximum length of 100 for columns
    char last_column[LENGTH];
    int last_index[LENGTH];
    int first_index[LENGTH];
    int last_first_index[LENGTH];
    int first_original_mapping[LENGTH];
} BurrowsWheeler;

typedef struct {
    int A;
    int C;
    int T;
    int G;
    int $;
  } encounter_dict;



typedef struct {
    int lastColCharLocation;
    int firstColCharLocation;
    UT_hash_handle hh;
  } last_first_dict;
// struct dictionary {
//   char *key;
//   int value;
// };

// struct dictionary *dictionary_new() {
//   struct dictionary *dictionary = malloc(sizeof(struct dictionary));
//   dictionary->key;
//   dictionary->value = 0;
//   return dictionary;
// }

// void dictionary_add(struct dictionary *dictionary, char *key, int value) {
//   dictionary->key = key;
//   dictionary->value = value;
// }

// int dictionary_get(struct dictionary *dictionary, char *key) {
//   return dictionary->value;
// }

// void dictionary_free(struct dictionary *dictionary) {
//   free(dictionary);
// } 
#endif