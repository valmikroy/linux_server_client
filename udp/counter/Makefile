CC=gcc -v


CFLAGS+=-Wall -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+=-Wmissing-declarations -Wshadow -Wpointer-arith -Wcast-qual -Wdeprecated-declarations
CFLAGS+=-Wsign-compare -Iincludes -g
LDFLAGS+=-lpthread -pthread

all:
	$(CC) $(CFLAGS) $(LDFLAGS) udp_client.c -o udp_client
	$(CC) $(CFLAGS) udp_server.c -o udp_server

clean:
	rm -f udp_client udp_server


.PHONY: clean
