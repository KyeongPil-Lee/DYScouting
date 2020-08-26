#include "WeightProducer.h"

void WeightProducer()
{
  TString fileName_LO  = "./Distribution/LO/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";
  TString fileName_NLO = "./Distribution/NLO/ROOTFile_MakeGenHist_DiMuPtRap_NLO_DYMuMu_M10to50_2016.root";

  NLOWeightProducerPerMassBin* producer = new NLOWeightProducerPerMassBin(fileName_LO, fileName_NLO);
  producer->Produce();
  producer->Save("ROOTFile_NLOWeight.root");
}