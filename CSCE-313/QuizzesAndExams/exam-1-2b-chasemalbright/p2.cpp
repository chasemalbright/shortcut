#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#define MAX_MESSAGE 256

long long unsigned int hash(int seed, char* buf, int nbytes) {
    long long unsigned int H = seed; 
    for (int i = 0; i < nbytes; ++i) 
        H = H * 2 * buf[i] + 1;
    return H;
}

int main(int argc, char** argv) {

    if (argc < 2){
        printf("there is an error");
    }
	
	// TODO: create pipe 
    int fd1[2];
    //int fd2[2];

    pipe(fd1);
    //pipe(fd2);

    std::string input = argv[1]; // .cstr()
    char buf[MAX_MESSAGE];


    int pid = fork();

    if (pid == 0) {
        // TODO: read from parent
        
        read(fd1[0],buf,sizeof(buf));
        close(fd1[0]);

        // TODO: compute hash 
        long long unsigned int h = hash(getpid(), buf, sizeof(buf));

        // TODO: send hash to parent 

        write(fd1[1],&h,sizeof(&h));
        close(fd1[1]);

    }
    else {
        // TODO: write to child 

        write(fd1[1],buf,sizeof(buf));
        close(fd1[1]);

        waitpid(pid,nullptr,0);
        
        // TODO: get hash from child 
        long long unsigned int hrecv;
        read(fd1[0],&hrecv,sizeof(&hrecv));
        close(fd1[0]);
        // TODO: calculate hash on parent side
        long long unsigned int h = hash(hrecv, buf, sizeof(buf));
		
		// print hashes; DO NOT change
        printf("%llX\n", h);
        printf("%llX\n", hrecv);
    }

    return 0;
}