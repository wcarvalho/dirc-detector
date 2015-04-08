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
   * @param preE1              [description]
   * @param preEC1              [description]
   * @param P2              [description]
   * @param C2              [description]
   */
  template < typename preEventParser, typename preEventParserCondition, typename trialParser, typename trialParserCondition, typename postEventParser, typename postEventParserCondition >
  void parseEvents(TTree &t1, TTree &t2,
                  ParticleEvent &originals, TrackRecons& reconstructions,
                  int start, int end,
                  preEventParser preE, preEventParserCondition preEC,
                  trialParser T, trialParserCondition C,
                  postEventParser postE, postEventParserCondition postEC,
                  bool print = false){
    for (unsigned ev = start; ev < end; ++ev){
      if (print) std::cout << "Event " << ev << std::endl;
      t1.GetEntry(ev); t2.GetEntry(ev);
      auto &pars   = originals.Particles;
      auto &recons = reconstructions.Recon;

      if (preEC(t1, t2, recons, pars, print)){
        preE(t1, t2, recons, pars, print);
      }

      for (unsigned p = 0; p < pars.size(); ++p){
        if (print) std::cout << "\tParticle " << p << std::endl;
        Particle& par     = pars.at(p);
        TrackRecon& recon = recons.at(p);
        if (!(C(recon, par, print))) continue;
        T(recon, par, print);
      }
      if (!(postEC(t1, t2, recons, pars, print))) continue;
      postE(t1, t2, recons, pars, print);
    }
  }

  void empty_eventparser(TTree &t1, TTree &t2, std::vector<TrackRecon>& recons,std::vector<Particle>& pars, bool print){};
  bool true_eventcondition(TTree &t1, TTree &t2, std::vector<TrackRecon>& recons,std::vector<Particle>& pars, bool print){ return true; };
  bool false_eventcondition(TTree &t1, TTree &t2, std::vector<TrackRecon>& recons,std::vector<Particle>& pars, bool print){ return false; };

  void empty_trialparser(TrackRecon& r, Particle& p, bool print){};
  bool true_trialcondition(TrackRecon& r, Particle& p, bool print){ return true; }
  bool false_trialcondition(TrackRecon& r, Particle& p, bool print){ return false; }

  void matchDataSize(TTree &t1, TTree &t2, std::vector<TrackRecon>& recons,std::vector<Particle>& pars, bool print){

    //sizes of a and b
    int sa = recons.size();
    int sb = pars.size();
    int dif = fabs(sb-sa);

    if (sb>sa){
      for (unsigned int i = 0; i < dif; ++i)
        pars.erase(pars.begin());
    }
    else{
      for (unsigned int i = 0; i < dif; ++i)
        recons.erase(recons.begin());
    }
  }
  template < typename trialParser, typename trialParserCondition >
  void parseTrials(TTree &t1, TTree &t2,
                  ParticleEvent &originals, TrackRecons& reconstructions,
                  int start, int end,
                  trialParser T, trialParserCondition C,
                  bool print = false){

    parseEvents(t1, t2, originals, reconstructions, start, end,
      empty_eventparser, false_eventcondition,
      T, C,
      empty_eventparser, false_eventcondition,
      print);
  }
}