Mini shell with pipes and signals
Authored by Zenab Mahmoud
212384390

==Description==
In this program , we implement a simple shell in the C language under the Linux operating system .The shell display a prompt to the user , and it read the commands and send them to the operating system for execution .

In this program we use one process ,we use array to put every command in one element .
 ** To pipes :
we use one array to take one command and make dup2 if it read or write and excute to this command.

** > :
we open file with the name that user input and write the execute of the command that before '>'.

** & :
if see '&' in the end of the sentence , it do the execute of the comand in the background and the father didnt wait his son to finish his process , it print the prompt.

==Functions==
-MakeFork-:
to make fork.

-makeExecute-
to execute the command.

-child_fun-
this the function to the signal child , it free the childs when they end them process in background.

-sigtstp_handler-
this function to signal tstp, when the user put ctrl-z it stopped the process and save the pid to the last child.



==Program Files==
ex2.c - the mini shell program

==How to compile?==
compile: gcc ex2.c -o ex2
run: ./ex2

==Input:==
The commands in the shell 

==Output:==
the execuate of the commands.



