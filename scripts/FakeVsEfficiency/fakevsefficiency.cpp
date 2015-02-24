#include <tclap/CmdLine.h>
#include "TObject.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TAxis.h"
#include "FileProperties.h"
#include "utility_library.h"
#include "TCanvas.h"

using namespace std;
void checkValid(const TFile& f);

int main(int argc, char const *argv[])
{

string efficiencyFile = "";
string fakerateFile = "";
string outputFile = "";
bool print = false;


TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{

	TCLAP::ValueArg<std::string> efficiencyFileArg("e","efficiency-file","File with efficiency plot data",true,"efficiency.root","string", cmd);
	TCLAP::ValueArg<std::string> fakerateFileArg("f","fakerate-file","File with fakerate plot data",true,"fakerate.root","string", cmd);

	TCLAP::ValueArg<std::string> outputFileArg("o","output-file","File with output plot data",false,"efficiencyfakerate.root","string", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);

	cmd.parse( argc, argv );
	efficiencyFile = efficiencyFileArg.getValue();
	fakerateFile = fakerateFileArg.getValue();
	outputFile = outputFileArg.getValue();
	print = verboseArg.isSet();

}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }

	wul::FileProperties effProp(efficiencyFile);
	wul::FileProperties fakeProp(fakerateFile);
	wul::FileProperties outputProp(outputFile);

	std::string efficiencyPlotName = effProp.get_filename_without_suffix().c_str();
	std::string fakeratePlotName = fakeProp.get_filename_without_suffix().c_str();
	std::string outputPlotName = outputProp.get_filename_without_suffix().c_str();
	std::string outputPlotPath = outputProp.get_path_without_suffix().c_str();


	TFile efficiencyTFile(efficiencyFile.c_str());
	checkValid(efficiencyTFile);
	TGraphAsymmErrors* efficiencyPlot = (TGraphAsymmErrors*)efficiencyTFile.Get(efficiencyPlotName.c_str());

	TFile fakerateTFile(fakerateFile.c_str());
	checkValid(fakerateTFile);
	TGraphAsymmErrors* fakeratePlot = (TGraphAsymmErrors*)fakerateTFile.Get(fakeratePlotName.c_str());

	int npoints = efficiencyPlot->GetN();

	TGraphAsymmErrors g(npoints);
	g.SetName(outputPlotName.c_str());

	for (unsigned int i = 0; i < npoints; ++i){
		static double effx = 0., effy = 0.;
		static double fakex = 0., fakey = 0.;
		static double exl = 0., exh = 0., eyl = 0., eyh = 0.;

		efficiencyPlot->GetPoint(i, effx, effy);
		fakeratePlot->GetPoint(i, fakex, fakey);

		exl = efficiencyPlot->GetErrorYlow(i);
		exh = efficiencyPlot->GetErrorYhigh(i);
		eyl = fakeratePlot->GetErrorYlow(i);
		eyh = fakeratePlot->GetErrorYhigh(i);

		g.SetPoint(i, effy, fakey);
		g.SetPointError(i, exl, exh, eyl, eyh);
	}

	// TFile outputTFile(outputFile.c_str(), "recreate");
	TCanvas C("C", "C", 1000, 600);
	C.SetGrid();
	g.SetMarkerStyle(20);
	g.SetMarkerSize(2);
	g.Draw("AP");
	g.SetTitle("FakeRate vs. Efficiency");
	g.GetXaxis()->SetTitle("Efficiency (number of electrons identified as electrons)");
	g.GetYaxis()->SetTitle("FakeRate (number of pions identified as electrons)");
	// g.SetDrawOption("AP");
	// g.Paint("AP");
	g.SaveAs(outputFile.c_str(), "AP");
	C.Print(outputPlotPath.append(".pdf").c_str());
	// outputTFile.cd();
	// outputTFile.Close();

	efficiencyTFile.cd();
	efficiencyTFile.Close();

	fakerateTFile.cd();
	fakerateTFile.Close();
}

void checkValid(const TFile& f){
	if (!(f.IsOpen())) {
		cout << f.GetName() << " invalid!\n";
		exit(1);
	}
}