#include <Test/TrigEff_Orthogonal/ZPeakHistContainer.h>

class ZPeakHistProducer: public DYTool::ClassTemplate
{
public:
  ZPeakHistProducer(): ClassTemplate()
  {

  }

  void Run()
  {
    CheckSampleInfo();
    StartTimer();

    // -- define the histogram
    ZPeakHistContainer* hist_DEN = new ZPeakHistContainer("DEN"); // -- 1 muon pair with medium + loose PF iso.
    ZPeakHistContainer* hist_NUM = new ZPeakHistContainer("NUM"); // -- pass the scouting path

    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );
    // ntuple->TurnOnBranches_OffMuon(); // -- not needed

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    // -- not needed in principle, but anyway apply it to be consistent with the orthogonal method...
    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");

    for(Int_t i=0; i<nEvent; i++)
    {
      if( !debug_ ) DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      if( debug_ ) printf("[%dth event]\n", i);

      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      Double_t totWeight;
      if( sampleInfo_.isMC ) totWeight = sampleInfo_.normFactor * genWeight * PUTool->Weight( ntuple->truePU );
      else                   totWeight = 1.0;

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        vector<DYTool::GenParticle> vec_genMuon = DYTool::GetAllGenLeptons(ntuple, 13, "fromHardProcessFinalState");

        if( vec_genMuon.size() != 2 )
        {
          cout << "vec_genMuon.size() != 2 ... need to check!" << endl;
          return;
        }

        DYTool::GenPair DYMuPair = DYTool::GenPair( vec_genMuon[0], vec_genMuon[1] );

        if( DYMuPair.first_.pt > 15 && DYMuPair.second_.pt > 15 && 
            fabs(DYMuPair.first_.eta) < 2.4 && fabs(DYMuPair.second_.eta) < 2.4 ) // -- acceptance
        {
          Double_t diMuM = DYMuPair.mass;

          hist_DEN->Fill( DYMuPair, totWeight );
          if( IsFired_ScoutingPath(ntuple) ) hist_NUM->Fill( DYMuPair, totWeight );
        }

      } // -- sample type selection

    } // -- end of event iteration

    TString outputName = TString::Format("ROOTFile_ZPeakHistProducer_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    f_output->cd();

    hist_DEN->Write( f_output );
    hist_NUM->Write( f_output );

    f_output->Close();

    PrintRunTime();
  }

private:
  Bool_t debug_ = kFALSE;

  Bool_t IsFired_ScoutingPath(DYTool::DYTree* ntuple)
  {
    // -- pass unprescaled L1 (only for 2018)
    // -- 4 = L1_DoubleMu_15_7
    // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
    // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
    Bool_t doPassL1 = kFALSE;
    // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;
    if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut

    Bool_t doPassTrig = kFALSE;
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("DST_DoubleMu3_noVtx_CaloScouting_v") )
      {
        doPassTrig = kTRUE;
        break;
      }
    }

    return doPassL1 && doPassTrig;
  }
};