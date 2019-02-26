// Client side implementation of UDP client-server model
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>

#include "udp_client.h"

#define UDP_ADDRESS "127.0.0.1"
#define UDP_PORT 8080
#define UDP_PAYLOAD 1472




static const char *optString = "a:p:s:v?";
static const struct option longOpts[] = {
  { "verbose", no_argument,NULL,'v'},
  { "udp_address", required_argument, NULL, 'a' },
  { "udp_port", required_argument, NULL, 'p' },
  { "udp_payload", required_argument, NULL, 's' },
  {NULL, 0, NULL, 0}
};

void display_usage() {
    puts( "Simple UDP packet generator" );
    puts( "-v --verbose     \t\t Verbose");
    puts( "-a --udp_address \t\t UDP address");
    puts( "-p --udp_port    \t\t UDP port");
    puts( "-s --udp_payload \t\t UDP payload size");
    exit( EXIT_FAILURE );
}

void read_random(char *b) {
    int fp = open("/dev/urandom", O_RDONLY);
    read(fp, &b, sizeof(b));
    close(fp);
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

struct sockaddr_in udpSockaddr(char *addr,int port) {

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = inet_addr(addr);

  return servaddr;
}



void udpSendtoLoop(){
  int sockfd = udpSocket();
  long start, stop, b, cnt = 0 ;
  char hostname[1024];
  gethostname(hostname, 1024);

  struct timeval timecheck;

  struct sockaddr_in servaddr = udpSockaddr(cmdArgs.udp_address,cmdArgs.udp_port);

  for (;;) {

    if ((stop - start) > 1000) {
      gettimeofday(&timecheck, NULL);
      start = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
      printf("%ld\tpps=%ld\t\tbytes=%ld\t\ttype=send\thost=%s\n",(long)timecheck.tv_sec, cnt, b,hostname);
      cnt = 0;
      b = 0;
    }
    char buffer[cmdArgs.udp_payload];
    read_random(buffer);
    int n = sendto(sockfd, (const char *)buffer, sizeof(buffer), MSG_CONFIRM,
           (const struct sockaddr *)&servaddr, sizeof(servaddr));
    //printf("bytes sent %d\n",n);
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
  cmdArgs.udp_address = UDP_ADDRESS;
  cmdArgs.udp_port = UDP_PORT;
  cmdArgs.udp_payload = UDP_PAYLOAD;

  opt = getopt_long(argc, argv, optString, longOpts, &longIndex );
  while(opt != -1 ) {
    switch(opt){
      case 'v':
        cmdArgs.verbose++;
        break;
      case 'a':
        cmdArgs.udp_address = optarg;
        break;
      case 'p':
        cmdArgs.udp_port = atoi(optarg);
        break;
      case 's':
        cmdArgs.udp_payload = atoi(optarg);
        break;
      case 'h':
      case '?':
        display_usage();
        break;
      default:
        // unreachable
        break;

    }
    opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
  }

  udpSendtoLoop();

  return 0;
}
