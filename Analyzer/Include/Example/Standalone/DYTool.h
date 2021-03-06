// -- namespace with variables, classes for Drell-Yan differential cross section measurements
#pragma once

#include <fstream>
#include <iostream>
#include <TStopwatch.h>
#include <TSystem.h>

#include "Object.h"

namespace DYTool
{

const Double_t lumi = 0; // -- will be set later

enum ColorCode
{
  // -- stack plots
  kDY    = kOrange-2,  // -- signal (DY->mumu, DY->ee)
  kTop   = kRed+2,     // -- ttbar + tW + tbarW
  kEW    = kOrange+10, // -- DY->tautau + WW + WZ + ZZ
  kFakes = kViolet-5,  // -- W+jets + QCD

  kttbar = kRed+2,
  ktW    = kRed+1,
  ktbarW = kRed+3,
  kDYTauTau = kBlue-9,
  kWW = kGreen+2,
  kWZ = kGreen-9,
  kZZ = kGreen,
  kWJets = kCyan,
  kQCD   = kViolet,

  // -- for cross section plots
  kStatUnc = kBlack,
  kTotUnc  = kGray+1,
  kFEWZ    = kBlue,
  kaMCNLO  = kRed
};

Bool_t SelectGenEventBySampleType(TString type, DYTool::DYTree *ntuple)
{
  Bool_t flag = kFALSE;

  if( type.Contains("DYMuMu") || type.Contains("DYEE") || type.Contains("DYTauTau") )
  {
    Int_t requiredID = 0;
    if( type.Contains("DYEE") )     requiredID = 11;
    if( type.Contains("DYMuMu") )   requiredID = 13;
    if( type.Contains("DYTauTau") ) requiredID = 15;

    vector<GenParticle> vec_genLepton;
    for(Int_t i_gen=0; i_gen<ntuple->nGenParticle; i_gen++)
    {
      GenParticle genLepton(ntuple, i_gen);
      if( abs(genLepton.ID) == requiredID && genLepton.isHardProcess )
        vec_genLepton.push_back( genLepton );
    }

    Int_t nGenLeptonHP = (Int_t)vec_genLepton.size();
    if( nGenLeptonHP == 2 )
    {
      TLorentzVector v1 = vec_genLepton[0].vecP;
      TLorentzVector v2 = vec_genLepton[1].vecP;
      Double_t dileptonM = (v1 + v2).M();

      if( type.Contains("_M50to100") )
      {
        if( dileptonM < 100 ) flag = kTRUE;
      }
      else if( type.Contains("_M50to200") )
      {
        if( dileptonM < 200 ) flag = kTRUE;
      }
      else if( type.Contains("_M50to400") )
      {
        if( dileptonM < 400 ) flag = kTRUE;
      }
      else if( type.Contains("_M14p5to50") )
      {
        if( dileptonM > 14.5 && dileptonM < 50 ) flag = kTRUE;
      }
      else if( type.Contains("_M15to50") )
      {
        if( dileptonM > 15 && dileptonM < 50 ) flag = kTRUE;
      }
      else
        flag = kTRUE;
    }
  }
  else // -- if it is not DY sample
    flag = kTRUE;

  return flag;
}

struct SampleInfo
{
  TString type;
  TString ntuplePathFile;
  Bool_t isMC;
  Double_t xSec;
  Double_t sumWeight;
  Double_t normFactor;
};

class ClassTemplate
{
public:
  DYTool::SampleInfo sampleInfo_;
  TStopwatch timer_;

  ClassTemplate()
  {
    sampleInfo_.type = "";
    sampleInfo_.ntuplePathFile = "";
    sampleInfo_.isMC = kFALSE;
    sampleInfo_.xSec = 0;
    sampleInfo_.sumWeight = 0;
    sampleInfo_.normFactor = 0;
  }

  virtual void Run() {}

  void CheckSampleInfo()
  {
    if( sampleInfo_.type == "" )
      printf("[ClassTemplate::CheckSampleInfo] sampleInfo_.type is not assigned\n");

    if( sampleInfo_.ntuplePathFile == "" )
      printf("[ClassTemplate::CheckSampleInfo] sampleInfo_.ntuplePathFile is not assigned\n");

    if( sampleInfo_.xSec == 0 )
      printf("[ClassTemplate::CheckSampleInfo] sampleInfo_.xSec is not assigned\n");

    if( sampleInfo_.sumWeight == 0 )
      printf("[ClassTemplate::CheckSampleInfo] sampleInfo_.sumWeight is not assigned\n");

    if( sampleInfo_.normFactor == 0 )
      printf("[ClassTemplate::CheckSampleInfo] sampleInfo_.normFactor is not assigned\n");
  }

  void PrintSampleInfo()
  {
    cout << "[ClassTemplate::CheckSampleInfo]" << endl;
    cout << "  sampleInfo_.type =           " << sampleInfo_.type << endl;
    cout << "  sampleInfo_.ntuplePathFile = " << sampleInfo_.ntuplePathFile << endl;
    cout << "  sampleInfo_.isMC =           " << sampleInfo_.isMC << endl;
    cout << "  sampleInfo_.xSec =           " << sampleInfo_.xSec << endl;
    cout << "  sampleInfo_.sumWeight =      " << sampleInfo_.sumWeight << endl;
    cout << "  sampleInfo_.normFactor =     " << sampleInfo_.normFactor << endl;
    cout << endl;
  }

  void StartTimer()
  {
    timer_.Start();
  }

  void PrintRunTime()
  {
    Double_t cpuTime = timer_.CpuTime();
    Double_t realTime = timer_.RealTime();

    cout << "************************************************" << endl;
    cout << "Total real time: " << realTime << " seconds = " << realTime / 60.0 << " minutes = " << realTime / (60.0*60.0) << " hours" << endl;
    cout << "Total CPU time:  " << cpuTime  << " seconds = " << cpuTime / 60.0  << " minutes = " << cpuTime / (60.0*60.0)  << " hours" << endl;
    cout << "  CPU time / real time = " << cpuTime / realTime << endl;
    cout << "************************************************" << endl;
  }

};

void AddNtupleToChain(TChain* chain, TString textFileName)
{
  ifstream openFile( textFileName.Data() );
  cout << "===============================" << endl;
  cout << "Read " << textFileName << endl;
  cout << "===============================" << endl;

  if( openFile.is_open() )
  {
    string line;
    while(getline(openFile, line))
    {
      cout << line << endl;
      chain->Add(line.data());
    }
    openFile.close();
  }

  cout << "==================================" << endl;
  cout << "All ROOT Files are put into TChain" << endl;
  cout << "==================================" << endl;
}


vector<DYTool::Muon> GetAllMuons(DYTool::DYTree *ntuple)
{
  vector<DYTool::Muon> vec_muon;
  for(Int_t i_mu=0; i_mu<ntuple->nMuon; i_mu++)
  {
    DYTool::Muon mu(ntuple, i_mu);
    vec_muon.push_back( mu );
  }

  return vec_muon;
}


vector<DYTool::MuPair> GetAllMuPairs(DYTool::DYTree *ntuple)
{
  vector<DYTool::MuPair> vec_muonPair;

  vector<DYTool::Muon> vec_muon = DYTool::GetAllMuons(ntuple);

  for(Int_t i_mu=0; i_mu<ntuple->nMuon; i_mu++)
  {
    DYTool::Muon mu1 = vec_muon[i_mu];
    for(Int_t j_mu=i_mu+1; j_mu<ntuple->nMuon; j_mu++) // -- iterate from i_mu+1 to avoid duplications
    {
      DYTool::Muon mu2 = vec_muon[j_mu];

      DYTool::MuPair muonPair(mu1, mu2);
      vec_muonPair.push_back( muonPair );
    }
  }

  return vec_muonPair;
}


vector<DYTool::GenParticle> GetAllGenLeptons(DYTool::DYTree *ntuple, Int_t pdgID, TString genFlagName)
{
  vector< DYTool::GenParticle > vec_genLepton;
  vec_genLepton.clear();

  if( genFlagName != "isHardProcess" && 
      genFlagName != "fromHardProcessFinalState" )
  {
    cout << "[DYTool::GetAllGenLeptons] genFlagName = " << genFlagName << " is not supported -> empty vector is returned" << endl;
    cout << "  Update the code if you want to use " << genFlagName << endl;
    return vec_genLepton;
  }

  if( pdgID != 11 && pdgID != 13 )
  {
    cout << "[DYTool::GetAllGenLeptons] pdgID = " << pdgID << "is not supported -> empty vector is returned" << endl;
    cout << "  Update the code if you want to use " << pdgID << endl;
    return vec_genLepton;
  }

  for(Int_t i_gen=0; i_gen<ntuple->nGenParticle; i_gen++)
  {
    DYTool::GenParticle genLepton( ntuple, i_gen );

    if( abs(genLepton.ID) == pdgID )
    {
      if( genFlagName == "isHardProcess" )
      {
        if( genLepton.isHardProcess ) 
          vec_genLepton.push_back( genLepton );
      }
      else if( genFlagName == "fromHardProcessFinalState" )
      {
        if( genLepton.fromHardProcessFinalState ) 
          vec_genLepton.push_back( genLepton );
      }
    }
  }

  return vec_genLepton;
}

Bool_t CompareMuPair_LargerDimuonMass( DYTool::MuPair pair1, DYTool::MuPair pair2 )
{
  // -- the pair with the highest mass
  return pair1.mass > pair2.mass; 
}


DYTool::MuPair EventSelection(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  DYTool::MuPair muPair_dummy;

  if( ntuple->nVtx <= 0 ) return muPair_dummy;

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

  if( !doPassTrig ) return muPair_dummy;


  vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

  vector< DYTool::MuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
    if( muPair.IsDYCandidate(ntuple) ) vec_goodMuPair.push_back( muPair );

  if( vec_goodMuPair.size() == 1 ) // -- only takes the case when there's exact 1 muon pair
  {
    doPass = kTRUE;
    return vec_goodMuPair[0];
  }
  else
  {
    doPass = kFALSE;
    return muPair_dummy;
  }
}

// -- require unprescaled L1 fired
DYTool::MuPair EventSelection_L1Requirement(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  DYTool::MuPair muPair_dummy;

  // -- at least 1 vertex in a event (not checking whether it is the vertex for selected muons, yet)
  if( ntuple->nVtx <= 0 ) return muPair_dummy;

  // -- pass unprescaled L1 (only for 2018)
  // -- 4 = L1_DoubleMu_15_7
  // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
  // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
  Bool_t doPassL1 = kFALSE;
  if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;

  if( !doPassL1 ) return muPair_dummy;

  // -- pass HLT
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

  if( !doPassTrig ) return muPair_dummy;

  // -- dimuon selection
  vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

  vector< DYTool::MuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
    if( muPair.IsDYCandidate(ntuple) ) vec_goodMuPair.push_back( muPair );

  if( vec_goodMuPair.size() == 1 ) // -- only takes the case when there's exact 1 muon pair
  {
    doPass = kTRUE;
    return vec_goodMuPair[0];
  }
  else
  {
    doPass = kFALSE;
    return muPair_dummy;
  }
}

// -- require unprescaled L1 fired
DYTool::MuPair EventSelection_BDTInput(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  DYTool::MuPair muPair_dummy;

  // -- at least 1 vertex in a event (not checking whether it is the vertex for selected muons, yet)
  if( ntuple->nVtx <= 0 ) return muPair_dummy;

  // -- pass unprescaled L1 (only for 2018)
  // -- 4 = L1_DoubleMu_15_7
  // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
  // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
  Bool_t doPassL1 = kFALSE;
  // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;
  if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut

  if( !doPassL1 ) return muPair_dummy;

  // -- pass HLT
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

  if( !doPassTrig ) return muPair_dummy;

  // -- dimuon selection
  vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

  vector< DYTool::MuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
    if( muPair.IsDYCandidate_BDTInput(ntuple) ) vec_goodMuPair.push_back( muPair );

  if( vec_goodMuPair.size() == 1 ) // -- only takes the case when there's exact 1 muon pair
  {
    doPass = kTRUE;
    return vec_goodMuPair[0];
  }
  else
  {
    doPass = kFALSE;
    return muPair_dummy;
  }
}

Bool_t CompareMuonPair_SmallerVtxChi2( DYTool::MuPair pair1, DYTool::MuPair pair2 )
{
  // -- the pair with "smallest" vertex chi2 will be the first element -- //
  return pair1.normVtxChi2 < pair2.normVtxChi2; 
}

// -- exactly two muons -> select smallest vertex chi2
DYTool::MuPair EventSelection_BDTInput_SmallestVtxChi2(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  DYTool::MuPair muPair_dummy;

  // -- at least 1 vertex in a event (not checking whether it is the vertex for selected muons, yet)
  if( ntuple->nVtx <= 0 ) return muPair_dummy;

  // -- pass unprescaled L1 (only for 2018)
  // -- 4 = L1_DoubleMu_15_7
  // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
  // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
  Bool_t doPassL1 = kFALSE;
  // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;
  if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut

  if( !doPassL1 ) return muPair_dummy;

  // -- pass HLT
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

  if( !doPassTrig ) return muPair_dummy;

  // -- dimuon selection
  vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

  vector< DYTool::MuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
    if( muPair.IsDYCandidate_BDTInput(ntuple) ) vec_goodMuPair.push_back( muPair );

  if( vec_goodMuPair.size() == 0 )
  {
    doPass = kFALSE;
    return muPair_dummy;
  }
  else // -- take the pair with the smallest chi2
  {
    doPass = kTRUE;
    std::sort(vec_goodMuPair.begin(), vec_goodMuPair.end(), DYTool::CompareMuonPair_SmallerVtxChi2);
    return vec_goodMuPair[0]; // -- pair with smallest vertex chi2 value
  }
}

// -- remove OS requirement (to use OS/SS distribution for the BDT) + smallest vertex chi2
DYTool::MuPair EventSelection_BDTInput_noOS(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  DYTool::MuPair muPair_dummy;

  // -- at least 1 vertex in a event (not checking whether it is the vertex for selected muons, yet)
  if( ntuple->nVtx <= 0 ) return muPair_dummy;

  // -- pass unprescaled L1 (only for 2018)
  // -- 4 = L1_DoubleMu_15_7
  // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
  // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
  Bool_t doPassL1 = kFALSE;
  // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;
  if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut

  if( !doPassL1 ) return muPair_dummy;

  // -- pass HLT
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

  if( !doPassTrig ) return muPair_dummy;

  // -- dimuon selection
  vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

  vector< DYTool::MuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
    if( muPair.IsDYCandidate_BDTInput_noOS(ntuple) ) vec_goodMuPair.push_back( muPair );

  Int_t nGoodPair = (Int_t)vec_goodMuPair.size();
  if( nGoodPair == 0 )
  {
    doPass = kFALSE;
    return muPair_dummy;
  }
  else if( nGoodPair == 1) // -- exactly one pair: no sort is needed
  {
    doPass = kTRUE;
    return vec_goodMuPair[0];
  }
  else // -- multiple pair: take the pair with the smallest chi2
  {
    doPass = kTRUE;
    std::sort(vec_goodMuPair.begin(), vec_goodMuPair.end(), DYTool::CompareMuonPair_SmallerVtxChi2);
    return vec_goodMuPair[0]; // -- pair with smallest vertex chi2 value
  }
}

// -- remove M > 10 GeV condition; to check upsilon contribution in details
DYTool::MuPair EventSelection_BDTInput_noOS_noMassCut(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  DYTool::MuPair muPair_dummy;

  // -- at least 1 vertex in a event (not checking whether it is the vertex for selected muons, yet)
  if( ntuple->nVtx <= 0 ) return muPair_dummy;

  // -- pass unprescaled L1 (only for 2018)
  // -- 4 = L1_DoubleMu_15_7
  // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
  // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
  Bool_t doPassL1 = kFALSE;
  // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;
  if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut

  if( !doPassL1 ) return muPair_dummy;

  // -- pass HLT
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

  if( !doPassTrig ) return muPair_dummy;

  // -- dimuon selection
  vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

  vector< DYTool::MuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
    if( muPair.IsDYCandidate_BDTInput_noOS_noMassCut(ntuple) ) vec_goodMuPair.push_back( muPair );

  Int_t nGoodPair = (Int_t)vec_goodMuPair.size();
  if( nGoodPair == 0 )
  {
    doPass = kFALSE;
    return muPair_dummy;
  }
  else if( nGoodPair == 1) // -- exactly one pair: no sort is needed
  {
    doPass = kTRUE;
    return vec_goodMuPair[0];
  }
  else // -- multiple pair: take the pair with the smallest chi2
  {
    doPass = kTRUE;
    std::sort(vec_goodMuPair.begin(), vec_goodMuPair.end(), DYTool::CompareMuonPair_SmallerVtxChi2);
    return vec_goodMuPair[0]; // -- pair with smallest vertex chi2 value
  }
}

// -- remove OS requirement (to use OS/SS distribution for the BDT) + smallest vertex chi2 + remove L1 with OS requirement
DYTool::MuPair EventSelection_BDTInput_noOS_noOSL1(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  DYTool::MuPair muPair_dummy;

  // -- at least 1 vertex in a event (not checking whether it is the vertex for selected muons, yet)
  if( ntuple->nVtx <= 0 ) return muPair_dummy;

  // -- pass unprescaled L1 (only for 2018)
  // -- 4 = L1_DoubleMu_15_7
  // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
  // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
  Bool_t doPassL1 = kFALSE;
  // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;
  // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut
  if( ntuple->vec_L1Bit->at(4) ) doPassL1 = kTRUE; // -- only L1_DoubleMu_15_7

  if( !doPassL1 ) return muPair_dummy;

  // -- pass HLT
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

  if( !doPassTrig ) return muPair_dummy;

  // -- dimuon selection
  vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

  vector< DYTool::MuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
    if( muPair.IsDYCandidate_BDTInput_noOS(ntuple) ) vec_goodMuPair.push_back( muPair );

  Int_t nGoodPair = (Int_t)vec_goodMuPair.size();
  if( nGoodPair == 0 )
  {
    doPass = kFALSE;
    return muPair_dummy;
  }
  else if( nGoodPair == 1) // -- exactly one pair: no sort is needed
  {
    doPass = kTRUE;
    return vec_goodMuPair[0];
  }
  else // -- multiple pair: take the pair with the smallest chi2
  {
    doPass = kTRUE;
    std::sort(vec_goodMuPair.begin(), vec_goodMuPair.end(), DYTool::CompareMuonPair_SmallerVtxChi2);
    return vec_goodMuPair[0]; // -- pair with smallest vertex chi2 value
  }
}


// -- require unprescaled L1 fired
DYTool::MuPair EventSelection_BDTInput_Loose(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  DYTool::MuPair muPair_dummy;

  // -- at least 1 vertex in a event (not checking whether it is the vertex for selected muons, yet)
  if( ntuple->nVtx <= 0 ) return muPair_dummy;

  // -- pass unprescaled L1 (only for 2018)
  // -- 4 = L1_DoubleMu_15_7
  // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
  // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
  Bool_t doPassL1 = kFALSE;
  // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;
  if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut

  if( !doPassL1 ) return muPair_dummy;

  // -- pass HLT
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

  if( !doPassTrig ) return muPair_dummy;

  // -- dimuon selection
  vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

  vector< DYTool::MuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
    if( muPair.IsDYCandidate_BDTInput_Loose(ntuple) ) vec_goodMuPair.push_back( muPair );

  if( vec_goodMuPair.size() == 1 ) // -- only takes the case when there's exact 1 muon pair
  {
    doPass = kTRUE;
    return vec_goodMuPair[0];
  }
  else
  {
    doPass = kFALSE;
    return muPair_dummy;
  }
}

// -- for BDT test: return all pairs passing dimuon selection (w/o final pair selection)
vector< DYTool::MuPair > EventSelection_BDTInput_Loose_noPairSel(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  vector<DYTool::MuPair> vec_muPair_dummy;

  // -- at least 1 vertex in a event (not checking whether it is the vertex for selected muons, yet)
  if( ntuple->nVtx <= 0 ) return vec_muPair_dummy;

  // -- pass unprescaled L1 (only for 2018)
  // -- 4 = L1_DoubleMu_15_7
  // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
  // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
  Bool_t doPassL1 = kFALSE;
  // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;
  if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut

  if( !doPassL1 ) return vec_muPair_dummy;

  // -- pass HLT
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

  if( !doPassTrig ) return vec_muPair_dummy;

  // -- dimuon selection
  vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

  vector< DYTool::MuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
    if( muPair.IsDYCandidate_BDTInput_Loose(ntuple) ) vec_goodMuPair.push_back( muPair );

  if( vec_goodMuPair.size() > 0 )
  {
    doPass = kTRUE;
    return vec_goodMuPair;
  }
  else
    return vec_goodMuPair; // -- empty vector
}

// -- for BDT test: remove almost all criteria
vector< DYTool::MuPair > EventSelection_BDTInput_AlmostNoCut(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  vector<DYTool::MuPair> vec_muPair_dummy;

  // -- pass unprescaled L1 (only for 2018)
  // -- 4 = L1_DoubleMu_15_7
  // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
  // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
  Bool_t doPassL1 = kFALSE;
  // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;
  if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut

  if( !doPassL1 ) return vec_muPair_dummy;

  // -- pass HLT
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

  if( !doPassTrig ) return vec_muPair_dummy;

  // -- dimuon selection
  vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

  vector< DYTool::MuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
    if( muPair.IsDYCandidate_BDTInput_AlmostNoCut(ntuple) ) vec_goodMuPair.push_back( muPair );

  if( vec_goodMuPair.size() > 0 )
  {
    doPass = kTRUE;
    return vec_goodMuPair;
  }
  else
    return vec_muPair_dummy; // -- empty vector
}

// -- L1 requirement: not yet
DYTool::MuPair EventSelection_Tight(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  DYTool::MuPair muPair_dummy;

  if( ntuple->nVtx <= 0 ) return muPair_dummy;

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

  if( !doPassTrig ) return muPair_dummy;


  vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

  vector< DYTool::MuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
    if( muPair.IsDYCandidate_Tight(ntuple) ) vec_goodMuPair.push_back( muPair );

  if( vec_goodMuPair.size() == 1 ) // -- only takes the case when there's exact 1 muon pair
  {
    doPass = kTRUE;
    return vec_goodMuPair[0];
  }
  else
  {
    doPass = kFALSE;
    return muPair_dummy;
  }
}

// -- add L1 requirement
DYTool::MuPair EventSelection_Tight_L1(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  DYTool::MuPair muPair_dummy;

  if( ntuple->nVtx <= 0 ) return muPair_dummy;

  // -- pass unprescaled L1 (only for 2018)
  // -- 4 = L1_DoubleMu_15_7
  // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
  // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
  Bool_t doPassL1 = kFALSE;
  if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;

  if( !doPassL1 ) return muPair_dummy;

  // -- HLT
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

  if( !doPassTrig ) return muPair_dummy;


  vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

  vector< DYTool::MuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
    if( muPair.IsDYCandidate_Tight(ntuple) ) vec_goodMuPair.push_back( muPair );

  if( vec_goodMuPair.size() == 1 ) // -- only takes the case when there's exact 1 muon pair
  {
    doPass = kTRUE;
    return vec_goodMuPair[0];
  }
  else
  {
    doPass = kFALSE;
    return muPair_dummy;
  }
}

DYTool::MuPair EventSelection_Tight_L1_NoMassCut(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  DYTool::MuPair muPair_dummy;

  if( ntuple->nVtx <= 0 ) return muPair_dummy;

  // -- pass unprescaled L1 (only for 2018)
  // -- 4 = L1_DoubleMu_15_7
  // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
  Bool_t doPassL1 = kFALSE;
  if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;

  if( !doPassL1 ) return muPair_dummy;

  // -- HLT
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

  if( !doPassTrig ) return muPair_dummy;


  vector< DYTool::MuPair > vec_muPair = DYTool::GetAllMuPairs(ntuple);

  vector< DYTool::MuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
    if( muPair.IsDYCandidate_Tight(ntuple) ) vec_goodMuPair.push_back( muPair );

  if( vec_goodMuPair.size() == 1 ) // -- only takes the case when there's exact 1 muon pair
  {
    doPass = kTRUE;
    return vec_goodMuPair[0];
  }
  else
  {
    doPass = kFALSE;
    return muPair_dummy;
  }
}

Bool_t GenRecoMatching(DYTool::Muon muon, DYTool::DYTree *ntuple)
{
  Bool_t flag = kFALSE;

  vector<GenParticle> vec_genLepton;
  for(Int_t i_gen=0; i_gen<ntuple->nGenParticle; i_gen++)
  {
    GenParticle genLepton(ntuple, i_gen);
    if( abs(genLepton.ID) == 13 && genLepton.fromHardProcessFinalState )
      vec_genLepton.push_back( genLepton );
  }

  Int_t nGenLepton = (Int_t)vec_genLepton.size();
  if( nGenLepton != 2 )
  {
    cout << "[DYTool::GenRecoMatching] nGenLepton != 2" << endl;
    return flag;
  }

  for(auto& genLepton : vec_genLepton)
  {
    Double_t dR = genLepton.vecP.DeltaR( muon.vecP );
    if( dR < 0.3 )
    {
      flag = kTRUE;
      break;
    }
  }

  return flag;
}

Bool_t GenRecoMatching(DYTool::MuPair pair, DYTool::DYTree *ntuple)
{
  Bool_t flag = kFALSE;

  if( GenRecoMatching(pair.first_, ntuple) && GenRecoMatching(pair.second_, ntuple) ) flag = kTRUE;

  return flag;
}

class PUReweightTool
{
public:
  TString type_;
  TH1D* h_PUWeight_ = nullptr;
  Double_t lastPUBin_;

  PUReweightTool(TString type)
  {
    type_ = type;
    Init();
  }

  PUReweightTool(TString fileName, TString histName)
  {
    h_PUWeight_ = Get_Hist(fileName, histName);
  }

  Double_t Weight( Int_t truePU )
  {
    if( truePU > lastPUBin_ ) return 0;

    return h_PUWeight_->GetBinContent(truePU+1); // -- e.g. truePU = 0 -> 1st bin value (PU = 0), truePU = 98 -> 99th bin [98, 99] value (PU=98) 
  }

private:
  void Init()
  {
    if( h_PUWeight_ == nullptr ) // -- if it is not set yet
    {
      TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
      if( type_ == "2018" )
      {
        TString rootFilePath = analyzerPath+"/Include/Pileup/ROOTFile_PUReweighting_2018.root";
        h_PUWeight_ = Get_Hist(rootFilePath, "h_ratio");
      }
      else
      {
        cout << "[PUReweightTool] type = " << type_ << " is not recognizable" << endl;
        h_PUWeight_ = nullptr;
        lastPUBin_ = -999;
      }
    }


    if( h_PUWeight_ != nullptr )
    {
      Int_t nBin = h_PUWeight_->GetNbinsX();
      lastPUBin_ = h_PUWeight_->GetBinLowEdge(nBin);
      cout << "[PUReweightTool] lastPUBin = " << lastPUBin_ << endl;
    }
  }

  TH1D* Get_Hist(TString fileName, TString histName, TString histName_new = "" )
  {
    TH1::AddDirectory(kFALSE);

    TFile* f_input = TFile::Open( fileName );
    TH1D* h_temp = (TH1D*)f_input->Get(histName)->Clone();

    if( histName_new != "" )
      h_temp->SetName( histName_new );

    f_input->Close();

    return h_temp;
  }
};

static inline void loadBar(int x, int n, int r, int w)
{
    // Only update r times.
    if( x == n )
      cout << endl;

    if ( x % (n/r +1) != 0 ) return;

 
    // Calculuate the ratio of complete-to-incomplete.
    float ratio = x/(float)n;
    int   c     = ratio * w;
 
    // Show the percentage complete.
    printf("%3d%% [", (int)(ratio*100) );
 
    // Show the load bar.
    for (int x=0; x<c; x++) cout << "=";
 
    for (int x=c; x<w; x++) cout << " ";
 
    // ANSI Control codes to go back to the
    // previous line and clear it.
  cout << "]\r" << flush;

}


}; // -- end of namespace DYTool