#include "dirc_objects.h"
#include "../scripts.h"
#include "delSig_pt.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TGraph.h"

#include <tclap/CmdLine.h>


int main(int argc, char** argv){

	std::string graphdir;
	std::string simdata;
	std::string recdata;
	std::string delSigThetaFile = "delSigTheta_v_Pt";
	std::string delSigAreaFile = "delSigArea_v_Pt";

	std::string momentum_slices_file;
	std::vector< std::vector< double > > momentum_slices;

	vector<int> nbins_theta; nbins_theta.push_back(100); nbins_theta.push_back(100);
	vector<int> nbins_area; nbins_area.push_back(100); nbins_area.push_back(100);

	bool print;

TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{   

	TCLAP::ValueArg<std::string> DirectoryArg("D","directory","Directory where graphs will be stored",false,"comparison","string", cmd);

	TCLAP::ValueArg<std::string> simdataArg("S","simulation-data","file with particle and photon data",false,"photons.root","string", cmd);

	TCLAP::ValueArg<std::string> recdataArg("R","reconstruction-data","file with particle reconstruction data",false,"reconstructions.root","string", cmd);

	TCLAP::ValueArg<std::string> momSliceArg("p","momentum-slices","Text file where momentum slices are stored",false,"momentum-slices.txt","string", cmd);

	TCLAP::MultiArg<int> thetaBinArg("t","theta-bin-edges","The number of x and y bins (in that order) bins for the theta delta sigma graphs",false,"int", cmd);
	
	TCLAP::MultiArg<int> areaBinArg("a","area-bin-edges","The number of x and y bins (in that order) bins for the area delta sigma graphs",false,"int", cmd);
	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);
	
	cmd.parse( argc, argv );
	graphdir = DirectoryArg.getValue();
	simdata = simdataArg.getValue();
	recdata = recdataArg.getValue();
	momentum_slices_file=momSliceArg.getValue();
	print = verboseArg.getValue();
	if (thetaBinArg.isSet())
		nbins_theta = thetaBinArg.getValue();
	if (areaBinArg.isSet())
		nbins_area = areaBinArg.getValue();
}
catch (TCLAP::ArgException& e){
	cout << "ERROR: " << e.error() << " " << e.argId() << endl;
}

	file_to_dir(graphdir, delSigAreaFile);
	file_to_dir(graphdir, delSigThetaFile);
	momentum_slices = filearray(momentum_slices_file);
	TCanvas C("C", "C", 1000, 600);

	ParticleEvent *particles = NULL;
	TrackRecons *reconstructions = NULL;

	TFile f1(simdata.c_str(), "read");
	TTree *t1 = (TTree*)f1.Get("cheat_info");
	t1 -> SetBranchAddress("Particle Event", &particles);

	TFile f2(recdata.c_str(), "read");
	TTree *t2 = (TTree*)f2.Get("identifications");
	t2 -> SetBranchAddress("guesses", &reconstructions);

	std::vector<Particle> *pars = &particles->Particles;
	std::vector<TrackRecon> *recons = &reconstructions->Recon;

	int nentries = t1->GetEntries();

	std::vector<double> pts;
	std::vector<double> delSigTheta;
	std::vector<double> delSigArea;
	std::vector<double> momentums;
	for (unsigned int ev = 0; ev < nentries; ++ev){
		if (print) cout << "event = " << ev << endl;
		t1->GetEntry(ev);
		t2->GetEntry(ev);

		int npar = pars->size();
		if (npar != recons->size()) continue;

		for (unsigned int i = 0; i < npar; ++i){
			Particle &par = pars->at(i);
			pts.push_back(par.pt);
			if (print) cout << "par.pt = " << par.pt << endl;
			double ThetaBeam = 2*atan(exp(-par.Eta));
			double momentum = par.pt/sin(ThetaBeam);
			momentums.push_back(momentum);
			TrackRecon &recon = recons->at(i);
			std::string &name = par.name;
			if (print) cout << "name = " << name << endl;
			for (unsigned int j = 0; j < recon.Options.size(); ++j){
				if (print) cout << "j = " << j << endl;
				if (print) cout << "recon.Options.at(j) = " << recon.Options.at(j) << endl;
				if (print) cout << "recon.delSigTheta.at(j) = " << recon.delSigTheta.at(j) << endl;
				if (print) cout << "recon.delSigArea.at(j) = " << recon.delSigArea.at(j) << endl;
				if (recon.Options.at(j) == name){
					delSigTheta.push_back(recon.delSigTheta.at(j));
					delSigArea.push_back(recon.delSigArea.at(j));
				}
			}

		}
	}

	std::string name = pars->at(0).name;
	int npoints  = pts.size();
	for ( unsigned int i=0; i < momentum_slices.size(); ++i){
		double lb = momentum_slices.at(i).at(0);		// lower bound
		double hb = momentum_slices.at(i).at(1);		// higher bound
		make_hist(C, lb, hb, nbins_theta.at(0), nbins_theta.at(1), momentums, delSigTheta, momentums, delSigThetaFile, i, name);
		make_hist(C, lb, hb, nbins_area.at(0), nbins_area.at(1), momentums, delSigArea, momentums, delSigAreaFile, i, name);
	}

	return 0;	

}