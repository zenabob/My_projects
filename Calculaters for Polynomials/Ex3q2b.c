#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#define SHARED_MEMORY_SIZE 1280
#define size 128

void addPolynomials(int *pol1, int *pol2, int power1, int power2) {
    int max;
    if (power1 > power2){
        max = power1;
    }
    else{
        max =power2;
    }
    int min;
    if (max==power1){
        min=power2;
    }
    else{
        min=power1;
    }
    int y=0;
    int addpol[size] = {0};
    while(y!=min+1){
        int value1=pol1[y];
        int value2=pol2[y];
        addpol[y]=value1+value2;
        y++;
    }
    if(max==power1){
        while(y!=power1+1){
            addpol[y]=pol1[y];
            y++;
        }
    }
    else{
        while(y!=power2+1){
            addpol[y]=pol2[y];
            y++;
        }
    }
    int count=0;
    for (int r=0;r<max+1;r++){
        if (addpol[r]==0){
            count++;
        }
    }
    int mmax=max+1;
    int length=max;
    if(count==max+1){
        printf("0\n");
    }
    else{
        count=0;
        while(mmax !=0){
            if (addpol[max]==0){
                if (max==0){
                    printf("\n");
                }
                max--;
                mmax--;
            }
            else {
                for (int q = length; q > max; q--) {
                    if (addpol[q] == 0) {
                        count++;
                    }
                }
                if (count == (length-max)&& count!=0) {
                    if (max == 0) {
                        if (addpol[max] > 0) {
                            printf("%d\n", addpol[max]);
                        } else if (addpol[max] < 0) {
                            printf("- %d\n", addpol[max]);
                        }
                    } else if (max != length) {
                        if (addpol[max] > 0) {
                            printf("%dx^%d", addpol[max],max);
                        } else if (addpol[max] < 0) {
                            printf("- %dx^%d", addpol[max],max);
                        }
                    }
                    mmax--;
                    max--;

                } else {
                    if (addpol[max] < 0) {
                        int neg = addpol[max] * -1;
                        if (max == length) {
                            printf("%dx^%d", addpol[max], max);
                        } else if (max == 0) {
                            printf(" - %d\n", neg);
                        } else {
                            printf(" - %dx^%d", neg, max);
                        }
                        mmax--;
                        max--;
                    } else {
                        if (max == 0) {
                            printf(" + %d\n", addpol[max]);
                        } else if (max == length) {
                            printf("%dx^%d", addpol[max], max);
                        } else {
                            printf(" + %dx^%d", addpol[max], max);
                        }
                        mmax--;
                        max--;
                    }
                }
            }
        }
    }
}

void subtractPolynomials(int* pol1, int *pol2, int power1, int power2) {
    int max;
    if (power1 > power2){
        max = power1;
    }
    else{
        max =power2;
    }
    int min;
    if (max==power1){
        min=power2;
    }
    else{
        min=power1;
    }
    int y=0;
    int addpol[size] = {0};
    while(y!=min+1){
        int value1=pol1[y];
        int value2=pol2[y];
        addpol[y]=value1-value2;
        y++;
    }
    if(max==power1){
        while(y!=power1+1){
            addpol[y]=pol1[y];
            y++;
        }
    }
    else{
        while(y!=power2+1){
            addpol[y]=-1*pol2[y];
            y++;
        }
    }
    int count=0;
    for (int r=0;r<max+1;r++){
        if (addpol[r]==0){
            count++;
        }
    }
    int mmax=max+1;
    int length=max;


    if(count==max+1){
        printf("0\n");
    }
    else{
        count=0;
        while(mmax !=0){
            if (addpol[max]==0){
                if (max==0){
                    printf("\n");
                }
                max--;
                mmax--;
            }
            else {
                for (int q = length; q > max; q--) {
                    if (addpol[q] == 0) {
                        count++;
                    }
                }
                if (count == (length-max)&& count!=0) {
                    if (max == 0) {
                        if (addpol[max] > 0) {
                            printf("%d\n", addpol[max]);
                        } else if (addpol[max] < 0) {
                            printf("- %d\n", addpol[max]);
                        }
                    } else if (max != length) {
                        if (addpol[max] > 0) {
                            printf("%dx^%d\n", addpol[max],max);
                        } else if (addpol[max] < 0) {
                            printf("- %dx^%d\n", addpol[max],max);
                        }
                    }
                    mmax--;
                    max--;

                } else {
                    if (addpol[max] < 0) {
                        int neg = addpol[max] * -1;
                        if (max == length) {
                            printf("%dx^%d", addpol[max], max);
                        } else if (max == 0) {
                            printf(" - %d\n", neg);
                        } else {
                            printf(" - %dx^%d", neg, max);
                        }
                        mmax--;
                        max--;
                    } else {
                        if (max == 0) {
                            printf(" + %d\n", addpol[max]);
                        } else if (max == length) {
                            printf("%dx^%d", addpol[max], max);
                        } else {
                            printf(" + %dx^%d", addpol[max], max);
                        }
                        mmax--;
                        max--;
                    }
                }
            }
        }
    }
}

void multiplyPolynomials(int *pol1, int *pol2, int power1, int power2) {
    int max;
    int addpol[size] = {0};
    int power;
    int count=0;
    for (int t = 0; t <= power1; t++) {
        for (int o = 0; o <= power2; o++) {
            power = t + o;
            int value = pol1[t] * pol2[o];
            addpol[power] += value;
        }
    }
    max=power1+power2;
    for (int r=0;r<max+1;r++){
        if (addpol[r]==0){
            count++;
        }
    }
    int mmax=max+1;
    int length=max;


    if(count==max+1){
        printf("0\n");
    }
    else{
        count=0;
        while(mmax !=0){
            if (addpol[max]==0){
                if (max==0){
                    printf("\n");
                }
                max--;
                mmax--;
            }
            else {
                for (int q = length; q > max; q--) {
                    if (addpol[q] == 0) {
                        count++;
                    }
                }
                if (count == (length-max)&& count!=0) {
                    if (max == 0) {
                        if (addpol[max] > 0) {
                            printf("%d\n", addpol[max]);
                        } else if (addpol[max] < 0) {
                            printf("- %d\n", addpol[max]);
                        }
                    } else if (max != length) {
                        if (addpol[max] > 0) {
                            printf("%dx^%d\n", addpol[max],max);
                        } else if (addpol[max] < 0) {
                            printf("- %dx^%d\n", addpol[max],max);
                        }
                    }
                    mmax--;
                    max--;

                } else {
                    if (addpol[max] < 0) {
                        int neg = addpol[max] * -1;
                        if (max == length) {
                            printf("%dx^%d", addpol[max], max);
                        } else if (max == 0) {
                            printf(" - %d\n", neg);
                        } else {
                            printf(" - %dx^%d", neg, max);
                        }
                        mmax--;
                        max--;
                    } else {
                        if (max == 0) {
                            printf(" + %d\n", addpol[max]);
                        } else if (max == length) {
                            printf("%dx^%d", addpol[max], max);
                        } else {
                            printf(" + %dx^%d", addpol[max], max);
                        }
                        mmax--;
                        max--;
                    }
                }
            }
        }
    }
}

void calculator (int* pol1 ,int* pol2, int power1, int power2 ,void (callBack)(int*,int*,int,int)){
    return callBack(pol1,pol2,power1,power2);
}

typedef struct {
    int wr;
    char equation[10][size];
    int index;
    int re;
    struct sembuf semaphore;
} sharedMemory;
int main() {
    int pol1[size];
    int pol2[size];
    char equation[size];
    char operation[10];
    char powerr1[size];
    char powerr2[size];
    char valuee[size];
    char valuee2[size];
    sharedMemory *ptr;
    key_t key = ftok(".", 'S');
    int shmid, semid;

    // Get shared memory segment
    shmid = shmget(key, 0, 0666);
    if (shmid == -1) {
        perror("failed to get memory...\n");
        exit(1);
    }
    ptr =(sharedMemory*) shmat(shmid, NULL, 0);
    if (ptr < 0) {
        perror("failed to attach memory...\n");
        exit(1);
    }
    semid = semget(key, 1, 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }
    ptr->re=0;
    while (strcmp(ptr->equation[ptr->index], "END") != 0) {
        ptr->semaphore.sem_op = -1;  // Set semaphore operation
        ptr->semaphore.sem_flg = 0;  // Set semaphore flags
        semop(semid, &ptr->semaphore, 1);
        ptr->re=0;
        while(ptr->wr!=0 ) {
            // Wait for the semaphore of reader
            if (ptr->index == -2) {
                break;
            }
            // Read the equation from shared memory
            if (ptr->index==0){
                strncpy(equation, ptr->equation[ptr->index], size);
            }
            else{
                strncpy(equation, ptr->equation[ptr->index-1], size);
            }
                int i = 0;
                int j = 0;
                while (equation[i] != ' ' && i == 0) {
                    i++;
                }
                if (equation[i] == '(') {
                    i++;
                }
                int power1;
                while (equation[i] != ':') {
                    powerr1[j] = equation[i];
                    i++;
                    j++;
                }
                power1 = atoi(powerr1);
                if (equation[i] == ':') {
                    i++;
                }
                int index = power1;
                int value;
                int v;
                while (equation[i] != ')') {
                    memset(valuee, '\0', size);
                    v = 0;
                    while (equation[i] != ',' && equation[i] != ')') {
                        valuee[v] = equation[i];
                        i++;
                        v++;
                    }
                    value = atoi(valuee);
                    pol1[index] = value;
                    index--;
                    if (equation[i] == ',') {
                        i++;
                    }
                }
                if (equation[i] == ')') {
                    i++;
                }
                if (equation[i] == ' ') {
                    while (equation[i] == ' ') {
                        i++;
                    }
                }
                int g = 0;
                while (equation[i] != '(' && equation[i] != ' ') {
                    operation[g] = equation[i];
                    g++;
                    i++;
                }
                if (equation[i] == ' ') {
                    while (equation[i] == ' ') {
                        i++;
                    }
                }
                if (equation[i] == '(') {
                    i++;
                }
                int power2;
                int p = 0;
                while (equation[i] != ':') {
                    powerr2[p] = equation[i];
                    i++;
                    p++;
                }
                power2 = atoi(powerr2);
                if (equation[i] == ':') {
                    i++;
                }
                int index2 = power2;
                int value2;
                int v2;
                while (equation[i] != ')') {
                    memset(valuee2, '\0', size);
                    v2 = 0;
                    while (equation[i] != ',' && equation[i] != ')') {
                        valuee2[v2] = equation[i];
                        i++;
                        v2++;
                    }
                    value2 = atoi(valuee2);
                    pol2[index2] = value2;
                    index2--;
                    if (equation[i] == ',') {
                        i++;
                    }
                }
                if (strcmp(operation, "ADD") == 0) {
                    calculator(pol1, pol2, power1, power2, addPolynomials);
                } else if (strcmp(operation, "SUB") == 0) {
                    calculator(pol1, pol2, power1, power2, subtractPolynomials);
                } else if (strcmp(operation, "MUL") == 0) {
                    calculator(pol1, pol2, power1, power2, multiplyPolynomials);
                }
                if(ptr->index>0) {
                    ptr->index--;
                }
                if(ptr->index==0){
                    ptr->re=1;
                }
                ptr->wr--;
                if (ptr->index==0){
                    memset(ptr->equation[ptr->index], '\0', 0);
                }
                else {
                    memset(ptr->equation[ptr->index - 1], '\0', 0);
                }
                memset(equation, '\0', 0);
                memset(operation, '\0', 10);
                memset(powerr1, '\0', size);
                memset(powerr2, '\0', size);
                memset(valuee, '\0', size);
                memset(valuee2, '\0', size);
                memset(pol1, 0, size * sizeof(int));
                memset(pol2, 0, size * sizeof(int));
        }
        if(ptr->index == -2){
            break;
        }
        ptr->semaphore.sem_op = 1;   // Set semaphore operation
        semop(semid, &ptr->semaphore, 1);
        sleep(1);
    }

    shmdt(ptr);
}