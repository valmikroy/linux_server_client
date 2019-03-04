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
#include <sys/utsname.h>
#include <netdb.h>
#include <pthread.h>


#include "udp_client.h"

#define UDP_ADDRESS "127.0.0.1"
#define UDP_PORT 8080
#define UDP_PAYLOAD 1472
#define THREADS 4
#define INTERVAL 60





char hostname[1024];
static const char *optString = "a:p:t:i:s:v?";
pthread_t tid[100];
pthread_mutex_t lock;
long bytes=0;
long count=0;
long start=0;
long stop=0;
struct timeval timecheck;





static const struct option longOpts[] = {
  { "verbose", no_argument,NULL,'v'},
  { "udp_address", required_argument, NULL, 'a' },
  { "udp_port", required_argument, NULL, 'p' },
  { "udp_payload", required_argument, NULL, 's' },
  { "threads", required_argument, NULL, 't' },
  { "interval", required_argument, NULL, 'i' },
  {NULL, 0, NULL, 0}
};

void display_usage() {
    puts( "Simple UDP packet generator" );
    puts( "-v --verbose     \t\t Verbose");
    puts( "-a --udp_address \t\t UDP address");
    puts( "-p --udp_port    \t\t UDP port");
    puts( "-s --udp_payload \t\t UDP payload size");
    puts( "-t --threads \t\t Number of client threads");
    puts( "-i --interval \t\t Timing interval between spawning new threads");
    exit( EXIT_FAILURE );
}

void read_random(char *b) {
    int fp = open("/dev/urandom", O_RDONLY);
    read(fp, &b, sizeof(b));
    close(fp);
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

struct sockaddr_in udpSockaddr(char *addr,int port) {

  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = inet_addr(addr);

  return servaddr;
}

void updateReadings(int n) {
  pthread_mutex_lock(&lock);
  bytes += n;
  count++;
  gettimeofday(&timecheck, NULL);
  stop = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
  pthread_mutex_unlock(&lock);
}

void displayReadings(long ts) {
  pthread_mutex_lock(&lock);
  gettimeofday(&timecheck, NULL);
  start = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
  printf("timestamp=%ld\tpps=%ld\t\tbytes=%ld\t\ttype=send\tdst=%s\tsrc=%s\n", ts, count, bytes, cmdArgs.udp_address,hostname);
  fflush(stdout);
  count = 0;
  bytes = 0;
  pthread_mutex_unlock(&lock);
}


void udpSendtoLoop(){
  int sockfd = udpSocket();
  struct sockaddr_in servaddr = udpSockaddr(cmdArgs.udp_address,cmdArgs.udp_port);

  for (;;) {

    if ((stop - start) > 1000) {
      displayReadings((long)timecheck.tv_sec);
    }
    char buffer[cmdArgs.udp_payload];
    read_random(buffer);
    int n = sendto(sockfd, (const char *)buffer, sizeof(buffer), MSG_CONFIRM,
           (const struct sockaddr *)&servaddr, sizeof(servaddr));
    //printf("bytes sent %d\n",n);

    updateReadings(n);
  }
}

int main(int argc, char *argv[]) {

  int opt = 0;
  int longIndex = 0;

  cmdArgs.verbose = 0;
  get_hostname(hostname, 1024);
  cmdArgs.udp_address = UDP_ADDRESS;
  cmdArgs.udp_port = UDP_PORT;
  cmdArgs.udp_payload = UDP_PAYLOAD;
  cmdArgs.threads = THREADS;
  cmdArgs.interval = INTERVAL;

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
      case 't':
        cmdArgs.threads = atoi(optarg);
        break;
      case 'i':
        cmdArgs.interval = atoi(optarg);
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

  if (pthread_mutex_init(&lock, NULL) != 0) {
    perror("mutex initiation failed");
    exit(EXIT_FAILURE);
  }

  int i;
  for(i = 0; i < cmdArgs.threads ; i++) {
    pthread_create(&(tid[i]), NULL, &udpSendtoLoop, NULL);
    sleep(cmdArgs.interval);
  }
  sleep(cmdArgs.interval);

  for(i = 0; i < cmdArgs.threads ; i++)
    pthread_cancel(tid[i]);


  pthread_exit(NULL);
  pthread_mutex_destroy(&lock);
  return 0;
}
