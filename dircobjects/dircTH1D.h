#ifndef ___miscgraphing___
#define ___miscgraphing___

#include "TH1D.h"
#include "TF1.h"
// #include <unordered_set>

namespace dirc
{
	class dircTH1D : public TH1D
	{
	public:
		using TH1D::TH1D;


		void BinProperties(const double& value, int &value_bin, double &binheight, double &bincenter);
		void FindDistributionCenter(double &center_start, double &sigma_start);
		double getWidth() const { return distributionSigma; };
		void defineDistributionRange(const int& nsigma, const double& sigma, const double& center){
			distributionRange[0] = center-nsigma*sigma; distributionRange[1] = center+nsigma*sigma;
		}
		void defineDistributionRange(const double& xlow, const double& xhi){
			distributionRange[0] = xlow; distributionRange[1] = xhi;
		}

		int findBinDistance(const double& from, const double& to);

		double defineSigma(double const center_guess, double const sigma_guess, double const percent, bool print=false, std::string integral_setting = "");
		void RemovePastSigma(bool print = false);


		double distributionCenter;
		double distributionSigma;
		double distributionRange[2];

	};
}

#endif