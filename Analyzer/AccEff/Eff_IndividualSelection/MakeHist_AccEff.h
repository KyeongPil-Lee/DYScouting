#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TEfficiency.h>

#include <vector>

#include <Include/DYTool.h>
#include <NLOReweight/NLOWeightTool.h>
#include <ControlPlot/HistContainer.h>
#include <BDT/Trial/v06_noOS/dataset/weights/TMVAClassification_BDTG.class.C>

class AccEffProducer: public DYTool::ClassTemplate
{
public:
  AccEffProducer(): ClassTemplate()
  {

  }

  void Run()
  {
    CheckSampleInfo();
    StartTimer();

    // -- initialize the histograms
    const Int_t nMassBin = 44;
    Double_t arr_massBinEdge[nMassBin+1] = {
      10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
      64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
      110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
      200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
      830, 1000, 1500, 3000};

    TH1D* h_acc_pass = new TH1D("h_acc_pass", "", nMassBin, arr_massBinEdge);
    TH1D* h_eff_pass_L1      = new TH1D("h_eff_pass_L1", "", nMassBin, arr_massBinEdge);
    TH1D* h_eff_pass_L1HLT   = new TH1D("h_eff_pass_L1HLT", "", nMassBin, arr_massBinEdge);
    TH1D* h_eff_pass_presel  = new TH1D("h_eff_pass_presel", "", nMassBin, arr_massBinEdge);
    TH1D* h_eff_pass_full    = new TH1D("h_eff_pass_full", "", nMassBin, arr_massBinEdge);

    // -- BDT setup
    vector<std::string> vec_inputVarName = {
      "diMuNormVtxChi2", "diMuDEta", "diMuDPhi", 
      "caloMET_pt", 
      "nMuonHit_lead", "nMatchedStation_lead", "nPixelHit_lead", "nTrackerLayer_lead", "normChi2_lead", "relTrkIso_lead", 
      "nMuonHit_sub", "nMatchedStation_sub", "nPixelHit_sub", "nTrackerLayer_sub", "normChi2_sub", "relTrkIso_sub"
    };

    ReadBDTG* BDTClassifier = new ReadBDTG(vec_inputVarName);

    // -- ntuple setup
    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");
    NLOWeightTool* nloWeightTool = new NLOWeightTool();

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

        vector<DYTool::GenParticle> vec_genLepton = DYTool::GetAllGenLeptons(ntuple, 13, "fromHardProcessFinalState");
        if( vec_genLepton.size() != 2 )
        {
          cout << "vec_genLepton.size() != 2 even though it pass SelectGenEventBySampleType" << endl;
          return;
        }

        DYTool::GenPair genMuPair(vec_genLepton[0], vec_genLepton[1]);
        Double_t diMuM = genMuPair.mass;

        // -- acceptance test
        Bool_t doPass_acc = PassAcceptance(genMuPair);        
        if( doPass_acc )
        {
          h_acc_pass->Fill( diMuM, totWeight );

          // -- efficiency test: for the events passing the acceptance only
          Bool_t doPass_L1       = kFALSE; // -- pass the L1
          Bool_t doPass_L1HLT    = kFALSE; // -- pass the L1+HLT
          Bool_t doPass_presel   = kFALSE; // -- pass the preselection
          Bool_t doPass_eff_full = kFALSE; // -- pass the full efficiency step (preselection + BDT)

          doPass_eff_full = PassEfficiency(ntuple, BDTClassifier, doPass_L1, doPass_L1HLT, doPass_presel);
          if( doPass_L1 )       h_eff_pass_L1->Fill( diMuM, totWeight );
          if( doPass_L1HLT )    h_eff_pass_L1HLT->Fill( diMuM, totWeight );
          if( doPass_presel )   h_eff_pass_presel->Fill( diMuM, totWeight );
          if( doPass_eff_full ) h_eff_pass_full->Fill( diMuM, totWeight );

        } // -- end of doPass_acc
      } // -- end of SelectGenEventBySampleType
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_AccEff");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    f_output->cd();

    h_acc_pass->Write();
    h_eff_pass_L1->Write();
    h_eff_pass_L1HLT->Write();
    h_eff_pass_presel->Write();
    h_eff_pass_full->Write();

    f_output->Close();

    PrintRunTime();
  }
private:
  Bool_t PassAcceptance(DYTool::GenPair genPair)
  {
    Bool_t doPass = kFALSE;

    if( genPair.first_.pt > 5  && fabs(genPair.first_.eta) < 2.4 && 
        genPair.second_.pt > 5 && fabs(genPair.second_.eta) < 2.4 )
      doPass = kTRUE;

    return doPass;
  }

  Bool_t PassEfficiency(DYTool::DYTree *ntuple, ReadBDTG* BDTClassifier, Bool_t& doPass_L1, Bool_t& doPass_L1HLT, Bool_t& doPass_presel)
  {
    Bool_t doPass = kFALSE;

    // -- doPass_presel will be decided
    DYTool::MuPair DYMuPair = EventSelection_BDTInput_noOS_AdditionalInfo(ntuple, doPass_L1, doPass_L1HLT, doPass_presel);

    if( doPass_presel )
    {
      vector<Double_t> vec_BDTInputVar = {
        DYMuPair.normVtxChi2, DYMuPair.dEta, DYMuPair.dPhi,
        ntuple->caloMET_pt,
        (Double_t)DYMuPair.first_.nMuonHit, (Double_t)DYMuPair.first_.nMatchedStation, (Double_t)DYMuPair.first_.nPixelHit, (Double_t)DYMuPair.first_.nTrackerLayer, DYMuPair.first_.normChi2, DYMuPair.first_.relTrkIso,
        (Double_t)DYMuPair.second_.nMuonHit, (Double_t)DYMuPair.second_.nMatchedStation, (Double_t)DYMuPair.second_.nPixelHit, (Double_t)DYMuPair.second_.nTrackerLayer, DYMuPair.second_.normChi2, DYMuPair.second_.relTrkIso
      };

      Double_t mvaValue = BDTClassifier->GetMvaValue( vec_BDTInputVar );

      Double_t WP = 0.466016; // -- 78% signal efficiency with 90% background rejection
      if (mvaValue > WP )
        doPass = kTRUE;
    }

    return doPass;
  }

  DYTool::MuPair EventSelection_BDTInput_noOS_AdditionalInfo(DYTool::DYTree *ntuple, Bool_t& doPass_L1, Bool_t& doPass_L1HLT, Bool_t& doPass)
  {
    doPass_L1 = kFALSE;
    doPass_L1HLT = kFALSE;
    doPass = kFALSE;

    DYTool::MuPair muPair_dummy;

    // -- at least 1 vertex in a event (not checking whether it is the vertex for selected muons, yet)
    if( ntuple->nVtx <= 0 ) return muPair_dummy;

    // -- pass unprescaled L1 (only for 2018)
    // -- 4 = L1_DoubleMu_15_7
    // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
    // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
    // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPass_L1 = kTRUE;
    if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPass_L1 = kTRUE; // -- no L1 w/ mass cut

    if( !doPass_L1 ) return muPair_dummy;

    // -- pass HLT
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("DST_DoubleMu3_noVtx_CaloScouting_v") )
      {
        doPass_L1HLT = kTRUE;
        break;
      }
    }

    if( !doPass_L1HLT ) return muPair_dummy;

    // -- dimuon selection
    vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

    vector< DYTool::MuPair > vec_goodMuPair;
    for( auto& muPair : vec_muPair )
      if( muPair.IsDYCandidate_BDTInput_noOS(ntuple) ) vec_goodMuPair.push_back( muPair );

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
      std::sort(vec_goodMuPair.begin(), vec_goodMuPair.end(), DYTool::CompareMuonPair_SmallerVtxChi2);
      return vec_goodMuPair[0]; // -- pair with smallest vertex chi2 value
    }
  }

};

void Test()
{
  AccEffProducer* producer = new AccEffProducer();

  producer->sampleInfo_.type = "DYMuMu_M10to50_NLOXSec_TEST";
  producer->sampleInfo_.ntuplePathFile = "/Users/KyeongPil_Lee/Physics/DYScouting/Analyzer/Include/Example/ntuplePath_example.txt";
  producer->sampleInfo_.xSec = 10000;
  producer->sampleInfo_.sumWeight = 10000;
  Double_t lumi = 999;
  producer->sampleInfo_.normFactor = (lumi * 10000.0 ) / 10000.0;

  producer->Run();
}