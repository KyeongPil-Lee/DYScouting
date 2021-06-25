#include <Include/SimplePlotTools.h>
#include <EffCorr/Trigger/Validation/TnP/TnPTool.h>

TGraphAsymmErrors* Graph_MCTruthEff(TString var)
{
  TString histName_base = "";
  if( var == "pt" )  histName_base = "h_muPt";
  if( var == "eta" ) histName_base = "h_muEta";
  TString fileName = "ROOTFile_IsoMu24Eff_MCTruth_Offline_DYMuMu_M50toInf.root";
  TH1D* h_DEN = PlotTool::Get_Hist(fileName, histName_base+"_DEN");
  TH1D* h_NUM = PlotTool::Get_Hist(fileName, histName_base+"_NUM");

  TEfficiency *TEff = new TEfficiency(*h_NUM, *h_DEN);
  return (TGraphAsymmErrors*)TEff->CreateGraph()->Clone();
}

TGraphAsymmErrors* Graph_TnPEff(TString var)
{
  TString fileName = "ROOTFile_MakeHist_IsoMu24_Offline_DYMuMu_M50toInf.root";
  TnPEffTool *tool = new TnPEffTool(fileName);
  if( var == "pt" ) var = "highPt";
  return tool->CalcTnPEff_CutAndCount( var );
}


void Draw_Comparison(TString var)
{
  TGraphAsymmErrors* g_eff_MCTruth = Graph_MCTruthEff(var);
  TGraphAsymmErrors* g_eff_TnP     = Graph_TnPEff(var);

  TString canvasName = "c_comparison_MCTruth_vs_TnP_"+var;
  PlotTool::GraphCanvaswRatio* canvas = new PlotTool::GraphCanvaswRatio(canvasName, 0, 0);

  TString titleX = "";
  if( var == "pt" )  titleX = "p_{T} [GeV]";
  if( var == "eta" ) titleX = "#eta(#mu)";
  canvas->SetTitle(titleX, "IsoMu24 efficiency", "TnP/MCTruth");

  canvas->Register( g_eff_MCTruth, "MC-truth efficiency", kBlack);
  canvas->Register( g_eff_TnP,     "TnP efficiency",      kBlue);

  // canvas->SetRangeX(10, 200);
  canvas->SetRangeRatio(0.97, 1.03);
  canvas->SetRangeY(0.6, 1.05);
  canvas->SetLegendPosition(0.50, 0.32, 0.95, 0.45);

  canvas->Latex_CMSInternal();
  canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{IsoMu24 over TightID + PFIso(d#beta)/p_{T}<0.15}}");

  canvas->Draw();
}

void Draw_Comparison()
{
  Draw_Comparison("pt");
  Draw_Comparison("eta");
}