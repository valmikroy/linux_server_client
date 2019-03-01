
struct cmdArgs_t {
  int verbose;
  char *udp_address;
  int udp_port;
  long udp_payload;
  int threads;
  int interval;
} cmdArgs;


void read_random(char *b);
void udpSendtoLoop();
int udpSocket();
void updateReadings(int n);
void displayReadings(long ts);
struct sockaddr_in udpSockaddr(char *addr,int port);
