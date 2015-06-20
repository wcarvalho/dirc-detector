#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "dircTH1D.h"
#include "TError.h"

#include "dirc_objects.h"
#include "event_parsers.h"
#include "utility_library.h"
#include "calibration_data.h"
#include "calibration_functions.h"

using namespace std;

double center_of_mass(TH1D const& h);

void get_calibration_data(TTree &t1, TTree &t2, ParticleEvent &originals, TrackRecons& reconstructions, const double& percent, int nbins, string const &graphfile, string const &calibrationfile, double pStep_setting, const bool& calibrateArea, const bool& calibrateTheta, const bool& print){

  int nentries = t1.GetEntries();

  gErrorIgnoreLevel = 5000;         // turn off all root printing

  double pLowerBound = 1e10;
  double pHigherBound = 0.;
  auto findPBounds = [&pLowerBound, &pHigherBound](TrackRecon& r, Particle& p, bool print){
    double momentum = p.CalculateMomentum();
    if (momentum > pHigherBound) pHigherBound = momentum;
    if (momentum < pLowerBound) pLowerBound = momentum;
  };
  dirc::parseTrials(t1, t2, originals, reconstructions, 0, nentries,
    findPBounds, dirc::true_trialcondition, false);

  if (print) cout << "lowest momentum = " << pLowerBound << endl;
  if (print) cout << "highest momentum = " << pHigherBound << endl;

  double pStep = pStep_setting;
  pLowerBound = getLowerBound(pLowerBound, pStep);
  pHigherBound = getHigherBound(pHigherBound, pStep);

  int upper_index=getNormalizedIndex(pHigherBound, pLowerBound, pStep);
  typedef std::vector< std::vector<double> > vec2d;
  vec2d nphoton_data;
  vec2d theta_data;
  for (unsigned i = 0; i < upper_index; ++i){
    static vector<double> temp_vector;
    nphoton_data.push_back(temp_vector);
    theta_data.push_back(temp_vector);
  }

  auto seperateData = [&nphoton_data, &theta_data, &pLowerBound, &pStep](TrackRecon& recon, Particle& par, bool print){

    for (unsigned int i = 0; i < recon.size(); ++i){
      std::string name = recon.Options.at(i);
      if (name != par.name) continue;
      int index = getNormalizedIndex(par.CalculateMomentum(), pLowerBound, pStep);      // index that the momentum will belong to
      nphoton_data.at(index).push_back(recon.getnSigmaAreaAt(i));
      theta_data.at(index).push_back(recon.getnSigmaThetaAt(i));
    }
  };

  dirc::parseTrials(t1, t2, originals, reconstructions, 0, nentries,
    seperateData, dirc::true_trialcondition, false);

  auto histname = [](std::string& name, double& pLow, double&pHi){
    stringstream ss; ss.str("");
    ss << name << ", p = " << pLow << " : " << pHi;
    return ss.str();
  };


  typedef vector< pair < double, double > > vec_pair;

  bool print_details = false;
  auto generate_histogram = [&nbins, &histname, &pStep, &print, &percent, &graphfile, &print_details, &pLowerBound](string key, vector<double>& data, vec_pair& sigmas, vector<double>& percentAchieved, unsigned& index){
    static double lowerbound; lowerbound=getLowerBoundFromIndex(pLowerBound, index, pStep);
    static double higherbound; higherbound=getHigherBoundFromIndex(pLowerBound, index, pStep);
    if (print) cout << key << ", generating for: " << lowerbound << " : " << higherbound << endl;
    std::string name = histname(key, lowerbound, higherbound);
    static double min = 0., max = 0.;
    if (data.size() == 0) return;
    for (unsigned j = 0; j < data.size(); ++j){
      if (data.at(j) > 25){
        data.erase(data.begin() + j);
        --j;
      }
    }
    dirc::vectorminmax(data, min, max);
    // cout << "min = " << min << endl;
    // cout << "max = " << max << endl;
    dirc::dircTH1D h(name.c_str(), name.c_str(), nbins, min, max);
    for (unsigned j = 0; j < data.size(); ++j){
      double const& _d=data.at(j);
      h.Fill(_d);
      // cout << "\t" << j << ": " << _d << endl;
    }

    static stringstream ss; ss.str(""); ss << key << "_" << lowerbound << "_" << higherbound << ".root";
    h.SaveAs(ss.str().c_str());
    static TH1D h_copy;
    h_copy = h;
    h_copy.Draw();
    h.defineDistributionRange(min, max);

    static double center_guess = 0., sigma_guess = 0.;
    // h.FindDistributionCenter(center_guess, sigma_guess);

    // static double mean; mean = h.GetMean();
    static double com; com = center_of_mass(h);
    // cout << "center from mean = " << mean << endl;
    // cout << "center from com  = " << com << endl;
    center_guess = com;
    sigma_guess = sqrt( h.GetBinContent(h.GetMaximumBin()) );
    // if (key == "theta") { center_guess = 0.; sigma_guess = .01; }
    double percent_achieved = h.defineSigma(center_guess, sigma_guess, percent, print_details);
    percentAchieved.push_back(percent_achieved);
    if (print) cout << key << ": percent_achieved = " << percent_achieved << " vs. " << percent << endl;
    if (print) cout << "\tCenter = " << h.distributionCenter << endl;
    if (print) cout << "\tWidth = " << h.distributionSigma << endl;
    h.RemovePastSigma(print_details);
    h.SetFillColor(kRed);
    h.Draw("same");
    if (index == 0) system( dirc::appendStrings("rm ", graphfile).c_str() );
    TFile f(graphfile.c_str(), "update");
    h_copy.Write();
    h.Write();
    f.Close();
    pair < double, double > center_sigma(h.distributionCenter, h.distributionSigma);
    sigmas.push_back(center_sigma);
  };

  calibration_data cal_data(percent);
  for (unsigned i = 0; i < upper_index; ++i){
    vector<double>& photon_data_i = nphoton_data.at(i);
    generate_histogram("photons", photon_data_i, cal_data.NPhotonsCenterAndWidth, cal_data.NPhotonsPercentAchieved, i);

    vector<double>& theta_data_i = theta_data.at(i);
    generate_histogram("thetas", theta_data_i, cal_data.ThetaCenterAndWidth, cal_data.ThetaPercentAchieved, i);
  }

  cal_data.low=pLowerBound;
  cal_data.step=pStep;
  TFile f(calibrationfile.c_str(), "update");
  cal_data.WriteAsPercent();
  f.Close();
}

double center_of_mass(TH1D const& h){
  static double weighted_mass; weighted_mass = 0;
  static double total_mass; total_mass = 0;

  for (unsigned i = 0; i < h.GetNbinsX(); ++i){
    static double bin_center; bin_center = h.GetBinCenter(i);
    static double bin_content; bin_content = h.GetBinContent(i);
    weighted_mass += bin_center*bin_content;
    total_mass += bin_content;
  }
  static double center; center = weighted_mass/total_mass;
  return center;
}