#include "WeightProducer.h"

void WeightProducer()
{
  // TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
  // TString fileName_LO  = analyzerPath+"/NLOReweight/postFSR/Weight/MassDepW/Test/v01_M10to15/Distribution/LO/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";
  // TString fileName_NLO = analyzerPath+"/NLOReweight/postFSR/Weight/MassDepW/Test/v01_M10to15/Distribution/NLO/ROOTFile_MakeGenHist_DiMuPtRap_NLO_DYMuMu_M10to50_2016.root";
  TString fileName_LO  = "./Distribution/LO/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";
  TString fileName_NLO = "./Distribution/NLO/ROOTFile_MakeGenHist_DiMuPtRap_NLO_DYMuMu_M10to50_2016.root";

  NLOWeightProducerPerMassBin* producer = new NLOWeightProducerPerMassBin(fileName_LO, fileName_NLO);
  producer->Produce();
  producer->Save("ROOTFile_NLOWeight.root");
}