#include "dircTH1D.h"
#include "TFile.h"
#include <iomanip>
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

	int dircTH1D::findBinDistance(const double& from, const double& to){
		return (FindBin(from) - FindBin(to));
	}

	double dircTH1D::defineSigma(double const center_guess, double const sigma_guess, double const percent, bool print, std::string integral_setting){

		int center_bin = FindBin(center_guess);

		static int distributionRangeBins[2];
		for (unsigned i = 0; i < 2; ++i){
			distributionRangeBins[i] = FindBin(distributionRange[i]);
		}

		static unsigned steps_count; steps_count = 0;

		double distributionIntegral = Integral(distributionRangeBins[0], distributionRangeBins[1], integral_setting.c_str());
		if (print){
			std::cout << steps_count << ". Total distributionIntegral = " << distributionIntegral << std::endl;
			++steps_count;
		}

		static int sigmaBinDistance;
		sigmaBinDistance = std::abs(findBinDistance(center_guess, center_guess -sigma_guess));

		int sigmaBinDistance_start = sigmaBinDistance;
		if (print){
			std::cout << steps_count << ". Starting Bin Distance = " << sigmaBinDistance_start << std::endl;
			++steps_count;
		}

		auto WithinPercentRange = [percent] (const double& percentFound, const double& percentRange){
			static double percent_low; percent_low = percent-percentRange/2.;
			static double percent_high; percent_high = percent+percentRange/2.;
			bool greaterthanpercentlow = (percentFound > percent_low);
			bool lessthanpercenthigh = (percentFound < percent_high);
			return ( greaterthanpercentlow && lessthanpercenthigh );
		};

		bool switchdirection      = false;
		enum direction_t {forward = 1, backward = -1};
		direction_t direction     = forward, currentdirection;

		double percentFound  = 0.;
		int binlow           = 0, binhi = 0;
		int stepsize         = 1;
		int counts           = 0;

		auto getpercent = [&center_bin, &distributionIntegral, integral_setting, this](int& binlow, int& binhi, int sigmaBinDistance){
			binlow = center_bin - sigmaBinDistance;
			binhi  = center_bin + sigmaBinDistance;
			static double foundintegral;
			foundintegral = this->Integral(binlow, binhi, integral_setting.c_str());
			double percentFound = foundintegral/distributionIntegral;
			return std::move(percentFound);
		};

		auto checkDirection = [&percent](direction_t& direction, const double& percentFound){
			if (percentFound < percent) direction = forward;
			if (percentFound > percent) direction = backward;
		};

		if (print){
			std::cout << steps_count << ". Center at " << std::setprecision(4) << center_guess << " at bin " << center_bin << std::endl;
			++steps_count;
		}

		while(true){

			currentdirection = direction;
			percentFound = getpercent(binlow, binhi, sigmaBinDistance);
			if (print) std::cout << "\tpercentFound = " << percentFound << " at bin distance " << sigmaBinDistance << ", step size = " << stepsize << std::endl;
			 // in center range " << GetBinCenter(binlow) << " : " << this->GetBinCenter(binhi) <<  std::endl;
			if (counts >= 40)
				break;
			if (WithinPercentRange(percentFound, .10)){
				break;
			}
			checkDirection(direction, percentFound);

			stepsize *= 2;
			if (direction != currentdirection) {
				if (print) std::cout << "\tchanging direction\n";
				stepsize = 1;
			}
			sigmaBinDistance += stepsize*direction;
			if ((sigmaBinDistance < 0) && ( getpercent(binlow, binhi, 1) == 1)){
				distributionCenter = center_guess;
				double centerheight = GetBinContent(center_bin);
				distributionSigma = sqrt(centerheight);
				sigmaBinDistance = std::abs(findBinDistance(distributionCenter, distributionSigma));
				return getpercent(binlow, binhi, sigmaBinDistance);
			}
			else if (sigmaBinDistance < 0) {
				direction = forward;
				sigmaBinDistance_start *= .5;
				sigmaBinDistance = (int)sigmaBinDistance_start;
			}
			++counts;
		}

		double percentDifference = 1;
		double tempdif;
		stepsize = 1;
		counts = 0;
		if (print) std::cout << "\tROUND 2\n";
		while(true){
			percentFound = getpercent(binlow, binhi, sigmaBinDistance);
			tempdif = fabs(percentFound - percent);
			if (counts >= 30) break;
			if (print) std::cout << "\tpercentFound = " << percentFound << " at bin distance " << sigmaBinDistance <<  " with difference " << tempdif << std::endl;
			if(tempdif <= percentDifference)
				percentDifference = tempdif;
			else{
				sigmaBinDistance -= stepsize*direction;
				percentFound = getpercent(binlow, binhi, sigmaBinDistance);
				break;
			}

			checkDirection(direction, percentFound);
			sigmaBinDistance += stepsize*direction;
			++counts;
		}

		auto findSigma = [&center_guess, &center_bin, this](int& sigmaBinDistance){
			double lower_edge = this->GetBinCenter(center_bin - sigmaBinDistance);
			return (center_guess - lower_edge);
		};

		distributionCenter = center_guess;
		distributionSigma  = findSigma(sigmaBinDistance);
		return percentFound;
	}

	void dircTH1D::RemovePastSigma(bool print){
		static double sigma = 0.;
		for (unsigned int i = 0; i < fNcells; ++i){
			sigma = fabs(distributionCenter - GetBinCenter(i));
			if (sigma < distributionSigma) continue;
			// if(print)	std::cout << "Sigma: " << sigma << " vs. distributionSigma: " << distributionSigma << std::endl;
			SetBinContent(i, 0);
		}

	}

}
