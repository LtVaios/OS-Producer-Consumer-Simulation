#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "../utils/utilities.h"
#include "server.h"

struct shared{
    //the line that children will request from the parent
    int line_req;
    char line_text[LINE_SZ];
};

typedef struct shared* shared_mem;

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
    if(!isPositiveNumber(argv[2]) && !isPositiveNumber(argv[3])) {
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

    //variables, semaphores and shared memory initialization
    int k = atoi(argv[2]);
    int n = atoi(argv[3]);
    int pid;
    int status;

    //semaphore
    create_sem(SEM_NAME);
    sem_t *semaphore = sem_open(SEM_NAME, O_RDWR, 1);
    if (semaphore == SEM_FAILED) {
        perror("Error! sem_open failed in parent process");
        exit(EXIT_FAILURE);
    }
    //we post once the semaphore so it has 1 as initial value
    if (sem_post(semaphore) < 0)
        perror("Error! sem_post failed on child");

    //shared memory
    void *shared_memory_addr = (void *)0;
    int shmid;
    shmid = shmget(SHMKEY, sizeof(struct shared), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("Error! shmget failed in parent");
        exit(EXIT_FAILURE);
    }
    shared_memory_addr = shmat(shmid, NULL, 0);
    if (shared_memory_addr == (void *)-1) {
        perror("Error! shmat failed in parent");
        exit(EXIT_FAILURE);
    }
    shared_mem sm = (shared_mem) shared_memory_addr;
    printf("Shared memory segment with id %d attached at %p\n", shmid, shared_memory_addr);

    char* str_no_of_lines = number_to_str(get_number_lines(f));
    //make the children
    for (int i = 0; i < k; i++) {
        if ((pid = fork()) < 0) {
            perror("Error! Could not fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            if (execl(CHILD_PROGRAM, argv[3], str_no_of_lines, NULL) < 0) { //argv[3] is N (number of each child's transactions)
                perror("Error! exec failed");
                exit(EXIT_FAILURE);
            }
            break;
        }
    }

    //parent communication with children section
    for(int j=0 ; j < k*n ; j++) {
        //Entry section
        if (sem_wait(semaphore) < 0) {
            perror("Error! sem_wait failed on parent");
            continue;
        }
        //Critical section 1
        char* line = malloc(MAX_L_CHARS * sizeof(char));
        line = get_specific_line(f, sm->line_req);
        strncpy(sm->line_text,line,MAX_L_CHARS);
        printf("dinw line %d\n",sm->line_req);
        free(line);


        //Exit section
        if (sem_post(semaphore) < 0)
            perror("Error! sem_post failed on parent");
    }

    // Wait for all the children to finish
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

    //shared memory detach
    if (shmdt(shared_memory_addr) == -1) {
        printf("Error! shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    //freeing all the memory and destroying all sem, shm
    free(str_no_of_lines);
    delete_sem(SEM_NAME);
    fclose(f);
    exit(EXIT_SUCCESS);
}

void create_sem(char* name){
    sem_t *semaphore = sem_open(name, O_CREAT, SEM_PERMS, 1);
    if (semaphore == SEM_FAILED) {
        perror("Error! sem_open failed in create_sem");
        exit(EXIT_FAILURE);
    }
    return;
}

void delete_sem(char* name){
    if (sem_unlink(SEM_NAME) < 0) {
        perror("Error! sem_unlink failed in delete_sem");
        exit(EXIT_FAILURE);
    }
}

