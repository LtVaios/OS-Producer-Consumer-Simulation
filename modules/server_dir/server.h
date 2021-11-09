#ifndef OS2021_SERVER_H
#define OS2021_SERVER_H

#define CHILD_PROGRAM "./client"
#define LINE_SZ 100
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

void create_sem(char[] name);

#endif
