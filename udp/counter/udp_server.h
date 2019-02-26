
struct cmdArgs_t {
  int verbose;
  char *udp_address;
  int udp_port;
} cmdArgs;


int udpSocket();
struct sockaddr_in udpSockaddr(char *addr,int port);
int udpBindSocket();
void udpRecvLoop( int sockfd );
