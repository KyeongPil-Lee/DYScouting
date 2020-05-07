#include "../StatCheck.h"

void StatCheck()
{
  TString fileName = "./ROOTFile_MakeGenHist_DiMuPtRap_NLO_DYMuMu_M10to50_2016.root";
  PlotProducer *producer = new PlotProducer(fileName);
  producer->SetDebug();
  producer->Produce();
}