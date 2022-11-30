/**
 * #Compil native
 * g++ yolo.cpp -o yolo `pkg-config --cflags --libs opencv4`
 * 
 * #X-Compil
 * export PKG_CONFIG_LIBDIR=/initrd/mnt/dev_save/packages/opencv-4.6.0/platforms/linux/build_aarch64/install/lib/pkgconfig/
 * export PATH=$PATH:/initrd/mnt/dev_save/rpi/cross/bin/
 * aarch64-linux-gnu-g++ yolo.cpp -o yolo `pkg-config --cflags --libs opencv4`
 * 
 * https://learnopencv.com/deep-learning-based-object-detection-using-yolov3-with-opencv-python-c/
 * https://github.com/krutikabapat/DNN-Object-Detection-YOLOv3/blob/master/yolo.cpp
 * 
 * Fichier de noms de classes vient de opencv-4.6.0/samples/data/dnn/
 * yolov3.cfg et .weights viennent de https://opencv-tutorial.readthedocs.io/en/latest/yolo/yolo.html
 * je stocke dans
 * /initrd/mnt/dev_save/packages/cv_dnn_data/detection/yolov3-opencv/
 * 
 */ 

#include <fstream>
#include <sstream>
#include <iostream>
// Required for dnn modules.
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace dnn;

// confidence threshold
float conf_threshold = 0.5;
// nms threshold
float nmsThreshold = 0.4;
int inpWidth = 416;
int inpHeight = 416;

vector<string> classes;

// Get the names of the output layers
vector<Mat> outs;
vector<String> getOutputsNames(const Net& net)
{
    static vector<String> names;
    if (names.empty())
    {
        //Get the indices of the output layers, i.e. the layers with unconnected outputs
        vector<int> outLayers = net.getUnconnectedOutLayers();
        
        //get the names of all the layers in the network
        vector<String> layersNames = net.getLayerNames();
        
        // Get the names of the output layers in names
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
        names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

int main( int argc, char** argv)
{
	string classesFile = "object_detection_classes_yolov3.txt";
	ifstream ifs(classesFile.c_str());
	string line;
	while (getline(ifs, line)) classes.push_back(line);
		
	String modelConfiguration = "yolov3.cfg";
	String modelWeights = "yolov3.weights";
		
	Net net = readNetFromDarknet(modelConfiguration, modelWeights);
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	net.setPreferableTarget(DNN_TARGET_CPU);
	
	Mat frame, blob;
    frame = imread(argv[1]);
    blobFromImage(frame, blob, 1/255, Size(inpWidth,inpHeight),Scalar(0,0,0), true, false);
    net.setInput(blob);
    
    vector<Mat> outs;
    net.forward(outs, getOutputsNames(net));

    return 0;
}


