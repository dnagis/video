/*
	udp_server.c
	 
	gcc -o server udp_server.c
	
	inspiré de:
	
	https://www.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html
	et
	https://www.binarytides.com/programming-udp-sockets-c-linux/
	 
	si je comprends bien en général le server reçoit
	
	netstat -u -a --> 
	
	Active Internet connections (servers and established)
	Proto Recv-Q Send-Q Local Address           Foreign Address         State       
	...                         
	udp        0      0 0.0.0.0:8888            0.0.0.0:*                           
	...
	
*/
#include <sys/types.h>
#include <stdio.h>	//printf
#include <sys/socket.h>
#include <netinet/in.h> 
#include <string.h> //memset




#define PORT 8888	//The port on which to listen for incoming data
#define BUFSIZE 2048 


int main(void)
{
	struct sockaddr_in myaddr; //notre adresse
	struct sockaddr_in remaddr; //remote addr
	socklen_t addrlen = sizeof(remaddr);            /* length of addresses */
	
	int fd;	
	int recvlen;                    // bytes received
	unsigned char buf[BUFSIZE];     // receive buffer
	
	//create a UDP socket
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket");
		return 0;
	}
	
	// zero out the struct sockaddr_in
	memset((char *)&myaddr, 0, sizeof(myaddr));
	
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(PORT);
	

	
	//bind socket to port
	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}
	
	

    /* now loop, receiving data and printing what we received */
	for (;;) {
		printf("waiting on port %d\n", PORT);
		recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		printf("received %d bytes\n", recvlen);
		if (recvlen > 0) {
				buf[recvlen] = 0;
				printf("received message: \"%s\"\n", buf);
		}
	}


	return 0;
}
