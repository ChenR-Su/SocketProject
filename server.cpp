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
#include <csignal>
#include <sys/select.h>

void signal_handler(int signl_num){
  std::cout << "test signal";
  exit(0);
}


int main(int argc, char *argv[])
{

  signal(SIGQUIT,signal_handler);
  signal(SIGTERM,signal_handler);
  //Define the file size to max of 100MB
  int SIZE = 521;
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
  while(1){
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
    char buffer[SIZE];
    char fileName[50];
    fd_set readfds;
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    FD_ZERO(&readfds);
    FD_SET(clientSockfd, &readfds);
    


    sprintf(fileName,"%d.file",conneCount);
    conneCount++;
    FILE *file = fopen(fileName, "w");
    bzero(buffer,SIZE);
    int f_block_size = 0;
    int transfer = 0;
    while(transfer == 0){
      while(f_block_size = recv(clientSockfd,buffer,SIZE,0)){
        int time = select(clientSockfd + 1,&readfds,NULL,NULL,&timeout);
        if(f_block_size == -1){
          std::cerr << "ERROR in Receiving";
          break;
        }
        if(time == 0){
          freopen(NULL, "w", file);
          char timeOutBuffer[] = "ERROR";
          fwrite(timeOutBuffer,sizeof(char),sizeof(timeOutBuffer),file);
        }
        else{
          int write = fwrite(buffer,sizeof(char),f_block_size,file);
        if(write < f_block_size){
          std::cerr << "ERROR in Writing";
          break;
        }
        if(f_block_size == 0){
          close(clientSockfd);
          break;
        }
        bzero(buffer,SIZE);
        }
        
      }
      transfer = 1;
      fclose(file);
    }
  }
  



  return 0;
}