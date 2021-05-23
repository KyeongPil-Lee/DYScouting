class HistAttribute
{
public:
  TString histName_ = "";

  TString titleX_ = "";
  TString titleY_ = "";

  Bool_t setRangeX_ = kFALSE;
  Double_t minX_;
  Double_t maxX_;

  Bool_t setRangeY_ = kFALSE;
  Double_t minY_;
  Double_t maxY_;

  Bool_t setRebin_ = kFALSE;
  Int_t nRebin_;

  HistAttribute()
  {

  }

  void Set_HistName(TString histName) {histName_ = histName; }
  void Set_TitleX(TString titleX) {titleX_ = titleX; }
  void Set_TitleY(TString titleY) {titleY_ = titleY; }

  void Set_RangeX(Double_t minX, Double_t maxX)
  {
    setRangeX_ = kTRUE;
    minX_ = minX;
    maxX_ = maxX;
  }

  void Set_RangeY(Double_t minY, Double_t maxY)
  {
    setRangeY_ = kTRUE;
    minY_ = minY;
    maxY_ = maxY;
  }

  void Set_Rebin(Int_t nRebin)
  {
    setRebin_ = kTRUE;
    nRebin_ = nRebin;
  }

private:

};

vector<HistAttribute> Get_ZPeakHistAttribute()
{
  vector<HistAttribute> vec_histAtt;
  // -- single muon variables
  HistAttribute histAtt_pt;
  histAtt_pt.Set_HistName("h_mu_pt");
  histAtt_pt.Set_TitleX("p_{T}(#mu) [GeV]");
  histAtt_pt.Set_RangeX(0, 1000);
  vec_histAtt.push_back( histAtt_pt );

  HistAttribute histAtt_eta;
  histAtt_eta.Set_HistName("h_mu_eta");
  histAtt_eta.Set_TitleX("#eta(#mu)");
  histAtt_eta.Set_RangeX(-2.4, 2.4);
  vec_histAtt.push_back( histAtt_eta );

  HistAttribute histAtt_phi;
  histAtt_phi.Set_HistName("h_mu_phi");
  histAtt_phi.Set_TitleX("#phi(#mu)");
  vec_histAtt.push_back( histAtt_phi );

  // -- leading muon
  HistAttribute histAtt_pt_lead;
  histAtt_pt_lead.Set_HistName("h_mu_pt_lead");
  histAtt_pt_lead.Set_TitleX("p_{T}(#mu^{lead}) [GeV]");
  histAtt_pt_lead.Set_RangeX(0, 1000);
  vec_histAtt.push_back( histAtt_pt_lead );

  HistAttribute histAtt_eta_lead;
  histAtt_eta_lead.Set_HistName("h_mu_eta_lead");
  histAtt_eta_lead.Set_TitleX("#eta(#mu^{lead})");
  histAtt_eta_lead.Set_RangeX(-2.4, 2.4);
  vec_histAtt.push_back( histAtt_eta_lead );

  HistAttribute histAtt_phi_lead;
  histAtt_phi_lead.Set_HistName("h_mu_phi_lead");
  histAtt_phi_lead.Set_TitleX("#phi(#mu^{lead})");
  vec_histAtt.push_back( histAtt_phi_lead );

  // -- sub-leading muon
  HistAttribute histAtt_pt_sub;
  histAtt_pt_sub.Set_HistName("h_mu_pt_sub");
  histAtt_pt_sub.Set_TitleX("p_{T}(#mu^{sub}) [GeV]");
  histAtt_pt_sub.Set_RangeX(0, 1000);
  vec_histAtt.push_back( histAtt_pt_sub );

  HistAttribute histAtt_eta_sub;
  histAtt_eta_sub.Set_HistName("h_mu_eta_sub");
  histAtt_eta_sub.Set_TitleX("#eta(#mu^{sub})");
  histAtt_eta_sub.Set_RangeX(-2.4, 2.4);
  vec_histAtt.push_back( histAtt_eta_sub );

  HistAttribute histAtt_phi_sub;
  histAtt_phi_sub.Set_HistName("h_mu_phi_sub");
  histAtt_phi_sub.Set_TitleX("#phi(#mu^{sub})");
  vec_histAtt.push_back( histAtt_phi_sub );

  // -- dimuon variables
  HistAttribute histAtt_diMu_pt;
  histAtt_diMu_pt.Set_HistName("h_diMu_pt");
  histAtt_diMu_pt.Set_TitleX("p_{T}(#mu#mu) [GeV]");
  histAtt_diMu_pt.Set_RangeX(0, 1000);
  vec_histAtt.push_back( histAtt_diMu_pt );

  HistAttribute histAtt_diMu_rap;
  histAtt_diMu_rap.Set_HistName("h_diMu_rap");
  histAtt_diMu_rap.Set_TitleX("Y(#mu#mu)");
  histAtt_diMu_rap.Set_RangeX(-2.4, 2.4);
  vec_histAtt.push_back( histAtt_diMu_rap );

  HistAttribute histAtt_diMu_mass;
  histAtt_diMu_mass.Set_HistName("h_diMu_mass");
  histAtt_diMu_mass.Set_TitleX("m(#mu#mu) [GeV]");
  histAtt_diMu_mass.Set_RangeX(81, 101);
  vec_histAtt.push_back( histAtt_diMu_mass );

  return vec_histAtt;
}

