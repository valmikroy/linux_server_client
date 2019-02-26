#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdio.h>
#include <sys/socket.h> 
#include <sys/types.h> 
#include <time.h>
#include <unistd.h>
#include <netinet/tcp.h>

#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 



void write_loop(int sockfd)
{
	
	char buff[MAX]; 
	for (;;) {
		bzero(buff, MAX); 
		sprintf(buff,"%d",(unsigned long)time(NULL));
		write(sockfd, buff, sizeof(buff));
		sleep(2);
	}
}



void timefunc(int sockfd)
{
	char buff[MAX]; 
        //int flags =1;
        //setsockopt(sockfd, SOL_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags));


	for (;;) { 
		bzero(buff, MAX); 
		read(sockfd, buff, sizeof(buff)); 
		printf("From Cient: %s\n", buff); 
		fflush(stdout);

		bzero(buff, MAX); 
		sprintf(buff,"%d",(unsigned long)time(NULL));
		write(sockfd, buff, sizeof(buff));
		sleep(2);
		
	}
}




// Driver function 
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	//timefunc(connfd);
	write_loop(connfd);

	// After chatting close the socket 
	close(sockfd); 
} 

