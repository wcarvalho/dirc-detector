#include "TCanvas.h"
#include <iomanip>
#include "utility_library.h"
#include "TLatex.h"

typedef std::unordered_map<int, bool(*)(const Particle&, const TrackRecon&, const int&)> flag_fun_map;

double pi = 3.14159265358979312;
// prints histograms of fits
void print1Dfits(TCanvas &C, int Event, int par, Particle &P, TrackRecon &R, std::string const dir, flag_fun_map& fmap, const std::vector< int >& flags, double threshold){
	TH1D &h = R.Hist;

	std::stringstream ss; ss.str("");
	ss << dir << "Event_" << std::setfill('0') <<std::setw(3) << Event << "_";
	ss << "Particle_" << std::setfill('0') <<std::setw(3) << par << "_";
	ss << P.name;
	std::string base_name = ss.str();
	// h.SetName(base_name.c_str());

	std::stringstream ss2; ss2.str("");
	ss2 << dir << P.name << "_1Dfits.root";
	std::string filename = ss2.str();

	TF1 f2("F", "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) ) + [3]");

	// TF1 expectedangle_func("F", "(x==[0])*1");
	auto expected_angle_map = P.EmissionAngleMap();
	for (unsigned int i = 0; i < R.Params.size(); ++i){
		double latmax = 1;
		double xlow = R.Params.at(i).at(4);
		double xhi = R.Params.at(i).at(5);
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

		//------------------------------
		// static bool printfit;
		// printfit = true;
		// for (unsigned int f = 0; f < flags.size(); ++f){
		// 	const int& condition = flags.at(f);
		// 	printfit *= fmap[condition](P, R, i);
		// }
		// if (!printfit) continue;
		if (!passConditions(flags, fmap, P, R, i)) continue;

		//------------------------------
		// expectedangle_func.SetParameter(0, expected_angle);


		// printFitInfo(h, xlow, xhi, P.ConeAngle, width, N, ss2.str());

		double momentum = P.CalculateMomentum();

		std::string histitle_part1 = wul::appendStrings("", P.name);
		std::string histitle_part2 = wul::appendStrings(" as ", R.Options.at(i));
		std::string histitle_part3 = wul::stringDouble(", p = ", momentum);
		std::string histitle_part4 = wul::stringDouble(", emissionangle = ", expected_angle);
		std::string histtitle = wul::appendStrings(histitle_part1, histitle_part2, histitle_part3, histitle_part4);
		h.SetTitle(histtitle.c_str());
		// h.GetXaxis()->SetRangeUser(center-.25,center+.25);
		h.GetListOfFunctions()->Add(&f2);
		// h.GetListOfFunctions()->Add(&expectedangle_func);

		// f2.Draw("same");


		TLatex T1;
		T1.SetTextAlign(23);
		T1.SetTextSize(0.03);
		// T1.DrawLatex(2, latmax*h.GetMaximum(),
		//              wul::appendStrings("Particle = ", P.name).c_str()); latmax -= .05;
		// T1.DrawLatex(2, latmax*h.GetMaximum(),
		//              wul::appendStrings("Guess = ", R.Options.at(i)).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("Sigma = ", R.Sigmas.at(i)).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("Eta = ", P.Eta).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("p = ", momentum).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("expected emission angle = ", expected_angle).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("found emission angle = ", center).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("Expected Photons = ", N).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("Expected From Fit = ", Area).c_str()); latmax -= .05;
		double IncidentTheta = P.Theta/pi;
		double IncidentPhi = P.Phi/pi;
		std::string IncidentAngleStr = wul::appendStrings(wul::stringDouble("Incident #theta, #phi = ", IncidentTheta), "#pi, ");
		T1.DrawLatex(2, latmax*h.GetMaximum(), wul::appendStrings(wul::stringDouble(IncidentAngleStr, IncidentPhi), "#pi").c_str()); latmax -= .05;

		TFile f(filename.c_str(), "update");
		h.Write(histtitle.c_str());
		// C.Write(filename.c_str());
		f.Close();
		C.Clear();
	}

}