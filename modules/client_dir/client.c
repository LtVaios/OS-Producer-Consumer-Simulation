#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <errno.h>

#include "../utils/utilities.h"
#include "client.h"

int main(int argc, char *argv[]) {
    int n = argv[0];
    sem_t *semaphore = sem_open(SEM_NAME, O_RDWR);
    if (semaphore == SEM_FAILED) {
        perror("Error! sem_open failed in child process\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < n; i++){
        if (sem_wait(semaphore) < 0) {
            perror("Error! sem_wait failed on child\n");
            continue;
        }

        printf("PID %ld acquired semaphore\n", (long)getpid());

        if (sem_post(semaphore) < 0) {
            perror("Error! sem_wait failed on child\n");
        }
        sleep(1);
    }
    printf("This is a child with n:%s\n",argv[0]);

    if (sem_close(semaphore) < 0) {
        perror("Error! sem_close failed on child\n");
        sem_unlink(SEM_NAME);
        exit(EXIT_FAILURE);
    }
    exit(0);
}