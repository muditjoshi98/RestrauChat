/******************************************
           Header files Required
******************************************/

#include "user.h"


/************************************************
        Function for Dynamic String Input
*************************************************/

char *getln()										
{
    char *line = NULL, *tmp = NULL;
    size_t size = 0, index = 0;
    int ch = EOF;

    while (ch) {
        ch = getc(stdin);

        /* Check if we need to stop. */
        if (ch == EOF || ch == '\n')
            ch = 0;

        /* Check if we need to expand. */
        if (size <= index) {
            size += 1;
            tmp = realloc(line, size);
            if (!tmp) {
                free(line);
                line = NULL;
                break;
            }
            line = tmp;
        }

        /* Actually store the thing. */
        line[index++] = ch;
    }

    return line;
}


/*******************************************************
        Function to send booking data to server
********************************************************/

int book_seat() { 
	
    struct booking book;
	//printf("Book");
    printf("\n**** %c[4mRestraùChat - Booking Portal%c[0m ****\n\n",27,27);
    send(sock, "book", 4,0);

    printf("Bon Choix (Good Choice), lets get started by telling me your name:-");
	//getchar();
	book.name = getln();
    send(sock , book.name , strlen(book.name) , 0 );


	printf("How many people are we expecting:- ");
    book.no_of_seats = getln();
    send(sock , book.no_of_seats , strlen(book.no_of_seats) , 0 );

    printf("Time Slot (booking from-booking till) like (3-4) in 24 hour format:-");
	book.slot=getln();
    send(sock , book.slot , strlen(book.slot) , 0 );

    if((valread=read( sock , buffer, 1024))!=0) {
        buffer[valread] = '\0';
        printf("\nHere's your reference no, please keep it safe :- \"%s\"\n\nWe will be waiting to serve you...\n\n",buffer);
    }
    else {
        printf("\nUnable to recieve message\n");
    }
	printf("Press Enter to continue....");
	getchar();
	return 0;
}


/************************************************
        Function for checking user data
*************************************************/

int book_status() {

	int i,flag=0;							
	char *ref_no;

	printf("\n**** %c[4mRestraùChat - Booking Status%c[0m ****\n\n",27,27);

    send(sock, "status", 6, 0);

    printf("Your Reference Number, S'il vous plait (Please):-");
    //getchar();
    ref_no= getln();
    send(sock, ref_no, strlen(ref_no), 0);

    int no=10;
    if((valread=read( sock , buffer, 1024))!=0) {
        buffer[valread] = '\0';
        if(!strcmp(buffer,"notfound")) {
        	printf("Pardon, your Reference No is wrong please recheck it...\n");
        }
        else {
	        int len=strlen(buffer);
	        printf("\n\nHere are your details, Monsieur :-\n\tName:- ");

	        for(i=0;i<len;i++) {
	        	if(buffer[i]=='_')
	        		printf(" ");
	        	else if(buffer[i]==',') {
	        		if(flag==0) {
	        			printf("\n\tReference No:- ");
	        			flag++;
	        		}
	        		else if(flag==1) {
	        			printf("\n\tTime Slot:- ");
	        			flag++;
	        		}
	        		else if(flag==2) {
	        			printf("\n\tNo of Seats:- ");
	        			flag++;	
	        		}
	        	}
	        	else
	        		printf("%c",buffer[i]);
	        }
	        flag=0;
	    }
	}
	printf("\nPress Enter to continue....");
	getchar();
    return 0;
}


/*******************************************************
        Function to send cancelling data to server
********************************************************/

int cancel_seat() {		
	int i,flag=0;
	char *ref_no,*buff,choice,c;
    printf("\n**** %c[4mRestraùChat - Cancel Portal%c[0m ****\n\n",27,27);

    send(sock, "cancel", 6, 0);

    printf("Your Reference Number, S'il vous plait (Please):-");
    //getchar();
    ref_no= getln();
    send(sock, ref_no, strlen(ref_no), 0);
    int no=10;
    if((valread=read( sock , buffer, 1024))!=0) {
        buffer[valread] = '\0';
        if(!strcmp(buffer,"notfound")) {
        	printf("Pardon, your Reference No is wrong please recheck it...\n");
        }
        else {
	        int len=strlen(buffer);
	        printf("\n\nHere are your details, Monsieur :-\n\tName:- ");

	        for(i=0;i<len;i++) {
	        	if(buffer[i]=='_')
	        		printf(" ");
	        	else if(buffer[i]==',') {
	        		if(flag==0) {
	        			printf("\n\tReference No:- ");
	        			flag++;
	        		}
	        		else if(flag==1) {
	        			printf("\n\tTime Slot:- ");
	        			flag++;
	        		}
	        		else if(flag==2) {
	        			printf("\n\tNo of Seats:- ");
	        			flag++;
	        		}
	        	}
	        	else
	        		printf("%c",buffer[i]);
	        }
	        flag=0;
	        printf("\n\nAre you Sure, to cancel your booking (Y/N):- ");
	        scanf("%c",&choice);
	        if(tolower(choice)=='y'){
	        	c='y';
	        }
	        else if(tolower(choice)=='n'){
	        	c='n';
	        	no=2;
	        }
	        send(sock, &c, sizeof(c), 0);
	        read(sock, &no, sizeof(no));
		    if(no==0) {
		    	printf ("\nYour booking is successfully cancelled, À bientôt (see you soon)\n");
		    }
		    else if(no==2) {
		    	printf ("\nWe will be ready to serve you\n");
		    }
		    else if(no==1) {
		    	printf("Writing Error");
		    }
	    }
    }
    else {
        printf("\nUnable to recieve message\n");
    }
	printf("\nPress Enter to continue....");
	getchar();
	getchar();
	return 0;
}


/******************************************
        Main driver function
******************************************/

int main() {

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton	(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

	int choice;
	while(1) {
		clear();
		printf("\n**** %c[4mWelcome to RestraùChat - Your French Companion for Délicieuse Food%c[0m ****\n\n",27,27);
        printf("Hi! I am your friend, ChefBot\n");
		printf("Please, enter your choice in number:-\n1. Book a table\n2. Booking Status\n3. Cancel a Booking\n4. Exit\n");
		scanf("%d",&choice);
		getchar();
		if(choice==1){
			book_seat();
        }
        else if(choice==2) {
        	book_status();
        }
		else if(choice==3){
			cancel_seat();
        }
		else if(choice==4){
			printf("\nThank you, hoping to see you soon");
			return 0;
		}
		else{
			printf("Opps wrong choice!!! Enter the correct number please.\n");
			getchar();
        }
	}
    close(sock);
	return 0;
}