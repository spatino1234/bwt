#include <stdio.h>
#include <string.h>

// Structure to represent the Burrows-Wheeler class
typedef struct {
    char first_column[100];     // Assuming a maximum length of 100 for columns
    char last_column[100];
    int last_index[100];
    int first_index[100];
    int last_first_index[100];
    int first_original_mapping[100];
} BurrowsWheeler;

// Function to construct the various columns and indices from the transformed string
void construct_indices(BurrowsWheeler *bw, char *transform) {
    // Initialize encounter counts
    int encounters[256] = {0};

    // Copy the transformed string to the first and last columns
    strcpy(bw->first_column, transform);
    strcpy(bw->last_column, transform);

    // Sort the first column
    int length = strlen(transform);
    for (int i = 0; i < length; i++) {
        bw->first_column[i] = transform[i];
    }
    qsort(bw->first_column, length, sizeof(char), compare_chars);

    // Initialize last_index and first_index dictionaries
    for (int i = 0; i < length; i++) {
        char cur_char = bw->last_column[i];
        bw->last_index[i] = encounters[cur_char];
        encounters[cur_char]++;
        snprintf(bw->last_column + i, 100 - i, "%c%d", cur_char, bw->last_index[i]);
        bw->first_index[i] = bw->last_index[i];
        snprintf(bw->first_column + i, 100 - i, "%c%d", cur_char, bw->first_index[i]);
    }

    // Create the mapping from last column to first column (FM index)
    for (int i = 0; i < length; i++) {
        bw->last_first_index[bw->last_index[i]] = bw->first_index[i];
    }

    // Mapping from first column back to the original string
    int original_index = length - 1;
    char current_char = bw->last_column[bw->last_first_index[bw->last_index['$0']]];
    bw->first_original_mapping[bw->first_index['$0']] = original_index - 1;

    for (int i = 2; i < length; i++) {
        current_char = bw->last_column[bw->last_first_index[bw->last_index[current_char]]];
        bw->first_original_mapping[bw->first_index[current_char]] = original_index - i;
    }
}

// Function to reverse the Burrows-Wheeler transformation
char *reverse(BurrowsWheeler *bw, char *transform) {
    int length = strlen(transform);
    char *original = (char *)malloc(length + 1);
    original[0] = '$';

    char current_char = bw->last_column[bw->last_first_index[bw->last_index['$0']]];
    original[1] = current_char;

    for (int i = 2; i < length; i++) {
        current_char = bw->last_column[bw->last_first_index[bw->last_index[current_char]]];
        original[i] = current_char;
    }

    original[length] = '\0';

    return original;
}

// Function to find all occurrences of a given substring within the original string
void query(BurrowsWheeler *bw, char *query) {
    int length = strlen(bw->first_column);
    int top = 0;
    int bottom = length - 1;

    for (int i = strlen(query) - 1; i >= 0; i--) {
        char cur_query_char = query[i];

        for (int j = 0; j < length; j++) {
            int top_set = 0;
            if (bw->last_column[top] != cur_query_char) {
                top++;
            } else {
                top_set = 1;
            }
            if (bw->last_column[bottom] != cur_query_char) {
                bottom--;
            } else {
                if (top_set) break;
            }
            if (top > bottom) {
                printf("Query doesn't match anywhere\n");
                return;
            }
        }

        top = bw->last_first_index[top];
        bottom = bw->last_first_index[bottom];
    }

    for (int i = top; i <= bottom; i++) {
        printf("%d:%d\n", bw->first_original_mapping[i], bw->first_original_mapping[i] + (strlen(query) - 1));
    }
}

// Function to perform the Burrows-Wheeler transformation
void bwt(char *input, char *output) {
    int length = strlen(input);
    char transformed[100]; // Assuming a maximum length of 100

    strcpy(transformed, input);
    strcat(transformed, "$");

    int indices[100];
    for (int i = 0; i < length; i++) {
        indices[i] = i;
    }

    qsort(indices, length, sizeof(int), compare_indices);

    for (int i = 0; i < length; i++) {
        output[i] = transformed[indices[i] - 1];
    }
    output[length] = '\0';
}

// Comparison function for qsort to compare characters
int compare_chars(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);
}

// Comparison function for qsort to compare integers
int compare_indices(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main() {
    char string[] = "ACATGAGCAT";
    char transformed[100]; // Assuming a maximum length of 100
    bwt(string, transformed);
    printf("%s\n", transformed);

    BurrowsWheeler bw;
    construct_indices(&bw, transformed);
    char *reversed = reverse(&bw, transformed);
    printf("%s\n", reversed);
    query(&bw, "CAT");

    return 0;
}