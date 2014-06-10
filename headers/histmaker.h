//------------------------------------------
//		C++
//------------------------------------------
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>      // std::stringstream

//------------------------------------------
//		ROOT
//------------------------------------------
#include "TMath.h"
#include "TObject.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH2D.h"

//------------------------------------------
//		mine
//------------------------------------------
#include "dirc_objects.h"
#include "functions.h"
//------------------------------------------
//		Functions
//------------------------------------------
using namespace std;


class Printer : public Analysis
{
public:
	Printer(){}
	~Printer(){}
  
  void PrintTH1D(unsigned int event);
  void PrintTH2D(unsigned int event);

  string filename;
};