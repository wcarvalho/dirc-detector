#include "../headers/reconstructor.h"
#include "analysis_objects.h"
/**
 * [removeFirstParticles removes the beginning particles of a particle set. used to analyze only the particles after a certain point]
 * @param Remove       [description]
 * @param event_output [description]
 * @param last         [description]
 */
void removeFirstParticles(bool Remove, GeneratorOut*& event_output, int last){
	if (Remove){
      int beginning = event_output->Particles.size() - last;
      if (beginning>0){
	      for (unsigned int j = 0; j < beginning; ++j)
	      	event_output->Particles.erase(event_output->Particles.begin());
      }
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

	std::string histTitle = wul::appendStrings(wul::stringDouble("Event ", ev), wul::stringDouble(", Particle ", par+1));
	std::string histname = histName0(ev, par);
  std::string TH1Name = histname; TH1Name.append("_1D");
  std::string TH2Name = histname; TH2Name.append("_2D");

  if (phos.size()){
	  A.AddTH1D(TH1Name.c_str(), histTitle.c_str(), xbins, 0, pi, 1);
	  A.AddTH2D(TH2Name.c_str(), histTitle.c_str(), xbins, -pi, pi, ybins, 0, pi);
	}
}

// for one particle this function will calculate the histogram fit, the area under the fit, the expected number of photons for each mass. the area and expected number of photons are compared and a delta sigma is delta_Sigma is calculated
void CalculateParticleFits(std::pair<double, double> (*ExpectedNumberofPhotons)(double const&, double const&, double const&, double const&, double const&), ParticleOut &P, Analysis &A, double range, double smear, bool print){

	mass m(P.Eta, P.pt);
	map<double, double> &atm = m.AngletoMass;
	map<double, string> &mtn = m.MasstoName;

	double travels = 0.;

	A.AddTrackRecon();

	TrackRecon &guess  = A.Recon.back();
	TH1D& h            = A.Hists1D.back();
	string defaultname = h.GetName();
	guess.Hist         = h;

	for(map<double, double>::iterator i = atm.begin(); i != atm.end(); ++i){
		static TCanvas c1("c1","c1",10,10,800,600);
		static TCanvas *c1_p = &c1;
		const double &angle = i->first;
		const double &mass = i->second;
		string name = mtn[mass];

		stringstream currentname;
		currentname << defaultname << "_" << name;
		string newhname = currentname.str();
		double Area = 0.;

		double weight = .1;
		double center = angle;
		double centerbounds[2] = {angle - weight*smear, angle + weight*smear};
		double width = smear;
		double widthbounds[2] = {.8*smear, 1.2*smear};

		A.FitGaussianPlusConstant(center-range, center+range, center, centerbounds, width, widthbounds, Area);
		// Area = guesser.FitParticle1D(c1_p, *h, params, angle-range, angle+range, angle, smear, newhname, print);	// area under gaussian (calculated number of photons)
		double Beta = P.CalculateBeta(mass);
		static std::pair<double, double> vals;
		double &N = vals.first;
		double &Sigma_N = vals.second;

		vals = ExpectedNumberofPhotons(P.X, P.Y, P.Theta, P.Phi, Beta);

		// FIXME: hack for time
		// double d =
		// double t = d/t

		if (print) cout << "X, Y, Theta, Phi, Beta = " << P.X << ", " << P.Y << ", " << P.Theta << ", " << P.Phi << ", " << Beta << endl;

		vector< double > &params = A.Recon.back().Params.back();				// vector to store parameters for efficiency analysis

		double pi2 = TMath::Pi()/2;
		double delSigTheta = (angle - params.at(1))/smear;
		double delSigA = (N-Area)/Sigma_N;
		if (print) cout << "delSigA = " << delSigA << ", delSigTheta = " << delSigTheta << endl;
		double delSigma = sqrt(delSigTheta*delSigTheta + delSigA*delSigA);

		guess.Options.push_back(name);
		guess.delSigTheta.push_back(delSigTheta);
		guess.delSigArea.push_back(delSigA);
		guess.Sigmas.push_back(delSigma);
		guess.Areas.push_back(Area);
		guess.ExpectedNumber.push_back(N);
		if (print) guess.printLatest();
	}

	}