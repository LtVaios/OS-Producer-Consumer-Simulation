#ifndef OS2021_UTILITIES_H
#define OS2021_UTILITIES_H

#define MAX_L_CHARS 100

char* get_specific_line(FILE* f,int no_line);
int get_number_lines(FILE* f);
int isPositiveNumber(char number[]);
char* number_to_str(int i);
void print_sem_value(sem_t* s);
int get_sem_value(sem_t* s);

#endif
