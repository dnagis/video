//opencv_contrib/modules/plot/samples/plot_demo.cpp
//https://gist.github.com/UnaNancyOwen/ae41d341c054f0b1193839ecf03bb01e
//ce qui s'affiche en haut à gauche c'est la valeur du dernier point

#include <opencv2/highgui.hpp>
#include <opencv2/plot.hpp>
#include <vector>
#include <cmath>

using namespace cv;

int main( int argc, char* argv[] )
{
    std::vector<double> mon_vector;
    for( int f = 0; f < 1500; f++ ){
        //mon_vector.push_back( std::sin( t * CV_PI / 180.0 ) );
        mon_vector.push_back( f*-4 );
    }


    Mat data( mon_vector );
    
    Ptr<plot::Plot2d> plot = plot::Plot2d::create( data );
    
    Mat image;
    plot->render(image);
    imshow( "Plot Title", image );    
    waitKey();

    return 0;
}
