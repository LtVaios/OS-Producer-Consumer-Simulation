#ifndef OS2021_SERVER_H
#define OS2021_SERVER_H

#define CHILD_PROGRAM "./client"
#define LINE_SZ 100
#define SHMKEY (key_t)0001
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define SEM_NAME "sem_one"

void create_sem(char* name);
void delete_sem(char* name);

#endif
