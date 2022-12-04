/**
 * g++ mat_basics.cpp -o mat_basics `pkg-config --cflags --libs opencv4`
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
	
	
	Mat img;
    img = imread(argv[1], IMREAD_COLOR);
    
    if(img.empty())
	    {
	        cout << "Could not read the image: " << argv[1] << endl;
	        return 1;
	    }
	    
	if(img.type() == CV_8UC3)
		{
	        cout << "img type = CV_8UC3" << endl;
	    }
    
    cout << "img.rows: " << img.rows << endl; 
    cout << "img.cols: " << img.cols << endl; 
    cout << "img.depth: " << img.depth() << endl; //img.depth()=0
    cout << "img.channels: " << img.channels() << endl; //img.channels()=3
    cout << "img.type: " << img.type() << endl; //img.type: 16     
    
    //avec 3 channels et type = 16 ce serait CV_8U selon https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
    
    //Comment accéder à une valeur particulière pour comparer avec Python
    //cout << "img.at(X,Y): " << img.at<uchar>(0,0) << endl; //
    
    //countNonZero() can handle only single channel images (https://answers.opencv.org/question/172784/count-non-zero-error/)
    //cout << "countNonZeros: " << countNonZero(img) << endl; //countNonZero()=
    
    
    return 0;
}


