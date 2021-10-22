#include <NLOReweight/NLOWeightTool.h>

class HistProducer: public DYTool::ClassTemplate
{
public:
  Bool_t debug_ = kFALSE;

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

    // vector<Double_t> vec_lowPtBinEdge  = {0, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 30, 35, 40, 50, 60, 120, 200, 500};
    // vector<Double_t> vec_highPtBinEdge = {0, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 10.5, 11, 11.5, 12, 12.5, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 30, 40, 50, 60, 120, 200, 500};
    // vector<Double_t> vec_etaBinEdge    = {-2.4, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.4};

    // -- check lowPtL1 case only (more significant)
    // -- 1st bin (0 < bin < 1): L1 matching was failed
    // -- 2nd bin (1 < bin < 2): HLT filter object didn't exist in the event
    // -- 3rd bin (2 < bin < 3): HLT filter object exists, but failed to be matched
    TH1D* h_failedCase_nMu1_pt5to10     = new TH1D("h_failedCase_nMu1_pt5to10", "", 3, 0, 3);
    TH1D* h_dR_failedCase3_nMu1_pt5to10 = new TH1D("h_dR_failedCase3_nMu1_pt5to10", "", 100, 0, 10);

    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      Double_t totWeight = sampleInfo_.normFactor * genWeight;
      // Double_t totWeight = genWeight;
      if( sampleInfo_.isMC ) totWeight *= PUTool->Weight( ntuple->truePU );

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        // -- DY low mass sample: apply NLO/LO k-factor
        if( sampleInfo_.type.Contains("DYMuMu_M10to50"))
          totWeight *= nloWeightTool->GetWeight(ntuple);

        vector<DYTool::OffMuon> vec_matchedOffMuon = DYTool::GenMatchedOfflineMuon(ntuple);

        if( debug_ ) cout << TString::Format("[%dth event]", i) << endl;

        // -- calc. efficiency using gen-matched offline muons
        Int_t nOffMuon = (Int_t)vec_matchedOffMuon.size();
        for(auto offMuon : vec_matchedOffMuon )
        {
          if( offMuon.isTRK ) // -- only if it is tracker muon
          {
            if( nOffMuon == 1 && 5 < offMuon.pt && offMuon.pt < 10 )
            {
              if( !IsMatched_LowPtL1Leg_L3MuonNoVtx_Pt3(offMuon, ntuple) ) // -- fail to be matched
              {
                if( !IsMatched_LowPtL1(offMuon, ntuple) )
                  h_failedCase_nMu1_pt5to10->Fill( 0.5, totWeight ); // -- 1st case
                else
                {
                  vector<DYTool::L3MuonNoVtx> vec_filterObj;
                  Bool_t flag_pass = DYTool::CustomSingleMuFilter_MimicDoubleMu3Leg(ntuple, vec_filterObj);

                  if( vec_filterObj.size() == 0 )
                    h_failedCase_nMu1_pt5to10->Fill( 1.5, totWeight ); // -- 2nd case (no HLT filter object exists)
                  else
                  {
                    h_failedCase_nMu1_pt5to10->Fill( 2.5, totWeight ); // -- 3rd case (object exists but fail to be matched)
                    for(auto& filterObj : vec_filterObj )
                    {
                      Double_t dR = offMuon.vecP.DeltaR( filterObj.vecP );
                      h_dR_failedCase3_nMu1_pt5to10->Fill( dR, totWeight );
                    }
                  }
                } // -- else of if( L1 failed )
              } // -- end of if( matching is failed )
            } // -- end of if( nMu==1, 5 < pt < 10 )
          } // -- end of isTrackerMuon
        } // -- end of loop over offline muons
        if( debug_ ) cout << "=============================\n" << endl;
      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_FailedCase");
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    h_failedCase_nMu1_pt5to10->Write();
    h_dR_failedCase3_nMu1_pt5to10->Write();

    f_output->Close();

    PrintRunTime();
  }

private:
  Bool_t IsFired_L1(DYTool::DYTree* ntuple)
  {
    Bool_t doPass_L1 = kFALSE;
    // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPass_L1 = kTRUE; // -- no L1 w/ mass cut
    if( ntuple->vec_L1Bit->at(4) ) doPass_L1 = kTRUE; // -- no L1 w/ mass cut

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

  Bool_t IsMatched_HighPtL1(DYTool::OffMuon mu, DYTool::DYTree *ntuple)
  {
    Double_t minDR = 0.3;
    Double_t minQuality = 8.0;
    Double_t minL1Pt = 15.0;
    Double_t maxL1Pt = 9999.0;
    Bool_t isHighPtL1Matched = DYTool::dRMatching_L1Muon(mu.vecP_Propagated(), ntuple, minDR, minQuality, minL1Pt, maxL1Pt );

    return isHighPtL1Matched;
  }

  Bool_t IsMatched_LowPtL1(DYTool::OffMuon mu, DYTool::DYTree *ntuple)
  {
    Double_t minDR = 0.3;
    Double_t minQuality = 8.0;
    Double_t minL1Pt = 7.0;
    Double_t maxL1Pt = 9999.0;
    Bool_t isLowPtL1Matched = DYTool::dRMatching_L1Muon(mu.vecP_Propagated(), ntuple, minDR, minQuality, minL1Pt, maxL1Pt );

    return isLowPtL1Matched;
  }

  Bool_t IsMatched_HLTObj(DYTool::OffMuon mu, DYTool::DYTree *ntuple)
  {
    Bool_t isHLTMatched = DYTool::dRMatching_HLTObj(mu.vecP, ntuple, "hltDoubleMu3L3FilteredNoVtx", 0.1);

    return isHLTMatched;
  }

  Bool_t IsMatched_L3MuonNoVtx_CustomSingleMu3Filter(DYTool::OffMuon mu, DYTool::DYTree *ntuple)
  {
    vector<DYTool::L3MuonNoVtx> vec_filterObj;
    Bool_t flag_pass = DYTool::CustomSingleMuFilter_MimicDoubleMu3Leg(ntuple, vec_filterObj);

    vector<TLorentzVector> vec_vecP_filterObj;
    for( auto& filterObj : vec_filterObj )
      vec_vecP_filterObj.push_back( filterObj.vecP );

    Double_t minDR = 0.1;
    return DYTool::dRMatching(mu.vecP, vec_vecP_filterObj, minDR);
  }

  Bool_t IsMatched_LowPtL1Leg_L3MuonNoVtx_Pt3(DYTool::OffMuon mu, DYTool::DYTree *ntuple)
  {
    return IsMatched_LowPtL1(mu, ntuple) && IsMatched_L3MuonNoVtx_CustomSingleMu3Filter(mu, ntuple);
  }

  Bool_t IsMatched_HighPtL1Leg_L3MuonNoVtx_Pt3(DYTool::OffMuon mu, DYTool::DYTree *ntuple)
  {
    return IsMatched_HighPtL1(mu, ntuple) && IsMatched_L3MuonNoVtx_CustomSingleMu3Filter(mu, ntuple);
  }

};