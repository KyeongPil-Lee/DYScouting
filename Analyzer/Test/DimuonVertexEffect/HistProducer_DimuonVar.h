// -- compare the distributions
// ---- 1) select the events with only exact two muons
// ---- 2) select the dimuon candidate with best vertex chi2

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

    // -- define the histogram
    TH1D* h_diMu_mass_twoMu = new TH1D("h_diMu_mass_twoMu", "", 10000, 0, 10000);
    TH1D* h_diMu_pt_twoMu   = new TH1D("h_diMu_pt_twoMu", "", 10000, 0, 10000);
    TH1D* h_diMu_rap_twoMu  = new TH1D("h_diMu_rap_twoMu", "", 60, -3, 3);

    TH1D* h_diMu_mass_vtxChi2 = new TH1D("h_diMu_mass_vtxChi2", "", 10000, 0, 10000);
    TH1D* h_diMu_pt_vtxChi2   = new TH1D("h_diMu_pt_vtxChi2", "", 10000, 0, 10000);
    TH1D* h_diMu_rap_vtxChi2  = new TH1D("h_diMu_rap_vtxChi2", "", 60, -3, 3);

    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );
    ntuple->TurnOnBranches_OffMuon();

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      // -- do not normalize to the integrated luminosity; just pure statistics
      Double_t totWeight;
      if( sampleInfo_.isMC ) totWeight = genWeight;
      else                   totWeight = 1.0;

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        // if( debug_ ) printf("  Pass SelectGenEventBySampleType\n");

        if( !IsFired_IsoMu24(ntuple) ) continue;

        vector<DYTool::OffMuPair> vec_allPair = DYTool::GetAllOffMuPairs(ntuple);
        vector<DYTool::OffMuPair> vec_selectedPair;
        for(auto muPair : vec_allPair )
        {
          if( muPair.first_.pt > 28.0  && fabs(muPair.first_.eta) < 2.4 &&
              muPair.second_.pt > 17.0 && fabs(muPair.second_.eta) < 2.4 && 
              muPair.first_.isTight  && muPair.first_.relPFIso_dBeta < 0.15 && 
              muPair.second_.isTight && muPair.second_.relPFIso_dBeta < 0.15 &&
              muPair.mass > 50.0 )
            vec_selectedPair.push_back( muPair );
        }

        if( vec_selectedPair.size() == 1 )
        {
          h_diMu_mass_twoMu->Fill( vec_selectedPair[0].mass );
          h_diMu_pt_twoMu->Fill( vec_selectedPair[0].pt );
          h_diMu_rap_twoMu->Fill( vec_selectedPair[0].rap );
        }

        std::sort(vec_selectedPair.begin(), vec_selectedPair.end(), DYTool::CompareOffMuonPair_SmallerVtxChi2);
        if( vec_selectedPair.size() > 0 )
        {
          h_diMu_mass_vtxChi2->Fill( vec_selectedPair[0].mass );
          h_diMu_pt_vtxChi2->Fill( vec_selectedPair[0].pt );
          h_diMu_rap_vtxChi2->Fill( vec_selectedPair[0].rap );
        }
      } // -- sample type selection

    } // -- end of event iteration

    TString outputName = TString::Format("ROOTFile_HistProducer_DimuonVar_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    f_output->cd();

    h_diMu_mass_twoMu->Write();
    h_diMu_pt_twoMu->Write();
    h_diMu_rap_twoMu->Write();

    h_diMu_mass_vtxChi2->Write();
    h_diMu_pt_vtxChi2->Write();
    h_diMu_rap_vtxChi2->Write();

    f_output->Close();

    PrintRunTime();
  }

private:
  Bool_t IsFired_IsoMu24(DYTool::DYTree* ntuple)
  {
    Bool_t doPassTrig = kFALSE;
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("HLT_IsoMu24_v") )
      {
        doPassTrig = kTRUE;
        break;
      }
    }

    return doPassTrig;
  }
};