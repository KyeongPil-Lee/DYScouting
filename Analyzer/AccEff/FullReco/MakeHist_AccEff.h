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

    TH1D* h_acc_tot  = new TH1D("h_acc_tot", "", nMassBin, arr_massBinEdge);
    TH1D* h_acc_pass = new TH1D("h_acc_pass", "", nMassBin, arr_massBinEdge);
    TH1D* h_eff_pass = new TH1D("h_eff_pass", "", nMassBin, arr_massBinEdge);

    // -- ntuple setup
    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );
    ntuple->TurnOnBranches_OffMuon();

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");
    NLOWeightTool* nloWeightTool = new NLOWeightTool();

    nEvent = 1000000;

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

        h_acc_tot->Fill( diMuM, totWeight );

        // -- acceptance test
        Bool_t doPass_acc = PassAcceptance(genMuPair);        
        if( doPass_acc )
        {
          h_acc_pass->Fill( diMuM, totWeight );

          // -- efficiency test: for the events passing the acceptance only
          Bool_t doPass_eff = PassEfficiency(ntuple);
          if( doPass_eff ) h_eff_pass->Fill( diMuM, totWeight );

        } // -- end of doPass_acc
      } // -- end of SelectGenEventBySampleType
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_AccEff");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    f_output->cd();

    h_acc_tot->Write();
    h_acc_pass->Write();
    h_eff_pass->Write();

    f_output->Close();

    PrintRunTime();
  }
private:
  Bool_t PassAcceptance(DYTool::GenPair genPair)
  {
    Bool_t doPass = kFALSE;

    if( genPair.first_.pt > 28  && fabs(genPair.first_.eta) < 2.4 && 
        genPair.second_.pt > 17 && fabs(genPair.second_.eta) < 2.4 )
      doPass = kTRUE;

    return doPass;
  }

  Bool_t PassEfficiency(DYTool::DYTree *ntuple)
  {
    Bool_t doPass = kFALSE;

    // -- pass HLT
    Bool_t doPassTrig = kFALSE;
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("HLT_IsoMu24_v") )
      {
        doPassTrig = kTRUE;
        break;
      }
    }

    if( !doPassTrig ) return kFALSE;

    // -- dimuon selection
    vector< DYTool::OffMuPair > vec_muPair = DYTool::GetAllOffMuPairs(ntuple);

    vector< DYTool::OffMuPair > vec_goodMuPair;
    for( auto& muPair : vec_muPair )
      if( IsDYCandidate(muPair) ) vec_goodMuPair.push_back( muPair );

    if( vec_goodMuPair.size() > 0 ) doPass = kTRUE;

    return doPass;
  }

  Bool_t IsDYCandidate(DYTool::OffMuPair& muPair)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( muPair.first_.pt > 28  && fabs(muPair.first_.eta) < 2.4 &&
        muPair.second_.pt > 17 && fabs(muPair.second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( muPair.first_.isTight  && muPair.first_.relPFIso_dBeta < 0.15 &&
        muPair.second_.isTight && muPair.second_.relPFIso_dBeta < 0.15 )
      isGoodMuon = kTRUE;

    if( isWithinAcc && isGoodMuon && muPair.angle3D < TMath::Pi() - 0.005 && muPair.normVtxChi2 < 20 ) flag = kTRUE;

    return flag;
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