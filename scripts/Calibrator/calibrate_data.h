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
// #include "utility_library.h"
#include "calibration_functions.h"
#include "event_parsers.h"

void calibrateSigmas(TTree &t1, TTree &t2, TTree &t2prime, ParticleEvent &originals, TrackRecons& reconstructions, calibration_data* const& cal_data, vector<string>& calibrationTypes, const bool& calibrateArea, const bool& calibrateTheta, const bool& print){

  auto applyCalibration = [&calibrateArea, &calibrateTheta, &print](TrackRecon& recon, int index, double photons_center, double photons_sigma, double theta_center, double theta_sigma){
    if (calibrateArea){
      recon.delSigArea.at(index)  = (recon.delSigArea.at(index) - photons_center)/photons_sigma;
      recon.SigArea.at(index) = (recon.getExpectedIntensityAt(index) - recon.getIntegralAt(index))/recon.delSigArea.at(index);
    }



    if (calibrateTheta){
      recon.delSigTheta.at(index)  = (recon.delSigTheta.at(index) - theta_center)/theta_sigma;
      recon.SigTheta.at(index) = (recon.getExpectedAngleAt(index) - recon.getIntegralCenterAt(index))/recon.delSigTheta.at(index);
    }
  };

  auto calibrate = [&cal_data, &applyCalibration, &calibrationTypes](TrackRecon& recon, Particle& par, bool print){

    double momentum = par.CalculateMomentum();
    const double& step = cal_data->step;
    const double& low = cal_data->low;
    int index = getNormalizedIndex(getLowerBound(momentum, step), low, step);
    static double photons_center;
    static double theta_center;
    try {
      photons_center = cal_data->NPhotonsCenterAndWidth.at(index).first;
      theta_center = cal_data->ThetaCenterAndWidth.at(index).first;
    }
    catch (const std::out_of_range& oor) {
      cout << "low = " << low << endl;
      cout << "momentum = " << momentum << endl;
      cout << "index = " << index << " but size = " << cal_data->NPhotonsCenterAndWidth.size() << endl;
      std::cerr << "Out of Range error: " << oor.what() << '\n';
    }

    static double photons_sigma;
    static double theta_sigma;
    photons_sigma = cal_data->NPhotonsCenterAndWidth.at(index).second;
    theta_sigma = cal_data->ThetaCenterAndWidth.at(index).second;

    for (unsigned i = 0; i < recon.Options.size(); ++i){
      std::string &name = recon.Options.at(i);
      if (calibrationTypes.size() == 0)
        applyCalibration(recon, i, photons_center, photons_sigma, theta_center, theta_sigma);
      else{
        if ( std::find(calibrationTypes.begin(), calibrationTypes.end(), name) == calibrationTypes.end() ) continue;
        else applyCalibration(recon, i, photons_center, photons_sigma, theta_center, theta_sigma);
      }
    }
  };

  auto fill = [&t2prime](TTree &t1, TTree &t2, decltype(reconstructions.Recon)& recons, decltype(originals.Particles)& pars, bool print){
    t2prime.Fill();
  };

  int nentries = t1.GetEntries();
  // calibrate && fill events
  dirc::parseEvents(t1, t2, originals, reconstructions, 0, nentries,
    dirc::matchDataSize, dirc::true_eventcondition,
    calibrate, dirc::true_trialcondition,
    fill, dirc::true_eventcondition,
    print);

}