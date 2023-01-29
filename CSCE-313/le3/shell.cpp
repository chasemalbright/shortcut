/****************
LE2: Basic Shell
****************/
#include <unistd.h> // pipe, fork, dup2, execvp, close
#include <sys/wait.h> // wait
#include "Tokenizer.h"
#include <iostream>
using namespace std;

int main () {
    
    dup(1); // dups right here
    dup(2);

    while(true){
        cout << "Enter Commands:  " << endl;
        string in; // we get input
        getline(cin, in);

        if (in == "exit"){
            break;
        }

        Tokenizer token(in); // parse input

        auto commandLength = token.commands.size(); // constant for size

        for (long unsigned int i = 0; i < commandLength; i++){

            int fd[2]; // file descriptor
            pid_t pid; // pid var

            pipe(fd); // start pipe

            pid = fork(); // fork 

            if (pid == 0){ // if chile\d

                if (i < commandLength-1){
                    dup2(fd[1], STDOUT_FILENO); // for last command
                } 

                close(fd[0]); // close channel

                std::vector<char*> argc; // vector of args

                for(auto const& cmd : token.commands[i]->args){ // go through all commands in vector

                    argc.push_back(const_cast<char*> (cmd.c_str())); // push each command

                }

                argc.push_back(nullptr); // ERROR here need to push nullptr
                 
                execvp(argc[0], argc.data()); // exec commands 


            } else {

                dup2(fd[0], STDIN_FILENO); // dup for the parent process

                close(fd[1]); // close channel

                if (i == commandLength -1){
                    waitpid(pid,nullptr,0); // terminate processes
                }
            }

        }

        dup2(STDOUT_FILENO,3); // dups from marys vids
        dup2(STDIN_FILENO,4);

    }
}