/**
 * 
 * Voir simpleYoloV3 pour des commentaires explicatifs
 * 
 * ./yolo 0.7 horse.jpg
 * 
 * #Compil native
 * g++ yolo.cpp -o yolo `pkg-config --cflags --libs opencv4`
 * 
 * #X-Compil
 * export PKG_CONFIG_LIBDIR=/initrd/mnt/dev_save/packages/opencv-4.6.0/platforms/linux/build/install/lib/pkgconfig/
 * export PATH=$PATH:/initrd/mnt/dev_save/rpi/cross/bin/
 * aarch64-linux-gnu-g++ yolo.cpp -o yolo `pkg-config --cflags --libs opencv4`
 * 
 * https://learnopencv.com/deep-learning-based-object-detection-using-yolov3-with-opencv-python-c/
 * https://github.com/krutikabapat/DNN-Object-Detection-YOLOv3/blob/master/yolo.cpp
 * 
 * Fichier de noms de classes vient de opencv-4.6.0/samples/data/dnn/
 * 
 * Pour yolov3:
 * yolov3.cfg et .weights viennent de https://opencv-tutorial.readthedocs.io/en/latest/yolo/yolo.html
 * je stocke dans
 * /initrd/mnt/dev_save/packages/cv_dnn_data/detection/yolov3-opencv/
 * 
 * Pour tiny yolov3:
 * https://pjreddie.com/darknet/yolo/
 * https://pjreddie.com/media/files/yolov3-tiny.weights
 * https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/yolov3-tiny.cfg
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
	 if(argc != 3) {
      cout << "You need to supply 2 arguments to this program: conf_threshold et input file\n";
      return -1;
	 }
	
	
	
	//PATH dans dev_save sur le NUC
	//string prependPath = "/initrd/mnt/dev_save/packages/cv_dnn_data/detection/yolov3-opencv/";
	
	//PATH dans /root/: nb: marche très bien avec des symlinks
	string prependPath = "/root/";
	
	
	string classesFile = "object_detection_classes_yolov3.txt";
	string pathClassesFile = prependPath + classesFile;
	ifstream ifs(pathClassesFile.c_str());
	string line;
	while (getline(ifs, line)) classes.push_back(line);
		
	String modelConfiguration = "yolov3-tiny.cfg";	//https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/yolov3-tiny.cfg
	//String modelConfiguration = "yolov3.cfg";
	String modelConfigurationPath = prependPath + modelConfiguration;
	
	String modelWeights = "yolov3-tiny.weights"; //https://pjreddie.com/media/files/yolov3-tiny.weights	
	//String modelWeights = "yolov3.weights";
	String modelWeightsPath = prependPath + modelWeights;
		
	Net net = readNetFromDarknet(modelConfigurationPath, modelWeightsPath);
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	//net.setPreferableTarget(DNN_TARGET_CPU);
	
	double confThreshold;
	confThreshold = atof(argv[1]);
	
	String inputFile = argv[2];
	cout << "##### Fichier analysé: " << inputFile << endl;
	
	Mat img, blob;
    img = imread(inputFile, IMREAD_COLOR);
   
	//Création du blob
	//dans le script original tel que téléchargé (première ligne ci dessous) --> toutes les valeurs de CI de l'output sont à zero
    //blobFromImage(img, blob, 1/255, Size(inpWidth,inpHeight),Scalar(0,0,0), true, false);
    
    //Modif du troisième argument: marche mieux:
    blobFromImage(img, blob, 1/255.0, Size(inpWidth,inpHeight) ,Scalar(0,0,0), true, false); //avec ça j'ai des valeurs dans l'output > 0
    
    net.setInput(blob);
    
    vector<Mat> outs;
    net.forward(outs, getOutputsNames(net));
    
	/**Début processing des résultats**/
	

	//Afficher des infos sur les arrays de resultats
	//cout << "Nombre de Mat dans outs: " << outs.size() << endl; 		//outs.size() = 3 en yolov3, 2 en yolov3 tiny
	/*for (size_t i = 0; i < outs.size(); ++i)
	{
		cout << "Taille du vector " << i << " : " << outs[i].size() << endl;
	}*/

	
	//Afficher une row de length 85 d'un des Mat juste pour debug
	//cout << "une row : " << outs[0].row(208) << endl;
	
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
            
            // Get the value and location of the maximum score
            // https://docs.opencv.org/3.4/d2/de8/group__core__array.html#gab473bf2eb6d14ff97e89b355dac20707
            // arg1: input array, arg2: min value (0 car non used), arg3: max value, arg4: min index (non used), arg5: max value index
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            //cout << "confidence=" << confidence << endl;
            //if (confidence > confThreshold && classIdPoint.x == 0)
            if (confidence > confThreshold)
            {
               cout << "class=" << classIdPoint.x << " CI=" << confidence << " raw box: [" << *data << "," << *(data+1) << "," << *(data+2) << "," << *(data+3)<< "]" << endl;
            }
        }
    }
	

    return 0;
}


