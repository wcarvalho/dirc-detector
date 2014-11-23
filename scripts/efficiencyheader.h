#include "dirc_objects.h"

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>      // std::setfill, std::setw
#include <sys/time.h>

#include "TLatex.h"
#include "TMath.h"
#include "TH1D.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"

using namespace std;

// returns n by 2 array for files 
vector< vector< double > > filearray(string file){

	ifstream f;
	f.open(file.c_str());

	vector< vector< double > > x; x.clear();
	vector< double > temp; temp.clear();
	
	int size = 256;
	char val1[size];
	char val2[size];

	while(f.good()){
		f.getline(val1, size, ',');
		f.getline(val2, size);
		temp.push_back(atof(val1));
		temp.push_back(atof(val2));
		x.push_back(temp);
		temp.clear();
		
	}	
	return x;
}





// get general array and data-specfic array
void fillparameters(string const &settings, string &pre, string &data_dir, string &fit_dir, string &graph_dir){

	ifstream f;
	f.open(settings.c_str());
	int size = 256;
	char val[size];
	int i = 0;
	while (f.good()){
		f.getline(val, size);
		if (i == 0) pre = val;
		if (i == 1)	data_dir = val;
		if (i == 2)	graph_dir = val;
		if (i == 3)	fit_dir = val;
		++i;
	}

}



// concatonate strings together
string appendStrings(string const &string1, string const &string2){
	stringstream ss; ss.str("");
	ss << string1 << string2;
	return ss.str();
}





// shrinks the original Particle data so it only has the data corresponding to the Reconstructed Data (assuming that info comes from the end of the particle data array)
int matchDataSize(vector<TrackRecon>& a, vector<Particle>& b){

	//sizes of a and b
	int sa = a.size();
	int sb = b.size();
	int dif = abs(sb-sa);

	if (sb>sa){
		for (unsigned int i = 0; i < dif; ++i)
			b.erase(b.begin());
	}
	else{
		for (unsigned int i = 0; i < dif; ++i)
			a.erase(a.begin());
	}
	return dif;
}





// finds every case that the name of the particle matches search and the name of the reconstruction matches tomatch. The former fills search_den, and the latter search_num. The multiplicity of the event is recorded for every pair added to either vector.
void getMatch(Particle *P, string search, string tomatch, double threshold, TrackRecon* R, int multiplicity, vector< pair<int, pair <Particle, TrackRecon> > > &search_num, vector< pair<int, pair <Particle, TrackRecon> > > &search_den, bool print){

	vector<double> &sigmas = R->Sigmas;
	vector<string> &names = R->Options;

	if(P->name == search){
		pair <Particle, TrackRecon> PR; PR.first = *P; PR.second = *R;
		pair <int, pair <Particle, TrackRecon> > den(multiplicity, PR);
		search_den.push_back(den);
		// if (print) cout << "\t" << P->name << " with pt, multiplicity = " << P->pt << ", " << multiplicity << " and " << sigmas.size() << " options\n";
		if ( sigmas.size() != names.size() ) exit(1);
		for (unsigned int opt = 0; opt < sigmas.size(); ++opt){
			// if (print){
			// 	cout << "\t\t" << names[opt] << ": sigma = " << sigmas[opt] << ", ";
			// 	cout << "Area = " << R->Areas[opt] << ", ";
			// 	cout << "Expected = " << R->ExpectedNumber[opt] << endl;
			// }
			if ((sigmas[opt] < threshold) && (names[opt] == tomatch)){
				search_num.push_back(den);
				// if (print) cout << "\tSELECTED\n";
			}
		}
	}

}





// fill a histogram with reconstructed data
void FillHist(TH1D &H, vector< pair<int, pair <Particle, TrackRecon> > > const & vec, int Case, vector< double > range, bool print){

	if (print) cout << "range = " << range[0] << ", " << range[1] << endl;
	for(unsigned int i = 0; i < vec.size(); ++i){

		const int &multiplicity = vec.at(i).first;
		const double &pt = vec.at(i).second.first.pt;
		const double &IncidentTheta = vec.at(i).second.first.Theta;
		const double &IncidentPhi = vec.at(i).second.first.Phi;
		const double &EmissionAngle = vec.at(i).second.first.ConeAngle;
		bool NotInPtRange = ((pt < range[0])||(pt > range[1]));
		bool NotInMultiRange = ((multiplicity < range[0])||(multiplicity > range[1]));
		// cout << "NotInPtRange = " << NotInPtRange << endl;
		// cout << "NotInMultiRange = " << NotInMultiRange << endl;
		switch (Case) {
			case 1:	// vs. Multiplicity
				if (NotInPtRange)
					continue;
				H.Fill(multiplicity);
				break;
			case 2: // vs. PT
				if (NotInMultiRange)
					continue;
				H.Fill(pt);
				break;
			case 3: // vs. Incident Angle (Theta) against pt
				if (NotInPtRange)
					continue;
				H.Fill(IncidentTheta);
				break;
			case 4: // vs. Incident Angle (Phi) against pt
				if (NotInPtRange)
						continue;
				H.Fill(IncidentPhi);
			break;
			default:
				cout << "Wrong Case!\n";
				exit(1);
			break;
		}
	}
}

				// if (print) cout << "\tmultiplicity, pt = " << multiplicity << ", " << pt << endl;
				// if (print) cout << "\tmultiplicity, pt = " << multiplicity << ", " << pt << endl;






TGraphAsymmErrors graphASymm(TCanvas &c, double low, double hi, double nbins, vector< pair<int, pair <Particle, TrackRecon> > > const & Num, vector< pair<int, pair <Particle, TrackRecon> > > const & Den, int Case, vector< double > range, bool &makegraph, bool print){

	TH1D NumHist("Num", "Num", nbins, low, hi);
	TH1D DenHist("Den", "Den", nbins, low, hi);
	
	if (print) cout << "Numerator:\n";
	FillHist(NumHist, Num, Case, range, print);
	
	if (print) cout << "Denominator:\n";
	FillHist(DenHist, Den, Case, range, print);

	TGraphAsymmErrors graph;
	
	int numcounter = NumHist.GetEntries();
	int dencounter = DenHist.GetEntries();
	
	
	makegraph = dencounter;
	if (!dencounter){
		if (numcounter)
			cout << "The denominator is empty but the numerator isn't\n";
	}

	graph.BayesDivide(&NumHist, &DenHist);
	graph.SetMarkerStyle(20);
	graph.Draw("AP");
	graph.GetYaxis()->SetRangeUser(0.,1.);
	
	return graph;
}







void GraphXYTitle(TGraphAsymmErrors &g, string title, string x, string y){

	g.SetTitle(title.c_str());
	g.GetXaxis()->SetTitle(x.c_str());
	g.GetYaxis()->SetTitle(y.c_str());
}







string GraphFileName(string data_dir, string graph_dir, string filename_base, string xaxis, int appendage, string suffix){
	stringstream ss;
	ss << data_dir << graph_dir << filename_base << appendage << "_vs._"<< xaxis << suffix;
	return ss.str();

}





// ---------------------------------------------------------------------------

void makePlots(TCanvas &C, vector< vector <double> > &bounds, string xtitle, string parameter, string data_dir, string graph_dir,
               string matchgraph_filebase, string falsegraph_filebase, double histlow, double histhi, int nbins,
               vector< pair<int, pair <Particle, TrackRecon> > > const & numMatch, vector< pair<int, pair <Particle, TrackRecon> > > const & denMatch,
               vector< pair<int, pair <Particle, TrackRecon> > > const & numFalse, vector< pair<int, pair <Particle, TrackRecon> > > const & denFalse, int Case, int &filenumber, bool print){
	bool makegraph = true;
	for (unsigned int i = 0; i < bounds.size(); ++i){
		if (print){
			cout << "------------------------------------------" << endl;
			cout << "-----Efficiency----\n";
		}
		stringstream ss;
		ss << xtitle << "( " << parameter << " = [" << bounds[i][0] << " : " << bounds[i][1] << "] )";
		string xaxis = ss.str();

		string filename = GraphFileName(data_dir, graph_dir, matchgraph_filebase, xtitle, filenumber, ".pdf");
		string rootfilename = GraphFileName(data_dir, graph_dir, matchgraph_filebase, xtitle,filenumber, ".root");
		TGraphAsymmErrors match_graph = graphASymm(C, histlow, histhi, nbins, numMatch, denMatch, Case, bounds[i], makegraph, print);
		match_graph.SetName(GraphFileName("", "", matchgraph_filebase, xtitle,filenumber, "").c_str());
		GraphXYTitle(match_graph, "Efficiency for Identifying Electrons", xaxis.c_str(), "Efficiency");
		
		if (makegraph){
			C.Print(filename.c_str());
			// TFile f(rootfilename.c_str(), "recreate");
			// TFile f("test", "recreate");
			// match_graph.Write();
			// f.Close();
		}
		C.Clear();
		makegraph = true;
		if (print) cout << "-----False----\n";
		
		filename = GraphFileName(data_dir, graph_dir, falsegraph_filebase, xtitle,filenumber, ".pdf");
		rootfilename = GraphFileName(data_dir, graph_dir, falsegraph_filebase, xtitle,filenumber, ".root");
		TGraphAsymmErrors false_graph = graphASymm(C, histlow, histhi, nbins, numFalse, denFalse, Case, bounds[i], makegraph, print);
		false_graph.SetName(GraphFileName(data_dir, graph_dir, falsegraph_filebase, xtitle,filenumber, "").c_str());
		
		GraphXYTitle(false_graph, "False Positive for Identifying Electrons", xaxis.c_str(), "False Positive");
		
		if (makegraph){
			C.Print(filename.c_str());
			// TFile f(rootfilename.c_str(), "recreate");
			// match_graph.Write();
			// f.Close();
		}
		C.Clear();
				// exit(1);

		++filenumber;
	}
}

// ---------------------------------------------------------------------------


bool withinBounds(double const value, double comparison, double percent){
	// cout << "comparison:\t" << percent*comparison << "|\t" << value << "\t\t|";
	// cout << (1-percent)*comparison << endl;
	bool result = !((value < percent*comparison)||(value > (1-percent)*comparison));
	// cout << "result = " << result << endl;
	return result;
}
void clearOuterParticles(vector< pair<int, pair <Particle, TrackRecon> > > const &set, vector< pair<int, pair <Particle, TrackRecon> > > &newset, double length, double width, double percent){

	for (unsigned int i = 0; i < set.size(); ++i){
		const double &x = set.at(i).second.first.X;
		const double &y = set.at(i).second.first.Y;
		if ( ( (withinBounds(x, length, percent)) && (withinBounds(y, width, percent)) ) ){
			newset.push_back(set.at(i));
		}
	}
}

// ---------------------------------------------------------------------------
// prints fit information provided to a text file
void printFitInfo(TH1D &h, double xlow, double xhi, double center, double width, double N, string filebase){

	stringstream txt_file;
	txt_file << filebase << ".txt";

	ofstream f;
	f.open(txt_file.str().c_str());
	f << h.GetName() << endl;
	f << xlow << endl;
	f << xhi << endl;
	f << center << endl;
	f << width << endl;
	f << N << endl;
	f.close();

	stringstream hist_file;
	hist_file << filebase << "_hist.root";
	
  TFile tf(hist_file.str().c_str(), "recreate");
	h.Write();
  tf.Close();

}

string stringDouble(string s, double d){

	stringstream ss; ss.str("");
	ss << s << d;
	return ss.str();

}

// ---------------------------------------------------------------------------
// prints histograms of fits
void printfits(TCanvas &C, int Event, int par, Particle const &P, TrackRecon &R, string const dir, bool print){

	stringstream ss; ss.str("");
	ss << dir << "Event_" << setfill('0') <<setw(3) << Event << "_";
	ss << "Particle_" << setfill('0') <<setw(3) << par << "_";
	ss << P.name;

	string base_name = ss.str();
	
	TF1 f2("hello", "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) ) + [3]");
	TH1D &h = R.Hist;

	for (unsigned int i = 0; i < R.Params.size(); ++i){
		double latmax = 1;
		double xlow = R.Params.at(i).at(4);
		double xhi = R.Params.at(i).at(5);
		static double center = 0;
		static double width = 0;
		double &N = R.ExpectedNumber.at(i);

		for (unsigned int j = 0; j < 4; j++){
			f2.SetParameter(j, R.Params.at(i).at(j));
			if ( j == 1 ) center = R.Params.at(i).at(j);
			if ( j == 2 ) width = R.Params.at(i).at(j);
		}
		f2.SetRange(xlow, xhi);

		stringstream ss2; ss2.str("");
		ss2 << base_name << "_guess_" << R.Options.at(i);
		
		// printFitInfo(h, xlow, xhi, P.ConeAngle, width, N, ss2.str());

		ss2 << ".pdf";
		string filename = ss2.str();
		string histitle_part1 = appendStrings("Fit: ", P.name);
		string histitle_part2 = appendStrings(" as ", R.Options.at(i));
		h.SetTitle(appendStrings(histitle_part1, histitle_part2).c_str());
		h.Draw();
		f2.Draw("same");

		TLatex T1;
		T1.SetTextAlign(23);
		T1.SetTextSize(0.03);
		// T1.DrawLatex(2, latmax*h.GetMaximum(), 
		//              appendStrings("Particle = ", P.name).c_str()); latmax -= .05;
		// T1.DrawLatex(2, latmax*h.GetMaximum(), 
		//              appendStrings("Guess = ", R.Options.at(i)).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(), 
		             stringDouble("Sigma = ", R.Sigmas.at(i)).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(), 
		             stringDouble("Eta = ", P.Eta).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(), 
		             stringDouble("pt = ", P.pt).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(), 
		             stringDouble("Expected Photons = ", N).c_str()); latmax -= .05;
		double IncidentTheta = P.Theta/TMath::Pi();
		double IncidentPhi = P.Phi/TMath::Pi();
		string IncidentAngleStr = appendStrings(stringDouble("Incident #theta, #phi = ", IncidentTheta), "#pi, ");
		T1.DrawLatex(2, latmax*h.GetMaximum(), appendStrings(stringDouble(IncidentAngleStr, IncidentPhi), "#pi").c_str()); latmax -= .05;
		C.Print(filename.c_str());
		C.Clear();
	}

}