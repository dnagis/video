
/**
 * Objectif: read() des frames en stdin, et sur une frame de temps en temps faire une detection
 * 
 * Attention pour les threads la compil nécessite un flag particulier: -pthread
 * Hello world threads trouvé avec recherche 'c++ threads'
 * https://cplusplus.com/reference/thread/thread/
 * 
 * g++ -pthread stdin_to_detect.cpp -o stdin_to_detect `pkg-config --cflags --libs opencv4`
 *  
 * aarch64-linux-gnu-g++ -pthread stdin_to_detect.cpp -o stdin_to_detect #reste à ajouter pkg config opencv
 * 
 * gst-launch-1.0 --quiet videotestsrc is-live=true ! clockoverlay ! video/x-raw,width=640,height=480,format=BGR,framerate=30/1 ! fdsink | ./stdin_to_detect
 * 
 * kill -s SIGINT `pidof gst-launch-1.0`
 * 
 * */ 



#include <iostream>       
#include <thread>     
#include <unistd.h>
#include <opencv2/opencv.hpp>    

using namespace std;
using namespace cv;

int IMAGE_WIDTH=640;
int IMAGE_HEIGHT=480;
int IMAGE_DEPTH=3;

char *imagebuffer = NULL;
int framesize = IMAGE_WIDTH * IMAGE_HEIGHT * IMAGE_DEPTH;

Mat img;


 
void read_stdin() 
{
		//Memory allocation for image data buffer.
		imagebuffer = (char*) malloc (sizeof(char) * framesize);		
		
		while (!cin.eof()) {
			//cout << "Début loop read d un buffer" << endl;
			//Rx depuis stdin ref: https://cplusplus.com/reference/istream/istream/read/
			cin.read(imagebuffer, framesize);
			memcpy(img.data, imagebuffer, framesize);
		}		
		free(imagebuffer);	
}

void detect()
{
		while (!cin.eof()) {
			cout << "Début loop detect" << endl;
			sleep(5); //unistd.h en secondes
			imshow("Display window", img);
			waitKey(3000); // opencv2/opencv.hpp en millisecondes
			destroyAllWindows();
		}
}

int main() 
{
	//Création d'une Mat
	img.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);	


	thread first (read_stdin);     // spawn new thread that calls read_stdin() nb on peut passer des arguments, à partir d'arg2
	thread second (detect);  // spawn new thread that calls detect(0)
	
	cout << "main, read_stdin and detect now execute concurrently...\n";
	
	// synchronize threads:
	first.join();                // pauses until first finishes
	second.join();               // pauses until second finishes
	
	cout << "read_stdin and detect completed.\n";
	
	return 0;
}
