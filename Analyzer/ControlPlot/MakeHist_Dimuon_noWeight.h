#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TEfficiency.h>

#include <vector>

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
  TH1D* h_diMuRap_;
  TH1D* h_diMuPt_;
  TH1D* h_diMuOS_;
  TH1D* h_diMuDEta_;
  TH1D* h_diMuDPhi_;
  TH1D* h_diMuAngle3D_;

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

  void Fill( DYTool::MuPair& muPair, Double_t weight )
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
    h_diMuRap_->Fill( muPair.rap, weight );
    h_diMuPt_->Fill( muPair.pt, weight );
    h_diMuOS_->Fill( muPair.isOS, weight );
    h_diMuDEta_->Fill( muPair.dEta, weight );
    h_diMuDPhi_->Fill( muPair.dPhi, weight );
    h_diMuAngle3D_->Fill( muPair.angle3D, weight );
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

    h_diMuM_   = new TH1D("h_diMuM",     "", 10000, 0, 10000);
    h_diMuPt_  = new TH1D("h_diMuPt",   "", 10000, 0, 10000);
    h_diMuRap_ = new TH1D("h_diMuRap", "", 60, -3, 3);
    h_diMuOS_  = new TH1D("h_diMuOS", "", 2, 0, 2);
    h_diMuDEta_     = new TH1D("h_diMuDEta",    "", 60, 0, 6);
    h_diMuDPhi_     = new TH1D("h_diMuDPhi",    "", 40, 0, 4);
    h_diMuAngle3D_  = new TH1D("h_diMuAngle3D", "", 40, 0, 4);

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
    vec_hists_.push_back(h_diMuRap_);
    vec_hists_.push_back(h_diMuPt_);
    vec_hists_.push_back(h_diMuOS_);
    vec_hists_.push_back(h_diMuDEta_);
    vec_hists_.push_back(h_diMuDPhi_);
    vec_hists_.push_back(h_diMuAngle3D_);

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


  void Fill_SingleMu(DYTool::Muon& mu, Double_t weight)
  {
    h_pt_->Fill( mu.pt, weight );
    h_eta_->Fill( mu.eta, weight );
    h_phi_->Fill( mu.phi, weight );

    h_nMuonHit_        ->Fill(mu.nMuonHit, weight );
    h_nMatchedStation_ ->Fill(mu.nMatchedStation, weight );
    h_dxy_             ->Fill(mu.dxy, weight );
    h_dz_              ->Fill(mu.dz, weight );
    h_nPixelHit_       ->Fill(mu.nPixelHit, weight );
    h_normChi2_        ->Fill(mu.normChi2, weight );
    h_nTrackerLayer_   ->Fill(mu.nTrackerLayer, weight );
    h_relTrkIso_       ->Fill(mu.relTrkIso, weight );
  }

};

class HistProducer: public DYTool::ClassTemplate
{
public:
  HistProducer(): ClassTemplate()
  {

  }

  void Run()
  {
    CheckSampleInfo();
    StartTimer();

    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");

    HistContainer* hists         = new HistContainer();
    HistContainer* hists_ZPeak   = new HistContainer();
    HistContainer* hists_M10to60 = new HistContainer();

    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      // Double_t totWeight = sampleInfo_.normFactor * genWeight;
      Double_t totWeight = genWeight;
      if( sampleInfo_.isMC ) totWeight = genWeight * PUTool->Weight( ntuple->truePU );

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        Bool_t isDYEvent = kFALSE;
        DYTool::MuPair DYMuPair = DYTool::EventSelection(ntuple, isDYEvent);
        if( isDYEvent )
        {
          Double_t mass = DYMuPair.mass;
          if( mass > 10 )
            hists->Fill(DYMuPair, totWeight);

          if( 60 < mass && mass < 120 )
            hists_ZPeak->Fill(DYMuPair, totWeight);

          if( 10 < mass && mass < 60 )
            hists_M10to60->Fill(DYMuPair, totWeight);
        }
      }
    }

    TString outputName = TString::Format("ROOTFile_MakeHist_Dimuon_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    hists->Write(f_output);

    f_output->mkdir("ZPeak");
    f_output->cd("ZPeak");
    hists_ZPeak->Write(f_output);

    f_output->mkdir("M10to60");
    f_output->cd("M10to60");
    hists_M10to60->Write(f_output);

    f_output->Close();

    PrintRunTime();
  }

};

void Test()
{
  HistProducer* producer = new HistProducer();

  producer->sampleInfo_.type = "DYMuMu_M10to50_NLOXSec";
  producer->sampleInfo_.ntuplePathFile = "/Users/KyeongPil_Lee/Physics/DYScouting/Analyzer/Include/Example/ntuplePath_example.txt";
  producer->sampleInfo_.xSec = 10000;
  producer->sampleInfo_.sumWeight = 10000;
  Double_t lumi = 999;
  producer->sampleInfo_.normFactor = (lumi * 10000.0 ) / 10000.0;

  producer->Run();
}