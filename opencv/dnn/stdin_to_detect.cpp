
/**
 * 
 * read() de toutes les frames captured en stdin (Rx depuis un fdsink gstreamer), et detection yolo sur une partie des frames (30 detections par secondes pas possible)
 * 
 * Threads nécessaires: un pour le read (qui doit importer toutes les frames), l'autre pour lancer la détection yolo
 * ***pour les threads la compil nécessite un flag particulier: -pthread***
 * 	Hello world threads trouvé avec recherche 'c++ threads': https://cplusplus.com/reference/thread/thread/
 * 
 * Compil:
 * 
 * g++ -pthread stdin_to_detect.cpp -o stdin_to_detect `pkg-config --cflags --libs opencv4`
 *  
 * aarch64-linux-gnu-g++ -pthread stdin_to_detect.cpp -o stdin_to_detect `pkg-config --cflags --libs opencv4`
 * 
 * La partie détection yolo vient de:
 * 		--> video/opencv/dnn/yolo.cpp
 * J'ai travaillé sur l'import de raw frames depuis une file puis depuis stdin (pour fdsink) dans:
 * 		--> video/opencv/file_to_mat.cpp
 * 		--> video/opencv/stdin_to_mat.cpp
 * 
 * Pipeline simple pour tests
 * **ATTENTION avec videotestsrc sans is-live=true on est bien au dessus de 30 fps, bien que le capsfilter contienne framerate=30/1***
 * gst-launch-1.0 --quiet videotestsrc is-live=true ! video/x-raw,width=640,height=480,format=BGR,framerate=30/1 ! fdsink | ./stdin_to_detect
 * 
 * kill -s SIGINT `pidof gst-launch-1.0`
 * 
 * Les pipelines avec tee rpicamsrc / v4l2src sont dans video/gst/LOG_fdsink
 * 
 * #fpsdisplaysink pour voir le compte de frames
 * gst-launch-1.0 filesrc location=capture.mp4 ! qtdemux ! vaapih264dec ! videorate ! videoconvert ! fpsdisplaysink
 * 
 * */ 


#include <fstream>
#include <sstream>
#include <iostream>       
#include <thread>     
#include <unistd.h>
#include <opencv2/opencv.hpp>
// Required for dnn modules.
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
  

using namespace std;
using namespace cv;
using namespace dnn;


//Pour chercher les fichiers de config dnn au bon endroit selon si sur le RPi ou sur le NUC/XPS13
bool ARM=true;

int frame_n=0; //le compteur de frame "de base"
int detect_frame; //il faut figer le numéro de la frame au lancement dun detect sinon il continue à augmenter dans l'autre thread

int IMAGE_WIDTH=480;
int IMAGE_HEIGHT=640;
int IMAGE_DEPTH=3;

char *imagebuffer = NULL;
int framesize = IMAGE_WIDTH * IMAGE_HEIGHT * IMAGE_DEPTH;

Mat img, blob;

ofstream results_file;

//dnn
double confThreshold;
int searched_class_id = 0; //la classe recherchée, 0 = person

int inpWidth = 416;
int inpHeight = 416;
vector<string> classes;
Net net;

//dnn: Get the names of the output layers
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

void process_results(vector<Mat> outs) {
	bool detect_pos = false;
	
	cout << "	On a un résultat" << endl;
	//results_file << "	On a un résultat" << endl;
	//cout << "	Nombre de Mat dans vector<Mat> outs: " << outs.size() << endl;
		
	vector<int> classIds;
    vector<float> confidences;
    
    for (size_t i = 0; i < outs.size(); ++i)
    {
        float* data = (float*)outs[i].data;
        
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            Point classIdPoint;
            double confidence;
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > confThreshold)
            {
				if (classIdPoint.x==searched_class_id) detect_pos = true; 	
				cout << "	class=" << classIdPoint.x << " CI=" << confidence << " raw box: [" << *data << "," << *(data+1) << "," << *(data+2) << "," << *(data+3)<< "]" << endl;
				//results_file << "	class=" << classIdPoint.x << " CI=" << confidence << " raw box: [" << *data << "," << *(data+1) << "," << *(data+2) << "," << *(data+3)<< "]" << endl;
            }
        }
    }
    string result = (detect_pos) ? "1" : "0";
    cout << "bilan frame n°" << detect_frame << " détection = " << result << endl; 
    results_file << detect_frame << " " << result << endl;	
}


//Le thread qui va importer les frames via fdsink (cin) 
void read_stdin() 
{
		//Memory allocation for image data buffer.
		imagebuffer = (char*) malloc (sizeof(char) * framesize);		
		
		while (!cin.eof()) {
			//cout << "Début loop read d un buffer" << endl;
			//Rx depuis stdin ref: https://cplusplus.com/reference/istream/istream/read/
			cin.read(imagebuffer, framesize);
			memcpy(img.data, imagebuffer, framesize);
			frame_n++;
		}		
		free(imagebuffer);	
}

//Le thread qui initie le travail de détection
void detect()
{
		while (!cin.eof()) {
			detect_frame = frame_n;
			cout << "Début loop dans thread detect on est sur la frame n° " << detect_frame << endl;
						
			//imshow("Display window", img);
			//waitKey(3000); // opencv2/opencv.hpp en millisecondes
			//destroyAllWindows();
			
			blobFromImage(img, blob, 1/255.0, Size(inpWidth,inpHeight) ,Scalar(0,0,0), true, false);
			net.setInput(blob);
		    vector<Mat> outs;
		    net.forward(outs, getOutputsNames(net));
		    process_results(outs);
		    
		    //Principe de précaution utilisé à partir du début par peur de crash, j'essaie d'enlever le 230123
		    //sleep(2); //unistd.h en secondes
		}
}



int main(int argc, char *argv[]) 
{
	//confThreshold en premier
	if (argc < 2) {
		cout << "pas assez d arguments" << endl;		
		return 1;
		}
	confThreshold = atof(argv[1]);
	
	
	
	results_file.open("results.txt");
	
	//Création d'une Mat img qui va recevoir la frame passée à read_stdin via stdin
	img.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);	

	
	/**Préparation net pour dnn**/
	
	//Path où je mets les fichiers de configuration dnn
	string prependPath = (ARM) ? "/root/" : "/initrd/mnt/dev_save/packages/cv_dnn_data/detection/yolov3-opencv/";
	
	string classesFile = "object_detection_classes_yolov3.txt";
	string pathClassesFile = prependPath + classesFile;
	ifstream ifs(pathClassesFile.c_str());
	string line;
	while (getline(ifs, line)) classes.push_back(line);
		
	String modelConfiguration = (ARM) ? "yolov3-tiny.cfg" : "yolov3.cfg"; //https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/yolov3-tiny.cfg
	String modelConfigurationPath = prependPath + modelConfiguration;

	String modelWeights = (ARM) ? "yolov3-tiny.weights" : "yolov3.weights"; //https://pjreddie.com/media/files/yolov3-tiny.weights	
	String modelWeightsPath = prependPath + modelWeights;
		
	net = readNetFromDarknet(modelConfigurationPath, modelWeightsPath);
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	//net.setPreferableTarget(DNN_TARGET_CPU);
	
	
	
	
	
	
	

	/**Démarrage (spawn) des deux threads**/
	
	thread first (read_stdin);     // spawn new thread that calls read_stdin() nb on peut passer des arguments, à partir d'arg2
	thread second (detect);  // spawn new thread that calls detect(0)
	
	//cout << "main, read_stdin and detect now execute concurrently...\n";
	
	// synchronize threads:
	first.join();                // pauses until first finishes
	second.join();               // pauses until second finishes
	
	//cout << "Fin du programme read_stdin and detect completed.\n";
	
	results_file << "Derniere frame vue " << frame_n << endl;
	
	results_file.close();
	
	cout << "Fin du programme frame_n=" << frame_n << endl;
	
	return 0;
}
