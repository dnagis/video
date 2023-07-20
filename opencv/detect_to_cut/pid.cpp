#include <dirent.h>

#include <iostream>
#include <fstream>
#include <string>

int SearchProc() {
  
  const char *procName = "/proc/";
  struct dirent *entry;
  DIR *dp;


  dp = ::opendir(procName);
  if (dp == NULL) {
    perror("opendir: Path does not exist or could not be read.");
    return -1;
  }

  while ((entry = ::readdir(dp))) {
	
	//d_type == 0x4 : directory
    if (entry->d_type == 0x4) {
		
		//std::cout << entry->d_name << std::endl;
		
		std::fstream my_file;
		
		std::cout << "/proc/"+std::string(entry->d_name)+"/cmdline" << std::endl;
		
		my_file.open("/proc/"+std::string(entry->d_name)+"/cmdline", std::ios::in);
		
		
		if (!my_file) {
			std::cout << "File not accessed!" << std::endl;
		}	
		else {
			std::cout << "File accessed!" << std::endl;
			my_file.close(); 
		}
		
		
		
	}
    
  }

  ::closedir(dp);
  return 0;
}

int main (int argc, char *argv[]) {

  SearchProc();
  return 0;
}
