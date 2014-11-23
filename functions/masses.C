#include <map>
#include "../headers/functions.h"

 void mass::MassNameMapper() //mass -> name
{
	MasstoName[.105658] = "muon";
	MasstoName[.511e-3] = "electron";
	MasstoName[.1396] = "pion";
	MasstoName[.493667] = "kaon";
	MasstoName[.938] = "proton";

	for (map<double, string>::iterator i = MasstoName.begin(); i != MasstoName.end(); ++i)
    NametoMass[i->second] = i->first;
}

void mass::AngleMassMapper() // angle -> mass
{
	double pi = TMath::Pi();
	double ThetaBeam = 2*atan(exp(-eta));
	double P = pt/sin(ThetaBeam);
	double thetaC_temp = 0;
	double beta_temp = 0;

	for (std::map<double, std::string>::iterator it = MasstoName.begin(); it != MasstoName.end(); ++it)
	{
		const double &m = it->first;
		beta_temp = P/pow(( m*m + P*P ),.5);
		thetaC_temp = acos(1./(1.474*beta_temp));
		// cout << MasstoName[m] << " - > angle ";
		// cout << thetaC_temp << endl;
		if (thetaC_temp == thetaC_temp){
			AngletoMass[thetaC_temp] = m;
		}
	}
}
