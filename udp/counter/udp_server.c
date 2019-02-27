// Server side implementation of UDP client-server model
#include <arpa/inet.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <netdb.h>


#include "udp_server.h"

#define UDP_ADDRESS "127.0.0.1"
#define UDP_PORT 8080
#define UDP_PAYLOAD 10240

char hostname[1024];
static const char *optString = "a:p:v?";
static const struct option longOpts[] = {
    {"verbose", no_argument, NULL, 'v'},
    {"udp_address", required_argument, NULL, 'a'},
    {"udp_port", required_argument, NULL, 'p'},
    {NULL, 0, NULL, 0}};

void display_usage() {
  puts("Simple UDP server for incoming packet tracking");
  puts("-v --verbose     \t\t Verbose");
  puts("-a --udp_address \t\t UDP address");
  puts("-p --udp_port    \t\t UDP port");
  exit(EXIT_FAILURE);
}

void get_hostname(){
        gethostname(hostname,1024);
        struct addrinfo hints={0};
        struct addrinfo* res=0;
        hints.ai_family=AF_UNSPEC;
        hints.ai_flags=AI_CANONNAME;

        if (getaddrinfo(hostname,0,&hints,&res)==0) {
            strcpy(hostname,res->ai_canonname);
            freeaddrinfo(res);
        }
        else {
          strcpy(hostname,UDP_ADDRESS);
        }

}

int udpSocket() {
  int sockfd;
  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  return sockfd;
}

struct sockaddr_in udpSockaddr(char *addr, int port) {

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = inet_addr(addr);

  return servaddr;
}

int udpBindSocket() {

  int sockfd = udpSocket();
  struct sockaddr_in servaddr =
      udpSockaddr(cmdArgs.udp_address, cmdArgs.udp_port);

  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  return sockfd;
}

void udpRecvLoop(int sockfd) {

  long start=0;
  long stop=0;
  long b=0;
  long cnt=0;
  size_t len;
  struct timeval timecheck;
  char buffer[UDP_PAYLOAD];

  for (;;) {

    if ((stop - start) > 1000) {
      gettimeofday(&timecheck, NULL);
      start = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
      printf("%ld\tpps=%ld\t\tbytes=%ld\t\ttype=recv\tlistento=%s\n", (long)timecheck.tv_sec, cnt, b, cmdArgs.udp_address);
      fflush(stdout);
      cnt = 0;
      b = 0;
    }

    int n = recv(sockfd, (char *)buffer, &len, MSG_WAITALL);

    b += n;
    cnt++;
    gettimeofday(&timecheck, NULL);
    stop = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
  }
}

int main(int argc, char *argv[]) {

  int opt = 0;
  int longIndex = 0;

  cmdArgs.verbose = 0;
  get_hostname(hostname, 1024);
  cmdArgs.udp_address = UDP_ADDRESS;
  cmdArgs.udp_port = UDP_PORT;

  opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
  while (opt != -1) {
    switch (opt) {
    case 'v':
      cmdArgs.verbose++;
      break;
    case 'a':
      cmdArgs.udp_address = optarg;
      break;
    case 'p':
      cmdArgs.udp_port = atoi(optarg);
      break;
    case 'h':
    case '?':
      display_usage();
      break;
    default:
      // unreachable
      break;
    }
    opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
  }

  int sockfd = udpBindSocket();
  udpRecvLoop(sockfd);

  return 0;
}
