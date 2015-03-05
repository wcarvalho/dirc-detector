#include "calibrate_data.h"
#include "FileProperties.h"
#include <tclap/CmdLine.h>

int main(int argc, char const *argv[])
{

  string particleFile;
  string reconstructionFile;
	string calibrationFile;
  string graphFile;
  string outputDirectory;

  int nbins;
  double pStep;
	double calibrationPercent = 0.;

	bool print = false;
	bool calibrateArea = false;
	bool calibrateTheta = false;

TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{

  TCLAP::ValueArg<std::string> particleFileArg("p","particle-file","file with particle data", true, "particles.root","string", cmd);
  TCLAP::ValueArg<std::string> reconstructionFileArg("r","reconstruction-file","file with reconstruction data for calibration", true, "particles.root","string", cmd);
	TCLAP::ValueArg<std::string> calibratedFileArg("o","output-file","Calibration of particle-file",false,"reconstructions_calibrated.root","string", cmd);

  TCLAP::ValueArg<std::string> graphFileArg("g","graph-file","file with calibration graphs",false,"calibration_graphs.root","string", cmd);

  TCLAP::ValueArg<std::string> DirectoryArg("D","Directory","Directory where graphs for sigma calibration will be stored",false,"","string", cmd);

  TCLAP::ValueArg<double> pBinArg("","p-bin","pt bin size for calibration",false, .25, "int", cmd);

  TCLAP::ValueArg<int> nBinArg("n","nbins","number of bins in calibration graph",false, 200, "int", cmd);

	TCLAP::ValueArg<double> calibrationPercentArg("","percent","the desired acceptance for \"1 sigma\" in generating efficiency", false, .68, "double", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);
	TCLAP::SwitchArg calibrateAreaArg("N","area","", cmd, false);
	TCLAP::SwitchArg calibrateThetaArg("T","theta","", cmd, false);

  cmd.parse( argc, argv );

  particleFile = particleFileArg.getValue();
  reconstructionFile = reconstructionFileArg.getValue();
	calibrationFile = calibratedFileArg.getValue();
  graphFile = graphFileArg.getValue();

  outputDirectory = DirectoryArg.getValue();
  pStep = pBinArg.getValue();
  nbins = nBinArg.getValue();

	calibrationPercent = calibrationPercentArg.getValue();
	print = verboseArg.isSet();
	calibrateArea = calibrateAreaArg.isSet();
	calibrateTheta = calibrateThetaArg.isSet();

}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }

	ParticleEvent *originals = 0;
  TrackRecons *reconstructions = 0;

  TFile f1(particleFile.c_str(), "read");
  TTree* t1 = (TTree*)f1.Get("cheat_info");
  t1 -> SetBranchAddress("Particle Event", &originals);

  TFile f2(reconstructionFile.c_str(), "read");
  TTree* t2 = (TTree*)f2.Get("identifications");
  t2 -> SetBranchAddress("guesses", &reconstructions);

  wul::FileProperties fp(reconstructionFile);
  fp.get_directory();
  string reconstruction_directory = fp.directory;
  cout << "calibrationFile = " << calibrationFile << endl;
  TFile *f2prime = new TFile(calibrationFile.c_str(),"recreate");
  TTree *t2prime = t2->CloneTree(0);

  graphFile = wul::appendStrings(outputDirectory, "/", graphFile);
  calibrateSigmas(*t1, *t2, *t2prime, *originals, *reconstructions, calibrationPercent, nbins, graphFile, pStep, calibrateArea, calibrateTheta, print);

  f1.cd();
  f2.Close();
  f2.cd();
  f2.Close();
  f2prime->cd();
  f2prime->Write();
  f2prime->Close();

 	return 0;
}