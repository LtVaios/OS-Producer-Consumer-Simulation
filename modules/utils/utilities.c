#include <stdio.h>
#include <stdlib.h>
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
    char* result;
    char line[MAX_L_CHARS];
    int count = 1;
    while ( fgets(line, MAX_L_CHARS, f) != NULL ) {
        printf("1\n");
        if(count == no_line){
            result = &line[0];
            rewind(f);
            return result;
        }
        count++;
    }
    return NULL;
}

bool isPositiveNumber(char number[]) {
    int i = 0;

    //reject negative numbers
    if (number[0] == '-')
        return false;

    for (; number[i] != 0; i++) {
        if (!isdigit(number[i]))
            return false;
    }

    return true;
}