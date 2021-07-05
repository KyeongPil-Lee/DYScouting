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

  TH1D* h_eff_pt_lowPt_;
  // Int_t nPtBin_lowPt_;

  TH1D* h_eff_pt_highPt_;
  // Int_t nPtBin_highPt_;

  TnPEffMapTool()
  {
    Init();
  }

  // Double_t GetEff(TString type, Double_t pt, Double_t eta)
  // {
  //   Double_t eff = -1.0;

  //   if( type == "lowPt" )       eff = GetEff_GivenHist( h2D_eff_lowPt_, pt, eta );
  //   else if( type == "highPt" ) eff = GetEff_GivenHist( h2D_eff_highPt_, pt, eta );
  //   else
  //     cout << "[TnPEffMapTool::GetEff] type = " << type << " is wrong type... please check" << endl;

  //   return eff;
  // }

  // Double_t GetEff_GivenHist(TH2D* h2D_eff, Double_t pt, Double_t eta)
  // {

  //   Int_t theBinX = h2D_eff->GetXaxis()->FindBin(pt);
  //   Int_t theBinY = h2D_eff->GetYaxis()->FindBin(eta);

  //   // -- extrapolation: under and overflow
  //   if( theBinX == 0 )       theBinX = 1;
  //   if( theBinX == nBinX_+1 ) theBinX = nBinX_;
  //   if( theBinY == 0 )       theBinY = 1;
  //   if( theBinY == nBinY_+1 ) theBinY = nBinY_;

  //   return h2D_eff->GetBinContent(theBinX, theBinY);
  // }

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
    if( thePtBin == 0 )       thePtBin = 1;
    if( thePtBin == nPtBin+1 ) thePtBin = nPtBin;

    return h_eff_pt->GetBinContent(thePtBin);
  }

private:
  void Init()
  {
    TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
    fileName_ = analyzerPath+"/EffCorr/Trigger/Validation/FineTnPPtBin/SplitLowHighL1/ROOTFile_L1HLTLegEfficiencyMap.root";
    histName_lowPt_  = "h2D_eff_lowPt";
    histName_highPt_ = "h2D_eff_highPt";

    h2D_eff_lowPt_  = PlotTool::Get_Hist2D(fileName_, histName_lowPt_);
    h2D_eff_highPt_ = PlotTool::Get_Hist2D(fileName_, histName_highPt_);

    // -- assumption: nBinX & nBinY is same for lowPt and highPt histograms
    nBinX_ = h2D_eff_lowPt_->GetNbinsX();
    nBinY_ = h2D_eff_lowPt_->GetNbinsY();

    h_eff_pt_lowPt_ = PlotTool::Get_Hist(fileName_, "h_eff_pt_lowPt");
    // nPtBin_lowPt_ = h_eff_pt_lowPt_->GetNbinsX();

    h_eff_pt_highPt_ = PlotTool::Get_Hist(fileName_, "h_eff_pt_highPt");
    // nPtBin_highPt_ = h_eff_pt_highPt_->GetNbinsX();
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

    TH1D* h_diMuM_acc     = new TH1D("h_diMuM_acc",     "", 200, 0, 200);
    TH1D* h_diMuM_noEff   = new TH1D("h_diMuM_noEff",   "", 200, 0, 200);
    TH1D* h_diMuM_withEff = new TH1D("h_diMuM_withEff", "", 200, 0, 200);
    TH1D* h_diMuM_corr    = new TH1D("h_diMuM_corr",    "", 200, 0, 200);

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
            h_diMuM_acc->Fill( diMuM, totWeight );

            if( IsFired_L1(ntuple) && IsFired_DoubleMu3(ntuple) )
            {
              h_diMuM_noEff->Fill(diMuM, totWeight); // -- already MC efficiency is taken into account

              Double_t eff_mu1 = GetLegEfficiency(effTool, vec_matchedOffMuon[0], ntuple);
              Double_t eff_mu2 = GetLegEfficiency(effTool, vec_matchedOffMuon[1], ntuple);

              Double_t eventEff = eff_mu1 * eff_mu2;
              h_diMuM_withEff->Fill(diMuM, totWeight*eventEff);

              Double_t corr;
              if( eventEff > 0 ) corr = 1.0 / eventEff;
              else
              {
                cout << "eventEff = " << eventEff << " ... set the correction as 1e10" << endl;
                corr = 1e10;
              }
              h_diMuM_corr->Fill(diMuM, totWeight*corr);

            } // -- is L1+HLT fired?
          } // -- pass acceptance at the reco level
        } // -- pass acceptance at the gen level
      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_EventEff");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    h_diMuM_acc->Write();
    h_diMuM_noEff->Write();
    h_diMuM_withEff->Write();
    h_diMuM_corr->Write();
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