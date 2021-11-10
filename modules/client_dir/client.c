#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../utils/utilities.h"
#include "client.h"

struct shared{
    int line_req;
    char line_text[LINE_SZ];
};

typedef struct shared* shared_mem;

int main(int argc, char *argv[]) {
    int n = atoi(argv[0]);
    int number_of_lines = atoi(argv[1]);
    clock_t t;
    double full_time = 0;
    srand(time(NULL));

    //opening the semaphore
    sem_t *semaphore = sem_open(SEM_NAME, O_RDWR);
    if (semaphore == SEM_FAILED) {
        perror("Error! sem_open failed in child process");
        exit(EXIT_FAILURE);
    }

    //getting access to the same shared memory segment as the parent
    //shared memory
    void *shared_memory_addr = (void *)0;
    int shmid;
    shmid = shmget(SHMKEY, sizeof(struct shared), 0666 | IPC_CREAT);
    if (shmid == -1) {
        printf("Error! shmget failed\n");
        exit(EXIT_FAILURE);
    }
    shared_memory_addr = shmat(shmid, NULL, 0);
    if (shared_memory_addr == (void *)-1) {
        printf("Error! shmat failed\n");
        exit(EXIT_FAILURE);
    }
    shared_mem sm = (shared_mem) shared_memory_addr;

    //main loop
    for(int i = 0; i < n; i++){
        //Entry section
        t = clock();
        if (sem_wait(semaphore) < 0) {
            perror("Error! sem_wait failed on child");
            continue;
        }
        //Critical section 1
        sm->line_req = rand()%(number_of_lines+1);
        printf("zitaw line %d\n",sm->line_req);

        //Exit section
        if (sem_post(semaphore) < 0)
            perror("Error! sem_post failed on child");



        sleep(2);
        //2
        if (sem_wait(semaphore) < 0) {
            perror("Error! sem_wait failed on child");
            continue;
        }
        printf("pira to line: %s\n",sm->line_text);
        if (sem_post(semaphore) < 0)
            perror("Error! sem_post failed on child");

        full_time += (clock() - t);
    }

    //Remaining section
    printf("This is the child %d with average time:%f\n",getpid(),full_time/n);
    exit(EXIT_SUCCESS);
}