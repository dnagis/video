/**
 * Import de raw frames from gstreamer dans une Mat opencv via fdsink -> stdin 
 * 
 * g++ stdin_to_mat.cpp -o stdin_to_mat `pkg-config --cflags --libs opencv4`
 * 
 * gst-launch-1.0 --quiet videotestsrc is-live=true ! clockoverlay ! video/x-raw,width=640,height=480,format=BGR,framerate=30/1 ! fdsink | ./stdin_to_mat
 * 
 * kill -s SIGINT `pidof gst-launch-1.0`
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


		
		//Création d'une Mat
		img.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);
		
		//Memory allocation for image data buffer.
		imagebuffer = (char*) malloc (sizeof(char) * framesize);		
		
		while (!cin.eof()) {
			cout << "Début loop read buffer" << endl;
			

			
			//Rx depuis stdin ref: https://cplusplus.com/reference/istream/istream/read/
			cin.read(imagebuffer, framesize);
			memcpy(img.data, imagebuffer, framesize);
			

				
		}
		
			free(imagebuffer);		
		
		
		
		
		
		//Affichage
		//imshow("Display window", img);
		//int k = waitKey(0); // Wait for a keystroke in the window
		
		cout << "Fin du programme" << endl;
		return 0;

}
