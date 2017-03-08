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
#include <time.h>

#define DEBUG     1
#define DEBUG_LED 1

#define PORTNO 5001

#define NUM_OF_ROWS 6 // # of rows in LED matrix    (full = 9)
#define NUM_OF_COLS 4 // # of columns in lED matrix (full = 16)

// {screenWidth, screenHeight, xCrd, yCrd, isLEDOn, LEDgradient, demo, demoLength}
#define PARAM_LEN     8
#define SCREEN_WIDTH  0
#define SCREEN_HEIGHT 1
#define X_COORD       2
#define Y_COORD       3
#define LED_ON        4
#define LED_GRADIENT  5
#define DEMO          6
#define DEMO_LENGTH   7

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

// Selects the row to light up
void selectR(int sel) {
    if ( sel > NUM_OF_ROWS ) {
        digitalWrite(PIN_R0, LOW);
        digitalWrite(PIN_R1, LOW);
        digitalWrite(PIN_R2, LOW);
        digitalWrite(PIN_R3, LOW);
        digitalWrite(PIN_R4, LOW);
        digitalWrite(PIN_R5, LOW);
        digitalWrite(PIN_R6, LOW);
        digitalWrite(PIN_R7, LOW);
        digitalWrite(PIN_R8, LOW);
    }
    else {
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
}

// Selects the column to light up
void selectC(int sel) {
    if ( sel > NUM_OF_COLS ) {
        digitalWrite(PIN_C_B0, HIGH);
        digitalWrite(PIN_C_B1, HIGH);
        digitalWrite(PIN_C_B2, LOW);
        digitalWrite(PIN_C_B3, LOW);
        digitalWrite(PIN_C_B4, LOW);
        digitalWrite(PIN_C_B5, LOW);
        digitalWrite(PIN_C14, LOW);
        digitalWrite(PIN_C15, LOW);
    }
    else {
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
}

// Turns on the LED at specified row and column
// note: Delay not accounted for, needs to include delay manually 
//       after call to select(), usually delay(2) is good enough
void select(int row, int col) {
    selectR(row);
    selectC(col);
}

int rangeTransform(int oldVal, int oldRange, int newRange) {
   return ((oldVal * newRange) / oldRange) - 0.5;
}

// Draws the LED on the board based on the xy coordinates in
// relation to the screen width and height
// note: Delay is already accounted for
void displayLED(int xCrd, int yCrd, int screenWidth, int screenHeight, int gradient) {
   //softPwmWrite(xCrd * yCrd,  gradient);
   //int row = rangeTransform(yCrd, screenHeight, NUM_OF_ROWS);
   //int col = rangeTransform(xCrd, screenWidth,  NUM_OF_COLS);
   int row = ((float) yCrd / screenHeight * NUM_OF_ROWS) - 0.5;
   int col = ((float) xCrd / screenWidth  * NUM_OF_COLS) - 0.5;

   if (DEBUG_LED) {
      printf("xCrd is %d, yCrd is %d\t", xCrd, yCrd);
      printf("Drawing row %d, col %d\n", row, col);
   }

   // turn on selected LED
   select(row, col); delay(2);
}

/*
void drawHLine(int row, int b, int e) {
   for(int i = b; i < e; i++) {
      displayLED(row, i, 255); // needs screen height and width
   }
}

void drawVLine(int row, int b, int e) {
   for(int i = b; i < e; i++) {
      displayLED(row, i, 255); // needs screen height and width
   }
}
*/

// draw box
void runDemo0() {
   int delay_dur = 1;
   for (int j = 0; j < NUM_OF_ROWS; j++) {
      for (int i = 0; i < NUM_OF_COLS; i++) {
         if ((j == 0) | (j == NUM_OF_ROWS - 1)) {
            select(j, i); delay(delay_dur);
         }
         else {
            select(j, 0); delay(delay_dur);
            select(j, NUM_OF_COLS - 1); delay(delay_dur);
            break;
         }
      }
   }
}

void runDemo1() {
}

void runDemo2() {
}

void displayFallingRain(int col) {
    select(0,col); delay(200);
    select(1,col); delay(200);
    select(2,col); delay(200);
    select(3,col); delay(200);
    select(4,col); delay(200);
    select(5,col); delay(200);
}

void displayAllCoords(int screenWidth, int screenHeight) {
   for (int i = 0; i < screenWidth; i++) {
      for (int j = 0; j < screenHeight; j++) {
         displayLED(i, j, screenWidth, screenHeight, 255);
      }
   }
}

int main(int argc, char *argv[]) {
   initializePins();
   wiringPiSetup();

   int counter = 0;
   while(DEBUG_LED) {
      //displayFallingRain(counter); counter++;
      runDemo0();
      //runDemo1();
      //runDemo2();
      //displayAllCoords(100, 100);
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
   
   // { PARAM_LEN, SCREEN_WIDTH, SCREEN_HEIGHT, X_COORD, Y_COORD, LED_ON, LED_GRADIENT, DEMO }
   int currState[PARAM_LEN] = {0};
   select(NUM_OF_ROWS, NUM_OF_COLS); delay (1);

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
         // turn LED on if isLEDOn is true
         if ( currState[LED_ON] ) {
            displayLED( currState[X_COORD], currState[Y_COORD],
                        currState[SCREEN_WIDTH], currState[SCREEN_HEIGHT], currState[LED_GRADIENT] );
         }
         else {
            select(NUM_OF_ROWS, NUM_OF_COLS); delay (1);
         }
      }
      else {
         clock_t t = clock();
         clock_t duration  = CLOCKS_PER_SEC * currState[DEMO_LENGTH];
         switch (currState[DEMO]) {
            case 1:
               while (1) {
                  runDemo0();
                  if ( clock() - t > duration ) { break; }
               }
               break;
            case 2:
               while (1) {
                  runDemo1();
                  if ( clock() - t > duration ) { break; }
               }
               break;
            case 3:
               while (1) {
                  runDemo2();
                  if ( clock() - t > duration ) { break; }
               }
               break;
            default:
               break;
         }
      }
   }

   return 0;
}
