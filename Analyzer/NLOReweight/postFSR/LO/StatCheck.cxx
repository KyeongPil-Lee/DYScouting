#include "../StatCheck.h"

void StatCheck()
{
  TString fileName = "./ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";
  PlotProducer *producer = new PlotProducer(fileName);
  producer->SetDebug();
  producer->Produce();
}