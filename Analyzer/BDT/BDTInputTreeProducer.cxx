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

class TreeProducer
{
public:
  TString sampleType_;
  Int_t nNeededEvent_;

  TreeProducer( TString sampleType, Int_t nNeededEvent )
  {
    sampleType_ = sampleType;
    nNeededEvent_ = nNeededEvent;
  }

  void Produce()
  {
    InitSetup();

    TString ntupleBasePath = gSystem->Getenv("DY_NTUPLE_PATH");
    TString ntuplePath = ntupleBasePath + "/v1.2/" + sampleType_ + "/*.root";

    TChain *chain = new TChain("DYTree/ntuple");
    chain->Add(ntuplePath);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );
    
    Int_t nEvent = chain->GetEntries();
    printf("ntuple path = %s\n", ntuplePath.Data());
    printf("  -> # total events in the chain = %d: save only %d events passing baseline DY selection\n", nEvent, nNeededEvent_);

    Int_t nSavedEvent = 0;
    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      TString type = sampleType_;
      if( sampleType_ == "DYJetsToLL_M10to50_Madgraph" ) type = "DYMuMu_M10to50";

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(type, ntuple) )
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
          nSavedEvent++;
        }
      } // -- end of sample type selection

      if( nSavedEvent == nNeededEvent_ ) break;

    } // -- end of event iteration

    if( nSavedEvent < nNeededEvent_ )
      cout << "+++ can't find " << nNeededEvent_ << " events even though full # events were used ... # saved events = " << nSavedEvent << endl;

    TString fileName = "ROOTFile_BDTInputTree_"+sampleType_+".root";
    TFile *f_output = TFile::Open(fileName, "RECREATE");
    f_output->cd();
    tree_->Write();
    f_output->Close();

    cout << "The tree is saved in " << fileName << endl;
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

Double_t Get_xSec(TString type)
{
  Double_t xSec = 0;

  if( type == "QCDMuEnriched_Pt20toInf" )        xSec = 239400.0;
  else if( type == "QCDMuEnriched_Pt15to20" )    xSec = 2799000.0;
  else if( type == "QCDMuEnriched_Pt20to30" )    xSec = 2526000.0;
  else if( type == "QCDMuEnriched_Pt30to50" )    xSec = 1362000.0;
  else if( type == "QCDMuEnriched_Pt50to80" )    xSec = 376600.0;
  else if( type == "QCDMuEnriched_Pt80to120" )   xSec = 88930.0;
  else if( type == "QCDMuEnriched_Pt120to170" )  xSec = 21230.0;
  else if( type == "QCDMuEnriched_Pt170to300" )  xSec = 7055.0;
  else if( type == "QCDMuEnriched_Pt300to470" )  xSec = 797.3; // -- TuneCUETP8M1
  else if( type == "QCDMuEnriched_Pt470to600" )  xSec = 59.24;
  else if( type == "QCDMuEnriched_Pt600to800" )  xSec = 25.25; // -- TuneCUETP8M1
  else if( type == "QCDMuEnriched_Pt800to1000" ) xSec = 4.723; // -- TuneCUETP8M1
  else if( type == "QCDMuEnriched_Pt1000toInf" ) xSec = 1.613; // -- TuneCUETP8M1

  return xSec;
}

Int_t CalcNEvent_QCD(TString type, Int_t nTotEvent = 100000)
{
  Double_t xSec = Get_xSec(type);

  // vector<TString> vec_ptRange = {
  //   "Pt15to20", "Pt20to30", "Pt30to50", "Pt50to80", 
  //   "Pt80to120", "Pt120to170", "Pt170to300", "Pt300to470", 
  //   "Pt470to600", "Pt600to800", "Pt800to1000", "Pt1000toInf"
  // };

  // -- remove Pt15to20: only ~5000 events are included in the baseline selection
  vector<TString> vec_ptRange = {
    "Pt20to30", "Pt30to50", "Pt50to80", 
    "Pt80to120", "Pt120to170", "Pt170to300", "Pt300to470", 
    "Pt470to600", "Pt600to800", "Pt800to1000", "Pt1000toInf"
  };

  Double_t xSec_sum = 0;
  for(UInt_t i=0; i<vec_ptRange.size(); i++)
  {
    TString sampleType = "QCDMuEnriched_"+vec_ptRange[i];
    xSec_sum += Get_xSec(sampleType);
    cout << "type = " << sampleType << " -> xSec = " << Get_xSec(sampleType) << endl;
  }

  Double_t nEvent = nTotEvent * (xSec / xSec_sum);

  return (int)nEvent;
}


void BDTInputTreeProducer()
{
  // -- signal
  TString type_signal = "DYJetsToLL_M10to50_Madgraph";
  Int_t nEvent_signal = 100000;

  TreeProducer* signalTree = new TreeProducer(type_signal, nEvent_signal);
  signalTree->Produce();


  // -- background
  // vector<TString> vec_ptRange = {
  //   "Pt15to20", "Pt20to30", "Pt30to50", "Pt50to80", 
  //   "Pt80to120", "Pt120to170", "Pt170to300", "Pt300to470", 
  //   "Pt470to600", "Pt600to800", "Pt800to1000", "Pt1000toInf"
  // };

  // -- remove Pt15to20: only ~5000 events are included in the baseline selection
  vector<TString> vec_ptRange = {
    "Pt20to30", "Pt30to50", "Pt50to80", 
    "Pt80to120", "Pt120to170", "Pt170to300", "Pt300to470", 
    "Pt470to600", "Pt600to800", "Pt800to1000", "Pt1000toInf"
  };

  for(const auto& ptRange : vec_ptRange )
  {
    TString type_bkg = "QCDMuEnriched_"+ptRange;
    Int_t nEvent_bkg = CalcNEvent_QCD( type_bkg, nEvent_signal );
    cout << "background type = " << type_bkg << " -> # events: " << nEvent_bkg << endl;

    TreeProducer* bkgTree = new TreeProducer(type_bkg, nEvent_bkg);
    bkgTree->Produce();
  }
}