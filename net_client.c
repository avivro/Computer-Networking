#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SIM_LENGTH 10
/*#define IP_ADDRESS  "127.0.0.1"*/
#define PORT 1337

int main(int argc, char* argv[])///
{
	int sock;
	struct sockaddr_in cli_name;
	int count;
	int value;
	char* hostname;
	struct addrinfo* res;
	/*
	 * struct addrinfo { https://en.wikipedia.org/wiki/Getaddrinfo ,http://beej.us/guide/bgnet/output/html/multipage/getaddrinfoman.html
    int     ai_flags;
    int     ai_family;
    int     ai_socktype;
    int     ai_protocol;
    size_t  ai_addrlen;
    struct  sockaddr* ai_addr;
    char*   ai_canonname;     canonical name
    struct  addrinfo* ai_next;this struct can form a linked list */

	struct sockaddr_in* saddr;

	if (argc != 2) {
		perror("Usage: hostnamelookup <hostname>\n");
		exit(1);
	}

	hostname = argv[1];

	if (0 != getaddrinfo(hostname, NULL, NULL, &res))  /*int getaddrinfo(const char *nodename, const char *servname, const struct addrinfo *hints, struct addrinfo **res);*/
	{
		printf("Error in resolving hostname %s\n", hostname);
		exit(1);
	}

	printf("Client is alive and establishing socket connection.\n");


	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{ perror ("Error opening channel");
	close(sock);
	exit(1);
	}
	saddr = (struct sockaddr_in*)res->ai_addr;


	memset((char *)&cli_name,0, sizeof(cli_name));
	cli_name.sin_family = AF_INET;
	cli_name.sin_addr.s_addr = inet_addr(inet_ntoa(saddr->sin_addr));
	cli_name.sin_port = htons(PORT);


	if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0) // connection and check success//
	{
		perror ("Error establishing communications");
		close(sock);
		exit(1);
	}


	for (count = 1; count <= SIM_LENGTH; count++)
	{ read(sock, &value, 4);
	printf("Client has received %d from socket.\n", value);
	}

	printf("Exiting now.\n");

	close(sock);
	exit(0);

}
