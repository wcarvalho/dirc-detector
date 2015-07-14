#include <iostream>
#include "functions.h"
#include "TVector3.h"
#include <iomanip>

using namespace std;

/*================================================
Update Theta and
================================================*/

void Update_Photon_ThetaPhi(Photon &photon, string Output)
{
	if (Output == "yes")
	{
		TabToLevel(4); cout << "Theta: " << photon.Theta << "\t->\t";
	}
	photon.Theta = photon.UnitVector.Theta();
	if (Output == "yes")
	{
		cout  << photon.Theta << endl;
	}

	if (Output == "yes")
	{
		TabToLevel(4); cout << "Phi: " << photon.Phi << "\t->\t";
	}
	photon.Phi = photon.UnitVector.Phi();
	if (Output == "yes")
	{
		cout  << photon.Phi << endl;
	}

}