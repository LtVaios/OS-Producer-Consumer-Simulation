#ifndef OS2021_UTILITIES_H
#define OS2021_UTILITIES_H

#define MAX_L_CHARS 100

char* get_specific_line(FILE* f,int no_line);
int get_number_lines(FILE* f);
bool isPositiveNumber(char number[]);

#endif
