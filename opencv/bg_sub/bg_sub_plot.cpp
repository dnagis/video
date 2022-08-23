//faire du background substraction (bg_sub.cpp) sur une video, et sur chaque frame compter le nombre de pixels ("non zero") du
//foreground mask résultant. Puis plot



#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/plot.hpp>


using namespace cv;
using namespace std;


int main(int argc, char* argv[])
{

    //create Background Subtractor objects
    Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorKNN();

    VideoCapture capture("cut.mp4") ;
    
    //vector qui va contenir le nb de pixels nonZero comptés par frame, après background substraction (countNonZero)
	std::vector<double> zeroCount;

    Mat frame, fgMask;
    
    while (true) {
        capture >> frame;
        if (frame.empty())
            break;
        //update the background model
        pBackSub->apply(frame, fgMask);        
		zeroCount.push_back( countNonZero(fgMask) );
    }
    
    //int max_count = 0;
    //max_count = *max_element(zeroCount.begin(), zeroCount.end());
    
    //Sur le premier essai, les 4 premières frames sont > 300000, je les passe à 0 
	for (int i = 0; i < 10; i++) {
		zeroCount[i] = 0;
	}
    
    Ptr<plot::Plot2d> plot = plot::Plot2d::create( zeroCount );
    
    Mat plot_img;
    
    plot->setInvertOrientation( true );
    plot->setShowText( false );   
    
    
    plot->render(plot_img);
    

    
    imshow( "Plot Title", plot_img );    
    waitKey();
    
    


    return 0;
}
