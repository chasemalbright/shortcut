#include <iostream>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctime>

#include <vector>
#include <string>

#include "Tokenizer.h"

// all the basic colours for a shell prompt
#define RED     "\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE	"\033[1;34m"
#define WHITE	"\033[1;37m"
#define NC      "\033[0m"

using namespace std;

int main () {

    vector<int> ids;
    char cwd[2000];

    string currDir = getcwd(cwd, sizeof(cwd));
    string prevDir = getcwd(cwd, sizeof(cwd));




    dup(1); // dups right here
    dup(2);
    for (;;) {


        for (auto id: ids){ // handling background process
            waitpid(id, nullptr, WNOHANG);
        }



        getcwd(cwd, sizeof(cwd));
        time_t curr_time; // time
	    curr_time = time(NULL);

	    char *tm = ctime(&curr_time);

        string s(tm);

        if (!s.empty() && s[s.length()-1] == '\n') { // remove new line
            s.erase(s.length()-1);
        }
        

    
        

        // need date/time, username, and absolute path to current dir
        cout << YELLOW << "Shell$" << BLUE << " ~" << BLUE << getenv("USER") << "~" << GREEN  << " " << s << WHITE << cwd << "  ";
        
        // get user inputted command
        string input;
        getline(cin, input);

        if(input == ""){
            continue;
        }

        if (input == "exit") {  // print exit message and break out of infinite loop
            cout << RED << "Now exiting shell..." << endl << "Goodbye" << NC << endl;
            break;
        }
        

        // get tokenized commands from user input
        Tokenizer token(input);

        

        if (token.commands[0]->args[0] == "cd"){ // cd commands
            if (token.commands[0]->args[1] == "-"){
                cout << "GO FORWARD" << endl;
                chdir(prevDir.c_str());
                prevDir = currDir;
                continue;
            } else {
                cout << "GO BACK " << endl;
                chdir((token.commands[0]->args[1]).c_str());
                prevDir = currDir;
                continue;
            }
        }

        




        if (token.hasError()) {  // continue to next prompt if input had an error
            continue;
        }

        // changing directories


        auto commandLength = token.commands.size(); // constant for size

        for (long unsigned int i = 0; i < commandLength; i++){

            string filename = token.commands[i]->out_file;
            string filenameIn = token.commands[i]->in_file;

            int fd[2]; // file descriptor
            pid_t pid; // pid var

            pipe(fd); // start pipe

            pid = fork(); // fork 

            if (pid == 0){ // if chile\d



                if (i < commandLength - 1){
                    dup2(fd[1], STDOUT_FILENO); // for last command
                } 


                if (filename != ""){ // check for out file
                    int fd = open(filename.c_str(), O_WRONLY|O_CREAT,S_IWUSR|S_IRUSR);
                    dup2(fd,1);
                    close(fd);
                }

                if (filenameIn != "") { // check for in file
                    int fd = open(filenameIn.c_str(), O_RDONLY,S_IWUSR|S_IRUSR);
                    dup2(fd,0);
                    close(fd);
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

                if (i == commandLength - 1){

                    if (!token.commands[i]->isBackground()){
                        waitpid(pid,nullptr,0); // terminate processes
                    } else {
                        ids.push_back(pid);
                    }
                }


                // if(commandLength == 1){
                //     dup2(3, 0);
                // }
            }
            close(fd[0]);
            close(fd[1]);


        }
        dup2(3, 0);

    }
    dup2(STDOUT_FILENO,3); // dups from marys vids
    dup2(STDIN_FILENO,4);


}
