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
		fitfunc.SetParameter(2, sigma_guess);

		Fit(&fitfunc, "QBIRC");
		center_start = fitfunc.GetParameter(1);
		sigma_start = fitfunc.GetParameter(2);
	}

	void dircTH1D::defineSigma(const double &percent){

		double center_guess=0., sigma_guess=0.;
		FindDistributionCenter(center_guess, sigma_guess);
		defineDistributionRange(5, sigma_guess, center_guess);

		int center_bin = FindBin(center_guess);

		int distributionRangeBins[2];
		for (unsigned i = 0; i < 2; ++i)
			distributionRangeBins[i] = FindBin(distributionRange[i]);
		double distributionIntegral = Integral(distributionRangeBins[0], distributionRangeBins[1], "width");

		int sigmaBinDistance;
		sigmaBinDistance = abs(center_bin - FindBin(center_guess-sigma_guess));


		double foundintegral = 0., percentFound = 0.;
		int binlow           = 0, binhi = 0;
		int stepsize         = 1;

		bool switchdirection      = false;
		enum direction_t 					{forward = 1, backward = -1};
		direction_t direction     = forward, currentdirection;

		auto notWithinRange = [percent] (const double& percentFound){ return !((percentFound > percent-.25)&&(percentFound < percent+.25)); };

		while (notWithinRange(percentFound)){
			currentdirection = direction;

			binlow = center_guess - sigmaBinDistance;
			binhi  = center_guess + sigmaBinDistance;

			foundintegral = Integral(binlow, binhi, "width");
			percentFound  = foundintegral/distributionIntegral;

			if (percentFound < percent) direction = forward;
			if (percentFound > percent) direction = backward;
			if (direction != currentdirection) switchdirection = true;
			if (switchdirection) stepsize = 1;

			sigmaBinDistance += stepsize*direction;
			stepsize *= 2;
		}

		distributionCenter = center_guess;
		distributionSigma  = center_guess - GetBinCenter(center_guess - sigmaBinDistance);

	}

}