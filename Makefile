default: all

all:
	-gcc -o tcp/simple_loop/tcp_server tcp/simple_loop/tcp_server.c
	-gcc -o tcp/simple_loop/tcp_client tcp/simple_loop/tcp_client.c
	-gcc -o udp/simple/udp_server udp/simple/udp_server.c
	-gcc -o udp/simple/udp_client udp/simple/udp_client.c

clean:
	-rm -rf tcp/simple_loop/tcp_server tcp/simple_loop/tcp_client udp/simple/udp_server udp/simple/udp_client
