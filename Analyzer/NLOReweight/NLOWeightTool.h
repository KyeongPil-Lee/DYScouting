#include <Include/DYTool.h>
#include <Include/SimplePlotTools.h>

class NLOWeightTool
{
public:
  vector<Double_t> vec_massBinEdge_;
  Int_t nMassBin_;

  vector<TH2D*> vec_h2D_weight_;

  Int_t nBinX_;
  Int_t nBinY_;

  NLOWeightTool()
  {
    cout << "[NLOWeightTool] Default constructor: use the default ROOT file" << endl;
    TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
    TString rootFilePath = analyzerPath+"/NLOReweight/ROOTFile_NLOWeight.root";
    cout << "---> root file path = " << rootFilePath << endl;

    vector<Double_t> vec_temp = {
      10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
      23, 26, 29, 32, 35, 38, 41, 44, 47, 50};
    vec_massBinEdge_ = vec_temp;

    nMassBin_ = (Int_t)vec_massBinEdge_.size() - 1;
    for(Int_t i=0; i<nMassBin_; i++)
    {
      Double_t minM = vec_massBinEdge_[i];
      Double_t maxM = vec_massBinEdge_[i+1];

      TString dirName = TString::Format("M%0.lfto%0.lf", minM, maxM);
      vec_h2D_weight_.push_back( PlotTool::Get_Hist2D(rootFilePath, dirName+"/h_diMuRapPt_weight") );
    }

    // -- assume that pt-rapidity bin is same for all mass bins
    nBinX_ = vec_h2D_weight_[0]->GetNbinsX();
    nBinY_ = vec_h2D_weight_[0]->GetNbinsY();
  }

  NLOWeightTool(TString fileName, TString histName)
  {
    vector<Double_t> vec_temp = {
      10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
      23, 26, 29, 32, 35, 38, 41, 44, 47, 50};
    vec_massBinEdge_ = vec_temp;

    cout << "[NLOWeightTool] Private ROOT file for weight" << endl;
    cout << "---> root file path = " << fileName << ", histName = " << histName << endl;

    nMassBin_ = (Int_t)vec_massBinEdge_.size() - 1;
    for(Int_t i=0; i<nMassBin_; i++)
    {
      Double_t minM = vec_massBinEdge_[i];
      Double_t maxM = vec_massBinEdge_[i+1];

      TString dirName = TString::Format("M%0.lfto%0.lf", minM, maxM);
      vec_h2D_weight_.push_back( PlotTool::Get_Hist2D(fileName, dirName+"/"+histName) );
    }

    // -- assume that pt-rapidity bin is same for all mass bins
    nBinX_ = vec_h2D_weight_[0]->GetNbinsX();
    nBinY_ = vec_h2D_weight_[0]->GetNbinsY();
  }

  Double_t GetWeight( DYTool::DYTree *ntuple )
  {
    vector<DYTool::GenParticle> vec_genLeptonHPFS = DYTool::GetAllGenLeptons(ntuple, 13, "fromHardProcessFinalState");

    if( vec_genLeptonHPFS.size() != 2 )
    {
      cout << "[NLOWeightTool::GetWeight] vec_genLeptonHPFS.size() == " << vec_genLeptonHPFS.size() << "!= 2 ... need to check";
      cout << "---> return -1" << endl;
      return -1;
    }

    DYTool::GenPair genPair(vec_genLeptonHPFS[0], vec_genLeptonHPFS[1]);
    return GetWeight( genPair.mass, genPair.pt, genPair.rap );
  }

  Double_t GetWeight( Double_t diMuM, Double_t diMuPt, Double_t diMuRap )
  {
    Int_t massBinIndex = -1;    
    for(Int_t i_mass=0; i_mass<nMassBin_; i_mass++)
    {
      Double_t minM = vec_massBinEdge_[i_mass];
      Double_t maxM = vec_massBinEdge_[i_mass+1];
      if( minM < diMuM && diMuM < maxM )
      {
        massBinIndex = i_mass;
        break;
      }
    }

    if( massBinIndex == -1 )
    {
      cout << "diMuM = " << diMuM << " is out of mass range for weight value" << endl;
      cout << " ---> weight = 1" << endl;
      return 1.0;
    }

    // -- x-axis: dimuon rapidity
    Int_t i_binX = vec_h2D_weight_[massBinIndex]->GetXaxis()->FindBin(diMuRap);

    // -- under/overflow: extrapolation
    if( i_binX == 0 )       i_binX = 1;
    if (i_binX == nBinX_+1) i_binX = nBinX_;

    // -- y-axis: dimuon pt
    Int_t i_binY = vec_h2D_weight_[massBinIndex]->GetYaxis()->FindBin(diMuPt);

    // -- under/overflow: extrapolation
    if( i_binY == 0 )       i_binY = 1;
    if (i_binY == nBinY_+1) i_binY = nBinY_;

    return vec_h2D_weight_[massBinIndex]->GetBinContent(i_binX, i_binY);
  }

private:

};