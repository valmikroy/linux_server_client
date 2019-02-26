
struct cmdArgs_t {
  int verbose;
  char *udp_address;
  int udp_port;
  long udp_payload;
} cmdArgs;


void read_random(char *b);
int udpSocket();
struct sockaddr_in udpSockaddr(char *addr,int port);
void udpSendtoLoop();
