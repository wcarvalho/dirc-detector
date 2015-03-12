#ifndef ___CALIBRATIONDATA___
#define ___CALIBRATIONDATA___

#include "TObject.h"
#include <vector>
#include <map>
#include <sstream>

class calibration_data: public TObject
{
public:
	calibration_data(){}
	calibration_data(double p) : percent_desired(p) { }
	~calibration_data(){}

	void WriteAsPercent(){
		std::stringstream ss; ss.str("");
		ss << percent_desired;
		Write(ss.str().c_str(), TObject::kOverwrite);
	}

	std::vector< std::pair < double, double > > ThetaCenterAndWidth;
	std::vector< std::pair < double, double > > NPhotonsCenterAndWidth;
	std::vector < double > ThetaPercentAchieved;
	std::vector < double > NPhotonsPercentAchieved;
	double percent_desired;
	double low;
	double step;

	ClassDef(calibration_data, 1)  // Calibration Data

};


#endif