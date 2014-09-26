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
	
	if (make){
		string prefix = "../../Graphs/";
		string command = "rm ";
		command.append(prefix);
		command.append("*.png");
		system(command.c_str());
	}
	string analysisf = "../../root_files/analysis.root";			// file with created histograms
	string recf = "../../root_files/simulator.root";					// file with particle data


	Analysis* A = 0;
	GeneratorOut *data = 0;
	map<double, std::string> massmap;

	TFile AnaF(analysisf.c_str(), "Update");
	TTree* Analyses = (TTree*)AnaF.Get("THists");
	Analyses->SetBranchAddress("EventHists", &A);

	TFile GenF(recf.c_str(), "read");
	TTree* EventOutput = (TTree*)GenF.Get("sim_out");
	EventOutput->SetBranchAddress("simEvent", &data);

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
		if (print == true) cout << "Event " << ev << endl;
		//----- ERROR HERE. Seems if histograms are printed in histmaker, this messed up and starts calling errors...-----
		Analyses->GetEntry(ev);
		//-----------------------------------------------------
		EventOutput->GetEntry(ev);
		
		vector<ParticleOut> &pars = data->Particles;
		unsigned int npars = pars.size();
		vector<TH1D> &hists1 = A->Hists1D;
		vector<TH2D> &hists2 = A->Hists2D;
		
		// Fit *F = new Fit();
		// F->NumPar(npars);

		for (unsigned int par = 0; par < pars.size(); par++) // (one particle per histogram)
		{
			Identifier *guesser = new Identifier();
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
			map<string, double> &pm = guesser->probabilitymap;
			map<double, double> projectedpass;

			double travels = 0.;
			FindLostPhotons(P.X, P.Y, P.Theta, P.Phi, P.Eta, P.pt, d.Length, d.Width, d.Height, projectedpass, travels, print);
			vector<TObject> tempVector;
			string defaultname = h1_p->GetName(); 

			for(map<double, double>::iterator i = atm.begin(); i != atm.end(); ++i){
				TCanvas *c1 = new TCanvas("c1","c1",10,10,800,600);
				const double &angle = i->first;
				const double &mass = i->second;
				double r = .1;		// range
				string name = mtn[mass];
				TH1D *temphist = h1_p;

				stringstream currentname;
				currentname << defaultname << "_" << name;
				string newhname = currentname.str();
				
				double A = guesser->FitParticle1D(c1, *temphist, angle-r, angle+r, angle, .01, newhname, make, print);	// area under gaussian (calculated number of photons)
				double N = projectedpass[angle]*travels*P.PhotonsPercm;	// expected number of photons
				double Sigma = sqrt(N);
				double nSigma = (abs(N-A))/Sigma;

				pm[name] = nSigma;
				if (print) {
					printf("\t\t%s: Area = %f, Expected = %f, sigma = %f\n", name.c_str(), A, N, nSigma);
					cout << "\t\t\ttotal photons = " << travels*P.PhotonsPercm << endl;
				}

				TObject *hist = temphist->Clone();		// Tobject instead of TH1 to keep functions with histogram
				tempVector.push_back(*hist);

				delete hist;

				// hist->Print();
				delete c1;
			}
			if (print) cout << endl;
			guesser->particle1Dfits[P.Theta] = tempVector;
		

		// 	map<double, double> projectedpass;						// Emission Angle to Projected Survival Percent

		// // 	int &nphotons =  P.NumberofPhotons;
			
			delete guesser;
		}
	}

}