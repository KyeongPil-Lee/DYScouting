#include <Include/SimplePlotTools.h>
#include <EffCorr/Trigger/Validation/TnP/TnPTool.h>

TGraphAsymmErrors* Graph_TnPEff(TString fileName, TString var)
{
  TnPEffTool *tool = new TnPEffTool(fileName);
  // if( var == "pt" ) var = "highPt";
  return tool->CalcTnPEff_CutAndCount( var );
}

void SaveResultAsROOTFile()
{
  TString fileName_lowPt  = "./LowPtL1/ROOTFile_MakeTnPHist_LowPtL1andHLT_DYMuMu_M50toInf.root";
  TString fileName_highPt = "./HighPtL1/ROOTFile_MakeTnPHist_HighPtL1andHLT_DYMuMu_M50toInf.root";

  // -- 1D efficiency (convert to TH1D)
  TGraphAsymmErrors* g_eff_pt_lowPt  = Graph_TnPEff(fileName_lowPt, "pt");
  TGraphAsymmErrors* g_eff_eta_lowPt = Graph_TnPEff(fileName_lowPt, "eta");

  TH1D* h_eff_pt_lowPt  = PlotTool::Convert_GraphToHist(g_eff_pt_lowPt);
  TH1D* h_eff_eta_lowPt = PlotTool::Convert_GraphToHist(g_eff_eta_lowPt);
  h_eff_pt_lowPt->SetName("h_eff_pt_lowPt");
  h_eff_eta_lowPt->SetName("h_eff_eta_lowPt");

  TGraphAsymmErrors* g_eff_pt_highPt  = Graph_TnPEff(fileName_highPt, "pt");
  TGraphAsymmErrors* g_eff_eta_highPt = Graph_TnPEff(fileName_highPt, "eta");

  TH1D* h_eff_pt_highPt  = PlotTool::Convert_GraphToHist(g_eff_pt_highPt);
  TH1D* h_eff_eta_highPt = PlotTool::Convert_GraphToHist(g_eff_eta_highPt);
  h_eff_pt_highPt->SetName("h_eff_pt_highPt");
  h_eff_eta_highPt->SetName("h_eff_eta_highPt");

  // -- 2D efficiency map (TH2D)
  vector<Double_t> vec_ptEtaBinEdge_pt = {5, 6, 7, 10, 15, 20, 30, 40, 50, 60, 120, 500};
  TnP2DEffTool* tool_lowPt = new TnP2DEffTool(fileName_lowPt, vec_ptEtaBinEdge_pt);
  vector<TGraphAsymmErrors*> vec_effGraph_lowPt = tool_lowPt->CalcTnPEff_CutAndCount("pteta");
  TH2D* h2D_eff_lowPt = tool_lowPt->Get_2DEffMap();
  h2D_eff_lowPt->SetName("h2D_eff_lowPt");

  TnP2DEffTool* tool_highPt = new TnP2DEffTool(fileName_highPt, vec_ptEtaBinEdge_pt);
  vector<TGraphAsymmErrors*> vec_effGraph_highPt = tool_highPt->CalcTnPEff_CutAndCount("pteta");
  TH2D* h2D_eff_highPt = tool_highPt->Get_2DEffMap();
  h2D_eff_highPt->SetName("h2D_eff_highPt");

  // -- save
  TFile* f_output = TFile::Open("ROOTFile_L1HLTLegEfficiencyMap.root", "RECREATE");
  h_eff_pt_lowPt->Write();
  h_eff_eta_lowPt->Write();

  h_eff_pt_highPt->Write();
  h_eff_eta_highPt->Write();

  h2D_eff_lowPt->Write();
  h2D_eff_highPt->Write();
}