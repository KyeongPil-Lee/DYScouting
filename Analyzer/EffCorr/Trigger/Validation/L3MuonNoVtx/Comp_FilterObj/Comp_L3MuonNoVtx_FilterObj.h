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

    TH1D* h_nFilterObj       = new TH1D("h_nFilterObj",       "", 100, 0, 100);
    TH1D* h_nL3MuonNoVtx_pt3 = new TH1D("h_nL3MuonNoVtx_pt3", "", 100, 0, 100);

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

        Bool_t isL1Fired  = IsFired_L1(ntuple);
        Bool_t isHLTFired = IsFired_DoubleMu3(ntuple);
        printf("[%d event] (L1, HLT) = (%d, %d)\n", i, isL1Fired, isHLTFired);

        vector<DYTool::HLTObj>      vec_filterObj   = DYTool::GetAllHLTObj(ntuple, "hltDoubleMu3L3FilteredNoVtx");
        vector<DYTool::L3MuonNoVtx> vec_L3MuonNoVtx_pt3 = DYTool::GetAllL3MuonNoVtx(ntuple, 3.0);

        printf("[Filter(hltDoubleMu3L3FilteredNoVtx) object]\n");
        for(auto& filterObj : vec_filterObj )
          printf("(pt, eta, phi) = (%.1lf, %.3lf, %.3lf)\n", filterObj.pt, filterObj.eta, filterObj.phi);

        printf("[L3MuonCandidateNoVtx object with pT > 3 GeV]\n");
        for(auto& L3MuonNoVtx_pt3 : vec_L3MuonNoVtx_pt3 )
          printf("(pt, eta, phi) = (%.1lf, %.3lf, %.3lf)\n", L3MuonNoVtx_pt3.pt, L3MuonNoVtx_pt3.eta, L3MuonNoVtx_pt3.phi);

        if( i > 100 ) break;


        Int_t nFilterObj       = (Int_t)vec_filterObj.size();
        Int_t nL3MuonNoVtx_pt3 = (Int_t)vec_L3MuonNoVtx_pt3.size();

        h_nFilterObj->Fill( nFilterObj, totWeight );
        h_nL3MuonNoVtx_pt3->Fill( nL3MuonNoVtx_pt3, totWeight );

      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("Comp_L3MuonNoVtx_FilterObj");
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    h_nFilterObj->Write();
    h_nL3MuonNoVtx_pt3->Write();

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

  Bool_t CheckMatching_L1_15_7_HLT_DoubleMu3(DYTool::DYTree* ntuple, DYTool::OffMuon mu1, DYTool::OffMuon mu2)
  {
    Bool_t flag = kFALSE;

    TString type1 = EfficiencyType(mu1, ntuple);
    TString type2 = EfficiencyType(mu2, ntuple);

    if( type1 != "none" && type2 != "none" &&
        (type1 == "highPt" || type2 == "highPt") ) // -- two muons should be matched (not "none") && at least one muon should be matched to high pT L1 (pT >15 GeV)
      flag = kTRUE;

    return flag;
  }


};