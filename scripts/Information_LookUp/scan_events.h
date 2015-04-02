#include <string>
#include <sstream>
#include <unordered_map>

#include "TFile.h"
#include "TTree.h"
#include "dircTFile.h"

#include "event_parsers.h"
#include "dircComparisons.h"
#include "../Efficiency/match_conditions.h"

bool passed_as_bad_event(const Particle& P, const TrackRecon& R, const int& rec_i, const double& threshold,
	flag_fun_map& func_map, vector<int> const& conditions){
	static bool passed;
	passed = false;
	for(auto i: conditions){
		if (!func_map[i](P, R, rec_i, -threshold)) return true;
	}
	return passed;
}
