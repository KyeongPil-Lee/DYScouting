#pragma once

#define ArrSize 50000
#include <TTree.h>
#include <TChain.h>
#include <vector>

namespace DYTool
{

// -- for the physical variables, do not use _ at the end of the variable name.
// -- because getter functions for those variables will not be created (too many!)
class DYTree
{
public:
  TChain *chain_;

  // --  event informations
  Int_t runNum;
  Int_t lumiBlockNum;
  ULong64_t eventNum;

  Double_t genWeight;

  Int_t truePU;

  // -- trigger information
  vector< std::string >* vec_firedTrigger;
  vector< Bool_t >* vec_L1Bit;
  vector< Int_t >*  vec_L1Prescale;

  // -- vertex information (@ HLT)
  Int_t nVtx;
  Double_t vtx_x[ArrSize];
  Double_t vtx_y[ArrSize];
  Double_t vtx_z[ArrSize];
  Double_t vtx_xErr[ArrSize];
  Double_t vtx_yErr[ArrSize];
  Double_t vtx_zErr[ArrSize];
  Double_t vtx_chi2[ArrSize];
  Int_t    vtx_nDOF[ArrSize];
  Int_t    vtx_isValid[ArrSize];

  // -- pixel vertex from HLT tracking
  Int_t nPixelVtx;
  Double_t pixelVtx_x[ArrSize];
  Double_t pixelVtx_y[ArrSize];
  Double_t pixelVtx_z[ArrSize];
  Double_t pixelVtx_xErr[ArrSize];
  Double_t pixelVtx_yErr[ArrSize];
  Double_t pixelVtx_zErr[ArrSize];
  Double_t pixelVtx_chi2[ArrSize];
  Double_t pixelVtx_nDOF[ArrSize];
  Int_t    pixelVtx_isValid[ArrSize];

  // -- pixel vertex made by tracks near L3 muons
  Int_t nPixelVtxMu;
  Double_t pixelVtxMu_x[ArrSize];
  Double_t pixelVtxMu_y[ArrSize];
  Double_t pixelVtxMu_z[ArrSize];
  Double_t pixelVtxMu_xErr[ArrSize];
  Double_t pixelVtxMu_yErr[ArrSize];
  Double_t pixelVtxMu_zErr[ArrSize];
  Double_t pixelVtxMu_chi2[ArrSize];
  Double_t pixelVtxMu_nDOF[ArrSize];
  Int_t    pixelVtxMu_isValid[ArrSize];

  // -- muon information
  Int_t nMuon;
  Double_t muon_pt[ArrSize];
  Double_t muon_eta[ArrSize];
  Double_t muon_phi[ArrSize];
  // Double_t muon_px[ArrSize]; // -- px, py and pz are not available for scouting muons
  // Double_t muon_py[ArrSize];
  // Double_t muon_pz[ArrSize];
  Double_t muon_charge[ArrSize];

  Int_t muon_nPixelHit[ArrSize];
  Int_t muon_nStripHit[ArrSize];
  Int_t muon_nTrackerLayer[ArrSize];
  Int_t muon_nMuonHit[ArrSize];
  Int_t muon_nMatchedStation[ArrSize];
  Double_t muon_normChi2[ArrSize];
  Double_t muon_dxy[ArrSize];
  Double_t muon_dz[ArrSize];
  Double_t muon_trkIso[ArrSize];

  vector< vector<int> >* muon_vtxIndex;

  // -- generator level information
  Int_t nGenParticle;
  Int_t genParticle_ID[ArrSize];
  Int_t genParticle_status[ArrSize];
  Int_t genParticle_mother[ArrSize];

  Double_t genParticle_pt[ArrSize];
  Double_t genParticle_eta[ArrSize];
  Double_t genParticle_phi[ArrSize];
  Double_t genParticle_px[ArrSize];
  Double_t genParticle_py[ArrSize];
  Double_t genParticle_pz[ArrSize];
  Double_t genParticle_energy[ArrSize];
  Double_t genParticle_charge[ArrSize];

  Int_t genParticle_isPrompt[ArrSize];
  Int_t genParticle_isPromptFinalState[ArrSize];
  Int_t genParticle_isTauDecayProduct[ArrSize];
  Int_t genParticle_isPromptTauDecayProduct[ArrSize];
  Int_t genParticle_isDirectPromptTauDecayProductFinalState[ArrSize];
  Int_t genParticle_isHardProcess[ArrSize];
  Int_t genParticle_isLastCopy[ArrSize];
  Int_t genParticle_isLastCopyBeforeFSR[ArrSize];
  Int_t genParticle_isPromptDecayed[ArrSize];
  Int_t genParticle_isDecayedLeptonHadron[ArrSize];
  Int_t genParticle_fromHardProcessBeforeFSR[ArrSize];
  Int_t genParticle_fromHardProcessDecayed[ArrSize];
  Int_t genParticle_fromHardProcessFinalState[ArrSize];
  Int_t genParticle_isMostlyLikePythia6Status3[ArrSize];

  Double_t caloMET_phi;
  Double_t caloMET_pt;
  Double_t rho;

  // -- offline objects
  // -- offline muons
  Int_t           nOffMuon;
  Double_t        offMuon_pt[ArrSize];
  Double_t        offMuon_eta[ArrSize];
  Double_t        offMuon_phi[ArrSize];
  Double_t        offMuon_px[ArrSize];
  Double_t        offMuon_py[ArrSize];
  Double_t        offMuon_pz[ArrSize];
  Double_t        offMuon_dB[ArrSize];
  Double_t        offMuon_charge[ArrSize];
  Int_t           offMuon_isGLB[ArrSize];
  Int_t           offMuon_isSTA[ArrSize];
  Int_t           offMuon_isTRK[ArrSize];
  Int_t           offMuon_isPF[ArrSize];
  Int_t           offMuon_isTight[ArrSize];
  Int_t           offMuon_isMedium[ArrSize];
  Int_t           offMuon_isLoose[ArrSize];
  Int_t           offMuon_isHighPt[ArrSize];
  Int_t           offMuon_isSoft[ArrSize];
  Double_t        offMuon_iso03_sumPt[ArrSize];
  Double_t        offMuon_iso03_hadEt[ArrSize];
  Double_t        offMuon_iso03_emEt[ArrSize];
  Double_t        offMuon_PFIso03_charged[ArrSize];
  Double_t        offMuon_PFIso03_neutral[ArrSize];
  Double_t        offMuon_PFIso03_photon[ArrSize];
  Double_t        offMuon_PFIso03_sumPU[ArrSize];
  Double_t        offMuon_PFIso04_charged[ArrSize];
  Double_t        offMuon_PFIso04_neutral[ArrSize];
  Double_t        offMuon_PFIso04_photon[ArrSize];
  Double_t        offMuon_PFIso04_sumPU[ArrSize];
  Double_t        offMuon_PFCluster03_ECAL[ArrSize];
  Double_t        offMuon_PFCluster03_HCAL[ArrSize];
  Double_t        offMuon_PFCluster04_ECAL[ArrSize];
  Double_t        offMuon_PFCluster04_HCAL[ArrSize];
  Double_t        offMuon_normChi2_global[ArrSize];
  Int_t           offMuon_nTrackerHit_global[ArrSize];
  Int_t           offMuon_nTrackerLayer_global[ArrSize];
  Int_t           offMuon_nPixelHit_global[ArrSize];
  Int_t           offMuon_nMuonHit_global[ArrSize];
  Double_t        offMuon_normChi2_inner[ArrSize];
  Int_t           offMuon_nTrackerHit_inner[ArrSize];
  Int_t           offMuon_nTrackerLayer_inner[ArrSize];
  Int_t           offMuon_nPixelHit_inner[ArrSize];
  Double_t        offMuon_pt_tuneP[ArrSize];
  Double_t        offMuon_ptError_tuneP[ArrSize];
  Double_t        offMuon_dxyVTX_best[ArrSize];
  Double_t        offMuon_dzVTX_best[ArrSize];
  Int_t           offMuon_nMatchedStation[ArrSize];
  Int_t           offMuon_nMatchedRPCLayer[ArrSize];
  Int_t           offMuon_stationMask[ArrSize];

  DYTree()
  {
    cout << "Default constructor ... nothing happens" << endl;
  }

  DYTree( TChain *chain )
  {
    chain_ = chain;

    chain_->SetBranchStatus("*", 0);
    // -- turn on all branches
    TurnOnBranches_EventInfo();
    TurnOnBranches_VtxInfo();
    TurnOnBranches_Muon();
    TurnOnBranches_GenParticle();
  }

  void GetEvent(Int_t index)
  {
    chain_->GetEntry(index);
  }

  void TurnOnBranches_EventInfo()
  {
    // -- Event Information -- //
    chain_->SetBranchStatus("runNum", 1);
    chain_->SetBranchStatus("lumiBlockNum", 1);
    chain_->SetBranchStatus("eventNum", 1);
    chain_->SetBranchStatus("genWeight", 1);
    chain_->SetBranchStatus("truePU", 1);
    chain_->SetBranchStatus("vec_firedTrigger", 1);
    chain_->SetBranchStatus("vec_L1Bit", 1);
    chain_->SetBranchStatus("vec_L1Prescale", 1);
    chain_->SetBranchStatus("caloMET_pt", 1);
    chain_->SetBranchStatus("caloMET_phi", 1);
    chain_->SetBranchStatus("rho", 1);

    chain_->SetBranchAddress("runNum",           &runNum);
    chain_->SetBranchAddress("lumiBlockNum",     &lumiBlockNum);
    chain_->SetBranchAddress("eventNum",         &eventNum);
    chain_->SetBranchAddress("genWeight",        &genWeight);
    chain_->SetBranchAddress("truePU",           &truePU);
    chain_->SetBranchAddress("vec_firedTrigger", &vec_firedTrigger);
    chain_->SetBranchAddress("vec_L1Bit",      &vec_L1Bit);
    chain_->SetBranchAddress("vec_L1Prescale", &vec_L1Prescale);
    chain_->SetBranchAddress("caloMET_pt", &caloMET_pt);
    chain_->SetBranchAddress("caloMET_phi", &caloMET_phi);
    chain_->SetBranchAddress("rho", &rho);
  }

  void TurnOnBranches_VtxInfo()
  {
    // -- vertex information (@ HLT)
    chain_->SetBranchStatus("nVtx", 1);
    chain_->SetBranchStatus("vtx_x", 1);
    chain_->SetBranchStatus("vtx_y", 1);
    chain_->SetBranchStatus("vtx_z", 1);
    chain_->SetBranchStatus("vtx_xErr", 1);
    chain_->SetBranchStatus("vtx_yErr", 1);
    chain_->SetBranchStatus("vtx_zErr", 1);
    chain_->SetBranchStatus("vtx_chi2", 1);
    chain_->SetBranchStatus("vtx_nDOF", 1);
    chain_->SetBranchStatus("vtx_isValid", 1);

    chain_->SetBranchAddress("nVtx",     &nVtx);
    chain_->SetBranchAddress("vtx_x",    &vtx_x);
    chain_->SetBranchAddress("vtx_y",    &vtx_y);
    chain_->SetBranchAddress("vtx_z",    &vtx_z);
    chain_->SetBranchAddress("vtx_xErr",    &vtx_xErr);
    chain_->SetBranchAddress("vtx_yErr",    &vtx_yErr);
    chain_->SetBranchAddress("vtx_zErr",    &vtx_zErr);
    chain_->SetBranchAddress("vtx_chi2",    &vtx_chi2);
    chain_->SetBranchAddress("vtx_nDOF",    &vtx_nDOF);
    chain_->SetBranchAddress("vtx_isValid", &vtx_isValid);


    chain_->SetBranchStatus("nPixelVtx", 1);
    chain_->SetBranchStatus("pixelVtx_x", 1);
    chain_->SetBranchStatus("pixelVtx_y", 1);
    chain_->SetBranchStatus("pixelVtx_z", 1);
    chain_->SetBranchStatus("pixelVtx_xErr", 1);
    chain_->SetBranchStatus("pixelVtx_yErr", 1);
    chain_->SetBranchStatus("pixelVtx_zErr", 1);
    chain_->SetBranchStatus("pixelVtx_chi2", 1);
    chain_->SetBranchStatus("pixelVtx_nDOF", 1);
    chain_->SetBranchStatus("pixelVtx_isValid", 1);

    chain_->SetBranchAddress("nPixelVtx",     &nPixelVtx);
    chain_->SetBranchAddress("pixelVtx_x",    &pixelVtx_x);
    chain_->SetBranchAddress("pixelVtx_y",    &pixelVtx_y);
    chain_->SetBranchAddress("pixelVtx_z",    &pixelVtx_z);
    chain_->SetBranchAddress("pixelVtx_xErr",    &pixelVtx_xErr);
    chain_->SetBranchAddress("pixelVtx_yErr",    &pixelVtx_yErr);
    chain_->SetBranchAddress("pixelVtx_zErr",    &pixelVtx_zErr);
    chain_->SetBranchAddress("pixelVtx_chi2",    &pixelVtx_chi2);
    chain_->SetBranchAddress("pixelVtx_nDOF",    &pixelVtx_nDOF);
    chain_->SetBranchAddress("pixelVtx_isValid", &pixelVtx_isValid);


    chain_->SetBranchStatus("nPixelVtxMu", 1);
    chain_->SetBranchStatus("pixelVtxMu_x", 1);
    chain_->SetBranchStatus("pixelVtxMu_y", 1);
    chain_->SetBranchStatus("pixelVtxMu_z", 1);
    chain_->SetBranchStatus("pixelVtxMu_xErr", 1);
    chain_->SetBranchStatus("pixelVtxMu_yErr", 1);
    chain_->SetBranchStatus("pixelVtxMu_zErr", 1);
    chain_->SetBranchStatus("pixelVtxMu_chi2", 1);
    chain_->SetBranchStatus("pixelVtxMu_nDOF", 1);
    chain_->SetBranchStatus("pixelVtxMu_isValid", 1);

    chain_->SetBranchAddress("nPixelVtxMu",     &nPixelVtxMu);
    chain_->SetBranchAddress("pixelVtxMu_x",    &pixelVtxMu_x);
    chain_->SetBranchAddress("pixelVtxMu_y",    &pixelVtxMu_y);
    chain_->SetBranchAddress("pixelVtxMu_z",    &pixelVtxMu_z);
    chain_->SetBranchAddress("pixelVtxMu_xErr",    &pixelVtxMu_xErr);
    chain_->SetBranchAddress("pixelVtxMu_yErr",    &pixelVtxMu_yErr);
    chain_->SetBranchAddress("pixelVtxMu_zErr",    &pixelVtxMu_zErr);
    chain_->SetBranchAddress("pixelVtxMu_chi2",    &pixelVtxMu_chi2);
    chain_->SetBranchAddress("pixelVtxMu_nDOF",    &pixelVtxMu_nDOF);
    chain_->SetBranchAddress("pixelVtxMu_isValid", &pixelVtxMu_isValid);
  }

  void TurnOnBranches_Muon()
  {
    chain_->SetBranchStatus("nMuon", 1);
    chain_->SetBranchStatus("muon_pt", 1);
    chain_->SetBranchStatus("muon_eta", 1);
    chain_->SetBranchStatus("muon_phi", 1);
    // chain_->SetBranchStatus("muon_px", 1);
    // chain_->SetBranchStatus("muon_py", 1);
    // chain_->SetBranchStatus("muon_pz", 1);
    chain_->SetBranchStatus("muon_charge", 1);

    chain_->SetBranchStatus("muon_nPixelHit", 1);
    chain_->SetBranchStatus("muon_nStripHit", 1);
    chain_->SetBranchStatus("muon_nTrackerLayer", 1);
    chain_->SetBranchStatus("muon_nMuonHit", 1);
    chain_->SetBranchStatus("muon_nMatchedStation", 1);
    chain_->SetBranchStatus("muon_normChi2", 1);
    chain_->SetBranchStatus("muon_dxy", 1);
    chain_->SetBranchStatus("muon_dz", 1);
    chain_->SetBranchStatus("muon_trkIso", 1);
    chain_->SetBranchStatus("muon_vtxIndex", 1);

    chain_->SetBranchAddress("nMuon",       &nMuon);
    chain_->SetBranchAddress("muon_pt",     &muon_pt);
    chain_->SetBranchAddress("muon_eta",    &muon_eta);
    chain_->SetBranchAddress("muon_phi",    &muon_phi);
    // chain_->SetBranchAddress("muon_px",  &muon_px);
    // chain_->SetBranchAddress("muon_py",  &muon_py);
    // chain_->SetBranchAddress("muon_pz",  &muon_pz);
    chain_->SetBranchAddress("muon_charge", &muon_charge);

    chain_->SetBranchAddress("muon_nPixelHit",       &muon_nPixelHit);
    chain_->SetBranchAddress("muon_nStripHit",       &muon_nStripHit);
    chain_->SetBranchAddress("muon_nTrackerLayer",   &muon_nTrackerLayer);
    chain_->SetBranchAddress("muon_nMuonHit",        &muon_nMuonHit);
    chain_->SetBranchAddress("muon_nMatchedStation", &muon_nMatchedStation);
    chain_->SetBranchAddress("muon_normChi2",        &muon_normChi2);
    chain_->SetBranchAddress("muon_dxy",             &muon_dxy);
    chain_->SetBranchAddress("muon_dz",              &muon_dz);
    chain_->SetBranchAddress("muon_trkIso",          &muon_trkIso);
    chain_->SetBranchAddress("muon_vtxIndex",        &muon_vtxIndex);
  }

  void TurnOnBranches_GenParticle()
  {
    chain_->SetBranchStatus("nGenParticle", 1);
    chain_->SetBranchAddress("nGenParticle", &nGenParticle);

    chain_->SetBranchStatus("genParticle_ID", 1);
    chain_->SetBranchAddress("genParticle_ID", &genParticle_ID);

    chain_->SetBranchStatus("genParticle_status", 1);
    chain_->SetBranchAddress("genParticle_status", &genParticle_status);

    chain_->SetBranchStatus("genParticle_mother", 1);
    chain_->SetBranchAddress("genParticle_mother", &genParticle_mother);

    chain_->SetBranchStatus("genParticle_pt", 1);
    chain_->SetBranchAddress("genParticle_pt", &genParticle_pt);

    chain_->SetBranchStatus("genParticle_eta", 1);
    chain_->SetBranchAddress("genParticle_eta", &genParticle_eta);

    chain_->SetBranchStatus("genParticle_phi", 1);
    chain_->SetBranchAddress("genParticle_phi", &genParticle_phi);

    chain_->SetBranchStatus("genParticle_px", 1);
    chain_->SetBranchAddress("genParticle_px", &genParticle_px);

    chain_->SetBranchStatus("genParticle_py", 1);
    chain_->SetBranchAddress("genParticle_py", &genParticle_py);

    chain_->SetBranchStatus("genParticle_pz", 1);
    chain_->SetBranchAddress("genParticle_pz", &genParticle_pz);

    chain_->SetBranchStatus("genParticle_energy", 1);
    chain_->SetBranchAddress("genParticle_energy", &genParticle_energy);

    chain_->SetBranchStatus("genParticle_charge", 1);
    chain_->SetBranchAddress("genParticle_charge", &genParticle_charge);

    chain_->SetBranchStatus("genParticle_isPrompt", 1);
    chain_->SetBranchAddress("genParticle_isPrompt", &genParticle_isPrompt);

    chain_->SetBranchStatus("genParticle_isPromptFinalState", 1);
    chain_->SetBranchAddress("genParticle_isPromptFinalState", &genParticle_isPromptFinalState);

    chain_->SetBranchStatus("genParticle_isTauDecayProduct", 1);
    chain_->SetBranchAddress("genParticle_isTauDecayProduct", &genParticle_isTauDecayProduct);

    chain_->SetBranchStatus("genParticle_isPromptTauDecayProduct", 1);
    chain_->SetBranchAddress("genParticle_isPromptTauDecayProduct", &genParticle_isPromptTauDecayProduct);

    chain_->SetBranchStatus("genParticle_isDirectPromptTauDecayProductFinalState", 1);
    chain_->SetBranchAddress("genParticle_isDirectPromptTauDecayProductFinalState", &genParticle_isDirectPromptTauDecayProductFinalState);

    chain_->SetBranchStatus("genParticle_isHardProcess", 1);
    chain_->SetBranchAddress("genParticle_isHardProcess", &genParticle_isHardProcess);

    chain_->SetBranchStatus("genParticle_isLastCopy", 1);
    chain_->SetBranchAddress("genParticle_isLastCopy", &genParticle_isLastCopy);

    chain_->SetBranchStatus("genParticle_isLastCopyBeforeFSR", 1);
    chain_->SetBranchAddress("genParticle_isLastCopyBeforeFSR", &genParticle_isLastCopyBeforeFSR);

    chain_->SetBranchStatus("genParticle_isPromptDecayed", 1);
    chain_->SetBranchAddress("genParticle_isPromptDecayed", &genParticle_isPromptDecayed);

    chain_->SetBranchStatus("genParticle_isDecayedLeptonHadron", 1);
    chain_->SetBranchAddress("genParticle_isDecayedLeptonHadron", &genParticle_isDecayedLeptonHadron);

    chain_->SetBranchStatus("genParticle_fromHardProcessBeforeFSR", 1);
    chain_->SetBranchAddress("genParticle_fromHardProcessBeforeFSR", &genParticle_fromHardProcessBeforeFSR);

    chain_->SetBranchStatus("genParticle_fromHardProcessDecayed", 1);
    chain_->SetBranchAddress("genParticle_fromHardProcessDecayed", &genParticle_fromHardProcessDecayed);

    chain_->SetBranchStatus("genParticle_fromHardProcessFinalState", 1);
    chain_->SetBranchAddress("genParticle_fromHardProcessFinalState", &genParticle_fromHardProcessFinalState);

    chain_->SetBranchStatus("genParticle_isMostlyLikePythia6Status3", 1);
    chain_->SetBranchAddress("genParticle_isMostlyLikePythia6Status3", &genParticle_isMostlyLikePythia6Status3);
  }

  void TurnOnBranches_OffMuon()
  {
    chain_->SetBranchStatus("nOffMuon", 1);
    chain_->SetBranchAddress("nOffMuon", &nOffMuon);

    chain_->SetBranchStatus("offMuon_pt", 1);
    chain_->SetBranchAddress("offMuon_pt", &offMuon_pt);

    chain_->SetBranchStatus("offMuon_eta", 1);
    chain_->SetBranchAddress("offMuon_eta", &offMuon_eta);

    chain_->SetBranchStatus("offMuon_phi", 1);
    chain_->SetBranchAddress("offMuon_phi", &offMuon_phi);

    chain_->SetBranchStatus("offMuon_px", 1);
    chain_->SetBranchAddress("offMuon_px", &offMuon_px);

    chain_->SetBranchStatus("offMuon_py", 1);
    chain_->SetBranchAddress("offMuon_py", &offMuon_py);

    chain_->SetBranchStatus("offMuon_pz", 1);
    chain_->SetBranchAddress("offMuon_pz", &offMuon_pz);

    chain_->SetBranchStatus("offMuon_dB", 1);
    chain_->SetBranchAddress("offMuon_dB", &offMuon_dB);

    chain_->SetBranchStatus("offMuon_charge", 1);
    chain_->SetBranchAddress("offMuon_charge", &offMuon_charge);

    chain_->SetBranchStatus("offMuon_isGLB", 1);
    chain_->SetBranchAddress("offMuon_isGLB", &offMuon_isGLB);

    chain_->SetBranchStatus("offMuon_isSTA", 1);
    chain_->SetBranchAddress("offMuon_isSTA", &offMuon_isSTA);

    chain_->SetBranchStatus("offMuon_isTRK", 1);
    chain_->SetBranchAddress("offMuon_isTRK", &offMuon_isTRK);

    chain_->SetBranchStatus("offMuon_isPF", 1);
    chain_->SetBranchAddress("offMuon_isPF", &offMuon_isPF);

    chain_->SetBranchStatus("offMuon_isTight", 1);
    chain_->SetBranchAddress("offMuon_isTight", &offMuon_isTight);

    chain_->SetBranchStatus("offMuon_isMedium", 1);
    chain_->SetBranchAddress("offMuon_isMedium", &offMuon_isMedium);

    chain_->SetBranchStatus("offMuon_isLoose", 1);
    chain_->SetBranchAddress("offMuon_isLoose", &offMuon_isLoose);

    chain_->SetBranchStatus("offMuon_isHighPt", 1);
    chain_->SetBranchAddress("offMuon_isHighPt", &offMuon_isHighPt);

    chain_->SetBranchStatus("offMuon_isSoft", 1);
    chain_->SetBranchAddress("offMuon_isSoft", &offMuon_isSoft);

    chain_->SetBranchStatus("offMuon_iso03_sumPt", 1);
    chain_->SetBranchAddress("offMuon_iso03_sumPt", &offMuon_iso03_sumPt);

    chain_->SetBranchStatus("offMuon_iso03_hadEt", 1);
    chain_->SetBranchAddress("offMuon_iso03_hadEt", &offMuon_iso03_hadEt);

    chain_->SetBranchStatus("offMuon_iso03_emEt", 1);
    chain_->SetBranchAddress("offMuon_iso03_emEt", &offMuon_iso03_emEt);

    chain_->SetBranchStatus("offMuon_PFIso03_charged", 1);
    chain_->SetBranchAddress("offMuon_PFIso03_charged", &offMuon_PFIso03_charged);

    chain_->SetBranchStatus("offMuon_PFIso03_neutral", 1);
    chain_->SetBranchAddress("offMuon_PFIso03_neutral", &offMuon_PFIso03_neutral);

    chain_->SetBranchStatus("offMuon_PFIso03_photon", 1);
    chain_->SetBranchAddress("offMuon_PFIso03_photon", &offMuon_PFIso03_photon);

    chain_->SetBranchStatus("offMuon_PFIso03_sumPU", 1);
    chain_->SetBranchAddress("offMuon_PFIso03_sumPU", &offMuon_PFIso03_sumPU);

    chain_->SetBranchStatus("offMuon_PFIso04_charged", 1);
    chain_->SetBranchAddress("offMuon_PFIso04_charged", &offMuon_PFIso04_charged);

    chain_->SetBranchStatus("offMuon_PFIso04_neutral", 1);
    chain_->SetBranchAddress("offMuon_PFIso04_neutral", &offMuon_PFIso04_neutral);

    chain_->SetBranchStatus("offMuon_PFIso04_photon", 1);
    chain_->SetBranchAddress("offMuon_PFIso04_photon", &offMuon_PFIso04_photon);

    chain_->SetBranchStatus("offMuon_PFIso04_sumPU", 1);
    chain_->SetBranchAddress("offMuon_PFIso04_sumPU", &offMuon_PFIso04_sumPU);

    chain_->SetBranchStatus("offMuon_PFCluster03_ECAL", 1);
    chain_->SetBranchAddress("offMuon_PFCluster03_ECAL", &offMuon_PFCluster03_ECAL);

    chain_->SetBranchStatus("offMuon_PFCluster03_HCAL", 1);
    chain_->SetBranchAddress("offMuon_PFCluster03_HCAL", &offMuon_PFCluster03_HCAL);

    chain_->SetBranchStatus("offMuon_PFCluster04_ECAL", 1);
    chain_->SetBranchAddress("offMuon_PFCluster04_ECAL", &offMuon_PFCluster04_ECAL);

    chain_->SetBranchStatus("offMuon_PFCluster04_HCAL", 1);
    chain_->SetBranchAddress("offMuon_PFCluster04_HCAL", &offMuon_PFCluster04_HCAL);

    chain_->SetBranchStatus("offMuon_normChi2_global", 1);
    chain_->SetBranchAddress("offMuon_normChi2_global", &offMuon_normChi2_global);

    chain_->SetBranchStatus("offMuon_nTrackerHit_global", 1);
    chain_->SetBranchAddress("offMuon_nTrackerHit_global", &offMuon_nTrackerHit_global);

    chain_->SetBranchStatus("offMuon_nTrackerLayer_global", 1);
    chain_->SetBranchAddress("offMuon_nTrackerLayer_global", &offMuon_nTrackerLayer_global);

    chain_->SetBranchStatus("offMuon_nPixelHit_global", 1);
    chain_->SetBranchAddress("offMuon_nPixelHit_global", &offMuon_nPixelHit_global);

    chain_->SetBranchStatus("offMuon_nMuonHit_global", 1);
    chain_->SetBranchAddress("offMuon_nMuonHit_global", &offMuon_nMuonHit_global);

    chain_->SetBranchStatus("offMuon_normChi2_inner", 1);
    chain_->SetBranchAddress("offMuon_normChi2_inner", &offMuon_normChi2_inner);

    chain_->SetBranchStatus("offMuon_nTrackerHit_inner", 1);
    chain_->SetBranchAddress("offMuon_nTrackerHit_inner", &offMuon_nTrackerHit_inner);

    chain_->SetBranchStatus("offMuon_nTrackerLayer_inner", 1);
    chain_->SetBranchAddress("offMuon_nTrackerLayer_inner", &offMuon_nTrackerLayer_inner);

    chain_->SetBranchStatus("offMuon_nPixelHit_inner", 1);
    chain_->SetBranchAddress("offMuon_nPixelHit_inner", &offMuon_nPixelHit_inner);

    chain_->SetBranchStatus("offMuon_pt_tuneP", 1);
    chain_->SetBranchAddress("offMuon_pt_tuneP", &offMuon_pt_tuneP);

    chain_->SetBranchStatus("offMuon_ptError_tuneP", 1);
    chain_->SetBranchAddress("offMuon_ptError_tuneP", &offMuon_ptError_tuneP);

    chain_->SetBranchStatus("offMuon_dxyVTX_best", 1);
    chain_->SetBranchAddress("offMuon_dxyVTX_best", &offMuon_dxyVTX_best);

    chain_->SetBranchStatus("offMuon_dzVTX_best", 1);
    chain_->SetBranchAddress("offMuon_dzVTX_best", &offMuon_dzVTX_best);

    chain_->SetBranchStatus("offMuon_nMatchedStation", 1);
    chain_->SetBranchAddress("offMuon_nMatchedStation", &offMuon_nMatchedStation);

    chain_->SetBranchStatus("offMuon_nMatchedRPCLayer", 1);
    chain_->SetBranchAddress("offMuon_nMatchedRPCLayer", &offMuon_nMatchedRPCLayer);

    chain_->SetBranchStatus("offMuon_stationMask", 1);
    chain_->SetBranchAddress("offMuon_stationMask", &offMuon_stationMask);
  }


private:
  
};

}; // -- end of namespace DYTool