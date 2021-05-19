#include <Include/SimplePlotTools.h>

void PrintCountedEvents(TString sampleType)
{
  printf("[SampleType] %s\n", sampleType.Data());

  TString fileName = TString::Format("ROOTFile_CountFiredEvents_%s.root", sampleType.Data());

  TH1D* h_nEvent_filter_1mu = PlotTool::Get_Hist(fileName, "h_nEvent_filter_1mu");
  TH1D* h_nEvent_filter_2mu = PlotTool::Get_Hist(fileName, "h_nEvent_filter_2mu");
  TH1D* h_nEvent_trigFired  = PlotTool::Get_Hist(fileName, "h_nEvent_trigFired");

  Double_t nEvent_filter_1mu = h_nEvent_filter_1mu->GetBinContent(1);
  Double_t nEvent_filter_2mu = h_nEvent_filter_2mu->GetBinContent(1);
  Double_t nEvent_trigFired  = h_nEvent_trigFired->GetBinContent(1);

  printf("[# events] (filter_1mu, filter_2mu+, trigFired) = (%.1lf, %.1lf, %.1lf)\n", nEvent_filter_1mu, nEvent_filter_2mu, nEvent_trigFired);
  printf("\n");
}

void PrintCountedEvents()
{
  PrintCountedEvents("Offline_DYMuMu_M10to50");
  PrintCountedEvents("Offline_DYMuMu_M50toInf");
  PrintCountedEvents("Offline_SingleMuon_Run2018All");
}

