#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <fstream>


int main(int argc, char *argv[])
{
  //Define the file size to max of 100MB
  int SIZE = 100000000;
  int conneCount = 1;
  // create a socket using TCP IP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  //Reuse of Address
  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    return 1;
  }


  //Assigning port number
  int portNum = atoi(argv[1]);
  if(portNum <= 1023){
    std::cerr << "ERROR: Port Number Out Of Range";
    return 1;
  }

  //Designing the directory
  char currentWorkingDir[260];
  if(getcwd(currentWorkingDir,sizeof(currentWorkingDir))== NULL){
    perror("Error Occured While Generating Current Directory");
    return 1;
  }
  printf("%s",currentWorkingDir);

  int changeDir = chdir(argv[2]);
  if(changeDir != 0){
    perror("Error Occured While Changing to Destination Directory");
    return 1;
  }


  // bind address to socket
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;

  addr.sin_port = htons(portNum);    
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("Error Occur Within Binding");
    return 1;
  }

  // set socket to listen status
  if (listen(sockfd, 1) == -1) {
    perror("Error Occur Within Listen");
    return 1;
  }
  else
    printf("Listening...\n");

  // accept a new connection
  struct sockaddr_in clientAddr;
  socklen_t clientAddrSize = sizeof(clientAddr);
  int clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize);

  if (clientSockfd == -1) {
    perror("Error Occur Within Accepting");
    return 1;
  }
  else
    printf("Connection Accepted\n");

  char ipstr[INET_ADDRSTRLEN] = {'\0'};
  inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
  std::cout << "Accept a connection from: " << ipstr << ":" <<
    ntohs(clientAddr.sin_port) << std::endl;

  // read/write data from/into the connection
  char buffer[SIZE] = {0};
  char fileName[50];
  int n = sprintf(fileName,"%d.file",conneCount);
  std::ofstream file;
  file.open(fileName);
  while(1){
    if(recv(clientSockfd,buffer,sizeof(buffer),0) <= 0){
      close(clientSockfd);
      break;
      return 0;
    }
    file << buffer << std::endl;
    file.close();
    conneCount++;
  }
  printf("%d",conneCount);



  return 0;
}