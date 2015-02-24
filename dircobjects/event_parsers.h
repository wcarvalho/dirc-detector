#include "dirc_objects.h"
#include "particle_objects.h"
#include "TTree.h"
#include <vector>

namespace dirc{
  /**
   * [parseEvents description]
   * @param t1              [description]
   * @param t2              [description]
   * @param originals       [description]
   * @param reconstructions [description]
   * @param start           [description]
   * @param end             [description]
   * @param P1              [description]
   * @param C1              [description]
   * @param P2              [description]
   * @param C2              [description]
   */
  template < typename eventParser, typename eventParserCondition, typename trialParser, typename trialParserCondition >
  void parseEvents(TTree &t1, TTree &t2,
                  ParticleEvent &originals, TrackRecons& reconstructions,
                  int start, int end,
                  eventParser P1, eventParserCondition C1,
                  trialParser P2, trialParserCondition C2, bool print = false){

    for (unsigned ev = start; ev < end; ++ev){
      t1.GetEntry(ev); t2.GetEntry(ev);
      auto &pars   = originals.Particles;
      auto &recons = reconstructions.Recon;

      if (recons.size() == 0) continue;
      for (unsigned int p = 0; p < pars.size(); ++p){
        Particle& par     = pars.at(p);
        TrackRecon& recon = recons.at(p);
        if (!(C2(recon, par))) continue;
        P2(recon, par);
      }
      if (!(C1(t1, t2, recons, pars))) continue;
      P1(t1, t2, recons, pars);
    }
  }

  void empty_eventparser(TTree &t1, TTree &t2, std::vector<TrackRecon>& recons,std::vector<Particle>& pars){};
  bool true_eventcondition(TTree &t1, TTree &t2, std::vector<TrackRecon>& recons,std::vector<Particle>& pars){ return true; };
  bool false_eventcondition(TTree &t1, TTree &t2, std::vector<TrackRecon>& recons,std::vector<Particle>& pars){ return false; };

  void empty_trialparser(TrackRecon& r, Particle& p){};
  bool true_trialcondition(TrackRecon& r, Particle& p){ return true; }
  bool false_trialcondition(TrackRecon& r, Particle& p){ return false; }


  /**
   * [reconstructionParser description]
   * @param recons    [description]
   * @param parse     [description]
   * @param condition [description]
   */
  // template<typename Parser, typename ContinueCondition>
  // void reconstructionParser(TrackRecons recons, Parser parse, ContinueCondition condition){
  //   for (unsigned i = 0; i < recons.size(); ++i){
  //     auto& recon = recons.at(i);
  //     for (unsigned j = 0; j< recon.Options.size(); ++j){
  //       if (condition(recon)) continue;
  //       parse(recon);
  //     }
  //   }
  // }
}