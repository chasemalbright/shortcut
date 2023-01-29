#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h> 

using namespace std;

int main(int argc, char *argv[])
{
    int option = 0; // default option: execute the command ls -l and terminate normally

    // Get flag from command line
    int opt;
    while ((opt = getopt(argc, argv, "n:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            option = atoi(optarg);
            break;
        }
    }


    pid_t pid = fork();
    pid_t parent = getpid();

    if (pid < 0)
    {
        cout << "Fork failed" << endl;
        return 1;
    }
    else if (pid == 0)
    {
        cout << "Hello from the child process!" << endl;
        cout << "THE PARENT PID value: " << parent << endl;

        if (option % 2 == 0) // if the option number is even, execute the command ls -l and terminate normally
        {
            std::cout << "The child process will execute the command: ls -l after 6 seconds" << std::endl;
            sleep(6);

            char * args[] = {const_cast<char *>("ls - l")};

            if (execvp(args[0], args) < 0){
                perror("execvp");
			    exit(EXIT_FAILURE);
            }
        }
        else // if the option number is odd, terminate with a kill signal
        {
            std::cout << "The child process is exiting" << std::endl;
            kill(getpid(), SIGINT);
        }
    }
    else if (pid > 0)
    {
        int status;

        

        cout << "\nHello from the parent process!" << endl;

        cout << "The child process ID is " << pid << endl;

        /* TODO: PRINT THE EXIT STATUS OF THE CHILD PROCESS BASED waitpid().
        MAKE SURE TO PASS BY REFERENCE THE STATUS VARIABLE TO THE SECOND PARAMETER OF waitpid()
        IF WIFEXITED, PRINT THE MESSAGE "The child process exited normally" WITH ENDLINE
        IF WIFSIGNALED, PRINT THE MESSAGE "The child process exited due to the kill signal" WITH ENDLINE
        */
       if (WIFEXITED(status)){
        cout << "The child process exited normally" << endl;
       } else if(WIFSIGNALED(status)) {
        cout << "The child process exited due to the kill signal" << endl;
       }
       waitpid(pid,&status, 0);

    }

    return 0;
}