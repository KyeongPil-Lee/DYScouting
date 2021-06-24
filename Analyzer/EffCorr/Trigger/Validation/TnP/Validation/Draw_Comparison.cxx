#include <Include/SimplePlotTools.h>
#include <EffCorr/Trigger/Validation/TnP/TnPTool.h>

TGraphAsymmErrors* Graph_MCTruthEff(TString var)
{
  TString histName_base = "";
  if( var == "pt" )  histName_base = "h_muPt";
  if( var == "eta" ) histName_base = "h_muEta";
  TString fileName = "ROOTFile_MakeHist_MCTruthIsoMu24Eff_DYMuMu_M50toInf.root";
  TH1D* h_DEN = PlotTool::Get_Hist(fileName, histName_base+"_DEN");
  TH1D* h_NUM = PlotTool::Get_Hist(fileName, histName_base+"_NUM");

  TEfficiency *TEff = new TEfficiency(*h_NUM, *h_DEN);
  return (TGraphAsymmErrors*)TEff->CreateGraph()->Clone();
}

TGraphAsymmErrors* Graph_TnPEff(TString var)
{
  TString fileName = "ROOTFile_MakeHist_TnPHist_DYMuMu_M50toInf.root";
  TnPEffTool *tool = new TnPEffTool(fileName);
  return tool->CalcTnPEff_CutAndCount( var );
}


void Draw_Comparison(TString var)
{
  TGraphAsymmErrors* g_eff_MCTruth = Graph_MCTruthEff(var);
  TGraphAsymmErrors* g_eff_TnP     = Graph_TnPEff(var);

  TString canvasName = "c_comparison_MCTruth_vs_TnP";
  PlotTool::GraphCanvaswRatio* canvas = new PlotTool::GraphCanvaswRatio(canvasName, 0, 0);

  TString titleX = "";
  if( var == "pt" )  titleX = "p_{T} [GeV]";
  if( var == "eta" ) titleX = "#eta(#mu)";
  canvas->SetTitle(titleX, "IsoMu24 efficiency", "TnP/MCTruth");

  canvas->Register( g_eff_MCTruth, "MC-truth efficiency", kBlack);
  canvas->Register( g_eff_TnP,     "TnP efficiency",      kBlue);

  // canvas->SetRangeX(10, 200);
  canvas->SetRangeRatio(0.7, 1.3);
  canvas->SetLegendPosition(0.50, 0.15, 0.95, 0.32);

  canvas->Latex_CMSInternal();

  canvas->Draw();
}

void Draw_Comparison()
{
  Draw_Comparison("pt");
  Draw_Comparison("eta");
}