#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

std::vector<int> frames_vector{};
std::vector<int> detect_vector{};

int N_TAIL = 30; //n detections à analyser à la fin (tail)
int MAX_GAP = 3; //longueur d'une sequence de non detection qui signe la fin d'une séquence et pas un gap de detect
int MIN_SIZE = 5; //taille en dessous de laquelle on estime que séquence pas assez longue

//Overloading << Operator juste pour imprimer les vectors avec un simple std::cout << detect_vector;
//https://www.techiedelight.com/print-vector-cpp/
std::ostream &operator<<(std::ostream &os, const std::vector<int> &input)
{
    for (auto const &i: input) {
        os << i << " ";
    }
    return os;
}


//dans un vector de detections chercher après l'index start la première occurence d'une série de 
//n=MAX_GAP 0
//https://www.educative.io/answers/how-to-use-the-search-function-in-cpp
int findEnd(std::vector<int> detections, int start) {
	
	std::vector<int> n_zeros(MAX_GAP, 0); //un vector de MAX_GAP zeros
	
	auto it = search(detections.begin() + start, detections.end(), n_zeros.begin(), n_zeros.end());
	
	if (it == detections.end()) 
		{
			//std::cout << "End not found" << std::endl;
			return -1;			
		} else
		{
			auto index = std::distance(detections.begin(), it);
			//std::cout << "End found at position " << index << std::endl;
			//std::cout << "End found at position " << (it-detections.begin()) << std::endl;
			return index;
		}
}

//dans un vector de detections chercher la première occurence de 1, renvoyer index, sinon -1
int findStart(std::vector<int> detections) {
	
	auto it = std::find(detections.begin(), detections.end(), 1);
	
	if (it == detections.end())
		{
			return -1; //not found 
		} else
		{
			auto index = std::distance(detections.begin(), it);
			return index;
		}
}


//obtenir un subvector de la fin du vecteur de detections (style tail)
std::vector<int> getTail(std::vector<int> detections) {
	std::cout << detections << std::endl;
	
	
	int totDetectSize = detections.size();
	
	//construction d'un subvector
	std::vector<int>::const_iterator first = detections.end() - N_TAIL;
	std::vector<int>::const_iterator end = detections.end();
	
	std::vector<int> tail_detect(first, end);
	
	//std::cout << " taille de tail_detect:" << tail_detect.size() << std::endl;
	//std::cout << tail_detect << std::endl;
	
	return tail_detect;
	
}

//Ouvre le fichier /root/results.txt, getline sur chaque ligne, regex qq chose type 634 0, 
//Je ne peux pas renvoyer deux vectors pour frames et detection donc wide scope variables
void parseResults() {
	
	
  std::ifstream results_file("/root/results.txt");
  
  if (!results_file.is_open()) {
	std::cerr << "Error opening the file" << std::endl;
	//return 1;
  }
  
  //regex pour les lignes: frame number ([0-9]*), puis un espace, puis 0 ou 1 ([01])
  //https://en.cppreference.com/w/cpp/regex
  std::regex ma_regex("^[0-9]* [01]");
  
  std::string line;
	while (std::getline(results_file, line)) {
	if (std::regex_search(line, ma_regex)) {
        //std::cout << "regex match\n";
        //std::cout << "The size of line is " << line.size() << " bytes.\n";
        std::string frame = line.substr (0,line.size()-2); 
        std::string detect = line.substr (line.size()-1,1);
        //std::cout << line << " frame=" << frame << " detect=" << detect << std::endl;
        
        int frame_int=atoi(frame.c_str()); //atoi(): You have to call the c_str() function of the string class when passing it to standard C functions. It returns you a const char*
        frames_vector.push_back(frame_int);    
              
        int detect_int=atoi(detect.c_str()); //atoi(): You have to call the c_str() function of the string class when passing it to standard C functions. It returns you a const char*
        detect_vector.push_back(detect_int);
    }
  }
  results_file.close();
	
}





int main (int argc, char *argv[]) {

  parseResults(); //doit remplir detect_vector
  
  //std::cout << frames_vector;
  //std::cout << detect_vector;
  
  //std::cout << " taille de detect_vector après parseResults:" << detect_vector.size() << std::endl;
  
  std::vector<int> tail_detect = getTail(detect_vector);
  
  std::cout << "Tail_detect size:" << tail_detect.size() << std::endl;
  std::cout << tail_detect << std::endl;

  int start_index = findStart(tail_detect);
  
  std::cout << "Start:" << start_index << std::endl;
  
  if (start_index != -1) {
	int end_index = findEnd(tail_detect, start_index);  
	std::cout << "End: " << end_index << std::endl;
	
	
	
	
	}
  
  return 0;
}
