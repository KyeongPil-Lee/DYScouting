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

    Double_t lumi = 61300;
    Double_t sumWeight = Get_SumWeight( sampleInfo_.type );
    Double_t xSec      = Get_xSec( sampleInfo_.type );
    Double_t normFactor = (lumi * xSec ) / sumWeight;
    if( verBose_ )
      printf("(lumi, xSec, sumWeight) = (%.0lf, %lf, %.0lf) -> norm. factor = %lf\n", lumi, xSec, sumWeight, normFactor);

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
          if( DYMuPair.mass > 60.0 ) continue; // -- because the DY sample is truncated at M=60 GeV

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

          weight_ = normFactor;

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
  Bool_t verBose_ = kTRUE;
  
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

  // -- norm. factor
  Double_t weight_;


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

    tree_->Branch("weight", &weight_, "weight/D");
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

    weight_ = 0;
  }

  Double_t Get_SumWeight(TString sampleName)
  {
    Double_t sumWeight = 0;
    if( sampleName == "DYMuMu_M10to50" )                 sumWeight = 28985097.0;
    else if( sampleName == "DYMuMu_M50toInf" )           sumWeight = 44318555.0;
    else if( sampleName == "DYTauTau_M10to50" )          sumWeight = 28843598.0;
    else if( sampleName == "DYTauTau_M50toInf" )         sumWeight = 44279551.0;
    else if( sampleName == "ttbar" )                     sumWeight = 63811310.0;
    else if( sampleName == "WJets" )                     sumWeight = 71061720.0;
    else if( sampleName == "QCDMuEnriched_Pt20toInf" )   sumWeight = 22165320.0;
    else if( sampleName == "QCDMuEnriched_Pt15to20" )    sumWeight = 4572168.0;
    else if( sampleName == "QCDMuEnriched_Pt20to30" )    sumWeight = 26549827.0;
    else if( sampleName == "QCDMuEnriched_Pt30to50" )    sumWeight = 30431342.0;
    else if( sampleName == "QCDMuEnriched_Pt50to80" )    sumWeight = 20360155.0;
    else if( sampleName == "QCDMuEnriched_Pt80to120" )   sumWeight = 25652280.0;
    else if( sampleName == "QCDMuEnriched_Pt120to170" )  sumWeight = 21315922.0;
    else if( sampleName == "QCDMuEnriched_Pt170to300" )  sumWeight = 36372547.0;
    else if( sampleName == "QCDMuEnriched_Pt300to470" )  sumWeight = 29488563.0;
    else if( sampleName == "QCDMuEnriched_Pt470to600" )  sumWeight = 20542857.0;
    else if( sampleName == "QCDMuEnriched_Pt600to800" )  sumWeight = 16891461.0;
    else if( sampleName == "QCDMuEnriched_Pt800to1000" ) sumWeight = 16749914.0;
    else if( sampleName == "QCDMuEnriched_Pt1000toInf" ) sumWeight = 11039499.0;
    else
    {
      cout << "no information for sampleName = " << sampleName << endl;
      sumWeight = 0;
    }

    return sumWeight;
  }

  Double_t Get_xSec(TString sampleName)
  {

    Double_t xSec = 0;
    if( sampleName == "DYMuMu_M10to50" )                 xSec = 6270.0;
    else if( sampleName == "DYMuMu_M50toInf" )           xSec = 2009.41;
    else if( sampleName == "DYTauTau_M10to50" )          xSec = 6270.0;
    else if( sampleName == "DYTauTau_M50toInf" )         xSec = 2009.41;
    else if( sampleName == "ttbar" )                     xSec = 88.29;
    else if( sampleName == "WJets" )                     xSec = 61526.7;
    else if( sampleName == "QCDMuEnriched_Pt20toInf" )   xSec = 239400.0;
    else if( sampleName == "QCDMuEnriched_Pt15to20" )    xSec = 2799000.0;
    else if( sampleName == "QCDMuEnriched_Pt20to30" )    xSec = 2526000.0;
    else if( sampleName == "QCDMuEnriched_Pt30to50" )    xSec = 1362000.0;
    else if( sampleName == "QCDMuEnriched_Pt50to80" )    xSec = 376600.0;
    else if( sampleName == "QCDMuEnriched_Pt80to120" )   xSec = 88930.0;
    else if( sampleName == "QCDMuEnriched_Pt120to170" )  xSec = 21230.0;
    else if( sampleName == "QCDMuEnriched_Pt170to300" )  xSec = 7055.0;
    else if( sampleName == "QCDMuEnriched_Pt300to470" )  xSec = 797.3; // -- TuneCUETP8M1
    else if( sampleName == "QCDMuEnriched_Pt470to600" )  xSec = 59.24;
    else if( sampleName == "QCDMuEnriched_Pt600to800" )  xSec = 25.25; // -- TuneCUETP8M1
    else if( sampleName == "QCDMuEnriched_Pt800to1000" ) xSec = 4.723; // -- TuneCUETP8M1
    else if( sampleName == "QCDMuEnriched_Pt1000toInf" ) xSec = 1.613; // -- TuneCUETP8M1
    else
    {
      cout << "no information for sampleName = " << sampleName << endl;
      xSec = 0;
    }

    return xSec;
  }

};