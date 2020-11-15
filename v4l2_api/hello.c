

/**
 * https://medium.com/@athul929/capture-an-image-using-v4l2-api-5b6022d79e1d
 * https://www.linuxtv.org/downloads/v4l-dvb-apis-old/capture-example.html
 * 
 * */

#include <stdio.h> 
#include <fcntl.h> 
#include <unistd.h> // for close
#include <errno.h> 
#include <sys/ioctl.h>
#include <linux/videodev2.h>



extern int errno; 

int query_caps(int fd) {
	struct v4l2_capability cap;
	if (-1 == ioctl(fd, VIDIOC_QUERYCAP, &cap)) {
		perror("VIDIOC_QUERYCAP");
		return 1;
	}
	printf("caps=0x%08x\n",cap.capabilities); //0x84a00001
	return 0;

}



int set_format(int fd) {
    struct v4l2_format format = {0};
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = 640;
    format.fmt.pix.height = 360;
    //format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; //définition dans linux/videodev2.h
    //format.fmt.pix.pixelformat = V4L2_PIX_FMT_H264; //juste pour tester
    format.fmt.pix.field = V4L2_FIELD_NONE;
    int res = ioctl(fd, VIDIOC_S_FMT, &format);
    if(res == -1) {
        perror("Could not set format");
        return 1;
    }
    return res;
}

int main() 
{	 

	int fd = open("/dev/video0", O_RDWR); 
	
	printf("fd = %d\n", fd); 
	
	if (fd == -1) 
	{ 
		printf("Error Number % d au open fd\n", errno); 		
	} 
	
	//set_format(fd);
	query_caps(fd);

	
	
	close(fd); 

	return 0; 
} 
