#include <Include/SimplePlotTools.h>

class WeightedMeanCalculator
{
public:
  TString fileName_LO_;
  TString fileName_weight_;

  TH2D* h_diMuRapPt_LO_;
  TH2D* h_diMuRapPt_weight_;

  WeightedMeanCalculator(TString fileName_LO, TString fileName_weight)
  {
    fileName_LO_ = fileName_LO;
    fileName_weight_ = fileName_weight;
  }

  void Calculate()
  {
    Init();

    Double_t nTotEvent = 0;
    Double_t weightedSum  = 0;

    Int_t nBinX = h_diMuRapPt_LO_->GetNbinsX();
    Int_t nBinY = h_diMuRapPt_LO_->GetNbinsY();

    for(Int_t i_x=0; i_x<nBinX; i_x++)
    {
      Int_t i_binX = i_x + 1;
      for(Int_t i_y=0; i_y<nBinY; i_y++)
      {
        Int_t i_binY = i_y + 1;

        Double_t nEvent_LO = h_diMuRapPt_LO_->GetBinContent(i_binX, i_binY);
        Double_t weight    = h_diMuRapPt_weight_->GetBinContent(i_binX, i_binY);

        nTotEvent    = nTotEvent    + nEvent_LO;
        weightedSum  = weightedSum  + nEvent_LO * weight;
      }
    }

    Double_t weightedMean = weightedSum / nTotEvent;

    printf("(nTotEvent, weightedSum) = (%lf, %lf) ---> weighted mean = %lf\n", nTotEvent, weightedSum, weightedMean);
  }

private:
  void Init()
  {
    h_diMuRapPt_LO_     = PlotTool::Get_Hist2D(fileName_LO_,      "h_diMuRapPt" );
    h_diMuRapPt_weight_ = PlotTool::Get_Hist2D(fileName_weight_,  "h_diMuRapPt_weight" );
  }

};

void Calculate_WeightedMeanForM14to15Event_withWeightsFromGivenMassRange(TString massRange)
{
  TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
  TString basePath = analyzerPath+"/NLOReweight/postFSR/Test_v01_SubMassRange/";

  TString fileName_LO = basePath+"/M14to15/LO/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";

  cout << "Mass range: " << massRange << endl;
  TString fileName_weight = basePath+"/"+massRange+"/Weight/ROOTFile_NLOWeight.root";

  WeightedMeanCalculator* calculator = new WeightedMeanCalculator(fileName_LO, fileName_weight);
  calculator->Calculate();
}

void Calculate_WeightedMeanForM10to11Event_withWeightsFromGivenMassRange(TString massRange)
{
  TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
  TString basePath = analyzerPath+"/NLOReweight/postFSR/Test_v01_SubMassRange/";

  TString fileName_LO = basePath+"/M10to11/LO/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";

  cout << "Mass range: " << massRange << endl;
  TString fileName_weight = basePath+"/"+massRange+"/Weight/ROOTFile_NLOWeight.root";

  WeightedMeanCalculator* calculator = new WeightedMeanCalculator(fileName_LO, fileName_weight);
  calculator->Calculate();
}

void Weight_weightedMean()
{
  Calculate_WeightedMeanForM14to15Event_withWeightsFromGivenMassRange("M10to15");
  Calculate_WeightedMeanForM14to15Event_withWeightsFromGivenMassRange("M14to15");

  Calculate_WeightedMeanForM10to11Event_withWeightsFromGivenMassRange("M10to11");
  Calculate_WeightedMeanForM10to11Event_withWeightsFromGivenMassRange("M10to15");
}