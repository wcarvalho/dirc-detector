// #include "FileProperties.h"
#include <tclap/CmdLine.h>
// #include <boost/lexical_cast.hpp>
#include "calibration_data.h"
#include "calibrate_data.h"
void checkValid(const TFile& f){
  if (!(f.IsOpen())) {
    cout << f.GetName() << " invalid!\n";
    exit(1);
  }
}
int main(int argc, char const *argv[])
{

  string particleFile;
  string reconstructionFile;
	string calibrationFile;
  string calibratedFile;

	double calibrationPercent = 0.;

	bool print = false;
	bool calibrateArea = false;
	bool calibrateTheta = false;
  vector<string> calibrationType;

TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{

  TCLAP::ValueArg<std::string> particleFileArg("p","particle-file","file with particle data", true, "particles.root","string", cmd);
  TCLAP::ValueArg<std::string> reconstructionFileArg("r","reconstruction-file","file with reconstruction data for calibration", true, "particles.root","string", cmd);

  TCLAP::ValueArg<std::string> calibrationFileArg("c","calibration-file","Root file with data for calibration",false,"reconstructions_calibrated.root","string", cmd);

	TCLAP::ValueArg<std::string> calibratedFileArg("o","output-file","Calibrated particle-file",false,"reconstructions_calibrated.root","string", cmd);

	TCLAP::ValueArg<double> calibrationPercentArg("","percent","the desired acceptance for \"1 sigma\" in generating efficiency", false, .68, "double", cmd);

  TCLAP::MultiArg<string> calibrationTypeArg("t","particle-type","particle-types which will be calibrated", false, "string", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);
	TCLAP::SwitchArg calibrateAreaArg("N","area","", cmd, false);
	TCLAP::SwitchArg calibrateThetaArg("T","theta","", cmd, false);

  cmd.parse( argc, argv );

  particleFile = particleFileArg.getValue();
  reconstructionFile = reconstructionFileArg.getValue();
  calibrationFile = calibrationFileArg.getValue();
  calibratedFile = calibratedFileArg.getValue();
  if (calibrationTypeArg.isSet())
    calibrationType = calibrationTypeArg.getValue();

	calibrationPercent = calibrationPercentArg.getValue();
	print = verboseArg.isSet();
	calibrateArea = calibrateAreaArg.isSet();
	calibrateTheta = calibrateThetaArg.isSet();

}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }

	ParticleEvent *originals = 0;
  TrackRecons *reconstructions = 0;
  Detector *d = 0;

  TFile f1(particleFile.c_str(), "read"); checkValid(f1);
  TTree* t1 = (TTree*)f1.Get("cheat_info");
  t1 -> SetBranchAddress("Particle Event", &originals);

  TFile f2(reconstructionFile.c_str(), "read"); checkValid(f2);
  TTree* t2 = (TTree*)f2.Get("identifications");
  t2 -> SetBranchAddress("guesses", &reconstructions);
  t2 -> SetBranchAddress("detector", &d);

  stringstream ss; ss << calibrationPercent;
  std::string calibrationKey = ss.str();

  TFile calibrationTFile(calibrationFile.c_str()); checkValid(calibrationTFile);
  calibration_data* cal_data = (calibration_data*)calibrationTFile.Get(calibrationKey.c_str());


  TFile *f2prime = new TFile(calibratedFile.c_str(),"recreate");
  TTree *t2prime = t2->CloneTree(0);

  if ( (calibrateArea) || (calibrateTheta))
    calibrateSigmas(*t1, *t2, *t2prime, *originals, *reconstructions, cal_data, calibrationType, calibrateArea, calibrateTheta, print);

  f2prime->cd();
  f2prime->Write();
  f2prime->Close();
  f1.cd();
  f2.Close();
  f2.cd();
  f2.Close();
  calibrationTFile.cd();
  calibrationTFile.Close();
  delete f2prime;

 	return 0;
}