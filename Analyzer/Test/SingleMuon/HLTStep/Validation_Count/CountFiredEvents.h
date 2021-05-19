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

    // DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");
    // NLOWeightTool* nloWeightTool = new NLOWeightTool();

    TH1D* h_nEvent_filter_1mu = new TH1D("h_nEvent_filter_1mu", "", 1, 0, 1);
    TH1D* h_nEvent_filter_2mu = new TH1D("h_nEvent_filter_2mu", "", 1, 0, 1);
    TH1D* h_nEvent_trigFired  = new TH1D("h_nEvent_trigFired",  "", 1, 0, 1);

    // -- ignore weights; count the pure statistics
    Int_t nEvent_filter_1mu = 0;
    Int_t nEvent_filter_2mu = 0;
    Int_t nEvent_trigFired  = 0;

    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      // Double_t genWeight;
      // ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      // Double_t totWeight = sampleInfo_.normFactor * genWeight;
      // Double_t totWeight = genWeight;
      // if( sampleInfo_.isMC ) totWeight = genWeight * PUTool->Weight( ntuple->truePU );

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        // -- DY low mass sample: apply NLO/LO k-factor
        // if( sampleInfo_.type.Contains("DYMuMu_M10to50"))
        //   totWeight *= nloWeightTool->GetWeight(ntuple);

        // -- baseline selection: pass L1 and IsoMu24 (to guarantee that the event is stored in data)
        if( IsFired_L1_IsoMu24(ntuple) )
        {
          vector<DYTool::HLTObj> vec_HLTObj = DYTool::GetAllHLTObj(ntuple, "hltDoubleMu3L3FilteredNoVtx");
          Int_t nHLTObj = (Int_t)vec_HLTObj.size();

          if( nHLTObj == 1 ) nEvent_filter_1mu++;
          if( nHLTObj >= 2 ) nEvent_filter_2mu++;

          if( IsFired_DoubleMu3(ntuple) ) nEvent_trigFired++;
        }
      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    h_nEvent_filter_1mu->SetBinContent(1, nEvent_filter_1mu);
    h_nEvent_filter_1mu->SetBinError(1, 0);

    h_nEvent_filter_2mu->SetBinContent(1, nEvent_filter_2mu);
    h_nEvent_filter_2mu->SetBinError(1, 0);

    h_nEvent_trigFired->SetBinContent(1, nEvent_trigFired);
    h_nEvent_trigFired->SetBinError(1, 0);

    TString outputName = GetOutputFileName("CountFiredEvents");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    h_nEvent_filter_1mu->Write();
    h_nEvent_filter_2mu->Write();
    h_nEvent_trigFired->Write();

    f_output->Close();

    PrintRunTime();
  }

private:
  Bool_t IsFired_L1_IsoMu24(DYTool::DYTree* ntuple)
  {
    Bool_t doPass_L1 = kFALSE;
    if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPass_L1 = kTRUE; // -- no L1 w/ mass cut

    Bool_t doPass_HLT = kFALSE;
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("HLT_IsoMu24_v") ) doPass_HLT = kTRUE;

      if( doPass_HLT ) break;
    }

    return doPass_L1 && doPass_HLT;
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
};
