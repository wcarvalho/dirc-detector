#include "TH2D.h"

#include "dirc_objects.h"

#include <vector>
#include <string>

using namespace std;


TH2D CreateReducedHistogram(vector<PhotonOut> const& photons, Analysis const& A, int const& particle_index, int &loss){

	TH2D reduced_histogram = A.Hists2D.at(particle_index);
	// reduced_histogram.SaveAs("original_histogram.root");
	string histogram_name = reduced_histogram.GetName();
	histogram_name.append("_reduced");

	reduced_histogram.Reset();
	reduced_histogram.SetName(histogram_name.c_str());

	for (unsigned i = 0; i < photons.size(); ++i){
		if (A.index.at(i) == -1) ++loss;
		if (A.index.at(i) != particle_index) continue;
		auto const& photon = photons.at(i);
		auto const& x = photon.Phi;
		auto const& y = photon.Theta;
		reduced_histogram.Fill(x, y);
	}
	cout << "loss = " << endl;
	// reduced_histogram.SaveAs("reduced_histogram.root");
	return std::move(reduced_histogram);
}