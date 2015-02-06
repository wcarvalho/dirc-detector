#ifndef ___miscgraphing___
#define ___miscgraphing___

#include "TH1D.h"
#include "TF1.h"

namespace dirc
{
	class dircTH1D : public TH1D
	{
	public:

		void BinProperties(const double& value, int &value_bin, double &binheight, double &bincenter);

		void FindDistributionCenter(double &center_start, double &sigma_start);

		void defineDistributionRange(const int& nsigma, const double& sigma, const double& center){
			distributionRange[0] = center-nsigma*sigma; distributionRange[1] = center+nsigma*sigma;
		}
		void defineDistributionRange(const double& xlow, const double& xhi){
			distributionRange[0] = xlow; distributionRange[1] = xhi;
		}
		void defineSigma(const double &percent);

		double distributionCenter;
		double distributionSigma;
		double distributionRange[2];

	};
}

#endif