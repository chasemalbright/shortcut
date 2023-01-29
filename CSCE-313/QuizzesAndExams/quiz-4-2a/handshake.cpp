#include <iostream>
#include <mutex>
#include <vector>
#include <thread> // thread, join
#include <semaphore.h>

using namespace std;
// include additional necessary headers

sem_t querySem{0}; // semaphors
sem_t responseSem{0};

void query(string s, int i) {
    // Should print: the print number (starting from 0), "SYN", and the three dots "..."
    
    for(int k = 0; k < i; k++){
        sem_wait(&querySem); // wait
        cout << "[" << k << "] " << s << " ... ";
        sem_post(&responseSem); // send
    }
}

void response(string s, int i) {
    for(int k = 0; k < i; k++){
        sem_wait(&responseSem); // wait
        std::cout << s << endl;
        sem_post(&querySem); // send
    }

}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: ./handshake <count>\n");
        exit(1);
    }

    /**
     * Steps to follow:
     * 1. Get the "count" from cmd args
     * 2. Create necessary synchronization primitive(s)
     * 3. Create two threads, one for "SYN" and the other for "ACK"
     * 4. Provide the threads with necessary args
     * 5. Update the "query" and "response" functions to synchronize the output
    */

   int count = atoi(argv[1]);

//    vector<thread> q;

//    for (int i = 0; i < count; i++){
//     q.push_back(thread(query,"SYN", i));
//    }

//    vector<thread> r;
//    for (int i = 0; i < count; i++){
//     q.push_back(thread(response, "ACK"));
//    }


//    for (int i = 0; i < count; i++){
//     q.at(i).join();
//    }

//    for (int i = 0; i < count; i++){
//     r.at(i).join();
//    }

    sem_post(&querySem);
    thread quer(query,"SYN",count); // threading
    thread res(response,"ACK",count);

    quer.join(); // join
    res.join();

    sem_destroy(&responseSem); //DESTROY
    sem_destroy(&querySem);


   
    return 0;
}