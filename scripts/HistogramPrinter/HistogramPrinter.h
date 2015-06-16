#include "TCanvas.h"
#include <iomanip>
#include "utility_library.h"
#include "TLatex.h"

typedef std::unordered_map<int, bool(*)(const Particle&, const TrackRecon&, const int&, const double&)> flag_fun_map;

double pi = 3.14159265358979312;




void print1DHistogram(TCanvas &C,TH1D& h, Particle &P, TrackRecon &R, flag_fun_map& fmap, const std::vector< int >& flags, const double& threshold, const string& filename, bool printFit){

	if (!(printFit)){
			TFile f(filename.c_str(), "update");
			h.Write();
			f.Close();

	}
	else{
		static TF1 f2("F", "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) ) + [3]");
		auto expected_angle_map = P.EmissionAngleMap();
		for (unsigned int i = 0; i < R.Params.size(); ++i){
			double xlow = R.Params.at(i).at(4);
			double xhi = R.Params.at(i).at(5);
			cout << "xlow - xhi = " << xhi - xlow << endl;
			static double center = 0;
			static double width = 0;
			double &N = R.ExpectedNumber.at(i);
			double &Area = R.Areas.at(i);

			for (unsigned int j = 0; j < 4; j++){
				f2.SetParameter(j, R.Params.at(i).at(j));
				if ( j == 1 ) center = R.Params.at(i).at(j);
				if ( j == 2 ) width = R.Params.at(i).at(j);
			}
			f2.SetRange(xlow, xhi);
			double expected_angle = expected_angle_map[R.Options.at(i)];

			if (!passConditions(flags, fmap, P, R, i, threshold)) continue;
			double momentum = P.CalculateMomentum();
			std::string histitle_part1 = dirc::appendStrings("", P.name);
			std::string histitle_part2 = dirc::appendStrings(" as ", R.Options.at(i));
			std::string histtitle = dirc::appendStrings(h.GetName(), histitle_part1, histitle_part2);
			h.SetTitle(histtitle.c_str());
			h.GetListOfFunctions()->Add(&f2);

			TFile f(filename.c_str(), "update");
			h.Write();
			f.Close();
			h.GetListOfFunctions()->Clear();
		}

	}
}