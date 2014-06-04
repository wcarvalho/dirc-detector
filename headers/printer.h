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


class Printer
{
public:
  string Name;
  int which;
  string Hist_Title;
};

void Print_EventTH2D(Reconstruction* reconstruction, Printer printer, Displayer display);
void Print_EventTH1D(Reconstruction* reconstruction, Printer printer, Displayer display);

