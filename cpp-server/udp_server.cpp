//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

//-----------------------------------------------------------------------------
#include <wiringPi.h>
#include <softPwm.h>
#include "interface.h"

#define WIDTH      16
#define HEIGHT     9

#define DEBUG 1

#define PORTNO 5001

void initializePins() {
   softPwmCreate(PIN_00, 0x00, 0xFF);
   softPwmCreate(PIN_01, 0x00, 0xFF);
   softPwmCreate(PIN_02, 0x00, 0xFF);
   softPwmCreate(PIN_03, 0x00, 0xFF);
   softPwmCreate(PIN_04, 0x00, 0xFF);
   softPwmCreate(PIN_05, 0x00, 0xFF);
   softPwmCreate(PIN_06, 0x00, 0xFF);
   softPwmCreate(PIN_07, 0x00, 0xFF);
   softPwmCreate(PIN_08, 0x00, 0xFF);
   softPwmCreate(PIN_09, 0x00, 0xFF);
   
   softPwmCreate(PIN_10, 0x00, 0xFF);
   softPwmCreate(PIN_11, 0x00, 0xFF);
   softPwmCreate(PIN_12, 0x00, 0xFF);
   softPwmCreate(PIN_13, 0x00, 0xFF);
   softPwmCreate(PIN_14, 0x00, 0xFF);
   softPwmCreate(PIN_15, 0x00, 0xFF);
   softPwmCreate(PIN_16, 0x00, 0xFF);
   softPwmCreate(PIN_17, 0x00, 0xFF);
   softPwmCreate(PIN_18, 0x00, 0xFF);
   softPwmCreate(PIN_19, 0x00, 0xFF);

   softPwmCreate(PIN_20, 0x00, 0xFF);
   softPwmCreate(PIN_21, 0x00, 0xFF);
   softPwmCreate(PIN_22, 0x00, 0xFF);
   softPwmCreate(PIN_23, 0x00, 0xFF);
   softPwmCreate(PIN_24, 0x00, 0xFF);
   softPwmCreate(PIN_25, 0x00, 0xFF);
   softPwmCreate(PIN_26, 0x00, 0xFF);
   softPwmCreate(PIN_27, 0x00, 0xFF);
   softPwmCreate(PIN_28, 0x00, 0xFF);
   softPwmCreate(PIN_29, 0x00, 0xFF);

   softPwmCreate(PIN_30, 0x00, 0xFF);
   softPwmCreate(PIN_31, 0x00, 0xFF);

}

int rangeTransform(int oldVal, int oldRange, int newRange) {
   // OldRange = (OldMax - OldMin)  
   // NewRange = (NewMax - NewMin)  
   // NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin
   //printf("\trangeTransform: oldVal %d oldRange: %d newRange: %d\n", oldVal, oldRange, newRange);
   return (((oldVal - 0) * newRange) / oldRange) + 0;
}

void drawLED(int xCrd, int yCrd, int gradient) {
   softPwmWrite(xCrd * yCrd,  gradient);
}


int main(int argc, char *argv[]) {

   if(DEBUG) printf("Hello ALPACA\n");

   initializePins();

   int sockfd; 
   socklen_t newsockfd, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int n;

   // Initialize socket structure 
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(PORTNO);


   // First call to socket() function
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   // Now bind the host address using bind() call.
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
   
   //inet_ntop(AF_INET, &(serv_addr.sin_addr.s_addr), buffer, 256);

   // {screenWidth, screenHeight, xCrd, yCrd, isOn, gradient}
   int prevState[6] = {0, 0, 0, 0, 1, 0};
   int currState[6] = {0};

   while(1) {
      // Now start listening for the clients, here process will
      // go in sleep mode and will wait for the incoming connection
      listen(sockfd, 5);
      clilen = sizeof(cli_addr);

      // Accept actual connection from the client
      newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
      if (newsockfd < 0) {
         perror("ERROR on accept");
         exit(1);
      }

      // If connection is established then start communicating
      bzero(buffer, 256);
      n = read(newsockfd, buffer,255);
      if (n < 0) {
         perror("ERROR reading from socket");
         exit(1);
      }
      printf("Here is the message: %s", buffer);
      
      // tokenize rece msg
      char *token = strtok(buffer, " ");
      for(int i = 0; i < 6; i++) {
         currState[i] = atoi(token);
         token = strtok(NULL,  " ");
      }

      // transform range to led grid resolution
      currState[2] = rangeTransform(currState[2], currState[0],  WIDTH);
      currState[3] = rangeTransform(currState[3], currState[1], HEIGHT);
      currState[5] = rangeTransform(currState[5], 100, 0xFF);
 
      if(DEBUG) {
         printf("after rangeTransform: ");
         for(int i = 0; i < 5; i++) {
            printf("%2d ", currState[i]);
         }
         printf("0x%x\n", currState[5]);
      }

      // turn current led on, turn previous led off
      if(currState[4]) {
         drawLED(prevState[2], prevState[3], prevState[5]);
         drawLED(currState[2], currState[3], currState[5]);
      }
      else {
         drawLED(prevState[2], prevState[3], 0x00);
      }
      
      // save current state informations
      for(int i = 0; i < 6; i++) {
         prevState[i] = currState[i];
      }

      /* Write a response to the client */
      // char msg[] = "I got your message\n";
      // n = write(newsockfd, msg, strlen(msg));
      // if (n < 0) {
      //    perror("ERROR writing to socket");
      //    exit(1);
      // }
   }
   
   return 0;
}
