#include "TMultiGraph.h"
#include "TGraph.h"
#include "TLegend.h"
#include <vector>
#include "dirc_objects.h"
#include <iomanip>      // std::setprecision
#include "TFile.h"
using namespace std;

void createGraph(vector<TGraph*>& graphs, TLegend& L, string legend_entry, vector<PhotonOut> const& photons, vector<int> const& index, int particle_index, int markerstyle, int markercolor, double markersize){
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
		auto const& photon = photons.at(j);
		double const& x = photon.Phi;
		double const& y = photon.Theta;
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

void DrawScatterPlot(TMultiGraph*& mg, TLegend& L, vector<ParticleOut> const& particles, vector<PhotonOut> const& photons, vector<int> const& index, int particle_index){

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
		createGraph(graphs, L, ss.str(), photons, index, i, markerstyle, i+2, markersize);
	}

	createGraph(graphs, L, "shared index", photons, index, -1, markerstyle, 1, markersize);

	createGraph(graphs, L, "not indexed", photons, index, -10, markerstyle, nparticles+2, markersize);

	for (auto& graph: graphs){
		mg->Add(graph);
	}
	ss.str(""); ss << "#theta vs. #phi In Frame of Particle " << particle_index << "; #phi (radians); #theta (radians)";
	mg->SetTitle(ss.str().c_str());


}

void createIndexedPhotonScatterPlot(vector<ParticleOut> const& particles, vector<PhotonOut> const& photons, vector<int> const& index, int particle_index, string histogramname, string filename, string status){


	TCanvas c1("c2","c2",10,10,800,600);
	TMultiGraph *mg = new TMultiGraph;
	TLegend L(0.1,0.6,0.25,0.9, "particle #theta, #phi");
	DrawScatterPlot(mg, L, particles, photons, index, particle_index);
	mg->Draw("AP");
	L.Draw();
	TFile f(filename.c_str(), status.c_str());
	c1.Write(histogramname.c_str());
	f.Close();
	c1.Clear();
	delete mg;
	// for (auto*& graph: graphs){
	// 	delete graph;
	// }
}