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

  Double_t dEta;
  Double_t dPhi;
  Double_t angle3D;

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

    dEta = fabs(first_.eta - second_.eta);
    dPhi = first_.vecP.DeltaPhi( second_.vecP );
    angle3D = first_.vecP.Angle( second_.vecP.Vect() );
  }
};

class HLTObj: public Object
{
public:
  TString filterName;

  HLTObj() { Init(); };

  HLTObj(DYTool::DYTree *ntuple, Int_t index): HLTObj()
  {
    FillFromNtuple(ntuple, index);
  }

  void FillFromNtuple(DYTool::DYTree *ntuple, Int_t index)
  {
    pt  = ntuple->vec_HLTObj_pt->at(index);
    eta = ntuple->vec_HLTObj_eta->at(index);
    phi = ntuple->vec_HLTObj_phi->at(index);
    vecP.SetPtEtaPhiM(pt, eta, phi, DYTool::M_mu); // -- assumption: HLT "muon" object

    filterName = ntuple->vec_filterName->at(index);
  }

private:
  void Init()
  {
    pt = -999;
    eta = -999;
    phi = -999;
  }
};

class L1Muon: public Object
{
public:
  Double_t charge;
  Double_t quality;

  L1Muon() { Init(); };

  L1Muon(DYTool::DYTree *ntuple, Int_t index): L1Muon()
  {
    FillFromNtuple(ntuple, index);
  }

  void FillFromNtuple(DYTool::DYTree *ntuple, Int_t index)
  {
    pt       = ntuple->L1Muon_pt[index];
    eta      = ntuple->L1Muon_eta[index];
    phi      = ntuple->L1Muon_phi[index];
    charge   = ntuple->L1Muon_charge[index];
    quality  = ntuple->L1Muon_quality[index];

    vecP.SetPtEtaPhiM(pt, eta, phi, DYTool::M_mu); // -- assumption: L1 "muon" object
  }

private:
  void Init()
  {
    pt = -999;
    eta = -999;
    phi = -999;
    charge = -999;
    quality = -999;
  }
};


// -- Scouting muon
class Muon: public Object
{
public:
  Double_t charge;

  Int_t nPixelHit;
  Int_t nStripHit;
  Int_t nTrackerLayer;
  Int_t nMuonHit;
  Int_t nMatchedStation;
  Double_t normChi2;
  Double_t dxy;
  Double_t dz;
  Double_t trkIso;

  Double_t relTrkIso;

  vector< Int_t > vec_vtxIndex;

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
    nStripHit       = ntuple->muon_nStripHit[index];
    nTrackerLayer   = ntuple->muon_nTrackerLayer[index];
    nMuonHit        = ntuple->muon_nMuonHit[index];
    nMatchedStation = ntuple->muon_nMatchedStation[index];
    normChi2        = ntuple->muon_normChi2[index];
    dxy             = ntuple->muon_dxy[index];
    dz              = ntuple->muon_dz[index];
    trkIso          = ntuple->muon_trkIso[index];
    vec_vtxIndex    = ntuple->muon_vtxIndex->at(index);

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

// -- Dimuon vertex reconstructed @ HLT
class DimuonVertex
{
public:
  Int_t index; // -- to be used for linking with the scouting muons

  Double_t x;
  Double_t y;
  Double_t z;
  Double_t xErr;
  Double_t yErr;
  Double_t zErr;
  Double_t chi2;
  Int_t nDOF;
  Int_t isValid;

  Double_t normChi2;

  Double_t r; // -- sqrt(x^x + y^y)
  Double_t R; // -- sqrt(x^x + y^y + z*z)

  DimuonVertex()
  {
    Init();
  }

  DimuonVertex(DYTool::DYTree *ntuple, Int_t index): DimuonVertex()
  {
    FillFromNtuple(ntuple, index);
  }

  void FillFromNtuple(DYTool::DYTree *ntuple, Int_t idx)
  {
    index = idx;

    x = ntuple->vtx_x[idx];
    y = ntuple->vtx_y[idx];
    z = ntuple->vtx_z[idx];

    xErr = ntuple->vtx_xErr[idx];
    yErr = ntuple->vtx_yErr[idx];
    zErr = ntuple->vtx_zErr[idx];

    chi2 = ntuple->vtx_chi2[idx];
    nDOF = ntuple->vtx_nDOF[idx];

    isValid = ntuple->vtx_isValid[idx];

    if( nDOF != 0 ) normChi2 = chi2 / nDOF;

    r = sqrt(x*x + y*y);
    R = sqrt(x*x + y*y + z*z);
  }

private:
  void Init()
  {
    Int_t index = -999;

    Double_t x = -999;
    Double_t y = -999;
    Double_t z = -999;
    Double_t xErr = -999;
    Double_t yErr = -999;
    Double_t zErr = -999;
    Double_t chi2 = -999;
    Double_t nDOF = -999;
    Int_t isValid = -999;

    r = -999;
    R = -999;
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
  Double_t signSum;

  Double_t dEta;
  Double_t dPhi;
  Double_t dR;
  Double_t angle3D;

  Double_t dEtaCM;
  Double_t dPhiCM;
  Double_t angle3DCM;

  // -- assigned when CheckVertex() is colled
  Bool_t hasVertex;
  DimuonVertex vertex;
  Double_t normVtxChi2;

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

  Bool_t IsDYCandidate(DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( first_.pt > 5  && fabs(first_.eta) < 2.4 &&
        second_.pt > 5 && fabs(second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( first_.nPixelHit > 0  && first_.nTrackerLayer > 5  && first_.normChi2 < 10  && first_.relTrkIso < 0.15 &&
        second_.nPixelHit > 0 && second_.nTrackerLayer > 5 && second_.normChi2 < 10 && second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    CheckVertex(ntuple);

    if( isWithinAcc && isGoodMuon ) flag = kTRUE;

    return flag;
  }

  Bool_t IsDYCandidate_BDTInput(DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( first_.pt > 5  && fabs(first_.eta) < 2.4 &&
        second_.pt > 5 && fabs(second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( first_.nPixelHit > 0  && first_.nTrackerLayer > 5  && first_.normChi2 < 10  && first_.relTrkIso < 0.15 &&
        second_.nPixelHit > 0 && second_.nTrackerLayer > 5 && second_.normChi2 < 10 && second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    CheckVertex(ntuple);

    if( isWithinAcc && isGoodMuon && hasVertex && isOS && mass > 10.0 ) flag = kTRUE;

    return flag;
  }

  Bool_t IsDYCandidate_BDTInput_noOS(DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( first_.pt > 5  && fabs(first_.eta) < 2.4 &&
        second_.pt > 5 && fabs(second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( first_.nPixelHit > 0  && first_.nTrackerLayer > 5  && first_.normChi2 < 10  && first_.relTrkIso < 0.15 &&
        second_.nPixelHit > 0 && second_.nTrackerLayer > 5 && second_.normChi2 < 10 && second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    CheckVertex(ntuple);

    if( isWithinAcc && isGoodMuon && hasVertex && mass > 10.0 ) flag = kTRUE;

    return flag;
  }

  // -- 26 / 10 GeV
  Bool_t IsDYCandidate_BDTInput_noOS_HigherPt(DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( first_.pt > 26  && fabs(first_.eta) < 2.4 &&
        second_.pt > 10 && fabs(second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( first_.nPixelHit > 0  && first_.nTrackerLayer > 5  && first_.normChi2 < 10  && first_.relTrkIso < 0.15 &&
        second_.nPixelHit > 0 && second_.nTrackerLayer > 5 && second_.normChi2 < 10 && second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    CheckVertex(ntuple);

    if( isWithinAcc && isGoodMuon && hasVertex && mass > 10.0 ) flag = kTRUE;

    return flag;
  }

  Bool_t IsDYCandidate_BDTInput_noOS_noMassCut(DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( first_.pt > 5  && fabs(first_.eta) < 2.4 &&
        second_.pt > 5 && fabs(second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( first_.nPixelHit > 0  && first_.nTrackerLayer > 5  && first_.normChi2 < 10  && first_.relTrkIso < 0.15 &&
        second_.nPixelHit > 0 && second_.nTrackerLayer > 5 && second_.normChi2 < 10 && second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    CheckVertex(ntuple);

    if( isWithinAcc && isGoodMuon && hasVertex ) flag = kTRUE;

    return flag;
  }

  // -- w.r.t default BDTInput, no OS requirement & looser isolation condition
  Bool_t IsDYCandidate_BDTInput_Loose(DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( first_.pt > 5  && fabs(first_.eta) < 2.4 &&
        second_.pt > 5 && fabs(second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( first_.nPixelHit > 0  && first_.nTrackerLayer > 5  && first_.normChi2 < 10  && first_.relTrkIso < 0.20 &&
        second_.nPixelHit > 0 && second_.nTrackerLayer > 5 && second_.normChi2 < 10 && second_.relTrkIso < 0.20 )
      isGoodMuon = kTRUE;

    CheckVertex(ntuple);

    if( isWithinAcc && isGoodMuon && hasVertex && mass > 10.0 ) flag = kTRUE;

    return flag;
  }

  // -- w.r.t default BDTInput, no OS requirement & looser isolation condition
  Bool_t IsDYCandidate_BDTInput_AlmostNoCut(DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( first_.pt > 5  && fabs(first_.eta) < 2.4 &&
        second_.pt > 5 && fabs(second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    CheckVertex(ntuple);

    if( isWithinAcc && hasVertex && mass > 10.0 ) flag = kTRUE;

    return flag;
  }

  Bool_t IsDYCandidate_Tight(DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( first_.pt > 5  && fabs(first_.eta) < 2.4 &&
        second_.pt > 5 && fabs(second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( first_.nPixelHit > 0  && first_.nTrackerLayer > 5  && first_.normChi2 < 10  && first_.relTrkIso < 0.01 &&
        second_.nPixelHit > 0 && second_.nTrackerLayer > 5 && second_.normChi2 < 10 && second_.relTrkIso < 0.01 )
      isGoodMuon = kTRUE;

    CheckVertex(ntuple);

    if( isWithinAcc && isGoodMuon && hasVertex && normVtxChi2 < 5.0 && isOS ) flag = kTRUE;

    return flag;
  }

  void CheckVertex(DYTool::DYTree *ntuple)
  {
    Int_t nVtx = ntuple->nVtx;
    if( nVtx == 0 )
    {
      hasVertex = kFALSE;
      normVtxChi2 = -999;
      return;
    }

    for(Int_t i_vtx=0; i_vtx<nVtx; i_vtx++)
    {
      Bool_t mu1_associated = kFALSE;
      for(auto vtxIndex : first_.vec_vtxIndex)
      {
        if(i_vtx == vtxIndex)
        {
          mu1_associated = kTRUE;
          break;
        }
      }

      Bool_t mu2_associated = kFALSE;
      for(auto vtxIndex : second_.vec_vtxIndex)
      {
        if(i_vtx == vtxIndex)
        {
          mu2_associated = kTRUE;
          break;
        }
      }

      if( mu1_associated && mu2_associated )
      {
        hasVertex = kTRUE;
        vertex = DimuonVertex(ntuple, i_vtx);
        normVtxChi2 = vertex.normChi2;
        // cout << "(chi2, nDOF, normChi2, isValid) = " << vertex.chi2 << ", " << vertex.nDOF << ", " << vertex.normChi2 << ", " << vertex.isValid << endl;
        break;
      }
    }
  }

  Bool_t IsDiMuCandidate_EXO19018(DYTool::DYTree *ntuple)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( first_.pt  > 4 && first_.pt  > (mass/3.0) && fabs(first_.eta) < 1.9 &&
        second_.pt > 4 && second_.pt > (mass/4.0) && fabs(second_.eta) < 1.9 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( first_.nPixelHit > 0  && first_.nTrackerLayer > 5  && first_.normChi2 < 10  && first_.relTrkIso < 0.15 &&
        second_.nPixelHit > 0 && second_.nTrackerLayer > 5 && second_.normChi2 < 10 && second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    CheckVertex(ntuple);

    if( isWithinAcc && isGoodMuon && isOS ) flag = kTRUE;

    return flag;
  }

  // -- IsDYCandidate_BDTInput_noOS + remove vertex information
  Bool_t IsDYCandidate_BDTInput_noOS_noVTX(DYTool::DYTree *ntuple);

  // -- IsDYCandidate_BDTInput_noOS + tighter condition
  Bool_t IsDYCandidate_BDTInput_VeryTight(DYTool::DYTree *ntuple);

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
    signSum = first_.charge + second_.charge; // -- for TMVA (which can't recognize boolean as an input variable)

    dEta = fabs(first_.eta - second_.eta);
    dPhi = first_.vecP.DeltaPhi( second_.vecP );
    dR   = sqrt(dEta*dEta + dPhi*dPhi);
    angle3D = first_.vecP.Angle( second_.vecP.Vect() );

    // -- boost to CM frame
    TVector3 boostVector = vecP.BoostVector();
    TLorentzVector vecP_first_CM  = first_.vecP;
    vecP_first_CM.Boost( -boostVector );

    TLorentzVector vecP_second_CM = second_.vecP;
    vecP_second_CM.Boost( -boostVector );

    dEtaCM    = fabs( vecP_first_CM.Eta() - vecP_second_CM.Eta() );
    dPhiCM    = vecP_first_CM.DeltaPhi( vecP_second_CM );
    angle3DCM = vecP_first_CM.Angle( vecP_second_CM.Vect() );
  }

  void Init()
  {
    mass = -999;
    rap = -999;
    absRap = -999;
    signSum = -999;

    isOS = kFALSE;

    // -- assigned when CheckVertex() is called
    hasVertex = kFALSE;
    normVtxChi2 = 999;
  }
};

// -- offline muon
class OffMuon : public Object
{
public:
  Double_t px;
  Double_t py;
  Double_t pz;
  Double_t mass;
  Double_t energy;
  Double_t dB;
  Double_t charge;
  Int_t    isGLB;
  Int_t    isSTA;
  Int_t    isTRK;
  Int_t    isPF;
  Int_t    isTight;
  Int_t    isMedium;
  Int_t    isLoose;
  Int_t    isHighPt;
  Int_t    isSoft;
  Double_t iso03_sumPt;
  Double_t iso03_hadEt;
  Double_t iso03_emEt;
  Double_t PFIso03_charged;
  Double_t PFIso03_neutral;
  Double_t PFIso03_photon;
  Double_t PFIso03_sumPU;
  Double_t PFIso04_charged;
  Double_t PFIso04_neutral;
  Double_t PFIso04_photon;
  Double_t PFIso04_sumPU;
  Double_t PFCluster03_ECAL;
  Double_t PFCluster03_HCAL;
  Double_t PFCluster04_ECAL;
  Double_t PFCluster04_HCAL;
  Double_t normChi2_global;
  Int_t    nTrackerHit_global;
  Int_t    nTrackerLayer_global;
  Int_t    nPixelHit_global;
  Int_t    nMuonHit_global;
  Double_t normChi2_inner;
  Int_t    nTrackerHit_inner;
  Int_t    nTrackerLayer_inner;
  Int_t    nPixelHit_inner;
  Double_t pt_tuneP;
  Double_t ptError_tuneP;
  Double_t dxyVTX_best;
  Double_t dzVTX_best;
  Int_t    nMatchedStation;
  Int_t    nMatchedRPCLayer;
  Int_t    stationMask;

  Double_t relPFIso_dBeta;
  Double_t relTrkIso;

  Double_t pt_inner;

  Double_t propEta;
  Double_t propPhi;

  OffMuon()
  {
    Init();
  }

  OffMuon(DYTree* ntuple, Int_t index): OffMuon()
  {
    FillFromNtuple(ntuple, index);
  }

  void FillFromNtuple(DYTree* ntuple, Int_t index)
  {
    pt     = ntuple->offMuon_pt[index];
    eta    = ntuple->offMuon_eta[index];
    phi    = ntuple->offMuon_phi[index];
    charge = ntuple->offMuon_charge[index];

    px  = ntuple->offMuon_px[index];
    py  = ntuple->offMuon_py[index];
    pz  = ntuple->offMuon_pz[index];
    mass = DYTool::M_mu;
    energy = sqrt(px*px + py*py + pz*pz + mass*mass);
    vecP.SetPxPyPzE(px, py, pz, energy);

    isGLB = ntuple->offMuon_isGLB[index];
    isSTA = ntuple->offMuon_isSTA[index];
    isTRK = ntuple->offMuon_isTRK[index];
    isPF  = ntuple->offMuon_isPF[index];

    isTight  = ntuple->offMuon_isTight[index];
    isMedium = ntuple->offMuon_isMedium[index];
    isLoose  = ntuple->offMuon_isLoose[index];
    isHighPt = ntuple->offMuon_isHighPt[index];
    isSoft   = ntuple->offMuon_isSoft[index];

    iso03_sumPt = ntuple->offMuon_iso03_sumPt[index];
    iso03_hadEt = ntuple->offMuon_iso03_hadEt[index];
    iso03_emEt  = ntuple->offMuon_iso03_emEt[index];

    PFIso03_charged = ntuple->offMuon_PFIso03_charged[index];
    PFIso03_neutral = ntuple->offMuon_PFIso03_neutral[index];
    PFIso03_photon  = ntuple->offMuon_PFIso03_photon[index];
    PFIso03_sumPU   = ntuple->offMuon_PFIso03_sumPU[index];
    PFIso04_charged = ntuple->offMuon_PFIso04_charged[index];
    PFIso04_neutral = ntuple->offMuon_PFIso04_neutral[index];
    PFIso04_photon  = ntuple->offMuon_PFIso04_photon[index];
    PFIso04_sumPU   = ntuple->offMuon_PFIso04_sumPU[index];

    PFCluster03_ECAL = ntuple->offMuon_PFCluster03_ECAL[index];
    PFCluster03_HCAL = ntuple->offMuon_PFCluster03_HCAL[index];
    PFCluster04_ECAL = ntuple->offMuon_PFCluster04_ECAL[index];
    PFCluster04_HCAL = ntuple->offMuon_PFCluster04_HCAL[index];

    normChi2_global      = ntuple->offMuon_normChi2_global[index];
    nTrackerHit_global   = ntuple->offMuon_nTrackerHit_global[index];
    nTrackerLayer_global = ntuple->offMuon_nTrackerLayer_global[index];
    nPixelHit_global     = ntuple->offMuon_nPixelHit_global[index];
    nMuonHit_global      = ntuple->offMuon_nMuonHit_global[index];

    normChi2_inner      = ntuple->offMuon_normChi2_inner[index];
    nTrackerHit_inner   = ntuple->offMuon_nTrackerHit_inner[index];
    nTrackerLayer_inner = ntuple->offMuon_nTrackerLayer_inner[index];
    nPixelHit_inner     = ntuple->offMuon_nPixelHit_inner[index];

    pt_tuneP      = ntuple->offMuon_pt_tuneP[index];
    ptError_tuneP = ntuple->offMuon_ptError_tuneP[index];

    dxyVTX_best = ntuple->offMuon_dxyVTX_best[index];
    dzVTX_best  = ntuple->offMuon_dzVTX_best[index];
    dB          = ntuple->offMuon_dB[index];

    nMatchedStation  = ntuple->offMuon_nMatchedStation[index];
    nMatchedRPCLayer = ntuple->offMuon_nMatchedRPCLayer[index];
    stationMask      = ntuple->offMuon_stationMask[index];

    pt_inner = ntuple->offMuon_pt_inner[index];

    propEta = ntuple->offMuon_propEta[index];
    propPhi = ntuple->offMuon_propPhi[index];

    relPFIso_dBeta = (PFIso04_charged + max(0., PFIso04_neutral + PFIso04_photon - 0.5*PFIso04_sumPU))/pt;
    relTrkIso = iso03_sumPt / pt;
  }

private:
  void Init()
  {
    pt = -999;
    eta = -999;
    phi = -999;
    mass = -999;
    px = -999;
    py = -999;
    pz = -999;
    energy = -999;
    dB = -999;
    charge = -999;
    isGLB = -999;
    isSTA = -999;
    isTRK = -999;
    isPF = -999;
    isTight = -999;
    isMedium = -999;
    isLoose = -999;
    isHighPt = -999;
    isSoft = -999;
    iso03_sumPt = -999;
    iso03_hadEt = -999;
    iso03_emEt = -999;
    PFIso03_charged = -999;
    PFIso03_neutral = -999;
    PFIso03_photon = -999;
    PFIso03_sumPU = -999;
    PFIso04_charged = -999;
    PFIso04_neutral = -999;
    PFIso04_photon = -999;
    PFIso04_sumPU = -999;
    PFCluster03_ECAL = -999;
    PFCluster03_HCAL = -999;
    PFCluster04_ECAL = -999;
    PFCluster04_HCAL = -999;
    normChi2_global = -999;
    nTrackerHit_global = -999;
    nTrackerLayer_global = -999;
    nPixelHit_global = -999;
    nMuonHit_global = -999;
    normChi2_inner = -999;
    nTrackerHit_inner = -999;
    nTrackerLayer_inner = -999;
    nPixelHit_inner = -999;
    pt_tuneP = -999;
    ptError_tuneP = -999;
    dxyVTX_best = -999;
    dzVTX_best = -999;
    nMatchedStation = -999;
    nMatchedRPCLayer = -999;
    stationMask = -999;

    pt_inner = -999;

    relPFIso_dBeta = -999;
    relTrkIso = -999;
  }
};

// -- offline muon pair
class OffMuPair: public Object
{
public:
  OffMuon first_;
  OffMuon second_;

  Double_t mass;
  Double_t rap;
  Double_t absRap;

  Bool_t isOS;
  Double_t signSum;

  Double_t dEta;
  Double_t dPhi;
  Double_t dR;
  Double_t angle3D;

  Double_t vtxProb;
  Double_t vtxChi2;
  Double_t vtxNdof;
  Double_t normVtxChi2;

  OffMuPair() { Init(); }

  OffMuPair(OffMuon muon1, OffMuon muon2): OffMuPair()
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

  void Set_DimuonVertexVariables(DYTree* ntuple)
  {
    Int_t nPt = (Int_t)ntuple->offMuon_vtxTrkPt1->size();
    if( nPt != (Int_t)ntuple->offMuon_vtxTrkPt2->size() )
    {
      cout << "[OffMuPair::Set_DimuonVertexVariables] offMuon_vtxTrkPt1.size() != offMuon_vtxTrkPt2.size()" << endl;
      return;
    }

    for(Int_t i_pt=0; i_pt<nPt; i_pt++)
    {
      Double_t pt1 = ntuple->offMuon_vtxTrkPt1->at(i_pt);
      Double_t pt2 = ntuple->offMuon_vtxTrkPt2->at(i_pt);

      if( (first_.pt == pt1 && second_.pt == pt2) ||
          (first_.pt == pt2 && second_.pt == pt1)  )
      {
        vtxProb = ntuple->offMuon_vtxTrkProb->at(i_pt);
        vtxChi2 = ntuple->offMuon_vtxTrkChi2->at(i_pt);
        vtxNdof = ntuple->offMuon_vtxTrkNdof->at(i_pt);
        normVtxChi2 = vtxChi2 / vtxNdof;
        break;
      }
    }
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
    signSum = first_.charge + second_.charge; // -- for TMVA (which can't recognize boolean as an input variable)

    dEta = fabs(first_.eta - second_.eta);
    dPhi = first_.vecP.DeltaPhi( second_.vecP );
    dR   = sqrt(dEta*dEta + dPhi*dPhi);
    angle3D = first_.vecP.Angle( second_.vecP.Vect() );
  }

  void Init()
  {
    mass = -999;
    rap = -999;
    absRap = -999;
    signSum = -999;

    isOS = kFALSE;

    vtxProb = -999;
    vtxChi2 = -999;
    vtxNdof = -999;
    normVtxChi2 = -999;
  }
};

}; // -- end of namespace DYTool