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

class TreeProducer: public DYTool::ClassTemplate
{
public:
  TreeProducer(): ClassTemplate()
  {

  }

  void Run()
  {
    CheckSampleInfo();
    StartTimer();

    InitSetup();

    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        Bool_t isDYEvent = kFALSE;
        DYTool::MuPair DYMuPair = DYTool::EventSelection_BDTInput(ntuple, isDYEvent);
        if( isDYEvent )
        {
          if( DYMuPair.mass > 50.0 ) continue; // -- because the DY sample is truncated at M=50 GeV

          diMuM_   = DYMuPair.mass;
          diMuRap_ = DYMuPair.rap;
          diMuPt_  = DYMuPair.pt;
          diMuNormVtxChi2_ = DYMuPair.normVtxChi2;
          diMuDEta_ = DYMuPair.dEta;
          diMuDPhi_ = DYMuPair.dPhi;

          caloMET_pt_ = ntuple->caloMET_pt;
          caloMET_phi_ = ntuple->caloMET_phi;

          nMuonHit_lead_        = DYMuPair.first_.nMuonHit;
          nMatchedStation_lead_ = DYMuPair.first_.nMatchedStation;
          nPixelHit_lead_       = DYMuPair.first_.nPixelHit;
          nTrackerLayer_lead_   = DYMuPair.first_.nTrackerLayer;
          normChi2_lead_        = DYMuPair.first_.normChi2;
          relTrkIso_lead_       = DYMuPair.first_.relTrkIso;

          nMuonHit_sub_        = DYMuPair.second_.nMuonHit;
          nMatchedStation_sub_ = DYMuPair.second_.nMatchedStation;
          nPixelHit_sub_       = DYMuPair.second_.nPixelHit;
          nTrackerLayer_sub_   = DYMuPair.second_.nTrackerLayer;
          normChi2_sub_        = DYMuPair.second_.normChi2;
          relTrkIso_sub_       = DYMuPair.second_.relTrkIso;

          tree_->Fill();
        }
      }
    }

    TString fileName = "ROOTFile_BDTInputTree_"+sampleInfo_.type+".root";
    TFile *f_output = TFile::Open(fileName, "RECREATE");
    f_output->cd();
    tree_->Write();
    f_output->Close();

    PrintRunTime();
  }

private:
  TTree *tree_;

  Double_t diMuM_;
  Double_t diMuRap_;
  Double_t diMuPt_;
  Double_t diMuNormVtxChi2_;
  Double_t diMuDEta_;
  Double_t diMuDPhi_;

  Double_t caloMET_pt_;
  Double_t caloMET_phi_;


  // -- single muon variables
  // -- leading muon
  Int_t nMuonHit_lead_;
  Int_t nMatchedStation_lead_;
  Int_t nPixelHit_lead_;
  Int_t nTrackerLayer_lead_;
  Double_t normChi2_lead_;
  Double_t relTrkIso_lead_;

  // -- sub-leading muon
  Int_t nMuonHit_sub_;
  Int_t nMatchedStation_sub_;
  Int_t nPixelHit_sub_;
  Int_t nTrackerLayer_sub_;
  Double_t normChi2_sub_;
  Double_t relTrkIso_sub_;


  void InitSetup()
  {
    InitVar();

    tree_ = new TTree("BDTInput", "BDT input tree");
    tree_->Branch("diMuM", &diMuM_, "diMuM/D");
    tree_->Branch("diMuRap", &diMuRap_, "diMuRap/D");
    tree_->Branch("diMuPt", &diMuPt_, "diMuPt/D");
    tree_->Branch("diMuNormVtxChi2", &diMuNormVtxChi2_, "diMuNormVtxChi2/D");
    tree_->Branch("diMuDEta", &diMuDEta_, "diMuDEta/D");
    tree_->Branch("diMuDPhi", &diMuDPhi_, "diMuDPhi/D");
    tree_->Branch("caloMET_pt", &caloMET_pt_, "caloMET_pt/D");
    tree_->Branch("caloMET_phi", &caloMET_phi_, "caloMET_phi/D");

    tree_->Branch("nMuonHit_lead", &nMuonHit_lead_, "nMuonHit_lead/I");
    tree_->Branch("nMatchedStation_lead", &nMatchedStation_lead_, "nMatchedStation_lead/I");
    tree_->Branch("nPixelHit_lead", &nPixelHit_lead_, "nPixelHit_lead/I");
    tree_->Branch("nTrackerLayer_lead", &nTrackerLayer_lead_, "nTrackerLayer_lead/I");
    tree_->Branch("normChi2_lead", &normChi2_lead_, "normChi2_lead/D");
    tree_->Branch("relTrkIso_lead", &relTrkIso_lead_, "relTrkIso_lead/D");

    tree_->Branch("nMuonHit_sub", &nMuonHit_sub_, "nMuonHit_sub/I");
    tree_->Branch("nMatchedStation_sub", &nMatchedStation_sub_, "nMatchedStation_sub/I");
    tree_->Branch("nPixelHit_sub", &nPixelHit_sub_, "nPixelHit_sub/I");
    tree_->Branch("nTrackerLayer_sub", &nTrackerLayer_sub_, "nTrackerLayer_sub/I");
    tree_->Branch("normChi2_sub", &normChi2_sub_, "normChi2_sub/D");
    tree_->Branch("relTrkIso_sub", &relTrkIso_sub_, "relTrkIso_sub/D");
  }

  void InitVar()
  {
    diMuM_ = 0;
    diMuRap_ = 0;
    diMuPt_ = 0;
    diMuNormVtxChi2_ = 0;
    diMuDEta_ = 0;
    diMuDPhi_ = 0;

    caloMET_pt_ = 0;
    caloMET_phi_ = 0;


    // -- single muon variables
    // -- leading muon
    nMuonHit_lead_ = 0;
    nMatchedStation_lead_ = 0;
    nPixelHit_lead_ = 0;
    nTrackerLayer_lead_ = 0;
    normChi2_lead_ = 0;
    relTrkIso_lead_ = 0;

    // -- sub-leading muon
    nMuonHit_sub_ = 0;
    nMatchedStation_sub_ = 0;
    nPixelHit_sub_ = 0;
    nTrackerLayer_sub_ = 0;
    normChi2_sub_ = 0;
    relTrkIso_sub_ = 0;
  }

};