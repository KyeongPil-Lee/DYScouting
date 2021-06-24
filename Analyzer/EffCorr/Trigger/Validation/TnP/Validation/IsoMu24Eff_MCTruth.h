#include <Test/SingleMuon/OfflineHistContainer.h>
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
    ntuple->TurnOnBranches_OffMuon();

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");
    NLOWeightTool* nloWeightTool = new NLOWeightTool();

    const Int_t nPtBin = 12;
    const Double_t arr_ptBinEdge[nPtBin+1] = {2, 18, 22, 24, 26, 30, 40, 50, 60, 120, 200, 300, 500};
    TH1D* h_muPt_DEN = new TH1D("h_diMuM_DEN", "", nPtBin, arr_ptBinEdge);
    TH1D* h_muPt_NUM = new TH1D("h_diMuM_NUM", "", nPtBin, arr_ptBinEdge);

    const Int_t nEtaBin = 14;
    const Double_t arr_etaBinEdge[nEtaBin+1] = {-2.4, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.4};
    TH1D* h_muEta_DEN = new TH1D("h_diMuM_DEN", "", nEtaBin, arr_etaBinEdge);
    TH1D* h_muEta_NUM = new TH1D("h_diMuM_NUM", "", nEtaBin, arr_etaBinEdge);

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
        vector<DYTool::OffMuon> vec_matchedOffMuon = DYTool::GenMatchedOfflineMuon(ntuple);
        if( vec_matchedOffMuon.size() != 2 ) continue;

        for(auto matchedOffMuon : vec_matchedOffMuon )
        {
          if( matchedOffMuon.isTight && matchedOffMuon.relPFIso_dBeta < 0.15 && fabs(matchedOffMuon.eta) < 2.4 )
          {
            h_muPt_DEN->Fill( matchedOffMuon.pt, totWeight );

            Bool_t isMatchedToHLTObj = DYTool::dRMatching_HLTObj(matchedOffMuon.vecP, ntuple, "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07", 0.1);
            if( isMatchedToHLTObj ) h_muPt_NUM->Fill( matchedOffMuon.pt, totWeight );


            if( matchedOffMuon.pt > 26 )
            {
              h_muEta_DEN->Fill( matchedOffMuon.eta, totWeight );
              if( isMatchedToHLTObj ) h_muEta_NUM->Fill( matchedOffMuon.eta, totWeight );
            }
          }
        } // -- loop over matched offline muons      
      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_MCTruthIsoMu24Eff");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    h_muPt_DEN->Write();
    h_muPt_NUM->Write();
    h_muEta_DEN->Write();
    h_muEta_NUM->Write();

    f_output->Close();

    PrintRunTime();
  }

private:
};