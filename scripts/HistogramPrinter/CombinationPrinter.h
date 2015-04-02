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





void addScatterPlot(TPad& pad, TMultiGraph *mg, vector<ParticleOut> const& particles, vector<PhotonOut> const& photons, vector<int> const& index, int particle_index){
	
	
	TLegend L;
	DrawScatterPlot(mg, L, particles, photons, index, particle_index);

	mg->SetTitle("Indexed Photons; #phi (radians); #theta (radians)");

	pad.cd();
	mg->Draw("AP");
	L.Draw();

	pad.Update();
	pad.Modified();

}




void addFit(TH1D &h, TrackRecon const& R, int search_index){

	static TF1 f2("F", "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) ) + [3]");
	for (unsigned int j = 0; j < 4; j++)
			f2.SetParameter(j, R.Params.at(search_index).at(j));

	const double& xlow = R.Params.at(search_index).at(4);
	const double& xhi = R.Params.at(search_index).at(5);
	f2.SetRange(xlow, xhi);

	double const& center = R.getIntegralCenterAt(search_index);
	h.GetXaxis()->SetRangeUser(center-.075, center+.075);

	h.GetListOfFunctions()->Add(&f2);

}


TH1D* addReducedHistogram(TPad& pad, TrackRecon const& R, vector<PhotonOut> const& photons, vector<int> const& index, int particle_index, int search_index){

	auto& h2 = R.Hist2D;
	double xlow = .4;
	double xhi = 1.;
	double binwidth = h2.GetXaxis()->GetBinWidth(1);
	int nbins = (xhi-xlow)/binwidth;

	string histname = wul::appendStrings(h2.GetName(), "1D");

	TH1D* h1 = CreateReducedHistogram(photons, index, particle_index, histname, nbins, xlow, xhi);
	h1->SetTitle("Reduced #theta Projection");
	h1->GetXaxis()->SetTitle("#theta (radians)");
	h1->SetDefaultSumw2();
	h1->SetStats(0);

	addFit(*h1, R, search_index);
	pad.cd();
	h1->Draw();

	pad.Update();
	pad.Modified();
	return std::move(h1);
}





TH1D* addFullHistogram(TPad& pad, TrackRecon const& R, vector<PhotonOut> const& photons, vector<int> const& index, int particle_index, int search_index){

	auto& h2 = R.Hist2D;
	double xlow = .4;
	double xhi = 1.;

	TH1D* h1 = h2.ProjectionY();

	h1->SetTitle("#theta Projection");
	h1->GetXaxis()->SetTitle("#theta (radians)");
	h1->SetDefaultSumw2();
	h1->SetStats(0);
	addFit(*h1, R, search_index);
	
	pad.cd();
	h1->Draw();

	pad.Update();
	pad.Modified();

	return std::move(h1);
	
}


void AddEventDetails(TPad &pad, TLegend& L, ParticleOut & P, TrackRecon const& R, int const& particle_index, double const& momentum, string particle_search){

	pad.cd();
	stringstream ss;
	int precision = 2;


	// particle incidence angle
	ss.str(""); ss << "Incidence Angle: "<< std::setprecision(precision) << P.Theta << ", "<< std::setprecision(precision) << P.Phi;
	L.AddEntry((TObject*)0, ss.str().c_str(),"");
	
	// momentum information
	ss.str(""); ss << "Momentum: " << std::setprecision(precision) << momentum;
	L.AddEntry((TObject*)0, ss.str().c_str(),"");

	
	// theta reconstruction information
	// expacted angle
	auto anglemap = P.EmissionAngleMap();
	double expected_angle = anglemap[particle_search];
	ss.str(""); ss << "Expected Emission Angle: " << std::setprecision(5) << expected_angle;
	L.AddEntry((TObject*)0, ss.str().c_str(),"");

	// found angle
	ss.str(""); ss << "Found Emission Angle: "<< std::setprecision(5) << R.getIntegralCenterAt(particle_index);
	L.AddEntry((TObject*)0, ss.str().c_str(),"");

	// nsigma
	ss.str(""); ss << "nSigma: "<< std::setprecision(2) << fabs(R.getnSigmaThetaAt(particle_index));
	L.AddEntry((TObject*)0, ss.str().c_str(),"");
	L.Draw();

	pad.Update();
	pad.Modified();

}

void AddCombination(TFile& f, vector<ParticleOut> & particles, vector<PhotonOut> const& photons, vector<int> const& index, int particle_index, TrackRecon const& R, int search_index, double const& momentum, string particle_search, unsigned& count){
  

	int length = 1920;
	int height = 1080;
  static TCanvas C("C", "C", 1920, 1080);
  C.Clear();

  
  C.Divide(1,2);
  TPad* upperpad = (TPad*)C.GetPad(1);
  TPad* lowerpad = (TPad*)C.GetPad(2);
  lowerpad->Divide(2,1);
  TPad* lowerleft = (TPad*)lowerpad->GetPad(1);
  TPad* lowerright = (TPad*)lowerpad->GetPad(2);
  TMultiGraph *mg = new TMultiGraph();
	addScatterPlot(*upperpad, mg, particles, photons, index, particle_index);

	int i = 0;
	TH1D* hr = addReducedHistogram(*lowerright, R, photons, index, particle_index, search_index);	

	TH1D* hf = addFullHistogram(*lowerleft, R, photons, index, particle_index, search_index);

	stringstream ss; ss << "Particle Details: " << particle_search;
	TLegend L(0.7, 0.7, 1.0, 1.0, ss.str().c_str());
	L.SetTextSize(.03);
  AddEventDetails(*lowerleft, L, particles.at(particle_index), R, search_index, momentum, particle_search);
	f.cd();
	C.Write();
	
	hr->GetListOfFunctions()->Clear();
	hf->GetListOfFunctions()->Clear();
	delete hr;
	delete mg;
	++count;
}

