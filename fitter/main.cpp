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
  // system("rm *.png");
	gengetopt_args_info ai;  
	if (cmdline_parser (argc, argv, &ai) != 0){ exit(1); }
	if (ai.new_given)
	{
	  system("exec ./../../generator/build/generator");
	  system("exec ./../../simulator/build/simulator");
	  system("exec ./../../reconstructor/build/reconstructor");
	  system("exec ./../../histmaker/build/histmaker");
	}

	bool make = ai.make_given;
	bool print = ai.verbose_given;
	bool modified = ai.particle_info_modified_given;
	bool writefile = ai.writefile_given;
	bool NewBatch = ai.New_Batch_given;

	// if (make){
	// 	string prefix = "../../Graphs/";
	// 	string command = "rm ";
	// 	command.append(prefix);
	// 	command.append("*.png");
	// 	system(command.c_str());
	// }
	string analysisf = "../../root_files/analysis.root";			// file with created histograms
	string recf = "../../root_files/simulator.root";					// file with particleOut data
	string wf = "../../root_files/fitresults.root";							// file with results on identification 
	string batch = "fits.txt";
	
	if(ai.Batch_given) batch = ai.Batch_arg;

	if (NewBatch){
		string cm = "rm ";
		cm.append(batch);
		system(cm.c_str());
	}

	if (writefile){
		wf = ai.writefile_arg;
		string prefix = returnBeg(wf, returnEnd(wf, "/"));
		string tempfile = returnEnd(wf, "/");				// get file name for input
		string batchpath = prefix; batchpath.append(batch);
		addtoFile(batchpath, tempfile);
	}
	if (modified)
		recf = ai.particle_info_modified_arg;

	Analysis* A = 0;
	GeneratorOut *data = 0;
	TrackRecons Tracks; Tracks.Recon.clear();		// stores information on particle guesses
	map<double, std::string> massmap;

	TFile AnaF(analysisf.c_str(), "Update");
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
	cout << "\nHistogram Fitter\n";
	Detector d;
	// There is a vector of Histograms for each event A->Hists1D
	// For each histogram in this vector, 4 more are created
	// Now there are 5 histograms and 5 Emission Angles
	for (unsigned int ev = 0; ev < Analyses->GetEntries(); ev++)
	{
		cout << "Event " << ev << endl;
		//----- ERROR HERE. Seems if histograms are printed in histmaker, this messed up and starts calling errors...-----
		Analyses->GetEntry(ev);

		//-----------------------------------------------------
		TrackRecon guess;

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
			Identifier guesser;
			TH1D *h1_p = &hists1.at(par);
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
			FindLostPhotons(P.X, P.Y, P.Theta, P.Phi, P.Eta, P.pt, d.Length, d.Width, d.Height, projectedpass, travels, print);
			vector<TH1D> tempVector;
			string defaultname = h1_p->GetName(); 

			for(map<double, double>::iterator i = atm.begin(); i != atm.end(); ++i){
				TCanvas c1("c1","c1",10,10,800,600);
				TCanvas *c1_p = &c1;
				const double &angle = i->first;
				const double &mass = i->second;
				double r = .1;		// range
				string name = mtn[mass];
				TH1D *temphist = h1_p;

				stringstream currentname;
				currentname << defaultname << "_" << name;
				string newhname = currentname.str();
			
				double A = guesser.FitParticle1D(c1_p, *temphist, angle-r, angle+r, angle, .01, newhname, make, print);	// area under gaussian (calculated number of photons)
				double N = projectedpass[angle]*travels*P.PhotonsPercm;	// expected number of photons
				d.get_Critical_Angle(1);
				double pi2 = TMath::Pi()/2;
				double p = (pi2-d.CriticalAngle)/pi2;
				double Sigma = sqrt(N*p*(1-p));
				double nSigma = (abs(N-A))/Sigma;

				pm[name] = nSigma;
				if (print) {
					printf("\t\t%s: Area = %f, Expected = %f, sigma = %f\n", name.c_str(), A, N, nSigma);
					cout << "\t\t\ttotal photons = " << travels*P.PhotonsPercm << endl;
					cout << "\t\t\tAngle = " << angle << endl;
				}

				// TObject *hist = temphist->Clone();		// Tobject instead of TH1 to keep functions with histogram
				tempVector.push_back(*temphist);

				// hist->Print();
			}
			guesser.reverseprobabilitymap();
			double dSigma = getMin(pm);
			guess.name = rpm[dSigma];
			guess.deltaSigma = dSigma;
			Tracks.Recon.push_back(guess);
			if (print){
				cout << endl;
				cout << "\tchose: " << guess.name;
				cout << " with delta sigma " << guess.deltaSigma << "\n\t----------------------------------------\n";
			}

		}
		tree.Fill();
		Tracks.Recon.clear();
	}
	writef.cd();
	writef.Write();
	writef.Close();

	GenF.cd();
	GenF.Close();
	

	AnaF.cd();
	AnaF.Close();
}