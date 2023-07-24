#include <unistd.h>
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string>

int INTERVAL_S = 2;


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

int main (int argc, char *argv[]) {
	
	int pid_stdin_to_detect;
	
	while (1) {
	std::cout << "Debut loop" << std::endl;
	pid_stdin_to_detect = searchPid("stdin_to_detect");
	
	//std::cout << "pid_stdin_to_detect=" << pid_stdin_to_detect << std::endl;
	
	if (pid_stdin_to_detect == -1) goto end;
	
	
	
	std::cout << "Le travail se fera ici" << std::endl;
	
	
	end:
	sleep(INTERVAL_S); //#include <unistd.h>
	}
	
	
return 0;
}
