// fill a histogram with reconstructed data
void FillHist(TH1D &H, vector< pair<int, pair <Particle, TrackRecon> > > const & vec, int Case, vector< double > range, bool print){

	if (print) cout << "range = " << range[0] << ", " << range[1] << endl;
	for(unsigned int i = 0; i < vec.size(); ++i){

		const auto& P = vec.at(i).second.first;
		const int& multiplicity = vec.at(i).first;
		const double& m = P.m;
		const double& beta = P.Beta;
		double momentum = sqrt((m*beta)*(m*beta)/(1-beta*beta));
		const double& pt = P.pt;
		const double& IncidentTheta = P.Theta;
		const double& IncidentPhi = P.Phi;
		const double& EmissionAngle = P.ConeAngle;
		bool NotInPtRange = ((pt < range[0])||(pt > range[1]));
		bool NotInMomentumRange = ((momentum < range[0])||(momentum > range[1]));
		bool NotInMultiRange = ((multiplicity < range[0])||(multiplicity > range[1]));
		// cout << "NotInPtRange = " << NotInPtRange << endl;
		// cout << "NotInMultiRange = " << NotInMultiRange << endl;
		switch (Case) {
			case 1:	// vs. Multiplicity
				if (NotInMomentumRange)
					continue;
				H.Fill(multiplicity);
				break;
			case 2: // vs. Momentum
				if (NotInMultiRange)
					continue;
				H.Fill(momentum);
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






TGraphAsymmErrors graphASymm(TCanvas &c, double low, double hi, double nbins, vector< pair<int, pair <Particle, TrackRecon> > > const & Num, vector< pair<int, pair <Particle, TrackRecon> > > const & Den, int Case, vector< double > range, bool &makegraph, bool print, string graph_name){

	TH1D NumHist("Num", "Num", nbins, low, hi);
	string numfile = wul::appendStrings(graph_name, "_Num.root");

	TH1D DenHist("Den", "Den", nbins, low, hi);
	string denfile = wul::appendStrings(graph_name, "_Den.root");

	if (print) cout << "Numerator:\n";
	FillHist(NumHist, Num, Case, range, print);
	// TFile f1(numfile.c_str(), "recreate");
	// NumHist.Write();
	// f1.Close();

	if (print) cout << "Denominator:\n";
	FillHist(DenHist, Den, Case, range, print);
	// TFile f2(denfile.c_str(), "recreate");
	// DenHist.Write();
	// f2.Close();

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
	double max = graph.GetMaximum();
	graph.GetYaxis()->SetRangeUser(0.,max);

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

		string graph_name = GraphFileName("", "", matchgraph_filebase, xtitle,filenumber, "");
		string filename = GraphFileName(data_dir, graph_dir, matchgraph_filebase, xtitle, filenumber, ".pdf");
		string rootfilename = GraphFileName(data_dir, graph_dir, matchgraph_filebase, xtitle,filenumber, ".root");
		TGraphAsymmErrors match_graph = graphASymm(C, histlow, histhi, nbins, numMatch, denMatch, Case, bounds[i], makegraph, print, graph_name);
		match_graph.SetName(graph_name.c_str());
		GraphXYTitle(match_graph, "Efficiency for Identifying Electrons", xaxis.c_str(), "Efficiency");
		if (makegraph){
			cout << "filename: " << filename << endl;
			C.Print(filename.c_str());
			TFile f(rootfilename.c_str(), "recreate");
			match_graph.Write();
			f.Close();
		}
		C.Clear();
		makegraph = true;
		if (print) cout << "-----False----\n";
		graph_name = GraphFileName("", "", falsegraph_filebase, xtitle,filenumber, "");
		filename = GraphFileName(data_dir, graph_dir, falsegraph_filebase, xtitle,filenumber, ".pdf");
		rootfilename = GraphFileName(data_dir, graph_dir, falsegraph_filebase, xtitle,filenumber, ".root");
		TGraphAsymmErrors false_graph = graphASymm(C, histlow, histhi, nbins, numFalse, denFalse, Case, bounds[i], makegraph, print, graph_name);
		false_graph.SetName(graph_name.c_str());

		GraphXYTitle(false_graph, "False Positive for Identifying Electrons", xaxis.c_str(), "False Positive");

		if (makegraph){
			cout << "filename: " << filename << endl;
			C.Print(filename.c_str());
			TFile f(rootfilename.c_str(), "recreate");
			false_graph.Write();
			f.Close();
		}
		C.Clear();
				// exit(1);

		++filenumber;
	}
}