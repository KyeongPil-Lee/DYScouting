#include <Include/DYTool.h>
#include <Include/SimplePlotTools.h>

class NLOWeightTool
{
public:
  TH2D* h2D_weight_;

  Int_t nBinX_;
  Int_t nBinY_;

  NLOWeightTool()
  {
    cout << "[NLOWeightTool] Default constructor: use the default ROOT file" << endl;
    TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
    TString rootFilePath = analyzerPath+"/NLOReweight/postFSR/Weight/MassDepW/Test/v01_M10to15/ROOTFile_NLOWeight.root";
    cout << "---> root file path = " << rootFilePath << endl;

    h2D_weight_ = PlotTool::Get_Hist2D(rootFilePath, "h_diMuRapPt_weight");
    nBinX_ = h2D_weight_->GetNbinsX();
    nBinY_ = h2D_weight_->GetNbinsY();
  }

  NLOWeightTool(TString fileName, TString histName)
  {
    cout << "[NLOWeightTool] Private ROOT file for weight" << endl;
    cout << "---> root file path = " << fileName << ", histName = " << histName << endl;

    h2D_weight_ = PlotTool::Get_Hist2D(fileName, histName);
    nBinX_ = h2D_weight_->GetNbinsX();
    nBinY_ = h2D_weight_->GetNbinsY();
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
    return GetWeight( genPair.pt, genPair.rap );
  }

  Double_t GetWeight( Double_t diMuPt, Double_t diMuRap )
  {    
    // -- x-axis: dimuon rapidity
    Int_t i_binX = h2D_weight_->GetXaxis()->FindBin(diMuRap);

    // -- under/overflow: extrapolation
    if( i_binX == 0 )       i_binX = 1;
    if (i_binX == nBinX_+1) i_binX = nBinX_;

    // -- y-axis: dimuon pt
    Int_t i_binY = h2D_weight_->GetYaxis()->FindBin(diMuPt);

    // -- under/overflow: extrapolation
    if( i_binY == 0 )       i_binY = 1;
    if (i_binY == nBinY_+1) i_binY = nBinY_;

    return h2D_weight_->GetBinContent(i_binX, i_binY);
  }

private:

};