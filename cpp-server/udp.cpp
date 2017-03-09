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
#include "interface.h"
#include <time.h>

#define PORT      5001

#define DEBUG     0
#define DEBUG_LED 0

#define NUM_OF_ROWS 9  // # of rows in LED matrix    (full = 9)
#define NUM_OF_COLS 16 // # of columns in lED matrix (full = 16)

//-----------------------------------------------------------------------------
#define BUFSIZE   1024
#define INPUTSIZE 8

#define PHONE_WIDTH   0
#define PHONE_HEIGHT  1
#define PHONE_XCRD    2
#define PHONE_YCRD    3
#define LED_STATUS    4
#define LED_GRADIENT  5
#define DEMO_STATUS   6
#define DEMO_DURATION 7

int helloRow0[36];
int helloRow1[36];
int helloRow2[36];
int helloRow3[36];
int helloRow4[36];
int helloRow5[36];
int helloRow6[36];
int helloRow7[36];
int helloRow8[36];

// sets up all the row and column pins to output
void initializePins() {
    pinMode(PIN_C00, OUTPUT);
    pinMode(PIN_C01, OUTPUT);
    pinMode(PIN_C02, OUTPUT);
    pinMode(PIN_C03, OUTPUT);
    pinMode(PIN_C04, OUTPUT);
    pinMode(PIN_C05, OUTPUT);
    pinMode(PIN_C06, OUTPUT);
    pinMode(PIN_C07, OUTPUT);
    pinMode(PIN_C08, OUTPUT);
    pinMode(PIN_C09, OUTPUT);
    pinMode(PIN_C10, OUTPUT);
    pinMode(PIN_C11, OUTPUT);
    pinMode(PIN_C12, OUTPUT);
    pinMode(PIN_C13, OUTPUT);
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
    digitalWrite(PIN_C00, LOW);
    digitalWrite(PIN_C01, LOW);
    digitalWrite(PIN_C02, LOW);
    digitalWrite(PIN_C03, LOW);
    digitalWrite(PIN_C04, LOW);
    digitalWrite(PIN_C05, LOW);
    digitalWrite(PIN_C06, LOW);
    digitalWrite(PIN_C07, LOW);
    digitalWrite(PIN_C08, LOW);
    digitalWrite(PIN_C09, LOW);
    digitalWrite(PIN_C10, LOW);
    digitalWrite(PIN_C11, LOW);
    digitalWrite(PIN_C12, LOW);
    digitalWrite(PIN_C13, LOW);
    digitalWrite(PIN_C14, LOW);
    digitalWrite(PIN_C15, LOW);
    if ( sel > NUM_OF_COLS ) { return; }
    else {
    switch(sel) {
    case 0:
        digitalWrite(PIN_C00, HIGH);
        break;
    case 1:
        digitalWrite(PIN_C01, HIGH);
        break;
    case 2:
        digitalWrite(PIN_C02, HIGH);
        break;
    case 3:
        digitalWrite(PIN_C03, HIGH);
        break;
    case 4:
        digitalWrite(PIN_C04, HIGH);
        break;
    case 5:
        digitalWrite(PIN_C05, HIGH);
        break;
    case 6:
        digitalWrite(PIN_C06, HIGH);
        break;
    case 7:
        digitalWrite(PIN_C07, HIGH);
        break;
    case 8:
        digitalWrite(PIN_C08, HIGH);
        break;
    case 9:
        digitalWrite(PIN_C09, HIGH);
        break;
    case 10:
        digitalWrite(PIN_C10, HIGH);
        break;
    case 11:
        digitalWrite(PIN_C11, HIGH);
        break;
    case 12:
        digitalWrite(PIN_C12, HIGH);
        break;
    case 13:
        digitalWrite(PIN_C13, HIGH);
        break;
    case 14:
        digitalWrite(PIN_C14, HIGH);
        break;
    case 15:
        digitalWrite(PIN_C15, HIGH);
        break;
    default:
        digitalWrite(PIN_C00, LOW);
        digitalWrite(PIN_C01, LOW);
        digitalWrite(PIN_C02, LOW);
        digitalWrite(PIN_C03, LOW);
        digitalWrite(PIN_C04, LOW);
        digitalWrite(PIN_C05, LOW);
        digitalWrite(PIN_C06, LOW);
        digitalWrite(PIN_C07, LOW);
        digitalWrite(PIN_C08, LOW);
        digitalWrite(PIN_C09, LOW);
        digitalWrite(PIN_C10, LOW);
        digitalWrite(PIN_C11, LOW);
        digitalWrite(PIN_C12, LOW);
        digitalWrite(PIN_C13, LOW);
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
   return ((oldVal * newRange) / oldRange);
}

// Draws the LED on the board based on the xy coordinates in
// relation to the screen width and height
// note: Delay is already accounted for
void displayLED(int xCrd, int yCrd, int screenWidth, int screenHeight, int gradient) {
   //softPwmWrite(xCrd * yCrd,  gradient);
   //int row = rangeTransform(yCrd, screenHeight, NUM_OF_ROWS);
   //int col = rangeTransform(xCrd, screenWidth,  NUM_OF_COLS);
   int row = ((float) yCrd / screenHeight * NUM_OF_ROWS);
   int col = ((float) xCrd / screenWidth  * NUM_OF_COLS);

   if (DEBUG) {
      printf("xCrd is %d, yCrd is %d\t", xCrd, yCrd);
      printf("Drawing row %d, col %d\n", row, col);
   }

   // turn on selected LED
   select(row, col); delay(2);
}

// draw box
int displayCollapsingBox(int counter) {
   int   runtime   = 1;
   float delay_dur = 0.5;
   int rows = NUM_OF_ROWS - counter;
   int cols = NUM_OF_COLS - counter;

   time_t t = time(NULL);
   while (1) {
      for (int j = counter; j < rows; j++) {
         for (int i = counter; i < cols; i++) {
            if ((j == counter) | (j == rows - 1)) {
               select(j, i); delay(delay_dur);
            }
            else {
               select(j, counter); delay(delay_dur);
               select(j, cols - 1); delay(delay_dur);
               break;
            }
         }
      }
      delay(delay_dur);
      if ( difftime(time(NULL), t) >= runtime ) { break; }
   }
   if ( NUM_OF_ROWS == 1 || NUM_OF_COLS == 1 )
       return counter;
   else if ( NUM_OF_ROWS < NUM_OF_COLS )
       return (counter + 1) % (NUM_OF_ROWS/2);
   else
       return (counter + 1) % (NUM_OF_COLS/2);
}

void shiftArray(int * arr, int size) {
   for (int i = 0; i < size; i++) {
      arr[i] = arr[i+1];
   }
   arr[size-1] = 0;
}

// Hello World
void displayHelloWorld9x16() {
   float delay_dur = 0.5;
   int row0[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0};
   int row1[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0};
   int row2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0};
   int row3[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0};
   int row4[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0};
   int row5[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0};
   int row6[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0};
   int row7[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1};
   int row8[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
   // row 8 doesn't show anything
   int size = sizeof(row0) / sizeof(int);
   int counter = 0;
   while (1) {
      int   runtime   = 1;
      time_t t = time(NULL);
      for (int j = 0; j < 16; j++) {
         if ( row0[j] ) { select(0, j); delay(delay_dur); }
         if ( row1[j] ) { select(1, j); delay(delay_dur); }
         if ( row2[j] ) { select(2, j); delay(delay_dur); }
         if ( row3[j] ) { select(3, j); delay(delay_dur); }
         if ( row4[j] ) { select(4, j); delay(delay_dur); }
         if ( row5[j] ) { select(5, j); delay(delay_dur); }
         if ( row6[j] ) { select(6, j); delay(delay_dur); }
         if ( row7[j] ) { select(7, j); delay(delay_dur); }
         if ( row8[j] ) { select(8, j); delay(delay_dur); }
      }
      if ( difftime(time(NULL), t) >= runtime ) {
         shiftArray(row0, size);
         shiftArray(row1, size);
         shiftArray(row2, size);
         shiftArray(row3, size);
         shiftArray(row4, size);
         shiftArray(row5, size);
         shiftArray(row6, size);
         shiftArray(row7, size);
         if (++counter == size) break;
      }
   }
}

void displayFallingRain(int col) {
    if (DEBUG_LED) printf("Falling Rain Col: %d\n", col);
    for (int i = 0; i < NUM_OF_ROWS; i++) {
        select(i,col); delay(100);
    }
}

void displayAllCoords() {
   for (int i = 0; i < NUM_OF_ROWS; i++) {
      for (int j = 0; j < NUM_OF_COLS; j++) {
         select(i, j); delay(0.5);
      }
   }
}

int main(int argc, char **argv) {
   wiringPiSetup();
   initializePins();

   int counter = 0;
   while(DEBUG_LED) {
      //counter = displayCollapsingBox(counter);
      displayFallingRain(counter); counter = (counter + 1) % NUM_OF_COLS;
      //counter = rand() % NUM_OF_COLS;
      //displayHelloWorld9x16();
      //displayAllCoords();
   }

   struct sockaddr_in myaddr;             /* our address */
   struct sockaddr_in remaddr;            /* remote address */
   socklen_t addrlen = sizeof(remaddr);   /* length of addresses */
   int recvlen;                           /* # bytes received */
   int fd;                                /* our socket */
   char buf[BUFSIZE];                     /* receive buffer */
   
   /* create a UDP socket */
   if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("ERROR: cannot create socket\n");
      return 0;
   }

   /* bind the socket to any valid IP address and a specific port */
   memset((char *)&myaddr, 0, sizeof(myaddr));
   myaddr.sin_family = AF_INET;
   myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   myaddr.sin_port = htons(PORT);

   if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
      perror("ERROR: bind failed");
      return 0;
   }

   select(NUM_OF_ROWS, NUM_OF_COLS); delay (1);
   int inputbuf[INPUTSIZE];

   /* now loop, receiving data and printing what we received */
   while(1) {
      if (DEBUG) printf("waiting on port %d\n", PORT);
      recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
      if (DEBUG) printf("received %d bytes\n", recvlen);
      if (recvlen > 0) {
         buf[recvlen] = 0;
         if (DEBUG) printf("received message: \"%s\"\n", buf);
      }
   
      // tokenize recv msg
      int inputbuf[INPUTSIZE] = {0};
      char *token = strtok(buf, " ");
      for(int i = 0; i < INPUTSIZE; i++) {
         inputbuf[i] = atoi(token);
         token = strtok(NULL,  " ");
      }  

      if( inputbuf[DEMO_STATUS] < 0 ) {
         // turn LED on if isLEDOn is true
         if ( inputbuf[LED_STATUS] ) {
            displayLED( inputbuf[PHONE_XCRD], inputbuf[PHONE_YCRD],
                        inputbuf[PHONE_WIDTH], inputbuf[PHONE_HEIGHT], inputbuf[LED_GRADIENT] );
         }
         else {
            select(NUM_OF_ROWS, NUM_OF_COLS); delay (1);
         }
      }
      else {
         time_t t     = time(NULL);
         int runtime  = inputbuf[DEMO_DURATION];
         switch (inputbuf[DEMO_STATUS]) {
            case 1:
               displayHelloWorld9x16();
               break;
            case 2:
               while (1) {
      	          displayFallingRain(counter); counter = (counter + 11) % NUM_OF_COLS;
                  if ( difftime(time(NULL), t) >= runtime ) { break; }
               }
               break;
            case 3:
               while (1) {
                  counter = displayCollapsingBox(counter); delay(2);
                  if ( difftime(time(NULL), t) >= runtime ) { break; }
               }
               break;
            default:
               break;
         }
         select(NUM_OF_ROWS, NUM_OF_COLS);
      }
   }
   /* never exits */
}
