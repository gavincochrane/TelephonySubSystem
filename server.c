/** Server
 */
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h> //for threading , link with lpthread

void *connection_handler(void *socket_desc);

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 
	pthread_t thread_id;
	
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(1)
    {
		printf("Waiting\n");
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &connfd) < 0)
        {
            perror("could not create thread");
            break;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        printf("Handler assigned");
     }
     
     printf("Exiting\n");
     
     return 0;
}

void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    char sendBuff[1025];
    time_t ticks; 

    //int read_size;
    //char *message , client_message[2000];
    
    memset(sendBuff, '0', sizeof(sendBuff)); 
    
	ticks = time(NULL);
	snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
	write(sock, sendBuff, strlen(sendBuff)); 
	printf("Time sent\n");	
	close(sock);
	printf("Closed\n");
	sleep(1);

#if 0
    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock , message , strlen(message));
     
    message = "Now type something and i shall repeat what you type \n";
    write(sock , message , strlen(message));
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //end of string marker
		client_message[read_size] = '\0';
		
		//Send the message back to client
        write(sock , client_message , strlen(client_message));
		
		//clear the message buffer
		memset(client_message, 0, 2000);
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
#endif
         
    return 0;
} 
