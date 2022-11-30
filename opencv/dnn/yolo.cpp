/**
 * g++ yolo.cpp -o yolo `pkg-config --cflags --libs opencv4`
 * 
 * https://learnopencv.com/deep-learning-based-object-detection-using-yolov3-with-opencv-python-c/
 * 
 */ 

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;

int main()
{
    //! [imread]
    std::string image_path = "/root/starry_night.jpg";
    Mat img = imread(image_path, IMREAD_COLOR);
    //! [imread]

    //! [empty]
    if(img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    //! [empty]

    return 0;
}
