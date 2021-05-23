#include <Test/SingleMuon/Mu17Mu8/OfflineZPeakHistContainer.h>
#include <NLOReweight/NLOWeightTool.h>

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
    ntuple->TurnOnBranches_OffMuon();

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");
    NLOWeightTool* nloWeightTool = new NLOWeightTool();

    OfflineZPeakHistContainer* hist_IsoMu24Z = new OfflineZPeakHistContainer("IsoMu24Z");
    OfflineZPeakHistContainer* hist_L1       = new OfflineZPeakHistContainer("L1");
    OfflineZPeakHistContainer* hist_DST      = new OfflineZPeakHistContainer("DST"); // -- scouting trigger
    OfflineZPeakHistContainer* hist_HLT      = new OfflineZPeakHistContainer("HLT"); // -- standard dimuon trigger

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
        // -- DY low mass sample: apply NLO/LO k-factor
        if( sampleInfo_.type.Contains("DYMuMu_M10to50"))
          totWeight *= nloWeightTool->GetWeight(ntuple);

        // -- test: does it pass L1+IsoMu24+Dimuon selection+Z mass window?
        Bool_t isDYEvent_IsoMu24Z = kFALSE;
        DYTool::OffMuPair DYMuPair = OfflineEventSelection_IsoMu24_ZMassWindow(ntuple, isDYEvent_IsoMu24Z);

        if( isDYEvent_IsoMu24Z )
        {
          hist_IsoMu24Z->Fill( DYMuPair, totWeight );
          if( IsFired_L1_DoubleMu_15_7(ntuple) )
          {
            hist_L1->Fill( DYMuPair, totWeight );

            if( IsFired_DST_DoubleMu3(ntuple) )
              hist_DST->Fill( DYMuPair, totWeight );

            if( IsFired_HLT_DoubleMu_17_8(ntuple) )
              hist_HLT->Fill( DYMuPair, totWeight );
          }
        } // -- end of if( isDYEvent_IsoMu24Z )
      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_OfflineZPeak");
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    hist_IsoMu24Z->Write(f_output);
    hist_L1->Write(f_output);
    hist_DST->Write(f_output);
    hist_HLT->Write(f_output);

    f_output->Close();

    PrintRunTime();
  }

private:
  Bool_t IsFired_L1_DoubleMu_15_7(DYTool::DYTree* ntuple)
  {
    Bool_t doPass_L1 = kFALSE;
    // -- ntuple->vec_L1Bit->at(4): L1_DoubleMu_15_7
    if( ntuple->vec_L1Bit->at(4) ) doPass_L1 = kTRUE;

    return doPass_L1;
  }

  Bool_t IsFired_DST_DoubleMu3(DYTool::DYTree* ntuple)
  {
    Bool_t doPass_HLT = kFALSE;
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("DST_DoubleMu3_noVtx_CaloScouting_v") ) doPass_HLT = kTRUE;

      if( doPass_HLT ) break;
    }

    return doPass_HLT;
  }

  Bool_t IsFired_HLT_DoubleMu_17_8(DYTool::DYTree* ntuple)
  {
    Bool_t doPass_HLT = kFALSE;
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v*") ) doPass_HLT = kTRUE;

      if( doPass_HLT ) break;
    }

    return doPass_HLT;
  }

  DYTool::OffMuPair OfflineEventSelection_IsoMu24_ZMassWindow(DYTool::DYTree* ntuple, Bool_t& doPass)
  {
    DYTool::OffMuPair muPair_dummy;

    DYTool::OffMuPair DYMuPair = OfflineEventSelection_IsoMu24(ntuple, doPass);
    Double_t diMuM = DYMuPair.mass;

    if( 81 < diMuM && diMuM < 101 )
    {
      doPass = kTRUE;
      return DYMuPair;
    }
    else
    {
      doPass = kFALSE;
      return muPair_dummy;
    }

    return muPair_dummy;
  }

  DYTool::OffMuPair OfflineEventSelection_IsoMu24(DYTool::DYTree* ntuple, Bool_t& doPass)
  {
    doPass = kFALSE;

    DYTool::OffMuPair muPair_dummy;

    // -- pass unprescaled L1 (only for 2018)
    // -- 4 = L1_DoubleMu_15_7
    // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
    // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
    // Bool_t doPassL1 = kFALSE;
    // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;
    // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut
    Bool_t doPassL1 = kTRUE;

    if( !doPassL1 ) return muPair_dummy;

    // -- pass HLT
    // Bool_t doPassTrig1 = kFALSE;
    // Bool_t doPassTrig2 = kFALSE;
    // for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    // {
    //   TString tstr_firedTrig = firedTrigger;
    //   if( tstr_firedTrig.Contains("HLT_IsoMu24_v") )                      doPassTrig1 = kTRUE;
    //   if( tstr_firedTrig.Contains("DST_DoubleMu3_noVtx_CaloScouting_v") ) doPassTrig2 = kTRUE;

    //   if( doPassTrig1 && doPassTrig2 ) break;
    // }

    // if( !(doPassTrig1 && doPassTrig2) ) return muPair_dummy;

    Bool_t doPassTrig = kFALSE;
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("HLT_IsoMu24_v") ) doPassTrig = kTRUE;

      if( doPassTrig ) break;
    }

    if( !doPassTrig ) return muPair_dummy;

    // -- dimuon selection
    vector< DYTool::OffMuPair > vec_muPair = DYTool::GetAllOffMuPairs(ntuple);

    vector< DYTool::OffMuPair > vec_goodMuPair;
    for( auto& muPair : vec_muPair )
      if( IsDYCandidate_BDTInput_noOS_Offline(muPair) ) vec_goodMuPair.push_back( muPair );

    Int_t nGoodPair = (Int_t)vec_goodMuPair.size();
    if( nGoodPair == 0 )
    {
      doPass = kFALSE;
      return muPair_dummy;
    }
    else if( nGoodPair == 1) // -- exactly one pair: no sort is needed
    {
      doPass = kTRUE;
      return vec_goodMuPair[0];
    }
    else // -- multiple pair: take the pair with the smallest chi2
    {
      doPass = kTRUE;
      std::sort(vec_goodMuPair.begin(), vec_goodMuPair.end(), DYTool::CompareOffMuonPair_SmallerVtxChi2);
      return vec_goodMuPair[0]; // -- pair with smallest vertex chi2 value
    }
  }



  Bool_t IsDYCandidate_BDTInput_noOS_Offline(DYTool::OffMuPair& muPair)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( muPair.first_.pt > 26  && fabs(muPair.first_.eta) < 2.4 &&
        muPair.second_.pt > 10 && fabs(muPair.second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( muPair.first_.nPixelHit_inner > 0  && muPair.first_.nTrackerLayer_inner > 5  && muPair.first_.normChi2_global < 10  && muPair.first_.relTrkIso < 0.15 &&
        muPair.second_.nPixelHit_inner > 0 && muPair.second_.nTrackerLayer_inner > 5 && muPair.second_.normChi2_global < 10 && muPair.second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    if( isWithinAcc && isGoodMuon && muPair.mass > 10.0 ) flag = kTRUE;

    return flag;
  }


};
