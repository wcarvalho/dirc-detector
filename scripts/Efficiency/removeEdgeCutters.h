bool withinBounds(double const value, double comparison, double percent){
	// cout << "comparison:\t" << percent*comparison << "|\t" << value << "\t\t|";
	// cout << (1-percent)*comparison << endl;
	bool result = !((value < percent*comparison)||(value > (1-percent)*comparison));
	// cout << "result = " << result << endl;
	return result;
}
void clearOuterParticles(vector< pair<int, pair <Particle, TrackRecon> > > const &set, vector< pair<int, pair <Particle, TrackRecon> > > &newset, double length, double width, double percent){

	for (unsigned int i = 0; i < set.size(); ++i){
		const double &x = set.at(i).second.first.X;
		const double &y = set.at(i).second.first.Y;
		if ( ( (withinBounds(x, length, percent)) && (withinBounds(y, width, percent)) ) ){
			newset.push_back(set.at(i));
		}
	}
}