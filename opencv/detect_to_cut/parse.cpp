#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

std::vector<int> frames_vector{};
std::vector<int> detect_vector{};

//Overloading << Operator juste pour imprimer les vectors à la fin
//https://www.techiedelight.com/print-vector-cpp/
std::ostream &operator<<(std::ostream &os, const std::vector<int> &input)
{
    for (auto const &i: input) {
        os << i << " ";
    }
    return os;
}


void parseResults() {
	
	
  std::ifstream results_file("/root/results.txt");
  
  if (!results_file.is_open()) {
	std::cerr << "Error opening the file" << std::endl;
	//return 1;
  }
  
  //regex pour les lignes frame number, un espace, puis 0 ou 1
  //https://en.cppreference.com/w/cpp/regex
  std::regex ma_regex("^[0-9]* [01]");
  
  std::string line;
	while (std::getline(results_file, line)) {
	
	
	if (std::regex_search(line, ma_regex)) {
        //std::cout << "regex match\n";
        //std::cout << "The size of line is " << line.size() << " bytes.\n";
        std::string frame = line.substr (0,line.size()-2); 
        std::string detect = line.substr (line.size()-1,1);
        std::cout << line << " frame=" << frame << " detect=" << detect << std::endl;
        
        int frame_int=atoi(frame.c_str()); //atoi(): You have to call the c_str() function of the string class when passing it to standard C functions. It returns you a const char*
        frames_vector.push_back(frame_int);    
              
        int detect_int=atoi(detect.c_str()); //atoi(): You have to call the c_str() function of the string class when passing it to standard C functions. It returns you a const char*
        detect_vector.push_back(detect_int);
        
      
        
        
        
    }
	
	
  }
  
  results_file.close();
	
}





int main (int argc, char *argv[]) {

  parseResults();
  
  std::cout << frames_vector;
  std::cout << detect_vector;
  
  return 0;
}
