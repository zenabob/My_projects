#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#define SHARED_MEMORY_SIZE 1280
#define size 128

typedef struct {
    int wr;
    char equation[10][size];
    int index;
    int re;
    struct sembuf semaphore;
} sharedMemory;

int main() {
    sharedMemory* ptr;
    key_t key = ftok(".", 'S');
    int shmid, semid;

    // Create shared memory segment
    shmid = shmget(key, SHARED_MEMORY_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("failed to get memory...\n");
        exit(1);
    }
    ptr = (sharedMemory*)shmat(shmid, NULL, 0);
    if (ptr < 0) {
        perror("failed to attach memory...\n");
        exit(1);
    }
    // Create semaphore
    semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // Initialize semaphores
    if (semctl(semid, 0, SETVAL, 1) == -1 ) {
        perror("semctl");
        exit(1);
    }

    ptr->index = 0;
    ptr->wr=0;
    while (strcmp(ptr->equation[ptr->index], "END") != 0) {
        // Write data to shared memory
        // Wait for the semaphore of writer
        if (ptr->index < 10) {
            ptr->semaphore.sem_op = -1;  // Set semaphore operation
            ptr->semaphore.sem_flg = 0;  // Set semaphore flags
            semop(semid, &ptr->semaphore, 1);
            printf("Enter equation (or 'END' to exit): ");
            fgets(ptr->equation[ptr->index], size, stdin);
            ptr->equation[ptr->index][strlen(ptr->equation[ptr->index]) - 1] = '\0';
            if (ptr->re!=1) {
                ptr->index++;
            }
            ptr->wr+=1;
        }
        if(strcmp(ptr->equation[ptr->index], "END") == 0){
            ptr->index=-2;
            break;
        }
        ptr->semaphore.sem_op = 1;   // Set semaphore operation
        semop(semid, &ptr->semaphore, 1);
        sleep(5);
    }

    // Detach and remove shared memory
    shmdt(ptr);
    shmctl(shmid, IPC_RMID, NULL);
    // Destroy semaphores
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(1);
    }

    return 0;
}
