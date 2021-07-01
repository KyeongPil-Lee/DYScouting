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

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");
    NLOWeightTool* nloWeightTool = new NLOWeightTool();

    // -- histogram
    TH1D* h_nMuon       = new TH1D("h_nMuon",       "", 100, 0, 100);
    TH1D* h_nMuon_trig  = new TH1D("h_nMuon_trig",  "", 100, 0, 100);
    TH1D* h_nMuon_acc   = new TH1D("h_nMuon_acc",   "", 100, 0, 100);
    TH1D* h_nMuon_IDISO = new TH1D("h_nMuon_IDISO", "", 100, 0, 100);

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

        h_nMuon->Fill( ntuple->nMuon, totWeight );

        if( IsFired_L1(ntuple) && IsFired_DoubleMu3(ntuple) )
        {
          h_nMuon_trig->Fill( ntuple->nMuon, totWeight );

          Int_t nMuon_passingAcc = 0;
          Int_t nMuon_passingAccIDIso = CountMuon_PassingAccIDIso(ntuple, nMuon_passingAcc);

          h_nMuon_acc->Fill( nMuon_passingAcc, totWeight );
          h_nMuon_IDISO->Fill( nMuon_passingAccIDIso, totWeight );
        } // -- end of L1+HLT fired
      } // -- end of select MC type
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_Dimuon");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    f_output->cd();

    h_nMuon->Write();
    h_nMuon_trig->Write();
    h_nMuon_acc->Write();
    h_nMuon_IDISO->Write();

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

  Int_t CountMuon_PassingAccIDIso(DYTool::DYTree *ntuple, Int_t& nMuon_passingAcc)
  {
    nMuon_passingAcc = 0;
    Int_t nMuon_passingAccIDIso = 0;

    vector<DYTool::Muon> vec_scoutingMuon = DYTool::GetAllMuons(ntuple);
    for( auto mu : vec_scoutingMuon )
    {
      if( mu.pt > 5 && fabs(mu.eta) < 2.4 )
      {
        nMuon_passingAcc++;

        if( mu.nPixelHit > 0 && mu.nTrackerLayer > 5 && mu.normChi2 < 10 && mu.relTrkIso < 0.15 ) nMuon_passingAccIDIso++;
      }    
    }

    return nMuon_passingAccIDIso;
  }

};

void Test()
{
  HistProducer* producer = new HistProducer();

  producer->sampleInfo_.type = "DYMuMu_M10to50_NLOXSec_TEST";
  producer->sampleInfo_.ntuplePathFile = "/Users/KyeongPil_Lee/Physics/DYScouting/Analyzer/Include/Example/ntuplePath_example.txt";
  producer->sampleInfo_.xSec = 10000;
  producer->sampleInfo_.sumWeight = 10000;
  Double_t lumi = 999;
  producer->sampleInfo_.normFactor = (lumi * 10000.0 ) / 10000.0;

  producer->Run();
}