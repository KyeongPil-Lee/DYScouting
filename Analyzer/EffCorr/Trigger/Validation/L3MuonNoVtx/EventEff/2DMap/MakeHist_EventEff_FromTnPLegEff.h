#include <Include/SimplePlotTools.h>
#include <Include/DYTool.h>
#include <NLOReweight/NLOWeightTool.h>

class TnPEffMapTool
{
public:
  TString fileName_ = "";

  TH1D* h_eff_pt_lowPt_;
  TH1D* h_eff_pt_highPt_;

  TH1D* h_eff_eta_lowPt_;
  TH1D* h_eff_eta_highPt_;

  TH2D* h2D_eff_lowPt_;
  TH2D* h2D_eff_highPt_;

  TnPEffMapTool(TString fileName)
  {
    fileName_ = fileName;
    Init();
  }

  // -- type = lowPt, highPt
  // -- effType = 1D_pt, 1D_eta, 2D_pteta
  Double_t GetEff(TString type, TString effType, Double_t pt, Double_t eta)
  {
    Double_t eff = -1.0;
    if( type == "lowPt" )
    {
      if( effType == "1D_pt" )    eff = GetEff_GivenHist_1DMap( h_eff_pt_lowPt_, pt );
      if( effType == "1D_eta" )   eff = GetEff_GivenHist_1DMap( h_eff_eta_lowPt_, eta );
      if( effType == "2D_pteta" ) eff = GetEff_GivenHist_2DMap( h2D_eff_lowPt_, pt, eta );
    }
    else if( type == "highPt" )
    {
      if( effType == "1D_pt" )    eff = GetEff_GivenHist_1DMap( h_eff_pt_highPt_, pt );
      if( effType == "1D_eta" )   eff = GetEff_GivenHist_1DMap( h_eff_eta_highPt_, eta );
      if( effType == "2D_pteta" ) eff = GetEff_GivenHist_2DMap( h2D_eff_highPt_, pt, eta );
    }
    else
      cout << "[TnPEffMapTool::GetEff] type = " << type << ", effType = " << effType << " is wrong type... please check" << endl;

    return eff;
  }

  Double_t GetEff_GivenHist_1DMap(TH1D* h_eff, Double_t var)
  {
    Int_t nBin = h_eff->GetNbinsX();

    Int_t theBin = h_eff->GetXaxis()->FindBin(var);

    // -- extrapolation: under and overflow
    if( theBin == 0 )      theBin = 1;
    if( theBin == nBin+1 ) theBin = nBin;

    return h_eff->GetBinContent(theBin);
  }

  Double_t GetEff_GivenHist_2DMap(TH2D* h2D_eff, Double_t varX, Double_t varY)
  {
    Int_t nBinX = h2D_eff->GetNbinsX();
    Int_t nBinY = h2D_eff->GetNbinsY();

    Int_t theBinX = h2D_eff->GetXaxis()->FindBin(varX);
    Int_t theBinY = h2D_eff->GetYaxis()->FindBin(varY);

    // -- extrapolation: under and overflow
    if( theBinX == 0 )       theBinX = 1;
    if( theBinX == nBinX+1 ) theBinX = nBinX;

    if( theBinY == 0 )       theBinY = 1;
    if( theBinY == nBinY+1 ) theBinY = nBinY;

    return h2D_eff->GetBinContent(theBinX, theBinY);
  }

private:
  void Init()
  {
    h_eff_pt_lowPt_  = PlotTool::Get_Hist(fileName_, "h_eff_pt_lowPt");
    h_eff_pt_highPt_ = PlotTool::Get_Hist(fileName_, "h_eff_pt_highPt");

    h_eff_eta_lowPt_  = PlotTool::Get_Hist(fileName_, "h_eff_eta_lowPt");
    h_eff_eta_highPt_ = PlotTool::Get_Hist(fileName_, "h_eff_eta_highPt");

    h2D_eff_lowPt_  = PlotTool::Get_Hist2D(fileName_, "h2D_eff_lowPt");
    h2D_eff_highPt_ = PlotTool::Get_Hist2D(fileName_, "h2D_eff_highPt");
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

    TH1D* h_diMuM_withEff_TnP_1D_pt    = new TH1D("h_diMuM_withEff_TnP_1D_pt", "", 200, 0, 200);
    TH1D* h_diMuM_withEff_TnP_1D_eta   = new TH1D("h_diMuM_withEff_TnP_1D_eta", "", 200, 0, 200);
    TH1D* h_diMuM_withEff_TnP_2D_pteta = new TH1D("h_diMuM_withEff_TnP_2D_pteta", "", 200, 0, 200);

    // -- different leg efficiencies
    TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
    TString basePath = analyzerPath+"/EffCorr/Trigger/Validation/L3MuonNoVtx/LegEff/";
    TString effFileName_TnP = basePath+"TnP/ROOTFile_L1HLTLegEfficiencyMap.root";
    TnPEffMapTool* effTool_TnP = new TnPEffMapTool(effFileName_TnP);

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

            Double_t eventEff_TnP_1D_pt    = Calc_EventEff(effTool_TnP, "1D_pt", vec_matchedOffMuon[0], vec_matchedOffMuon[1]);
            Double_t eventEff_TnP_1D_eta   = Calc_EventEff(effTool_TnP, "1D_eta", vec_matchedOffMuon[0], vec_matchedOffMuon[1]);
            Double_t eventEff_TnP_2D_pteta = Calc_EventEff(effTool_TnP, "2D_pteta", vec_matchedOffMuon[0], vec_matchedOffMuon[1]);

            h_diMuM_withEff_TnP_1D_pt->Fill( diMuM, totWeight*eventEff_TnP_1D_pt );
            h_diMuM_withEff_TnP_1D_eta->Fill( diMuM, totWeight*eventEff_TnP_1D_eta );
            h_diMuM_withEff_TnP_2D_pteta->Fill( diMuM, totWeight*eventEff_TnP_2D_pteta );

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

    h_diMuM_withEff_TnP_1D_pt->Write();
    h_diMuM_withEff_TnP_1D_eta->Write();
    h_diMuM_withEff_TnP_2D_pteta->Write();

    f_output->Close();

    PrintRunTime();
  }

private:
  Double_t Calc_EventEff(TnPEffMapTool* effTool, TString effType, DYTool::OffMuon mu1, DYTool::OffMuon mu2)
  {
    Double_t eff_mu1_highPt = effTool->GetEff("highPt", effType, mu1.pt, mu1.eta);
    Double_t eff_mu2_highPt = effTool->GetEff("highPt", effType, mu2.pt, mu2.eta);

    Double_t eff_mu1_lowPt = effTool->GetEff("lowPt", effType, mu1.pt, mu1.eta);
    Double_t eff_mu2_lowPt = effTool->GetEff("lowPt", effType, mu2.pt, mu2.eta);

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