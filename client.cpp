#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
  //Set the max size of the file;
  int maxFileSize = 521;
  // create a socket using TCP IP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  //Getting Port number from cmd arg
  int portNum = atoi(argv[2]);
  printf("%d\n",portNum);
  //Test for valid port Number
  if(portNum <= 1023){
    std::cerr << "ERROR: invalid port number";
    exit(1);
  }

  //Setting potential host
  std::string local = "localhost";
  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(portNum); 
  //Transfering localhost to local IP Addr, or take in other IP Addr
  if(argv[1] == local)
     serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  else
     serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

  memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

  //Setting Timeout Structure
  fd_set fdset;
  struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;


  //Setting Socket to NONBLOCK 
  fcntl(sockfd,F_SETFL, O_NONBLOCK);

  // Initial Connection
  int connection = connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
  if (connection < 0) {
    //In Progress
    if(errno == EINPROGRESS){
      FD_ZERO(&fdset);
      FD_SET(sockfd, &fdset);
      //TimeoutLimit
      connection = select(sockfd + 1,NULL,&fdset, NULL, &timeout);
    }
  }

  //If still unable to connect
  if(connection == -1){
      std::cerr << "ERROR: ";
      exit(1);
  }
  /*if(time <= 0){
    std::cerr << "ERROR: connection timeout";
    close(sockfd);
    exit(1);
  }*/


  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  if (getsockname(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
    perror("getsockname");
    exit(1);
  }

  char ipstr[INET_ADDRSTRLEN] = {'\0'};
  inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
  std::cout << "Set up a connection from: " << ipstr << ":" <<
    ntohs(clientAddr.sin_port) << std::endl;


  //Looking for File;
  char data[maxFileSize];
  FILE *file =  fopen(argv[3],"r");
  if(file == NULL){
    std::cerr << "ERROR: File not found";
    exit(1);
  }
  //Zero outt the buffer
  bzero(data,maxFileSize);
  int fileSegment;

  //Read and write while there still containsed contransferred data
  while ((fileSegment = fread(data,sizeof(char),maxFileSize,file))>0)
  {
    //Test for connection timeout;
    connection = select(sockfd + 1,NULL,&fdset, NULL, &timeout);
    if(connection == 0){
    std::cerr << "ERROR: connection timeout During File Transfer";
    //close(sockfd);
      exit(1);
    }
    else if(connection == -1){
      std::cerr << "ERROR: Some error has occured";
      exit(1);
    }
    //Test for sending Issue
    if(send(sockfd,data,fileSegment,0) < 0){
      std::cerr << "ERROR: Sending Failed";
      exit(1);
    }
    //Zero out the filter after
    bzero(data,maxFileSize);
  }
  //Wait 10 Sec after file transfer to close the socket
  connection = select(sockfd + 1,NULL,&fdset, NULL, &timeout);
    if(connection == 0){
    std::cerr << "ERROR: connection timeout After File Transfer";
    //close(sockfd);
      exit(1);
    }
    else if(connection == -1){
      std::cerr << "ERROR: Some error has occured";
      exit(1);
    }

  close(sockfd);
  return 0;
}