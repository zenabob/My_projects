#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        while(mmax !=0) {
            if (addpol[max] == 0) {
                if (max == 0) {
                    printf("\n");
                }
                max--;
                mmax--;
            } else {
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
    int max=power1+power2;
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
                if (count == (length-max) && count!=0) {
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
int main() {
    int pol1[size];
    int pol2[size];
    char equation[size];
    char operation[10];
    char powerr1[size];
    char powerr2[size];
    char valuee[size];
    char valuee2[size];

    while (strcmp(equation, "END") != 0) {
        printf("Enter equation (or 'END' to exit): \n");
        fgets(equation, size, stdin);
        equation[strlen(equation) - 1] = '\0';
        if(strcmp(equation, "END") == 0){
            break;
        }
        int i = 0;
        int j = 0;
        while (equation[i]!=' ' && i==0){
            i++;
        }
        if (equation[i] =='('){
            i++;
        }
        int power1;
        memset(powerr1,'\0',size);
        while (equation[i]!= ':'){
            powerr1[j]=equation[i];
            i++;
            j++;
        }
        power1=atoi(powerr1);
        if (equation[i]==':'){
            i++;
        }
        int index = power1;
        int value;
        int v;
        while (equation[i]!=')'){
            memset(valuee,'\0',size);
            v=0;
            while (equation [i] != ',' && equation[i]!=')') {
                valuee[v] = equation[i];
                i++;
                v++;
            }
            value= atoi(valuee);
            pol1[index]=value;
            index--;
            if(equation[i]==','){
                i++;
            }
        }
        if (equation[i] == ')'){
            i++;
        }
        if (equation [i] == ' '){
            while (equation[i]==' '){
                i++;
            }
        }
        int g=0;
        while (equation [i] !='(' && equation[i]!=' '){
            operation[g]=equation[i];
            g++;
            i++;
        }
        if (equation [i] == ' '){
            while (equation[i]==' '){
                i++;
            }
        }
        if (equation[i] =='('){
            i++;
        }
        int power2;
        int p=0;
        while (equation[i]!= ':'){
            powerr2[p]=equation[i];
            i++;
            p++;
        }
        power2=atoi(powerr2);
        if (equation[i]==':'){
            i++;
        }
        int index2=power2;
        int value2;
        int v2;
        while (equation[i]!=')'){
            memset(valuee2,'\0',size);
            v2=0;
            while (equation [i] != ',' && equation[i]!=')') {
                valuee2[v2] = equation[i];
                i++;
                v2++;
            }
            value2= atoi(valuee2);
            pol2[index2]=value2;
            index2--;
            if(equation[i]==','){
                i++;
            }
        }
        if (strcmp(operation, "ADD") == 0) {
            calculator(pol1, pol2,power1,power2, addPolynomials);
        } else if (strcmp(operation, "SUB") == 0) {
            calculator(pol1, pol2,power1,power2, subtractPolynomials);
        } else if (strcmp(operation, "MUL") == 0) {
            calculator(pol1, pol2,power1,power2, multiplyPolynomials);
        }

        // Resetting variables for the next iteration...
        memset(equation, '\0', size);
        memset(operation, '\0', 10);
        memset(powerr1, '\0', size);
        memset(powerr2, '\0', size);
        memset(valuee, '\0', size);
        memset(valuee2, '\0', size);
        memset(pol1, 0, size * sizeof(int));
        memset(pol2, 0, size * sizeof(int));
    }

    return 0;
}
