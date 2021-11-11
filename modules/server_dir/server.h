#ifndef OS2021_SERVER_H
#define OS2021_SERVER_H

#define CHILD_PROGRAM "./client"
#define LINE_SZ 100
#define SHMKEY (key_t)0001
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define SSEM_NAME "ssem"
#define CSEM_NAME "csem"

void create_sem(char* name,int init_val);
void delete_sem(char* name,sem_t* sem);
void delete_shm(void* addr,int id);

#endif
