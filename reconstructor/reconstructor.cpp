#include "reconstructor.h"
#include "analysis_objects.h"

/**
 * [removeFirstParticles removes the beginning particles of a particle set. used to analyze only the particles after a certain point]
 * @param Remove       [description]
 * @param event_output [description]
 * @param last         [description]
 */
void removeFirstParticles(GeneratorOut*& event_output, int last, bool print){
	int sizebefore = event_output->Particles.size();
  int beginning = event_output->Particles.size() - last;
  if (beginning>0){
    for (unsigned int j = 0; j < beginning; ++j)
    	event_output->Particles.erase(event_output->Particles.begin());
  }
  if (print){
  	int sizeafter = event_output->Particles.size();
	  int sizedifference = sizebefore - sizeafter;
	  cout << sizedifference << " Particles Removed\n";
	}
}

// Create 1D and 2D histograms for one particle in the event. Histogram is added to the analysis class
void CreateHistogram_1D2D(int ev, int par, Analysis &A, std::vector<PhotonOut> &phos, int xbins, int ybins){

	double pi = TMath::Pi();
	std::vector< std::vector<double> > data; data.clear();
	std::vector<double> data_sub; data_sub.clear();

	for (unsigned int i = 0; i < phos.size(); i++)
	{
		data.push_back(data_sub);
			data.back().push_back(phos.at(i).Phi);
			data.back().push_back(phos.at(i).Theta);
	}
	A.SetData(data);

	std::string histTitle = wul::appendStrings(wul::stringDouble("Event ", ev), wul::stringDouble(", Particle ", par));
	std::string histname = histTitle;
  std::string TH1Name = histname; TH1Name.append("_1D");
  std::string TH2Name = histname; TH2Name.append("_2D");

  if (phos.size()){
	  // A.AddTH1D(TH1Name.c_str(), histTitle.c_str(), xbins, 0, pi, 1);
	  A.AddTH2D(TH2Name.c_str(), histTitle.c_str(), xbins, -pi, pi, ybins, 0, pi);
	}
}


bool passed_index_photons_condition(ParticleOut & P, double momentum_indexing_threshold){
	static std::map<std::string,double> masses;
	masses = P.MassMap();

	static double momentum {0.};
	momentum = P.CalculateMomentum(masses["electron"]);


	Simulate simPar(P.Theta, P.Phi);
	Detector d;
	simPar.SetStart(P.X, P.Y, 0);
	simPar.SetDim(d.Length, d.Width, d.Height);
	simPar.DistancetoWalls();
	simPar.WhichWall();
	int wall = simPar.wall;
	if (wall == 2){
		return false;
	}
	else{
		return true;
	}

}