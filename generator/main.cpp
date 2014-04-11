//====================================================
//
//                GENERATOR
//
//====================================================

#include "../headers/generator.h"

int main()
{
  int input = 4;
  
  double pi = TMath::Pi();
  string Output;

  Detector detector(10.0, 1.0, 1.0);
  vector<PhotonEvent> photon_events;											// vector of vector of photons; photon event is vector of photons
  vector<ParticleEvent> particle_events;									// vector of vector of particles; photon event is vector of photons
  ParticleEvent default_ParticleEvent;

  GeneratorOut event_output;
  TGraph rest(1);
  TGraph focus(1);
	TGraph* g1 = &focus;
	TGraph* g2 = &rest;

  photon_events.clear();
  particle_events.clear();
  
  TFile file("../../dirc_events.root", "recreate");
  TTree* tree = new TTree("events", "a tree of collision events");
	
	tree->Branch("event_output", &event_output);
	tree->Branch("focus", &focus);
	tree->Branch("rest", &rest);

  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------
  
  Output = "no";

  find_critical_angle(detector.CriticalAngle, Output);                                      // get the angle to implement total internal reflection
  detector.Checker = -1;

  Fill_ParticleEvent(detector, default_ParticleEvent, 1, input, Output);
	default_ParticleEvent.Particles[0].NumberofPhotons = 1000;
	default_ParticleEvent.Particles[0].ConeAngle = .8;
	default_ParticleEvent.NumberofParticles = 4;


 //  GenerateEvent(detector, particle_events, photon_events, 
 //                1, .8,
 //                default_ParticleEvent.Particles[0].X, default_ParticleEvent.Particles[0].Y,
 //                2, default_ParticleEvent.Particles[0].NumberofPhotons, 5,
 //                tree, g1, g2, input, Output);

  ScanTheta(detector, particle_events, photon_events,
	    default_ParticleEvent, event_output,
	    0, 1, 1.0/20,
	    tree, g1, g2, input, Output);


  // ScanX(detector, particle_events, photon_events,
	 //    default_ParticleEvent, event_output,
	 //    .1*detector.Length, .9*detector.Length, .8*detector.Length/5,
	 //    tree, g1, g2, input, Output);

  // ScanY(detector, particle_events, photon_events,
	 //    default_ParticleEvent, event_output,
	 //    .1*detector.Width, .9*detector.Width, .8*detector.Width/5,
	 //    tree, g1, g2, input, Output);

  // ScanPhi(detector, particle_events, photon_events,
	 //    default_ParticleEvent, event_output,
	 //    0, 3, 3./5,
	 //    tree, g1, g2, input, Output);

  file.Write();
  file.Close();

  return 0;
}