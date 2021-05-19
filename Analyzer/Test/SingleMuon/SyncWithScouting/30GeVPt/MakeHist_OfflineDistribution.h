#include <Test/SingleMuon/OfflineHistContainer.h>
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

    HistContainer* hists            = new HistContainer();
    HistContainer* hists_ZPeak      = new HistContainer();
    HistContainer* hists_M10to60    = new HistContainer();
    HistContainer* hists_M10to60_OS = new HistContainer();
    HistContainer* hists_M10to60_SS = new HistContainer();

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

        Bool_t isDYEvent = kFALSE;
        DYTool::OffMuPair DYMuPair = OfflineEventSelection(ntuple, isDYEvent);
        if( isDYEvent )
        {
          Double_t mass = DYMuPair.mass;
          if( mass > 10 )
            hists->Fill(ntuple, DYMuPair, totWeight);

          if( 60 < mass && mass < 120 )
            hists_ZPeak->Fill(ntuple, DYMuPair, totWeight);

          if( 10 < mass && mass < 60 )
          {
            hists_M10to60->Fill(ntuple, DYMuPair, totWeight);

            if( DYMuPair.isOS ) hists_M10to60_OS->Fill(ntuple, DYMuPair, totWeight);
            else                hists_M10to60_SS->Fill(ntuple, DYMuPair, totWeight);
          }
        } // -- isDYEvent
      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_Dimuon");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    hists->Write(f_output);

    f_output->mkdir("ZPeak");
    f_output->cd("ZPeak");
    hists_ZPeak->Write(f_output);

    f_output->mkdir("M10to60");
    f_output->cd("M10to60");
    hists_M10to60->Write(f_output);

    f_output->mkdir("M10to60_OS");
    f_output->cd("M10to60_OS");
    hists_M10to60_OS->Write(f_output);

    f_output->mkdir("M10to60_SS");
    f_output->cd("M10to60_SS");
    hists_M10to60_SS->Write(f_output);

    f_output->Close();

    PrintRunTime();
  }

private:
  DYTool::OffMuPair OfflineEventSelection(DYTool::DYTree* ntuple, Bool_t& doPass)
  {
    doPass = kFALSE;

    DYTool::OffMuPair muPair_dummy;

    // -- pass unprescaled L1 (only for 2018)
    // -- 4 = L1_DoubleMu_15_7
    // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
    // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
    Bool_t doPassL1 = kFALSE;
    // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;
    if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut

    if( !doPassL1 ) return muPair_dummy;

    // -- pass HLT
    Bool_t doPassTrig1 = kFALSE;
    Bool_t doPassTrig2 = kFALSE;
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("HLT_IsoMu24_v") )                      doPassTrig1 = kTRUE;
      if( tstr_firedTrig.Contains("DST_DoubleMu3_noVtx_CaloScouting_v") ) doPassTrig2 = kTRUE;

      if( doPassTrig1 && doPassTrig2 ) break;
    }

    if( !(doPassTrig1 && doPassTrig2) ) return muPair_dummy;

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
    if( muPair.first_.pt > 30  && fabs(muPair.first_.eta) < 2.4 &&
        muPair.second_.pt > 30 && fabs(muPair.second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( muPair.first_.nPixelHit_inner > 0  && muPair.first_.nTrackerLayer_inner > 5  && muPair.first_.normChi2_global < 10  && muPair.first_.relTrkIso < 0.15 &&
        muPair.second_.nPixelHit_inner > 0 && muPair.second_.nTrackerLayer_inner > 5 && muPair.second_.normChi2_global < 10 && muPair.second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    if( isWithinAcc && isGoodMuon && muPair.mass > 10.0 ) flag = kTRUE;

    return flag;
  }


};
