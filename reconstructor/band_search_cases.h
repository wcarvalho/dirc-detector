// #include <cv.hpp>
// #include <highgui.h>
#include "Rounder.h"
#include <math.h>
#include "reconstructor.h"
#include <unordered_map>
#include <TSpectrum.h>
double pi=3.14159265358979312;

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

void FindGaussianPeak(ParticleOut & particle, const TH2D& h, double const& smear, double& center_min, double &center_max, bool const& print){
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

void RecursiveCounting(ParticleOut & particle, const TH2D& h, double const& smear, double& center_min, double &center_max, bool const& print){
	TH1D* h1 = h.ProjectionY();
	TH1D &hist = *h1;

	auto emission_angle_map = particle.EmissionAngleMap();
	// hist.rebin()
	for (auto i = emission_angle_map.begin(); i != emission_angle_map.end(); ++i){

	}
}

void TallestBinContent(ParticleOut & particle, TH2D const& h, double const& smear, double& center_min, double &center_max, bool const& print){
	TH1D* h1 = h.ProjectionY();

	static double pi2 = pi/2;
	for (unsigned i = 0; i < h1->GetNbinsX(); ++i){
		double bin_center = h1->GetBinCenter(i);
		if (bin_center > pi2)
			h1->SetBinContent(i, 0);
	}
	int max_bin = h1->GetMaximumBin();
	double center = h1->GetBinCenter(max_bin);
	center_min = center -.025;
	center_max = center +.025;
}
// void HoughTransform(ParticleOut & particle, const TH2D& h, double const& smear, double& center_min, double &center_max, bool const& print){

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
