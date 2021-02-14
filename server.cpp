#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <sstream>

int main(int argc, char** argv){

    //Socket creation via TCP, IPv4
    int socket = socket(AF_INET, SOCK_STREAM,0)

    //Binding
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1025);
    addr.sin_addr.s_addr = inet_addr(localhost);
    memset(addr.sin_zero,'\0',sizeof(addr.sin_zero));

    //Checking for binding errors
    if (bind(socket, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Error Occured within binding");
        return 1;
    }

    //Listening Procedure and checking for listening error
    if (listen(socket, 1) == -1) {
        perror("Error Occured within listening");
        return 3;
    }

    //Connection
    struct sockaddr_in client;
    socklen_t clientAddSize = sizeof(client);
    int clientSocket = accept(socket,(struct sockaddr*)&client,&clientAddSize);

    //Error checking for acceptance
    if (clientSocket == -1)
    {
        perror("Error Occured within acceptance");
        return 4;
    }
    

}