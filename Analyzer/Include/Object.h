#pragma once
#include <TLorentzVector.h>

//customized header files
#include <Include/DYTree.h>


namespace DYTool
{

const Double_t M_mu  = 0.1056583715;  // -- GeV -- //
const Double_t M_el  = 0.000510998;  // -- GeV -- //
const Double_t M_tau = 1.77682;      // -- GeV -- //


class Object
{
public:
  Double_t pt;
  Double_t eta;
  Double_t phi;
  TLorentzVector vecP;

  Object() {}
};


class GenParticle : public Object
{
public:
  Double_t px;
  Double_t py;
  Double_t pz;
  Double_t energy;
  Double_t mother;
  Int_t ID;
  Double_t charge;
  Double_t mass;
  Int_t status;

  Int_t isPrompt;
  Int_t isPromptFinalState;
  Int_t isTauDecayProduct;
  Int_t isPromptTauDecayProduct;
  Int_t isDirectPromptTauDecayProductFinalState;
  Int_t isHardProcess;
  Int_t isLastCopy;
  Int_t isLastCopyBeforeFSR;
  Int_t isPromptDecayed;
  Int_t isDecayedLeptonHadron;
  Int_t fromHardProcessBeforeFSR;
  Int_t fromHardProcessDecayed;
  Int_t fromHardProcessFinalState;

  GenParticle()
  {
    Init();
  }

  GenParticle( DYTool::DYTree *ntuple, Int_t index ): GenParticle()
  {
    FillFromNtuple( ntuple, index );
  }

  void FillFromNtuple(DYTool::DYTree *ntuple, Int_t index)
  {
    pt      = ntuple->genParticle_pt[index];
    eta     = ntuple->genParticle_eta[index];
    phi     = ntuple->genParticle_phi[index];
    px      = ntuple->genParticle_px[index];
    py      = ntuple->genParticle_py[index];
    pz      = ntuple->genParticle_pz[index];
    mother  = ntuple->genParticle_mother[index];
    ID      = ntuple->genParticle_ID[index];
    charge  = ntuple->genParticle_charge[index];
    status  = ntuple->genParticle_status[index];

    if     ( std::abs(ID) == 11 ) mass = DYTool::M_el;
    else if( std::abs(ID) == 13 ) mass = DYTool::M_mu;
    else if( std::abs(ID) == 15 ) mass = DYTool::M_tau;
    else if( std::abs(ID) == 12 || std::abs(ID) == 14 || std::abs(ID) == 16 ) mass = 0; // -- neutrino
    else
    {
      printf("[DYTool::GenParticle] ID = %d is not recognized... mass = 0 for 4-momentum\n", ID);
      mass = 0;
    }

    energy = sqrt(px*px + py*py + pz*pz + mass*mass);
    vecP.SetPxPyPzE(px, py, pz, energy);

    isPrompt                                = ntuple->genParticle_isPrompt[index];
    isPromptFinalState                      = ntuple->genParticle_isPromptFinalState[index];
    isTauDecayProduct                       = ntuple->genParticle_isTauDecayProduct[index];
    isPromptTauDecayProduct                 = ntuple->genParticle_isPromptTauDecayProduct[index];
    isDirectPromptTauDecayProductFinalState = ntuple->genParticle_isDirectPromptTauDecayProductFinalState[index];
    isHardProcess                           = ntuple->genParticle_isHardProcess[index];
    isLastCopy                              = ntuple->genParticle_isLastCopy[index];
    isLastCopyBeforeFSR                     = ntuple->genParticle_isLastCopyBeforeFSR[index];
    isPromptDecayed                         = ntuple->genParticle_isPromptDecayed[index];
    isDecayedLeptonHadron                   = ntuple->genParticle_isDecayedLeptonHadron[index];
    fromHardProcessBeforeFSR                = ntuple->genParticle_fromHardProcessBeforeFSR[index];
    fromHardProcessDecayed                  = ntuple->genParticle_fromHardProcessDecayed[index];
    fromHardProcessFinalState               = ntuple->genParticle_fromHardProcessFinalState[index];
  }

private:
  void Init()
  {
    px = -999;
    py = -999;
    pz = -999;
    energy = -999;
    mother = -999;
    ID = -999;
    charge = -999;
    mass = -999;
    status = -999;

    isPrompt = -999;
    isPromptFinalState = -999;
    isTauDecayProduct = -999;
    isPromptTauDecayProduct = -999;
    isDirectPromptTauDecayProductFinalState = -999;
    isHardProcess = -999;
    isLastCopy = -999;
    isLastCopyBeforeFSR = -999;
    isPromptDecayed = -999;
    isDecayedLeptonHadron = -999;
    fromHardProcessBeforeFSR = -999;
    fromHardProcessDecayed = -999;
    fromHardProcessFinalState = -999;
  }
};


class GenPair: public Object
{
public:
  GenParticle first_;
  GenParticle second_;

  Double_t mass;
  Double_t rap;
  Double_t absRap;

  GenPair() {}

  GenPair(GenParticle genParticle1, GenParticle genParticle2)
  {
    if( genParticle1.pt > genParticle2.pt )
    {
      first_ = genParticle1;
      second_ = genParticle2;
    }
    else
    {
      first_ = genParticle2;
      second_ = genParticle1;
    }

    Assign();
  }

private:
  void Assign()
  {
    vecP = first_.vecP + second_.vecP;

    pt = vecP.Pt();
    eta = vecP.Eta();
    phi = vecP.Phi();

    mass = vecP.M();
    rap = vecP.Rapidity();
    absRap = fabs(rap);
  }
};


// -- Scouting muon
class Muon: public Object
{
public:
  Double_t charge;

  Int_t nPixelHit;
  Int_t nTrackerLayer;
  Int_t nMuonHit;
  Int_t nMatchedStation;
  Double_t normChi2;
  Double_t dxy;
  Double_t dz;
  Double_t trkIso;

  Double_t relTrkIso;

  Muon()
  {
    Init();
  }

  Muon(DYTool::DYTree *ntuple, Int_t index): Muon()
  {
    FillFromNtuple(ntuple, index);
  }

  void FillFromNtuple(DYTool::DYTree *ntuple, Int_t index)
  {
    pt     = ntuple->muon_pt[index];
    eta    = ntuple->muon_eta[index];
    phi    = ntuple->muon_phi[index];
    vecP.SetPtEtaPhiM(pt, eta, phi, DYTool::M_mu);

    charge = ntuple->muon_charge[index];

    nPixelHit       = ntuple->muon_nPixelHit[index];
    nTrackerLayer   = ntuple->muon_nTrackerLayer[index];
    nMuonHit        = ntuple->muon_nMuonHit[index];
    nMatchedStation = ntuple->muon_nMatchedStation[index];
    normChi2        = ntuple->muon_normChi2[index];
    dxy             = ntuple->muon_dxy[index];
    dz              = ntuple->muon_dz[index];
    trkIso          = ntuple->muon_trkIso[index];

    relTrkIso       = trkIso / pt;
  }

private:
  void Init()
  {
    pt = -999;
    eta = -999;
    phi = -999;
    charge = -999;

    nPixelHit = -999;
    nTrackerLayer = -999;
    nMuonHit = -999;
    nMatchedStation = -999;
    normChi2 = -999;
    dxy = -999;
    dz = -999;
    trkIso = -999;

    relTrkIso = -999;
  }
};

class MuPair: public Object
{
public:
  Muon first_;
  Muon second_;

  Double_t mass;
  Double_t rap;
  Double_t absRap;

  Bool_t isOS;

  MuPair() { Init(); }

  MuPair(Muon muon1, Muon muon2): MuPair()
  {
    if( muon1.pt > muon2.pt )
    {
      first_ = muon1;
      second_ = muon2;
    }
    else
    {
      first_ = muon2;
      second_ = muon1;
    }

    Assign();
  }

private:
  void Assign()
  {
    vecP = first_.vecP + second_.vecP;

    pt = vecP.Pt();
    eta = vecP.Eta();
    phi = vecP.Phi();

    mass = vecP.M();
    rap = vecP.Rapidity();
    absRap = fabs(rap);

    isOS = first_.charge != second_.charge ? kTRUE : kFALSE;
  }

  void Init()
  {
    mass = -999;
    rap = -999;
    absRap = -999;

    isOS = kFALSE;
  }
};


}; // -- end of namespace DYTool