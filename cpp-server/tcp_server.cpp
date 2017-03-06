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

#define DEBUG 0

#define PORTNO 5001

#define PARAM_LEN 8

// {screenWidth, screenHeight, xCrd, yCrd, isLEDOn, LEDgradient, demo, DemoAmount}
#define SCREEN_WIDTH  100
#define SCREEN_HEIGHT 100
#define DEMO 7

#define NUM_OF_ROWS 6 // # of rows in LED matrix    (full = 9)
#define NUM_OF_COLS 4 // # of columns in lED matrix (full = 16)

// sets up all the row and column pins to output
void initializePins() {
    pinMode(PIN_C_B0, OUTPUT);
    pinMode(PIN_C_B1, OUTPUT);
    pinMode(PIN_C_B2, OUTPUT);
    pinMode(PIN_C_B3, OUTPUT);
    pinMode(PIN_C_B4, OUTPUT);
    pinMode(PIN_C_B5, OUTPUT);
    pinMode(PIN_C14, OUTPUT);
    pinMode(PIN_C15, OUTPUT);

    pinMode(PIN_R0, OUTPUT);
    pinMode(PIN_R1, OUTPUT);
    pinMode(PIN_R2, OUTPUT);
    pinMode(PIN_R3, OUTPUT);
    pinMode(PIN_R4, OUTPUT);
    pinMode(PIN_R5, OUTPUT);
    pinMode(PIN_R6, OUTPUT);
    pinMode(PIN_R7, OUTPUT);
    pinMode(PIN_R8, OUTPUT);
}

int rangeTransform(int oldVal, int oldRange, int newRange) {
   // OldRange = (OldMax - OldMin)  
   // NewRange = (NewMax - NewMin)  
   // NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin
   //printf("\trangeTransform: oldVal %d oldRange: %d newRange: %d\n", oldVal, oldRange, newRange);
   return (((oldVal - 0) * newRange) / oldRange) + 0;
}

// Selects the row to light up
void selectR(int sel) {
    switch(sel) {
    case 0:
        digitalWrite(PIN_R0, HIGH);
        digitalWrite(PIN_R1, LOW);
        digitalWrite(PIN_R2, LOW);
        digitalWrite(PIN_R3, LOW);
        digitalWrite(PIN_R4, LOW);
        digitalWrite(PIN_R5, LOW);
        digitalWrite(PIN_R6, LOW);
        digitalWrite(PIN_R7, LOW);
        digitalWrite(PIN_R8, LOW);
        break;
    case 1:
        digitalWrite(PIN_R0, LOW);
        digitalWrite(PIN_R1, HIGH);
        digitalWrite(PIN_R2, LOW);
        digitalWrite(PIN_R3, LOW);
        digitalWrite(PIN_R4, LOW);
        digitalWrite(PIN_R5, LOW);
        digitalWrite(PIN_R6, LOW);
        digitalWrite(PIN_R7, LOW);
        digitalWrite(PIN_R8, LOW);
        break;
    case 2:
        digitalWrite(PIN_R0, LOW);
        digitalWrite(PIN_R1, LOW);
        digitalWrite(PIN_R2, HIGH);
        digitalWrite(PIN_R3, LOW);
        digitalWrite(PIN_R4, LOW);
        digitalWrite(PIN_R5, LOW);
        digitalWrite(PIN_R6, LOW);
        digitalWrite(PIN_R7, LOW);
        digitalWrite(PIN_R8, LOW);
        break;
    case 3:
        digitalWrite(PIN_R0, LOW);
        digitalWrite(PIN_R1, LOW);
        digitalWrite(PIN_R2, LOW);
        digitalWrite(PIN_R3, HIGH);
        digitalWrite(PIN_R4, LOW);
        digitalWrite(PIN_R5, LOW);
        digitalWrite(PIN_R6, LOW);
        digitalWrite(PIN_R7, LOW);
        digitalWrite(PIN_R8, LOW);
        break;
    case 4:
        digitalWrite(PIN_R0, LOW);
        digitalWrite(PIN_R1, LOW);
        digitalWrite(PIN_R2, LOW);
        digitalWrite(PIN_R3, LOW);
        digitalWrite(PIN_R4, HIGH);
        digitalWrite(PIN_R5, LOW);
        digitalWrite(PIN_R6, LOW);
        digitalWrite(PIN_R7, LOW);
        digitalWrite(PIN_R8, LOW);
        break;
    case 5:
        digitalWrite(PIN_R0, LOW);
        digitalWrite(PIN_R1, LOW);
        digitalWrite(PIN_R2, LOW);
        digitalWrite(PIN_R3, LOW);
        digitalWrite(PIN_R4, LOW);
        digitalWrite(PIN_R5, HIGH);
        digitalWrite(PIN_R6, LOW);
        digitalWrite(PIN_R7, LOW);
        digitalWrite(PIN_R8, LOW);
        break;
    case 6:
        digitalWrite(PIN_R0, LOW);
        digitalWrite(PIN_R1, LOW);
        digitalWrite(PIN_R2, LOW);
        digitalWrite(PIN_R3, LOW);
        digitalWrite(PIN_R4, LOW);
        digitalWrite(PIN_R5, LOW);
        digitalWrite(PIN_R6, HIGH);
        digitalWrite(PIN_R7, LOW);
        digitalWrite(PIN_R8, LOW);
        break;
    case 7:
        digitalWrite(PIN_R0, LOW);
        digitalWrite(PIN_R1, LOW);
        digitalWrite(PIN_R2, LOW);
        digitalWrite(PIN_R3, LOW);
        digitalWrite(PIN_R4, LOW);
        digitalWrite(PIN_R5, LOW);
        digitalWrite(PIN_R6, LOW);
        digitalWrite(PIN_R7, HIGH);
        digitalWrite(PIN_R8, LOW);
        break;
    case 8:
        digitalWrite(PIN_R0, LOW);
        digitalWrite(PIN_R1, LOW);
        digitalWrite(PIN_R2, LOW);
        digitalWrite(PIN_R3, LOW);
        digitalWrite(PIN_R4, LOW);
        digitalWrite(PIN_R5, LOW);
        digitalWrite(PIN_R6, LOW);
        digitalWrite(PIN_R7, LOW);
        digitalWrite(PIN_R8, HIGH);
        break;
    default:
        digitalWrite(PIN_R0, LOW);
        digitalWrite(PIN_R1, LOW);
        digitalWrite(PIN_R2, LOW);
        digitalWrite(PIN_R3, LOW);
        digitalWrite(PIN_R4, LOW);
        digitalWrite(PIN_R5, LOW);
        digitalWrite(PIN_R6, LOW);
        digitalWrite(PIN_R7, LOW);
        digitalWrite(PIN_R8, LOW);
        break;
    }
}

// Selects the column to light up
void selectC(int sel) {
    switch(sel) {
    case 0:
        digitalWrite(PIN_C_B0, HIGH);
        digitalWrite(PIN_C_B1, HIGH);
        digitalWrite(PIN_C_B2, LOW);
        digitalWrite(PIN_C_B3, LOW);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 1:
        digitalWrite(PIN_C_B0, HIGH);
        digitalWrite(PIN_C_B1, LOW);
        digitalWrite(PIN_C_B2, HIGH);
        digitalWrite(PIN_C_B3, LOW);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 2:
        digitalWrite(PIN_C_B0, HIGH);
        digitalWrite(PIN_C_B1, LOW);
        digitalWrite(PIN_C_B2, LOW);
        digitalWrite(PIN_C_B3, HIGH);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 3:
        digitalWrite(PIN_C_B0, HIGH);
        digitalWrite(PIN_C_B1, LOW);
        digitalWrite(PIN_C_B2, LOW);
        digitalWrite(PIN_C_B3, LOW);
        digitalWrite(PIN_C_B4, HIGH);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 4:
        digitalWrite(PIN_C_B0, HIGH);
        digitalWrite(PIN_C_B1, LOW);
        digitalWrite(PIN_C_B2, LOW);
        digitalWrite(PIN_C_B3, LOW);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, HIGH);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 5:
        digitalWrite(PIN_C_B0, LOW);
        digitalWrite(PIN_C_B1, HIGH);
        digitalWrite(PIN_C_B2, HIGH);
        digitalWrite(PIN_C_B3, LOW);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 6:
        digitalWrite(PIN_C_B0, LOW);
        digitalWrite(PIN_C_B1, HIGH);
        digitalWrite(PIN_C_B2, LOW);
        digitalWrite(PIN_C_B3, HIGH);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 7:
        digitalWrite(PIN_C_B0, LOW);
        digitalWrite(PIN_C_B1, HIGH);
        digitalWrite(PIN_C_B2, LOW);
        digitalWrite(PIN_C_B3, LOW);
        digitalWrite(PIN_C_B4, HIGH);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 8:
        digitalWrite(PIN_C_B0, LOW);
        digitalWrite(PIN_C_B1, HIGH);
        digitalWrite(PIN_C_B2, LOW);
        digitalWrite(PIN_C_B3, LOW);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, HIGH);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 9:
        digitalWrite(PIN_C_B0, LOW);
        digitalWrite(PIN_C_B1, LOW);
        digitalWrite(PIN_C_B2, HIGH);
        digitalWrite(PIN_C_B3, HIGH);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 10:
        digitalWrite(PIN_C_B0, LOW);
        digitalWrite(PIN_C_B1, LOW);
        digitalWrite(PIN_C_B2, HIGH);
        digitalWrite(PIN_C_B3, LOW);
        digitalWrite(PIN_C_B4, HIGH);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 11:
        digitalWrite(PIN_C_B0, LOW);
        digitalWrite(PIN_C_B1, LOW);
        digitalWrite(PIN_C_B2, HIGH);
        digitalWrite(PIN_C_B3, LOW);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, HIGH);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 12:
        digitalWrite(PIN_C_B0, LOW);
        digitalWrite(PIN_C_B1, LOW);
        digitalWrite(PIN_C_B2, LOW);
        digitalWrite(PIN_C_B3, HIGH);
        digitalWrite(PIN_C_B4, HIGH);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 13:
        digitalWrite(PIN_C_B0, LOW);
        digitalWrite(PIN_C_B1, LOW);
        digitalWrite(PIN_C_B2, LOW);
        digitalWrite(PIN_C_B3, HIGH);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, HIGH);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    case 14:
        digitalWrite(PIN_C_B0, LOW);
        digitalWrite(PIN_C_B1, LOW);
        digitalWrite(PIN_C_B2, LOW);
        digitalWrite(PIN_C_B3, LOW);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, HIGH);
        digitalWrite(PIN_C15, LOW);
        break;
    case 15:
        digitalWrite(PIN_C_B0, LOW);
        digitalWrite(PIN_C_B1, LOW);
        digitalWrite(PIN_C_B2, LOW);
        digitalWrite(PIN_C_B3, LOW);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, HIGH);
        break;
    default:
        digitalWrite(PIN_C_B0, LOW);
        digitalWrite(PIN_C_B1, LOW);
        digitalWrite(PIN_C_B2, LOW);
        digitalWrite(PIN_C_B3, LOW);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
        break;
    }
}

// Turns on the LED at specified row and column
// note: Delay not accounted for, needs to include delay manually
//       usually delay(2) is good enough
void select(int row, int col) {
    selectR(row % NUM_OF_ROWS);
    selectC(col % NUM_OF_COLS);
}

// Draws the LED on the board based on the xy coordinates in
// relation to the screen width and height
// note: Delay is already accounted for
void drawLED(int xCrd, int yCrd, int gradient) {
   //softPwmWrite(xCrd * yCrd,  gradient);
   float blkH = SCREEN_HEIGHT / NUM_OF_ROWS;
   float blkW = SCREEN_WIDTH  / NUM_OF_COLS;
   float row = ((float) yCrd / SCREEN_HEIGHT * NUM_OF_ROWS) - 0.5;
   float col = ((float) xCrd / SCREEN_WIDTH  * NUM_OF_COLS) - 0.5;

   if (DEBUG) {
      printf("xCrd is %d, yCrd is %d\t", xCrd, yCrd);
      printf("Drawing row %.2f, col %.2f\n", row, col);
   }

   // turn on selected LED
   select((int) row, (int) col); delay(2);

/*
   bool nextRow = row - (int) row >= 0.5;
   bool nextCol = col - (int) col >= 0.5;

   // check if coordinates falls into multiple LEDS
   if (nextRow) {
      select(row + 1, col    ); delay(2);
   }
   if (nextCol) {
      select(row    , col + 1); delay(2);
   }
   if (nextRow & nextCol) {
      select(row + 1, col + 1); delay(2);
   }
*/
}

void drawHLine(int row, int b, int e) {
   for(int i = b; i < e; i++) {
      drawLED(row, i, 255);
   }
}

void drawVLine(int row, int b, int e) {
   for(int i = b; i < e; i++) {
      drawLED(row, i, 255);
   }
}

// draw box
void demo1(int amt) {
   do {
      for (int j = 0; j < NUM_OF_ROWS; j++) {
         for (int i = 0; i < NUM_OF_COLS; i++) {
            if ((j == 0) | (j == NUM_OF_ROWS - 1)) {
               select(j, i); delay(1);
            }
            else {
               select(j, 0); delay(1);
               select(j, NUM_OF_COLS - 1); delay(1);
               break;
            }
         }
      }
   } while(--amt);
}

void demo2(int amt) {

}

void demo3(int amt) {

}

void displayFallingRain(int col) {
    select(0,col); delay(200);
    select(1,col); delay(200);
    select(2,col); delay(200);
    select(3,col); delay(200);
    select(4,col); delay(200);
    select(5,col); delay(200);
}

void displayAllCoords() {
   for (int i = 0; i < SCREEN_WIDTH; i++) {
      for (int j = 0; j < SCREEN_HEIGHT; j++) {
         drawLED(i, j, 255);
      }
   }
}

int main(int argc, char *argv[]) {

   if(DEBUG) printf("Hello ALPACA\n");

   initializePins();
   wiringPiSetup();

   int counter = 0;
   while(1) {
      //displayFallingRain(counter); counter++;
      demo1(1);
      //displayAllCoords();
   }

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

   // {screenWidth, screenHeight, xCrd, yCrd, isLEDOn, LEDgradient, demo, DemoAmount}
   int prevState[PARAM_LEN] = {0};
   int currState[PARAM_LEN] = {0};

   prevState[4] = 1;

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

      if (DEBUG) printf("Here is the message: %s", buffer);
      
      // tokenize rece msg
      char *token = strtok(buffer, " ");
      for(int i = 0; i < PARAM_LEN; i++) {
         currState[i] = atoi(token);
         token = strtok(NULL,  " ");
      }


      if(currState[DEMO] == 0) {
         // transform range to led grid resolution
         currState[2] = rangeTransform(currState[2], currState[0],  NUM_OF_COLS);
         currState[3] = rangeTransform(currState[3], currState[1], NUM_OF_ROWS);
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
      }

      // save current state informations
      for(int i = 0; i < PARAM_LEN; i++) {
         prevState[i] = currState[i];
      }

   }
   
   return 0;
}
