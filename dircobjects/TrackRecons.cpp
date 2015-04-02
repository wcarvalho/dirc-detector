#include "dirc_objects.h"

int getReconIndex(TrackRecon &T, std::string desired_type){
	for (int i = 0; i < T.Options.size(); ++i){
		if (T.Options.at(i) == desired_type) return i;
	}
	return -1;
}