//===================================================
//                Histogram Fitter
//
//====================================================

#include "../headers/fitter.h"
#include <cstdlib>
#include "cmdline.h"

int main(int argc, char** argv)
{
	gengetopt_args_info ai;  
	if (cmdline_parser (argc, argv, &ai) != 0){ exit(1); }
	if (ai.new_given)
	{
	  system("exec ./../../generator/build/generator");
	  system("exec ./../../simulator/build/simulator");
	  system("exec ./../../reconstructor/build/reconstructor");
	  system("exec ./../../histmaker/build/histmaker");
	}
	string analysisf = "../../root_files/analysis.root";
	string recf = "../../root_files/simulator.root";

	Analysis* A = 0;
	GeneratorOut *data = 0;

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
	//	THe domain abot a sigma off each emission angle is checked - one for each histogram.
	for (unsigned int ev = 0; ev < Analyses->GetEntries(); ev++)
	{
		cout << "Event " << ev << endl;
		Analyses->GetEntry(ev);
		EventOutput->GetEntry(ev);
		
		vector<ParticleOut> &pars = data->Particles;
		vector<TH1D> &hists1 = A->Hists1D;
		vector<TH2D> &hists2 = A->Hists2D;
		
		Fit *F = new Fit();
		F->NumPar(hists1.size());
		// F->Add2DHists(A->Hists2D);
		for (unsigned int par = 0; par < pars.size(); par++)
		{
			cout << "\tpar = " << par << endl;
			ParticleOut &P = pars.at(par);
			TH1D *h1_p = &hists1.at(par);
			TH2D *h2_p = &hists2.at(par);

			map<double, double> projectedpass;						// Emission Angle to Projected Survival Percent
			FindLostPhotons(P.X, P.Y, P.Theta, P.Phi, P.Eta, P.pt, d.Length, d.Width, d.Height, projectedpass);

			int &nphotons =  P.NumberofPhotons;
			TCanvas *c1 = new TCanvas("c1","c1",10,10,1000,900);
			TSpectrum *s = new TSpectrum(nphotons);
			TH1 *h1 = h1_p; 
			TH1 *h2 = h2_p; 
			int nfound = s->Search(h1,2,"", .5);
      double* xpeaks = new double[nfound];
      xpeaks = s->GetPositionX();
			printf("Found %d candidate peaks to fit\n",nfound);
			cout << "\tpeak at " << xpeaks[0] << endl;

			
			for (map<double,double>::iterator it = projectedpass.begin(); it != projectedpass.end(); ++it)
			{
				double xlow = it->first -.1; double xhigh = it->first + .1;
				double angle = it->first;
				double loss = it->second;
				double dif = xpeaks[0] - angle;
				cout << angle << endl;
				cout << "dif = " << xpeaks[0] - angle << endl;
				// F->Add1DFit(hists1.at(par), par, angle);
			}
			delete xpeaks;
			delete c1;
		}
		delete F;
	}

}