#include "efficiencyheader.h"

int main()
{	

	string pre = "";
	string data_dir = "";
	string graph_dir = "graphs/";
	string fit_dir = "fits/";
	string fitdata = "fitresults.root";
	string cheatdata = "cheat.root";

	fillparameters("directories", pre, data_dir);

	string file1 = appendStrings(pre, cheatdata);	
	data_dir = appendStrings(pre, data_dir);
	string file2 = appendStrings(data_dir, fitdata);
	fit_dir = appendStrings(data_dir, fit_dir);

	ParticleEvent *originals = 0;
	TrackRecons *reconstructions = 0;
	
	TFile f1(file1.c_str(), "read");
	TTree *t1 = (TTree*)f1.Get("cheat_info");
	t1 -> SetBranchAddress("Particle Event", &originals);

	TFile f2(file2.c_str(), "read");
	TTree *t2 = (TTree*)f2.Get("identifications");
	t2 -> SetBranchAddress("guesses", &reconstructions);

	int nentries = t2->GetEntries();
	
	vector<Particle> *pars = &originals->Particles;
	vector<TrackRecon> *recons = &reconstructions->Recon;

	int i = 0;
	vector<int> vec_passed;
	vector<int> vec_expected;
	for (unsigned int ev = 0; ev < nentries; ++ev){
		cout << "Event = " << ev << endl;
		t1->GetEntry(ev); t2->GetEntry(ev);

		int size_difference = matchDataSize(*recons, *pars);
		for (unsigned int p = 0; p < recons->size(); ++p){
			++i;
			Particle* P = &pars->at(p);
			TrackRecon* R = &recons->at(p);
			vec_passed.push_back(P->nPhotonsPassed);
			// cout << vec_passed.back() << endl;
			for (unsigned int opt = 0; opt < R->Options.size(); ++opt){
				if (R->Options.at(opt) == P->name){
					vec_expected.push_back(R->ExpectedNumber.at(opt));
					// cout << vec_expected.back() << endl << endl;
				}
			}
		}
	}



	int bin_low = 1;
	int bin_high = vec_passed.size();
	TH1D photons_passed("Passed Photons", "Passed Photons", bin_high, bin_low, bin_high);
	TH1D photons_expected("Expected Photons", "Expected Photons vs. ", bin_high, bin_low, bin_high);

	TH1D disc("discrepancy", "Discrepancy in Expected vs. Actual ", 1200, -400, 400);

	vector<int> discrepancy;
	for (unsigned int i = 0; i < bin_high; ++i){
		photons_passed.SetBinContent(i, vec_passed.at(i));
		photons_expected.SetBinContent(i, vec_expected.at(i));
		int dif = vec_expected.at(i) - vec_passed.at(i);
		discrepancy.push_back(dif);	
		disc.Fill(dif);
	}


	TCanvas C("C", "C", 1000, 600);
	disc.Draw();
	// photons_expected.SetTitle("Expected Number of Photons vs. Actual Number of Photons");
	// photons_expected.GetXaxis()->SetTitle("Particle #");
	// photons_expected.GetYaxis()->SetTitle("Number of Photons");
	// photons_expected.Draw();
	// photons_passed.SetLineColor(kRed);
	// photons_passed.Draw("same");
	C.Print("hell.pdf");

   // // scale hint1 to the pad coordinates
   // Float_t rightmax = 1.1*photons_expected->GetMaximum();
   // Float_t scale = gPad->GetUymax()/rightmax;
   // hint1->SetLineColor(kRed);
   // hint1->Scale(scale);
   // hint1->Draw("same");

   // draw an axis on the right side
   // TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
   // gPad->GetUxmax(), gPad->GetUymax(),0,rightmax,510,"+L");
   // axis->SetLineColor(kRed);
   // axis->SetTextColor(kRed);
   // axis->Draw();
   // return c1;
}