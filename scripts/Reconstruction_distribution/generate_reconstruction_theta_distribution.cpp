#include <string>

#include "TFile.h"

#include "dirc_io.h"
#include <tclap/CmdLine.h>

using namespace std;

int main(int argc, char** argv){

string reconstructiondata;
string search_type;
string output;
vector<double> bounds {.5, .95};
bool print;


TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{

	TCLAP::ValueArg<std::string> reconstructionFileArg("r","reconstruction","file with reconstruction data",true,"reconstruction.root","string", cmd);

	TCLAP::ValueArg<std::string> search_typeArg("t","type","type for which distribution will be generated",true,"theta","string", cmd);

	TCLAP::ValueArg<std::string> outputArg("o","output","output-file",false,"theta_distribution.root","string", cmd);

	TCLAP::MultiArg<double> boundsArg("b","bounds","plot bounds",false,"string", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);

	cmd.parse( argc, argv );

	reconstructiondata = reconstructionFileArg.getValue();
	search_type = search_typeArg.getValue();
	output = outputArg.getValue();
	if (boundsArg.isSet()) bounds = boundsArg.getValue();
	print = verboseArg.getValue();

}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }

	TFile* f = 0;
	TTree* t = 0;
	TrackRecons* Tracks = 0;

	readInReconstructionData(f, reconstructiondata, t, Tracks);

	double pi=TMath::Pi();

	TH1D h("h", "h", 1000, bounds.at(0), bounds.at(1));

	if (print) cout << "searching for " << search_type << endl;

	for (unsigned ev = 0; ev < t->GetEntries(); ++ev){
		t->GetEntry(ev);
		for (auto& track: Tracks->Recon){
			static int indx;
			static double theta;
			indx = track.getIndexOf(search_type);
			if (indx < 0) continue;
			theta = track.getIntegralCenterAt(indx);
			h.Fill(theta);
		}
	}

	TFile f1(output.c_str(), "recreate");
	f1.cd();
	h.Write();
	f1.Close();
	f->cd();
	f->Close();


}