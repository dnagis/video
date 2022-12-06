/**
 * Juste de quoi comprendre les bases de Mat
 * 
 * 
 * g++ mat_basics.cpp -o mat_basics `pkg-config --cflags --libs opencv4`
 * 
 * 
 * https://docs.opencv.org/4.6.0/d6/d6d/tutorial_mat_the_basic_image_container.html
 * https://docs.opencv.org/4.6.0/db/da5/tutorial_how_to_scan_images.html
 * https://docs.opencv.org/4.6.0/d5/d98/tutorial_mat_operations.html
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
	Mat img;
    img = imread(argv[1], IMREAD_COLOR);
    
    if(img.empty())
	    {
	        cout << "Could not read the image: " << argv[1] << endl;
	        return 1;
	    }
	    

    
    cout << "img.rows: " << img.rows << endl; 
    cout << "img.cols: " << img.cols << endl; 
    cout << "img.depth: " << img.depth() << endl; //img.depth()=0
    cout << "img.channels: " << img.channels() << endl; //img.channels()=3
    
    //Le type (ex.: CV_8UC3)
    //malheureusement impossible de récupérer directement le type, la méthode 'politiquement correcte' est de tester:   
    if(img.type() == CV_8UC3)
		{
	        cout << "Mat type = CV_8UC3" << endl;
	    }    
    //Néanmoins, ce post SO a un tableau (user='empty') qui permet de déduire le type si tu connais le nombre de channels
    //https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
    
    cout << "img.type: " << img.type() << endl; //img.type: 16 --> si 3 channels ce serait CV_8U (donc CV_8UC3) 
    
    
    //Comment accéder à une valeur particulière (initialement pour debug pour comparer avec Python)
    //cout << "img.at(X,Y): " << img.at<uchar>(0,0) << endl; //
    
    //countNonZero() can handle only single channel images (https://answers.opencv.org/question/172784/count-non-zero-error/)
    //cout << "countNonZeros: " << countNonZero(img) << endl; //countNonZero()=
    
    //accéder aux valeurs individuelles
    //https://docs.opencv.org/4.6.0/db/da5/tutorial_how_to_scan_images.html
    uchar* p;
    p = img.ptr<uchar>(0); //un pointer vers le début de la row numéro N
    uchar value1 = *p;
    printf("value1 = %u\n", value1);
    p++;
    uchar value2 = *p;
    printf("value2 = %u\n", value2);
    //cout << "value: " << value << endl;
    
    
    
    return 0;
}


