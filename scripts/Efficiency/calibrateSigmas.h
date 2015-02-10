#include "FileProperties.h"
#include "TError.h"
#include "graphing.h"

template < typename eventParser, typename eventParserCondition,
					typename trialParser, typename trialParserCondition >
void parseEvents(TTree &t1, TTree &t2,
 								ParticleEvent &originals, TrackRecons& reconstructions,
 								int start, int end,
 								eventParser P1, eventParserCondition C1,
 								trialParser P2, trialParserCondition C2){

	for (unsigned ev = start; ev < end; ++ev){
		t1.GetEntry(ev); t2.GetEntry(ev);

		auto &pars   = originals.Particles;
		auto &recons = reconstructions.Recon;

		if (recons.size() == 0) continue;
		for (unsigned int p = 0; p < pars.size(); ++p){
			Particle& par   = pars.at(p);
			TrackRecon& recon = recons.at(p);
			if (!(C2(recon, par))) continue;
			P2(recon, par);
		}
		if (!(C1(t1, t2, recons, pars))) continue;
		P1(t1, t2, recons, pars);
	}
}

void calibrateSigmas(TTree &t1, TTree &t2, ParticleEvent &originals, TrackRecons& reconstructions, const std::string& particletype, double percent, int nbins, string const &prefix, int &startevent){

	double pBounds[2];
	pBounds[0] = 0;
	pBounds[1] = .5;

	auto parseConditon = [&pBounds](TrackRecon& r, Particle& p){
		return !( (p.pt < pBounds[0]) || (p.pt > pBounds[1]) );
	};

	std::map < std::string, std::vector< std::vector<double> > > datamap;
	auto generateDistribution = [&datamap, &pBounds](TrackRecon& recon, Particle& par){

		auto expected_theta = par.EmissionAngleMap()[par.name];
		for (unsigned int i = 0; i < recon.Options.size(); ++i){
			std::string name = recon.Options.at(i);
			if (name == par.name){
				double expected_area = recon.ExpectedNumber.at(i);
				double actual_area   = recon.Areas.at(i);
				double actual_theta  = recon.Params.at(i).at(1);
				datamap["photons"].back().push_back(expected_area);
				datamap["theta"].back().push_back(expected_theta);
			}
		}
	};

	auto histname = [&pBounds](std::string& name){
		stringstream ss; ss.str("");
		ss << "_pt_" << pBounds[0] << "_" << pBounds[1];
		return std::move(wul::appendStrings(name, ss.str()));
	};

	auto calibrate = [&sigmas, particletype](TrackRecon& recon, Particle& par){

		auto expected_theta = par.EmissionAngleMap()[particletype];
		for (unsigned int i = 0; i < recon.Options.size(); ++i){
			std::string &name = recon.Options.at(i);
			if (name == particletype){
				double& expected_area   = recon.ExpectedNumber.at(i);
				double& actual_area     = recon.Areas.at(i);
				double& actual_theta    = recon.Params.at(i).at(1);
				recon.delSigArea.at(i)  = (expected_area - actual_area)/photons_sigma;
				recon.delSigTheta.at(i) = (expected_theta - actual_theta)/theta_sigma;
			}
		}
	};
	auto fill = [](TTree &t1, TTree &t2, decltype(reconstructions.Recon)& recons, decltype(originals.Particles)& pars){
		t1.Fill();
		t2.Fill();
	};

	auto doNothing_trial   = [] (TrackRecon& r, Particle& p){ };
	auto returnfalse_trial = [] (TrackRecon& r, Particle& p) { return false; };

	auto doNothing_event   = [] (TTree &t1, TTree &t2, decltype(reconstructions.Recon)& recons, decltype(originals.Particles)& par){ };
	auto returnfalse_event = [] (TTree &t1, TTree &t2, decltype(reconstructions.Recon)& recons, decltype(originals.Particles)& pars) { return false; };


	////////////////////////////
	// Analyze && Callibrate //
	////////////////////////////
	int nentries = t1.GetEntries();
	vector< double > new_list;
	while (pBounds[1] <= 3.){

		auto& photons_data = datamap["photons"];
		auto& theta_data = datamap["theta"];
		photons_data.push_back(new_list);
		theta_data.push_back(new_list);

		// generate distributions
		parseEvents(t1, t2,
			originals, reconstructions,
			0, nentries,
			doNothing_event, returnfalse_event,
			generateDistribution, parseConditon);

		for(auto i = datamap.begin(); i != datamap.end(); ++i){
			string key = i->first;
			double* latest_data = &i->second.back()[0];
			std::string name = histname(key);
			static double min = 0., max = 0.;
		  wul::vectorminmax(x, min, max);
			dirc::dircTH1D h(name.c_str(), name.c_str(), nbins, min, max);
			h.defineDistributionRange(min, max);
			h.defineSigma(percent);
			sigmas[key].push_back(h.distributionSigma);
		}

		pBounds[i] += .5;
	}
	// FIXME. TTree.GetEntry(i) resets the values so calibration and filling have to be done in one shot. I'll have to adjust the lambda for calibrate to include the sigmas map. That map will have the sigmas for different pt distributions. something like a map from a range to a sigma. plug in the pt, will spit out the sigma. this can probably be done with another lambda. just another condition. that lambda will need to read know the the overall bounds for p i.e. 0,3, and the step sizes of the groups i.e. .5 (those are the values for now)

	// calibrate && fill events
	parseEvents(t1, t2, originals, reconstructions, 0, nentries,
		fill, [](TTree &t1, TTree &t2, decltype(reconstructions.Recon)& recons, decltype(originals.Particles)&) { return true; },
		calibrate, parseConditon);
	// for (unsigned int i = 0; i < 2; ++i)

}

// void calibrateSigmas(TTree& t1, TTree& t2, ParticleEvent& originals, TrackRecons& reconstructions, string const &particletype, string const &prefix, int &startevent){
// }
// 	int nentries = t2.GetEntries();
// 	startevent = nentries;

// 	// std::map<std::string, std::vector< std::vector<double> > > datamap;
// 	// map < string, vector < double > >& sigmas;
// 	// void analyzeDistributions(t1, t2, originals, reconstructions, distributions, 0, nentries, datamap, sigmas);

// 	// for (auto sigmas::iterator i = sigmas.begin(&); i < sigmas.end(); ++i){

// 	// }
// }




// void printDistributionFits(TCanvas &C, std::vector<std::vector<double> * > &distributions, vector < std::string > const& filenames, double& theta_fit_difference, double& expected_photon_fit_difference, double& theta_fit_sigma, double& expected_photon_fit_sigma, int nbins, string suffix){

// 	std::map< int, string > histtype;
// 	histtype[0] = "emission_angle";
// 	histtype[3] = "expected_photons";

// 	int nbins0 = nbins;

// 	for (map<int, string>::iterator i = histtype.begin(); i != histtype.end(); ++i){
// 		const int &indx = i->first;
// 		const std::string &type = i->second;
// 		string fname = filenames.at(indx);
// 		fname.append(suffix);

// 		std::string option = "";
// 		if (type == "emission_angle"){
// 			option = "theta";
// 		}
// 		else option = "";

// 		TH1D hist = makehist(*distributions.at(indx), fname, nbins, option);
// 		if (type == "emission_angle"){
// 			fitGaussian(hist, *distributions.at(indx), theta_fit_difference, theta_fit_sigma);
// 		}
// 		if (type == "expected_photons"){
// 			fitGaussian(hist, *distributions.at(indx), expected_photon_fit_difference, expected_photon_fit_sigma);
// 		}

// 		hist.Draw();
// 		C.Print(fname.append(".pdf").c_str());
// 		C.Clear();
// 	}

// }

	/**
 * [calibrateSigmas description]
 * @param t1              [description]
 * @param t2              [description]
 * @param originals       [description]
 * @param reconstructions [description]
 * @param particletype    [description]
 * @param prefix          [description]
 */
// void calibrateSigmas(TTree& t1, TTree& t2, ParticleEvent& originals, TrackRecons& reconstructions, string const &particletype, string const &prefix, int &startevent){

// 	int nentries = t2.GetEntries();
// 	startevent = nentries;

// 	auto expected_theta = 0.;
// 	auto expected_area  = 0.;
// 	auto actual_theta   = 0.;
// 	auto actual_area    = 0.;
// 	auto sigma_theta    = 0.01;
// 	auto sigma_area     = 0.;

// 	std::string theta_n_distribution_file = "theta_n_distribution";
// 	std::string theta_n_sigma_distribution_file = "theta_n_sigma_distribution";
// 	std::string theta_expected_file = "theta_expected";
// 	std::string area_n_distribution_file = "area_n_distribution";
// 	std::string area_n_sigma_distribution_file = "area_n_sigma_distribution";
// 	std::string area_expected_file = "area_expected";

// 	vector < std::string > filenames;
// 	filenames.push_back( wul::appendStrings(prefix, theta_n_distribution_file) );
// 	filenames.push_back( wul::appendStrings(prefix, theta_n_sigma_distribution_file) );
// 	filenames.push_back( wul::appendStrings(prefix, theta_expected_file) );
// 	filenames.push_back( wul::appendStrings(prefix, area_n_distribution_file) );
// 	filenames.push_back( wul::appendStrings(prefix, area_n_sigma_distribution_file) );
// 	filenames.push_back( wul::appendStrings(prefix, area_expected_file) );


// 	vector< double > n_distribution_theta;
// 	vector< double > n_sigma_distribution_theta;
// 	vector< double > expected_distribution_theta;
// 	vector< double > n_distribution_area;
// 	vector< double > n_sigma_distribution_area;
// 	vector< double > expected_distribution_area;

// 	std::vector<std::vector<double> * > distributions;
// 	distributions.push_back(&(n_distribution_theta));
// 	distributions.push_back(&(n_sigma_distribution_theta));
// 	distributions.push_back(&(expected_distribution_theta));
// 	distributions.push_back(&(n_distribution_area));
// 	distributions.push_back(&(n_sigma_distribution_area));
// 	distributions.push_back(&(expected_distribution_area));


// 	////////////////////////
// 	// Begin Calibrating //
// 	////////////////////////
// 	TCanvas C;

// 	generateDistributions(t1, t2, originals, reconstructions, distributions, 0, nentries);

// 	double theta_fit_difference           = 0.;
// 	double expected_photon_fit_difference = 0.;
// 	double theta_fit_sigma                = 0.;
// 	double expected_photon_fit_sigma      = 0.;

// 	printDistributionFits(C, distributions, filenames, theta_fit_difference, expected_photon_fit_difference, theta_fit_sigma, expected_photon_fit_sigma, 200, "_prefit");

// 	int matches = 0;
// 	int atpar = 0;
// 	for (unsigned int ev = 0; ev < nentries; ++ev){
// 		t1.GetEntry(ev); t2.GetEntry(ev);
// 		auto& pars   = originals.Particles;
// 		auto& recons = reconstructions.Recon;

// 		if (recons.size() == 0) continue;

// 		for (unsigned int p = 0; p < pars.size(); ++p){
// 			++atpar;
// 			Particle& par   = pars.at(p);
// 			TrackRecon& recon = recons.at(p);

// 			auto emission_angles = par.EmissionAngleMap();
// 			auto expected_theta  = emission_angles[particletype];
// 			bool cont = false;
// 			for (unsigned int i = 0; i < recon.Options.size(); ++i){
// 				string &name = recon.Options.at(i);
// 				if (name == particletype){
// 					// you want to define the new nSigma_theta and nSigma_Expected

// 					bool zeroarea = (recon.Areas.at(i) == 0);
// 					if (zeroarea) continue;
// 					expected_area = recon.ExpectedNumber.at(i);
// 					actual_area = recon.Areas.at(i);
// 					actual_theta = recon.Params.at(i).at(1);

// 					double original_nsig = recon.Sigmas.at(i);
// 					double original_nsigArea = recon.delSigArea.at(i);
// 					double original_nsigTheta = recon.delSigTheta.at(i);

// 					// expected photons
// 					double old_expectedphoton_difference = (expected_area - actual_area);

// 					double new_expected_area = (original_nsigArea - expected_photon_fit_difference)*sqrt(original_nsigArea)+actual_area;
// 					double new_nsigArea = (new_expected_area - actual_area)/sqrt(original_nsigArea);

// 					// recon.delSigArea.at(i) = new_nsigArea;

// 					// recon.delSigArea.at(i) -= expected_photon_fit_difference;

// 					// expected emission angle
// 					double old_theta_difference = (expected_theta - actual_theta);

// 					// recon.delSigTheta.at(i) -= theta_fit_difference;
// 					// (theta_fit_difference - old_theta_difference)/theta_fit_sigma;
// 					// recon.delSigTheta.at(i) = recon.delSigTheta.at(i);
// 					recon.Sigmas.at(i) = sqrt(pow(recon.delSigTheta.at(i),2)+pow(recon.delSigArea.at(i),2));

// 					bool print = true;
// 					if ( print && ((recon.delSigTheta.at(i) < 1.5) && (recon.delSigArea.at(i) < 1.5))){
// 						++matches;
// 						cout << "particle: " << atpar << ", match: " << matches << " particle: "<< par.name << ", type: " << particletype << endl;
// 						cout << "eta, pt = " << par.Eta<<"," << par.pt << endl;


// 						cout << "Expected vs. Actual\n";
// 						cout << "\texpected_area = " << expected_area << endl;
// 						cout << "\tactual_area = " << actual_area << endl;
// 						cout << "\tnew_expected_area = " << new_expected_area << endl;
// 						cout << "\texpected_theta = " << expected_theta << endl;
// 						cout << "\tactual_theta = " << actual_theta << endl;


// 						cout << "Sigma and Difference\n";
// 						cout << "\texpected_photon_fit_sigma = " << expected_photon_fit_sigma << endl;
// 						cout << "\ttheta_fit_sigma = " << theta_fit_sigma << endl;
// 						cout << "\texpected_photon_fit_difference = " << expected_photon_fit_difference << endl;
// 						cout << "\ttheta_fit_difference = " << theta_fit_difference << endl;

// 						cout << "sigma = " << original_nsig << " - > "  << recon.Sigmas.at(i) << endl;
// 						cout << "\trecon.delSigArea.at(i) = " << original_nsigArea << " - > " << recon.delSigArea.at(i) << endl;
// 						cout << "\trecon.delSigTheta.at(i) = " << original_nsigTheta << " -> " << recon.delSigTheta.at(i) << "\n\n";
// 					}
// 				cont = false;
// 				break;
// 				}
// 				else cont = true;
// 			}
// 			if (cont) continue;
// 		}
// 		t1.Fill(); t2.Fill();
// 	}

// 	for (unsigned i = 0; i < distributions.size(); ++i)
// 		distributions.at(i)->clear();
// 	generateDistributions(t1, t2, originals, reconstructions, distributions, nentries, 2*nentries-1);

// 	printDistributionFits(C, distributions, filenames, theta_fit_difference, expected_photon_fit_difference, theta_fit_sigma, expected_photon_fit_sigma, 200, "_postfit");

// }