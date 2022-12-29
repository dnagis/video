/**
 * Import d'une raw frame from gstreamer dans une Mat opencv via stdin 
 * 
 * g++ import_stdin_to_mat.cpp -o import_stdin_to_mat `pkg-config --cflags --libs opencv4`
 * 
 * gst-launch-1.0 --quiet videotestsrc num-buffers=1 ! video/x-raw,width=640,height=480,format=BGR,framerate=30/1 ! fdsink | ./import_stdin_to_mat
 * 
 * 
 * 
*/


#include <iostream>  
#include <opencv2/opencv.hpp>

using namespace std;  
using namespace cv;

int IMAGE_WIDTH=640;
int IMAGE_HEIGHT=480;
int IMAGE_DEPTH=3;

int main()

{
	
		char *imagebuffer = NULL;
		int framesize = IMAGE_WIDTH * IMAGE_HEIGHT * IMAGE_DEPTH;
		Mat img;

		//Memory allocation for image data buffer.
		imagebuffer = (char*) malloc (sizeof(char) * framesize);
		
		//Rx depuis stdin ref: https://cplusplus.com/reference/istream/istream/read/
		cin.read(imagebuffer, framesize);
		
		img.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);
		
		memcpy(img.data, imagebuffer, framesize);
		
		free(imagebuffer);
		
		imshow("Display window", img);
		int k = waitKey(0); // Wait for a keystroke in the window
		
		return 0;

}
