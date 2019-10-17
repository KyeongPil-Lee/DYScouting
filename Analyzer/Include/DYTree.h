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

  // -- vertex information (@ HLT)
  Int_t nVtx;
  Double_t vtx_x[ArrSize];
  Double_t vtx_y[ArrSize];
  Double_t vtx_z[ArrSize];
  Double_t vtx_chi2[ArrSize];

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
  Int_t muon_nTrackerLayer[ArrSize];
  Int_t muon_nMuonHit[ArrSize];
  Int_t muon_nMatchedStation[ArrSize];
  Double_t muon_normChi2[ArrSize];
  Double_t muon_dxy[ArrSize];
  Double_t muon_dz[ArrSize];
  Double_t muon_trkIso[ArrSize];

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

    chain_->SetBranchAddress("runNum",           &runNum);
    chain_->SetBranchAddress("lumiBlockNum",     &lumiBlockNum);
    chain_->SetBranchAddress("eventNum",         &eventNum);
    chain_->SetBranchAddress("genWeight",        &genWeight);
    chain_->SetBranchAddress("truePU",           &truePU);
    chain_->SetBranchAddress("vec_firedTrigger", &vec_firedTrigger);
  }

  void TurnOnBranches_VtxInfo()
  {
    // -- vertex information (@ HLT)
    chain_->SetBranchStatus("nVtx", 1);
    chain_->SetBranchStatus("vtx_x", 1);
    chain_->SetBranchStatus("vtx_y", 1);
    chain_->SetBranchStatus("vtx_z", 1);
    chain_->SetBranchStatus("vtx_chi2", 1);

    chain_->SetBranchAddress("nVtx",     &nVtx);
    chain_->SetBranchAddress("vtx_x",    &vtx_x);
    chain_->SetBranchAddress("vtx_y",    &vtx_y);
    chain_->SetBranchAddress("vtx_z",    &vtx_z);
    chain_->SetBranchAddress("vtx_chi2", &vtx_chi2);
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
    chain_->SetBranchStatus("muon_nTrackerLayer", 1);
    chain_->SetBranchStatus("muon_nMuonHit", 1);
    chain_->SetBranchStatus("muon_nMatchedStation", 1);
    chain_->SetBranchStatus("muon_normChi2", 1);
    chain_->SetBranchStatus("muon_dxy", 1);
    chain_->SetBranchStatus("muon_dz", 1);
    chain_->SetBranchStatus("muon_trkIso", 1);

    chain_->SetBranchAddress("nMuon",       &nMuon);
    chain_->SetBranchAddress("muon_pt",     &muon_pt);
    chain_->SetBranchAddress("muon_eta",    &muon_eta);
    chain_->SetBranchAddress("muon_phi",    &muon_phi);
    // chain_->SetBranchAddress("muon_px",  &muon_px);
    // chain_->SetBranchAddress("muon_py",  &muon_py);
    // chain_->SetBranchAddress("muon_pz",  &muon_pz);
    chain_->SetBranchAddress("muon_charge", &muon_charge);

    chain_->SetBranchAddress("muon_nPixelHit",       &muon_nPixelHit);
    chain_->SetBranchAddress("muon_nTrackerLayer",   &muon_nTrackerLayer);
    chain_->SetBranchAddress("muon_nMuonHit",        &muon_nMuonHit);
    chain_->SetBranchAddress("muon_nMatchedStation", &muon_nMatchedStation);
    chain_->SetBranchAddress("muon_normChi2",        &muon_normChi2);
    chain_->SetBranchAddress("muon_dxy",             &muon_dxy);
    chain_->SetBranchAddress("muon_dz",              &muon_dz);
    chain_->SetBranchAddress("muon_trkIso",          &muon_trkIso);
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


private:
  
};

}; // -- end of namespace DYTool