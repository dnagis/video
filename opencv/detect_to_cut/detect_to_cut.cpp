#include <unistd.h>
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

int INTERVAL_S = 2;
int N_TAIL = 30; //n detections à analyser à la fin (tail)
int MAX_GAP = 3; //longueur d'une sequence de non detection qui signe la fin d'une séquence et pas un gap de detect
int MIN_LENGTH = 5; //taille en dessous de laquelle on estime que séquence pas assez longue

std::vector<int> frames_vector{};
std::vector<int> detect_vector{};

//Overloading << Operator juste pour imprimer les vectors avec un simple std::cout << detect_vector;
//https://www.techiedelight.com/print-vector-cpp/
std::ostream &operator<<(std::ostream &os, const std::vector<int> &input)
{
    for (auto const &i: input) {
        os << i << " ";
    }
    return os;
}

//Cherche si le fichier /proc/<dirent_string>/cmdline contient "process_name"
int parseCmdlineFile(std::string dirent_string, std::string process_name)
{
  std::string filestr = "/proc/"+dirent_string+"/cmdline";
  std::ifstream file(filestr);
  std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  str.c_str();
  std::size_t found = str.find(process_name);

  if (found!=std::string::npos)
	{
		//std::cout << "found at: " << found << '\n';
		return 0;
	}	
	
  return 1;
}


int searchPid(std::string process_name) {
  
  const char *procDirName = "/proc/";
  struct dirent *entry;
  DIR *dp;

  dp = ::opendir(procDirName);
  if (dp == NULL) {
    perror("opendir: Path does not exist or could not be read.");
    return -1;
  }

  //iterate pour chaque dir/fichier dans /proc/, conditionne sur is directory, le cas échéant appelle parseCmdlineFile()
  while ((entry = ::readdir(dp))) {
	
	//d_type == 0x4 : directory
    if (entry->d_type == 0x4) {
		
		if (parseCmdlineFile(std::string(entry->d_name), process_name) == 0) {
			
			return atoi(entry->d_name);
		}
	}
    
  }

  ::closedir(dp);
  return -1;
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
	//std::cout << detections << std::endl;
	
	
	int totDetectSize = detections.size();
	
	//construction d'un subvector
	std::vector<int>::const_iterator first = detections.end() - N_TAIL;
	std::vector<int>::const_iterator end = detections.end();
	
	std::vector<int> tail_detect(first, end);
	
	//std::cout << " taille de tail_detect:" << tail_detect.size() << std::endl;
	//std::cout << tail_detect << std::endl;
	
	return tail_detect;
	
}


int parseResults() {
	
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
  
  
  
  if (detect_vector.size() < N_TAIL) {
    std::cout << "Pas encore " << N_TAIL << " detections --> break" << std::endl;
    return -1;
  }  
  
  
  std::vector<int> tail_detect = getTail(detect_vector);
  
  std::cout << "Tail_detect:" << tail_detect << std::endl;

  int start_index = findStart(tail_detect);
  
  if (start_index != -1) {
	std::cout << "Found a start: " << start_index << std::endl;
	int end_index = findEnd(tail_detect, start_index);  
	
	if (start_index != -1) {
		std::cout << "Found an end: " << end_index << std::endl;
		int seq_length = end_index - start_index;
		std::cout << "Length: " << seq_length << std::endl;
		if (seq_length > MIN_LENGTH) {
			 std::cout << "On a une séquence..." << std::endl;
			}
			//Déduction des indexes de frame start et end
			
			//	déduction des indexes start et end dans le vecteur total
			int index_start_tot = detect_vector.size() - tail_detect.size() + start_index;
			int index_end_tot = detect_vector.size() - tail_detect.size() + end_index;
			//std::cout << "Indexes dans le vector total: start: " << index_start_tot << " end: " << index_end_tot << std::endl;
			
			//	déduction des frames correspondantes
			int frame_start=frames_vector[index_start_tot];
			int frame_end=frames_vector[index_end_tot];
			
			std::cout << "frame start: " << frame_start << " end frame:" << frame_end << std::endl;
			return 0;
		}
	}
  
  return -1;
	
}

int main (int argc, char *argv[]) {
	
	int pid_stdin_to_detect;
	int seq_found;
	
	while (1) {
	std::cout << "Debut loop" << std::endl;
	pid_stdin_to_detect = searchPid("stdin_to_detect");
	
	//std::cout << "pid_stdin_to_detect=" << pid_stdin_to_detect << std::endl;
	
	if (pid_stdin_to_detect == -1) goto end;
	
	seq_found = parseResults();
	
	std::cout << "Resultat de parseResults: " << seq_found << std::endl;
	
	
	end:
	sleep(INTERVAL_S); //#include <unistd.h>
	}
	
	
return 0;
}
