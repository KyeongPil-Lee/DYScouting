class TnPEffTool
{
public:
  TString effType_;

  TH2F* h_eff_data_;
  TH2F* h_eff_MC_;

  Int_t nBin_X_;
  Int_t nBin_Y_;

  TnPEffTool(TString effType)
  {
    effType_ = effType;
    Init();
  }

  Double_t GetEff(Bool_t isMC, Double_t pt, Double_t abseta)
  {
    Double_t theEff = -1;

    // -- x-axis: muon |eta|
    Int_t theBinX = h_eff_MC_->GetXaxis()->FindBin(abseta);

    // -- under/overflow: extrapolation
    if( theBinX == 0 )        theBinX = 1;
    if (theBinX == nBin_X_+1) theBinX = nBin_X_;

    // -- y-axis: muon pt
    Int_t theBinY = h_eff_MC_->GetYaxis()->FindBin(pt);

    // -- under/overflow: extrapolation
    if( theBinY == 0 )        theBinY = 1;
    if (theBinY == nBin_Y_+1) theBinY = nBin_Y_;

    if( isMC ) theEff = h_eff_MC_->GetBinContent(theBinX, theBinY);
    else       theEff = h_eff_data_->GetBinContent(theBinX, theBinY);

    return theEff;
  }

private:
  void Init()
  {
    TH1::AddDirectory(kFALSE);

    TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
    TString basePath = analyzerPath+"/Test/TrigEff_Orthogonal/EffSF";

    TString rootFileName = "";
    TString histName_eff_data = "";
    TString histName_eff_MC = "";
    if( effType_ == "mediumID" )
    {
      rootFileName = basePath+"/Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root";
      histName_eff_data = "NUM_MediumID_DEN_TrackerMuons_abseta_pt_efficiencyData";
      histName_eff_MC   = "NUM_MediumID_DEN_TrackerMuons_abseta_pt_efficiencyMC";
    }
    if( effType_ == "loosePFIso_over_mediumID" )
    {
      rootFileName = basePath+"/Efficiencies_muon_generalTracks_Z_Run2018_UL_ISO.root";
      histName_eff_data = "NUM_LooseRelIso_DEN_MediumID_abseta_pt_efficiencyData";
      histName_eff_MC   = "NUM_LooseRelIso_DEN_MediumID_abseta_pt_efficiencyMC";
    }

    TFile *f_input = TFile::Open(rootFileName);
    h_eff_data_ = (TH2F*)f_input->Get(histName_eff_data)->Clone();
    h_eff_MC_   = (TH2F*)f_input->Get(histName_eff_MC)->Clone();

    nBin_X_ = h_eff_MC_->GetNbinsX();
    nBin_Y_ = h_eff_MC_->GetNbinsY();
  }
};