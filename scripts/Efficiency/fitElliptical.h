#include "utility_library.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TEllipse.h"
#include "TH2F.h"
#include "TMath.h"
#include "TRandom.h"
#include "TStyle.h"
#include "iostream"
#include "TMinuit.h"

using namespace std;
TH2D *h2 = 0;
Float_t thresh = 20;
//______________________________________________________________________________

Double_t InsideEllipse(Double_t x1, Double_t y1,Double_t x2, Double_t y2,
                     Double_t b, Double_t x,Double_t y, bool &inside)
{
// look if a point x, y is inside an ellipse defined by
// focal points (x1, y1), (x2, y2) and radius b
//
//  half dist of focal points
   Double_t a = 0.5 * TMath::Sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
   Double_t s = TMath::Sqrt(a*a + b*b);
   Double_t d1 = TMath::Sqrt((x-x1)*(x-x1) + (y-y1)*(y-y1));
   Double_t d2 = TMath::Sqrt((x-x2)*(x-x2) + (y-y2)*(y-y2));
// An ellipse is defined by points for which the sum of the distances
// to the focal points is constant  ( = 2 * s)
	 if (d2 + d1 <= 2 * s) inside = true;
	 else                  inside = false;
   return 2 * s - (d2 + d1);  // > 0: inside
}
//______________________________________________________________________________

Double_t Length(Double_t x1, Double_t y1, Double_t x2, Double_t y2)
{
   return TMath::Sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}
//______________________________________________________________________________

Double_t ThetaOfLineDegree(Double_t x1, Double_t y1, Double_t x2, Double_t y2)
{
   Double_t len = Length(x1, y1, x2, y2);
   Double_t phi = 0;
   if (len > 0) {
      Double_t sina = (y2 - y1) / len;
      Double_t cosa = (x2 - x1) / len;
      phi = TMath::ATan2(sina, cosa);
      if (phi < 0) phi += 2. * TMath::Pi();
   }

   return 180 * phi / TMath::Pi();
}
//______________________________________________________________________________

void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
   if (gin);    // keep compiler quiet
   if (npar);
   if (iflag);
   Double_t sum = 0;
   TAxis *ax = h2->GetXaxis();
   TAxis *ay = h2->GetYaxis();
   for (Int_t ix = 1; ix <= h2->GetNbinsX(); ix++) {
      for (Int_t iy = 1; iy <= h2->GetNbinsY(); iy++) {
      	Double_t x = ax->GetBinCenter(ix);
      	Double_t y = ay->GetBinCenter(iy);
      	Double_t cont = h2->GetBinContent(ix, iy);

//    	dont take just inside = 0, outside  = 1 to avoid steps
//    	in function to be minimized (should be differentiable)
      	bool inside;
      	Double_t d = InsideEllipse(par[0],par[1], par[2],par[3],par[4],x, y, inside);
      	if (d > 0) {
      		 if (cont < thresh) sum += d * (thresh - cont);
      	} else {
      		 if (cont > thresh) sum += -d * (cont - thresh);
      	}
      }
   }
   f = sum;
}
//________________________________________________________________________

void fitElliptical(std::vector<double>& x, std::vector<double>& y, double& x0_fit, double& y0_fit, double& a_fit, double& b_fit){

	thresh = 200;
	int nbins = 25;
	int nbinsx = nbins;
	int nbinsy = nbins;

	double xlow = -3, xup = 3;
	double ylow = 0., yup = 0.;
	wul::vectorminmax(y, ylow, yup);
	// (x-x0)^2/a + (y-y0)^2/b = 1
	h2 = new TH2D("ellipse_fit", "ellipse_fit", nbinsx, xlow, xup, nbinsy, ylow, yup);

	for (unsigned i = 0; i < x.size(); ++i){
		// cout << x.at(i) << "," << y.at(i) << endl;
		h2->Fill(x.at(i), y.at(i));
	}

	// a = xup;
	// b = (yup-ylow)/2;
	// x0 = 0.;
	// y0 = ylow + b;

	TCanvas* C = new TCanvas("C", "C", 1000, 600);
	// h2->Draw("colz");
	// C->Print("hey.pdf");

	TMinuit *gMinuit = new TMinuit(5);  //ini TMinuit with a maximum of 5 params
	gMinuit->SetFCN(fcn);

	Double_t arglist[10];
	Int_t ierflg = 0;

	arglist[0] = 1;
	// gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);

	double fitxlow = 0., fitxup = 0., fitylow = 0., fityup = 0., fitb = 0.;

	fitxlow = -1;
	fitxup = 1;
	fitylow = -.5;
	fityup = .5;
	fitb = .4;

	// fitylow -= .005;
	// fityup -= .005;
	// Set starting values and step sizes for parameters
	static Double_t vstart[5] = {fitxlow, fitylow , fitxup, fityup , fitb};
	static Double_t step[5] = {0.01 , 0.01 , 0.01 , 0.01, 0.001};
	gMinuit->mnparm(0, "x1", vstart[0], step[0], 0,0,ierflg);
	gMinuit->mnparm(1, "y1", vstart[1], step[1], 0,0,ierflg);
	gMinuit->mnparm(2, "x2", vstart[2], step[2], 0,0,ierflg);
	gMinuit->mnparm(3, "y2", vstart[3], step[3], 0,0,ierflg);
	gMinuit->mnparm(4, "b",  vstart[4], step[4], 0,0,ierflg);

	// Now ready for minimization step
	arglist[0] = 500;
	arglist[1] = 1.;
	gMinuit->mnexcm("MIGRAD", arglist ,2,ierflg);

	// Print results
	Double_t amin,edm,errdef;
	Int_t nvpar,nparx,icstat;
	// gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
	// gMinuit->mnprin(3,amin);
	gStyle->SetPalette(1,0); //rainbow colors
	h2->Draw("col");
	Double_t x1, y1, x2, y2, b, ex1, ey1, ex2, ey2, eb;
	gMinuit->GetParameter(0, x1, ex1);
	gMinuit->GetParameter(1, y1, ey1);
	gMinuit->GetParameter(2, x2, ex2);
	gMinuit->GetParameter(3, y2, ey2);
	gMinuit->GetParameter(4,  b, eb);
	// recalculate parameters needed by TEllipse
	Double_t xcenter = 0.5 * (x1 + x2);
	Double_t ycenter = 0.5 * (y1 + y2);
	Double_t r1 =  0.5 * TMath::Sqrt((x1 - x2)*(x1 - x2)+ (y1 - y2)*(y1 - y2));
	Double_t r2 = b;
	Double_t theta = ThetaOfLineDegree(x1, y1, x2, y2);
	cout << "xcenter = " << xcenter << endl;
	cout << "ycenter = " << ycenter << endl;
	cout << "r1 = " << r1 << endl;
	cout << "r2 = " << r2 << endl;
	TEllipse * el = new TEllipse(xcenter, ycenter, r1, r2, 0, 360, theta);
	el->SetLineColor(6);
	el->SetLineWidth(3);

	TFile f("ellipse.root", "recreate");
	el->Write();
	f.Close();

	C->cd();
	el->Draw();
	C->Print("hey.pdf");

	x0_fit = xcenter;
	y0_fit = ycenter;
	a_fit = r1;
	b_fit = r2;
	delete C;
	delete h2;
	delete gMinuit;
	delete el;
}