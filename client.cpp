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

int main(int argc, char *argv[])
{
  //Set the max size of the file;
  int maxFileSize = 521;
  // create a socket using TCP IP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  int portNum = atoi(argv[2]);
  printf("%d\n",portNum);
  //Test for valid port Number
  if(portNum <= 1023){
    std::cerr << "ERROR: invalid port number";
    return 1;
  }


  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(portNum); 
  if(argv[1] = "localhost")
     serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  else
     serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

  memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

  // connect to the server
  if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
    perror("Error in Connection: ");
    return 1;
  }

  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  if (getsockname(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
    perror("getsockname");
    return 1;
  }

  char ipstr[INET_ADDRSTRLEN] = {'\0'};
  inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
  std::cout << "Set up a connection from: " << ipstr << ":" <<
    ntohs(clientAddr.sin_port) << std::endl;

  //Determine if the file is capaple of sending
  std::ifstream fileCheck(argv[3],std::ios::binary);
  fileCheck.seekg(0,std::ios::end);
  int fileSize = fileCheck.tellg();
  std::cout<< "fileSize is " << fileSize<<std::endl;
  if(fileSize > maxFileSize){
    std::cerr << "File is larger than the maximum size";
    return 1;
  }

  // send/receive data to/from connection
  char data[maxFileSize];
  FILE *file =  fopen(argv[3],"r");
  if(file == NULL){
    std::cerr << "ERROR: File not found";
    return 1;
  }
  bzero(data,maxFileSize);
  int f_block_size;
  while ((f_block_size = fread(data,sizeof(char),maxFileSize,file))>0)
  {
    if(send(sockfd,data,f_block_size,0) < 0){
      std::cerr << "ERROR: Sending Failed";
      break;
    }
    bzero(data,maxFileSize);
  }
  
  
  
  
  close(sockfd);

  


  return 0;
}