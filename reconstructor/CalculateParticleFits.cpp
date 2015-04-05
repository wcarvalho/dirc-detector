#include "reconstructor.h"

// for one particle this function will calculate the histogram fit, the area under the fit, the expected number of photons for each mass. the area and expected number of photons are compared and a delta sigma is delta_Sigma is calculated
void CalculateParticleFits(TH1D &histogram, ParticleOut &P, const vector<PhotonOut>& phos, Analysis &A, const int particle_index, double smear, int const& loss, vec_pair& expected_photons, bool print){

	auto massmap = P.MassMap();
	auto anglemap = P.EmissionAngleMap();

	unsigned count = 0;
	for(auto i = anglemap.begin(); i != anglemap.end(); ++i){
		static TCanvas c1("c1","c1",10,10,800,600);
		static TCanvas *c1_p = &c1;
		const string name = i->first;
		const double &angle = i->second;
		const double &mass = massmap[name];
		double pi = TMath::Pi();

		static double Area;
		Area = 0.;
		double center = angle;
		double width = smear;
		static double constant;
		constant = 0.;
		static double height;
		height = 0.;
		static double xlow = 0.;
		xlow = angle - .05;
		static double xhi = 0.;
		xhi = angle + .05;

		FitGaussianPlusConstant(histogram, xlow, xhi, 0, pi, -1.10, 1.e10, center, width, constant, height, 8);

		Area = sqrt(2*pi)/histogram.GetBinWidth(1)*height*width;
		TrackRecon &guess  = A.Recon.at(particle_index);

		vector<double> params = {height, center, width, constant, xlow, xhi};
		A.Recon.at(particle_index).Params.push_back(std::move(params));
		auto& vals = expected_photons.at(count);

		double &N = vals.first;
		double &Sigma_N = vals.second;
		// if (print) cout << "X, Y, Theta, Phi, Beta = " << P.X << ", " << P.Y << ", " << P.Theta << ", " << P.Phi << ", " << Beta << endl;

		double pi2 = TMath::Pi()/2;
		double sigma_Theta = smear/(Sigma_N);
		double delSigTheta = (angle - center)/(sigma_Theta);
		double delSigA = (N-Area)/Sigma_N;
		double delSigma = sqrt(delSigTheta*delSigTheta + delSigA*delSigA);

		guess.Options.push_back(name);
		guess.delSigTheta.push_back(delSigTheta);
		guess.delSigArea.push_back(delSigA);
		guess.Sigmas.push_back(delSigma);
		guess.Areas.push_back(Area);
		guess.ExpectedNumber.push_back(N);
		// if (print) guess.printLatest();
		++count;
	}
}