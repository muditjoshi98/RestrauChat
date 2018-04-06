/******************************************
           Header files Required
******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


/******************************************
            MACRO Definition
******************************************/

#define PORT 8888
#define clear() printf("\033[H\033[J")


/******************************************
            Global Variables
******************************************/

struct sockaddr_in address;
int sock = 0, valread;
struct sockaddr_in serv_addr;
char buffer[1024] = {0};
int valread;

// structure contains all necessary data required for seat booking
struct booking {
	char *name,*reference_no,*slot,*no_of_seats;
};

/******************************************
            Function Declaration
******************************************/

char *getln();
int book_seat();
int cancel_seat();
int book_status();
