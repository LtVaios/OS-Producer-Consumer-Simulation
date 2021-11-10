#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <semaphore.h>
#include "utilities.h"

//returns the number of lines in a file and resets it's file pointer
int get_number_lines(FILE* f) {
    char line[MAX_L_CHARS];
    int count = 0;
    while (fgets( line, MAX_L_CHARS, f) != NULL )
        count++;
    rewind(f);
    return count;
}

//returns a specific line in a file and resets it's file pointer
char* get_specific_line(FILE* f,int no_line) {
    if(f == NULL){
        printf("Error! File cannot be null.\n");
        exit(EXIT_FAILURE);
    }
    char* result;
    char line[MAX_L_CHARS];
    int count = 1;
    while ( fgets(line, MAX_L_CHARS, f) != NULL ) {
        if(count == no_line){
            result = &line[0];
            printf("result grammi: %s\n",result);
            rewind(f);
            return result;
        }
        count++;
    }
    return NULL;
}

int isPositiveNumber(char number[]) {
    int i = 0;

    //reject negative numbers
    if (number[0] == '-')
        return 0;

    for (; number[i] != 0; i++) {
        if (!isdigit(number[i]))
            return 0;
    }

    return 1;
}

//number to string converter
//we need this function to convert number of lines to char* so the parent can give it
//as an argument to the childen
char* number_to_str(int i){
    int length = snprintf( NULL, 0, "%d", i );
    char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", i );
    return str;
}


void print_sem_value(sem_t* s){
    int* t=malloc(1*sizeof (int));
    sem_getvalue(s,test);
    printf("semaphore value:%d\n",*test);
    free(t);
}