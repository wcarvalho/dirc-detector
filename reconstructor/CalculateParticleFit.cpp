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
void CalculateParticleFit(TH1D &histogram, ParticleOut const &P, TrackRecon &T, vec_pair & expected_photons, double const& emission_angle, string const& current_particle_type, double const& smear, bool print){
	double pi = TMath::Pi();

	string const& name = current_particle_type;
	double const& angle = emission_angle;

	static double Area;
	Area = 0.;

	double center = angle;
	double width = smear;
	static double constant;
	static double height;
	static double xlow = 0.;
	static double xhi = 0.;
	constant = 0.;
	height = 0.;
	xlow = angle - .05;
	xhi = angle + .05;

	static TH1D rebinned_histogram;
	rebinned_histogram = rebinHistogram(histogram, 5);

	FitGaussianPlusConstant(rebinned_histogram, xlow, xhi, 0, pi, -1.10, 1.e10, center, width, constant, height, 8);

	xlow = center - .03;
	xhi = center + .03;
	FitGaussianPlusConstant(rebinned_histogram, xlow, xhi, 0, pi, -1.10, 1.e10, center, width, constant, height, 8);
	xlow = center - .03;
	xhi = center + .03;

	Area = CalculateArea(histogram, xlow, xhi, constant);
	TrackRecon &guess = T;

	vector<double> params = {height, center, width, constant, xlow, xhi};
	guess.Params.push_back(std::move(params));

	auto& vals = expected_photons[current_particle_type];
	double &N = vals.first;
	double &Sigma_N = vals.second;
	// if (print) cout << "X, Y, Theta, Phi, Beta = " << P.X << ", " << P.Y << ", " << P.Theta << ", " << P.Phi << ", " << Beta << endl;

	double pi2 = TMath::Pi()/2;
	double sigma_Theta = smear/(Sigma_N);
	double delSigTheta = (angle - center)/(sigma_Theta);
	double delSigA = (N-Area)/Sigma_N;

	double delSigma = sqrt(delSigTheta*delSigTheta + delSigA*delSigA);

	// if (print){
	// 	cout << "\t\t\t" << current_particle_type << endl;
	// 	cout << "\t\t\tdelSigA = " << delSigA << endl;
	// 	cout << "\t\t\tdelSigTheta = " << delSigTheta << endl;
	// }



	guess.Options.push_back(name);
	guess.delSigTheta.push_back(delSigTheta);
	guess.delSigArea.push_back(delSigA);
	guess.Sigmas.push_back(delSigma);
	guess.Areas.push_back(Area);
	guess.ExpectedNumber.push_back(N);
	guess.Final1D = rebinned_histogram;

}