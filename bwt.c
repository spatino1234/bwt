// BWT implementation in C 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Stores the rotation, its index and the results suffix.
struct rotation {
    int index;
    char *suffix;
};

/* Using const void * gives you a const pointer to void, generic pointer
 Creating rotation structs for each input, compares the text and returns strcmp value. 
    */
int compareRotation(const void *rot1, const void*rot2) {
    
   
    struct rotation *r1 = (struct rotation*)rot1;
    struct rotation *r2 = (struct rotation*)rot2;
    return strcmp(r1->suffix, r2->suffix);
}

int* makeSuffixArray(char *input_text, int len_text){
    
    // Array of rotations and corresponding index
    struct rotation suff[len_text];

    /* Goes through creates a substring suffix and assigns an index
    corresponding to the position where the substring starts*/
    for(int i = 0; i < len_text; i++){
        suff[i].index = i;
        suff[i].suffix = (input_text + i);
    }

    // for(int i = 0; i < len_text; i++){
    //     printf("%d ", suff[i].index);
    //     printf("%s\n", suff[i].suffix);
    // }
    /* Sort roations using comparison function from earlier*/
    qsort(suff, len_text, sizeof(struct rotation), compareRotation);
    // for(int i = 0; i < len_text; i++){
    //     printf("%d ", suff[i].index);
    //     printf("%s\n", suff[i].suffix);
    // }

    /* Store index of sorted rotations */
    int *suffix_arr = (int*)malloc(len_text * sizeof(int));

    for(int i = 0; i < len_text; i ++){
        suffix_arr[i] = suff[i].index;
    }

    for(int i = 0; i < len_text; i++){
        printf("%d ", suffix_arr[i]);
        // printf("%s\n", suff[i].suffix);
    }
    return suffix_arr;

}

char *findLastChar(char *input_text, int *suffix_arr, int n){
    char *bwt_arr = (char*)malloc(n * sizeof(char));
    int i ;

    // entering characters from the input text in the order 
    // of the bwt from suffix array. 
    for (i = 0; i < n ; i++){
        int j = suffix_arr[i];
        printf("%d\n", j);
        if (j < 0){
            j = j + n;
        }
        bwt_arr[i] = input_text[j];
    }

    for(int i = 0; i < n; i++){
        printf("%c\n", bwt_arr[i]);
    }

    bwt_arr[i] = '\0';

    return bwt_arr;
}

int main(){
   char input[] = "mississippi$";
    int len_text = strlen(input);

    int *suffix_arr = makeSuffixArray(input, len_text);

    char *bwt_arr = findLastChar(input, suffix_arr, len_text);
    printf("Input text : %s\n", input);
    printf("Burrows - Wheeler Transform : %s\n",
        bwt_arr);
    return 0;
}