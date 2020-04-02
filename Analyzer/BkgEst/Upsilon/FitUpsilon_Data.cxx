#include <Include/SimplePlotTools.h>
#include <Include/DoubleCB/RooDoubleCBFast.cc>

using namespace RooFit;

void FitUpsilon_Data()
{
  TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
  TString fileName = analyzerPath+"/ControlPlot/Optimization/v10_BDT_noOS/LowMass/ROOTFile_MakeHist_Dimuon_noWeight_ScoutingCaloMuon_Run2018All.root";
  TH1D* h_data = PlotTool::Get_Hist(fileName, "h_diMuM_Upsilon");

  // -- observable: mass
  // RooRealVar mass("mass","m(#mu#mu) [GeV]", 7.0, 13.0);
  RooRealVar mass("mass","m(#mu#mu) [GeV]", 9.2, 9.6);
  RooDataHist data("data", "dimuon mass distribution", mass, h_data);

  // -- signal PDF
  // -- FIXME: adjust lower and upper edge using upsilon's width
  RooRealVar mean_1S("mean_1S","m(1S) [GeV]", 9.46030,  9.2, 9.6);
  RooRealVar mean_2S("mean_2S","m(2S) [GeV]", 10.02326, 9.8, 10.2);
  RooRealVar mean_3S("mean_3S","m(1S) [GeV]", 10.3552,  10.2, 10.5);

  RooRealVar sigma_1S("sigma_1S","m(1S) [GeV]", 1.0, 0, 3.0);
  RooRealVar a1_1S("a1_1S","m(1S) [GeV]", 1.0, 0, 3.0);
  RooRealVar n1_1S("n1_1S","m(1S) [GeV]", 1.0, 0, 3.0);
  RooRealVar a2_1S("a2_1S","m(1S) [GeV]", 1.0, 0, 3.0);
  RooRealVar n2_1S("n2_1S","m(1S) [GeV]", 1.0, 0, 3.0);

  RooAbsPdf *doubleCB_1S = new RooDoubleCBFast(Form("doubleCB_1S"),Form("doubleCB_1S"), *mass,*mean_1S,*sigma_1S, *a1_1S, *n1_1S, *a2_1S, *n2_1S);

  // -- background PDF
  RooRealVar c0("c0", "0th coefficient", 1.0, -1.0, 1.0);
  RooRealVar c0("c1", "1th coefficient", 0.0, -1.0, 1.0);
  RooRealVar c0("c2", "2th coefficient", 0.0, -1.0, 1.0);
  RooPolynomial polynomial3rd("polynomial3rd", "3rd polynomial", mass, RooArgList(c0,c1,c2));

  // -- number of signal and background events (as fit parameters)
  RooRealVar nsig("nsig","signal fraction", 1e4, 0., 1e6);
  RooRealVar nbkg(“nbkg”,”background fraction”, 1e4, 0., 1e6);

  // -- final model
  RooAddPdf model("model","model", , RooArgList(*doubleCB_1S, polynomial3rd) ,RooArgList(nsig,nbkg));
}