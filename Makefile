default: all

all:
	-gcc -o tcp_server tcp_server.c
	-gcc -o tcp_client tcp_client.c
	-gcc -o udp_server udp_server.c
	-gcc -o udp_client udp_client.c

clean:
	-rm -rf tcp_server tcp_client udp_server udp_client
