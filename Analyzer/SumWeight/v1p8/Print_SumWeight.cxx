#include <Include/SimplePlotTools.h>

void Print_SumWeight()
{
  TString rootFileName = _file0->GetName();
  TH1D* h_sumWeight = PlotTool::Get_Hist(rootFileName, "h_sumWeight");
  Double_t sumWeight = h_sumWeight->GetBinContent(1);
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++\n");
  printf("[ROOTFile: %s] -> SumWeight = %.1lf\n", rootFileName.Data(), sumWeight);
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
}