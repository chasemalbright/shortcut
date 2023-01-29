#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main(int argc, char** argv) {
    int n = 1, opt;
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
        case 'n':
            n = atoi(optarg);
            break;
        }
    }

    /*
    1. TODO: fork n child processes and run p1-helper on each using execvp
        > note: we need to synchronize the child processes to print in the desired order
        > note: sleep(..) does not guarantee desired synchronization
        > note: check "raise" system call
        > note: check "WUNTRACED" flag for "waitpid"
    */

    vector<pid_t> processes;

    for (int i = 0; i < n; i++){

        pid_t pid = fork();


        if (pid == 0){
            char * args[] = {nullptr};
            execvp("./p1-helper",args);
        } else {
            processes.push_back(pid);
            kill(pid,SIGSTOP);
        }
    }








    
    /* 
    2. TODO: print children pids 
    */
   for (auto p : processes){
    printf("%d ", p);
   }
   printf("\n");


    fflush(stdout);             // DO NOT REMOVE: ensures the first line prints all pids
    std::sort(processes.begin(), processes.end(),greater<pid_t>()); // descending order

    for (auto p : processes){
        kill(p, SIGCONT);
        waitpid(p,nullptr,0);
    }
    /* 
    3. TODO: signal children with the reverse order of pids 
        > note: take a look at "kill" system call 
    */
    
    printf("Parent: exiting\n");

    return 0;
}