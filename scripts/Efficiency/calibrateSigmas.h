#include "FileProperties.h"

void fillvectors(double const& expected, double const& actual, double const& sigma, vector<double >& vexpected_actual_sigma, vector<double >& vexpected_actual, vector<double >& vexpected){

	double dif = (expected-actual);
	double dif_sig = dif/sigma;
	// cout << "expected = " << expected << endl;
	// cout << "actual = " << actual << endl;
	// cout << "sigma = " << sigma << endl;
	// cout << "dif = " << dif << endl;
	// cout << "dif_sig = " << dif_sig << endl;
	vexpected_actual_sigma.push_back(dif_sig);
	vexpected_actual.push_back(dif);
	vexpected.push_back(expected);

}

void makehist(TCanvas &C, vector<double> const& x, string filename){

	wul::FileProperties fp(filename);
	string &name = fp.filename;
	double xlow = 0.; double xhi = 0.;
	wul::vectorminmax(x, xlow, xhi);

	//FIXME: don't hardcode nbins. read in from code?
	int nbins = 100;

	TH1D h(name.c_str(), name.c_str(), nbins, xlow, xhi);
	for (unsigned int i = 0; i < x.size(); ++i)
		h.Fill(x.at(i));

	h.Draw();
	C.Print(filename.c_str());
	C.Clear();

}

void calibrateSigmas(TTree &t1, TTree &t2, ParticleEvent &originals, TrackRecons &reconstructions, string const &particletype, string const &prefix){

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
			auto par   = pars.at(p);
			auto recon = recons.at(p);

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

	// auto temp = *histvalues.at(0);
	// cout << "first value = " << temp.at(0) << endl;
	for(unsigned int i = 0; i < histvalues.size(); ++i){
		makehist(C, *histvalues.at(i), filenames.at(i));
	}
		// exit(1);

}