

/**
 * https://medium.com/@athul929/capture-an-image-using-v4l2-api-5b6022d79e1d
 * 
 * */

#include <stdio.h> 
#include <fcntl.h> 
#include <unistd.h> // for close
#include <errno.h> 
#include <linux/videodev2.h>



extern int errno; 


int main() 
{	 

	int fd = open("/dev/video0", O_RDWR); 
	
	printf("fd = %d\n", fd); 
	
	if (fd ==-1) 
	{ 
		printf("Error Number % d\n", errno); 		
		perror("Program");				 
	} 
	
	close(fd); 
 
	

	return 0; 
} 
