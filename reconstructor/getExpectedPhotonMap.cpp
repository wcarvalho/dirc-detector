#include "reconstructor.h"

void getExpectedPhotonMap(vector<ParticleOut> & pars, unordered_map <int, vec_pair>& expectedPhotonMap, std::pair<double, double> (*ExpectedNumberofPhotons)(double const&, double const&, double const&, double const&, double const&)){

	static std::map<std::string, double> massmap; massmap.clear();
	static std::map<std::string, double> anglemap; anglemap.clear();

  static vec_pair expectedNPhotons; expectedNPhotons.clear();
	for (unsigned i = 0; i < pars.size(); ++i){
		// cout << i << endl;
		auto& P = pars.at(i);
		massmap = P.MassMap();
		anglemap = P.EmissionAngleMap();
		for (auto i = anglemap.begin(); i != anglemap.end(); ++i){
			// cout << i->second << endl;
			const string &temp_name = i->first;
			double Beta = P.CalculateBeta(massmap[temp_name]);
			expectedNPhotons[temp_name] = (ExpectedNumberofPhotons(P.X, P.Y, P.Theta, P.Phi, Beta));
			// cout << temp_name << endl;
			// cout << "\t" << expectedNPhotons[temp_name].first << endl;
			// cout << "\t" << expectedNPhotons[temp_name].second << endl;
		}

		expectedPhotonMap[i] = expectedNPhotons;
	}


}