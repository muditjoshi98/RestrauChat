/******************************************
           Header files Required
******************************************/

#include "server.h"


/**********************************************************
        Function to Allocate space for struct book
***********************************************************/

struct booking *alloc(struct booking *main) {
	struct booking *temp=NULL;
	temp=(struct booking *)realloc(main,sizeof(struct booking)*max_clients);
	return temp;
}


/*****************************************************
        Function to generate reference number
******************************************************/

char *gen_ran(){
    int i;
    char *no;
    no=(char *)malloc(8);
    srand(time(0));
    for (i = 0; i < 8;) {
        int num = rand()%128;
        if(((num>65 && num<90) || (num>97 && num<122)) && i%2==0) {
            no[i]=num;
            i++;
        }
        else if((num>48 && num<57) && i%2==1) {
            no[i]=num;
            i++;
        }
    }
    no=strndup(no,8);
    return no;
}


/**********************************************
        Function to store booking data
***********************************************/

char *store(struct booking book) {
    char *ref_no;
    while(1) {
    	strcpy(book.reference_no,gen_ran());
    	if(!strcmp(search(book.reference_no),"notfound"))
    		break;
    }

    for(int i=0;i<strlen(book.name);i++) {
    	if(book.name[i]==' ') {
    		book.name[i]='_';
    	}
    }

    FILE *file;
    file = fopen ( "data.csv", "a+" ) ;
    if ( file == NULL ) {
            puts ( "Cannot open file" ) ;
            exit(1) ;
    }

    if( fprintf ( file, "%s,%s,%s,%s\n", book.name, book.reference_no, book.slot, book.no_of_seats) < 0) {
        printf ("Data writing error");
        exit(1);
    }

    fclose(file);
    ref_no=strdup(book.reference_no);
    return ref_no;
}


/***************************************************
        Function to parse Data from CSV file
****************************************************/

const char* getdata(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return tok;
}


/**********************************************
        Function to search data of user
***********************************************/

char * search(char *ref_no) {
	int search_f=0;
    char *tmp;
    FILE *file;
    file = fopen ( "data.csv", "r+" ) ;
    if ( file == NULL ) {
            puts ( "Cannot open file" ) ;
            return "notfound" ;
    }

    char *line;
    line=(char *)malloc(4096);
    while (fgets(line, 4096, file))
    {

    	tmp = strdup(line);
		if ( !strcmp ( getdata(tmp, 2), ref_no ))
        {	
        	search_f=1;
        	return line;
        }
        free(tmp);
    }
    fclose(file);
    if(!search_f) {
    	return "notfound";
    }
}


/************************************************
        Function to cancel booking of user
*************************************************/

int rem( char *ref_no) {

	char * tmp;
    FILE *file1,*file2;
    file1 = fopen ( "data.csv", "r+" ) ;
    if ( file1 == NULL ) {
            puts ( "Cannot open file" ) ;
            exit(1) ;
    }

    file2 = fopen ( "temp.csv", "w+" ) ;
    if ( file2 == NULL ) {
            puts ( "Cannot open file" ) ;
            exit(1) ;
    }

    char test[4][1024];

    char *line;
    line=(char *)malloc(4096);
    while (fgets(line, 4096, file1))
    {
    	tmp = strdup(line);
		if ( strcmp ( getdata(tmp, 2), ref_no ))
        {	
        	fputs(line,file2);
        }
        free(tmp);
    }
    fclose(file1);
    fclose(file2);
    remove("data.csv");
    rename("temp.csv","data.csv");
    return 0;
}


/******************************************
        Main driver function
******************************************/

int main(int argc , char *argv[])  
{  
    struct booking *book;
    book=alloc(book);
    
    //initialise all client_socket[] to 0 so not checked 
    for (i = 0; i < max_clients; i++)  
    {  
        client_socket[i] = 0;  
    }  
        
    //create a master socket 
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
    {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  
    
    //set master socket to allow multiple connections , 
    //this is just a good habit, it will work without this 
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
    {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
    
    //type of socket created 
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  			//inet_addr("10.12.110.57");
    address.sin_port = htons( PORT );  
        
    //bind the socket to localhost port 8888 
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)  
    {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
    printf("Listener on port %d \n", PORT);  
        
    //try to specify maximum of 3 pending connections for the master socket 
    if (listen(master_socket, 3) < 0)  
    {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }  
        
    //accept the incoming connection 
    addrlen = sizeof(address);  
    puts("Waiting for connections ...");  
        
    while(TRUE)  
    {  
        //clear the socket set 
        FD_ZERO(&readfds);  
    
        //add master socket to set 
        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;  
            
        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++)  
        {  
            //socket descriptor 
            sd = client_socket[i];  
                
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
    
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
      
        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error");  
        }  
            
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(master_socket, &readfds))  
        {  
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
            {  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
            
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
            
            /*send new connection greeting message 
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )  
            {  
                perror("send");  
            }  
                
            puts("Welcome message sent successfully");  */
        
            for (i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                    break;  
                }  
            }
        }  
            
        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)  
        {  
            sd = client_socket[i];  
                
            if (FD_ISSET( sd , &readfds))  
            {  
                //Check if it was for closing , and also read the 
                //incoming message 
                if ((valread = read( sd , buffer, 1024)) == 0)  
                {  
                    //Somebody disconnected , get his details and print 
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                        
                    //Close the socket and mark as 0 in list for reuse 
                    close( sd );  
                    client_socket[i] = 0;  
                    //book[i].name=book[i].no_of_seats=book[i].slot=book[i].reference_no;
                    memset(book[i].name,'\0',sizeof(book[i].name));
                    memset(book[i].no_of_seats,'\0',sizeof(book[i].no_of_seats));
                    memset(book[i].slot,'\0',sizeof(book[i].slot));
                    memset(book[i].reference_no,'\0',sizeof(book[i].reference_no));
                    book[i].counts=book[i].opr_check=book[i].countc=0;
                }  
                    
                //Echo back the message that came in 
                else
                {  
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    buffer[valread] = '\0';
                    //printf("%s ",buffer);
                    if(book[i].opr_check==0) {
                        if(!strcmp(buffer,"book")){
                            book[i].opr_check=1;
                        }
                        else if(!strcmp(buffer,"cancel")) {
                            book[i].opr_check=2;
                        }
                        else if(!strcmp(buffer,"status")) {
                        	book[i].opr_check=3;
                        }
                    }
                    else if(book[i].opr_check==1) {
                    	printf("\nStoring process started for socket %d\n",i);
                        if(book[i].counts==0) {
                            strcpy(book[i].name,buffer);
                            book[i].counts++;
                        }
                        else if(book[i].counts==1) {
                            strcpy(book[i].no_of_seats,buffer);
                            book[i].counts++;
                        }
                        else if(book[i].counts==2) {
                            strcpy(book[i].slot,buffer);
                            strcpy(buffer,store(book[i]));
                            strcpy(book[i].reference_no,buffer);
                            send(sd , buffer , strlen(buffer) , 0 );
                            book[i].counts=0;
                            book[i].opr_check=0;
                        }
                    }
                    else if(book[i].opr_check==2){
                    	printf("\nCanceling process started for socket %d\n",i);
                    	int res=2,count=0,k=0,j;
                    	char data[256]={'\0'};
                        if(book[i].countc==0) {
	                        strcpy(buffer,search(buffer));
	                        printf("\n---%s---\n",buffer);
                        	send(sd, buffer, strlen(buffer), 0);

                        	if(!strcmp(buffer,"notfound")) {
	                    		book[i].countc=0;
	                    		book[i].opr_check=0;
                        	}
                        	else {
                        		book[i].countc++;
                        		for(j=0;j<strlen(buffer);j++) {
	                        		if(buffer[j]==',') {
	                        			data[k]='\0';
	                        			printf("--%s %ld--",data,strlen(data));
	                        			if(count==0) {
	                            			strcpy(book[i].name,data);
	                            			count++;
	                        			}
	                        			else if(count==1) {
	                            			strcpy(book[i].reference_no,data);
	                            			count++;
	                        			}
	                        			else if(count==2) {
	                            			strcpy(book[i].slot,data);
	                            			count++;
	                        			}
	                        			else if(count==3) {
	                            			strcpy(book[i].no_of_seats,data);
	                            			count++;
	                        			}                        			
	                        			k=0;
	                        		}
	                        		else {
	                        			data[k++]=buffer[j];
	                        		}
                        		}
                        	}
                    	}
                    	else if(book[i].countc==1) {
                    		if(!strcmp(buffer,"y")) {
                    			res=rem(book[i].reference_no);
                    			send(sd,&res,sizeof(int),0);
                    		}
                    		else {
                    			send(sd,&res,sizeof(int),0);
                    		}
                    		book[i].countc=0;
                    		book[i].opr_check=0;
                    	}
                    }
                    else if(book[i].opr_check==3) {
                    	printf("\nChecking Status process started for socket %d\n",i);
                        if(book[i].countc==0) {
	                        strcpy(buffer,search(buffer));
                        	send(sd, buffer, strlen(buffer), 0);
                    	}
                    	book[i].opr_check=0;
                    }
                }  
            }  
        }  
    }  
    return 0;  
}  