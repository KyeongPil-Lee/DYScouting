#include <Include/SimplePlotTools.h>
#include <Include/DYTool.h>

void Print_DecreasedEvents(TString type, TH1D* h, Int_t i_bin_before, Int_t i_bin_after)
{
  Double_t nEvent_before = h->GetBinContent(i_bin_before);
  Double_t nEvent_after  = h->GetBinContent(i_bin_after);
  Double_t diff = nEvent_before - nEvent_after;
  Double_t relDiff = diff / nEvent_before;

  printf("[%s] %.1lf -> %.1lf (difference = %.1lf, relative diff. = %.3lf%%\n", 
    type.Data(), nEvent_before, nEvent_after, diff, relDiff * 100 );
}

void Print_DYFraction(TH1D* h_totMC, TH1D* h_DY, Int_t i_bin)
{
  Double_t nEvent_totMC = h_totMC->GetBinContent(i_bin);
  Double_t nEvent_DY    = h_DY->GetBinContent(i_bin);

  Double_t frac_DY = nEvent_DY / nEvent_totMC;

  printf("[%d bin] # events: (totMC, DY) = (%.1lf, %.1lf) ---> DY fraction = %.3lf%%\n\n", 
    i_bin, nEvent_totMC, nEvent_DY, frac_DY*100);

}

void DrawCutFlow()
{
  TString basePath = "./";
  TString baseFileName = "ROOTFile_CountEvent_HLTStep";
  DYTool::HistGetter* histGetter = new DYTool::HistGetter(basePath, baseFileName);
  histGetter->DoNormalization("2018");
  histGetter->DoRemoveNegativeBin();

  TString tag_data  = "Offline_SingleMuon_Run2018All";
  TString tag_ttbar = "Offline_ttbar";
  TString tag_WJets = "Offline_WJets";

  vector<TString> vec_tag_DYMuMu   = { "Offline_DYMuMu_M10to50", "Offline_DYMuMu_M50toInf" };
  vector<TString> vec_tag_DYTauTau = { "Offline_DYTauTau_M10to50", "Offline_DYTauTau_M50toInf" };

  vector<TString> vec_tag_QCDMuEnriched = { 
    "Offline_QCDMuEnriched_Pt15to20", 
    "Offline_QCDMuEnriched_Pt20to30",
    "Offline_QCDMuEnriched_Pt30to50",
    "Offline_QCDMuEnriched_Pt50to80",
    "Offline_QCDMuEnriched_Pt80to120",
    "Offline_QCDMuEnriched_Pt120to170",
    "Offline_QCDMuEnriched_Pt170to300",
    "Offline_QCDMuEnriched_Pt300to470",
    "Offline_QCDMuEnriched_Pt470to600",
    "Offline_QCDMuEnriched_Pt600to800",
    "Offline_QCDMuEnriched_Pt800to1000",
    "Offline_QCDMuEnriched_Pt1000toInf"
  };

  TString histName = "h_nEvent_HLTStep";

  Bool_t isMC = kFALSE;
  TH1D* h_data = histGetter->Get_Histogram(histName, tag_data, isMC);

  isMC = kTRUE;
  TH1D* h_ttbar = histGetter->Get_Histogram(histName, tag_ttbar, isMC);
  TH1D* h_WJets = histGetter->Get_Histogram(histName, tag_WJets, isMC);

  TH1D* h_DYMuMu        = histGetter->Get_Histogram(histName, vec_tag_DYMuMu, isMC);
  TH1D* h_DYTauTau      = histGetter->Get_Histogram(histName, vec_tag_DYTauTau, isMC);
  TH1D* h_QCDMuEnriched = histGetter->Get_Histogram(histName, vec_tag_QCDMuEnriched, isMC);

  TString canvasName = histName;
  canvasName.ReplaceAll("h_", "c_stack_");

  PlotTool::HistStackCanvaswRatio *canvas = new PlotTool::HistStackCanvaswRatio(canvasName, 0, 1);
  canvas->RegisterData(h_data, "2018Data (SingleMuon)", kBlack);

  canvas->Register(h_DYTauTau,      "Z/#gamma #rightarrow #tau#tau (M > 10 GeV)", DYTool::kDYTauTau);
  canvas->Register(h_ttbar,         "t#bar{t} (leptonic)", DYTool::kTop);
  canvas->Register(h_WJets,         "W+jets", DYTool::kWJets);
  canvas->Register(h_QCDMuEnriched, "QCD (p_{T}(#mu) > 5 GeV, #mu-enriched)", DYTool::kQCD);
  canvas->Register(h_DYMuMu,        "Z/#gamma #rightarrow #mu#mu (M > 10 GeV)", DYTool::kDY);

  canvas->SetLegendColumn(2);
  canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);
  canvas->SetTitle("HLT Step", "Entry", "data/MC");
  // canvas->SetAutoRangeY();
  canvas->SetRangeY(1e3, 2e10);
  canvas->SetRangeRatio(0.88, 1.01);

  canvas->Latex_CMSInternal();
  canvas->RegisterLatex(0.75, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", DYTool::LUMI_2018/1000.0));

  // canvas->ShowDataMCRatio();

  canvas->Draw();

  // -- linear version
  canvas->SetRangeY(3e7, 5e7);
  canvas->isLogY_ = kFALSE;
  canvas->canvasName_ = canvas->canvasName_ + "_linear";
  canvas->Draw();

  TH1D* h_totMC = (TH1D*)h_ttbar->Clone();
  h_totMC->Add(h_WJets);
  h_totMC->Add(h_DYTauTau);
  h_totMC->Add(h_QCDMuEnriched);
  h_totMC->Add(h_DYMuMu);

  // -- check DY fraction
  // Print_DYFraction( h_totMC, h_DYMuMu, 3);
  // Print_DYFraction( h_totMC, h_DYMuMu, 4);

  // -- check the # decreased events
  // Print_DecreasedEvents("Data",   h_data, 3, 4);
  // Print_DecreasedEvents("totMC",  h_totMC, 3, 4);
  // Print_DecreasedEvents("DYMuMu", h_DYMuMu, 3, 4);
}