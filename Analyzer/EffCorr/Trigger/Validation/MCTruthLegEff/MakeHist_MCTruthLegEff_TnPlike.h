#include <NLOReweight/NLOWeightTool.h>
#include <TRandom.h>

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

    // -- A: leading = tag, trailing = probe
    // -- B: trailing = probe, leading = tag
    // -- C: randomly pick tag and probe between two
    TH1D* h_mu_pt_lowPtBin_A_all      = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_lowPtBin_A_all",     vec_ptBinEdge);
    TH1D* h_mu_pt_lowPtBin_A_matched  = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_lowPtBin_A_matched", vec_ptBinEdge);
    TH1D* h_mu_pt_highPtBin_A_all     = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_highPtBin_A_all",     vec_highPtBinEdge);
    TH1D* h_mu_pt_highPtBin_A_matched = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_highPtBin_A_matched", vec_highPtBinEdge);

    TH1D* h_mu_pt_lowPtBin_B_all      = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_lowPtBin_B_all",     vec_ptBinEdge);
    TH1D* h_mu_pt_lowPtBin_B_matched  = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_lowPtBin_B_matched", vec_ptBinEdge);
    TH1D* h_mu_pt_highPtBin_B_all     = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_highPtBin_B_all",     vec_highPtBinEdge);
    TH1D* h_mu_pt_highPtBin_B_matched = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_highPtBin_B_matched", vec_highPtBinEdge);

    TH1D* h_mu_pt_lowPtBin_C_all      = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_lowPtBin_C_all",     vec_ptBinEdge);
    TH1D* h_mu_pt_lowPtBin_C_matched  = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_lowPtBin_C_matched", vec_ptBinEdge);
    TH1D* h_mu_pt_highPtBin_C_all     = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_highPtBin_C_all",     vec_highPtBinEdge);
    TH1D* h_mu_pt_highPtBin_C_matched = DYTool::MakeTH1D_BinEdgeVector("h_mu_pt_highPtBin_C_matched", vec_highPtBinEdge);

    // -- for random number
    TRandom* rndm = new TRandom();

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
        if( vec_matchedOffMuon.size() != 2 ) continue; // -- at least two muons should exist

        DYTool::OffMuPair muPair(vec_matchedOffMuon[0], vec_matchedOffMuon[1]);

        // -- scenario A
        FillHistogram_TnPlike(muPair.first_, muPair.second_, ntuple, totWeight, h_mu_pt_lowPtBin_A_all, h_mu_pt_lowPtBin_A_matched, h_mu_pt_highPtBin_A_all, h_mu_pt_highPtBin_A_matched);

        // -- scenario B
        FillHistogram_TnPlike(muPair.second_, muPair.first_, ntuple, totWeight, h_mu_pt_lowPtBin_B_all, h_mu_pt_lowPtBin_B_matched, h_mu_pt_highPtBin_B_all, h_mu_pt_highPtBin_B_matched);

        // -- scenario C
        Int_t tagIndex = (Int_t)rndm->Integer(2); // -- randomly return 0 or 1
        Int_t probeIndex = 1 - tagIndex; // -- tagIndex = 0 -> probeIndex = 1; tagIndex = 1 -> probeIndex = 0;
        FillHistogram_TnPlike(vec_matchedOffMuon[tagIndex], vec_matchedOffMuon[probeIndex], ntuple, totWeight, h_mu_pt_lowPtBin_C_all, h_mu_pt_lowPtBin_C_matched, h_mu_pt_highPtBin_C_all, h_mu_pt_highPtBin_C_matched);

      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_MCTruthLegEff");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    f_output->cd();

    h_mu_pt_lowPtBin_A_all->Write();
    h_mu_pt_lowPtBin_A_matched->Write();
    h_mu_pt_highPtBin_A_all->Write();
    h_mu_pt_highPtBin_A_matched->Write();

    h_mu_pt_lowPtBin_B_all->Write();
    h_mu_pt_lowPtBin_B_matched->Write();
    h_mu_pt_highPtBin_B_all->Write();
    h_mu_pt_highPtBin_B_matched->Write();

    h_mu_pt_lowPtBin_C_all->Write();
    h_mu_pt_lowPtBin_C_matched->Write();
    h_mu_pt_highPtBin_C_all->Write();
    h_mu_pt_highPtBin_C_matched->Write();

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

  void FillHistogram_TnPlike( DYTool::OffMuon tagCand, DYTool::OffMuon probeCand, DYTool::DYTree *ntuple, Double_t weight, TH1D* h_mu_pt_lowPtBin_all, TH1D* h_mu_pt_lowPtBin_matched, TH1D* h_mu_pt_highPtBin_all, TH1D* h_mu_pt_highPtBin_matched)
  {
    if( IsMatched_HLTObj(tagCand, ntuple) )
    {
      if( probeCand.isTRK )
      {
        Double_t pt = probeCand.pt;

        h_mu_pt_lowPtBin_all->Fill( pt, weight );
        h_mu_pt_highPtBin_all->Fill( pt, weight );

        TString matchingType = EfficiencyType(probeCand, ntuple);
        if( matchingType == "lowPt" ) 
        {
          h_mu_pt_lowPtBin_matched->Fill( pt, weight );
        }
        if( matchingType == "highPt" )
        {
          h_mu_pt_highPtBin_matched->Fill( pt, weight );
        }
      }
    }
  }

};