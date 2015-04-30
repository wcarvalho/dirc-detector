#include "TMultiGraph.h"
#include "TGraph.h"
#include "TLegend.h"
#include <vector>
#include "dirc_objects.h"
#include <iomanip>      // std::setprecision
#include "TFile.h"
using namespace std;

void createTimeGraph(vector<TGraph*>& graphs, TLegend& L, string legend_entry, vector<PhotonOut> & photons, vector<int> const& index, int particle_index, int markerstyle, int markercolor, double markersize){
	TGraph* g = new TGraph;
	g->SetMarkerStyle(markerstyle);
	g->SetMarkerSize(markersize);
	if (markercolor == 8) ++ markercolor;
	g->SetMarkerColor(markercolor);
	g->SetLineColor(markercolor);
	int point = 0;
	int nphotons = photons.size();
	// cout << "nphotons = " << nphotons << endl;
	// cout << "index size = " << index.size() << endl;
	for (unsigned j = 0; j < nphotons; ++j){
		// cout << "index at " << j << " = " << index.at(j) << endl;
		if (index.at(j) != particle_index ) continue;
		auto & photon = photons.at(j);
		double const& x = photon.Phi;
		double& y = photon.Time_Traveled;
		y *= 300*TMath::Cos(photon.Phi)*TMath::Sin(photon.Theta);
		g->SetPoint(point, x, y);
		++point;
	}
	static stringstream ss; ss.str("");
	if (point != 0){
		graphs.push_back(std::move(g));
	 	L.AddEntry(graphs.back(),legend_entry.c_str(),"lp");
	}
	// cout << "particle "<< particle_index <<": points = " << point << endl;
}

void DrawTransformedTimePlot(TMultiGraph*& mg, TLegend& L, vector<ParticleOut> const& particles, vector<PhotonOut> & photons, vector<int> const& index, int particle_index){

	int nparticles = particles.size();
	int	nphotons = photons.size();
	double markersize = .5;
	int markerstyle = 21;

	vector<TGraph*> graphs;
	graphs.clear();

	L.SetTextSize(.025);

	stringstream ss;
	for (unsigned i = 0; i < nparticles; ++i){
		auto const& particle = particles.at(i);
		 ss.str(""); ss << i << ": (" << std::setprecision(2) << particle.Theta << ", " << std::setprecision(2) << particle.Phi << "), " << int(particle.X);
		createTimeGraph(graphs, L, ss.str(), photons, index, i, markerstyle, i+2, markersize);
	}

	createGraph(graphs, L, "shared index", photons, index, -1, markerstyle, 1, markersize);

	// createGraph(graphs, L, "not indexed", photons, index, -10, markerstyle, nparticles+2, markersize);

	for (auto& graph: graphs){
		mg->Add(graph);
	}
	ss.str(""); ss << "#theta vs. #phi In Frame of Particle " << particle_index << "; #phi (radians); #theta (radians)";
	mg->SetTitle(ss.str().c_str());


}
