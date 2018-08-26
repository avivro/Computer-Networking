/*
 * Recvudp.c
 *
 *  Created on: Nov 18, 2017
 *      Author: aviv
 */

#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "Function.h"


int main(int argc, char *argv[]){
	int socket_fd, fsize;     ///socket type int call fd, int  number cc int  number fsize ///
	struct sockaddr_in  s_in, from;
	struct { char head; char body[1024]; char tail;} msg;

	socket_fd = socket (AF_INET, SOCK_DGRAM, 0); ///create a new socket with type UDP of Internet Protocol version 4 (IPv4) ///

	/* if (socket_fd < 0) {
  	perror ("recvack_udp:socket");
  	exit (1);
	 */
	memset((char *) &s_in,0, sizeof(s_in)); /* They say you must do this   values in the socket will be zero */

	s_in.sin_family = (short)AF_INET;    ///address family//
	s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */   ///ip//
	s_in.sin_port = htons((u_short)0x3333);        ///convert the decimal to port number-htons() host to network short////


	printsin( &s_in, "RECV_UDP", "Local socket is:");
	fflush(stdout);

	bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in)); //// check if port is available to operating system//


	//  details to recieve informaton//
	while(1)
	{

		// recieve  the message
		fsize = sizeof(from);
	/*	recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);*/
		recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);
		printsin( &from, "recv_udp: ", "Packet from:");
		/*printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail);*/
		printf("Server Got data \n");
		fflush(stdout);

		// Sending back
		strncpy(msg.body, argv[0],1024);
		sendto(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from, sizeof(from));

		//

	}


	return 0;
}


void printsin(struct sockaddr_in *sin, char *pname, char* msg)
{

	printf("%s\n",pname);
	char *ip=inet_ntoa(sin->sin_addr);
	printf("%s ip=%s,port=%d\n",msg,ip,sin -> sin_port);

}

