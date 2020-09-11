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
#include <BDT/Trial/v06_noOS/dataset/weights/TMVAClassification_BDTG.class.C>
#include <NLOReweight/NLOWeightTool.h>
#include <ControlPlot/HistContainer.h>

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

    HistContainer* hists            = new HistContainer();
    HistContainer* hists_ZPeak      = new HistContainer();
    HistContainer* hists_M10to60    = new HistContainer();
    HistContainer* hists_M10to60_OS = new HistContainer();
    HistContainer* hists_M10to60_SS = new HistContainer();

    vector<std::string> vec_inputVarName = {
      "diMuNormVtxChi2", "diMuDEta", "diMuDPhi", 
      "caloMET_pt", 
      "nMuonHit_lead", "nMatchedStation_lead", "nPixelHit_lead", "nTrackerLayer_lead", "normChi2_lead", "relTrkIso_lead", 
      "nMuonHit_sub", "nMatchedStation_sub", "nPixelHit_sub", "nTrackerLayer_sub", "normChi2_sub", "relTrkIso_sub"
    };

    ReadBDTG* BDTClassifier = new ReadBDTG(vec_inputVarName);

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

        Bool_t isDYEvent = kFALSE;
        DYTool::MuPair DYMuPair = DYTool::EventSelection_BDTInput_noOS(ntuple, isDYEvent);
        if( isDYEvent )
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
          {
            Double_t mass = DYMuPair.mass;
            if( mass > 10 )
              hists->Fill(ntuple, DYMuPair, totWeight);

            if( 60 < mass && mass < 120 )
              hists_ZPeak->Fill(ntuple, DYMuPair, totWeight);

            if( 10 < mass && mass < 60 )
            {
              hists_M10to60->Fill(ntuple, DYMuPair, totWeight);

              if( DYMuPair.isOS ) hists_M10to60_OS->Fill(ntuple, DYMuPair, totWeight);
              else                hists_M10to60_SS->Fill(ntuple, DYMuPair, totWeight);
            }
          } // -- end of MVA checking
        }
      }
    }

    TString outputName = TString::Format("ROOTFile_MakeHist_Dimuon_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    hists->Write(f_output);

    f_output->mkdir("ZPeak");
    f_output->cd("ZPeak");
    hists_ZPeak->Write(f_output);

    f_output->mkdir("M10to60");
    f_output->cd("M10to60");
    hists_M10to60->Write(f_output);

    f_output->mkdir("M10to60_OS");
    f_output->cd("M10to60_OS");
    hists_M10to60_OS->Write(f_output);

    f_output->mkdir("M10to60_SS");
    f_output->cd("M10to60_SS");
    hists_M10to60_SS->Write(f_output);

    f_output->Close();

    PrintRunTime();
  }

};

void Test()
{
  HistProducer* producer = new HistProducer();

  producer->sampleInfo_.type = "DYMuMu_M10to50_NLOXSec_TEST";
  TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
  producer->sampleInfo_.ntuplePathFile = analyzerPath+"/Include/Example/ntuplePath_example.txt";
  producer->sampleInfo_.xSec = 10000;
  producer->sampleInfo_.sumWeight = 10000;
  Double_t lumi = 999;
  producer->sampleInfo_.normFactor = (lumi * 10000.0 ) / 10000.0;

  producer->Run();
}