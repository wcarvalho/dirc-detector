#include "generator.h"

//class for generating particles
class gParticle : public Particle
{
public:
	gParticle(double in = 0) : r(in), chargeMarker(0) { setDefaults(); setPtDistributionFunction(); }
	~gParticle(){}

	void setDefaults();
	void genEta(){ Eta = r.Uniform(etarange[0],etarange[1]); }
	void genMass();
	void genPT();
	void genCharge();
	void getPhi(){ Phi_i = r.Uniform(phirange[0],phirange[1]); }
	void getEangle(){	ConeAngle = acos(1./(1.474*Beta));}
	void gen(){ genEta(); genMass(); genPT(); genCharge(); getPhi(); }

	void setPtDistributionFunction(std::string func = "x/(0.5+x*x*x*x)"){
		ptdisributionfunction = func; }

	void SetPhiRange(double low, double hi){ phirange[0] = low; phirange[1] = hi; }
	void SetEtaRange(double low, double hi){ etarange[0] = low; etarange[1] = hi; }
	void SetPtRange(double low, double hi){ ptrange[0] = low; ptrange[1] = hi; }

	void SetTypes(vector<string> newTypes){ types = newTypes; }
	void SetChargeMarker(double marker){ chargeMarker = marker; }

	double Phi_i;
	Random r;
	double etarange[2];
	double phirange[2];
	double ptrange[2];
	map<string, double> massmap;
	double chargeMarker;
	std::string ptdisributionfunction;

// private:

};