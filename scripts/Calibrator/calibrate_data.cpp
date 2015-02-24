#include "calibrate_data.h"
#include <tclap/CmdLine.h>

int main(int argc, char const *argv[])
{

	string calibrationFile = "";
	double calibrationPercent = 0.;
	bool print = false;
	bool calibrateArea = false;
	bool calibrateTheta = false;

TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{

	TCLAP::ValueArg<std::string> calibrationFileArg("o","output-file","Calibration file",false,"calibration.root","string", cmd);
	TCLAP::ValueArg<double> calibrationPercentArg("p","percent","the desired acceptance for \"1 sigma\" in generating efficiency", false, .68, "double", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);
	TCLAP::SwitchArg calibrateAreaArg("","area","", cmd, true);
	TCLAP::SwitchArg calibrateThetaArg("","theta","", cmd, false);


	calibrationFile = calibrationFileArg.getValue();
	calibrationPercent = calibrationPercentArg.getValue();
	print = verboseArg.isSet();
	calibrateArea = verboseArg.isSet();
	calibrateTheta = verboseArg.isSet();

}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }


	ParticleEvent *originals = 0;
  TrackRecons *reconstructions = 0;

  TFile f1("../../dirc_testing/data/default/1000_events_1_particles_electron/cheat.root", "read");
  TTree* t1 = (TTree*)f1.Get("cheat_info");
  t1 -> SetBranchAddress("Particle Event", &originals);

  TFile f2("../../dirc_testing/data/default/1000_events_1_particles_electron/smear0.01/particle_reconstructions_case2.root", "read");
  TTree* t2 = (TTree*)f2.Get("identifications");
  t2 -> SetBranchAddress("guesses", &reconstructions);

  TFile *f1prime = new TFile("temp1.root","recreate");
  TTree *t1prime = t1->CloneTree(0);
  TFile *f2prime = new TFile("temp2.root","recreate");
  TTree *t2prime = t2->CloneTree(0);

  TFile calibrations(calibrationFile.c_str(), "recreate");

  TCanvas C;
  std::string type = "electron";
  std::string directory = "";
  double percent = .7;
  int nbins = 50;

  calibrateSigmas(C, *t1, *t2, *t1prime, *t2prime, *originals, *reconstructions, percent, calibrateArea, calibrateTheta, nbins, calibrations, print);

  f1.cd();
  f2.Close();
  f2.cd();
  f2.Close();
  f1prime->cd();
  f1prime->Close();
  f2prime->cd();
  f2prime->Close();
  calibrations.cd();
  calibrations.Close();

 	return 0;
}