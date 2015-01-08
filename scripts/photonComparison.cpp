#include "efficiencyheader.h"
#include "folders.h"
#include "FileProperties.h"
#include "utility_library.h"

int main(int argc, char* argv[])
{
  argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
  option::Stats  stats(usage, argc, argv);
  option::Option* options = new option::Option[stats.options_max];
  option::Option* buffer  = new option::Option[stats.buffer_max];
  option::Parser parse(usage, argc, argv, options, buffer);

  if (parse.error())
    return 1;

  if (options[HELP] || argc == 0) {
    option::printUsage(std::cout, usage);
    return 0;
  }
  
	for (int i = 0; i < parse.nonOptionsCount(); ++i)
	  std::cout << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";

	string defaultdir = "";
	string graphdir = "comparison/";
	string simdata = "cheat.root";
	string recdata = "reconstruction.root";
	string area_sim_ouput = "area_vs_simulation.pdf";
	string sim_exp_ouput = "simulation_vs_expectation.pdf";
	string area_exp_ouput = "area_vs_expectation.pdf";

	if(options[DIRECTORY]) defaultdir = options[DIRECTORY].arg;
	if(options[SIMDATA]) simdata = options[SIMDATA].arg;
	if(options[RECDATA]) recdata = options[RECDATA].arg;
	if(options[GRAPHDIR]) graphdir = options[GRAPHDIR].arg;
	else wul::appendStrings(defaultdir, graphdir); // make graph directory appropriate

	wul::FileProperties area_sim_prop(area_sim_ouput);
	
	
	area_sim_prop.change_directory(graphdir);
	area_sim_prop.appendFileToDirectory(graphdir, sim_exp_ouput);
	area_sim_prop.appendFileToDirectory(graphdir, area_exp_ouput);
	area_sim_ouput = area_sim_prop.file();
	
	ParticleEvent *originals = 0;
	TrackRecons *reconstructions = 0;
	
	TFile f1(simdata.c_str(), "read");
	TTree *t1 = (TTree*)f1.Get("cheat_info");
	t1 -> SetBranchAddress("Particle Event", &originals);

	TFile f2(recdata.c_str(), "read");
	TTree *t2 = (TTree*)f2.Get("identifications");
	t2 -> SetBranchAddress("guesses", &reconstructions);

	int nentries = t2->GetEntries();
	
	vector<Particle> *pars = &originals->Particles;
	vector<TrackRecon> *recons = &reconstructions->Recon;

	vector<int> vec_area; 						// photons from area
	vector<int> vec_sim; 					// photons from simulation
	vector<int> vec_expected; 						// photons from expectations
	for (unsigned int ev = 0; ev < nentries; ++ev){
		t1->GetEntry(ev); t2->GetEntry(ev);

		int size_difference = matchDataSize(*recons, *pars);
		for (unsigned int p = 0; p < recons->size(); ++p){
			Particle* P = &pars->at(p);								// holds passed amount
			TrackRecon* R = &recons->at(p); 					// holds expected and area
			vec_sim.push_back(P->nPhotonsPassed);
			for (unsigned int opt = 0; opt < R->Options.size(); ++opt){
				if (R->Options.at(opt) == P->name){
					vec_expected.push_back(R->ExpectedNumber.at(opt));
					vec_area.push_back(R->Areas.at(opt));
				}
			}
		}
	}



	int bin_low = 1;
	int bin_high = vec_sim.size();
	
	TH1D sim_exp_disc("sim_exp_disc", "Discrepancy in Expected vs. Simulated Number of photons (Expected - Simulated) ", 1200, -100, 100);
	TH1D area_sim_disc("area_sim_disc", "Discrepancy in Simulated Number of photons vs. Area under fit (Simulated - Area)", 1200, -100, 100);
	TH1D area_exp_disc("area_exp_disc", "Discrepancy in Expected Number of photons vs. Area under fit (Expected - Area)", 1200, -100, 100);

	for (unsigned int i = 0; i < bin_high; ++i){
		int sim_exp_dif = vec_expected.at(i) - vec_sim.at(i);
		int area_sim_dif = -(vec_area.at(i) - vec_sim.at(i));
		int area_exp_dif = vec_expected.at(i) - vec_area.at(i);

		sim_exp_disc.Fill(sim_exp_dif);
		area_sim_disc.Fill(area_sim_dif);
		area_exp_disc.Fill(area_exp_dif);
	}


	TCanvas C("C", "C", 1000, 600);
	sim_exp_disc.Draw();
	C.Print(sim_exp_ouput.c_str());
	C.Clear();
	area_sim_disc.Draw();
	C.Print(area_sim_ouput.c_str());
	C.Clear();
	area_exp_disc.Draw();
	C.Print(area_exp_ouput.c_str());

	delete options;
	delete buffer;
	return 0;
}