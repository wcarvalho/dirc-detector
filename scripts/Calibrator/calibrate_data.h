#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "dircTH1D.h"
#include "TMath.h"

#include "dirc_objects.h"
#include "event_parsers.h"
#include "utility_library.h"

using namespace std;

double round(const double& value, const double& step){
  return (TMath::Nint(value*(1/step))/double(1/step));
}

int getIndex(const double& value, const double& step){
  return (int)(value/step);
}

double getLowerBound(const double& value, const double& step){
    double x = round(value, step);
    int i = getIndex(x, step);
    if (x > value) --i;
    return i*step;
}

double getHigherBound(const double& value, const double& step){
    double x = round(value, step);
    int i = getIndex(x, step);
    if (x < value) ++i;
    return i*step;
}

void calibrateSigmas(TTree &t1, TTree &t2, TTree &t2prime, ParticleEvent &originals, TrackRecons& reconstructions, const double& percent, int nbins, string const &graphfile, double pStep_setting, bool calibrateArea, bool calibrateTheta, bool print){

  int nentries = t1.GetEntries();

  double pStart = 1e10;
  double pEnd = 0.;
  auto findmaxP = [&pStart, &pEnd](TrackRecon& r, Particle& p){
    double momentum = p.CalculateMomentum();
    if (momentum > pEnd) pEnd = momentum;
    if (momentum < pStart) pStart = momentum;
  };
  dirc::parseEvents(t1, t2, originals, reconstructions, 0, nentries,
    dirc::empty_eventparser, dirc::false_eventcondition,
    findmaxP, dirc::true_trialcondition);
  cout << "pStart = " << pStart << endl;
  cout << "pEnd = " << pEnd << endl;

  double pStep = pStep_setting;
  pStart = getLowerBound(pStart, pStep);
  pEnd = getHigherBound(pEnd, pStep);
  double pBounds[2];
  pBounds[0] = pStart;
  pBounds[1] = pBounds[0] + pStep;

  auto parseConditon = [&pBounds](TrackRecon& r, Particle& p){
    double momentum = p.CalculateMomentum();
    return !( (momentum < pBounds[0]) || (momentum > pBounds[1]) );
  };
  std::map < std::string, std::vector< std::vector<double> > > datamap;
  auto generateDistribution = [&datamap, &pBounds](TrackRecon& recon, Particle& par){
    for (unsigned int i = 0; i < recon.Options.size(); ++i){
      std::string name = recon.Options.at(i);
      if (name == par.name){
        datamap["photons"].back().push_back(recon.delSigArea.at(i));
        datamap["theta"].back().push_back(recon.delSigTheta.at(i));
      }
    }
  };

  auto histname = [&pBounds](std::string& name){
    stringstream ss; ss.str("");
    ss << ", p = " << pBounds[0] << " : " << pBounds[1];
    return std::move(wul::appendStrings(name, ss.str()));
  };


  ////////////////////////////
  // Analyze && Callibrate //
  ////////////////////////////
  vector< double > new_list;
  map < string, vector< pair < double, double > > > sigmas;
  while (pBounds[1] <= pEnd){
    if (print) cout << "generating for range: " << pBounds[0] << ", " << pBounds[1] << endl;
    auto& photons_data = datamap["photons"];
    auto& theta_data = datamap["theta"];
    photons_data.push_back(new_list);
    theta_data.push_back(new_list);

    // generate distributions
    dirc::parseEvents(t1, t2,
      originals, reconstructions,
      0, nentries,
      dirc::empty_eventparser, dirc::false_eventcondition,
      generateDistribution, parseConditon);

    for(auto i = datamap.begin(); i != datamap.end(); ++i){
      static int nbins0 = nbins;
      nbins = nbins0;
      string key = i->first;
      // if (key == "theta") nbins = 200;
      vector< double > &latest_data = i->second.back();
      std::string name = histname(key);
      static double min = 0., max = 0.;
      if (i->second.back().size() == 0) continue;
      wul::vectorminmax(i->second.back(), min, max);
      // if (key == "theta") { min = -.15; max = .15; }
      dirc::dircTH1D h(name.c_str(), name.c_str(), nbins, min, max);
      for (unsigned j = 0; j < latest_data.size(); ++j){
        h.Fill(latest_data.at(j));
      }
      TH1D h_copy = h;
      h_copy.Draw();
      h.defineDistributionRange(min, max);

      static double center_guess = 0., sigma_guess = 0.;
      // h.FindDistributionCenter(center_guess, sigma_guess);
      center_guess = h.GetMean();
      sigma_guess = sqrt( h.GetBinContent(h.GetMaximumBin()) );
      if (key == "theta") { center_guess = 0.; sigma_guess = .01; }
      double percent_achieved = h.defineSigma(center_guess, sigma_guess, percent);
      if (print) cout << key << ": percent_achieved = " << percent_achieved << " vs. " << percent << endl;
      h.RemovePastSigma(false);
      h.SetFillColor(kRed);
      h.Draw("same");
      TFile f(graphfile.c_str(), "update");
      h_copy.Write();
      h.Write();
      f.Close();
      // C.Print(name.append(".pdf").c_str());
      pair < double, double > center_sigma(h.distributionCenter, h.distributionSigma);
      sigmas[key].push_back(center_sigma);
    }

    for (unsigned i = 0; i < 2; ++i){
      pBounds[i] += pStep;
    }
  }
  auto fill = [&t2prime](TTree &t1, TTree &t2, decltype(reconstructions.Recon)& recons, decltype(originals.Particles)& pars){
    t2prime.Fill();
  };
  auto calibrate = [&sigmas, &pStep, &pStart, &pEnd, &print, calibrateArea, calibrateTheta](TrackRecon& recon, Particle& par){

    double momentum = par.CalculateMomentum();
    int index = getIndex(getLowerBound(momentum, pStep), pStep);
    int normalization_index = getIndex(pStart, pStep);
    index -= normalization_index;

    static double photons_center;
    static double theta_center;
    try {
      photons_center = sigmas["photons"].at(index).first;
      theta_center = sigmas["theta"].at(index).first;
    }
    catch (const std::out_of_range& oor) {
      cout << "pStart = " << pStart << endl;
      cout << "pEnd = " << pEnd << endl;
      cout << "momentum = " << momentum << endl;
      cout << "index = " << index << " but size = " << sigmas["photons"].size() << endl;
      std::cerr << "Out of Range error: " << oor.what() << '\n';
    }

    static double photons_sigma;
    static double theta_sigma;
    photons_sigma = sigmas["photons"].at(index).second;
    theta_sigma = sigmas["theta"].at(index).second;

    for (unsigned int i = 0; i < recon.Options.size(); ++i){
      std::string &name = recon.Options.at(i);
      if (calibrateArea) recon.delSigArea.at(i)  = (recon.delSigArea.at(i) - photons_center)/photons_sigma;
      if (calibrateTheta) recon.delSigTheta.at(i)  = (recon.delSigTheta.at(i) - theta_center)/theta_sigma;
    }
  };
  // calibrate && fill events
  dirc::parseEvents(t1, t2, originals, reconstructions, 0, nentries,
    fill, dirc::true_eventcondition,
    calibrate, dirc::true_trialcondition);

}