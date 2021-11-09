#ifndef OS2021_MAIN_H
#define OS2021_MAIN_H

#define MAX_L_CHARS 100

int get_number_lines(FILE* f);
char* get_specific_line(FILE* f,int no_line);
int parent(FILE* f, int k, int n);

#endif
