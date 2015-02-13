#include "graphing.h"
namespace dirc
{

	void dircTH1D::BinProperties(const double& value, int &value_bin, double &binheight, double &bincenter){

		value_bin = FindBin(value);
		binheight = GetBinContent(value_bin);
		bincenter = GetBinCenter(value_bin);

	}

	void dircTH1D::FindDistributionCenter(double &center_start, double &sigma_start){

		double centerheight = 0., center = 0.;
		int centerbin = 0;
		BinProperties(GetMean(), centerbin, centerheight, center);
		if (centerheight < 0.3*GetBinContent(GetMaximumBin())){
			center = GetBinCenter(GetMaximumBin());
			centerheight = GetBinContent(GetMaximumBin());
		}
		double sigma_guess = sqrt(centerheight);
		double &amplitude_guess = centerheight;

		double xlow = GetXaxis()->GetXmin();
		double xhi = GetXaxis()->GetXmax();
		static TF1 fitfunc("f",
			"[0]*exp(-(x-[1])*(x-[1])/2*[2]*[2])",
			xlow, xhi);
		fitfunc.SetRange(xlow, xhi);
		fitfunc.SetParameter(0, amplitude_guess);
		fitfunc.SetParameter(1, center);
		fitfunc.SetParLimits(1, xlow, xhi);
		fitfunc.SetParameter(2, sigma_guess);

		Fit(&fitfunc, "QBIRCN0");
		center_start = fitfunc.GetParameter(1);
		sigma_start = fitfunc.GetParameter(2);
	}

	double dircTH1D::defineSigma(const double center_guess, const double &sigma_guess, const double &percent, bool print){

		int center_bin = FindBin(center_guess);
		int distributionRangeBins[2];
		for (unsigned i = 0; i < 2; ++i)
			distributionRangeBins[i] = FindBin(distributionRange[i]);
		double distributionIntegral = Integral(distributionRangeBins[0], distributionRangeBins[1], "width");

		int sigmaBinDistance;
		sigmaBinDistance = fabs(center_bin - FindBin(center_guess-sigma_guess));




		auto notWithinRange = [percent] (const double& percentFound, const double& percentRange){
			return !((percentFound > percent-percentRange/2.)&&(percentFound < percent+percentRange/2.));
		};
		bool switchdirection      = false;
		enum direction_t {forward = 1, backward = -1};
		direction_t direction     = forward, currentdirection;

		double foundintegral = 0., percentFound = 0.;
		int binlow           = 0, binhi = 0;
		int stepsize         = 1;
		int counts           = 0;

		auto getpercent = [center_bin, &sigmaBinDistance, &binlow, &binhi, &foundintegral, &distributionIntegral, &percentFound, this](){
			binlow = center_bin - sigmaBinDistance;
			binhi  = center_bin + sigmaBinDistance;
			foundintegral = this->Integral(binlow, binhi, "width");
			percentFound  = foundintegral/distributionIntegral;
		};

		if (print) std::cout << "center_guess = " << center_guess << std::endl;
		std::unordered_set < double > found_values;
		while(true){
			currentdirection = direction;
			getpercent();
			if (print) std::cout << "percentFound = " << percentFound << " at bin distance " << sigmaBinDistance << std::endl;
			found_values.insert(percentFound);
			if (counts >= 3) {
				found_values.erase(found_values.begin());
				if (found_values.find(percentFound) == found_values.end()) break;
			}
			if (!notWithinRange(percentFound, .15)){
				break;
			}
			if (percentFound < percent) direction = forward;
			if (percentFound > percent) direction = backward;

			stepsize *= 2;
			if (direction != currentdirection) switchdirection = true;
			if (switchdirection) { stepsize = 1; switchdirection = false; }
			sigmaBinDistance += stepsize*direction;
			++counts;
		}

		if (print) std::cout << "\n\n";
		double percentDifference = 1;
		double tempdif;
		stepsize = 1;
		while(true){
			getpercent();
			tempdif = fabs(percentFound - percent);
			if (print) std::cout << "percentFound = " << percentFound << " at bin distance " << sigmaBinDistance << " with different " << tempdif << std::endl;
			if(tempdif < percentDifference)
				percentDifference = tempdif;
			else{
				sigmaBinDistance -= stepsize*direction;
				getpercent();
				break;
			}

			if (percentFound < percent) direction = forward;
			if (percentFound > percent) direction = backward;
			sigmaBinDistance += stepsize*direction;

		}



		double lower_edge = GetBinCenter(center_bin - sigmaBinDistance);
		double upper_edge = GetBinCenter(center_bin + sigmaBinDistance);
		distributionCenter = center_guess;
		distributionSigma  = center_guess - lower_edge;
		return percentFound;
	}

	void dircTH1D::RemovePastSigma(bool print){
		double sigma = 0.;
		for (unsigned int i = 0; i < fNcells; ++i){
			sigma = fabs(distributionCenter - GetBinCenter(i));
			if (sigma > distributionSigma){
				if(print){
					std::cout << "Sigma: " << sigma << " vs. distributionSigma: " << distributionSigma << std::endl;
				}
				SetBinContent(i, 0);
			}
		}

	}

}