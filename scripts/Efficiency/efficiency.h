#include "dirc_objects.h"
#include "utility_library.h"

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>      // std::setfill, std::setw
#include <sys/time.h>
#include <sstream>

#include "TLatex.h"
#include "TMath.h"
#include "TH1D.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"

using namespace std;

// shrinks the original Particle data so it only has the data corresponding to the Reconstructed Data (assuming that info comes from the end of the particle data array)
int matchDataSize(vector<TrackRecon>& a, vector<Particle>& b){

	//sizes of a and b
	int sa = a.size();
	int sb = b.size();
	int dif = fabs(sb-sa);

	if (sb>sa){
		for (unsigned int i = 0; i < dif; ++i)
			b.erase(b.begin());
	}
	else{
		for (unsigned int i = 0; i < dif; ++i)
			a.erase(a.begin());
	}
	return dif;
}





// finds every case that the name of the particle matches search and the name of the reconstruction matches tomatch. The former fills search_den, and the latter search_num. The multiplicity of the event is recorded for every pair added to either vector.
template <typename matchCondition>
void getMatch(Particle* const& P, string search, string tomatch, double threshold, TrackRecon* const& R, int multiplicity, vector< pair<int, pair <Particle, TrackRecon> > > &search_num, vector< pair<int, pair <Particle, TrackRecon> > > &search_den, matchCondition passCondition, bool print){

	vector<double> &sigmas = R->Sigmas;
	vector<double> &thetasigmas = R->delSigTheta;
	vector<double> &areasigmas = R->delSigArea;
	vector<string> &names = R->Options;
	// cout << "mass = " << P->m << endl;
	if(P->name == search){
		pair <Particle, TrackRecon> PR; PR.first = *P; PR.second = *R;
		pair <int, pair <Particle, TrackRecon> > den(multiplicity, PR);
		search_den.push_back(den);

		if ( sigmas.size() != names.size() ) exit(1);
		for (unsigned int opt = 0; opt < sigmas.size(); ++opt){
			if (names[opt] != tomatch) continue;
			if (passCondition(*P, *R, opt, threshold))
				search_num.push_back(den);
		}
	}

}
