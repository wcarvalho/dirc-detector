//===================================================
//                Histogram Fitter
//
//====================================================

#include "../headers/fitter.h"
#include <cstdlib>
#include "cmdline.h"

using namespace std;


int main(int argc, char** argv)
{
	// Case statement for my current function or to the function that alan gives
	// void* fun = asjkajs(); 
  // system("rm *.png");
	gengetopt_args_info ai;  
	if (cmdline_parser (argc, argv, &ai) != 0){ exit(1); }

	bool make = ai.make_given;
	bool print = ai.verbose_given;
	bool modified = ai.particle_info_modified_given;
	bool writefile = ai.writefile_given;
	bool NewBatch = ai.New_Batch_given;
	double smear = .01;
	string graphprefix = "";
 	
 	timeval t1, t2;
	double time1 = 0;
	double time2 = 0;
	double fit_time=0.;
	double lost_time=0.;

	string analysisf = ai.input_analysis_arg;									// file with created histograms
	string recf = ai.input_reconstruction_arg;									// file with particleOut data
	string wf = "fitresults.root";							// file with results on identification 
	string batch = "fits.txt";
	
	if(ai.Batch_given) batch = ai.Batch_arg;
	if(ai.Smear_given) smear = ai.Smear_arg;
	if(ai.graph_prefix_given) graphprefix = ai.graph_prefix_arg;

	if (NewBatch){
		string cm = "rm ";
		cm.append(batch);
		system(cm.c_str());
	}

	if (modified){
		recf = ai.particle_info_modified_arg;
	}
		
	FileProperties readf_prop(recf);
	string directory = readf_prop.directory;
	
	if(ai.Directory_given) directory = ai.Directory_arg;
	readf_prop.appendFileToDirectory(directory, wf);

	Analysis* A = 0;
	GeneratorOut *data = 0;
	TrackRecons Tracks; Tracks.Recon.clear();		// stores information on particle guesses
	map<double, std::string> massmap;

	TFile AnaF(analysisf.c_str(), "read");
	TTree* Analyses = (TTree*)AnaF.Get("THists");
	Analyses->SetBranchAddress("EventHists", &A);

	TFile GenF(recf.c_str(), "read");
	TTree* EventOutput = (TTree*)GenF.Get("sim_out");
	EventOutput->SetBranchAddress("simEvent", &data);


	TFile writef(wf.c_str(), "recreate");
	TTree tree("identifications", "information on what particles tracks were reconstructed into, and with what probability");
	tree.Branch("guesses", &Tracks);



  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------
	cout << "\nHistogram Fitter with resolution " << smear << endl;
	Detector d;
	// There is a vector of Histograms for each event A->Hists1D
	// For each histogram in this vector, 4 more are created
	// Now there are 5 histograms and 5 Emission Angles
	for (unsigned int ev = 0; ev < Analyses->GetEntries(); ev++)
	{
		cout << "Event " << ev << endl;
		
		Analyses->GetEntry(ev);
		//-----------------------------------------------------
		TrackRecon guess;
		guess.Options.clear();
		guess.Sigmas.clear();
		guess.Areas.clear();
		guess.ExpectedNumber.clear();
		guess.Params.clear();
		vector< double > params;
		
		EventOutput->GetEntry(ev);
		vector<ParticleOut> &pars = data->Particles;
		unsigned int npars = pars.size();
		vector<TH1D> &hists1 = A->Hists1D;
		vector<TH2D> &hists2 = A->Hists2D;
		// Fit *F = new Fit();
		// F->NumPar(npars);
		if (hists1.size() == 0) continue;
		for (unsigned int par = 0; par < pars.size(); par++) // (one particle per histogram)
		{
			cout << "\tParticle " << par << endl;
			Identifier guesser;
			TH1D *h1_p = &hists1.at(par);
			guess.Hist = *h1_p;
			ParticleOut &P = pars.at(par);
			if (print)
			{
				printf("\tpar = %i: eta = %f, pt = %f\n", par+1, P.Eta, P.pt);// F->SetPrint();
			}
		// 	TH2D *h2_p = &hists2.at(par);
			
			mass m(P.Eta, P.pt);
			map<double, double> &atm = m.AngletoMass;
			map<double, string> mtn = m.MasstoName;
			map<string, double> &pm = guesser.probabilitymap;
			map<double, string> &rpm = guesser.rprobabilitymap;
			map<double, double> projectedpass;

			double travels = 0.;
			
			gettimeofday(&t1, NULL);
			FindLostPhotons(P.X, P.Y, P.Theta, P.Phi, P.Eta, P.pt, d.Length, d.Width, d.Height, projectedpass, travels, print);
			gettimeofday(&t2, NULL);
			time1 = (double)(t1.tv_sec) + (double)(t1.tv_usec)/1.0e6;
			time2 = (double)(t2.tv_sec) + (double)(t2.tv_usec)/1.0e6;
			lost_time += (time2-time1);


			vector<TH1D> tempVector;
			string defaultname = h1_p->GetName(); 

			for(map<double, double>::iterator i = atm.begin(); i != atm.end(); ++i){
				params.clear();
				TCanvas c1("c1","c1",10,10,800,600);
				TCanvas *c1_p = &c1;
				const double &angle = i->first;
				const double &mass = i->second;
				double r = .1;		// range
				string name = mtn[mass];

				stringstream currentname;
				currentname << defaultname << "_" << name;
				string newhname = currentname.str();
			  

        gettimeofday(&t1, NULL);
				double A = guesser.FitParticle1D(c1_p, *h1_p, params, angle-r, angle+r, angle, smear, newhname, graphprefix, make, print);	// area under gaussian (calculated number of photons)
				gettimeofday(&t2, NULL);
				time1 = (double)(t1.tv_sec) + (double)(t1.tv_usec)/1.0e6;
			  time2 = (double)(t2.tv_sec) + (double)(t2.tv_usec)/1.0e6;
			  fit_time += (time2-time1);


				double N = projectedpass[angle]*travels*P.PhotonsPercm;	// expected number of photons
				d.get_Critical_Angle(1);
				double pi2 = TMath::Pi()/2;
				double &p = projectedpass[angle];
				double Sigma = sqrt(N);
				double nSigma = (abs(N-A))/Sigma;

				pm[name] = nSigma;
				if (print) {
					printf("\t\t%s: Area = %f, Expected = %f, sigma = %f\n", name.c_str(), A, N, nSigma);
					cout << "\t\t\ttotal photons = " << travels*P.PhotonsPercm << endl;
					cout << "\t\t\tAngle = " << angle << endl;
				}

				guess.Options.push_back(name);
				guess.Sigmas.push_back(pm[name]);
				guess.Areas.push_back(A);
				guess.ExpectedNumber.push_back(N);
				guess.Params.push_back(params);
			}
			Tracks.Recon.push_back(guess);
		}
		tree.Fill();
		Tracks.Recon.clear();
	}
			cout << "time for FindLostPhotons " << lost_time << endl;
			cout << "time for Fitting " << fit_time << endl;
	writef.cd();
	writef.Write();
	writef.Close();

	cout << "file: " << wf << endl;
	GenF.cd();
	GenF.Close();
	

	AnaF.cd();
	AnaF.Close();
}