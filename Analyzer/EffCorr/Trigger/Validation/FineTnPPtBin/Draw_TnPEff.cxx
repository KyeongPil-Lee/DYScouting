#include <Include/SimplePlotTools.h>
#include <EffCorr/Trigger/Validation/TnP/TnPTool.h>


TGraphAsymmErrors* Graph_TnPEff(TString fileName, TString var)
{
  TnPEffTool *tool = new TnPEffTool(fileName);
  // if( var == "pt" ) var = "highPt";
  return tool->CalcTnPEff_CutAndCount( var );
}

void Draw_TnPEff(TString fileName, TString var)
{
  TGraphAsymmErrors* g_eff_TnP     = Graph_TnPEff(fileName, var);

  TString canvasName = "c_TnPEff_"+var;

  Bool_t isLogX = kFALSE;
  Bool_t isLogY = kFALSE;
  if( var == "pt" ) isLogX = kTRUE;
  PlotTool::GraphCanvas* canvas = new PlotTool::GraphCanvas(canvasName, isLogX, isLogY);

  TString titleX = "";
  if( var == "pt" )  titleX = "p_{T} [GeV]";
  if( var == "eta" ) titleX = "#eta(#mu)";
  canvas->SetTitle(titleX, "L1+HLT efficiency");

  // canvas->Register( g_eff_MCTruth, "MC-truth efficiency", kBlack);
  canvas->Register( g_eff_TnP,     "TnP efficiency",      kBlue);

  if( var == "eta" ) 
  {
    canvas->SetRangeX(-2.4, 2.4);
    canvas->SetRangeY(0.76, 1.01);
  }
  if( var == "pt" )  canvas->SetRangeY(0, 1.01);
  // canvas->SetRangeRatio(0.97, 1.03);
  // canvas->SetRangeY(0.6, 1.05);
  canvas->SetLegendPosition(0.50, 0.85, 0.95, 0.95);

  canvas->Latex_CMSInternal();
  // canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{IsoMu24 over TightID + PFIso(d#beta)/p_{T}<0.15}}");

  canvas->Draw();
}

void Print_TnP2DEff(TH2D* h2D)
{
  Int_t nBinX = h2D->GetNbinsX();
  Int_t nBinY = h2D->GetNbinsY();

  for(Int_t i_x=0; i_x<nBinX; i_x++) // -- x axis: pT
  {
    Int_t i_binX = i_x+1;

    Double_t ptMin = h2D->GetXaxis()->GetBinLowEdge(i_binX);
    Double_t ptMax = h2D->GetXaxis()->GetBinLowEdge(i_binX+1);

    printf("[%.1lf < pT < %.1lf]\n", ptMin, ptMax);

    for(Int_t i_y=0; i_y<nBinY; i_y++) // -- y axis: eta
    {
      Int_t i_binY = i_y+1;

      Double_t etaMin = h2D->GetYaxis()->GetBinLowEdge(i_binY);
      Double_t etaMax = h2D->GetYaxis()->GetBinLowEdge(i_binY+1);

      Double_t eff = h2D->GetBinContent(i_binX, i_binY);

      printf("  [%.1lf < eta < %.1lf] -> eff = %.3lf\n", etaMin, etaMax, eff);
    }

    cout << endl;
  }
}

void Draw_TnP2DEff(TString fileName)
{
  vector<Double_t> vec_ptEtaBinEdge_pt = {5, 6, 7, 10, 15, 20, 30, 40, 50, 60, 120, 500};

  TnP2DEffTool* tool = new TnP2DEffTool(fileName, vec_ptEtaBinEdge_pt);
  vector<TGraphAsymmErrors*> vec_effGraph = tool->CalcTnPEff_CutAndCount("pteta");
  TH2D* h2D_eff = tool->Get_2DEffMap();

  // -- drawing 2D histogram
  TString canvasName = "c_TnP2DEff";
  PlotTool::Hist2DCanvas *canvas = new PlotTool::Hist2DCanvas(canvasName, 1, 0, 0);
  canvas->SetTitle("p_{T} [GeV]", "#eta");

  canvas->Register( h2D_eff );
  canvas->SetRangeZ(0, 1.01);
  canvas->Latex_CMSInternal();

  canvas->Draw("COLZ");
  // canvas->Draw("COLZTEXT");

  Print_TnP2DEff(h2D_eff);
}

void Draw_TnPEff(TString fileName)
{
  Draw_TnPEff(fileName, "pt");
  Draw_TnPEff(fileName, "eta");
  Draw_TnP2DEff(fileName);
}

void Draw_TnPEff()
{
  TString fileName = "ROOTFile_MakeTnPHist_HLTLeg_DYMuMu_M50toInf.root";

  Draw_TnPEff(fileName);

  // -- make a root file with 2D map histogram
  vector<Double_t> vec_ptEtaBinEdge_pt = {5, 6, 7, 10, 15, 20, 30, 40, 50, 60, 120, 500};
  TnP2DEffTool* tool = new TnP2DEffTool(fileName, vec_ptEtaBinEdge_pt);
  vector<TGraphAsymmErrors*> vec_effGraph = tool->CalcTnPEff_CutAndCount("pteta");
  TH2D* h2D_eff = tool->Get_2DEffMap();
  h2D_eff->SetName("h2D_eff");

  // -- 1D histograms
  TGraphAsymmErrors* g_eff_pt  = Graph_TnPEff(fileName, "pt");
  TGraphAsymmErrors* g_eff_eta = Graph_TnPEff(fileName, "eta");

  TH1D* h_eff_pt  = PlotTool::Convert_GraphToHist(g_eff_pt);
  TH1D* h_eff_eta = PlotTool::Convert_GraphToHist(g_eff_eta);
  h_eff_pt->SetName("h_eff_pt");
  h_eff_eta->SetName("h_eff_eta");

  TFile* f_output = TFile::Open("ROOTFile_L1HLTLegEfficiencyMap.root", "RECREATE");
  h2D_eff->Write();
  h_eff_pt->Write();
  h_eff_eta->Write();
  f_output->Close();
}