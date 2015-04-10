#include "TLatex.h"
#include "TObject.h"

typedef std::unordered_map<int, bool(*)(const Particle&, const TrackRecon&, const int&, const double&)> func_map;

template < typename T>
bool withinrange(vector<T>& range, T value){
	if ( (value < range.at(0)) || (value > range.at(1)) )
		return false;
	else return true;
}

void checkValid(const TFile& f){
	if (!(f.IsOpen())) {
		cout << f.GetName() << " invalid!\n";
		exit(1);
	}
}
std::string findBestFit(TrackRecon const& R, double const& threshold){

	static double min_nSigma {0.}; min_nSigma = 1e1000;
	static double nSigma {0.};
	static int index_of_bestfit {0};
	if (R.size() == 0) return "";
	// cout << "options: " << R.size() << endl;
	for (int i = 0; i < R.size(); ++i){
		nSigma = fabs(R.getnSigmaThetaAt(i));
		// cout << "\tmin_nSigma = " << min_nSigma << endl;
		// cout << "\tnSigma = " << nSigma << endl;
		if (nSigma < min_nSigma){
			min_nSigma = nSigma;
			index_of_bestfit = i;
		}
	}
	return R.getNameAt(index_of_bestfit);
}

void addScatterPlot(TPad& pad, TMultiGraph *mg, TLegend& L, vector<ParticleOut> const& particles, vector<PhotonOut> const& photons, vector<int> const& index, int particle_index){

	DrawScatterPlot(mg, L, particles, photons, index, particle_index);

	mg->SetTitle("Indexed Photons; #phi (radians); #theta (radians)");

	pad.cd();
	mg->Draw("AP");

	pad.Update();
	pad.Modified();
}

void addFits(TH1D &h, TrackRecon const& R, vector<TF1*>& functions){

	if (R.Params.empty()) return;

	static double center_min;
	center_min = 1.e10;
	static double center_max;
	center_max = 0.;

	for (int i = 0; i < R.size(); ++i){
		TF1* f2_p = new TF1(R.getNameAt(i).c_str(), "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) ) + [3]");
		TF1 &f2 = *f2_p;
		for (unsigned int j = 0; j < 4; j++)
				f2.SetParameter(j, R.Params.at(i).at(j));

		const double& xlow = R.Params.at(i).at(4);
		const double& xhi = R.Params.at(i).at(5);
		f2.SetRange(xlow, xhi);
		f2.SetLineColor(i+4);
		double const& center = R.getIntegralCenterAt(i);
		if (center < center_min) center_min = center;
		if (center > center_max) center_max = center;
		functions.push_back(std::move(f2_p));
		h.GetListOfFunctions()->Add(functions.back());
	}

}


void addFit(TH1D &h, TrackRecon const& R, int search_index){

	if (R.Params.empty()) return;
	static TF1 f2("F", "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) ) + [3]");


	for (unsigned int j = 0; j < 4; j++)
			f2.SetParameter(j, R.Params.at(search_index).at(j));

	const double& xlow = R.Params.at(search_index).at(4);
	const double& xhi = R.Params.at(search_index).at(5);
	f2.SetRange(xlow, xhi);

	double const& center = R.getIntegralCenterAt(search_index);
	h.GetXaxis()->SetRangeUser(center-.1, center+.1);

	h.GetListOfFunctions()->Add(&f2);

}


TH1D* addReducedHistogram(TPad& pad, TrackRecon const& R, vector<PhotonOut> const& photons, vector<int> const& index, int particle_index, int search_index, vector<TF1*> const& functions){

	auto& h2 = R.Hist2D;
	double xlow = .4;
	double xhi = 1.;
	double binwidth = h2.GetYaxis()->GetBinWidth(1);
	int nbins = (xhi-xlow)/binwidth;

	string histname = wul::appendStrings(h2.GetName(), "1D");

	TH1D* h1 = CreateReducedHistogram(photons, index, particle_index, histname, nbins, xlow, xhi);

	h1->SetTitle("Reduced #theta Projection");
	h1->GetXaxis()->SetTitle("#theta (radians)");
	h1->SetDefaultSumw2();
	h1->SetStats(0);

	// cout << "\t\tprefit\n";
	// addFit(*h1, R, search_index);
	for (unsigned i = 0; i < functions.size(); ++i){
		h1->GetListOfFunctions()->Add(functions.at(i));
	}
	// cout << "\t\tpostfit\n";

	pad.cd();
	h1->Draw();

	pad.Update();
	pad.Modified();
	return std::move(h1);
}





TH1D* addFullHistogram(TPad& pad, TrackRecon const& R, vector<PhotonOut> const& photons, vector<int> const& index, int particle_index, int search_index, vector<TF1*>& functions){

	auto& h2 = R.Hist2D;
	double xlow = .4;
	double xhi = 1.;

	TH1D* h1 = h2.ProjectionY();

	h1->SetDefaultSumw2();
	h1->SetStats(0);
	h1->SetTitle("#theta Projection");
	h1->GetXaxis()->SetTitle("#theta (radians)");
	// addFit(*h1, R, search_index);
	addFits(*h1, R, functions);

	pad.cd();
	h1->Draw();

	pad.Update();
	pad.Modified();

	return std::move(h1);
}


void AddEventDetails(TPad &pad, TLegend& L, ParticleOut & P, TrackRecon const& R, int const& particle_index, double const& momentum, string particle_search, double threshold){

	pad.cd();
	stringstream ss;
	int precision = 2;


	// particle incidence angle
	string fitas = findBestFit(R, threshold);
	if (fitas != ""){
		ss.str(""); ss << "fit as "<< fitas;
		L.AddEntry((TObject*)0, ss.str().c_str(),"");
	}

	L.Draw();

	pad.Update();
	pad.Modified();
}

void AddFitDetails(TPad &pad, TLegend& L, ParticleOut & P, TrackRecon const& R, vector<TF1*> functions){

	pad.cd();
	stringstream ss;
	int precision = 2;
	auto anglemap = P.EmissionAngleMap();

	// theta reconstruction information
	for (unsigned i = 0; i < functions.size(); ++i){
		string name = R.getNameAt(i);
		TF1*& function = functions.at(i);
		// expected angle
		double expected_angle = anglemap[name];
		ss.str(""); ss << "#scale[1.2]{#bf{" << name << "}}";
		L.AddEntry(function, ss.str().c_str(),"l");


		if (R.Params.empty()) continue;
		// found angle

		ss.str(""); ss << std::setprecision(5) << expected_angle <<
		" vs. " << std::setprecision(5) << R.getIntegralCenterAt(i);
		L.AddEntry(function, ss.str().c_str(),"");
		// nsigma
		ss.str(""); ss << "#Delta #sigma = "<< std::setprecision(2) << fabs(R.getnSigmaThetaAt(i));
		L.AddEntry(function, ss.str().c_str(),"");
		L.Draw();
	}



	pad.Update();
	pad.Modified();



}
void AddCombination(string canvasname, TFile& f, vector<ParticleOut> & particles, vector<PhotonOut> const& photons, vector<int> const& index, int particle_index, TrackRecon const& R, int search_index, double const& momentum, string particle_type, string particle_search, double threshold){


	int length = 1920;
	int height = 1080;
  static TCanvas C("C", "C", length, height);
  C.SetName(canvasname.c_str());
  C.Clear();


  C.Divide(1,2);
  TPad* upperpad = (TPad*)C.GetPad(1);
  TPad* lowerpad = (TPad*)C.GetPad(2);

  upperpad->Divide(2,1);
  TPad* upperleft = (TPad*)upperpad->GetPad(1);
  TPad* upperright = (TPad*)upperpad->GetPad(2);
	upperleft->SetPad( 0, 0, .15, 1. );
	upperright->SetPad( .15, 0, 1. , 1. );

  lowerpad->Divide(3,1);
  TPad* lowerleft = (TPad*)lowerpad->GetPad(1);
  TPad* lowermiddle = (TPad*)lowerpad->GetPad(2);
  TPad* lowerright = (TPad*)lowerpad->GetPad(3);
	lowerleft->SetPad(0., 0., .45, 1.);
	lowermiddle->SetPad(.41, 0., .85, 1.);
	lowerright->SetPad(.81, 0., 1. , 1.);

  TMultiGraph *mg = new TMultiGraph();
	TLegend LScatter(0.15, 0.1, 1., 1., "Particles Incidence Angle (#theta, #phi)");
	addScatterPlot(*upperright, mg, LScatter, particles, photons, index, particle_index);
	LScatter.SetTextSize(.06);
	upperleft->cd();
	LScatter.Draw();
	upperleft->Update();
	upperleft->Modified();
	// cout << "\t\taddScatterPlot\n";

	vector< TF1* > functions;

	TH1D* hf = addFullHistogram(*lowerleft, R, photons, index, particle_index, search_index, functions);
	hf->GetXaxis()->SetRangeUser(.7, .85);
	TH1D* hr = addReducedHistogram(*lowermiddle, R, photons, index, particle_index, search_index, functions);
	hr->GetXaxis()->SetRangeUser(.7, .85);
	// cout << "\t\taddReduced\n";

	// cout << "\t\taddFull\n";

	stringstream ss; ss << "Particle "<< particle_index << ": " << particle_type << " with p = "<< setprecision(2) << momentum << "GeV";
	TLegend L(0., 0.9, 0.35, 1.0, ss.str().c_str());
	L.SetTextSize(.035);

  AddEventDetails(*lowermiddle, L, particles.at(particle_index), R, search_index, momentum, particle_search, threshold);

	TLegend FitLegend(0., 0., 1., 1., "Emission Angle: Expected Vs. Found");
	FitLegend.SetTextSize(.045);
	AddFitDetails(*lowerright, FitLegend, particles.at(particle_index), R, functions);

	f.cd();
	C.Write();

	hr->GetListOfFunctions()->Clear();
	hf->GetListOfFunctions()->Clear();
	delete hr;
	delete mg;
	for (auto& function: functions)
		delete function;
}

void AddBatch(int event, int match, TFile& f, vector<ParticleOut> & particle_outs, vector<Particle> & particles, vector< vector<PhotonOut> > const& photon_sets, vector<int> const& index, vector< TrackRecon> & reconstructions, string & particle_compare, unsigned& count, double threshold){

	static stringstream ss;
	static double momentum;
	for (unsigned i = 0; i < particle_outs.size(); ++i){
		ss.str(""); ss << "Event_" << event << "_Particle_" << i;
		if (i == match) ss << "_MATCH";
		auto& par = particles.at(i);
		auto& par_out = particle_outs.at(i);
		auto& photons = photon_sets.at(i);
		auto& recon = reconstructions.at(i);

		momentum = par.CalculateMomentum();
		int search_index = getReconIndex(recon, particle_compare);
		// cout << "AddBatch " << i << endl;
		AddCombination(ss.str(), f, particle_outs, photons, index, i, recon, search_index, momentum, par.GetName(), particle_compare, threshold);
	}

	++count;

}