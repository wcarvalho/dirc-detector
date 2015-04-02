#include "dircTFile.h"


namespace dirc{

	void checkValid(const TFile& f){
		if (!(f.IsOpen())) {
			std::cout << f.GetName() << " invalid!\n";
			exit(1);
		}
	}

}