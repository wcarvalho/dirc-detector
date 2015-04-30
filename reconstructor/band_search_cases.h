#include "Simulate.h"
#include "reconstructor.h"

void TallestBinContent(ParticleOut & particle, TH2D const& h, double const& smear, double const& width, double& center_min, double &center_max, vec_pair const&expected_photons, bool const& print){
	TH1D* h1 = h.ProjectionY();

	for (unsigned i = 0; i < h1->GetNbinsX(); ++i){
		double bin_center = h1->GetBinCenter(i);
		if ((bin_center > 1.) || (bin_center < .3)){
			h1->SetBinContent(i, 0);
		}
	}

	int max_bin = h1->GetMaximumBin();
	double center = h1->GetBinCenter(max_bin);
	center_min = center - width;
	center_max = center + width;
}

void findTimeBand(double& time_min, double& time_max, ParticleOut& P){

	static double x_start;
	static double x_final;

	static Detector d;
	static Simulate sim(0.,0.);

	double& l = d.Length;
	double& w = d.Width;
	double& h = d.Height;

	sim.SetAngle(P.Theta, P.Phi);
	sim.SetDim(l, w, h);
	sim.SetStart(P.X, P.Y, 0.);

	sim.GotoWall();

	x_start = P.X;
	x_final = sim.coord[0];

	if ( (x_final - x_start) >= 0 ){
		time_min = l - x_final;
		time_max = l - x_start;
	}
	else{
		time_min = -x_start;
		time_max = -x_final;
	}

	static double width = 3;

	time_min -= width;
	time_max += width;
	if (time_max < time_min){
		cout << "ERROR! time max < time min!\n\nExiting!!\n"; exit(1);
	}
}