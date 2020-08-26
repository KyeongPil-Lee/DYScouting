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
#include <NLOReweight/postFSR/Weight/LOWeighted/Test_MassDep/HistContainer.h>

#define ArrSize 50000

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

    HistContainer* hists_below30 = new HistContainer();
    HistContainer* hists_above30 = new HistContainer();

    TChain *chain = new TChain("recoTree/DYTree");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);
    InitChain(chain);

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);

      chain->GetEntry(i);

      Double_t genWeight;
      genEvent_weight_ < 0 ? genWeight = -1 : genWeight = 1;

      Double_t totWeight = genWeight;

      vector<Int_t> vec_index_genMuonHP;
      for(Int_t i_gen=0; i_gen<nGenLepton_; i_gen++)
      {
        Int_t ID   = genLepton_ID_[i_gen];
        Int_t isHP = genLepton_isHardProcess_[i_gen];
        Int_t isFHFS = genLepton_fromHardProcessFinalState_[i_gen];

        // if( abs(ID) == 13 && isHP == 1 )
        if( abs(ID) == 13 && isFHFS == 1 )
          vec_index_genMuonHP.push_back(i_gen);
      }

      if( vec_index_genMuonHP.size() == 2 ) // -- DY->mumu event
      {
        TLorentzVector vecP_mu1 = GetMuonMomentumVector( vec_index_genMuonHP[0] );
        TLorentzVector vecP_mu2 = GetMuonMomentumVector( vec_index_genMuonHP[1] );

        Double_t diMuM = (vecP_mu1+vecP_mu2).M();

        if( diMuM < 30 )
          hists_below30->FillHistogram( vecP_mu1, vecP_mu2, totWeight );
        else
          hists_above30->FillHistogram( vecP_mu1, vecP_mu2, totWeight );
      }

    } // -- event iteration

    TString outputName = TString::Format("ROOTFile_MakeGenHist_Dimuon_%s_NLO.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    f_output->mkdir("below30GeV");
    f_output->cd("below30GeV");
    hists_below30->SaveHistogram(f_output);

    f_output->cd();
    f_output->mkdir("above30GeV");
    f_output->cd("above30GeV");
    hists_above30->SaveHistogram(f_output);

    f_output->Close();

    PrintRunTime();
  }

private:
  Int_t    nGenLepton_;
  Double_t genEvent_weight_;
  Double_t genLepton_px_[ArrSize];
  Double_t genLepton_py_[ArrSize];
  Double_t genLepton_pz_[ArrSize];
  Int_t genLepton_ID_[ArrSize];
  Int_t genLepton_isHardProcess_[ArrSize];
  Int_t genLepton_fromHardProcessFinalState_[ArrSize];

  void InitChain(TChain *chain)
  {
    chain->SetBranchStatus("*", 0);

    chain->SetBranchStatus("GENnPair", 1);
    chain->SetBranchStatus("GENEvt_weight", 1);
    chain->SetBranchStatus("GENLepton_Px", 1);
    chain->SetBranchStatus("GENLepton_Py", 1);
    chain->SetBranchStatus("GENLepton_Pz", 1);
    chain->SetBranchStatus("GENLepton_ID", 1);
    chain->SetBranchStatus("GENLepton_isHardProcess", 1);
    chain->SetBranchStatus("GENLepton_fromHardProcessFinalState", 1);

    chain->SetBranchAddress("GENnPair", &nGenLepton_);
    chain->SetBranchAddress("GENEvt_weight", &genEvent_weight_);
    chain->SetBranchAddress("GENLepton_Px", genLepton_px_);
    chain->SetBranchAddress("GENLepton_Py", genLepton_py_);
    chain->SetBranchAddress("GENLepton_Pz", genLepton_pz_);
    chain->SetBranchAddress("GENLepton_ID", &genLepton_ID_);
    chain->SetBranchAddress("GENLepton_isHardProcess", &genLepton_isHardProcess_);
    chain->SetBranchAddress("GENLepton_fromHardProcessFinalState", &genLepton_fromHardProcessFinalState_);
  }

  TLorentzVector GetMuonMomentumVector(Int_t i_mu)
  {
    TLorentzVector vecP;

    Double_t px = genLepton_px_[i_mu];
    Double_t py = genLepton_py_[i_mu];
    Double_t pz = genLepton_pz_[i_mu];
    Double_t mass  = 0.1056583715; // -- muon mass in GeV

    Double_t energy = sqrt(px*px + py*py + pz*pz + mass*mass);
    vecP.SetPxPyPzE(px, py, pz, energy);

    return vecP;
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