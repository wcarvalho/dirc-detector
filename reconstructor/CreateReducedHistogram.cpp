#include "TH2D.h"
#include "TFile.h"
#include "dirc_objects.h"

#include <vector>
#include <string>

using namespace std;

TH1D* CreateReducedHistogram(vector<PhotonOut> const& photons, vector<int> const& index, int const particle_index, string histname, int nbins, double xlow, double xhi){

	TH1D* h1 = new TH1D(histname.c_str(), histname.c_str(), nbins, xlow, xhi);

	for (unsigned i = 0; i < photons.size(); ++i){
		if (index.at(i) != particle_index) continue;
		auto const& photon = photons.at(i);
		auto const& x = photon.Theta;
		h1->Fill(x);
	}

	return std::move(h1);
}

TH1D* ReducedHistogram(vector<PhotonOut> const& photons, TH2D const& h2, vector<int> const& index, int const& particle_index, string const& current_particle_type){

	string histogram_name = h2.GetName();
	histogram_name.append("_reduced1D_"); histogram_name.append(current_particle_type);

	double xlow = h2.GetYaxis()->GetXmin();
	double xhi = h2.GetYaxis()->GetXmax();
	int nbins = h2.GetNbinsY();
	TH1D* h1 = CreateReducedHistogram(photons, index, particle_index, histogram_name, nbins, xlow, xhi);

	return std::move(h1);
}