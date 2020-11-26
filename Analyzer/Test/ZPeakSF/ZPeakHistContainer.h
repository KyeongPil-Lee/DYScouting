#include <Include/DYTool.h>

// -- histogram for the dimuon events in 81 < m < 101 GeV
class ZPeakHistContainer
{
public:
  TString type_ = "";

  vector<TH1D*> vec_hist_;

  TH1D* h_mu_pt_;
  TH1D* h_mu_eta_;
  TH1D* h_mu_phi_;

  TH1D* h_mu_pt_lead_;
  TH1D* h_mu_eta_lead_;
  TH1D* h_mu_phi_lead_;

  TH1D* h_mu_pt_sub_;
  TH1D* h_mu_eta_sub_;
  TH1D* h_mu_phi_sub_;

  TH1D* h_diMu_mass_;
  TH1D* h_diMu_rap_;
  TH1D* h_diMu_pt_;

  ZPeakHistContainer(TString type)
  {
    type_ = type;
    Init();
  }

  void Fill(DYTool::MuPair& muPair, Double_t weight)
  {
    Fill_SingleMu( muPair.first_, weight );
    Fill_SingleMu( muPair.second_, weight );

    h_mu_pt_lead_->Fill( muPair.first_.pt, weight );
    h_mu_eta_lead_->Fill( muPair.first_.eta, weight );
    h_mu_phi_lead_->Fill( muPair.first_.phi, weight );

    h_mu_pt_sub_->Fill( muPair.second_.pt, weight );
    h_mu_eta_sub_->Fill( muPair.second_.eta, weight );
    h_mu_phi_sub_->Fill( muPair.second_.phi, weight );

    h_diMu_mass_->Fill( muPair.mass, weight );
    h_diMu_rap_->Fill( muPair.rap, weight );
    h_diMu_pt_->Fill( muPair.pt, weight );
  }

  void Fill_SingleMu(DYTool::Muon& mu, Double_t weight)
  {
    h_mu_pt_->Fill( mu.pt, weight );
    h_mu_eta_->Fill( mu.eta, weight );
    h_mu_phi_->Fill( mu.phi, weight );
  }

  void Write(TFile *f_output)
  {
    for(const auto& h: vec_hist_ )
      h->Write();

    cout << "[ZPeakHistContainer (type = " << type_ << ")]: saved in " << f_output->GetName() << endl;
  }


private:
  void Init()
  {
    TH1::SetDefaultSumw2(kTRUE);
    TH1::AddDirectory(kFALSE);

    h_mu_pt_  = new TH1D("h_mu_pt", "", 10000, 0, 10000); vec_hist_.push_back( h_mu_pt_ );
    h_mu_eta_ = new TH1D("h_mu_eta", "", 60, -3, 3);      vec_hist_.push_back( h_mu_eta_ );
    h_mu_phi_ = new TH1D("h_mu_phi", "", 80, -4, 4);      vec_hist_.push_back( h_mu_phi_ );

    h_mu_pt_lead_  = new TH1D("h_mu_pt_lead", "", 10000, 0, 10000); vec_hist_.push_back( h_mu_pt_lead_ );
    h_mu_eta_lead_ = new TH1D("h_mu_eta_lead", "", 60, -3, 3);      vec_hist_.push_back( h_mu_eta_lead_ );
    h_mu_phi_lead_ = new TH1D("h_mu_phi_lead", "", 80, -4, 4);      vec_hist_.push_back( h_mu_phi_lead_ );

    h_mu_pt_sub_  = new TH1D("h_mu_pt_sub", "", 10000, 0, 10000); vec_hist_.push_back( h_mu_pt_sub_ );
    h_mu_eta_sub_ = new TH1D("h_mu_eta_sub", "", 60, -3, 3);      vec_hist_.push_back( h_mu_eta_sub_ );
    h_mu_phi_sub_ = new TH1D("h_mu_phi_sub", "", 80, -4, 4);      vec_hist_.push_back( h_mu_phi_sub_ );

    h_diMu_mass_ = new TH1D("h_diMu_mass", "", 20, 81, 101);   vec_hist_.push_back( h_diMu_mass_ );
    h_diMu_rap_  = new TH1D("h_diMu_rap", "", 60, -3, 3);      vec_hist_.push_back( h_diMu_rap_ );
    h_diMu_pt_   = new TH1D("h_diMu_pt", "", 10000, 0, 10000); vec_hist_.push_back( h_diMu_pt_ );

    // -- add _(type) at the end of histogram name
    if( type_ != "" )
    {
      for(auto h : vec_hist_ )
      {
        TString histName_before = h->GetName();
        h->SetName( TString::Format("%s_%s", histName_before.Data(), type_.Data()) )
      }
    }
  }

};