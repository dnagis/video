//opencv_contrib/modules/plot/samples/plot_demo.cpp
//https://gist.github.com/UnaNancyOwen/ae41d341c054f0b1193839ecf03bb01e

#include <opencv2/highgui.hpp>
#include <opencv2/plot.hpp>
#include <vector>
#include <cmath>

using namespace cv;

int main( int argc, char* argv[] )
{
    std::vector<double> sine;
    for( int t = 0; t < 360; t++ ){
        sine.push_back( std::sin( t * CV_PI / 180.0 ) );
    }


    Mat data( sine );
    
    Ptr<plot::Plot2d> plot = plot::Plot2d::create( data );
    
    Mat image;
    plot->render(image);

	
    imshow( "Plot Title", image );    
    
    waitKey();
    
    


    return 0;
}
