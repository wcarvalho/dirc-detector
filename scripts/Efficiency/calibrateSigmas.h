#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "graphing.h"

#include "dirc_objects.h"
#include "utility_library.h"

using namespace std;

template < typename eventParser, typename eventParserCondition,
          typename trialParser, typename trialParserCondition >
void parseEvents(TTree &t1, TTree &t2,
                ParticleEvent &originals, TrackRecons& reconstructions,
                int start, int end,
                eventParser P1, eventParserCondition C1,
                trialParser P2, trialParserCondition C2){

  for (unsigned ev = start; ev < end; ++ev){
    t1.GetEntry(ev); t2.GetEntry(ev);

    auto &pars   = originals.Particles;
    auto &recons = reconstructions.Recon;

    if (recons.size() == 0) continue;
    for (unsigned int p = 0; p < pars.size(); ++p){
      Particle& par   = pars.at(p);
      TrackRecon& recon = recons.at(p);
      if (!(C2(recon, par))) continue;
      P2(recon, par);
    }
    if (!(C1(t1, t2, recons, pars))) continue;
    P1(t1, t2, recons, pars);
  }
}

void calibrateSigmas(TCanvas &C, TTree &t1, TTree &t2, TTree &t1prime, TTree &t2prime, ParticleEvent &originals, TrackRecons& reconstructions, const std::string& particletype, double percent, int nbins, string const &prefix, int &startevent, bool print){

  double pBounds[2];
  pBounds[0] = 0;
  double pStepSize = .5;
  pBounds[1] = pBounds[0] + pStepSize;

  auto parseConditon = [&pBounds](TrackRecon& r, Particle& p){
    return !( (p.pt < pBounds[0]) || (p.pt > pBounds[1]) );
  };

  std::map < std::string, std::vector< std::vector<double> > > datamap;
  auto generateDistribution = [&datamap, &pBounds](TrackRecon& recon, Particle& par){
    auto expected_theta = par.EmissionAngleMap()[par.name];
    for (unsigned int i = 0; i < recon.Options.size(); ++i){
      std::string name = recon.Options.at(i);
      if (name == par.name){
        double sigmatheta = recon.delSigTheta.at(i);
        double sigmaArea = recon.delSigArea.at(i);
        datamap["photons"].back().push_back(sigmaArea);
        datamap["theta"].back().push_back(sigmatheta);
      }
    }
  };

  auto histname = [&pBounds](std::string& name){
    stringstream ss; ss.str("");
    ss << "_pt_" << pBounds[0] << "_" << pBounds[1];
    return std::move(wul::appendStrings(name, ss.str()));
  };

  auto doNothing_trial   = [] (TrackRecon& r, Particle& p){ };
  auto returnfalse_trial = [] (TrackRecon& r, Particle& p) { return false; };

  auto doNothing_event   = [] (TTree &t1, TTree &t2, decltype(reconstructions.Recon)& recons, decltype(originals.Particles)& par){ };
  auto returnfalse_event = [] (TTree &t1, TTree &t2, decltype(reconstructions.Recon)& recons, decltype(originals.Particles)& pars) { return false; };


  ////////////////////////////
  // Analyze && Callibrate //
  ////////////////////////////
  int nentries = t1.GetEntries();
  vector< double > new_list;
  map < string, vector< pair < double, double > > > sigmas;
  while (pBounds[1] <= 3.){
    if (print) cout << "generating for range: " << pBounds[0] << ", " << pBounds[1] << endl;
    auto& photons_data = datamap["photons"];
    auto& theta_data = datamap["theta"];
    photons_data.push_back(new_list);
    theta_data.push_back(new_list);

    // generate distributions
    parseEvents(t1, t2,
      originals, reconstructions,
      0, nentries,
      doNothing_event, returnfalse_event,
      generateDistribution, parseConditon);

    for(auto i = datamap.begin(); i != datamap.end(); ++i){
      static int nbins0 = nbins;
      nbins = nbins0;
      string key = i->first;
      if (key == "theta") nbins = 200;
      vector< double > &latest_data = i->second.back();
      std::string name = histname(key);
      static double min = 0., max = 0.;
      if (i->second.back().size() == 0) continue;
      wul::vectorminmax(i->second.back(), min, max);
      if (key == "theta") {
        min = -.15; max = .15;
      }
      dirc::dircTH1D h(name.c_str(), name.c_str(), nbins, min, max);
      for (unsigned j = 0; j < latest_data.size(); ++j){
        h.Fill(latest_data.at(j));
      }
      TH1D h_copy = h;
      h_copy.Draw();
      h.defineDistributionRange(min, max);

      static double center_guess = 0., sigma_guess = 0.;
      h.FindDistributionCenter(center_guess, sigma_guess);
      if (key == "theta") { center_guess = 0.; sigma_guess = .01; }
      double percent_achieved = h.defineSigma(center_guess, sigma_guess, percent, false);
      if (print) cout << key << ": percent_achieved = " << percent_achieved << " vs. " << percent << endl;
      h.RemovePastSigma(false);
      h.SetFillColor(kRed);
      h.Draw("same");
      // TFile f(name.append(".root").c_str(), "recreate");
      // h_copy.Write();
      // h.Write();
      // f.Close();
      // C.Print(name.append(".pdf").c_str());
      C.Clear();
      pair < double, double > center_sigma(h.distributionCenter, h.distributionSigma);
      sigmas[key].push_back(center_sigma);
    }

    for (unsigned i = 0; i < 2; ++i){
      pBounds[i] += pStepSize;
    }
  }
  auto fill = [&t1prime, &t2prime](TTree &t1, TTree &t2, decltype(reconstructions.Recon)& recons, decltype(originals.Particles)& pars){
    t1prime.Fill();
    t2prime.Fill();
  };
  auto calibrate = [&sigmas, particletype, &pStepSize](TrackRecon& recon, Particle& par){
    auto Round = [](const double &num, int by){ return (float)((int)(num*by))/by; };

    double pt = par.pt;
    int index = Round(pt/pStepSize, 1);

    static double photons_center;
    static double theta_center;
    photons_center = sigmas["photons"].at(index).first;
    theta_center = sigmas["theta"].at(index).first;

    static double photons_sigma;
    static double theta_sigma;
    photons_sigma = sigmas["photons"].at(index).second;
    theta_sigma = sigmas["theta"].at(index).second;

    auto expected_theta = par.EmissionAngleMap()[particletype];
    for (unsigned int i = 0; i < recon.Options.size(); ++i){
      std::string &name = recon.Options.at(i);
      if (name == particletype){
        // recon.delSigTheta.at(i) = (theta_center - recon.delSigTheta.at(i))/theta_sigma;

        // std::cout << "recon.delSigArea.at(i) = ";
        // cout << "("<< recon.delSigArea.at(i) << " - " << photons_center << ")/" << photons_sigma << " =\n\t";
        recon.delSigArea.at(i)  = (recon.delSigArea.at(i) - photons_center)/photons_sigma;
        // cout << recon.delSigArea.at(i) << std::endl;
        // std::cout << "recon.delSigTheta.at(i) = " << recon.delSigTheta.at(i) << std::endl;
      }
    }
  };
  // calibrate && fill events
  parseEvents(t1, t2, originals, reconstructions, 0, nentries,
    fill,
    [](TTree &t1, TTree &t2, decltype(reconstructions.Recon)& recons, decltype(originals.Particles)&) { return true; },
    calibrate,
    [] (TrackRecon& r, Particle& p) { return true; });

}
