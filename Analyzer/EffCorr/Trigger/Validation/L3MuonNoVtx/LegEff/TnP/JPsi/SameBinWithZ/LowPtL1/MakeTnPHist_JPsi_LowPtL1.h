#include <TStopwatch.h>
#include <EffCorr/Trigger/Validation/L3MuonNoVtx/LegEff/TnP/JPsi/SameBinWithZ/LowPtL1/TnPTool_JPsi.h>
#include <NLOReweight/NLOWeightTool.h>

namespace TnPTool
{

// -- define Tag & Probe condition outside of TnPPair: 
// ---- to use it in HistProducer (only loop over the muons passing tag or probe condition to reduce the runtime)
Bool_t PassTagCondition(DYTool::OffMuon mu, DYTool::DYTree* ntuple)
{
  // -- IsoMu24 final filter: hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07 -- //
  // -- IsoMu27 final filter: hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07 -- //
  // -- Mu50 final filter: hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q -- //
  // -- Mu7p5_Track2_JPsi, muon leg: hltL3fLMu7p5TrackL3Filtered7p5
  Bool_t flag = kFALSE;

  if( DYTool::dRMatching_HLTObj(mu.vecP, ntuple, "hltL3fLMu7p5TrackL3Filtered7p5", 0.1) &&
      mu.pt > 8 &&
      fabs(mu.eta) < 2.4 &&
      mu.isTight ) // -- remove isolation condition (mu.relPFIso_dBeta < 0.15)
    flag = kTRUE;

  return flag;
}

// -- Mu7p5_Track2_JPsi, track leg: hltMu7p5Track2JpsiTrackMassFiltered
Bool_t PassProbeCondition(DYTool::OffMuon mu, DYTool::DYTree* ntuple)
{
  Bool_t flag = kFALSE;

  if( mu.isTRK 
      && DYTool::dRMatching_HLTObj(mu.vecP, ntuple, "hltMu7p5Track2JpsiTrackMassFiltered", 0.1) )
    flag = kTRUE;

  return flag;
}

Bool_t PassPassingProbeCondition(DYTool::OffMuon mu, DYTool::DYTree* ntuple)
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


class TnPPair: public TnPPairBase
{
public:
  TnPPair( DYTool::OffMuon tagCand, DYTool::OffMuon probeCand, DYTool::DYTree* ntuple ): 
  TnPPairBase( tagCand, probeCand, ntuple )
  {

  }

  // -- user-defined tag condition -- //
  Bool_t PassTagCondition()
  {
    return TnPTool::PassTagCondition(tag_, ntuple_);
  }

  Bool_t PassProbeCondition()
  {
    return TnPTool::PassProbeCondition(probe_, ntuple_);
  }

  Bool_t PassPassingProbeCondition()
  {
    return TnPTool::PassPassingProbeCondition(probe_, ntuple_);
  }
};

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

    Double_t minPt = 5.0; // -- minimum pt for vs. eta, phi, #vtx plots
    TnPHistProducer* tnpHist = new TnPHistProducer(minPt);


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

        // -- make tag&pobe pair
        for(Int_t i_tagCand=0; i_tagCand<ntuple->nOffMuon; i_tagCand++)
        {
          DYTool::OffMuon mu_tagCand( ntuple, i_tagCand );
          if( !TnPTool::PassTagCondition(mu_tagCand, ntuple) ) continue; // -- check here to save runtime

          // -- collect the probes sharing same tag
          vector<TnPPair*> vec_tnpPairs_sameTag;

          for(Int_t i_probeCand=0; i_probeCand<ntuple->nOffMuon; i_probeCand++)
          {
            // -- remove the case when tag muon == probe muon
            if( i_tagCand == i_probeCand ) continue;

            DYTool::OffMuon mu_probeCand( ntuple, i_probeCand );
            if( !TnPTool::PassProbeCondition(mu_probeCand, ntuple) ) continue; // -- check here to save runtime

            TnPPair *tnpPair_ij = new TnPPair( mu_tagCand, mu_probeCand, ntuple );
            if( tnpPair_ij->IsValid() )
              vec_tnpPairs_sameTag.push_back( tnpPair_ij );
            else
              delete tnpPair_ij;
          } // -- loop over probe candidates

          // -- fill TnP histogram only when probeMultiplicity == 1
          if( (Int_t)vec_tnpPairs_sameTag.size() == 1 ) 
            tnpHist->Fill( vec_tnpPairs_sameTag[0], totWeight );

          for( auto tnpPair : vec_tnpPairs_sameTag )
            delete tnpPair;

        } // -- loop over tag candidates
      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("MakeHist_TnPHist");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    tnpHist->Save( f_output );

    f_output->Close();

    PrintRunTime();
  }

private:

};
