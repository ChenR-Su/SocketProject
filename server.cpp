#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#define SIZE = 100000000 //~100mb

void writeFile(int client){

}



int main(int argc, char** argv){

    //Socket creation via TCP, IPv4
    int socket = socket(AF_INET, SOCK_STREAM,0)

    //Binding
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;


    /*Checking to see if the port falls in the range 0-1024
      Prints error message if it does, otherwise make such port
      as a the port for file transfer  */
    if(atoi(argv[0]) >= 0 && argv[0] <=1024){
        std::cerr << "ERROR: Desired Port entered is not Operational";
        return 1;
    }
    else
        addr.sin_port = htons(argv[0]);

    addr.sin_addr.s_addr = inet_addr(localhost);
    memset(addr.sin_zero,'\0',sizeof(addr.sin_zero));

    //Checking for binding errors
    if (bind(socket, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Error Occured within binding");
        return 2;
    }
    else
        printf("Binding Success");

    //Listening Procedure and checking for listening error
    if (listen(socket, 1) == -1) {
        perror("Error Occured within listening");
        return 3;
    }
    else
        printf("Listening For Future Request");

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
    else
        printf("Successly Accepted Connection")
    
    //Printing the establishment of connection
    char ipstr[INET_ADDRSTRLEN] = {'\0'};
    inet_ntop(client.sin_family, &client.sin_addr,ipstr,sizeof(ipstr));
    std::count << "Connection Accepted from:  " << ipstr << ":" <<
        ntohs(client.sin_port) << std::endl;

    
    /*Checking for file size
    File *file = fopen(argv[0],"w");
    fseek(file,0,SEEK_END);
    int fileSize = ftell(file);
    rewind(file);
    if(fileSize > 100000000){
        std::cerr << "ERROR: File Size is Out of Range";
        return 1;
    }*/
    
    /*
    //Writing file
    int size = 100000000;
    char buffer[size] = {0};
    char* directory = str.substr(argv[1],0);
    //Store the current directory, change/make the target directory to
    //save the file, then change back to the old directory
    char* initialDir[PATH_MAX]; 
    getcwd(initialDir,sizeof(initialDir));
    //Create the directory
    int makeCorrespondDir = !mkdir(directory, 0777))
    while(recv(client, buffer,size,0) > 0){
        fprintf(file, "%s",buffer)
    }
    //@TODO change the name of the file


    //Change the directory back to the original
    int dirChange = chdir(initialDir);
    */
    return 0;


}