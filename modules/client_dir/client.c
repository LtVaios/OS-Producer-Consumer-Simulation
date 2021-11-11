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

int main(int argc, char* argv[]) {
    int n = atoi(argv[0]);
    int number_of_lines = atoi(argv[1]);
    time_t start,end,elapsed;
    long int full_time=0;
    srand(time(NULL));

    //getting access to shared memory segment
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

    //opening the semaphores
    sem_t* serv_sem = sem_open(SSEM_NAME, O_RDWR);
    if (serv_sem == SEM_FAILED) {
        perror("Error! sem_open for server sem failed in child process");
        exit(EXIT_FAILURE);
    }
    sem_t* cli_sem = sem_open(CSEM_NAME, O_RDWR);
    if (cli_sem == SEM_FAILED) {
        perror("Error! sem_open for client sem failed in child process");
        exit(EXIT_FAILURE);
    }

    //main loop
    for(int i = 0; i < n; i++){
        //Entry section
        time(&start);
        //printf("child initial cli value:");
        //print_sem_value(cli_sem);
        if (sem_wait(cli_sem) < 0) {
            perror("Error! sem_wait failed on child");
            continue;
        }
        //Critical section 1
        //asking the parent for a random line from the file
        sm->line_req = (rand()%number_of_lines)+1;
        printf("zitaw line %d\n",sm->line_req);

        //Exit section
        if (sem_post(serv_sem) < 0)
            perror("Error! sem_post failed on child");
        //Entry section 2
        if (sem_wait(cli_sem) < 0) {
            perror("Error! sem_wait failed on child");
            continue;
        }
        //Critical section 2
        printf("pira to line: %s",sm->line_text);
        //Exit section 2
        if (sem_post(cli_sem) < 0)
            perror("Error! sem_post failed on child");
        time(&end);
        elapsed = end - start;
        full_time += elapsed;
    }

    //Remaining section
    printf("This is the child %d with average time: %lf seconds.\n",getpid(),(double)full_time/n);
    sem_close(serv_sem);
    sem_close(cli_sem);
    exit(EXIT_SUCCESS);
}