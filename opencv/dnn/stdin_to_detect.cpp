
/**
 * Objectif: read() des frames en stdin, et sur une frame de temps en temps faire une detection
 * 
 * Attention pour les threads la compil nécessite un flag particulier: -pthread
 * Hello world threads trouvé avec recherche 'c++ threads'
 * https://cplusplus.com/reference/thread/thread/
 * 
 * g++ -pthread stdin_to_detect.cpp -o stdin_to_detect
 * 
 * 
 * 
 * aarch64-linux-gnu-g++ -pthread stdin_to_detect.cpp -o stdin_to_detect
 * */ 



#include <iostream>       
#include <thread>         

using namespace std;


 
void read_stdin() 
{
  // do stuff...
}

void detect(int x)
{
  // do stuff...
}

int main() 
{
  thread first (read_stdin);     // spawn new thread that calls read_stdin()
  thread second (detect,0);  // spawn new thread that calls detect(0)

  cout << "main, read_stdin and detect now execute concurrently...\n";

  // synchronize threads:
  first.join();                // pauses until first finishes
  second.join();               // pauses until second finishes

  cout << "read_stdin and detect completed.\n";

  return 0;
}
