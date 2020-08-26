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
#include <NLOReweight/postFSR/Weight/NLOWeightTool.h>

class HistProducer: public DYTool::ClassTemplate
{
public:
  HistProducer(): ClassTemplate()
  {

  }

  void Verbose(Bool_t flag = kTRUE)
  {
    isVerbose_ = flag;
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
    TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
    TString fileName_nloWeight = analyzerPath+"/NLOReweight/postFSR/Test_v01_SubMassRange/M10to30/Weight/ROOTFile_NLOWeight.root";
    TString histName_nloWeight = "h_diMuRapPt_weight";
    NLOWeightTool* nloWeightTool = new NLOWeightTool(fileName_nloWeight, histName_nloWeight);

    Double_t sumWeight = 0;

    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      // Double_t totWeight = sampleInfo_.normFactor * genWeight;
      // Double_t totWeight = genWeight;

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

          Double_t nloWeight = nloWeightTool->GetWeight(ntuple);
          Double_t totWeight = genWeight * nloWeight;

          if( isVerbose_ )
          {
            cout << "[" << i << "th event]" << endl;
            printf("   (genWeight, nloWeight) = (%lf, %lf) ---> totWeight = %lf\n", genWeight, nloWeight, totWeight);
          }

          FillHistogram( vec_genLepton[0], vec_genLepton[1], totWeight );
          sumWeight += totWeight;
        }
      }
    }
    
    h_sumWeight_->SetBinContent(1, sumWeight);
    h_sumWeight_->SetBinError(1, 0);

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

  TH1D* h_sumWeight_;

  Bool_t isVerbose_ = kFALSE;

  void InitHist()
  {
    const Int_t nPtBin = 17;
    Double_t arr_ptBinEdge[nPtBin+1] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30, 40, 50, 1000};

    const Int_t nRapBin = 10;
    Double_t arr_rapBinEdge[nRapBin+1] = {-10, -4, -2, -1, -0.5, 0, 0.5, 1, 2, 4, 10};

    h_pt_  = new TH1D("h_pt", "", nPtBin, arr_ptBinEdge);
    h_eta_ = new TH1D("h_eta", "", nRapBin, arr_rapBinEdge);

    h_diMuPt_  = new TH1D("h_diMuPt", "", nPtBin, arr_ptBinEdge);
    h_diMuRap_ = new TH1D("h_diMuRap", "", nRapBin, arr_rapBinEdge);
    h_diMuM_   = new TH1D("h_diMuM", "", 10000, 0, 10000);

    h_diMuRapPt_ = new TH2D("h_diMuRapPt", "", nRapBin, arr_rapBinEdge, nPtBin, arr_ptBinEdge);

    h_sumWeight_ = new TH1D("h_sumWeight", "", 1, 0, 1);
  }

  void FillHistogram( DYTool::GenParticle& genLepton1, DYTool::GenParticle& genLepton2, Double_t weight )
  {
    TLorentzVector vecP_diMu = (genLepton1.vecP + genLepton2.vecP);
    Double_t diMuM = vecP_diMu.M();
    Double_t minM = 10;
    Double_t maxM = 30;
    if( !(minM < diMuM && diMuM < maxM) ) return;

    h_pt_->Fill( genLepton1.pt, weight );
    h_pt_->Fill( genLepton2.pt, weight );

    h_eta_->Fill( genLepton1.eta, weight );
    h_eta_->Fill( genLepton2.eta, weight );

    h_diMuPt_->Fill( vecP_diMu.Pt(), weight );
    h_diMuRap_->Fill( vecP_diMu.Rapidity(), weight );
    h_diMuM_->Fill( vecP_diMu.M(), weight );

    h_diMuRapPt_->Fill( vecP_diMu.Rapidity(), vecP_diMu.Pt(), weight );

    if( isVerbose_ )
    {
      printf("   (dimuon mass, pt, rapidity) = (%lf, %lf, %lf)\n\n", vecP_diMu.M(), vecP_diMu.Pt(), vecP_diMu.Rapidity());
    }
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

    h_sumWeight_->Write();
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
  producer->Verbose();

  producer->Run();
}