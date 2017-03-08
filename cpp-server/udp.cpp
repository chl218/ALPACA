//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define LED_WIDTH  6
#define LED_HEIGHT 4

#define PORT      5001
#define BUFSIZE   1024
#define INPUTSIZE 8

#define PHONE_WIDTH   0
#define PHONE_HEIGHT  1
#define PHONE_XCRD    2
#define PHONE_YCRD    3
#define LED_STATUS	 4
#define LED_GRADIENT  5
#define DEMO_STATUS   6
#define DEMO_DURATION 7


int rangeTransform(int oldVal, int oldRange, int newRange) {
   // OldRange = (OldMax - OldMin)  
   // NewRange = (NewMax - NewMin)  
   // NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin
   //printf("\trangeTransform: oldVal %d oldRange: %d newRange: %d\n", oldVal, oldRange, newRange);
   return (((oldVal - 0) * newRange) / oldRange) + 0;
}

void drawLED(int xCrd, int yCrd, int gradient) {

}

void runDemo1() {}
void runDemo2() {}
void runDemo3() {}

int main(int argc, char **argv) {
   struct sockaddr_in myaddr;             /* our address */
   struct sockaddr_in remaddr;            /* remote address */
   socklen_t addrlen = sizeof(remaddr);   /* length of addresses */
   int recvlen;                           /* # bytes received */
   int fd;                                /* our socket */
   char buf[BUFSIZE];            /* receive buffer */
   
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

	int inputbuf[INPUTSIZE];

   /* now loop, receiving data and printing what we received */
   while(1) {
      

		printf("waiting on port %d\n", PORT);
      recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
      printf("received %d bytes\n", recvlen);
      if (recvlen > 0) {
         buf[recvlen] = 0;
         printf("received message: \"%s\"\n", buf);
      }
   
		// tokenize recv msg
		int inputbuf[INPUTSIZE] = {0};
      char *token = strtok(buf, " ");
      for(int i = 0; i < INPUTSIZE; i++) {
         inputbuf[i] = atoi(token);
         token = strtok(NULL,  " ");
      }	

		int width 		  = inputbuf[PHONE_WIDTH];
		int height		  = inputbuf[PHONE_HEIGHT];
		int xCrd 		  = rangeTransform(inputbuf[PHONE_XCRD], inputbuf[PHONE_WIDTH], LED_WIDTH);
		int yCrd			  = rangeTransform(inputbuf[PHONE_YCRD], inputbuf[PHONE_HEIGHT], LED_HEIGHT);
		int isLEDOn		  = inputbuf[LED_STATUS];
		int LEDGradiant  = inputbuf[LED_GRADIENT];
		int demoStatus	  = inputbuf[DEMO_STATUS];
		int demoDuration = inputbuf[DEMO_DURATION];

		if(isLEDOn) {

		}
		else {
			switch(demoStatus) {
				case 1: runDemo1(); break;
				case 2: runDemo2(); break;
				case 3: runDemo3(); break;
				default: break;
			}
		}


	}
   /* never exits */
}





