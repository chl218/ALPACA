all:
	gcc -o tcp_server tcp_server.cpp -lwiringPi -lpthread
	gcc -o udp_server udp_server.cpp -lwiringPi -lpthread
	gcc -o udp udp.cpp -lwiringPi -lpthread

clean:
	rm tcp_server udp_server
