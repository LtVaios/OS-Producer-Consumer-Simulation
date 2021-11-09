#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

struct shared{
    int no_line;
    int n;
};

int main(int argc, char *argv[]){
    if(argc > 4) {
        printf("Too many arguments!\n");
        exit(-1);
    }
    if(argc < 4) {
        printf("The program needs 3 arguments!\n");
        exit(-1);
    }

    FILE *f  = fopen(argv[1], "r");
    if (!f) {
        printf("Error, could not open file!\n");
        exit(-1);
    }
    int k = atoi(argv[2]);
    int n = atoi(argv[3]);

    parent(f,k,n);
    fclose(f);
    return 0;
}

int parent(FILE* f, int k, int n) {
    return 0;
}

//returns the number of lines in a file and resets it's file pointer
int get_number_lines(FILE* f){
    char line[MAX_L_CHARS];
    int count = 0;
    while (fgets( line, MAX_L_CHARS, f) != NULL )
        count++;
    rewind(f);
    return count;
}

//returns a specific line in a file and resets it's file pointer
char* get_specific_line(FILE* f,int no_line){
    char* result=malloc(MAX_L_CHARS * sizeof(char));
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
