// Server side implementation of UDP client-server model
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define PORT 8080
#define MAXLINE 10240

// Driver code
int main() {
  int sockfd;
  // char buffer = NULL;
  char *hello = "H";
  struct sockaddr_in servaddr, cliaddr;

  // time_t start, stop;
  long start, stop;
  struct timeval timecheck;

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);

  // Bind the socket with the server address
  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  start = 0;
  stop = 0;
  int cnt = 0;
  long int b = 0;
  while (1) {

    int n;
    size_t len;
    char buffer[MAXLINE];
    if ((stop - start) > 1000) {
      // printf("Time elapsed in ms: %ld\n", stop - start);
      gettimeofday(&timecheck, NULL);
      start = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
      printf("pps:  %d  bytes: %ld\n", cnt, b);
      cnt = 0;
      b = 0;
    }

    n = recv(sockfd, (char *)buffer, &len, MSG_WAITALL);
    b = b + n;
    cnt++;

    gettimeofday(&timecheck, NULL);
    stop = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
  }

  return 0;
}
