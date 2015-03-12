#include "TCanvas.h"
#include <iomanip>
#include "utility_library.h"
#include "TLatex.h"

void print2DHists(TCanvas &C, int Event, int par, Particle &P, TrackRecon &R, std::string const dir, flag_fun_map& fmap, const std::vector< int >& flags){

	// if (!passConditions(flags, fmap, P, R, 0, 1)) return;
	std::stringstream ss; ss.str("");
	ss << dir << "Event_" << std::setfill('0') <<std::setw(3) << Event << "_";
	ss << "Particle_" << std::setfill('0') <<std::setw(3) << par << "_";
	ss << P.name;
	std::string base_name = ss.str();

	std::stringstream ss2; ss2.str("");
	ss2 << dir << P.name << "_2Dfits.root";
	std::string filename = ss2.str();

	TH2D &h2 = R.Hist2D;
	cout << filename << endl;
	TFile f(filename.c_str(), "update");
	h2.Draw("colz");
	h2.Write(h2.GetName());
	// C.Write(filename.c_str());
	f.Close();
}