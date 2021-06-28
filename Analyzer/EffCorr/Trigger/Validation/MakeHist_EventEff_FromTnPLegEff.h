#include <Include/SimplePlotTools.h>
#include <Include/DYTool.h>
#include <NLOReweight/NLOWeightTool.h>

class TnPEffMapTool
{
public:
  TString fileName_ = "";
  TString histName_lowPt_ = "";
  TString histName_highPt_ = "";

  TH2D* h2D_eff_lowPt_;
  TH2D* h2D_eff_highPt_;

  Int_t nBinX_;
  Int_t nBinY_;

  TnPEffMapTool()
  {
    Init();
  }

  Double_t GetEff(TString type, Double_t pt, Double_t eta)
  {
    Double_t eff = -1.0;

    if( type == "lowPt" )       eff = GetEff_GivenHist( h2D_eff_lowPt_, pt, eta );
    else if( type == "highPt" ) eff = GetEff_GivenHist( h2D_eff_highPt_, pt, eta );
    else
      cout << "[TnPEffMapTool::GetEff] type = " << type << " is wrong type... please check" << endl;

    return eff;
  }

  Double_t GetEff_GivenHist(TH2D* h2D_eff, Double_t pt, Double_t eta)
  {

    Int_t theBinX = h2D_eff->GetXaxis()->FindBin(pt);
    Int_t theBinY = h2D_eff->GetYaxis()->FindBin(eta);

    // -- extrapolation: under and overflow
    if( theBinX == 0 )       theBinX = 1;
    if( theBinX == nBinX_+1 ) theBinX = nBinX_;
    if( theBinY == 0 )       theBinY = 1;
    if( theBinY == nBinY_+1 ) theBinY = nBinY_;

    return h2D_eff->GetBinContent(theBinX, theBinY);
  }

private:
  void Init()
  {
    TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
    fileName_ = analyzerPath+"/EffCorr/Trigger/Validation/TnP/ROOTFile_L1HLTLegEfficiencyMap.root";
    histName_lowPt_  = "h2D_eff_lowPt";
    histName_highPt_ = "h2D_eff_highPt";

    h2D_eff_lowPt_  = PlotTool::Get_Hist2D(fileName_, histName_lowPt_);
    h2D_eff_highPt_ = PlotTool::Get_Hist2D(fileName_, histName_highPt_);

    // -- assumption: nBinX & nBinY is same for lowPt and highPt histograms
    nBinX_ = h2D_eff_lowPt_->GetNbinsX();
    nBinY_ = h2D_eff_lowPt_->GetNbinsY();
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

    TH1D* h_diMuM_noSF      = new TH1D("h_diMuM_noSF",   "", 200, 0, 200);
    TH1D* h_diMuM_corrected = new TH1D("h_diMuM_corrected", "", 200, 0, 200);

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
        if( vec_genMuon[0].pt > 5 && fabs(vec_genMuon[0].eta) < 2.4 && 
            vec_genMuon[1].pt > 5 && fabs(vec_genMuon[1].eta) < 2.4)
        {
          vector<DYTool::OffMuon> vec_matchedOffMuon = DYTool::GenMatchedOfflineMuon(ntuple);
          if( vec_matchedOffMuon.size() != 2 ) continue;

          if( vec_matchedOffMuon[0].pt > 5 && fabs(vec_matchedOffMuon[0].eta) < 2.4 && vec_matchedOffMuon[0].isTRK &&
              vec_matchedOffMuon[1].pt > 5 && fabs(vec_matchedOffMuon[1].eta) < 2.4 && vec_matchedOffMuon[1].isTRK )
          {
            Double_t diMuM = (vec_matchedOffMuon[0].vecP + vec_matchedOffMuon[1].vecP).M();

            if( IsFired_L1(ntuple) && IsFired_DoubleMu3(ntuple) )
            {
              h_diMuM_noSF->Fill(diMuM, totWeight); // -- already MC efficiency is taken into account

              Double_t eff_mu1 = GetLegEfficiency(effTool, vec_matchedOffMuon[0], ntuple);
              Double_t eff_mu2 = GetLegEfficiency(effTool, vec_matchedOffMuon[1], ntuple);

              Double_t corr = 1.0 / (eff_mu1 * eff_mu2); // -- correct the inefficiency

              h_diMuM_corrected->Fill(diMuM, totWeight*corr);
            } // -- is L1+HLT fired?
          } // -- pass acceptance at the reco level
        } // -- pass acceptance at the gen level
      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_Dimuon");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    h_diMuM_noSF->Write();
    h_diMuM_corrected->Write();

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

  Double_t GetLegEfficiency(TnPEffMapTool* effTool, DYTool::OffMuon mu, DYTool::DYTree *ntuple)
  {
    Double_t eff;

    TString type = EfficiencyType(mu, ntuple);

    if( type == "none" ) eff = 1.0; // -- non-matched
    else                 eff = effTool->GetEff( type, mu.pt, mu.eta );

    return eff;
  }


};