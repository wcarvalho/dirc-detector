#include "../headers/reconstructor.h"

// removes the beginning particles of a particle set. used to analyze only the particles after a certain point
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
void CalculateParticleFits(double (*ExpectedNumberofPhotons)(double const&, double const&, double const&, double const&, double const&),
                           ParticleOut &P, TH1D*& h, TrackRecon &guess, double range, double smear, bool print){

	Identifier guesser;
	mass m(P.Eta, P.pt);
	map<double, double> &atm = m.AngletoMass;
	map<double, string> &mtn = m.MasstoName;
	map<string, double> &pm = guesser.probabilitymap;

	double travels = 0.;

	string defaultname = h->GetName(); 

	for(map<double, double>::iterator i = atm.begin(); i != atm.end(); ++i){
		static TCanvas c1("c1","c1",10,10,800,600);
		static TCanvas *c1_p = &c1;
		vector< double > params;				// vector to store parameters for efficiency analysis
		const double &angle = i->first;
		const double &mass = i->second;
		double r = .1;		// range
		string name = mtn[mass];

		stringstream currentname;
		currentname << defaultname << "_" << name;
		string newhname = currentname.str();

		// FIXME Hardcoded xlow=0, xhi=pi. maybe change later
		double Area = guesser.FitParticle1D(c1_p, *h, params, 0, TMath::Pi(), angle, smear, newhname, print);	// area under gaussian (calculated number of photons)
	  double Beta = P.CalculateBeta(mass);
	  double N = ExpectedNumberofPhotons(P.X, P.Y, P.Theta, P.Phi, Beta);

	  // sigma = sqrt (sigma_C*sigma_C + sigma_N*sigma_N)


		double pi2 = TMath::Pi()/2;
		double Sigma_N = sqrt(N);
		double Sigma_C = (angle - params.at(1))/smear;
		double Sigma = sqrt(Sigma_N*Sigma_N + Sigma_C*Sigma_C);
		// cout << "old sigma = " << Sigma_N << endl;
		// cout << "new sigma = " << Sigma << endl;
		double nSigma = (abs(N-Area))/Sigma;

		pm[name] = nSigma;

		guess.Options.push_back(name);
		guess.Sigmas.push_back(pm[name]);
		guess.Areas.push_back(Area);
		guess.ExpectedNumber.push_back(N);
		guess.Params.push_back(params);
		if (print) guess.printLatest();
	}

	}