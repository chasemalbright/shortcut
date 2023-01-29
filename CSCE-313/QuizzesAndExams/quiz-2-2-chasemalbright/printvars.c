#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include<stdio.h>

char *custom_env[] = {"USER=Pikachu", "PATH=/home/chase/csce313/quiz-2-2-chasemalbright/echoall", NULL};

int main(void)
{
    printf("Running parent process PID: %d\n\n", getpid());
    pid_t pid = fork();

    if (pid < 0)
    {
        printf("Error in forking \n");
        return 1;
    } else if(pid == 0){ //child process

        printf("Running child process PID: %d\n", getpid());


        execle("/home/chase/csce313/quiz-2-2-chasemalbright/echoall", "echoall", "Bandicoot", "Pacman",NULL,custom_env);


    } else { // parent
        wait(NULL);
    }

    pid = fork();
    if (pid < 0)
    {
        printf("Error in forking \n");
        return 1;
    } else if(pid == 0){ //child process

        printf("Running child process PID: %d\n", getpid());


        execlp("./echoall", "echoall", "Spyro", NULL);


    } else { // parent
        wait(NULL);

    }


    
    exit(0);
}