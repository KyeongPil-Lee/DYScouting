#include <Include/SimplePlotTools.h>
#include <Include/DYTool.h>
#include <NLOReweight/NLOWeightTool.h>

class TnPEffMapTool
{
public:
  TString fileName_ = "";
  TString histName_ = "";

  TH1D* h_eff_pt_ = nullptr;
  Int_t nBin_;

  TnPEffMapTool()
  {
    Init();
  }

  Double_t GetEff(Double_t pt)
  {
    Int_t theBin = h_eff_pt_->GetXaxis()->FindBin(pt);

    // -- extrapolation: under and overflow
    if( theBin == 0 )       theBin = 1;
    if( theBin == nBin_+1 ) theBin = nBin_;

    return h_eff_pt_->GetBinContent(theBin);
  } 

private:
  void Init()
  {
    TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
    fileName_ = analyzerPath+"/EffCorr/Trigger/Validation/Test_IsoMu24/TnP/ROOTFile_TnPEff_IsoMu24.root";
    histName_ = "h_eff_pt";

    h_eff_pt_ = PlotTool::Get_Hist(fileName_, histName_);
    nBin_ = h_eff_pt_->GetNbinsX();
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

    TH1D* h_diMuM_noEff   = new TH1D("h_diMuM_noEff",   "", 200, 0, 200);
    TH1D* h_diMuM_withEff = new TH1D("h_diMuM_withEff", "", 200, 0, 200);

    TnPEffMapTool* effTool = new TnPEffMapTool();

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
        if( vec_genMuon[0].pt > 26 && fabs(vec_genMuon[0].eta) < 2.4 && 
            vec_genMuon[1].pt > 10 && fabs(vec_genMuon[1].eta) < 2.4)
        {
          vector<DYTool::OffMuon> vec_matchedOffMuon = DYTool::GenMatchedOfflineMuon(ntuple);
          if( vec_matchedOffMuon.size() != 2 ) continue;

          if( vec_matchedOffMuon[0].pt > 26 && fabs(vec_matchedOffMuon[0].eta) < 2.4 && vec_matchedOffMuon[0].isTight && vec_matchedOffMuon[0].relPFIso_dBeta < 0.15 &&
              vec_matchedOffMuon[1].pt > 10 && fabs(vec_matchedOffMuon[1].eta) < 2.4 && vec_matchedOffMuon[1].isTight && vec_matchedOffMuon[1].relPFIso_dBeta < 0.15 )
          {
            Double_t diMuM = (vec_matchedOffMuon[0].vecP + vec_matchedOffMuon[1].vecP).M();

            h_diMuM_noEff->Fill(diMuM, totWeight);

            // -- without checking IsoMu24 firing
            Double_t eff_mu1 = effTool->GetEff(vec_matchedOffMuon[0].pt);
            Double_t eff_mu2 = effTool->GetEff(vec_matchedOffMuon[1].pt);

            Double_t eventEff = 1.0 - (1.0 - eff_mu1) * (1.0 - eff_mu2);
            h_diMuM_withEff->Fill(diMuM, totWeight*eventEff);

          } // -- pass acceptance at the reco level
        } // -- pass acceptance at the gen level
      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_EventEff");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    h_diMuM_noEff->Write();
    h_diMuM_withEff->Write();
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

  Bool_t IsFired_IsoMu24(DYTool::DYTree* ntuple)
  {
    Bool_t doPass_HLT = kFALSE;
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("HLT_IsoMu24_v") ) doPass_HLT = kTRUE;

      if( doPass_HLT ) break;
    }

    return doPass_HLT;
  }
};