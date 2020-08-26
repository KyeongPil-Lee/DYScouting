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

    InitHist();

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

        if( abs(ID) == 13 && isHP == 1 )
          vec_index_genMuonHP.push_back(i_gen);
      }

      if( vec_index_genMuonHP.size() == 2 ) // -- DY->mumu event
      {
        TLorentzVector vecP_mu1 = GetMuonMomentumVector( vec_index_genMuonHP[0] );
        TLorentzVector vecP_mu2 = GetMuonMomentumVector( vec_index_genMuonHP[1] );

        FillHistogram( vecP_mu1, vecP_mu2, totWeight );
      }

    } // -- event iteration

    TString outputName = TString::Format("ROOTFile_MakeGenHist_Dimuon_%s_NLO.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    SaveHistogram(f_output);

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

  TH1D* h_pt_;
  TH1D* h_eta_;
  TH1D* h_diMuPt_;
  TH1D* h_diMuRap_;
  TH1D* h_diMuM_;
  TH2D* h_diMuRapPt_;

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

    chain->SetBranchAddress("GENnPair", &nGenLepton_);
    chain->SetBranchAddress("GENEvt_weight", &genEvent_weight_);
    chain->SetBranchAddress("GENLepton_Px", genLepton_px_);
    chain->SetBranchAddress("GENLepton_Py", genLepton_py_);
    chain->SetBranchAddress("GENLepton_Pz", genLepton_pz_);
    chain->SetBranchAddress("GENLepton_ID", &genLepton_ID_);
    chain->SetBranchAddress("GENLepton_isHardProcess", &genLepton_isHardProcess_);
  }

  void InitHist()
  {
    h_pt_  = new TH1D("h_pt", "", 10000, 0, 10000);
    h_eta_ = new TH1D("h_eta", "", 400, -20, 20);

    h_diMuPt_  = new TH1D("h_diMuPt", "", 10000, 0, 10000);
    h_diMuRap_ = new TH1D("h_diMuRap", "", 400, -20, 20);
    h_diMuM_   = new TH1D("h_diMuM", "", 10000, 0, 10000);

    h_diMuRapPt_ = new TH2D("h_diMuRapPt", "", 200, -10, 10, 100, 0, 100);
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

  void FillHistogram( TLorentzVector& vecP_mu1, TLorentzVector& vecP_mu2, Double_t weight )
  {
    h_pt_->Fill( vecP_mu1.Pt(), weight );
    h_pt_->Fill( vecP_mu2.Pt(), weight );

    h_eta_->Fill( vecP_mu1.Eta(), weight );
    h_eta_->Fill( vecP_mu2.Eta(), weight );

    TLorentzVector vecP_diMu = (vecP_mu1 + vecP_mu2);

    h_diMuPt_->Fill( vecP_diMu.Pt(), weight );
    h_diMuRap_->Fill( vecP_diMu.Rapidity(), weight );
    h_diMuM_->Fill( vecP_diMu.M(), weight );

    h_diMuRapPt_->Fill( vecP_diMu.Rapidity(), vecP_diMu.Pt(), weight );
  }

  void SaveHistogram( TFile *f_output )
  {
    f_output->cd();

    h_pt_->Write();
    h_eta_->Write();
    h_diMuPt_->Write();
    h_diMuRap_->Write();
    h_diMuM_->Write();
    h_diMuRapPt_->Write();
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