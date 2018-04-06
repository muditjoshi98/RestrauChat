/******************************************
           Header files Required
******************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>   
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> 			//FD_SET, FD_ISSET, FD_ZERO macros 
#include <time.h> 				//for srand function


/******************************************
            MACRO Definition
******************************************/

#define TRUE   1 
#define FALSE  0 
#define PORT 8888 
  

/******************************************
            Global Variables
******************************************/

//variables for creating connection
int opt = TRUE,count=0;  
int master_socket , addrlen , new_socket , client_socket[1000] , max_clients = 1000 , activity, i , valread , sd;  
int max_sd;  
int opr_check=0;
struct sockaddr_in address;  

//data buffer of 1K   
char buffer[1024];     

//set of socket descriptors 
fd_set readfds;  


// structure contains all necessary data required for seat booking
struct booking {
    char name[1024],reference_no[1024],slot[1024],no_of_seats[1024];
    int counts,opr_check,countc;
};


/******************************************
            Function Declaration
******************************************/

struct booking *alloc();
char *gen_ran();
char *store(struct booking);
char *search(char *);
int rem(char *);