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
	string classesFile = "/initrd/mnt/dev_save/packages/cv_dnn_data/detection/yolov3-opencv/object_detection_classes_yolov3.txt";
	ifstream ifs(classesFile.c_str());
	string line;
	while (getline(ifs, line)) classes.push_back(line);
		
	String modelConfiguration = "/initrd/mnt/dev_save/packages/cv_dnn_data/detection/yolov3-opencv/yolov3.cfg";
	String modelWeights = "/initrd/mnt/dev_save/packages/cv_dnn_data/detection/yolov3-opencv/yolov3.weights";
		
	Net net = readNetFromDarknet(modelConfiguration, modelWeights);
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	//net.setPreferableTarget(DNN_TARGET_CPU);
	
	Mat img, blob;
    img = imread(argv[1], IMREAD_COLOR);
   
    blobFromImage(img, blob, 1/255, Size(inpWidth,inpHeight),Scalar(0,0,0), true, false);
    net.setInput(blob);
    
    vector<Mat> outs;
    net.forward(outs, getOutputsNames(net));
    
	//Début processing des résultats
	
	//Dans https://github.com/krutikabapat/DNN-Object-Detection-YOLOv3/blob/master/yolo.cpp
	// c'est la fonction remove_box()
	cout << "Taille du vector out: " << outs.size() << endl; 		//outs.size() = 3
	cout << "Taille du vector out[0]: " << outs[0].size() << endl;	//outs[0].size() = [85 x 507]
	cout << "Taille du vector out[1]: " << outs[1].size() << endl;	//outs[1].size() = [85 x 2028]
	cout << "Taille du vector out[2]: " << outs[2].size() << endl;	//outs[2].size() = [85 x 8112]
	//507 + 2028 + 8112 = 10647
	
	//Voir une row d'un des 3 Mat de length 85
	cout << "une row : " << outs[0].row(208) << endl;
	
	
	
	vector<int> classIds;
    vector<float> confidences;
    
    //outs est un vector<Mat>
    for (size_t i = 0; i < outs.size(); ++i)
    {
        float* data = (float*)outs[i].data;
        //ce ne sont pas trois arguments mais le dernier argument est un "comma operator"
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            //les dimensions d'un scores: scores.rows=1 et scores.cols=80
            //cout << "scores.rows=" << scores.rows << " et scores.cols=" << scores.cols << endl; 
            
            Point classIdPoint;
            double confidence;
            float confThreshold = 0.5; 
            // Get the value and location of the maximum score
            // https://docs.opencv.org/3.4/d2/de8/group__core__array.html#gab473bf2eb6d14ff97e89b355dac20707
            // arg1: input array, arg2: min value (0 car non used), arg3: max value, arg4: min index (non used), arg5: max value index
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            //cout << "confidence=" << confidence << endl;
            if (confidence > confThreshold)
            {
               cout << "classIdPoint=" << classIdPoint.x << endl;
            }
        }
    }
	

    return 0;
}


