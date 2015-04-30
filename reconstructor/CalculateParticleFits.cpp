#include "reconstructor.h"

double CalculateArea(TH1D & h, double const& xlow, double const& xhi, double const& constant){

	unsigned binlow = h.FindBin(xlow);
	unsigned binhi = h.FindBin(xhi);

	static double height; height = 0;
	static double width; width = 0;

	double sum = 0;
	for (unsigned i = binlow; i < binhi; ++i){
		height = h.GetBinContent(i);
		width = h.GetBinWidth(i);
		sum += height;
	}

	double area_below_gaussian = constant*(xhi-xlow);

	return sum - area_below_gaussian;
}



// for one particle this function will calculate the histogram fit, the area under the fit, the expected number of photons for each mass. the area and expected number of photons are compared and a delta sigma is delta_Sigma is calculated
void CalculateParticleFits(TH1D &histogram, ParticleOut &P, TrackRecon &T, vec_pair& expected_photons, const int particle_index, double smear, bool print){

	auto massmap = P.MassMap();
	auto anglemap = P.EmissionAngleMap();

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
		static double xhi = 0.;
		xlow = angle - .05;
		xhi = angle + .05;

		// histogram.SaveAs("pre_rebinning.root");
		static TH1D rebinned_histogram;
		rebinned_histogram = rebinHistogram(histogram);

		FitGaussianPlusConstant(rebinned_histogram, xlow, xhi, 0, pi, -1.10, 1.e10, center, width, constant, height, 8);

		xlow = center - .03;
		xhi = center + .03;
		FitGaussianPlusConstant(rebinned_histogram, xlow, xhi, 0, pi, -1.10, 1.e10, center, width, constant, height, 8);
		xlow = center - .03;
		xhi = center + .03;
		// Area = sqrt(2*pi)/histogram.GetBinWidth(1)*height*width;
		Area = CalculateArea(histogram, xlow, xhi, constant);
		TrackRecon &guess  = T;

		vector<double> params = {height, center, width, constant, xlow, xhi};
		T.Params.push_back(std::move(params));
		auto& vals = expected_photons[name];

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
		guess.Final1D = rebinned_histogram;
		// if (print) guess.printLatest();
	}
}