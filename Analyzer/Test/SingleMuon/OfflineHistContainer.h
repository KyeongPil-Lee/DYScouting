#include <Include/DYTool.h>

class HistContainer
{
public:
  vector<TH1D*> vec_hists_;

  // -- basic kinematic distributions
  TH1D* h_pt_;
  TH1D* h_pt_lead_;
  TH1D* h_pt_sub_;

  TH1D* h_eta_;
  TH1D* h_eta_lead_;
  TH1D* h_eta_sub_;

  TH1D* h_phi_;
  TH1D* h_phi_lead_;
  TH1D* h_phi_sub_;

  TH1D* h_diMuM_;
  TH1D* h_diMuM_BB_; // -- barrel-barrel
  TH1D* h_diMuM_BE_; // -- barrel-endcap
  TH1D* h_diMuM_EE_; // -- endcap-endcap
  TH1D* h_diMuM_lowM_fineBin_;
  TH1D* h_diMuM_Upsilon_;
  TH1D* h_diMuRap_;
  TH1D* h_diMuPt_;
  TH1D* h_diMuOS_;
  TH1D* h_diMuDEta_;
  TH1D* h_diMuDPhi_;
  TH1D* h_diMuAngle3D_;
  TH1D* h_diMuNormVtxChi2_;
  // -- cut variables
  TH1D* h_nMuonHit_;
  TH1D* h_nMatchedStation_;
  TH1D* h_nPixelHit_;
  TH1D* h_nTrackerLayer_;
  TH1D* h_normChi2_;
  TH1D* h_dxy_;
  TH1D* h_dz_;

  TH1D* h_relTrkIso_;

  HistContainer()
  {
    Init();
  }

  void Fill( DYTool::DYTree *ntuple, DYTool::OffMuPair& muPair, Double_t weight )
  {
    Fill_SingleMu( muPair.first_, weight);
    Fill_SingleMu( muPair.second_, weight );

    h_pt_lead_->Fill( muPair.first_.pt, weight );
    h_pt_sub_ ->Fill( muPair.second_.pt, weight );

    h_eta_lead_->Fill( muPair.first_.eta, weight );
    h_eta_sub_ ->Fill( muPair.second_.eta, weight );

    h_phi_lead_->Fill( muPair.first_.phi, weight );
    h_phi_sub_ ->Fill( muPair.second_.phi, weight );

    h_diMuM_->Fill( muPair.mass, weight );
    if( fabs(muPair.first_.eta) < 1.2 && fabs(muPair.second_.eta) < 1.2 )      h_diMuM_BB_->Fill( muPair.mass, weight );
    else if( fabs(muPair.first_.eta) > 1.2 && fabs(muPair.second_.eta) > 1.2 ) h_diMuM_EE_->Fill( muPair.mass, weight );
    else                                                                       h_diMuM_BE_->Fill( muPair.mass, weight );
    h_diMuM_lowM_fineBin_->Fill( muPair.mass, weight );
    h_diMuM_Upsilon_->Fill( muPair.mass, weight );
    h_diMuRap_->Fill( muPair.rap, weight );
    h_diMuPt_->Fill( muPair.pt, weight );
    h_diMuOS_->Fill( muPair.isOS, weight );
    h_diMuDEta_->Fill( muPair.dEta, weight );
    h_diMuDPhi_->Fill( muPair.dPhi, weight );
    h_diMuAngle3D_->Fill( muPair.angle3D, weight );
    h_diMuNormVtxChi2_->Fill( muPair.normVtxChi2, weight );
  }

  void Write(TFile *f_output)
  {
    for(const auto& h: vec_hists_ )
      h->Write();
  }

private:
  void Init()
  {
    h_pt_      = new TH1D("h_pt",      "", 10000, 0, 10000);
    h_pt_lead_ = new TH1D("h_pt_lead", "", 10000, 0, 10000);
    h_pt_sub_  = new TH1D("h_pt_sub",  "", 10000, 0, 10000);

    h_eta_      = new TH1D("h_eta",      "", 60, -3, 3);
    h_eta_lead_ = new TH1D("h_eta_lead", "", 60, -3, 3);
    h_eta_sub_  = new TH1D("h_eta_sub",  "", 60, -3, 3);

    h_phi_      = new TH1D("h_phi",      "", 80, -4, 4);
    h_phi_lead_ = new TH1D("h_phi_lead", "", 80, -4, 4);
    h_phi_sub_  = new TH1D("h_phi_sub",  "", 80, -4, 4);

    h_diMuM_                = new TH1D("h_diMuM",             "", 10000, 0, 10000);
    h_diMuM_BB_             = new TH1D("h_diMuM_BB",          "", 10000, 0, 10000);
    h_diMuM_BE_             = new TH1D("h_diMuM_BE",          "", 10000, 0, 10000);
    h_diMuM_EE_             = new TH1D("h_diMuM_EE",          "", 10000, 0, 10000);
    h_diMuM_lowM_fineBin_   = new TH1D("h_diMuM_lowM_fineBin","", 200, 0, 20);
    h_diMuM_Upsilon_        = new TH1D("h_diMuM_Upsilon",     "", 600, 7, 13); // -- 0.01 GeV bin size
    h_diMuPt_  = new TH1D("h_diMuPt",   "", 10000, 0, 10000);
    h_diMuRap_ = new TH1D("h_diMuRap", "", 60, -3, 3);
    h_diMuOS_  = new TH1D("h_diMuOS", "", 2, 0, 2);
    h_diMuDEta_     = new TH1D("h_diMuDEta",    "", 60, 0, 6);
    h_diMuDPhi_     = new TH1D("h_diMuDPhi",    "", 40, 0, 4);
    h_diMuAngle3D_  = new TH1D("h_diMuAngle3D", "", 40, 0, 4);
    h_diMuNormVtxChi2_ = new TH1D("h_diMuNormVtxChi2", "", 40, 0, 40);

    h_nMuonHit_        = new TH1D("h_nMuonHit",        "", 100, 0, 100);
    h_nMatchedStation_ = new TH1D("h_nMatchedStation", "", 10, 0, 10);
    h_nPixelHit_       = new TH1D("h_nPixelHit",       "", 15, 0, 15);
    h_nTrackerLayer_   = new TH1D("h_nTrackerLayer",   "", 25, 0, 25);
    h_normChi2_        = new TH1D("h_normChi2",        "", 200, 0, 20);
    h_dxy_             = new TH1D("h_dxy",             "", 60, -0.3, 0.3);
    h_dz_              = new TH1D("h_dz",              "", 200, -1.0, 1.0);
    h_relTrkIso_       = new TH1D("h_relTrkIso",       "", 50, 0, 0.5);


    vec_hists_.push_back(h_pt_);
    vec_hists_.push_back(h_pt_lead_);
    vec_hists_.push_back(h_pt_sub_);

    vec_hists_.push_back(h_eta_);
    vec_hists_.push_back(h_eta_lead_);
    vec_hists_.push_back(h_eta_sub_);

    vec_hists_.push_back(h_phi_);
    vec_hists_.push_back(h_phi_lead_);
    vec_hists_.push_back(h_phi_sub_);

    vec_hists_.push_back(h_diMuM_);
    vec_hists_.push_back(h_diMuM_BB_);
    vec_hists_.push_back(h_diMuM_BE_);
    vec_hists_.push_back(h_diMuM_EE_);
    vec_hists_.push_back(h_diMuM_lowM_fineBin_);
    vec_hists_.push_back(h_diMuM_Upsilon_);
    vec_hists_.push_back(h_diMuRap_);
    vec_hists_.push_back(h_diMuPt_);
    vec_hists_.push_back(h_diMuOS_);
    vec_hists_.push_back(h_diMuDEta_);
    vec_hists_.push_back(h_diMuDPhi_);
    vec_hists_.push_back(h_diMuAngle3D_);
    vec_hists_.push_back(h_diMuNormVtxChi2_);

    vec_hists_.push_back(h_nMuonHit_);
    vec_hists_.push_back(h_nMatchedStation_);
    vec_hists_.push_back(h_nPixelHit_);
    vec_hists_.push_back(h_nTrackerLayer_);
    vec_hists_.push_back(h_normChi2_);
    vec_hists_.push_back(h_dxy_);
    vec_hists_.push_back(h_dz_);
    vec_hists_.push_back(h_relTrkIso_);

    for(const auto& h: vec_hists_ )
      h->Sumw2();
  }


  void Fill_SingleMu(DYTool::OffMuon& mu, Double_t weight)
  {
    h_pt_->Fill( mu.pt, weight );
    h_eta_->Fill( mu.eta, weight );
    h_phi_->Fill( mu.phi, weight );

    h_nMuonHit_        ->Fill(mu.nMuonHit_global, weight );
    h_nMatchedStation_ ->Fill(mu.nMatchedStation, weight );
    h_dxy_             ->Fill(mu.dxyVTX_best, weight );
    h_dz_              ->Fill(mu.dzVTX_best, weight );
    h_nPixelHit_       ->Fill(mu.nPixelHit_inner, weight );
    h_normChi2_        ->Fill(mu.normChi2_global, weight );
    h_nTrackerLayer_   ->Fill(mu.nTrackerLayer_inner, weight );
    h_relTrkIso_       ->Fill(mu.relTrkIso, weight );
  }

};
