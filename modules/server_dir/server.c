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
    //The variable that the parent will write the line and clients will read from
    char line_text[LINE_SZ];
};

typedef struct shared* shared_mem;

//1st arg: filename, 2nd arg: K (number of children), 3rd arg: N (number of each child's transactions)
int main(int argc, char *argv[]) {
    //Command line arguments error handling
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
        printf("Error, could not open text file!\n");
        exit(-1);
    }

    //variables, semaphores and shared memory initialization
    int k = atoi(argv[2]);
    int n = atoi(argv[3]);
    char* str_no_of_lines = number_to_str(get_number_lines(f));
    int pid;
    int stat;

    //shared memory init
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

    //server semaphore init
    create_sem(SSEM_NAME,0);
    sem_t* serv_sem = sem_open(SSEM_NAME, O_RDWR);
    if (serv_sem == SEM_FAILED) {
        perror("Error! sem_open failed in parent process");
        exit(EXIT_FAILURE);
    }

    //client semaphore init
    create_sem(CSEM_NAME,1);
    sem_t* cli_sem = sem_open(CSEM_NAME, O_RDWR);
    if (cli_sem == SEM_FAILED) {
        perror("Error! sem_open failed in parent process");
        exit(EXIT_FAILURE);
    }

    //client second semaphore init
    create_sem(CSEM_NAME_2,1);

    //make the clients(children)
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
    char* line = "empty";
    for(int j=0 ; j < k*n ; j++) {
        //Entry section
        if (sem_wait(serv_sem) < 0) {
            perror("Error! sem_wait failed on parent");
            continue;
        }

        //Critical section
        line = get_specific_line(f, sm->line_req);
        memcpy(sm->line_text,line,MAX_L_CHARS);

        //Exit section
        if (sem_post(cli_sem) < 0)
            perror("Error! sem_post failed on parent");
    }

    // Wait for all the children to finish
    while( 1 ) {
        pid = wait(&stat);
        if (pid < 0) {
            if (errno == ECHILD) {
                printf("All clients exited.\n");
                break;
            }
            else {
                perror("Could not wait");
            }
        }
        else {
            printf("Client: %d, exited with status: %d\n", pid, stat);
        }
    }

    //freeing all the memory and destroying all sem, shm
    free(str_no_of_lines);
    if (sem_close(serv_sem) < 0) {
        perror("Error! sem_close(3) in create_sem failed");
        exit(EXIT_FAILURE);
    }
    delete_sem(SSEM_NAME, serv_sem);
    if (sem_close(cli_sem) < 0) {
        perror("Error! sem_close(3) in create_sem failed");
        exit(EXIT_FAILURE);
    }
    delete_sem(CSEM_NAME, cli_sem);
    delete_shm(shared_memory_addr,shmid);
    fclose(f);
    exit(EXIT_SUCCESS);
}

void create_sem(char* name,int init_val){
    //create
    sem_t *semaphore = sem_open(name, O_CREAT, SEM_PERMS, init_val);
    if (semaphore == SEM_FAILED) {
        perror("Error! sem_open failed in create_sem");
        exit(EXIT_FAILURE);
    }
    //close
    if (sem_close(semaphore) < 0) {
        perror("Error! sem_close(3) in create_sem failed");
        exit(EXIT_FAILURE);
    }
    return;
}

void delete_sem(char* name,sem_t* sem){
    if (sem_unlink(name) < 0) {
        perror("Error! sem_unlink(3) in delete_sem failed");
        exit(EXIT_FAILURE);
    }
}

void delete_shm(void* addr,int id){
    //shared memory detach
    if (shmdt(addr) == -1) {
        printf("Error! shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    if(shmctl(id, IPC_RMID, 0) < 0){
        perror ("SHM remove error.");
        exit(EXIT_FAILURE);
    }
}

