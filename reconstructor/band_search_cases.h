// #include <cv.hpp>
// #include <highgui.h>
#include "Rounder.h"
#include <math.h>
#include "reconstructor.h"
#include <unordered_map>
#include <TSpectrum.h>
#include "Rotater.h"

double pi=3.14159265358979312;

bool isVectorForward(double const phi){
	if ( (phi > pi/2)  && (phi < 3*pi/2) )
		return false;
	else
		return true;
}


double distance_traveled(double theta, double phi, double x){

	static double xy = 0.;
	static double xyz = 0.;
	xy = sqrt(x*x*(TMath::Tan(phi)*TMath::Tan(phi) + 1));
	xyz = xy/TMath::Sin(theta);

	return xyz;
}


void findTimeBand(double& min_time, double& max_time, double const& emission_angle, ParticleOut const& particle){

	static Detector d;

	static Rotater r; r.Feed_Particle(particle.Theta, particle.Phi);
	r.ChangeFrame();


	int N = 100;
	static vector<double> phis; phis.resize(N,0.);

	static double target = 0.;
	static double delta_x = 0.;
	if (isVectorForward(particle.Phi)){
		target = 0.;
		delta_x = d.Length - particle.X;
	}
	else{
		target = pi;
		delta_x = particle.X;
	}

	static double target_difference = 0.;
	static double min_target_difference = 0.;
	static double closest_theta;
	static double closest_phi;
	static double current_theta;
	min_target_difference = 1.e10;

	for (unsigned i = 0; i < N; ++i){
		double& phi = phis.at(i);
		phi = i*((double)2*pi/N);
		current_theta = emission_angle;
		r.Rotate_Photon(current_theta, phi);

		target_difference = fabs(phi - target);
		if (target_difference < min_target_difference){
			min_target_difference = target_difference;
			closest_theta = current_theta;
			closest_phi = phi;
		}

	}

	static double photon_distance = 0.;
	photon_distance = distance_traveled(closest_theta, closest_phi, delta_x);

	static double shortest_time = 0.;
	shortest_time = photon_distance*1.e-2/3;

	min_time = shortest_time - .002;
	max_time = shortest_time + .008;
}









bool TallestBinContent(ParticleOut & particle, TH2D const& h, double const& smear, double const& width, double& center_min, double &center_max, vec_pair const&expected_photons, map<string,double> const& anglemap, bool const& print){
	TH1D* h1 = h.ProjectionY();

	static double pi2 = pi/2;
	for (unsigned i = 0; i < h1->GetNbinsX(); ++i){
		double bin_center = h1->GetBinCenter(i);
		if ((bin_center > 1.) || (bin_center < .3)){
			h1->SetBinContent(i, 0);
		}
	}

	int max_bin = h1->GetMaximumBin();
	double height = h1->GetBinContent(max_bin);
	double bin_width = h1->GetXaxis()->GetBinWidth(1);
	double Area = sqrt(2*pi)*smear*height/(bin_width) - (2*smear*height/5)/bin_width;
	unsigned i = 0;
	double minNSigma = 1.e10;
	for (auto& vals: expected_photons){
		double N = vals.first;
		// cout << "N = " << N << endl;
		// cout << "\tArea = " << Area << endl;
		// cout << "\theight = " << height << endl;
		double sigma = sqrt(Area);
		// cout << "\tsigma = " << sigma << endl;
		double nsigma = fabs(N-Area)/sigma;
		// cout << "\tnsigma = " << nsigma << endl;

		if (nsigma < minNSigma){
			minNSigma = nsigma;
		}
		++i;
	}
		// cout << "minNSigma = " << minNSigma << "\n\n";
	// if (minNSigma > 5) return false;
	if (height < 20) return false;
	double center = h1->GetBinCenter(max_bin);
	center_min = center - width;
	center_max = center + width;
	return true;
}

bool PeakNearExpectedThetas(ParticleOut & particle, TH2D const& h, double const& smear, double const& width, double& center_min, double &center_max, vec_pair const&expected_photons, map<string,double> const& anglemap, bool const& print){
	TH1D* h1 = h.ProjectionY();


	return true;
}







/**
 * NOT WORKING
 */
double FindPeakInRegion(TH1D *histogram, double const& center, double const range){

	double xlow = center - range/2;
	double xhi = center + range/2;
	for (unsigned i = 0; i < histogram->GetNbinsX(); ++i){
		double bin_center = histogram->GetBinCenter(i);
		if ((bin_center > xhi) || (bin_center < xlow))
			histogram->SetBinContent(i, 0);
	}

	// TSpectrum *s = new TSpectrum(20);
	// Int_t nfound = s->Search(histogram,1,"new");
	// Float_t *xpeaks = s->GetPositionX();
	// cout << "xpeaks =" << xpeaks[0] << endl;
	// delete s;
	// exit(1);
	return 0;
	// return xpeaks[0];
}

bool FindGaussianPeak(ParticleOut & particle, const TH2D& h, double const& smear, double& center_min, double &center_max, vec_pair const&expected_photons, map<string,double> const& anglemap, bool const& print){
	TH1D* h1 = h.ProjectionY();
	TH1D &hist = *h1;

	static double range = .05;
	auto emission_angle_map = particle.EmissionAngleMap();
	static vector<double> peaks;
	peaks.clear();
	for (auto i = emission_angle_map.begin(); i != emission_angle_map.end(); ++i){
		double &center = i->second;
		double peak = FindPeakInRegion(h1, center, range);
		peaks.push_back(std::move(peak));
		cout << "peak = " << peak << endl;
	}

	double maxheight = 0;
	int maxheightindex = -1;
	for (unsigned i = 0; i < peaks.size(); ++i){
		static double height;
		double const& peak = peaks.at(i);
		height = h1->GetBinContent(h1->FindBin(peak));
		cout << "height " << height << " at " << peak << endl;
		if (height > maxheight){
			maxheight = height;
			maxheightindex = i;
		}
	}

	double center = peaks.at(maxheightindex);
	cout << "max_peak = " << center << endl;
	center_min = center - 0.05;
	center_max = center + 0.05;
	return true;
	// static vector<double> centers;
	// centers.clear();
	// static double final_round = .05;
	// static double round_step = final_round/5;
	// static Rounder r(round_step);
	// r.setStepSize(round_step);
	// static unordered_map<double, int> center_to_count;
	// static unordered_map<double, int> center_to_height;
	// center_to_count.clear();
	// center_to_height.clear();
	// find values for gaussian peak


		// double center_try = i->second;
		// double width = smear;
		// cout << "center_try = " << center_try << " -> ";
		// FitGaussianPlusConstant(hist, xlow, xhi, center_try-.25, center_try+.25, 0, 10, center_try, width, constant, height);
		// cout << center_try << " rounded to " << r.round(center_try) << endl;
	// 	centers.push_back(center_try);
	// 	center_to_count[r.round(center_try)] = 0;
	// 	center_to_count[r.round(center_try)] = hist.GetBinHeight(FindBin(center_try));
	// }

	// // count the number of times each value occurs
	// for (auto& center_try: centers){
	// 	static double try_value = 0;
	// 	// cout << "try_value = " << center_try << " -> ";
	// 	try_value = r.round(center_try);
	// 	// cout << try_value << endl;
	// 	auto pos = center_to_count.find(try_value);
	// 	if (pos != center_to_count.end()){
	// 		++pos->second;
	// 	}
	// }

	// // find the mode
	// int most_counts = 0;
	// double mode_center;
	// double largestbin = 0;
	// double largestbin_center;
	// for (auto i = center_to_count.begin(); i != center_to_count.end(); ++i){
	// 	const double& center = i->first;
	// 	const int& count = i->second;
	// 	const double&height = center_to_height[center];
	// 	cout << "center " << center << " with " << count << " counts\n";
	// 	if (largestbin < height){
	// 		largestbin = height;
	// 		largestbin_center = center;
	// 	}
	// 	if (most_counts < count){
	// 		most_counts = count;
	// 		mode_center = center;
	// 	}
	// }
	// cout << endl;
	// // r.setStepSize(final_round);
	// center_min = largestbin_center - 0.025;
	// center_max = largestbin_center + 0.025;
}



// FIXME::error linking libraries
// bool HoughTransform(ParticleOut & particle, const TH2D& h, double const& smear, double& center_min, double &center_max, vec_pair const&expected_photons, map<string,double> const& anglemap, bool const& print){

//   int nbinsx = h.GetNbinsX();
//   int nbinsy = h.GetNbinsY();
//   cv::Mat image(nbinsx, nbinsy, CV_8UC1, cv::Scalar::all(0));

//   for (unsigned i = 0; i < nbinsx; ++i){
// 	  for	(unsigned j = 0; j < nbinsy; ++j){
// 	  	double val = h.GetBinContent(i,j);
// 	  	if (val == 0) continue;
// 	  	image.at<double>(i,j) = val;
// 	  }
//   }

//   vector<cv::Vec4i> lines;
//   HoughLinesP(image, lines, .1, .5, 80, 30., 10.);

//   for (auto& line: lines){
// 	  for (unsigned i = 0; i < 4; ++i)
// 	  	cout << line[i] << endl;
//   }
// }
