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
#include <NLOReweight/NLOWeightTool.h>

class HistProducer: public DYTool::ClassTemplate
{
public:
  HistProducer(): ClassTemplate()
  {

  }

  void Run()
  {
    Bool_t debug = kFALSE;

    CheckSampleInfo();
    StartTimer();

    // -- setup for counting the number of events
    TH1::SetDefaultSumw2(kTRUE);
    TH1::AddDirectory(kFALSE);

    // -- v1; original selection (used in the control plot)
    // -- v2: new selection (used in N-1 efficiency)
    TH1D* h_diMuM_v1 = new TH1D("h_diMuM_v1", "", 20, 81, 101);
    TH1D* h_diMuM_v2 = new TH1D("h_diMuM_v2", "", 20, 81, 101);
    TH1D* h_diMuM_v3 = new TH1D("h_diMuM_v3", "", 20, 81, 101);

    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");
    NLOWeightTool* nloWeightTool = new NLOWeightTool();

    for(Int_t i=0; i<nEvent; i++)
    {
      // -- flag
      Bool_t pass_v1 = kFALSE;
      Bool_t pass_v2 = kFALSE;
      Bool_t pass_v3 = kFALSE;

      if( !debug ) DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      // Double_t totWeight = sampleInfo_.normFactor * genWeight;
      Double_t totWeight = genWeight;
      if( sampleInfo_.isMC ) totWeight = genWeight * PUTool->Weight( ntuple->truePU );

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        // -- DY low mass sample: apply NLO/LO k-factor
        if( sampleInfo_.type.Contains("DYMuMu_M10to50"))
          totWeight *= nloWeightTool->GetWeight(ntuple);

        // -- v1: original selection
        Bool_t isDYEvent = kFALSE;
        DYTool::MuPair DYMuPair = DYTool::EventSelection_BDTInput_noOS(ntuple, isDYEvent);
        if( isDYEvent )
        {
          Double_t mass = DYMuPair.mass;
          if( 81 < mass && mass < 101 )
          {
            pass_v1 = kTRUE;
            h_diMuM_v1->Fill( mass, totWeight );
          }
        }

        // -- v2: new selection
        DYTool::MuPair DYPair_v2;
        if( EventSelection_ZMassRange_N(ntuple, DYPair_v2) )
        {
          pass_v2 = kTRUE;
          h_diMuM_v2->Fill( DYPair_v2.mass, totWeight );
        }

        // -- v3: new selection + best chi2
        DYTool::MuPair DYPair_v3;
        if( EventSelection_ZMassRange_N_BestChi2(ntuple, DYPair_v3) )
        {
          Double_t mass = DYPair_v3.mass;
          if( 81 < mass && mass < 101 )
          {
            pass_v3 = kTRUE;
            h_diMuM_v3->Fill( DYPair_v3.mass, totWeight );
          }
        }
      }

      if( debug )
      {
        if( pass_v1 || pass_v2 || pass_v3 )
        {
          printf("[%dth event]\n", i);
          printf("  [Pass] (v1, v2, v3) = (%d, %d, %d)\n", pass_v1, pass_v2, pass_v3);
          if( pass_v1 != pass_v3 )
            printf("    --> pass_v1 is not same with pass_v3!\n");
        }
      }
    } // -- end of event iteration

    TString outputName = TString::Format("ROOTFile_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    h_diMuM_v1->Write();
    h_diMuM_v2->Write();
    h_diMuM_v3->Write();

    f_output->Close();

    PrintRunTime();
  }
private:

  // -- same with EventSelection_BDTInput_noOS in DYTool.h
  // -- except for the requirement on the mass range (within 81 < m < 101 GeV) to ensure that it is true dimuon events
  Bool_t EventSelection_ZMassRange_N(DYTool::DYTree *ntuple, DYTool::MuPair &DYPair)
  {
    Bool_t doPass = kFALSE;

    if( !IsTriggerFired(ntuple) ) return 0;

    // -- dimuon selection
    vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

    vector< DYTool::MuPair > vec_goodMuPair;
    for( auto& muPair : vec_muPair )
      if( IsDYCandidate_BDTInput_noOS(muPair, ntuple) ) vec_goodMuPair.push_back( muPair );

    if( vec_goodMuPair.size() == 1 ) // -- select the event with exact 1 good pair
    {
      Double_t diMuM = vec_goodMuPair[0].mass;
      if( 81 < diMuM && diMuM < 101 )
      {
        doPass = kTRUE;
        DYPair = vec_goodMuPair[0];
      }
    }
    else
      doPass = kFALSE;

    return doPass;
  }

  // -- select the pair with the best chi2
  Bool_t EventSelection_ZMassRange_N_BestChi2(DYTool::DYTree *ntuple, DYTool::MuPair &DYPair)
  {
    Bool_t doPass = kFALSE;

    if( !IsTriggerFired(ntuple) ) return 0;

    // -- dimuon selection
    vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

    vector< DYTool::MuPair > vec_goodMuPair;
    for( auto& muPair : vec_muPair )
      if( IsDYCandidate_BDTInput_noOS(muPair, ntuple) ) vec_goodMuPair.push_back( muPair );

    Int_t nGoodPair = (Int_t)vec_goodMuPair.size();
    if( nGoodPair == 0 )
    {
      doPass = kFALSE;
    }
    else if( nGoodPair == 1) // -- exactly one pair: no sort is needed
    {
      doPass = kTRUE;
      DYPair = vec_goodMuPair[0];
    }
    else // -- multiple pair: take the pair with the smallest chi2
    {
      doPass = kTRUE;
      std::sort(vec_goodMuPair.begin(), vec_goodMuPair.end(), DYTool::CompareMuonPair_SmallerVtxChi2);
      DYPair = vec_goodMuPair[0]; // -- pair with smallest vertex chi2 value
    }

    return doPass;
  }


  Bool_t IsTriggerFired(DYTool::DYTree *ntuple)
  {
    // -- pass unprescaled L1 (only for 2018)
    // -- 4 = L1_DoubleMu_15_7
    // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
    // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
    Bool_t doPassL1 = kFALSE;
    // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;
    if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut

    if( !doPassL1 ) return 0;

    // -- pass HLT
    Bool_t doPassTrig = kFALSE;
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("DST_DoubleMu3_noVtx_CaloScouting_v") )
      {
        doPassTrig = kTRUE;
        break;
      }
    }

    return doPassL1 && doPassTrig;
  }

  Bool_t IsDYCandidate_BDTInput_noOS(DYTool::MuPair& muPair, DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( muPair.first_.pt > 5  && fabs(muPair.first_.eta) < 2.4 &&
        muPair.second_.pt > 5 && fabs(muPair.second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( muPair.first_.nPixelHit > 0  && muPair.first_.nTrackerLayer > 5  && muPair.first_.normChi2 < 10  && muPair.first_.relTrkIso < 0.15 &&
        muPair.second_.nPixelHit > 0 && muPair.second_.nTrackerLayer > 5 && muPair.second_.normChi2 < 10 && muPair.second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    Bool_t hasVertex = CheckVertex(muPair, ntuple);

    if( isWithinAcc && isGoodMuon && hasVertex && muPair.mass > 10.0 ) flag = kTRUE;

    return flag;
  }

  Bool_t CheckVertex(DYTool::MuPair& muPair, DYTool::DYTree *ntuple)
  {
    Bool_t hasVertex = kFALSE;

    Int_t nVtx = ntuple->nVtx;
    if( nVtx == 0 )
    {
      hasVertex = kFALSE;
      return kFALSE;
    }

    for(Int_t i_vtx=0; i_vtx<nVtx; i_vtx++)
    {
      Bool_t mu1_associated = kFALSE;
      for(auto vtxIndex : muPair.first_.vec_vtxIndex)
      {
        if(i_vtx == vtxIndex)
        {
          mu1_associated = kTRUE;
          break;
        }
      }

      Bool_t mu2_associated = kFALSE;
      for(auto vtxIndex : muPair.second_.vec_vtxIndex)
      {
        if(i_vtx == vtxIndex)
        {
          mu2_associated = kTRUE;
          break;
        }
      }

      if( mu1_associated && mu2_associated )
      {
        hasVertex = kTRUE;
        // -- set the dimuon pair member values
        muPair.hasVertex = kTRUE;
        muPair.vertex = DYTool::DimuonVertex(ntuple, i_vtx);
        muPair.normVtxChi2 = muPair.vertex.normChi2;

        break;
      }
    }

    return hasVertex;
  }

  Bool_t EventSelection_ZMassRange_N_minus_nPixelHit(DYTool::DYTree *ntuple, DYTool::MuPair &DYPair)
  {
    Bool_t doPass = kFALSE;

    if( !IsTriggerFired(ntuple) ) return 0;

    // -- dimuon selection
    vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

    vector< DYTool::MuPair > vec_goodMuPair;
    for( auto& muPair : vec_muPair )
      if( IsDYCandidate_BDTInput_noOS_minus_nPixelHit(muPair, ntuple) ) vec_goodMuPair.push_back( muPair );

    if( vec_goodMuPair.size() == 1 ) // -- select the event with exact 1 good pair
    {
      Double_t diMuM = vec_goodMuPair[0].mass;
      if( 81 < diMuM && diMuM < 101 )
      {
        doPass = kTRUE;
        DYPair = vec_goodMuPair[0];
      }
    }
    else
      doPass = kFALSE;

    return doPass;
  }

  Bool_t EventSelection_ZMassRange_N_minus_nTrackerLayer(DYTool::DYTree *ntuple, DYTool::MuPair &DYPair)
  {
    Bool_t doPass = kFALSE;

    if( !IsTriggerFired(ntuple) ) return 0;

    // -- dimuon selection
    vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

    vector< DYTool::MuPair > vec_goodMuPair;
    for( auto& muPair : vec_muPair )
      if( IsDYCandidate_BDTInput_noOS_minus_nTrackerLayer(muPair, ntuple) ) vec_goodMuPair.push_back( muPair );

    if( vec_goodMuPair.size() == 1 ) // -- select the event with exact 1 good pair
    {
      Double_t diMuM = vec_goodMuPair[0].mass;
      if( 81 < diMuM && diMuM < 101 )
      {
        doPass = kTRUE;
        DYPair = vec_goodMuPair[0];
      }
    }
    else
      doPass = kFALSE;

    return doPass;
  }

  Bool_t EventSelection_ZMassRange_N_minus_normChi2(DYTool::DYTree *ntuple, DYTool::MuPair &DYPair)
  {
    Bool_t doPass = kFALSE;

    if( !IsTriggerFired(ntuple) ) return 0;

    // -- dimuon selection
    vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

    vector< DYTool::MuPair > vec_goodMuPair;
    for( auto& muPair : vec_muPair )
      if( IsDYCandidate_BDTInput_noOS_minus_normChi2(muPair, ntuple) ) vec_goodMuPair.push_back( muPair );

    if( vec_goodMuPair.size() == 1 ) // -- select the event with exact 1 good pair
    {
      Double_t diMuM = vec_goodMuPair[0].mass;
      if( 81 < diMuM && diMuM < 101 )
      {
        doPass = kTRUE;
        DYPair = vec_goodMuPair[0];
      }
    }
    else
      doPass = kFALSE;

    return doPass;
  }

  Bool_t EventSelection_ZMassRange_N_minus_relTrkIso(DYTool::DYTree *ntuple, DYTool::MuPair &DYPair)
  {
    Bool_t doPass = kFALSE;

    if( !IsTriggerFired(ntuple) ) return 0;

    // -- dimuon selection
    vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

    vector< DYTool::MuPair > vec_goodMuPair;
    for( auto& muPair : vec_muPair )
      if( IsDYCandidate_BDTInput_noOS_minus_relTrkIso(muPair, ntuple) ) vec_goodMuPair.push_back( muPair );

    if( vec_goodMuPair.size() == 1 ) // -- select the event with exact 1 good pair
    {
      Double_t diMuM = vec_goodMuPair[0].mass;
      if( 81 < diMuM && diMuM < 101 )
      {
        doPass = kTRUE;
        DYPair = vec_goodMuPair[0];
      }
    }
    else
      doPass = kFALSE;

    return doPass;
  }

  Bool_t EventSelection_ZMassRange_N_minus_CommonVertex(DYTool::DYTree *ntuple, DYTool::MuPair &DYPair)
  {
    Bool_t doPass = kFALSE;

    if( !IsTriggerFired(ntuple) ) return 0;

    // -- dimuon selection
    vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

    vector< DYTool::MuPair > vec_goodMuPair;
    for( auto& muPair : vec_muPair )
      if( IsDYCandidate_BDTInput_noOS_minus_CommonVertex(muPair, ntuple) ) vec_goodMuPair.push_back( muPair );

    if( vec_goodMuPair.size() == 1 ) // -- select the event with exact 1 good pair
    {
      Double_t diMuM = vec_goodMuPair[0].mass;
      if( 81 < diMuM && diMuM < 101 )
      {
        doPass = kTRUE;
        DYPair = vec_goodMuPair[0];
      }
    }
    else
      doPass = kFALSE;

    return doPass;
  }

  Bool_t IsDYCandidate_BDTInput_noOS_minus_nPixelHit(DYTool::MuPair& muPair, DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( muPair.first_.pt > 5  && fabs(muPair.first_.eta) < 2.4 &&
        muPair.second_.pt > 5 && fabs(muPair.second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( muPair.first_.nTrackerLayer > 5  && muPair.first_.normChi2 < 10  && muPair.first_.relTrkIso < 0.15 &&
        muPair.second_.nTrackerLayer > 5 && muPair.second_.normChi2 < 10 && muPair.second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    Bool_t hasVertex = CheckVertex(muPair, ntuple);

    if( isWithinAcc && isGoodMuon && hasVertex && muPair.mass > 10.0 ) flag = kTRUE;

    return flag;
  }

  Bool_t IsDYCandidate_BDTInput_noOS_minus_nTrackerLayer(DYTool::MuPair& muPair, DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( muPair.first_.pt > 5  && fabs(muPair.first_.eta) < 2.4 &&
        muPair.second_.pt > 5 && fabs(muPair.second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( muPair.first_.nPixelHit > 0  && muPair.first_.normChi2 < 10  && muPair.first_.relTrkIso < 0.15 &&
        muPair.second_.nPixelHit > 0 && muPair.second_.normChi2 < 10 && muPair.second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    Bool_t hasVertex = CheckVertex(muPair, ntuple);

    if( isWithinAcc && isGoodMuon && hasVertex && muPair.mass > 10.0 ) flag = kTRUE;

    return flag;
  }

  Bool_t IsDYCandidate_BDTInput_noOS_minus_normChi2(DYTool::MuPair& muPair, DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( muPair.first_.pt > 5  && fabs(muPair.first_.eta) < 2.4 &&
        muPair.second_.pt > 5 && fabs(muPair.second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( muPair.first_.nPixelHit > 0  && muPair.first_.nTrackerLayer > 5  && muPair.first_.relTrkIso < 0.15 &&
        muPair.second_.nPixelHit > 0 && muPair.second_.nTrackerLayer > 5 && muPair.second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    Bool_t hasVertex = CheckVertex(muPair, ntuple);

    if( isWithinAcc && isGoodMuon && hasVertex && muPair.mass > 10.0 ) flag = kTRUE;

    return flag;
  }

  Bool_t IsDYCandidate_BDTInput_noOS_minus_relTrkIso(DYTool::MuPair& muPair, DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( muPair.first_.pt > 5  && fabs(muPair.first_.eta) < 2.4 &&
        muPair.second_.pt > 5 && fabs(muPair.second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( muPair.first_.nPixelHit > 0  && muPair.first_.nTrackerLayer > 5  && muPair.first_.normChi2 < 10  &&
        muPair.second_.nPixelHit > 0 && muPair.second_.nTrackerLayer > 5 && muPair.second_.normChi2 < 10 )
      isGoodMuon = kTRUE;

    Bool_t hasVertex = CheckVertex(muPair, ntuple);

    if( isWithinAcc && isGoodMuon && hasVertex && muPair.mass > 10.0 ) flag = kTRUE;

    return flag;
  }

  Bool_t IsDYCandidate_BDTInput_noOS_minus_CommonVertex(DYTool::MuPair& muPair, DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( muPair.first_.pt > 5  && fabs(muPair.first_.eta) < 2.4 &&
        muPair.second_.pt > 5 && fabs(muPair.second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( muPair.first_.nPixelHit > 0  && muPair.first_.nTrackerLayer > 5  && muPair.first_.normChi2 < 10  && muPair.first_.relTrkIso < 0.15 &&
        muPair.second_.nPixelHit > 0 && muPair.second_.nTrackerLayer > 5 && muPair.second_.normChi2 < 10 && muPair.second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    if( isWithinAcc && isGoodMuon && muPair.mass > 10.0 ) flag = kTRUE;

    return flag;
  }

};

void Test()
{
  HistProducer* producer = new HistProducer();

  producer->sampleInfo_.type = "DYMuMu_M10to50_NLOXSec_TEST";
  producer->sampleInfo_.ntuplePathFile = "/Users/KyeongPil_Lee/Physics/DYScouting/Analyzer/Include/Example/ntuplePath_example.txt";
  producer->sampleInfo_.xSec = 10000;
  producer->sampleInfo_.sumWeight = 10000;
  Double_t lumi = 999;
  producer->sampleInfo_.normFactor = (lumi * 10000.0 ) / 10000.0;

  producer->Run();
}