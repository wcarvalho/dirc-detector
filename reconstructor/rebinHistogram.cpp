#include "reconstructor.h"

TH1D rebinHistogram(TH1D& h, double minimum){

	//regular variables
	static double bincount; bincount = 0;
	static vector<double> lower_edges; lower_edges.clear();

	bool newbin = true;
	unsigned last_bin = h.GetNbinsX() - 1;
	for (unsigned i = 0; i < h.GetNbinsX(); ++i){

		if (newbin){
			lower_edges.push_back(h.GetBinLowEdge(i));
			newbin = false;
		}

		bincount += h.GetBinContent(i);

		if (bincount >= minimum){
			bincount = 0;
			newbin = true;
		}

		if (i == last_bin ){
			if (bincount < minimum)	lower_edges.pop_back();
		}
	}

	double upper_edge =h.GetXaxis()->GetXmax();
	lower_edges.push_back(upper_edge);

	string histogram_name = h.GetName(); histogram_name.append("_rebinned");
	int nbins = lower_edges.size() - 1;
	double* lower_edges_p = &lower_edges[0]; // pointer for double[], type required by root function

	TH1D* h_rebinned = (TH1D*)h.Rebin(nbins, histogram_name.c_str(), lower_edges_p);

	// h_rebinned->SaveAs("post_rebinning.root");

	return *h_rebinned;
	// static vector<double> binvalues; binvalues.clear();
	// static
	// 	binvalues.push_back(h.GetBinContent());


	// h.Rebin();
}
