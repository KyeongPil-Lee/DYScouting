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

    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    // DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");

    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      // Double_t totWeight = sampleInfo_.normFactor * genWeight;
      Double_t totWeight = genWeight;

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        vector<DYTool::GenParticle> vec_genLepton;
        for(Int_t i_gen=0; i_gen<ntuple->nGenParticle; i_gen++)
        {
          DYTool::GenParticle genLepton(ntuple, i_gen);
          // if( abs(genLepton.ID) == 13 && genLepton.isHardProcess )
          if( abs(genLepton.ID) == 13 && genLepton.fromHardProcessFinalState )
            vec_genLepton.push_back( genLepton );
        }

        Int_t nGenLeptonHP = (Int_t)vec_genLepton.size();
        if( nGenLeptonHP == 2 )
        {
          // DYTool::GenPair muonPair(vec_genLepton[0], vec_genLepton[1]);
          // hists->Fill(muonPair, totWeight);

          FillHistogram( vec_genLepton[0], vec_genLepton[1], totWeight );
        }
      }
    }

    TString outputName = TString::Format("ROOTFile_MakeGenHist_Dimuon_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    SaveHistogram(f_output);

    f_output->Close();

    PrintRunTime();
  }

private:
  TH1D* h_pt_;
  TH1D* h_eta_;
  TH1D* h_diMuPt_;
  TH1D* h_diMuRap_;
  TH1D* h_diMuM_;
  TH2D* h_diMuRapPt_;

  void InitHist()
  {
    h_pt_  = new TH1D("h_pt", "", 10000, 0, 10000);
    h_eta_ = new TH1D("h_eta", "", 400, -20, 20);

    h_diMuPt_  = new TH1D("h_diMuPt", "", 10000, 0, 10000);
    h_diMuRap_ = new TH1D("h_diMuRap", "", 400, -20, 20);
    h_diMuM_   = new TH1D("h_diMuM", "", 10000, 0, 10000);

    h_diMuRapPt_ = new TH2D("h_diMuRapPt", "", 200, -10, 10, 100, 0, 100);
  }

  void FillHistogram( DYTool::GenParticle& genLepton1, DYTool::GenParticle& genLepton2, Double_t weight )
  {
    h_pt_->Fill( genLepton1.pt, weight );
    h_pt_->Fill( genLepton2.pt, weight );

    h_eta_->Fill( genLepton1.eta, weight );
    h_eta_->Fill( genLepton2.eta, weight );

    TLorentzVector vecP_diMu = (genLepton1.vecP + genLepton2.vecP);

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