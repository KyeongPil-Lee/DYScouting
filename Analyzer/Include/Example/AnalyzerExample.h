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

    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );

    // -- turn on necessary branches here
    // ---- e.g. ntuple->TurnOnBranches_GenLepton();

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    // -- initiate histograms here

    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      DYTool::loadBar(i_ev+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i_ev);

      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      Double_t totWeight = sampleInfo_.normFactor * genWeight;

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        vector< DYTool::Muon > vec_muon = DYTool::GetAllMuons(ntuple);

        printf("[%d event]\n", i_ev);


        for(const auto& muon : vec_muon )
        {
          Int_t i_mu = &muon - &vec_muon[0];
          printf("  [%02d muon] (pt, eta, phi, charge) = (%lf, %lf, %lf, %lf)\n", i_mu, muon.pt, muon.eta, muon.phi, muon.charge);
        }

        printf("\n");

        // -- fill your histograms here
      }
    }

    TString outputName = TString::Format("ROOTFile_AnalyzerTemplate_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    // -- save your results here
    
    f_output->Close();

    PrintRunTime();
  }
};

void AnalyzerExample_TEST()
{
  HistProducer* producer = new HistProducer();

  producer->sampleInfo_.type = "DYMuMu_M10to50_NLOXSec";
  producer->sampleInfo_.ntuplePathFile = "/Users/KyeongPil_Lee/Physics/DYScouting/Analyzer/Include/Example/ntuplePath_example.txt";
  producer->sampleInfo_.xSec = 10000;
  producer->sampleInfo_.sumWeight = 10000;
  Double_t lumi = 999;
  producer->sampleInfo_.normFactor = (lumi * 10000.0 ) / 10000.0;

  producer->Run();
}