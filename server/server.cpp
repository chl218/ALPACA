#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {
   int sockfd, portno; 
   socklen_t newsockfd;
   socklen_t clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int n;
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 5001;
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);



   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
   

   inet_ntop(AF_INET, &(serv_addr.sin_addr.s_addr), buffer, 256);
   printf("hostname:%s port:%d\n", buffer, portno);

   while(1) {
      /* Now start listening for the clients, here process will
       * go in sleep mode and will wait for the incoming connection
       */
      listen(sockfd,5);
      clilen = sizeof(cli_addr);
      /* Accept actual connection from the client */
      newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
      if (newsockfd < 0) {
         perror("ERROR on accept");
         exit(1);
      }

      /* If connection is established then start communicating */
      bzero(buffer, 256);
      n = read(newsockfd, buffer,255);
      if (n < 0) {
         perror("ERROR reading from socket");
         exit(1);
      }
      printf("Here is the message: %s", buffer);
      

      int recv[6] = {0};


      char *token = strtok(buffer, " ");
      for(int i = 0; i < 6; i++) {
         recv[i] = atoi(token);
         token = strtok(NULL,  " ");
      }

      int screenWidth  = recv[0];
      int screenHeight = recv[1];

      int xCrd = recv[2];
      int yCrd = recv[3];

      int isOn     = recv[4];
      int gradient = recv[5];

      /* Write a response to the client */
      char msg[] = "I got your message\n";
      n = write(newsockfd, msg, strlen(msg));
      if (n < 0) {
         perror("ERROR writing to socket");
         exit(1);
      }
   }
   
   return 0;
}
