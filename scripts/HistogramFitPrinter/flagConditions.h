#include "../Efficiency/match_conditions.h";

bool momentum_exceeds_threshold(const Particle& P, const TrackRecon& R, const int& rec_i)
{
	double momentum = P.CalculateMomentum();
	return (momentum > 1.7);
}

bool correct_searchtype(const Particle& P, const TrackRecon& R, const int& rec_i)
{
	//FIXME hardcoded searchtype
	std::string type = "electron";
	return (R.Options[rec_i] == type );
}

bool within_sigma_threshold(const Particle& P, const TrackRecon& R, const int& rec_i)
{
	//FIXME hardcoded threshold
	double threshold = 1.5;
	bool inbox = inside_box(P, R, rec_i, threshold);
	return ( inbox );
}
