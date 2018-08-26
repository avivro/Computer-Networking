#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "Function.h"
#define A 5000 ///port A
#define B 5001 ///port B
#define C 5002 ///port C
#define sizemsg 1024



int main(int argc, char *argv[])
{
  int socket_fd;
  socklen_t fsize;
  struct sockaddr_in  s_in, from, to; //struct receive and sent//
  char msg[sizemsg];

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);///create a new socket with type UDP of Internet Protocol version 4 (IPv4) ///

  memset((char *) &s_in,0, sizeof(s_in)); /* They say you must do this   values in the socket will be zero */

  s_in.sin_family = (short)AF_INET; ///address family//
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
  s_in.sin_port = A; // A port//

  printsin( &s_in, "A_UDP", "Local socket is:");

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));/// check if port is available to operating system//

  while(1) {

    // sending the message to B//
    to.sin_family = (short) AF_INET; ///address family//
	 to.sin_addr.s_addr = htonl(INADDR_ANY);
    to.sin_port = B;// B port//

    fgets(msg,sizemsg,stdin);//get msg from to sending//


    puts(msg);
    if(strcmp(msg,"exit\n") == 0 )//if the msg is "exit" exit all"//
    {
    	strcpy(msg,"A exit");
    	sendto(socket_fd,msg,sizeof(msg),0,(struct sockaddr *)&to,sizeof(to));
    	return 0;
    }

    sendto(socket_fd,msg,sizeof(msg),0,(struct sockaddr *)&to,sizeof(to));//Send the message to B//

	 memset((char *) &to,0, sizeof(to));

	 //Receive from B//

    fsize = sizeof(from);
    recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize); //message from the B//
    printsin(&from, "recv_udp: ", "Packet from:");
    //printf("%u",from.sin_addr.s_addr);
    memset((char *) &from,0, sizeof(from));
    puts(msg);
    bzero(msg,sizemsg);
    fflush(stdout);

  }

  return 0;

}
void printsin(struct sockaddr_in *sin, char *pname, char* msg)
{

	printf("%s\n",pname);
	char *ip=inet_ntoa(sin->sin_addr);
	printf("%s ip=%s,port=%d\n",msg,ip,sin -> sin_port);

}

