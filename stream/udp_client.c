/*
 * le client, qui sendto()
 * 
 * 
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>	/* for fprintf */
#include <string.h>	/* for memcpy */
#include <netinet/in.h> 
#include <arpa/inet.h> /* inet_pton */

#define SERVER "5.135.183.126"
#define PORT 8888

int main(void)
{

	int fd;	
	struct sockaddr_in servaddr;    /* server address */
	char *my_message = "this is a test message";
	
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket");
		return 0;
	}
	
	/* fill in the server's address and data */
	memset((char*)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);	
	
	// put the host's address into the server address structure */
	inet_pton(AF_INET, SERVER, &(servaddr.sin_addr));
	
	/* send a message to the server */
	if (sendto(fd, my_message, strlen(my_message), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("sendto failed");
		return 0;
	}

}
