#include "FileProperties.h"
#include "TError.h"
/**
 * [fillvectors description]
 * @param expected               [description]
 * @param actual                 [description]
 * @param sigma                  [description]
 * @param vexpected_actual_sigma [description]
 * @param vexpected_actual       [description]
 * @param vexpected              [description]
 */
void fillvectors(double const& expected, double const& actual, double const& sigma, vector<double >& vexpected_actual_sigma, vector<double >& vexpected_actual, vector<double >& vexpected){

	double dif = (expected-actual);
	double dif_sig = dif/sigma;
	vexpected_actual_sigma.push_back(dif_sig);
	vexpected_actual.push_back(dif);
	vexpected.push_back(expected);

}
/**
 * [makehist description]
 * @param C        [description]
 * @param x        [description]
 * @param filename [description]
 */
TH1D makehist(vector<double> const& x, string filename, int nbins){

	wul::FileProperties fp(filename);
	string &name = fp.filename;
	double xlow = 0.; double xhi = 0.;
	wul::vectorminmax(x, xlow, xhi);

	TH1D h(name.c_str(), name.c_str(), nbins, xlow, xhi);
	for (unsigned int i = 0; i < x.size(); ++i)
		h.Fill(x.at(i));

	return std::move(h);

}

// void normalizehist(TH1D &h, int nbins){
// 	double nentries = h.GetEntries();
// 	cout << h.GetName() << endl;
// 	for (unsigned int i = 0; i < nbins; ++i){
// 		cout << "h.GetBinContent(i) = " << h.GetBinContent(i) << endl;
// 		cout << "h.GetBinContent(i)/nentries = " << h.GetBinContent(i)/nentries << endl;
// 		h.SetBinContent(i, h.GetBinContent(i)/nentries);
// 	}
// }

void maxBinProperties(TH1D &h, double &binheight, double &bincenter){

	int tallestbin = h.GetMaximumBin();
	binheight = h.GetBinContent(tallestbin);
	bincenter = h.GetBinCenter(tallestbin);
}

void fitBeta(TH1D &h, vector<double> const& x, double &mean, double &sigma){

	auto xlow = 0., xhi = 0.;
	wul::vectorminmax(x, xlow, xhi);

	auto binheight = 0., bincenter = 0.;
	maxBinProperties(h, binheight, bincenter);

	std::stringstream ss; ss.str("");
	ss << "( [0]*TMath::BetaDist( (x + [1] ),[2],[3]) )";
	string func = ss.str();

	static TF1 f("beta_distribution",func.c_str(), -2, 2);
	f.SetParameters(8, 1, 80, 1);

	gErrorIgnoreLevel = 5000;

	h.Fit(&f, "QBIRC");

	const double &alpha = f.GetParameter(1);
	const double &beta = f.GetParameter(2);

	mean = alpha/(alpha+beta);
	// sigma from standard deviation
	// double variance = (alpha*beta)/( (alpha+beta)*(alpha+beta)*(alpha+beta+1) );
	// sigma = sqrt(variance);
	// sigma from absolute deviation about mean
	sigma = 2*pow(alpha, alpha)*pow(beta,beta)/(TMath::Beta(alpha, beta)*pow( (alpha+beta), alpha+beta+1));
	// cout << "alpha = " << alpha << endl;
	// cout << "beta = " << beta << endl;
	// cout << "pow(alpha, alpha) = " << pow(alpha, alpha) << endl;
	// cout << "pow(beta,beta) = " << pow(beta,beta) << endl;
	// cout << "TMath::Beta(alpha, beta) = " << TMath::Beta(alpha, beta) << endl;
	// cout << "pow( (alpha+beta), alpha+beta+1) = " << pow( (alpha+beta), alpha+beta+1) << endl;
}

void fitGaussian(TH1D &h, vector<double> const& x, double& mean,double &sigma){

	auto xlow = 0., xhi = 0.;
	wul::vectorminmax(x, xlow, xhi);

	auto binheight = 0., bincenter = 0.;
	maxBinProperties(h, binheight, bincenter);

	static TF1 f("gauss", "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) )", xlow, xhi);
	f.SetParameter(0, binheight);
	f.SetParameter(1, bincenter);
	f.SetParameter(2, 20);

	h.Fit(&f, "QBIRC");

	mean = f.GetParameter(1);
	sigma = f.GetParameter(2);
}

/**
 * [calibrateSigmas description]
 * @param t1              [description]
 * @param t2              [description]
 * @param originals       [description]
 * @param reconstructions [description]
 * @param particletype    [description]
 * @param prefix          [description]
 */
void calibrateSigmas(TTree& t1, TTree& t2, ParticleEvent& originals, TrackRecons& reconstructions, string const &particletype, string const &prefix, int &startevent){

	int nentries = t2.GetEntries();
	startevent = nentries;

	auto expected_theta = 0.;
	auto expected_area  = 0.;
	auto actual_theta   = 0.;
	auto actual_area    = 0.;
	auto sigma_theta    = 0.01;
	auto sigma_area     = 0.;

	std::string theta_expected_actual_sigma_file = "theta_expected_actual_sigma.pdf";
	std::string theta_expected_actual_file = "theta_expected_actual.pdf";
	std::string theta_expected_file = "theta_expected.pdf";
	std::string area_expected_actual_sigma_file = "area_expected_actual_sigma.pdf";
	std::string area_expected_actual_file = "area_expected_actual.pdf";
	std::string area_expected_file = "area_expected.pdf";

	vector < std::string > filenames;

	filenames.push_back( wul::appendStrings(prefix, theta_expected_actual_sigma_file) );
	filenames.push_back( wul::appendStrings(prefix, theta_expected_actual_file) );
	filenames.push_back( wul::appendStrings(prefix, theta_expected_file) );
	filenames.push_back( wul::appendStrings(prefix, area_expected_actual_sigma_file) );
	filenames.push_back( wul::appendStrings(prefix, area_expected_actual_file) );
	filenames.push_back( wul::appendStrings(prefix, area_expected_file) );


	vector< double > vexpected_actual_sigma_theta;
	vector< double > vexpected_actual_theta;
	vector< double > vexpected_theta;

	vector< double > vexpected_actual_sigma_area;
	vector< double > vexpected_actual_area;
	vector< double > vexpected_area;

	for (unsigned int ev = 0; ev < t2.GetEntries(); ++ev){
		t1.GetEntry(ev); t2.GetEntry(ev);

		auto &pars   = originals.Particles;
		auto &recons = reconstructions.Recon;

		for (unsigned int p = 0; p < pars.size(); ++p){
			auto& par   = pars.at(p);
			auto& recon = recons.at(p);

			auto emission_angles = par.EmissionAngleMap();
			auto expected_theta  = emission_angles[par.name];

			bool cont = false;
			for (unsigned int i = 0; i < recon.Options.size(); ++i){
				string &name = recon.Options.at(i);
				if (name == par.name){
					expected_area  = recon.ExpectedNumber.at(i);
					sigma_area = sqrt(expected_area);
					actual_area    = recon.Areas.at(i);
					actual_theta   = recon.Params.at(i).at(1);
					cont = false;
					break;
				}
				else cont = true;
			}
			if (cont) continue;

			fillvectors(expected_theta, actual_theta, sigma_theta, vexpected_actual_sigma_theta, vexpected_actual_theta, vexpected_theta);

			fillvectors(expected_area, actual_area, sigma_area, vexpected_actual_sigma_area, vexpected_actual_area, vexpected_area);
		}
	}

	TCanvas C;
	// cout << "first value = " << vexpected_actual_sigma_theta.at(0) << endl;

	std::vector<std::vector<double> * > histvalues;

	histvalues.push_back(&(vexpected_actual_sigma_theta));
	histvalues.push_back(&(vexpected_actual_theta));
	histvalues.push_back(&(vexpected_theta));
	histvalues.push_back(&(vexpected_actual_sigma_area));
	histvalues.push_back(&(vexpected_actual_area));
	histvalues.push_back(&(vexpected_area));


	std::map< int, string > histtype;
	histtype[1] = "emission_angle";
	histtype[4] = "expected_photons";

	double new_theta_expected = 0.;
	double new_expectedphoton_expected = 0.;

	double new_theta_sigma = 0.;
	double new_expectedphoton_sigma = 0.;

	std:: vector< TH1D > hists;
	for (map<int, string>::iterator i = histtype.begin(); i != histtype.end(); ++i){
		const int &indx = i->first;
		const std::string &type = i->second;
		hists.push_back(makehist(*histvalues.at(indx), filenames.at(indx), 100));
		TH1D &hist = hists.back();
		// normalizehist(hist, 100);
		if (type == "emission_angle"){
			fitBeta(hist, *histvalues.at(indx), new_theta_expected, new_theta_sigma);
		}
		if (type == "expected_photons"){
			fitGaussian(hist, *histvalues.at(indx), new_expectedphoton_expected, new_expectedphoton_sigma);
		}
		hist.Draw();
		C.Print(filenames.at(indx).c_str());
		C.Clear();
	}

	for (unsigned int ev = 0; ev < nentries; ++ev){
		t1.GetEntry(ev); t2.GetEntry(ev);
		cout << "ev = " << ev << endl;
		auto& pars   = originals.Particles;
		auto& recons = reconstructions.Recon;

		for (unsigned int p = 0; p < pars.size(); ++p){
			cout << "p = " << p << endl;
			Particle& par   = pars.at(p);
			TrackRecon& recon = recons.at(p);

			auto emission_angles = par.EmissionAngleMap();
			auto expected_theta  = emission_angles[par.name];

			bool cont = false;
			for (unsigned int i = 0; i < recon.Options.size(); ++i){
				string &name = recon.Options.at(i);
				if (name == particletype){
					// you want to define the new nSigma_theta and nSigma_Expected
					// expected photons
					cout << i << " "<< particletype << " oldsigma = " << recon.Sigmas.at(i) << endl;
					cout << "recon.delSigArea.at(i) = " << recon.delSigArea.at(i) << endl;
					cout << "recon.delSigTheta.at(i) = " << recon.delSigTheta.at(i) << endl;
					expected_area = recon.ExpectedNumber.at(i);
					actual_area    = recon.Areas.at(i);
					recon.delSigArea.at(i) = (expected_area - actual_area)/new_expectedphoton_sigma;
					// expected emission angle
					actual_theta   = recon.Params.at(i).at(1);
					recon.delSigTheta.at(i) = (expected_theta - actual_theta)/new_theta_sigma;
					recon.Sigmas.at(i) = sqrt(pow(recon.delSigTheta.at(i),2)+pow(recon.delSigArea.at(i),2));
					cout << "newsigma = " << recon.Sigmas.at(i) << endl;
					cout << "recon.delSigArea.at(i) = " << recon.delSigArea.at(i) << endl;
					cout << "recon.delSigTheta.at(i) = " << recon.delSigTheta.at(i) << "\n\n";
					cont = false;
					break;
				}
				else cont = true;
			}
			if (cont) continue;
		}
		t1.Fill(); t2.Fill();
	}

}