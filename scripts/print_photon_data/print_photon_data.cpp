#include <vector>
#include <string>
#include <unordered_map>
#include "boost/variant.hpp"

#include "data_collection_conditions.h"
#include "TFile.h"
#include "TTree.h"

#include "dirc_objects.h"
#include "utility_library.h"
// #include "event_parsers.h"

#include <tclap/CmdLine.h>
using namespace std;


typedef std::unordered_map <int, double(*)(const PhotonOut&)> flag_func_map;
typedef std::unordered_map <int, std::vector<double> > flag_data_map;
typedef std::unordered_map <int, int > flag_bin_map;
typedef std::unordered_map <int, TFile* > flag_TFile_map;

void collect_data(const vector<int>& flags, flag_func_map& flag_funcs, flag_data_map& flag_data, const PhotonOut& P);
void print_data(TFile &F, int nbins, const vector<double>& data);
int main(int argc, char const *argv[])
{

string dir;
string photondata;
vector< int > event_range;
bool event_range_set = false;
string matchsearch;
vector< int> flags;
vector< int> nbins;
vector<string> TFile_names;
bool print;
TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{
	TCLAP::ValueArg<std::string> particleFileArg("p","photon-file","file with photon data (cheat data)",false,"photon.root","string", cmd);

	TCLAP::ValueArg<std::string> matchsearchArg("","match-search","particle which is being matched for",false,"electron","string", cmd);

	TCLAP::ValueArg<std::string> DirectoryArg("D","directory","Directory where plots will be stored",false,"fits","string", cmd);
	TCLAP::MultiArg<int> eventRangeArg("e","event-range","events for which you want to print fits", false, "int", cmd);

	TCLAP::MultiArg<int> flagsArg("c","flag-condition","prints histograms of data sets chosen."
		"\n\t\tcase 1: time",true,"int", cmd);
	TCLAP::MultiArg<int> binArg("n","nbins","number of bins used for each plot type. Note: you must set this in the same order you set the conditions. If option 2 is chosen before option 1, the number of bins for option 2 must be given first",true,"int", cmd);
	TCLAP::MultiArg<string> filenameArg("o","output-files","Filenames for TFiles Note: you must set this in the same order you set the conditions. (See nbins condition)",true,"int", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);

	cmd.parse( argc, argv);
	dir = DirectoryArg.getValue();
	photondata = particleFileArg.getValue();
	matchsearch = matchsearchArg.getValue();
	flags = flagsArg.getValue();
	print = verboseArg.getValue();
	TFile_names = filenameArg.getValue();
	event_range_set = eventRangeArg.isSet();
	nbins = binArg.getValue();
	if ( event_range_set ){
		event_range = eventRangeArg.getValue();
		if ( event_range.at(1) < event_range.at(0)){
			cout << "ERROR: The second value for the range must be higher than the first value.\n\nExiting!\n";
			return 0;
		}
	}
}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }

	dir.append("/");

	vector<double> empty_vec;
	//------------------------------
	flag_func_map flag_funcs;
	flag_data_map flag_data;

	flag_funcs[1] = &time_data;
	flag_data[1] = empty_vec;
	// flag_funcs[2] = &correct_searchtype;
	// flag_funcs[3] = &within_sigma_threshold;
	//------------------------------
	flag_bin_map binmap;
	//FIXME: implement:	if (flags.size() (something) nbins.size()) then
	for (unsigned i = 0; i < flags.size(); ++i)
		binmap[flags.at(i)]=nbins.at(i);

	flag_TFile_map TFilemap;
	// TFile* empty = 0;
	for (unsigned i = 0; i < flags.size(); ++i){
		string filename = dirc::appendStrings(dir, TFile_names.at(i));
		TFile* tempfile = new TFile(filename.c_str(), "update");
		TFilemap[flags.at(i)] = std::move(tempfile);
	}
	// ;



	GeneratorOut *photon_data = 0;

	string file1 = photondata;

	TFile f1(file1.c_str(), "read");
	TTree *t1 = (TTree*)f1.Get("sim_out");
	t1 -> SetBranchAddress("simEvent", &photon_data);

	int nentries = t1->GetEntries();
	if ( !(event_range_set) ){
		event_range.clear();
		event_range.push_back(0);
		event_range.push_back(nentries);
	}
	auto* phos = &photon_data->Photons;
	for (unsigned ev = 0; ev < nentries; ++ev){
		if (ev > event_range[1]) break;
		if (ev < event_range[0]) continue;
		if (print) cout << "Event = " << ev << endl;
		t1->GetEntry(ev);
		for (unsigned int p = 0; p < phos->size(); ++p){
			auto* pho = &phos->at(p);
			collect_data(flags, flag_funcs, flag_data, *pho);
		}
	}
	for (unsigned i; i < flags.size(); ++i){
		int& index = flags.at(i);
		if (print) cout << "index = " << index << endl;
		vector<double>& data = flag_data[index];
		TFile& tfile = *TFilemap[index];
		if (print) cout << "\t"; tfile.Print();
		int& bins = binmap[index];
		if (print) cout << "\tnbins = " << bins << endl;
		print_data(tfile, bins, data);
	}
	for (auto i = TFilemap.begin(); i != TFilemap.end(); ++i)
		i->second->Close();
return 0;
}

void collect_data(const vector<int>& flags, flag_func_map& flag_funcs, flag_data_map& flag_data, const PhotonOut& P){

	for (unsigned i = 0; i < flags.size(); ++i){
		const int& index = flags.at(i);
		flag_data[index].push_back(flag_funcs[index](P));
	}
}

void print_data(TFile &F, int nbins, const vector<double>& data){

	double xlow = 0., xhi = 0;
	dirc::vectorminmax(data, xlow, xhi);
	TH1D h("h", "h", nbins, xlow, xhi);
	for (unsigned i = 0; i < data.size(); ++i)
		h.Fill(data.at(i));
	F.cd();
	h.Write();
}