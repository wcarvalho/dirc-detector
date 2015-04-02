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
	std::string histname = histName0(ev, par);
  std::string TH1Name = histname; TH1Name.append("_1D");
  std::string TH2Name = histname; TH2Name.append("_2D");

  if (phos.size()){
	  // A.AddTH1D(TH1Name.c_str(), histTitle.c_str(), xbins, 0, pi, 1);
	  A.AddTH2D(TH2Name.c_str(), histTitle.c_str(), xbins, -pi, pi, ybins, 0, pi);
	}
}

// for one particle this function will calculate the histogram fit, the area under the fit, the expected number of photons for each mass. the area and expected number of photons are compared and a delta sigma is delta_Sigma is calculated
void CalculateParticleFits(std::pair<double, double> (*ExpectedNumberofPhotons)(double const&, double const&, double const&, double const&, double const&), TH1D &histogram, ParticleOut &P, const vector<PhotonOut>& phos, Analysis &A, const int particle_index, double smear, int const&  loss, bool print){

	mass m(P.Eta, P.pt);
	map<double, double> &atm = m.AngletoMass;
	map<double, string> &mtn = m.MasstoName;

	double travels = 0.;

	// cout << "particle: " << particle_index << " has added loss: " << loss << endl;
	for(map<double, double>::iterator i = atm.begin(); i != atm.end(); ++i){
		static TCanvas c1("c1","c1",10,10,800,600);
		static TCanvas *c1_p = &c1;
		const double &angle = i->first;
		const double &mass = i->second;
		string name = mtn[mass];
		double pi = TMath::Pi();

		double Area = 0.;
		double center = angle;
		double width = smear;
		double constant = 0.;
		double height = 0.;
		double xlow = angle - .05;
		double xhi = angle + .05;
		// TFile f("histfits.root", "update");
		// histogram.Write();
		// cout << "center = " << center << endl;
		// cout << "width = " << width << endl;
		FitGaussianPlusConstant(histogram, xlow, xhi, 0, pi, -1.10, 1.e10, center, width, constant, height, 8);
		// f.Close();
		// height += loss;
		Area = sqrt(2*pi)/histogram.GetBinWidth(1)*height*width;
		TrackRecon &guess  = A.Recon.at(particle_index);

		vector<double> params = {height, center, width, constant, xlow, xhi};
		A.Recon.at(particle_index).Params.push_back(std::move(params));
		static std::pair<double, double> vals;
		double Beta = P.CalculateBeta(mass);
		vals = ExpectedNumberofPhotons(P.X, P.Y, P.Theta, P.Phi, Beta);
		double &N = vals.first;
		double &Sigma_N = vals.second;
		// if (print) cout << "X, Y, Theta, Phi, Beta = " << P.X << ", " << P.Y << ", " << P.Theta << ", " << P.Phi << ", " << Beta << endl;

		double pi2 = TMath::Pi()/2;
		double sigma_Theta = smear/(Sigma_N);
		double delSigTheta = (angle - center)/(sigma_Theta);
		double delSigA = (N-Area)/Sigma_N;
		// if (print) cout << "delSigA = " << delSigA << ", delSigTheta = " << delSigTheta << endl;
		double delSigma = sqrt(delSigTheta*delSigTheta + delSigA*delSigA);

		guess.Options.push_back(name);
		guess.delSigTheta.push_back(delSigTheta);
		guess.delSigArea.push_back(delSigA);
		guess.Sigmas.push_back(delSigma);
		guess.Areas.push_back(Area);
		guess.ExpectedNumber.push_back(N);
		// if (print) guess.printLatest();
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