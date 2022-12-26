/**
 * Importer de la data (je suppose frame) venant de gstreamer type:
 * 
 * gst-launch-1.0 --quiet videotestsrc pattern=5 num-buffers=1 ! video/x-raw,width=640,height=480,format=BGR,framerate=30/1 ! fdsink > image.raw
 * 
 * 
 * g++ import_raw.cpp -o import_raw `pkg-config --cflags --libs opencv4`
 * 
 * 
 * 
 * 
 * 
 */ 

#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



int main( int argc, char** argv)
{
	//Mat img;
    
    printf("Hello\n");
    
    
    
    
    return 0;
}


