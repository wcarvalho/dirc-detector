#include "FileProperties.h"
#include "TError.h"








































/**
 * [fillvectors description]
 * @param expected               [description]
 * @param actual                 [description]
 * @param sigma                  [description]
 * @param n_distribution 	  		 [description]
 * @param n_sigma_distribution   [description]
 * @param expected_distribution  [description]
 */
void fillvectors(double const& expected, double const& actual, double const& del_sigma, vector<double >& n_distribution, vector<double >& n_sigma_distribution, vector<double >& expected_distribution){

	double dif = (expected-actual);
	n_distribution.push_back(del_sigma);
	n_sigma_distribution.push_back(dif);
	expected_distribution.push_back(expected);

}
/**
 * [makehist description]
 * @param C        [description]
 * @param x        [description]
 * @param filename [description]
 */
TH1D makehist(vector<double> const& x, string filename, int nbins, std::string option = ""){

	wul::FileProperties fp(filename);
	string &name = fp.filename;
	double xlow = 0.; double xhi = 0.;
	wul::vectorminmax(x, xlow, xhi);

	if (option == "theta"){
		xlow = -.2;
		xhi = .2;
	}

	TH1D h(name.c_str(), name.c_str(), nbins, xlow, xhi);
	for (unsigned int i = 0; i < x.size(); ++i)
		h.Fill(x.at(i));

	return std::move(h);

}


void meanBinProperties(TH1D &h, int &meanbin, double &binheight, double &bincenter){

	double mean = h.GetMean();
	meanbin = h.FindBin(mean);
	binheight = h.GetBinContent(meanbin);
	bincenter = h.GetBinCenter(meanbin);
}


void maxBinProperties(TH1D &h, int &tallestbin, double &binheight, double &bincenter){

	tallestbin = h.GetMaximumBin();
	binheight = h.GetBinContent(tallestbin);
	bincenter = h.GetBinCenter(tallestbin);
}

void fitBeta(TH1D &h, vector<double> const& x, double &mean, double &sigma){

	auto xlow = 0., xhi = 0.;
	wul::vectorminmax(x, xlow, xhi);

	auto tallestbinheight = 0., tallestbincenter = 0.;
	int tallestbin = 0;
	meanBinProperties(h, tallestbin, tallestbinheight, tallestbincenter);

	// double new_low = tallestbincenter - sqrt(tallestbincenter)*7;
	// double new_hi = tallestbincenter + sqrt(tallestbincenter)*7;
	// h.SetAxisRange(new_low, new_hi);
	// h.SetBins(1000, new_low, new_hi);


	double par1 = 0., par2 = 0., par3 = 0.;
	if (h.GetBinContent(tallestbin-2)>h.GetBinContent(tallestbin+2)){
		par1 = 1;
		par2 = 80;
		par3 = 1;
	}
	else{
		par1 = 0;
		par2 = 1;
		par3 = 80;
	}


	std::stringstream ss; ss.str("");
	ss << "( [0]*TMath::BetaDist( (x + [1] ),[2],[3]) )";
	string func = ss.str();

	TF1 f("beta_distribution",func.c_str(), -2, 2);
	f.SetParameters(10, par1, par2, par3);
	h.Fit(&f, "QBIRC");

	const double &alpha = f.GetParameter(1);
	const double &beta = f.GetParameter(2);

	mean = alpha/(alpha+beta);
	sigma = 2*pow(alpha, alpha)*pow(beta,beta)/(TMath::Beta(alpha, beta)*pow( (alpha+beta), alpha+beta+1));

	/////////////// sigma from standard deviation
	// double variance = (alpha*beta)/( (alpha+beta)*(alpha+beta)*(alpha+beta+1) );
	// sigma = sqrt(variance);
	// sigma from absolute deviation about mean
	/////////////// printing
	cout << "alpha = " << alpha << endl;
	cout << "beta = " << beta << endl;
	cout << "pow(alpha, alpha) = " << pow(alpha, alpha) << endl;
	cout << "pow(beta,beta) = " << pow(beta,beta) << endl;
	cout << "TMath::Beta(alpha, beta) = " << TMath::Beta(alpha, beta) << endl;
	cout << "pow( (alpha+beta), alpha+beta+1) = " << pow( (alpha+beta), alpha+beta+1) << endl;
}

void fitGaussian(TH1D &h, vector<double> const& x, double& mean,double &sigma){

	auto xlow = 0., xhi = 0., sigma_guess = 0.;
	// wul::vectorminmax(x, xlow, xhi);

	auto meanbinheight = 0., meanbincenter = 0.;
	int meanbin;
	meanBinProperties(h, meanbin, meanbinheight, meanbincenter);

	xlow = double(h.GetXaxis()->GetXmin());
	xhi = double(h.GetXaxis()->GetXmax());
	sigma_guess = sqrt(meanbinheight);

	TF1 f("gauss", "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) )", xlow, xhi);
	f.SetParameter(0, meanbinheight);
	f.SetParameter(1, meanbincenter);
	f.SetParameter(2, sigma_guess);

	h.Fit(&f, "QBIRC");

	mean = f.GetParameter(1);
	sigma = f.GetParameter(2);
}

void generateDistributions(TTree &t1, TTree &t2, ParticleEvent &originals, TrackRecons& reconstructions, std::vector<std::vector<double> * > &distributions, int start, int end){

	auto expected_theta = 0., expected_area = 0., actual_theta = 0., actual_area = 0., nsigma_theta = 0.01, nsigma_area = 0.;

	for (unsigned ev = start; ev < end; ++ev){
		t1.GetEntry(ev); t2.GetEntry(ev);

		auto &pars   = originals.Particles;
		auto &recons = reconstructions.Recon;

		if (recons.size() == 0) continue;

		for (unsigned int p = 0; p < pars.size(); ++p){
			auto& par   = pars.at(p);
			auto& recon = recons.at(p);

			auto emission_angles = par.EmissionAngleMap();
			auto expected_theta  = emission_angles[par.name];
			bool cont = false;
			for (unsigned int i = 0; i < recon.Options.size(); ++i){
				string &name = recon.Options.at(i);
				if (name == par.name){
					expected_area = recon.ExpectedNumber.at(i);
					actual_area   = recon.Areas.at(i);
					actual_theta  = recon.Params.at(i).at(1);
					nsigma_area   = recon.delSigArea.at(i);
					nsigma_theta  = recon.delSigTheta.at(i);
					cont = false;
					break;
				}
				else cont = true;
			}
			if (cont) continue;

			fillvectors(expected_theta, actual_theta, nsigma_theta, *distributions.at(0), *distributions.at(1), *distributions.at(2));

			fillvectors(expected_area, actual_area, nsigma_area, *distributions.at(3), *distributions.at(4), *distributions.at(5));
		}
	}
}

void printDistributionFits(TCanvas &C, std::vector<std::vector<double> * > &distributions, vector < std::string > const& filenames, double& theta_fit_difference, double& expected_photon_fit_difference, double& theta_fit_sigma, double& expected_photon_fit_sigma, int nbins, string suffix){

	std::map< int, string > histtype;
	histtype[0] = "emission_angle";
	histtype[3] = "expected_photons";

	int nbins0 = nbins;

	for (map<int, string>::iterator i = histtype.begin(); i != histtype.end(); ++i){
		const int &indx = i->first;
		const std::string &type = i->second;
		string fname = filenames.at(indx);
		fname.append(suffix);

		std::string option = "";
		if (type == "emission_angle"){
			option = "theta";
		}
		else option = "";

		TH1D hist = makehist(*distributions.at(indx), fname, nbins, option);
		if (type == "emission_angle"){
			fitGaussian(hist, *distributions.at(indx), theta_fit_difference, theta_fit_sigma);
		}
		if (type == "expected_photons"){
			fitGaussian(hist, *distributions.at(indx), expected_photon_fit_difference, expected_photon_fit_sigma);
		}

		hist.Draw();
		C.Print(fname.append(".pdf").c_str());
		C.Clear();
	}

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

	std::string theta_n_distribution_file = "theta_n_distribution";
	std::string theta_n_sigma_distribution_file = "theta_n_sigma_distribution";
	std::string theta_expected_file = "theta_expected";
	std::string area_n_distribution_file = "area_n_distribution";
	std::string area_n_sigma_distribution_file = "area_n_sigma_distribution";
	std::string area_expected_file = "area_expected";

	vector < std::string > filenames;
	filenames.push_back( wul::appendStrings(prefix, theta_n_distribution_file) );
	filenames.push_back( wul::appendStrings(prefix, theta_n_sigma_distribution_file) );
	filenames.push_back( wul::appendStrings(prefix, theta_expected_file) );
	filenames.push_back( wul::appendStrings(prefix, area_n_distribution_file) );
	filenames.push_back( wul::appendStrings(prefix, area_n_sigma_distribution_file) );
	filenames.push_back( wul::appendStrings(prefix, area_expected_file) );


	vector< double > n_distribution_theta;
	vector< double > n_sigma_distribution_theta;
	vector< double > expected_distribution_theta;
	vector< double > n_distribution_area;
	vector< double > n_sigma_distribution_area;
	vector< double > expected_distribution_area;

	std::vector<std::vector<double> * > distributions;
	distributions.push_back(&(n_distribution_theta));
	distributions.push_back(&(n_sigma_distribution_theta));
	distributions.push_back(&(expected_distribution_theta));
	distributions.push_back(&(n_distribution_area));
	distributions.push_back(&(n_sigma_distribution_area));
	distributions.push_back(&(expected_distribution_area));


	////////////////////////
	// Begin Calibrating //
	////////////////////////
	TCanvas C;

	generateDistributions(t1, t2, originals, reconstructions, distributions, 0, nentries);

	double theta_fit_difference           = 0.;
	double expected_photon_fit_difference = 0.;
	double theta_fit_sigma                = 0.;
	double expected_photon_fit_sigma      = 0.;

	printDistributionFits(C, distributions, filenames, theta_fit_difference, expected_photon_fit_difference, theta_fit_sigma, expected_photon_fit_sigma, 200, "_prefit");

	int matches = 0;
	int atpar = 0;
	for (unsigned int ev = 0; ev < nentries; ++ev){
		t1.GetEntry(ev); t2.GetEntry(ev);
		auto& pars   = originals.Particles;
		auto& recons = reconstructions.Recon;

		if (recons.size() == 0) continue;

		for (unsigned int p = 0; p < pars.size(); ++p){
			++atpar;
			Particle& par   = pars.at(p);
			TrackRecon& recon = recons.at(p);

			auto emission_angles = par.EmissionAngleMap();
			auto expected_theta  = emission_angles[particletype];
			bool cont = false;
			for (unsigned int i = 0; i < recon.Options.size(); ++i){
				string &name = recon.Options.at(i);
				if (name == particletype){
					// you want to define the new nSigma_theta and nSigma_Expected

					bool zeroarea = (recon.Areas.at(i) == 0);
					if (zeroarea) continue;
					expected_area = recon.ExpectedNumber.at(i);
					actual_area = recon.Areas.at(i);
					actual_theta = recon.Params.at(i).at(1);

					double original_nsig = recon.Sigmas.at(i);
					double original_nsigArea = recon.delSigArea.at(i);
					double original_nsigTheta = recon.delSigTheta.at(i);

					// expected photons
					double old_expectedphoton_difference = (expected_area - actual_area);

					double new_expected_area = (original_nsigArea - expected_photon_fit_difference)*sqrt(original_nsigArea)+actual_area;
					double new_nsigArea = (new_expected_area - actual_area)/sqrt(original_nsigArea);

					// recon.delSigArea.at(i) = new_nsigArea;

					// recon.delSigArea.at(i) -= expected_photon_fit_difference;

					// expected emission angle
					double old_theta_difference = (expected_theta - actual_theta);

					// recon.delSigTheta.at(i) -= theta_fit_difference;
					// (theta_fit_difference - old_theta_difference)/theta_fit_sigma;
					// recon.delSigTheta.at(i) = recon.delSigTheta.at(i);
					recon.Sigmas.at(i) = sqrt(pow(recon.delSigTheta.at(i),2)+pow(recon.delSigArea.at(i),2));

					bool print = true;
					if ( print && ((recon.delSigTheta.at(i) < 1.5) && (recon.delSigArea.at(i) < 1.5))){
						++matches;
						cout << "particle: " << atpar << ", match: " << matches << " particle: "<< par.name << ", type: " << particletype << endl;
						cout << "eta, pt = " << par.Eta<<"," << par.pt << endl;


						cout << "Expected vs. Actual\n";
						cout << "\texpected_area = " << expected_area << endl;
						cout << "\tactual_area = " << actual_area << endl;
						cout << "\tnew_expected_area = " << new_expected_area << endl;
						cout << "\texpected_theta = " << expected_theta << endl;
						cout << "\tactual_theta = " << actual_theta << endl;


						cout << "Sigma and Difference\n";
						cout << "\texpected_photon_fit_sigma = " << expected_photon_fit_sigma << endl;
						cout << "\ttheta_fit_sigma = " << theta_fit_sigma << endl;
						cout << "\texpected_photon_fit_difference = " << expected_photon_fit_difference << endl;
						cout << "\ttheta_fit_difference = " << theta_fit_difference << endl;

						cout << "sigma = " << original_nsig << " - > "  << recon.Sigmas.at(i) << endl;
						cout << "\trecon.delSigArea.at(i) = " << original_nsigArea << " - > " << recon.delSigArea.at(i) << endl;
						cout << "\trecon.delSigTheta.at(i) = " << original_nsigTheta << " -> " << recon.delSigTheta.at(i) << "\n\n";
					}
				cont = false;
				break;
				}
				else cont = true;
			}
			if (cont) continue;
		}
		t1.Fill(); t2.Fill();
	}

	for (unsigned i = 0; i < distributions.size(); ++i)
		distributions.at(i)->clear();
	generateDistributions(t1, t2, originals, reconstructions, distributions, nentries, 2*nentries-1);

	printDistributionFits(C, distributions, filenames, theta_fit_difference, expected_photon_fit_difference, theta_fit_sigma, expected_photon_fit_sigma, 200, "_postfit");

}