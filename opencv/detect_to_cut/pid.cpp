#include <dirent.h>
#include <iostream>
#include <fstream>
#include <string>


//Cherche si le fichier /proc/<PID>/cmdline contient "stdin_to_detect"
int parseCmdline(std::string filestr)
{
  std::ifstream file(filestr);
  
  std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  str.c_str();
  
  std::size_t found = str.find("stdin_to_detect");

  if (found!=std::string::npos)
	{
		//std::cout << "found at: " << found << '\n';
		return 0;
	}	
	
  return 1;
}


int iterateProcEntries() {
  
  const char *procDirName = "/proc/";
  struct dirent *entry;
  DIR *dp;

  dp = ::opendir(procDirName);
  if (dp == NULL) {
    perror("opendir: Path does not exist or could not be read.");
    return -1;
  }

  while ((entry = ::readdir(dp))) {
	
	//d_type == 0x4 : directory
    if (entry->d_type == 0x4) {

		std::string cmdline_file = "/proc/"+std::string(entry->d_name)+"/cmdline";
		
		if (parseCmdline(cmdline_file) == 0) {
			std::cout << "found stdin_to_detect for pid:" << entry->d_name << std::endl;
			return atoi(entry->d_name);
		}
	}
    
  }

  ::closedir(dp);
  return 0;
}

int main (int argc, char *argv[]) {

  iterateProcEntries();
  return 0;
}
