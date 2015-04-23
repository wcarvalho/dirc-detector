#include <unordered_map>

void GetPhotonSet(int const i, int const nphotons, vector<int>& set){
	set.at(0) = i;
	set.at(1) = 3*i + nphotons;
	set.at(2) = 3*i + nphotons + 1;
	set.at(3) = 3*i + nphotons + 2;
}

int GetBin(double const minimum, double const value, double const binsize){
	return (int)((value - minimum)/binsize);
}
double GetBinCenter(double const minimum, int const bin, double const binsize){
	return minimum + bin*binsize + binsize*.5;
}

void setUpGraphs(vector<TGraph*>& graphs, int markerstyle, int markersize){
	int i = 1;
	for (auto& graph: graphs){
		graph = new TGraph;
		graph->SetMarkerStyle(markerstyle);
		graph->SetMarkerSize(markersize);
		if (i == 5) ++i;
		graph->SetMarkerColor(i);
		graph->SetLineColor(i);
		++i;
	}
}

void plotPhotonSet(vector<int> const& set, vector<PhotonOut> const& photons, TGraph& graph, int& point){
	for (auto& index: set){
		auto& photon = photons.at(index);
		double const& x = photon.Phi;
		double const& y = photon.Theta;
		graph.SetPoint(point, x, y);
		++point;
	}
}
void create2DTimeProjection(TPad& pad, TMultiGraph*& mg, TLegend& L, vector<PhotonOut> const& photons, vector<Photon> const& cheat_photons, int particle_index, double min = 0., double max = 20.){

	int nbins = 10;


	double time_min = min/10.;
	double time_max = max/10.;
	double t_default_max = time_max;
	// for (auto& photon: cheat_photons){
	// 	double t = photon.Time_Traveled;
	// 	if (t > t_default_max) continue;
	// 	if (t > time_max) time_max = t;
	// 	if (t < time_min) time_min = t;
	// }

	double time_length = time_max - time_min;
	double time_step = (time_length)/nbins;


	vector<int> photonset; photonset.resize(4, 0);
	int n_photons_original = cheat_photons.size();
	// cout << "time_min = " <<std::setprecision(3)<< time_min << endl;
	// cout << "time_max = " <<std::setprecision(3)<< time_max << endl;
	// cout << "bin_size = " <<std::setprecision(3)<< time_step << endl;
	// exit(1);

	// each graph corresponds to a bin with its own color
	static vector<TGraph*> graphs; graphs.clear(); graphs.resize(nbins+1);
	double markersize = .5;
	int markerstyle = 21;
	setUpGraphs(graphs, markerstyle, markersize);

	vector<int> npoints; npoints.resize(nbins+1, 0);

	for (int i = 0; i < n_photons_original; ++i){
		GetPhotonSet(i, n_photons_original, photonset);
		auto& photon = cheat_photons.at(i);
		static int call; call = 0;
		double t = photon.Time_Traveled;

		static int t_bin = 0;

		if (t > t_default_max){
			t_bin = nbins;
		}
		else{
			t_bin = GetBin(time_min, t, time_step);
			if (t_bin == nbins) --t_bin;

		}
		auto& graph = graphs.at(t_bin);
		int& point = npoints.at(t_bin);

		plotPhotonSet(photonset, photons, *graph, point);

	}

		// time_bins[i] = i;
	unordered_map<int, int> time_bins;
	int key = 0;
	int value = 0;
	int counter = 0;
	for (unsigned i = 0; i < npoints.size(); ++i){
		int& points = npoints.at(i);
		if (points == 0){
			graphs.erase(graphs.begin() + i);
			npoints.erase(npoints.begin() + i);
			--i;
			--key;
			// cout << "empty " << counter << endl;
		}
		time_bins[key] = value;
		// cout << "key " << key << " value " << value << endl;
		++key;
		++value;
		++counter;
	}
	// exit(1);
	int i = 1;
	for (auto& graph : graphs){
		if (i == 5) ++i;
		if (i == 9) i += 2;
		graph->SetMarkerColor(i);
		graph->SetLineColor(i);
		++i;
	}
	// cout << "\terased empty graphs\n";

	stringstream ss;
	for (unsigned i = 0; i < (graphs.size() - 1); ++i){
		auto& graph = graphs.at(i);
		int bin = time_bins[i];
		double time_center = GetBinCenter(time_min, bin, time_step);
		ss.str(""); ss <<std::setprecision(5)<< 10.*(time_center - .5*time_step) << " : " <<std::setprecision(5)<< 10.*(time_center + .5*time_step);
		string entry = ss.str();
		L.AddEntry(graph, entry.c_str(), "lp");
	}
	int last=time_bins[graphs.size()-1];
	double time_center = GetBinCenter(time_min, last, time_step);
	ss.str(""); ss << " > " << std::setprecision(5)<< 10.*(time_center - .5*time_step);
	L.AddEntry(graphs.back(), ss.str().c_str(), "lp");

	for (auto& graph: graphs)
		mg->Add(graph);

	mg->SetTitle("Photon Time Projection; #phi (radians); #theta (radians)");
	pad.cd();
	mg->Draw("AP");

	pad.Update();
	pad.Modified();

}