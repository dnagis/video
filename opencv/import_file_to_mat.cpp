/**
 * Import d'une raw frame from gstreamer dans une Mat opencv
 * 
 * gst-launch-1.0 --quiet videotestsrc num-buffers=1 ! video/x-raw,width=640,height=480,format=BGR,framerate=30/1 ! fdsink > image.raw
 * 
 * 
 * g++ import_raw.cpp -o import_raw `pkg-config --cflags --libs opencv4`
 * 
 * ./import_raw image.raw
 * 
 * https://answers.opencv.org/question/58106/unable-to-open-raw-image-through-opencv/
 * 
 * 
 */ 

#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int IMAGE_WIDTH=640;
int IMAGE_HEIGHT=480;

int main( int argc, char** argv)
{
	Mat img;
	FILE *fp = NULL;
	char *imagedata = NULL;
	int framesize = IMAGE_WIDTH * IMAGE_HEIGHT;
	
	fp = fopen(argv[1], "rb");
    printf("Fichier à ouvrir:%s\n", argv[1]);	
	
	//Memory allocation for image data buffer.
	imagedata = (char*) malloc (sizeof(char) * framesize * 3);
	
	
	//Read image data and store in buffer.
	fread(imagedata, sizeof(char), framesize * 3, fp);
    
	//Create Opencv mat structure for image dimension. CV_8UC3.
	//https://docs.opencv.org/4.6.0/d3/d63/classcv_1_1Mat.html#a55ced2c8d844d683ea9a725c60037ad0
	img.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);
    
    memcpy(img.data, imagedata, framesize*3);

	free(imagedata);

	fclose(fp);
	
	
	
	imshow("Display window", img);
    int k = waitKey(0); // Wait for a keystroke in the window
    
    
    
    return 0;
}


