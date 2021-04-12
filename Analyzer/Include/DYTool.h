// -- namespace with variables, classes for Drell-Yan differential cross section measurements
#pragma once

#include <fstream>
#include <iostream>
#include <TStopwatch.h>
#include <TSystem.h>
#include <TObjArray.h>

#include <Include/Object.h>
#include <Include/SimplePlotTools.h>

namespace DYTool
{

const Double_t LUMI_2016 = 0; // -- will be set later
const Double_t LUMI_2017 = 0; // -- will be set later
const Double_t LUMI_2018 = 59740.565202; // -- 59.7  /fb

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

  // -- Give different name for each given text file
  TString GetOutputFileName(TString tag)
  {
    TString textFilePath = sampleInfo_.ntuplePathFile;
    TObjArray *tokens = textFilePath.Tokenize("/");
    TString textFileName = ((TObjString*)tokens->Last())->GetString();
    textFileName = textFileName.ReplaceAll(".txt", "");
    TString outputName = TString::Format("ROOTFile_%s_%s_%s.root", tag.Data(), sampleInfo_.type.Data(), textFileName.Data());

    return outputName;
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

vector<DYTool::OffMuon> GetAllOfflineMuons(DYTool::DYTree *ntuple)
{
  vector<DYTool::OffMuon> vec_muon;
  for(Int_t i_mu=0; i_mu<ntuple->nOffMuon; i_mu++)
  {
    DYTool::OffMuon mu(ntuple, i_mu);
    vec_muon.push_back( mu );
  }

  return vec_muon;
}


vector<DYTool::OffMuPair> GetAllOffMuPairs(DYTool::DYTree *ntuple)
{
  vector<DYTool::OffMuPair> vec_muonPair;

  vector<DYTool::OffMuon> vec_muon = DYTool::GetAllOfflineMuons(ntuple);

  for(Int_t i_mu=0; i_mu<ntuple->nOffMuon; i_mu++)
  {
    DYTool::OffMuon mu1 = vec_muon[i_mu];
    for(Int_t j_mu=i_mu+1; j_mu<ntuple->nOffMuon; j_mu++) // -- iterate from i_mu+1 to avoid duplications
    {
      DYTool::OffMuon mu2 = vec_muon[j_mu];

      DYTool::OffMuPair muonPair(mu1, mu2);
      muonPair.Set_DimuonVertexVariables(ntuple);
      vec_muonPair.push_back( muonPair );
    }
  }

  return vec_muonPair;
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

Bool_t CompareMuonPair_LargerDimuonPt( DYTool::MuPair pair1, DYTool::MuPair pair2 )
{
  return pair1.pt > pair2.pt; 
}

// -- mimic the selection used in EXO-19-018
DYTool::MuPair EventSelection_EXO19018(DYTool::DYTree *ntuple, Bool_t& doPass)
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
  // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut

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
    if( muPair.IsDiMuCandidate_EXO19018(ntuple) ) vec_goodMuPair.push_back( muPair );

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
  else // -- multiple pair: take the pair with the largest dimuon pT
  {
    doPass = kTRUE;
    std::sort(vec_goodMuPair.begin(), vec_goodMuPair.end(), DYTool::CompareMuonPair_LargerDimuonPt);
    return vec_goodMuPair[0]; // -- pair with the largest dimuon pT
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

Bool_t CompareOffMuonPair_SmallerVtxChi2( DYTool::OffMuPair pair1, DYTool::OffMuPair pair2 )
{
  // -- the pair with "smallest" vertex chi2 will be the first element -- //
  return pair1.normVtxChi2 < pair2.normVtxChi2; 
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
    h_PUWeight_ = PlotTool::Get_Hist(fileName, histName);
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
        h_PUWeight_ = PlotTool::Get_Hist(rootFilePath, "h_ratio");
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

class SimpleSampleInfo
{
public:
  TString type_ = "";

  Double_t xSec_      = 0;
  Double_t sumWeight_ = 0;

  SimpleSampleInfo()
  {
    cout << "Default constructor: type should be given to set internal variables" << endl;
    cout << "   ---> Use Set_Type(TString type)" << endl;
    cout << endl;
  }

  SimpleSampleInfo(TString type)
  {
    Set_Type(type);
  }

  void Set_Type(TString type)
  {
    type_ = type;
    Init();
  }

  Double_t NormFactorToLumi(TString year)
  {
    Double_t theLumi = 0;
    if(year == "2016")      theLumi = DYTool::LUMI_2016;
    else if(year == "2017") theLumi = DYTool::LUMI_2017; 
    else if(year == "2018") theLumi = DYTool::LUMI_2018;
    else                    cout << "year = " << year << "is not recognizable" << endl;

    return (xSec_ * theLumi) / sumWeight_;
  }

  // -- if luminosity is directly given as the argument
  Double_t NormFactorToLumi(Double_t theLumi)
  {
    return (xSec_ * theLumi) / sumWeight_;
  }

  Double_t Get_SumWeight() { return sumWeight_; }
  Double_t Get_xSec()      { return xSec_; }

private:
  void Init()
  {
    Set_SumWeight();
    Set_xSec();
  }

  void Set_SumWeight()
  {
    if( type_ == "DYMuMu_M10to50" )                 sumWeight_ = 28985097.0;
    else if( type_ == "DYMuMu_M50toInf" )           sumWeight_ = 44318555.0;
    else if( type_ == "DYTauTau_M10to50" )          sumWeight_ = 28843598.0;
    else if( type_ == "DYTauTau_M50toInf" )         sumWeight_ = 44279551.0;
    else if( type_ == "ttbar" )                     sumWeight_ = 63811310.0;
    else if( type_ == "WJets" )                     sumWeight_ = 71061720.0;
    else if( type_ == "QCDMuEnriched_Pt20toInf" )   sumWeight_ = 22165320.0;
    else if( type_ == "QCDMuEnriched_Pt15to20" )    sumWeight_ = 4572168.0;
    else if( type_ == "QCDMuEnriched_Pt20to30" )    sumWeight_ = 26549827.0;
    else if( type_ == "QCDMuEnriched_Pt30to50" )    sumWeight_ = 30431342.0;
    else if( type_ == "QCDMuEnriched_Pt50to80" )    sumWeight_ = 20360155.0;
    else if( type_ == "QCDMuEnriched_Pt80to120" )   sumWeight_ = 25652280.0;
    else if( type_ == "QCDMuEnriched_Pt120to170" )  sumWeight_ = 21315922.0;
    else if( type_ == "QCDMuEnriched_Pt170to300" )  sumWeight_ = 36372547.0;
    else if( type_ == "QCDMuEnriched_Pt300to470" )  sumWeight_ = 29488563.0;
    else if( type_ == "QCDMuEnriched_Pt470to600" )  sumWeight_ = 20542857.0;
    else if( type_ == "QCDMuEnriched_Pt600to800" )  sumWeight_ = 16891461.0;
    else if( type_ == "QCDMuEnriched_Pt800to1000" ) sumWeight_ = 16749914.0;
    else if( type_ == "QCDMuEnriched_Pt1000toInf" ) sumWeight_ = 11039499.0;
    else
    {
      cout << "[SimpleSampleInfo::Set_SumWeight] no information for type = " << type_ << endl;
      sumWeight_ = 0;
    }
  }

  void Set_xSec()
  {
    // -- temporary k-factor for low mass sample
    Double_t kFactor_M40to50_NNLOtoNLO = 1.0684240278;

    // if( type_ == "DYMuMu_M10to50" )                 xSec_ = 6270.0; # -- 
    // if( type_ == "DYMuMu_M10to50" )                 xSec_ = 6203.3333; // -- 18610 / 3
    if( type_ == "DYMuMu_M10to50" )                 xSec_ = 6203.3333 * kFactor_M40to50_NNLOtoNLO;
    else if( type_ == "DYMuMu_M50toInf" )           xSec_ = 2009.41;
    else if( type_ == "DYTauTau_M10to50" )          xSec_ = 6270.0;
    else if( type_ == "DYTauTau_M50toInf" )         xSec_ = 2009.41;
    else if( type_ == "ttbar" )                     xSec_ = 88.29;
    else if( type_ == "WJets" )                     xSec_ = 61526.7;
    else if( type_ == "QCDMuEnriched_Pt20toInf" )   xSec_ = 239400.0;
    else if( type_ == "QCDMuEnriched_Pt15to20" )    xSec_ = 2799000.0;
    else if( type_ == "QCDMuEnriched_Pt20to30" )    xSec_ = 2526000.0;
    else if( type_ == "QCDMuEnriched_Pt30to50" )    xSec_ = 1362000.0;
    else if( type_ == "QCDMuEnriched_Pt50to80" )    xSec_ = 376600.0;
    else if( type_ == "QCDMuEnriched_Pt80to120" )   xSec_ = 88930.0;
    else if( type_ == "QCDMuEnriched_Pt120to170" )  xSec_ = 21230.0;
    else if( type_ == "QCDMuEnriched_Pt170to300" )  xSec_ = 7055.0;
    else if( type_ == "QCDMuEnriched_Pt300to470" )  xSec_ = 797.3; // -- TuneCUETP8M1
    else if( type_ == "QCDMuEnriched_Pt470to600" )  xSec_ = 59.24;
    else if( type_ == "QCDMuEnriched_Pt600to800" )  xSec_ = 25.25; // -- TuneCUETP8M1
    else if( type_ == "QCDMuEnriched_Pt800to1000" ) xSec_ = 4.723; // -- TuneCUETP8M1
    else if( type_ == "QCDMuEnriched_Pt1000toInf" ) xSec_ = 1.613; // -- TuneCUETP8M1
    else
    {
      cout << "[SimpleSampleInfo::Set_xSec] no information for type = " << type_ << endl;
      xSec_ = 0;
    }
  }

};

// -- Get histograms from a ROOT file
// ---- normalization to the luminosity
// ---- automatic merging all histograms from binned samples
class HistGetter
{
public:

  // -- location with the ROOT file containing histograms
  TString basePath_ = "";

  // -- base ROOT file name (Name format: baseFileName + "processType" + .root)
  TString baseFileName_ = "";

  Bool_t doNorm_ = kFALSE;
  Double_t lumi_ = -1.0;

  Bool_t doRemoveNegBin_ = kFALSE;

  HistGetter()
  {
    cout << "Default constructor" << endl;
    cout << "--> Use HistGetter(TString basePath) instead of it" << endl;
  }

  HistGetter(TString basePath, TString baseFileName)
  {
    Set_BasePath(basePath);
    Set_BaseFileName(baseFileName);
  }

  void Set_BasePath(TString basePath)         { basePath_     = basePath; }
  void Set_BaseFileName(TString baseFileName) { baseFileName_ = baseFileName; }


  // -- when an exact lumi value is given (in /pb)
  void DoNormalization(Double_t lumi)
  {
    doNorm_ = kTRUE;
    lumi_ = lumi;
  }

  // -- when an year is given: use the luminosity of the full data taken in the year
  void DoNormalization(TString year)
  {
    doNorm_ = kTRUE;
    if(year == "2016")      lumi_ = DYTool::LUMI_2016;
    else if(year == "2017") lumi_ = DYTool::LUMI_2017; 
    else if(year == "2018") lumi_ = DYTool::LUMI_2018;
    else
    {
      cout << "year = " << year << "is not recognizable" << endl;
      lumi_ = -1.0;
    }
  }

  void DoRemoveNegativeBin(Bool_t flag = kTRUE)
  {
    doRemoveNegBin_ = flag;
  }


  // -- when a single histogram is enough
  // -- requirement on the processType
  // ---- 1) ROOT file name should contain processType
  // ---- 2) processType should be recognizable by SimpleSampleInfo
  // -- isMC: normalization is not performed if it is false
  TH1D* Get_Histogram(TString histName, TString processType, Bool_t isMC = kTRUE)
  {
    SanityCheck();

    return Init_Histogram(histName, processType, isMC);
  }

  // -- when multiple histograms should be merged
  TH1D* Get_Histogram(TString histName, vector<TString> vec_processType, Bool_t isMC = kTRUE)
  {
    SanityCheck();

    return Init_MergedHistogram(histName, vec_processType, isMC);
  }


private:
  void SanityCheck()
  {
    if( doNorm_ && lumi_ < 0 )
    {
      cout << "[HistGetter::GetHistogram] Normalization is activated but the luminosity is not correctly set: need to check" << endl;
      return;
    }
  }

  TH1D* Init_Histogram(TString histName, TString processType, Bool_t isMC)
  {
    TString fileName = TString::Format("%s/%s_%s.root", basePath_.Data(), baseFileName_.Data(), processType.Data());
    TH1D* h = PlotTool::Get_Hist(fileName, histName);
    h->Sumw2();
    RemoveNegativeBin(h, processType);

    if( isMC && doNorm_ )
    {
      DYTool::SimpleSampleInfo sampleInfo(processType);
      Double_t normFactor = sampleInfo.NormFactorToLumi(lumi_);
      h->Scale( normFactor );
    }

    return h;
  }

  void RemoveNegativeBin(TH1D* h, TString processType)
  {
    Int_t nBin = h->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t binContent = h->GetBinContent(i_bin);
      if( binContent < 0 )
      {
        cout << "[" << h->GetName() << ", " << processType << "] binContent (" << i_bin << " bin) = " << binContent << " < 0 ... -> set it as 0" << endl;
        h->SetBinContent(i_bin, 0);
      }
    }
  }

  TH1D* Init_MergedHistogram(TString histName, vector<TString> vec_processType, Bool_t isMC)
  {
    TH1D* h = nullptr;
    for(auto processType : vec_processType)
    {
      TH1D* h_temp = Init_Histogram( histName, processType, isMC );
      

      if( h == nullptr ) { h = (TH1D*)h_temp->Clone(); h->Sumw2(); }
      else               h->Add( h_temp );
    }

    return h;
  }

};


}; // -- end of namespace DYTool