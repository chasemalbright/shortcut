#include "TCPRequestChannel.h"

using namespace std;

//constructor
TCPRequestChannel::TCPRequestChannel (const std::string _ip_address, const std::string _port_no) {
    if(_ip_address != ""){
        // cread sockaddre server
        struct sockaddr_in server;
        

        //create the socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
        //create the server information
        memset(&server, 0, sizeof(server)); 

        server.sin_family = AF_INET; // family

        server.sin_port = htons(stoi(_port_no)); // port

        inet_pton(AF_INET, _ip_address.c_str(), &(server.sin_addr)); // man pton
        
        connect(sockfd, (struct sockaddr*)&server, sizeof(server)); //connect server
    } else{
        struct sockaddr_in server; // create the server
        
        sockfd = socket(AF_INET, SOCK_STREAM, 0); // socket start
        
        //get from class notes the info
        memset(&server, 0, sizeof(server)); 
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_family = AF_INET;
        server.sin_port = htons(stoi(_port_no));

        //binding the socket
        
        bind(sockfd, (struct sockaddr *) &server, sizeof(server));

        //listen on port
        listen(sockfd, 1024);
    }
}

TCPRequestChannel::TCPRequestChannel (int _sockfd) {
    sockfd = _sockfd;
}

TCPRequestChannel::~TCPRequestChannel () {
    close(sockfd);
}

int TCPRequestChannel::accept_conn () {
    struct sockaddr_storage add;
    socklen_t len = sizeof(add);
    
    return accept(sockfd, (struct sockaddr*)&add, &len);
}

int TCPRequestChannel::cread (void* msgbuf, int msgsize) {
    return read(sockfd, msgbuf, msgsize);
}

int TCPRequestChannel::cwrite (void* msgbuf, int msgsize) {
    return write(sockfd, msgbuf, msgsize);
}
