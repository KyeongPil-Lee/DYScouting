#include <Include/SimplePlotTools.h>
#include <Include/DYTool.h>
#include <NLOReweight/NLOWeightTool.h>

class TnPEffMapTool
{
public:
  TString fileName_ = "";

  TH1D* h_eff_pt_lowPt_;
  TH1D* h_eff_pt_highPt_;

  TnPEffMapTool(TString fileName)
  {
    fileName_ = fileName;
    Init();
  }

  Double_t GetEff(TString type, Double_t pt, Double_t eta)
  {
    Double_t eff = -1.0;

    if( type == "lowPt" )       eff = GetEff_GivenHist( h_eff_pt_lowPt_, pt, eta );
    else if( type == "highPt" ) eff = GetEff_GivenHist( h_eff_pt_highPt_, pt, eta );
    else
      cout << "[TnPEffMapTool::GetEff] type = " << type << " is wrong type... please check" << endl;

    return eff;
  }

  Double_t GetEff_GivenHist(TH1D* h_eff_pt, Double_t pt, Double_t eta)
  {
    Int_t nPtBin = h_eff_pt->GetNbinsX();

    Int_t thePtBin = h_eff_pt->GetXaxis()->FindBin(pt);

    // -- extrapolation: under and overflow
    if( thePtBin == 0 )        thePtBin = 1;
    if( thePtBin == nPtBin+1 ) thePtBin = nPtBin;

    return h_eff_pt->GetBinContent(thePtBin);
  }

private:
  void Init()
  {
    h_eff_pt_lowPt_  = PlotTool::Get_Hist(fileName_, "h_eff_pt_lowPt");
    h_eff_pt_highPt_ = PlotTool::Get_Hist(fileName_, "h_eff_pt_highPt");
  }
};

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

    // -- histogram for MC-truth event efficiency (trigger bit)
    TH1D* h_diMuM_acc   = new TH1D("h_diMuM_acc",   "", 200, 0, 200); // -- also a denominator for the event efficiency calculation from leg efficiencies
    TH1D* h_diMuM_fired = new TH1D("h_diMuM_fired", "", 200, 0, 200);

    // -- histogram for different leg efficiency type
    TH1D* h_diMuM_withEff_MCTruth = new TH1D("h_diMuM_withEff_MCTruth", "", 200, 0, 200);
    TH1D* h_diMuM_corr_MCTruth    = new TH1D("h_diMuM_corr_MCTruth",    "", 200, 0, 200);

    TH1D* h_diMuM_withEff_TnP = new TH1D("h_diMuM_withEff_TnP", "", 200, 0, 200);
    TH1D* h_diMuM_corr_TnP    = new TH1D("h_diMuM_corr_TnP",    "", 200, 0, 200);

    TH1D* h_diMuM_withEff_TnP_DMu3 = new TH1D("h_diMuM_withEff_TnP_DMu3", "", 200, 0, 200);
    TH1D* h_diMuM_corr_TnP_DMu3    = new TH1D("h_diMuM_corr_TnP_DMu3",    "", 200, 0, 200);

    TH1D* h_diMuM_withEff_TnP_DMu3_tag = new TH1D("h_diMuM_withEff_TnP_DMu3_tag", "", 200, 0, 200);
    TH1D* h_diMuM_corr_TnP_DMu3_tag    = new TH1D("h_diMuM_corr_TnP_DMu3_tag",    "", 200, 0, 200);

    // -- different leg efficiencies
    TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
    TString basePath = analyzerPath+"/EffCorr/Trigger/Validation/L3MuonNoVtx/LegEff/";
    TString effFileName_MCTruth      = basePath+"MCTruth/ROOTFile_L1HLTLegEfficiencyMap.root";
    TString effFileName_TnP          = basePath+"TnP/ROOTFile_L1HLTLegEfficiencyMap.root";
    TString effFileName_TnP_DMu3     = basePath+"TnP_DoubleMu3/Tag_Default/ROOTFile_L1HLTLegEfficiencyMap.root";
    TString effFileName_TnP_DMu3_tag = basePath+"TnP_DoubleMu3/Tag_FilterObjMatching/ROOTFile_L1HLTLegEfficiencyMap.root";

    TnPEffMapTool* effTool_MCTruth      = new TnPEffMapTool(effFileName_MCTruth);
    TnPEffMapTool* effTool_TnP          = new TnPEffMapTool(effFileName_TnP);
    TnPEffMapTool* effTool_TnP_DMu3     = new TnPEffMapTool(effFileName_TnP_DMu3);
    TnPEffMapTool* effTool_TnP_DMu3_tag = new TnPEffMapTool(effFileName_TnP_DMu3_tag);

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

        vector<DYTool::GenParticle> vec_genMuon = DYTool::GetAllGenLeptons(ntuple, 13, "fromHardProcessFinalState");
        if( vec_genMuon.size() != 2 ) continue;

        // -- if this event passes the acceptance condition at gen level
        if( vec_genMuon[0].pt > 5 && fabs(vec_genMuon[0].eta) < 2.4 && 
            vec_genMuon[1].pt > 5 && fabs(vec_genMuon[1].eta) < 2.4)
        {
          vector<DYTool::OffMuon> vec_matchedOffMuon = DYTool::GenMatchedOfflineMuon(ntuple);
          if( vec_matchedOffMuon.size() != 2 ) continue;

          if( vec_matchedOffMuon[0].pt > 5 && fabs(vec_matchedOffMuon[0].eta) < 2.4 && vec_matchedOffMuon[0].isTRK &&
              vec_matchedOffMuon[1].pt > 5 && fabs(vec_matchedOffMuon[1].eta) < 2.4 && vec_matchedOffMuon[1].isTRK )
          {
            Double_t diMuM = (vec_matchedOffMuon[0].vecP + vec_matchedOffMuon[1].vecP).M();

            // -- all events within the detector acceptance
            h_diMuM_acc->Fill( diMuM, totWeight );

            // -- calc. efficiency using the muons with the same condition of the denominator (= isTRK) (i.e. not the muons already fire the trigger)
            Double_t eventEff_MCTruth      = Calc_EventEff(effTool_MCTruth, vec_matchedOffMuon[0], vec_matchedOffMuon[1]);
            Double_t eventEff_TnP          = Calc_EventEff(effTool_TnP, vec_matchedOffMuon[0], vec_matchedOffMuon[1]);
            Double_t eventEff_TnP_DMu3     = Calc_EventEff(effTool_TnP_DMu3, vec_matchedOffMuon[0], vec_matchedOffMuon[1]);
            Double_t eventEff_TnP_DMu3_tag = Calc_EventEff(effTool_TnP_DMu3_tag, vec_matchedOffMuon[0], vec_matchedOffMuon[1]);

            h_diMuM_withEff_MCTruth->Fill(diMuM, totWeight*eventEff_MCTruth);
            h_diMuM_withEff_TnP->Fill(diMuM, totWeight*eventEff_TnP);
            h_diMuM_withEff_TnP_DMu3->Fill(diMuM, totWeight*eventEff_TnP_DMu3);
            h_diMuM_withEff_TnP_DMu3_tag->Fill(diMuM, totWeight*eventEff_TnP_DMu3_tag);

            Double_t corr_MCTruth      = eventEff_MCTruth != 0 ?      1.0 / eventEff_MCTruth      : 1e10;
            Double_t corr_TnP          = eventEff_TnP != 0 ?          1.0 / eventEff_TnP          : 1e10;
            Double_t corr_TnP_DMu3     = eventEff_TnP_DMu3 != 0 ?     1.0 / eventEff_TnP_DMu3     : 1e10;
            Double_t corr_TnP_DMu3_tag = eventEff_TnP_DMu3_tag != 0 ? 1.0 / eventEff_TnP_DMu3_tag : 1e10;

            h_diMuM_corr_MCTruth->Fill(diMuM, totWeight*corr_MCTruth);
            h_diMuM_corr_TnP->Fill(diMuM, totWeight*corr_TnP);
            h_diMuM_corr_TnP_DMu3->Fill(diMuM, totWeight*corr_TnP_DMu3);
            h_diMuM_corr_TnP_DMu3_tag->Fill(diMuM, totWeight*corr_TnP_DMu3_tag);

            // -- MC-truth event efficiency using the trigger bit
            if( IsFired_L1(ntuple) && IsFired_DoubleMu3(ntuple) ) h_diMuM_fired->Fill( diMuM, totWeight );

          } // -- pass acceptance at the reco level
        } // -- pass acceptance at the gen level
      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_EventEff");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    h_diMuM_acc->Write();
    h_diMuM_fired->Write();

    h_diMuM_withEff_MCTruth->Write();
    h_diMuM_withEff_TnP->Write();
    h_diMuM_withEff_TnP_DMu3->Write();
    h_diMuM_withEff_TnP_DMu3_tag->Write();

    h_diMuM_corr_MCTruth->Write();
    h_diMuM_corr_TnP->Write();
    h_diMuM_corr_TnP_DMu3->Write();
    h_diMuM_corr_TnP_DMu3_tag->Write();

    f_output->Close();

    PrintRunTime();
  }

private:
  Double_t Calc_EventEff(TnPEffMapTool* effTool, DYTool::OffMuon mu1, DYTool::OffMuon mu2)
  {
    Double_t eff_mu1_highPt = effTool->GetEff("highPt", mu1.pt, mu1.eta);
    Double_t eff_mu2_highPt = effTool->GetEff("highPt", mu2.pt, mu2.eta);

    Double_t eff_mu1_lowPt = effTool->GetEff("lowPt", mu1.pt, mu1.eta);
    Double_t eff_mu2_lowPt = effTool->GetEff("lowPt", mu2.pt, mu2.eta);

    // -- + eff_mu1_highPt*eff_mu2_lowPt:  1st muon: matched to high pT leg / 2nd muon: matched to low pT leg
    // -- + eff_mu2_highPt*eff_mu1_lowPt:  2nd muon: matched to high pT leg / 1st muon: matched to low pT leg
    // -- - eff_mu1_highPt*eff_mu2_highpt: remove duplicated case: both muons are matched to the high pT leg (included in above two terms twice)
    Double_t eventEff = eff_mu1_highPt*eff_mu2_lowPt + eff_mu2_highPt*eff_mu1_lowPt - eff_mu1_highPt*eff_mu2_highPt;

    return eventEff;
  }


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

};