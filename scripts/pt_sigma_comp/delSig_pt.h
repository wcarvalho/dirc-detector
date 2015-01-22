#include "TCanvas.h"
#include "TGraph.h"
#include "TH2D.h"
#include <vector>
#include <string>
#include <sstream>
#include "TLatex.h"

void make_hist(TCanvas &C, double lb, double hb, int nbinsx, int nbinsy, vector<double> const &x, vector<double> const &y, vector<double> const &marker, std::string filename, int filenum, string particletype){

	stringstream ss; ss.str(""); ss << filename << filenum << ".pdf";
	filename = ss.str();
	
	vector<double> xnew;
	vector<double> ynew;
	for (unsigned int i = 0; i < x.size(); ++i){
		if ((marker.at(i) > lb)&&(marker.at(i) < hb)){
			xnew.push_back(x.at(i));
			ynew.push_back(y.at(i));
		}

	}

	if (xnew.size() == 0) return;

	double minvalx = *min_element(xnew.begin(), xnew.end());
	double maxvalx = *max_element(xnew.begin(), xnew.end());
	double widthx = (maxvalx-minvalx);
	// minvalx -= widthx*.2;
	// maxvalx += widthx*.2;
	double centerx = minvalx+widthx*.5;

	double minvaly = (*min_element(ynew.begin(), ynew.end()));
	double maxvaly = (*max_element(ynew.begin(), ynew.end()));
	double widthy = (maxvaly-minvaly);
	double centery = minvaly+widthy*.5;
	// minvaly -= widthy*.2;
	// maxvalx += widthy*.2;

	TH2D g(filename.c_str(), "", nbinsx, minvalx, maxvalx, nbinsy, minvaly, maxvaly);

	for (unsigned int i = 0; i < xnew.size(); ++i){
		g.Fill(xnew.at(i), ynew.at(i));
	}

	int posb = filename.rfind("/") + 1;			// beginning position
	int posl = filename.find_first_of("_v")+1;			// end position
	string ytitle = filename.substr(posb, posl);


	g.Draw("colz");
  g.SetStats(0);
	g.GetXaxis()->SetTitle("momentum");
	g.GetYaxis()->SetTitle(ytitle.c_str());


	ss.str("");
	ss << particletype << ", momentum slice: " << lb << " - " << hb;
	string text = ss.str();

	// cout << "centerx = " << centerx << endl;
	TLatex T1;
	T1.SetTextAlign(15);
	T1.SetTextSize(0.03);
	T1.DrawLatexNDC(.6, .95, text.c_str());
	C.Print(filename.c_str());
	C.Clear();

}

// void make_graph(TCanvas &C, double lb, double hb, vector<double> const &x, vector<double> const &y, vector<double> const &marker, std::string filename){


// 	vector<int> goodpoints;
// 	for (unsigned int i = 0; i < x.size(); ++i){
// 		if ((marker.at(i) > lb)&&(marker.at(i) < hb))
// 			goodpoints.push_back(i);
// 	}

// 	int ngoodpoints = goodpoints.size();
// 	if (!(ngoodpoints)) return;

// 	TGraph g(ngoodpoints);

	
// 	for (unsigned int i = 0; i < ngoodpoints; ++i){
// 		int &indx = goodpoints.at(i);
// 		g.SetPoint(i, x.at(indx), y.at(indx));
// 	}

// 	int posb = filename.rfind("/") + 1;			// beginning position
// 	int posl = filename.find_first_of("_v")+1;			// end position
// 	string ytitle = filename.substr(posb, posl);


// 	g.Draw("AP");
// 	g.GetXaxis()->SetTitle("pt");
// 	g.GetYaxis()->SetTitle(ytitle.c_str());


// 	stringstream ss; ss.str("");
// 	ss << "momentum slice: " << lb << " - " << hb;
// 	string text = ss.str();

// 	TLatex T1;
// 	T1.SetTextAlign(4);
// 	T1.SetTextSize(0.03);
// 	T1.DrawLatex(1, .8*g.GetMaximum(), text.c_str());
// 	C.Print(filename.c_str());
// 	C.Clear();

// }