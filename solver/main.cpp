#include <iostream>
#include <math.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>

using namespace std;

#ifdef _WIN32
#include <windows.h>
#include <sys/stat.h>
#include <dirent.h>

#define DIV 1048576 
#define WIDTH 7
#endif

#ifdef linux
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

#include "../libs/IEC/IEC.H"

///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int main() {

  const char *path="../inputData/coolerData.txt";

  IEC *myObject;

  myObject = myObject -> IEC::New(path);

  myObject -> setBoundaryConditions();

  myObject -> solve();

  myObject -> writeAllInformationOfCooler();

  int numberOfNodes;

  numberOfNodes = myObject -> get_numberOfNodes();

  double **T_primaryAir, **T_secondaryAir, **T_water, **humidity_secondaryAir;

  T_primaryAir = myObject -> get_T_primaryAir();
  T_secondaryAir = myObject -> get_T_secondaryAir();
  T_water = myObject -> get_T_water();
  humidity_secondaryAir = myObject -> get_humidity_secondaryAir();

  double average_T_outletProductChannel, sum = 0.0;

  for (int i=0;i<numberOfNodes;i++)
		sum = sum +  T_primaryAir[numberOfNodes-1][i];

  average_T_outletProductChannel = sum/numberOfNodes;

  cout << "average air temperature at product channel outlet is: " << average_T_outletProductChannel
       << endl << endl;

  cout << "end of the program." << endl;

}
