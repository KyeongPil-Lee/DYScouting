#include "StatCheck.h"

void StatCheck_LOvsNLO()
{
  ComparisonPlotProducer* producer = new ComparisonPlotProducer(
    "./LO/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root", 
    "./NLO/ROOTFile_MakeGenHist_DiMuPtRap_NLO_DYMuMu_M10to50_2016.root");

  producer->Produce();
}