// Write CPP code here 
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <time.h>
#include <unistd.h>
#include <netinet/tcp.h>

#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 


void readfunc(int sockfd)
{
	char buff[MAX]; 
	unsigned long t;
        //int flags =1;
        //setsockopt(sockfd, SOL_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags));

	for (;;) { 
		bzero(buff, MAX); 
		sprintf(buff,"%d",(unsigned long)time(NULL));
		write(sockfd,buff,sizeof(buff));


		bzero(buff, MAX); 
		read(sockfd, buff, sizeof(buff)); 
		printf("From Server : %s\n", buff); 
		fflush(stdout);
		sleep(2);

	}
}


int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

        readfunc(sockfd);

	// close the socket 
	close(sockfd); 
} 

