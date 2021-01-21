#include <Test/TrigEff_Orthogonal/OrthogonalMethodTool.h>
#include <Test/TrigEff_Orthogonal/ZPeakHistContainer.h>
#include <Test/TrigEff_Orthogonal/EffSF/EffSFTool.h>

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
    ntuple->TurnOnBranches_OffMuon();

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");

    // -- TnP efficiencies
    TnPEffTool* tnpEff_ID  = new TnPEffTool("tightID");
    TnPEffTool* tnpEff_ISO = new TnPEffTool("tightPFIso_over_tightID");

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
        // if( debug_ ) printf("  Pass SelectGenEventBySampleType\n");

        Bool_t isDYEvent = kFALSE;
        DYTool::OffMuPair DYMuPair = OffEventSelection_EGamma_TightIDISO(ntuple, isDYEvent);
        if( isDYEvent )
        {
          if( debug_ ) printf("  Pass OffEventSelection_EGamma_TightIDISO\n");
          Double_t diMuM = DYMuPair.mass;

          Double_t eff_ID_lead  = tnpEff_ID->GetEff(sampleInfo_.isMC, DYMuPair.first_.pt, fabs(DYMuPair.first_.eta));
          Double_t eff_ID_sub   = tnpEff_ID->GetEff(sampleInfo_.isMC, DYMuPair.second_.pt, fabs(DYMuPair.second_.eta));
          Double_t eff_ISO_lead = tnpEff_ISO->GetEff(sampleInfo_.isMC, DYMuPair.first_.pt, fabs(DYMuPair.first_.eta));
          Double_t eff_ISO_sub  = tnpEff_ISO->GetEff(sampleInfo_.isMC, DYMuPair.second_.pt, fabs(DYMuPair.second_.eta));

          totWeight = totWeight / (eff_ID_lead * eff_ID_sub * eff_ISO_lead * eff_ISO_sub);

          if( 81 < diMuM && diMuM < 101 ) // -- only the events close to Z peak
          {
            hist_DEN->Fill( DYMuPair, totWeight );
            if( IsFired_ScoutingPath(ntuple) ) hist_NUM->Fill( DYMuPair, totWeight );
          }
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