#include <TStopwatch.h>
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

    TH1D* h_mu_pt_isTRK        = new TH1D("h_mu_pt_isTRK",        "", 10000, 0, 10000);
    TH1D* h_mu_pt_passingProbe = new TH1D("h_mu_pt_passingProbe", "", 10000, 0, 10000);

    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );
    ntuple->TurnOnBranches_OffMuon();

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
      if( sampleInfo_.isMC ) totWeight *= PUTool->Weight( ntuple->truePU );

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        // -- DY low mass sample: apply NLO/LO k-factor
        if( sampleInfo_.type.Contains("DYMuMu_M10to50"))
          totWeight *= nloWeightTool->GetWeight(ntuple);

        vector<DYTool::OffMuon> vec_muon = DYTool::GetAllOfflineMuons(ntuple);

        for( auto& mu : vec_muon )
        {
          if( mu.isTRK )
          {
            h_mu_pt_isTRK->Fill( mu.pt, totWeight );

            if( PassPassingProbeCondition_LowPtL1(mu, ntuple) )
            {
              h_mu_pt_passingProbe->Fill( mu.pt, totWeight );
            }
          }
        }

      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_Pt");
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    h_mu_pt_isTRK->Write();
    h_mu_pt_passingProbe->Write();

    f_output->Close();

    PrintRunTime();
  }

private:
  Bool_t PassPassingProbeCondition_LowPtL1(DYTool::OffMuon mu, DYTool::DYTree* ntuple)
  {
    Bool_t flag = kFALSE;
    Double_t minDR = 0.3;
    Double_t minQuality = 8; // -- double muon quality
    Double_t minL1Pt = 7.0;
    Double_t maxL1Pt = 9999.0;

    if( DYTool::dRMatching_L1Muon(mu.vecP_Propagated(), ntuple, minDR, minQuality, minL1Pt, maxL1Pt ) &&
        DYTool::IsMatched_L3MuonNoVtx_CustomSingleMuLegFilter(mu, ntuple) )
      flag = kTRUE;

    return flag;
  }

};
