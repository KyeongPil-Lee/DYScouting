#include <Include/DYTool.h>
#include <NLOReweight/NLOWeightTool.h>

namespace TnPTool
{
  Bool_t PassTagCondition_JPsi(DYTool::OffMuon mu, DYTool::DYTree* ntuple)
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
  Bool_t PassProbeCondition_JPsi(DYTool::OffMuon mu, DYTool::DYTree* ntuple)
  {
    Bool_t flag = kFALSE;

    if( mu.isTRK 
        && DYTool::dRMatching_HLTObj(mu.vecP, ntuple, "hltMu7p5Track2JpsiTrackMassFiltered", 0.1) )
      flag = kTRUE;

    return flag;
  }

  Bool_t PassPassingProbeCondition_HighPtL1_JPsi(DYTool::OffMuon mu, DYTool::DYTree* ntuple)
  {
    Bool_t flag = kFALSE;
    Double_t minDR = 0.3;
    Double_t minQuality = 8; // -- double muon quality
    Double_t minL1Pt = 15.0;
    Double_t maxL1Pt = 9999.0;

    if( DYTool::dRMatching_L1Muon(mu.vecP_Propagated(), ntuple, minDR, minQuality, minL1Pt, maxL1Pt ) &&
        DYTool::IsMatched_L3MuonNoVtx_CustomSingleMuLegFilter(mu, ntuple) )
      flag = kTRUE;

    return flag;
  }

  Bool_t PassPassingProbeCondition_LowPtL1_JPsi(DYTool::OffMuon mu, DYTool::DYTree* ntuple)
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

  Bool_t PassPairCondition(DYTool::OffMuon tagCand, DYTool::OffMuon probeCand, Double_t minM, Double_t maxM)
  {
    Bool_t flag = kFALSE;

    Double_t mass = (tagCand.vecP + probeCand.vecP).M();
    Double_t deltaR = tagCand.vecP.DeltaR(probeCand.vecP);

    if( minM < mass && mass < maxM && deltaR > 0.3 )
      flag = kTRUE;

    return flag;
  }

  Bool_t PassPairCondition_JPsi(DYTool::OffMuon tagCand, DYTool::OffMuon probeCand)
  {
    return TnPTool::PassPairCondition(tagCand, probeCand, 2.9, 3.3);
  }

  // -- Z -- //
  Bool_t PassTagCondition_Z(DYTool::OffMuon mu, DYTool::DYTree* ntuple)
  {
    // -- IsoMu24 final filter: hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07 -- //
    // -- IsoMu27 final filter: hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07 -- //
    // -- Mu50 final filter: hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q -- //
    Bool_t flag = kFALSE;

    if( DYTool::dRMatching_HLTObj(mu.vecP, ntuple, "hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07", 0.1) &&
        mu.pt > 26 &&
        fabs(mu.eta) < 2.4 &&
        mu.isTight && 
        mu.relPFIso_dBeta < 0.15 )
      flag = kTRUE;

    return flag;
  }

  Bool_t PassProbeCondition_Z(DYTool::OffMuon mu, DYTool::DYTree* ntuple)
  {
    Bool_t flag = kFALSE;

    if( mu.isTRK )
      flag = kTRUE;

    return flag;
  }

  Bool_t PassPassingProbeCondition_HighPtL1_Z(DYTool::OffMuon mu, DYTool::DYTree* ntuple)
  {
    Bool_t flag = kFALSE;
    Double_t minDR = 0.3;
    Double_t minQuality = 8; // -- double muon quality
    Double_t minL1Pt = 15.0;
    Double_t maxL1Pt = 9999.0;

    if( DYTool::dRMatching_L1Muon(mu.vecP_Propagated(), ntuple, minDR, minQuality, minL1Pt, maxL1Pt ) &&
        DYTool::IsMatched_L3MuonNoVtx_CustomSingleMuLegFilter(mu, ntuple) )
      flag = kTRUE;

    return flag;
  }

  Bool_t PassPassingProbeCondition_LowPtL1_Z(DYTool::OffMuon mu, DYTool::DYTree* ntuple)
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

  Bool_t PassPairCondition_Z(DYTool::OffMuon tagCand, DYTool::OffMuon probeCand)
  {
    return TnPTool::PassPairCondition(tagCand, probeCand, 60, 120);
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

    TH1D* h_probe_pt  = new TH1D("h_probe_pt",  "", 10000, 0, 10000);
    TH1D* h_probe_eta = new TH1D("h_probe_eta", "", 60, -3, 3);
    TH2D* h2D_probe_ptEta = new TH2D("h2D_probe_ptEta", "", 50, 0, 50, 60, -3, 3);

    TH1D* h_passingProbe_lowPtL1_pt  = new TH1D("h_passingProbe_lowPtL1_pt",  "", 10000, 0, 10000);
    TH1D* h_passingProbe_lowPtL1_eta = new TH1D("h_passingProbe_lowPtL1_eta", "", 60, -3, 3);
    TH2D* h2D_passingProbe_lowPtL1_ptEta = new TH2D("h2D_passingProbe_lowPtL1_ptEta", "", 50, 0, 50, 60, -3, 3);

    TH1D* h_passingProbe_highPtL1_pt  = new TH1D("h_passingProbe_highPtL1_pt",  "", 10000, 0, 10000);
    TH1D* h_passingProbe_highPtL1_eta = new TH1D("h_passingProbe_highPtL1_eta", "", 60, -3, 3);
    TH2D* h2D_passingProbe_highPtL1_ptEta = new TH2D("h2D_passingProbe_highPtL1_ptEta", "", 50, 0, 50, 60, -3, 3);

    Bool_t isZ = kFALSE;
    Bool_t isJPsi = kFALSE;
    if( sampleInfo_.type.Contains("DYMuMu_M50toInf") ) isZ   = kTRUE;
    if( sampleInfo_.type.Contains("JPsi") )            isJPsi = kTRUE;

    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );
    ntuple->TurnOnBranches_OffMuon();

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");
    NLOWeightTool* nloWeightTool = new NLOWeightTool();

    Double_t minPt = 5.0; // -- minimum pt for vs. eta, phi, #vtx plots

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

          Bool_t doPassTag = kFALSE;
          if( isZ ) doPassTag    = TnPTool::PassTagCondition_Z(mu_tagCand, ntuple);
          if( isJPsi ) doPassTag = TnPTool::PassTagCondition_JPsi(mu_tagCand, ntuple);

          if( !doPassTag ) continue;

          vector<DYTool::OffMuon> vec_probe_sameTag;

          for(Int_t i_probeCand=0; i_probeCand<ntuple->nOffMuon; i_probeCand++)
          {
            // -- remove the case when tag muon == probe muon
            if( i_tagCand == i_probeCand ) continue;

            DYTool::OffMuon mu_probeCand( ntuple, i_probeCand );

            Bool_t doPassProbe = kFALSE;
            if( isZ ) doPassProbe    = TnPTool::PassProbeCondition_Z(mu_probeCand, ntuple);
            if( isJPsi ) doPassProbe = TnPTool::PassProbeCondition_JPsi(mu_probeCand, ntuple);

            if( !doPassProbe ) continue;

            Bool_t doPassPair = kFALSE;
            if( isZ ) doPassPair    = TnPTool::PassPairCondition_Z(mu_tagCand, mu_probeCand);
            if( isJPsi ) doPassPair = TnPTool::PassPairCondition_JPsi(mu_tagCand, mu_probeCand);

            if( doPassPair )
              vec_probe_sameTag.push_back( mu_probeCand );

          } // -- iteration over probe candidate

          // -- fill TnP histogram only when probeMultiplicity == 1
          if( (Int_t)vec_probe_sameTag.size() == 1 )
          {
            DYTool::OffMuon probe = vec_probe_sameTag[0];
            h_probe_pt->Fill( probe.pt, totWeight );
            h_probe_eta->Fill( probe.eta, totWeight );
            h2D_probe_ptEta->Fill( probe.pt, probe.eta, totWeight );

            // -- low pT L1 
            Bool_t doPassProbe_lowPtL1 = kFALSE;
            if( isZ ) doPassProbe_lowPtL1    = TnPTool::PassPassingProbeCondition_LowPtL1_Z(probe, ntuple);
            if( isJPsi ) doPassProbe_lowPtL1 = TnPTool::PassPassingProbeCondition_LowPtL1_JPsi(probe, ntuple);

            if( doPassProbe_lowPtL1 )
            {
              h_passingProbe_lowPtL1_pt->Fill( probe.pt, totWeight );
              h_passingProbe_lowPtL1_eta->Fill( probe.eta, totWeight );
              h2D_passingProbe_lowPtL1_ptEta->Fill( probe.pt, probe.eta, totWeight );
            }

            // -- high pT L1
            Bool_t doPassProbe_highPtL1 = kFALSE;
            if( isZ ) doPassProbe_highPtL1    = TnPTool::PassPassingProbeCondition_HighPtL1_Z(probe, ntuple);
            if( isJPsi ) doPassProbe_highPtL1 = TnPTool::PassPassingProbeCondition_HighPtL1_JPsi(probe, ntuple);

            if( doPassProbe_highPtL1 )
            {
              h_passingProbe_highPtL1_pt->Fill( probe.pt, totWeight );
              h_passingProbe_highPtL1_eta->Fill( probe.eta, totWeight );
              h2D_passingProbe_highPtL1_ptEta->Fill( probe.pt, probe.eta, totWeight );
            }
          } // -- end of if probeMultiplicity == 1

        } // -- iteration over tag candidate
      }
    } // -- end of event ieration

    TString outputName = GetOutputFileName("MakeHist_Statistics_Probe");
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    h_probe_pt->Write();
    h_probe_eta->Write();
    h2D_probe_ptEta->Write();

    h_passingProbe_lowPtL1_pt->Write();
    h_passingProbe_lowPtL1_eta->Write();
    h2D_passingProbe_lowPtL1_ptEta->Write();

    h_passingProbe_highPtL1_pt->Write();
    h_passingProbe_highPtL1_eta->Write();
    h2D_passingProbe_highPtL1_ptEta->Write();

    f_output->Close();

    PrintRunTime();
  }
};