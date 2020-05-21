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
#include <NLOReweight/postFSR/Weight/MassDepW/Validation/HistContainer.h>

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

    HistContainer* hists = new HistContainer();

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
          hists->FillHistogram( vec_genLepton[0], vec_genLepton[1], totWeight );
      }
    }
    
    TString outputName = TString::Format("ROOTFile_MakeGenHist_Dimuon_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    f_output->cd();
    hists->SaveHistogram(f_output);

    f_output->Close();

    PrintRunTime();
  }

private:

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