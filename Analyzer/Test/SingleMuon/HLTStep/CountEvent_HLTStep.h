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

    // -- filter names
    vector<TString> vec_filter = {
      "hltL1sDoubleMuIorTripleMuIorQuadMu",
      "hltDimuon3L1Filtered0",
      "hltDimuon3L2PreFiltered0", # -- not stored yet
      "hltDoubleMu3L3FilteredNoVtx"
    };    
    Int_t nFilter = (Int_t)vec_filter.size();

    Int_t nBin = nFilter+3; // -- three more bins: 1 for IsoMu24+Dimuon (starting point), 1 for L1 bit and 1 for final HLT bit
    TH1D* h_nEvent_HLTStep = new TH1D("h_nEvent_HLTStep", "", nBin, 0, nBin);

    // -- set label
    h_nEvent_HLTStep->GetXaxis()->SetBinLabel(1, "IsoMu24+Dimuon sel.");
    h_nEvent_HLTStep->GetXaxis()->SetBinLabel(2, "L1 bit");
    for(Int_t i_filter=0; i_filter<nFilter; i_filter++)
    {
      Int_t i_bin = i_filter+3;
      h_nEvent_HLTStep->GetXaxis()->SetBinLabel(i_bin, vec_filter[i_filter]);
    }
    h_nEvent_HLTStep->GetXaxis()->SetBinLabel(nBin, "Trigger bit");

    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      // Double_t totWeight = sampleInfo_.normFactor * genWeight;
      Double_t totWeight = genWeight;
      if( sampleInfo_.isMC ) totWeight *= PUTool->Weight( ntuple->truePU );

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        // -- DY low mass sample: apply NLO/LO k-factor
        if( sampleInfo_.type.Contains("DYMuMu_M10to50"))
          totWeight *= nloWeightTool->GetWeight(ntuple);

        // -- test: does it pass IsoMu24+Dimuon selection+Z mass window?
        Bool_t isDYEvent_IsoMu24Z = kFALSE;
        DYTool::OffMuPair DYMuPair = OfflineEventSelection_IsoMu24_ZMassWindow(ntuple, isDYEvent_IsoMu24Z);

        if( isDYEvent_IsoMu24Z )
        {
          h_nEvent_HLTStep->Fill( 0.5, totWeight ); // -- 0.5 = 1st bin
          if( IsFired_L1(ntuple) )
          {
            h_nEvent_HLTStep->Fill( 1.5, totWeight ); // -- 1.5 = 2nd bin

            for(Int_t i_filter=0; i_filter<nFilter; i_filter++)
            {
              TString filterName = vec_filter[i_filter];
              if( DoPass_GivenFilter(ntuple, filterName) )
                h_nEvent_HLTStep->Fill(i_filter+2.5, totWeight); // -- example: i_filter=0 -> i_filter+2.5 = 2.5: 3rd bin
            }

            if( IsFired_DoubleMu3(ntuple) )
              h_nEvent_HLTStep->Fill(nBin-0.5, totWeight);
          }

        } // -- end of if( isDYEvent_IsoMu24Z )
      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("CountEvent_HLTStep");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    h_nEvent_HLTStep->Write();

    f_output->Close();

    PrintRunTime();
  }

private:
  Bool_t DoPass_GivenFilter(DYTool::DYTree* ntuple, TString theFilterName)
  {
    Bool_t doPass = kFALSE;
    vector<DYTool::HLTObj> vec_HLTObj = DYTool::GetAllHLTObj(ntuple, theFilterName);
    Int_t nHLTObj = (Int_t)vec_HLTObj.size();

    if( nHLTObj >= 2 ) doPass = kTRUE;

    return doPass;
  }

  Bool_t IsFired_L1(DYTool::DYTree* ntuple)
  {
    Bool_t doPass_L1 = kFALSE;
    if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPass_L1 = kTRUE; // -- no L1 w/ mass cut

    return doPass_L1;
  }

  Bool_t IsFired_DoubleMu3(DYTool::DYTree* ntuple)
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

    // if( !doPassL1 ) return muPair_dummy;

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
