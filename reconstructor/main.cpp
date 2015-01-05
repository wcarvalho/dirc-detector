//====================================================
//
//                RECONSTRUCTOR
//
//====================================================

#include "../headers/reconstructor.h"
#include <cstdlib>
#include "cmdline.h"

int main(int argc, char** argv)
{
	gengetopt_args_info ai;  
	if (cmdline_parser (argc, argv, &ai) != 0){ exit(1); }

  double pi = TMath::Pi();

  int last = 0;

  int xbins = 1000;
	int ybins = 1000;

	double smear = .01;

 	int ExpectedPhotonCase = ai.expected_photons_case_arg;

 	// i/o setup
	string graphprefix = "";
  string rf_default = ai.input_arg;
  string wf_default = "fitresults.root"; 
	string rf = rf_default;
  string wf = wf_default;

  wul::FileProperties readf_prop(rf);
  string directory = readf_prop.directory;

  // parse ai arguments
	bool print = ai.verbose_given;
  bool quiet = ai.quiet_given; if (quiet) print = !quiet;
	bool make = ai.make_given;
  if (ai.last_given) last = ai.last_arg;
  if (ai.write_file_given) wf = ai.write_file_arg;
	if (ai.Directory_given) directory = ai.Directory_arg; readf_prop.appendFileToDirectory(directory, wf);
	if (ai.Smear_given) smear = ai.Smear_arg;
	if (ai.graph_prefix_given) graphprefix = ai.graph_prefix_arg;

	
	// Function Pointer(s)
	double (*ExpectedNumberofPhotons)(double const&, double const&, double const&, double const&, double const&);


	// determine which function will be used to determine the expected number of photons
	switch(ExpectedPhotonCase) {
		case 1: // look-up table
			cout << "LookUpTable\n";
			ExpectedNumberofPhotons = &LookUpTableWrapper;
		break;
		case 2: // riemansum
			cout << "RiemannSum\n";
			ExpectedNumberofPhotons = &RiemannSum;
		break;
	}


  Reconstruction reconstruction;
	ReconstructionData data;
	reconstruction.Track.push_back(data);
	Analysis A;
	TrackRecons Tracks; Tracks.Recon.clear();		// stores information on particle guesses

  // pointers to data from ROOT File
  GeneratorOut *event_output = 0;
  Detector* d = 0;

  TFile file(rf.c_str(), "read");
	TTree *events = (TTree*)file.Get("sim_out");
	events->SetBranchAddress("simEvent", &event_output);
	events->SetBranchAddress("detector", &d);
	

	TFile file2(wf.c_str(), "recreate");
  TTree tree_np("identifications", "information on what particles tracks were reconstructed into, and with what probability");
	TTree* tree = &tree_np;
	tree->Branch("guesses", &Tracks);
	tree->Branch("detector", &d);

  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------
	cout << "\nRECONSTRUCTOR\n";

  for (unsigned int ev = 0; ev < events->GetEntries(); ++ev)
  {
		if (!quiet) cout << "Event " << ev << "\n";
		events->GetEntry(ev);

		// Declarations
		TrackRecon guess;
		guess.Options.clear();
		guess.Sigmas.clear();
		guess.Areas.clear();
		guess.ExpectedNumber.clear();
		guess.Params.clear();
		vector< double > params;
		vector<ParticleOut> &pars = event_output->Particles;
		// ---------------

	  // remove all particles except for last particles determined by option 'l'
    removeFirstParticles(ai.last_given, event_output, last);
	  ReconstructEvent(reconstruction, event_output, print);
	  
	  if (print && !(reconstruction.Photons.size())){
	  	printf("\tEvent %i had no reconstructions\n", ev);
	  	continue;
	  }
  	
	  // create histograms
	  for (unsigned int par = 0; par < pars.size(); par++){
	  	if (!quiet) cout << "\tParticle " << par << endl;
  		
  		vector<PhotonOut> &phos = reconstruction.Photons.at(par);
	  	vector< vector<double> > data; data.clear();
	  	vector<double> data_sub; data_sub.clear();

	  	for (unsigned int i = 0; i < phos.size(); i++)
	  	{
	  		data.push_back(data_sub);
	  			data.back().push_back(phos.at(i).Phi);
	  			data.back().push_back(phos.at(i).Theta);
	  	}
	  	A.SetData(data);
	  	
	  	string histTitle = wul::appendStrings(wul::stringDouble("Event ", ev), wul::stringDouble(", Particle ", par+1));
			string histname = histName0(ev, par);
		  string TH1Name = histname; TH1Name.append("_1D");
		  string TH2Name = histname; TH2Name.append("_2D");

		  if (phos.size()){
			  A.AddTH1D(TH1Name.c_str(), histTitle.c_str(), xbins, 0, pi, 1);
			  A.AddTH2D(TH2Name.c_str(), histTitle.c_str(), xbins, -pi, pi, ybins, 0, pi);
			}
			else
				continue;

			Identifier guesser;
			TH1D *h1_p = &A.Hists1D.back();
			guess.Hist = *h1_p;
			ParticleOut &P = pars.at(par);
			if (print){
				printf("\tpar = %i: eta = %f, pt = %f\n", par+1, P.Eta, P.pt);
			}

			mass m(P.Eta, P.pt);
			map<double, double> &atm = m.AngletoMass;
			map<double, string> &mtn = m.MasstoName;
			map<string, double> &pm = guesser.probabilitymap;

			double travels = 0.;

			string defaultname = h1_p->GetName(); 

			for(map<double, double>::iterator i = atm.begin(); i != atm.end(); ++i){
				TCanvas c1("c1","c1",10,10,800,600);
				TCanvas *c1_p = &c1;
				const double &angle = i->first;
				const double &mass = i->second;
				double r = .1;		// range
				string name = mtn[mass];

				stringstream currentname;
				currentname << defaultname << "_" << name;
				string newhname = currentname.str();

				// cout << "name = " << name << endl;			  
				// cout << "angle = " << angle << endl;
				double Area = guesser.FitParticle1D(c1_p, *h1_p, params, angle-r, angle+r, angle, smear, newhname, graphprefix, make, print);	// area under gaussian (calculated number of photons)
				// cout << "Area = " << Area << endl;
		  	double ThetaBeam = 2*atan(exp(-P.Eta));
				double momentum = P.pt/sin(ThetaBeam);
			  double Beta = momentum/pow(( mass*mass + momentum*momentum ),.5);
			  double N = ExpectedNumberofPhotons(P.X, P.Y, P.Theta, P.Phi, Beta);
			  // cout << "Look Up Table N = " << N << endl;
			  // cout << "RiemannSum N = " << RiemannSum(P.X, P.Y, P.Theta, P.Phi, Beta) << "\n\n";

				double pi2 = TMath::Pi()/2;
				double Sigma = sqrt(N);
				double nSigma = (abs(N-Area))/Sigma;

				pm[name] = nSigma;
				if (print) {
					printf("\t\t%s: Area = %f, Expected = %f, sigma = %f\n", name.c_str(), Area, N, nSigma);
					cout << "\t\t\ttotal photons = " << travels*P.PhotonsPercm << endl;
					cout << "\t\t\tAngle = " << angle << endl;
				}

				guess.Options.push_back(name);
				guess.Sigmas.push_back(pm[name]);
				guess.Areas.push_back(Area);
				guess.ExpectedNumber.push_back(N);
				guess.Params.push_back(params);
			}
			Tracks.Recon.push_back(guess);
	  }


	  tree->Fill();
		Tracks.Recon.clear();
  }

  file2.cd();
  file2.Write();
  file2.Close();
  
  file.cd();
  file.Close();


  if (!quiet) cout << "reconstruction file: " << wf << endl;

  return 0;
}