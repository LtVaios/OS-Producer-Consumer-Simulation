#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <errno.h>

#include "../utils/utilities.h"
#include "server.h"

//1st arg: filename, 2nd arg: K (no of children), 3rd arg: N (no of each child's transactions)
int main(int argc, char *argv[]) {
    //Arguments error handling
    if(argc > 4) {
        printf("Too many arguments!\n");
        exit(-1);
    }
    if(argc < 4) {
        printf("The program needs 3 arguments!\n");
        exit(-1);
    }
    if(!isPositiveNumber(argv[2]) && !isPositiveNumber(argv[3]) {
        printf("The last 2 arguments must be positive integers\n");
        exit(-1);
    }

    //all text files should be in txt_files directory
    char path[] = "./txt_files/";
    strcat( path, argv[1]);
    FILE *f  = fopen(path, "r");
    if (!f) {
        printf("Error, could not open file!\n");
        exit(-1);
    }
    int k = atoi(argv[2]);
    int pid;
    int status;
    create_sem("sem_one");

    //make the children
    for (int i = 0; i < k; i++) {
        if ((pid = fork()) < 0) {
            perror("Error! Could not fork\n");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            if (execl(CHILD_PROGRAM, argv[3], NULL) < 0) { //argv[3] is N (number of each child's transactions)
                perror("Error! exec failed\n");
                exit(EXIT_FAILURE);
            }
            break;
        }
    }

    //parent Remaining section
    printf("Done creating clients, sleeping for a while\n");
    sleep(5);

    // Wait for all children to finish
    for (;;) {
        pid = wait(&status);
        if (pid < 0) {
            if (errno == ECHILD) {
                printf("All children exited successfully!\n");
                break;
            }
            else {
                perror("Could not wait");
            }
        }
        else {
            printf("Child %d exited with status %d\n", pid, status);
        }
    }

    fclose(f);
    exit(0);
}

void create_sem(char[] name){
    sem_t *semaphore = sem_open(name, O_CREAT | O_EXCL, SEM_PERMS, 1);

    if (semaphore == SEM_FAILED) {
        perror("Error! sem_open failed in create_sem\n");
        exit(EXIT_FAILURE);
    }

    if (sem_close(semaphore) < 0) {
        perror("Error! sem_close failed in create_sem\n");
        sem_unlink(SEM_NAME);
        exit(EXIT_FAILURE);
    }
}
