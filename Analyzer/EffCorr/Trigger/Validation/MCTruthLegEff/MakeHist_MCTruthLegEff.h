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

    vector<Double_t> vec_ptBinEdge     = {0, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 30, 35, 40, 50, 60, 120, 200, 500};
    vector<Double_t> vec_highPtBinEdge = {0, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 10.5, 11, 11.5, 12, 12.5, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 30, 40, 50, 60, 120, 200, 500};
    vector<Double_t> vec_etaBinEdge    = {-2.4, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.4};

    TH1D* h_mu_pt_lowPtBin_all     = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_lowPtBin_all",     vec_ptBinEdge);
    TH1D* h_mu_pt_lowPtBin_matched = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_lowPtBin_matched", vec_ptBinEdge);
    TH1D* h_mu_eta_lowPtBin_all     = DYTool::MakeTH1D_BinEdgeVector("h_mu_eta_lowPtBin_all",     vec_etaBinEdge);
    TH1D* h_mu_eta_lowPtBin_matched = DYTool::MakeTH1D_BinEdgeVector("h_mu_eta_lowPtBin_matched", vec_etaBinEdge);

    TH1D* h_mu_pt_highPtBin_all     = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_highPtBin_all",     vec_highPtBinEdge);
    TH1D* h_mu_pt_highPtBin_matched = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_highPtBin_matched", vec_highPtBinEdge);
    TH1D* h_mu_eta_highPtBin_all     = DYTool::MakeTH1D_BinEdgeVector("h_mu_eta_highPtBin_all",     vec_etaBinEdge);
    TH1D* h_mu_eta_highPtBin_matched = DYTool::MakeTH1D_BinEdgeVector("h_mu_eta_highPtBin_matched", vec_etaBinEdge);

    TH1D* h_mu_pt_lowPtBin_L1Matched = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_lowPtBin_L1Matched", vec_ptBinEdge);
    TH1D* h_mu_pt_highPtBin_L1Matched = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_highPtBin_L1Matched", vec_highPtBinEdge);

    TH1D* h_mu_pt_lowPtBin_HLTMatched  = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_lowPtBin_HLTMatched", vec_ptBinEdge);
    TH1D* h_mu_pt_highPtBin_HLTMatched = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_highPtBin_HLTMatched", vec_highPtBinEdge);

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

        // -- calc. efficiency using gen-matched offline muons
        for(auto offMuon : vec_matchedOffMuon )
        {
          if( offMuon.isTRK ) // -- only if it is tracker muon
          {
            h_mu_pt_lowPtBin_all->Fill( offMuon.pt, totWeight );
            h_mu_pt_highPtBin_all->Fill( offMuon.pt, totWeight );
            h_mu_eta_lowPtBin_all->Fill( offMuon.eta, totWeight );
            h_mu_eta_highPtBin_all->Fill( offMuon.eta, totWeight );

            // -- only L1 check
            if( IsMatched_LowPtL1(offMuon, ntuple) )  h_mu_pt_lowPtBin_L1Matched->Fill( offMuon.pt, totWeight );
            if( IsMatched_HighPtL1(offMuon, ntuple) ) h_mu_pt_highPtBin_L1Matched->Fill( offMuon.pt, totWeight );

            if( IsMatched_HLTObj(offMuon, ntuple) ) h_mu_pt_lowPtBin_HLTMatched->Fill( offMuon.pt, totWeight );
            if( IsMatched_HLTObj(offMuon, ntuple) ) h_mu_pt_highPtBin_HLTMatched->Fill( offMuon.pt, totWeight );

            TString matchingType = EfficiencyType(offMuon, ntuple);
            if( matchingType == "lowPt" ) 
            {
              h_mu_pt_lowPtBin_matched->Fill( offMuon.pt, totWeight );
              h_mu_eta_lowPtBin_matched->Fill( offMuon.eta, totWeight );
            }
            if( matchingType == "highPt" )
            {
              h_mu_pt_highPtBin_matched->Fill( offMuon.pt, totWeight );
              h_mu_eta_highPtBin_matched->Fill( offMuon.eta, totWeight );
            }
          }

        } // -- end of loop over offline muons
      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_MCTruthLegEff");
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    h_mu_pt_lowPtBin_all->Write();
    h_mu_pt_lowPtBin_matched->Write();
    h_mu_eta_lowPtBin_all->Write();
    h_mu_eta_lowPtBin_matched->Write();

    h_mu_pt_highPtBin_all->Write();
    h_mu_pt_highPtBin_matched->Write();
    h_mu_eta_highPtBin_all->Write();
    h_mu_eta_highPtBin_matched->Write();

    h_mu_pt_lowPtBin_L1Matched->Write();
    h_mu_pt_highPtBin_L1Matched->Write();

    h_mu_pt_lowPtBin_HLTMatched->Write();
    h_mu_pt_highPtBin_HLTMatched->Write();

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

  TString EfficiencyType(DYTool::OffMuon mu, DYTool::DYTree *ntuple)
  {
    TString type = "none";

    Double_t minDR = 0.3;
    Double_t minQuality = 8.0;
    Double_t minL1Pt = 15.0;
    Double_t maxL1Pt = 9999.0;
    Bool_t isHighPtL1Matched = DYTool::dRMatching_L1Muon(mu.vecP_Propagated(), ntuple, minDR, minQuality, minL1Pt, maxL1Pt );

    minL1Pt = 7.0;
    maxL1Pt = 14.9999;
    Bool_t isLowPtL1Matched = DYTool::dRMatching_L1Muon(mu.vecP_Propagated(), ntuple, minDR, minQuality, minL1Pt, maxL1Pt );

    Bool_t isHLTMatched = DYTool::dRMatching_HLTObj(mu.vecP, ntuple, "hltDoubleMu3L3FilteredNoVtx", 0.1);

    if( isHLTMatched )
    {
      if( isHighPtL1Matched ) type = "highPt";
      if( isLowPtL1Matched ) type = "lowPt";
    }

    return type;
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
    Double_t maxL1Pt = 14.9999;
    Bool_t isLowPtL1Matched = DYTool::dRMatching_L1Muon(mu.vecP_Propagated(), ntuple, minDR, minQuality, minL1Pt, maxL1Pt );

    return isLowPtL1Matched;
  }

  Bool_t IsMatched_HLTObj(DYTool::OffMuon mu, DYTool::DYTree *ntuple)
  {
    Bool_t isHLTMatched = DYTool::dRMatching_HLTObj(mu.vecP, ntuple, "hltDoubleMu3L3FilteredNoVtx", 0.1);

    return isHLTMatched;
  }

};