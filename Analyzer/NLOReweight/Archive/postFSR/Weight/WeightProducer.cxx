#include "WeightProducer.h"

void WeightProducer()
{
  TString fileName_LO = "../LO/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";
  TString fileName_NLO = "../NLO/ROOTFile_MakeGenHist_DiMuPtRap_NLO_DYMuMu_M10to50_2016.root";
  NLOWeightProducer* producer = new NLOWeightProducer(fileName_LO, fileName_NLO);
  producer->Produce();
  producer->Save("ROOTFile_NLOWeight.root");
}