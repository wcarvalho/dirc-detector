#include "../Efficiency/match_conditions.h";

typedef std::unordered_map<int, bool(*)(const Particle&, const TrackRecon&, const int&, const double&)> flag_fun_map;

bool momentum_exceeds_threshold(const Particle& P, const TrackRecon& R, const int& rec_i, const double& threshold)
{
	double momentum = P.CalculateMomentum();
	return (momentum > 1.5);
}

bool correct_searchtype(const Particle& P, const TrackRecon& R, const int& rec_i, const double& threshold)
{
	//FIXME hardcoded searchtype
	std::string type = "electron";
	return (R.Options[rec_i] == type );
}

bool within_sigma_threshold(const Particle& P, const TrackRecon& R, const int& rec_i, const double& threshold)
{
	//FIXME hardcoded threshold
	bool inbox = inside_box(P, R, rec_i, 1.5);
	return ( inbox );
}


bool passConditions(const vector<int>& flags, flag_fun_map& fmap, const Particle& P, const TrackRecon& R, const int& rec_i, const double& threshold){
	static bool passed;
	passed = true;
	for (unsigned int f = 0; f < flags.size(); ++f){
		const int& condition = flags.at(f);
		passed *= fmap[condition](P, R, rec_i, threshold);
	}
	return passed;
}