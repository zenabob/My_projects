#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h>
#include <signal.h>
int flagg=0;
int status;
pid_t id;
int piid=0;

int  MakeFork(){
    pid_t pid;
    pid= fork();
    return pid;
}
void makeExecute(char ** arr){
    execvp(arr[0], arr);
    perror("error execvp");
    flagg = 1;
    exit(EXIT_FAILURE);
}

void child_fun(){
    int bgpid=0;
    waitpid(-1,&status,WNOHANG);
}
int is_stopped =0;
void signal_handler(int signum) {
    if (signum == SIGTSTP) {
            kill(id, SIGTSTP);// Stop the process
            is_stopped = 1;
            piid = id;
    }
}



int main() {
    signal(SIGTSTP, signal_handler);
    signal(SIGCHLD, child_fun);
    int size = 510;
    char *argv;
    argv = (char *) malloc(size * sizeof(char));
    char cwd[1024];
    int num = 0;
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return EXIT_FAILURE;
    } else {
        int numofcom = 0;
        int numofargs = 0;
        int lengthofcom = 0;
        int indName = 0;
        int indValue = 0;
        char **var_name = (char **) malloc(1000 * sizeof(char *));
        char *helpVar_name=(char*)malloc(510 *sizeof(char ));
        char **var_value = (char **) malloc(1000 * sizeof(char *));
        char *helpVar_value=(char*)malloc(510 *sizeof(char ));
        char *help=(char*)malloc(510 *sizeof (char ));
        char **array = (char **) malloc(size * sizeof(char *));
        char *helparray = (char *) malloc(510 * sizeof(char));
        char *varia = (char *) malloc(size * sizeof(char));
        char *fileName = (char *) malloc(size * sizeof(char));
        char *command = (char *) malloc(size * sizeof(char));
        char *commandhelp = (char *) malloc(size * sizeof(char));
        char **arr = (char **) malloc(sizeof(char));
        char *helppipe = (char *) malloc(size * sizeof(char));
        char **rightcom = (char **) malloc(size * sizeof(char *));
        char *helperr = (char *) malloc(size * sizeof(char));
        char *helper = (char *) malloc(size * sizeof(char));
        while (num != 3) {
            printf("#cwd:<%d>|#args:<%d>@ %s>", numofcom, numofargs, cwd);
            fgets(argv, size, stdin);
            argv[strlen(argv) - 1] = '\0';
                bool flaag = true;
                int j = 0;
                int i = 0;
                int l = 0;
                if (argv[j] == '\0') {
                    num++;
                } else {
                    num = 0;
                }
                if (num == 3) {
                    break;
                }

                while (argv[j] != '\0') {
                    flaag = true;
                    int inx[10];
                    int in = 0;
                    int pi = 0;
                    int oe = 0;
                    int cog = 0;
                    int replace;
                    int ig = 0;
                    int no = 1;
                    int heg = 0;
                    int an = 0;
                    while (argv[j] != ';' && argv[j] != '\0') {
                        if (argv[j] == '"' && argv[j] != '\0') {
                            j++;
                            while (argv[j] != '"' && argv[j] != '\0') {
                                if (argv[j] == '|') {
                                    pi = 1;
                                    helparray[i] = argv[j];
                                    i++;
                                    j++;

                                }
                                if (argv[j] == '>') {
                                    oe = 1;
                                    helparray[i] = argv[j];
                                    i++;
                                    j++;

                                }
                                if (argv[j] == '&') {
                                    an = 1;
                                    helparray[i] = argv[j];
                                    i++;
                                    j++;

                                } else {
                                    helparray[i] = argv[j];
                                    i++;
                                    j++;
                                }
                            }
                            if (argv[j] == '"') {
                                j++;
                            }

                        }
                        if (argv[j] == '$') {
                            // to check all the vars in the one command 
                            while (argv[j] == '$') {
                                while (argv[j] != '"' && argv[j] != '\0') {
                                    if (argv[j] == '|') { // check if i have pipe
                                        pi = 1;
                                        helparray[i] = argv[j];
                                        i++;
                                        j++;

                                    }
                                    if (argv[j] == '>') { // check if i have '>'
                                        oe = 1;
                                        helparray[i] = argv[j];
                                        i++;
                                        j++;

                                    }
                                    if (argv[j] == '&') {// check if have '&'
                                        an = 1;
                                        helparray[i] = argv[j];
                                        i++;
                                        j++;

                                    } else {
                                        helparray[i] = argv[j];
                                        i++;
                                        j++;
                                    }
                                }
                                if (argv[j] == '"') {
                                    heg++;// num of '""'
                                    inx[in] = j; // index of text in '""'
                                    cog = 0; // num of the '""' before the current '""'
                                    for (int y = 0; y < j; y++) {
                                        if (argv[y] == '"') {
                                            cog++;
                                        }
                                    }
                                    inx[in] = inx[in] - cog; // remove the num of the  '""' from the index because in helparray we dont  have '""'
                                    in++;
                                    j++;
                                    while (argv[j] != '"' && argv[j] != '\0') {
                                            helparray[i] = argv[j];
                                            i++;
                                            j++;
                                    }
                                    if (argv[j] == '"') {
                                        j++;
                                    }
                                }
                                
                                else {
                                     helparray[i] = argv[j];
                                     i++;
                                     j++;
                                }
                            }
                        }
                    }    
                    if ((strcmp("bg", helparray) == 0) && is_stopped == 1) { // have SIGCONT
                        kill(piid, SIGCONT);// Resume the process
                        is_stopped = 0;
                        piid = 0;
                    } else {
                    int numOfArgu = 0;
                    lengthofcom = strlen(helparray);
                    if (lengthofcom > 510) {
                        printf("have more of max length \n");
                        flaag = false;
                    }
                    // num of argu
                    for (int v = 0; helparray[v] != '\0'; v++) {
                        if (!(helparray[v] == '"' || helparray[v] == ' ' || helparray[v] == '\t' || helparray[v] == '\n')) {
                          numOfArgu++;
                        } if (helparray[v] == '"') {
                             v++;
                             while (helparray[v]!= '"') {
                                  v++;
                             }
                            if (helparray[v] == '"'){
                                 v++;
                            }
                        }
                    }
                    if (numOfArgu > 10) {
                        printf("have more of max length. \n");
                        flaag = false;
                    }
                    // check if have cd in command 
                    char *result = strstr(helparray, "cd");
                    if (result != NULL) {
                        int index = (int) (result - helparray);
                        int m = 0;
                        while (helparray[m] != '\0' && helparray[m] == ' ') {
                            m++;
                        }
                        if (m == index) {
                            if ((helparray[m + 2] == '\0') || (helparray[m + 2] == ' ')) {
                                printf("cd not supported. \n");
                                flaag = false;
                            }
                        }
                    }
                    int ind = -1;
                    for (int n = 0; helparray[n] != '\0'; n++) {
                        if (helparray[n] == '=') {
                            ind = n;
                            flaag = false;
                            break;
                        }
                    }
                    if (ind > 0) {
                        int m = 0;
                        int copyind = ind;
                        copyind--;
                        while (helparray[copyind] == ' ') {
                            copyind--;
                        }
                        while (helparray[copyind] != ' ' && copyind != 0 && helparray[copyind] != ';') { // back to the init of var name
                            copyind--;
                        }
                        while (copyind != ind) { // take the var name
                            helpVar_name[m] = helparray[copyind];
                            copyind++;
                            m++;
                        }
                        int r = 0;
                        // remove the ' ' from the name
                        for (int f = 0; helpVar_name[f] != '\0'; f++) {
                            if (helpVar_name[f] != ' ') {
                                helpVar_name[r++] = helpVar_name[f];
                            }
                        }
                        helpVar_name[r] = '\0';
                        // check if the name just letters and '_'
                        for (int x = 0 ; helpVar_name[x] != '\0'; x++) {
                            if (!isalnum(helpVar_name[x]) && helpVar_name[x] != '_') {
                                no = 0;
                                printf("Invalid variable name\n");
                                memset(helpVar_name, '\0', 511);
                                m = 0;
                                break;
                            }
                        }

                        if (no == 1) {
                            var_name[indName] = malloc(strlen(helpVar_name) + 1);
                            strcpy(var_name[indName], helpVar_name);
                            indName++;
                            memset(helpVar_name, '\0', 511);
                            m = 0;
                            // take the var value 
                            copyind = ind + 1;
                            while (helparray[copyind] == ' ') {
                                copyind++;
                            }
                            m = 0;
                            while (helparray[copyind] != '\0' && helparray[copyind] != ' ') {
                                helpVar_value[m] = helparray[copyind];
                                copyind++;
                                m++;
                            }
                            var_value[indValue] = malloc(strlen(helpVar_value) + 1);
                            strcpy(var_value[indValue], helpVar_value);
                            indValue++;
                            m = 0;
                            memset(helpVar_value, '\0', size);
                        }
                    }
                    int con = -1;
                    for (int n = 0; helparray[n] != '\0'; n++) {
                        int q = 0;//index to help array
                        int inde = 0;
                        if (helparray[n] == '$') {
                            inde = n; // save the index of '$'
                            con++;// num of $ 
                            n++;
                            if (helparray[n] == ' ') {
                                while (helparray[n] == ' ' && helparray[n] != '\0') {
                                    n++;
                                }
                            }
                            int dec = heg;// num of '""'
                            while (helparray[n] != '\0' && helparray[n] != ' ' && helparray[n] != '$') {
                                if (heg > 0) { // if num of '""' >0
                                    while (n < inx[con] && helparray[n] != '\0' && helparray[n] != ' ') {// check if the text not  in the '""'
                                        help[q] = helparray[n];
                                        q++;
                                        n++;
                                    }
                                    break;
                                } 
                                else {
                                    help[q] = helparray[n];
                                    q++;
                                    n++;
                                }
                            }
                            dec--;
                            heg = dec;
                            q = 0;
                            int flag = 0;
                            while (var_name[q] != NULL) {
                                if (strcmp(help, var_name[q]) == 0) {
                                    flag = 1;
                                    strcpy(varia, helparray);
                                    memset(helparray, '\0', size);
                                    n = 0;
                                    int hel = 0;
                                    int va = 0;
                                    int coun = 0;
                                    while (varia[va] != '\0') {
                                        if (varia[va] == '$' && coun == 0) {
                                            coun++;
                                            va++;
                                            int leng = strlen(var_name[q]);
                                            va = va + leng;
                                            int val = 0;
                                            while (var_value[q][val] != '\0') {
                                                helparray[hel] = var_value[q][val];
                                                val++;
                                                hel++;
                                            }
                                        } 
                                        else {
                                            helparray[hel] = varia[va];
                                            hel++;
                                            va++;
                                        }
                                    }
                                    memset(varia, '\0', size);
                                    va = 0;
                                }
                                q++;
                            }
                            // if the var name that after the $  not found in my var names array i dont put any thing in place it
                            if (flag == 0) {
                                int va = 0;
                                int hel = 0;
                                strcpy(varia, helparray);
                                memset(helparray, '\0', size);
                                while (varia[va] != '\0') {
                                    if (varia[va] == '$') {
                                        va++;
                                        int leng = strlen(help);
                                        va = va + leng;
                                    }
                                    helparray[hel] = varia[va];
                                    hel++;
                                    va++;
                                }
                                memset(varia, '\0', size);
                                va = 0;
                            }
                        }
                        memset(help, '\0', size);
                        q = 0;
                    }


                    if (num != 3 && flaag == true) {
                        int end = 0;
                        int amp = 0;
                        int background = 0;
                        while (helparray[end] != '\0') {
                            end++;
                        }
                        if (helparray[end - 1] == '&' && an == 0) {
                            background = 1;
                            amp = 1;
                            // remove the '&'
                            if (helparray[end - 2] == ' ') {
                                helparray[end - 1] = '\0';
                                helparray[end - 2] = '\0';
                            } else {
                                helparray[end - 1] = '\0';
                            }
                        }
                        int bg = 0;
                        // check if i have bg  
                        char *resultt = strstr(helparray, "bg");
                        if (resultt != NULL) {
                            int indexx = (int) (resultt - helparray);
                            int m = 0;
                            while (helparray[m] != '\0' && helparray[m] == ' ') {
                                m++;
                            }
                            if (m == indexx) {
                                if ((helparray[m + 2] == '\0') || (helparray[m + 2] == ' ')) {
                                    bg = 1;
                                }
                            }
                        }
                        int tre = 0;
                        int index = 0;
                        for (int u = 0; helparray[u] != '\0'; u++) {
                            if (helparray[u] == '>') {
                                tre = 1;
                                index = u;
                            }
                        }
                        if (tre == 1 && oe == 0) {
                            int f = 0;
                            int c = 0;
                            int ce = 0;
                            int ar = 0;
                            int hel = index;
                            hel++;
                            if (helparray[hel] == ' ') {
                                hel++;
                                while (helparray[hel] == ' ' && helparray[hel] != '\0') {
                                    hel++;
                                }
                            }
                            while (helparray[hel] != '\0' && helparray[hel] != ' ') {
                                fileName[f] = helparray[hel];
                                f++;
                                hel++;
                            }
                            while (ce != index) {
                                command[c] = helparray[ce];
                                c++;
                                ce++;
                            }
                            c = 0;
                            while (command[c] != '\0') {
                                int he = 0;
                                while (command[c] == ' ' && command[c] != '\0') {
                                    c++;
                                }
                                while (command[c] != ' ' && command[c] != '\0') {
                                    if (command[c] == '"') {
                                        commandhelp[he] = command[c];
                                        he++;
                                        c++;
                                        while (command[c] != '"' && command[c] != '\0') {
                                            commandhelp[he] = command[c];
                                            he++;
                                            c++;
                                        }
                                        if (command[c] == '"') {
                                            commandhelp[he] = command[c];
                                            c++;
                                            he++;
                                        }
                                    } else {
                                        if (command[c] == ' ') {
                                            c++;
                                        } else {
                                            commandhelp[he] = command[c];
                                            c++;
                                            he++;
                                        }
                                    }
                                }
                                while (command[c] == ' ' && command[c] != '\0') {
                                    c++;
                                }
                                arr[ar] = malloc(strlen(commandhelp) + 1);
                                strcpy(arr[ar], commandhelp);
                                ar++;
                                he = 0;
                                memset(commandhelp, '\0', size);
                            }
                            arr[ar] = NULL;
                            int fd;
                            pid_t pid = MakeFork();
                            if (pid < 0) {
                                perror("fork");
                            }
                            if (pid == 0) {
                                fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                                if (dup2(fd, STDOUT_FILENO) == -1) {
                                    perror("dup");
                                }
                                makeExecute(arr);
                            } else {
                                wait(NULL);
                            }
                            memset(command, '\0', size);
                            memset(fileName, '\0', size);
                            for (int x = 0; arr[x] != NULL; x++) {
                                arr[x] = NULL;
                            }
                            ar = 0;
                        } else {
                            int boole = 0;
                            int numofpipes = 0;
                            for (int c = 0; helparray[c] != '\0'; c++) {
                                if (helparray[c] == '"') {
                                    c++;
                                    while (helparray[c] != '"' && helparray[c] != '\0') {
                                        c++;
                                    }
                                }
                                if (helparray[c] == '|') {
                                    numofpipes++;
                                    boole = 1;
                                }
                            }

                            if (boole == 1 && pi == 0) {
                                int pipes[numofpipes][2];
                                int k = 0;
                                for (int a = 0; a < numofpipes; a++) {
                                    if (pipe(pipes[a]) == -1) {
                                        perror("pipe failed");
                                        exit(EXIT_FAILURE);
                                    }
                                }
                                int t = 0;
                                int le = 0;
                                while (helparray[t] != '\0') {
                                    int p = 0;
                                    if (helparray[t] == ' ') {
                                        while (helparray[t] == ' ' && helparray[t] != '\0') {
                                            t++;
                                        }
                                    }
                                    while (helparray[t] != '|' && helparray[t] != '\0') {
                                        helppipe[p] = helparray[t];
                                        t++;
                                        p++;
                                    }
                                    if (helparray[t] == '|') {
                                        t++;
                                    }
                                    int a = 0;
                                    int o = 0;
                                    while (helppipe[a] != '\0') {
                                        while (helppipe[a] == ' ' && helppipe[a] != '\0') {
                                            a++;
                                        }
                                        while (helppipe[a] != ' ' && helppipe[a] != '\0') {
                                            if (helppipe[a] == '"') {
                                                helperr[o] = helppipe[a];
                                                o++;
                                                a++;
                                                while (helppipe[a] != '"' && helppipe[a] != '\0') {
                                                    helperr[o] = helppipe[a];
                                                    o++;
                                                    a++;
                                                }
                                                if (helppipe[a] == '"') {
                                                    helperr[o] = helppipe[a];
                                                    o++;
                                                    a++;
                                                }
                                            } else {
                                                if (helppipe[a] == ' ') {
                                                    a++;
                                                } else {
                                                    helperr[o] = helppipe[a];
                                                    o++;
                                                    a++;
                                                }
                                            }
                                        }
                                        while (helppipe[a] == ' ' && helppipe[a] != '\0') {
                                            a++;
                                        }

                                        helperr[o] = '\0';
                                        rightcom[le] = malloc(strlen(helperr) + 1);
                                        strcpy(rightcom[le], helperr);
                                        le++;
                                        o = 0;
                                        memset(helperr, '\0', size);
                                    }
                                    rightcom[le] = NULL;
                                    pid_t rightproc = MakeFork();
                                    if (rightproc < 0) {
                                        perror("fork");
                                    }
                                    if (rightproc == 0) {
                                        if (k != 0) {
                                            if (dup2(pipes[k - 1][0], STDIN_FILENO) < 0) {
                                                perror("child couldnt get input");
                                                exit(1);
                                            }
                                        }
                                        if (k != numofpipes) {
                                            if (dup2(pipes[k][1], STDOUT_FILENO) < 0) {
                                                perror("child couldnt output");
                                                exit(1);
                                            }
                                        }
                                        for (int a = 0; a < numofpipes; a++) {
                                            close(pipes[a][0]);
                                            close(pipes[a][1]);
                                        }
                                        makeExecute(rightcom);
                                    }
                                    k++;
                                    memset(helppipe, '\0', size + 1);
                                    p = 0;
                                    for (int x = 0; rightcom[x] != NULL; x++) {
                                        rightcom[x] = NULL;
                                    }
                                    le = 0;

                                }
                                for (int a = 0; a < numofpipes; a++) {
                                    close(pipes[a][0]);
                                    close(pipes[a][1]);
                                }
                                for (int a = 0; a < numofpipes; a++) {
                                    int status;
                                    wait(&status);
                                }
                                wait(NULL);
                            } else {
                                int a = 0;
                                int o = 0;
                                while (helparray[a] != '\0') {
                                    if (helparray[a] == ' ') {
                                        while (helparray[a] == ' ' && helparray[a] != '\0') {
                                            a++;
                                        }
                                    }
                                    while (helparray[a] != ' ' && helparray[a] != '\0') {
                                        if (helparray[a] == '"') {
                                            helper[o] = helparray[a];
                                            o++;
                                            a++;
                                            while (helparray[a] != '"' && helparray[a] != '\0') {
                                                helper[o] = helparray[a];
                                                o++;
                                                a++;
                                            }
                                            if (helparray[a] == '"') {
                                                helper[o] = helparray[a];
                                                o++;
                                                a++;
                                            }
                                        }
                                        if (helparray[a] != ' ' && helparray[a] != '\0') {
                                            helper[o] = helparray[a];
                                            o++;
                                            a++;
                                        }
                                    }
                                    if (helparray[a] == ' ') {
                                        while (helparray[a] == ' ' && helparray[a] != '\0') {
                                            a++;
                                        }
                                    }
                                    array[l] = malloc(strlen(helper) + 1);
                                    strcpy(array[l], helper);
                                    l++;
                                    memset(helper, '\0', size);
                                    o = 0;
                                }
                                array[l] = NULL;
                                id = MakeFork();
                                if (id < 0) {
                                    printf("error fork");
                                }
                                if (id == 0) {
                                    makeExecute(array);
                                } else {
                                    // num of arg an cammand
                                    if (background == 1 && an == 0) { // if i have '&'
                                        numofcom++;
                                        int v = 0;
                                        while (helparray[v] != '\0') {
                                            if (helparray[v] == '"') {
                                                v++;
                                                numofargs++;
                                                while (helparray[v] != '"') {
                                                    v++;
                                                }
                                                if (helparray[v] == '"') {
                                                    v++;
                                                }
                                            }
                                            if (helparray[v] != ' ' && helparray[v] != '\0' && helparray[v] != '>' &&
                                                helparray[v] != '|') {
                                                v++;
                                            }
                                            if (helparray[v] == ' ') {
                                                while (helparray[v] == ' ' && helparray[v] != '\0') {
                                                    v++;
                                                }
                                            }
                                            if (helparray[v] == '>') {
                                                numofargs++;
                                                v++;
                                                if (helparray[v] == ' ') {
                                                    while (helparray[v] == ' ' && helparray[v] != '\0') {
                                                        v++;
                                                    }
                                                }
                                                if (helparray[v] != ' ' && helparray[v] != '\0') {
                                                    numofargs++;
                                                    v++;
                                                    while (helparray[v] != '"' && helparray[v] != ' ' &&
                                                           helparray[v] != '\0' && helparray[v] != '>' &&
                                                           helparray[v] != '|') {
                                                        v++;
                                                    }
                                                }
                                            }
                                            if (helparray[v] != '"' && helparray[v] != ' ' && helparray[v] != '\0' &&
                                                helparray[v] != '>' && helparray[v] != '|') {
                                                numofargs++;
                                                while (helparray[v] != ' ' && helparray[v] != '\0' &&
                                                       helparray[v] != '>' &&
                                                       helparray[v] != '|') {
                                                    v++;
                                                }
                                            }
                                        }
                                        memset(helparray, '\0', size);
                                        i = 0;
                                        break;
                                    }
                                    else {
                                        int status;
                                        waitpid(id, &status, WUNTRACED);
                                    }
                                }

                                for (int x = 0; x < size; x++) {
                                    array[x] = NULL;
                                }
                                l = 0;
                            }
                        }
                        if (flagg == 0) {
                            numofcom++;
                            int v = 0;
                            while (helparray[v] != '\0') {
                                if (helparray[v] == '"') {
                                    v++;
                                    numofargs++;
                                    while (helparray[v] != '"') {
                                        v++;
                                    }
                                    if (helparray[v] == '"') {
                                        v++;
                                    }
                                }
                                if (helparray[v] != ' ' && helparray[v] != '\0' && helparray[v] != '>' &&
                                    helparray[v] != '|') {
                                    v++;
                                }
                                if (helparray[v] == ' ' || helparray[v] == '|') {
                                    while ((helparray[v] == ' ' || helparray[v] == '|') && helparray[v] != '\0') {
                                        v++;
                                    }
                                }
                                if (helparray[v] == '>') {
                                    numofargs++;
                                    v++;
                                    if (helparray[v] == ' ') {
                                        while (helparray[v] == ' ' && helparray[v] != '\0') {
                                            v++;
                                        }
                                    }
                                    if (helparray[v] != ' ' && helparray[v] != '\0') {
                                        numofargs++;
                                        v++;
                                        while (helparray[v] != '"' && helparray[v] != ' ' && helparray[v] != '\0' &&
                                               helparray[v] != '>' && helparray[v] != '|') {
                                            v++;
                                        }
                                    }
                                }
                                if (helparray[v] != '"' && helparray[v] != ' ' && helparray[v] != '\0' &&
                                    helparray[v] != '>' && helparray[v] != '|') {
                                    numofargs++;
                                    while (helparray[v] != ' ' && helparray[v] != '\0' && helparray[v] != '>' &&
                                           helparray[v] != '|') {
                                        v++;
                                    }
                                }
                            }
                        }
                    }


                    j++;
                    memset(helparray, '\0', size);
                    i = 0;
                }
                memset(argv, '\0', size);
                j = 0;
            }
        }
        for (int k = 0; var_name[k] !=NULL ; k++) {
            free(var_name[k]);
        }
        free(var_name);
        for (int k = 0; var_value[k] !=NULL; k++) {
            free(var_value[k]);
        }
        free(var_value);
        for (int k = 0; array[k] !=NULL ; k++) {
            free(array[k]);
        }
        free(array);
        for (int k = 0; arr[k] !=NULL ; k++) {
            free(arr[k]);
        }
        free(arr);
        free(helppipe);
        for (int k = 0; rightcom[k] !=NULL ; k++) {
            free(rightcom[k]);
        }
        free(rightcom);
        free(argv);
        free(helparray);
        free(helpVar_name);
        free(helpVar_value);
        free(help);
        free(varia);
        free(fileName);
        free(command);
        free(commandhelp);
        free(helperr);
        free(helper);
    }
}



