// prints histograms of fits
void printfits(TCanvas &C, int Event, int par, Particle &P, TrackRecon &R, string const dir, bool print){

	stringstream ss; ss.str("");
	ss << dir << "Event_" << setfill('0') <<setw(3) << Event << "_";
	ss << "Particle_" << setfill('0') <<setw(3) << par << "_";
	ss << P.name;

	string base_name = ss.str();

	stringstream ss2; ss2.str("");
	ss2 << dir << P.name << "_guesses.root";
	string filename = ss2.str();
	TF1 f2("hello", "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) ) + [3]");
	TH1D &h = R.Hist;

	auto expected_angle_map = P.EmissionAngleMap();
	for (unsigned int i = 0; i < R.Params.size(); ++i){
		double latmax = 1;
		double xlow = R.Params.at(i).at(4);
		double xhi = R.Params.at(i).at(5);
		static double center = 0;
		static double width = 0;
		double &N = R.ExpectedNumber.at(i);
		double &Area = R.Areas.at(i);

		for (unsigned int j = 0; j < 4; j++){
			f2.SetParameter(j, R.Params.at(i).at(j));
			if ( j == 1 ) center = R.Params.at(i).at(j);
			if ( j == 2 ) width = R.Params.at(i).at(j);
		}
		f2.SetRange(xlow, xhi);


		// printFitInfo(h, xlow, xhi, P.ConeAngle, width, N, ss2.str());


		string histitle_part1 = wul::appendStrings("Fit: ", P.name);
		string histitle_part2 = wul::appendStrings(" as ", R.Options.at(i));
		h.SetTitle(wul::appendStrings(histitle_part1, histitle_part2).c_str());
		// h.GetXaxis()->SetRangeUser(center-.25,center+.25);
		h.Draw();
		f2.Draw("same");

		TLatex T1;
		T1.SetTextAlign(23);
		T1.SetTextSize(0.03);
		// T1.DrawLatex(2, latmax*h.GetMaximum(),
		//              wul::appendStrings("Particle = ", P.name).c_str()); latmax -= .05;
		// T1.DrawLatex(2, latmax*h.GetMaximum(),
		//              wul::appendStrings("Guess = ", R.Options.at(i)).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("Sigma = ", R.Sigmas.at(i)).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("Eta = ", P.Eta).c_str()); latmax -= .05;

		double m = P.m;
		double beta = P.Beta;
		double momentum = sqrt((m*beta)*(m*beta)/(1-beta*beta));
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("p = ", momentum).c_str()); latmax -= .05;
		double expected_angle = expected_angle_map[R.Options.at(i)];
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("expected emission angle = ", expected_angle).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("found emission angle = ", center).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("Expected Photons = ", N).c_str()); latmax -= .05;
		T1.DrawLatex(2, latmax*h.GetMaximum(),
		             wul::stringDouble("Expected From Fit = ", Area).c_str()); latmax -= .05;
		double IncidentTheta = P.Theta/TMath::Pi();
		double IncidentPhi = P.Phi/TMath::Pi();
		string IncidentAngleStr = wul::appendStrings(wul::stringDouble("Incident #theta, #phi = ", IncidentTheta), "#pi, ");
		T1.DrawLatex(2, latmax*h.GetMaximum(), wul::appendStrings(wul::stringDouble(IncidentAngleStr, IncidentPhi), "#pi").c_str()); latmax -= .05;

		TFile f(filename.c_str(), "update");
		C.Write(filename.c_str());
		f.Close();
		// cout << "filename: " << filename << endl;

		C.Clear();
	}

}