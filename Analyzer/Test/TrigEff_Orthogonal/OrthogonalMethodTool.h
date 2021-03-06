#include <Include/DYTool.h>

DYTool::OffMuPair OffEventSelection_EGamma(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  DYTool::OffMuPair muPair_dummy;

  Bool_t doPassTrig = kFALSE;
  for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
  {
    TString tstr_firedTrig = firedTrigger;
    if( tstr_firedTrig.Contains("HLT_Ele32_WPTight_Gsf_v") || // -- collection of unprescaled trigger in EGamma dataset
        tstr_firedTrig.Contains("HLT_Ele115_CaloIdVT_GsfTrkIdT") ||
        tstr_firedTrig.Contains("HLT_Photon200") ||
        tstr_firedTrig.Contains("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL") ||
        tstr_firedTrig.Contains("HLT_DoubleEle25_CaloIdL_MW") ||
        tstr_firedTrig.Contains("HLT_DoublePhoton70") )
    {
      doPassTrig = kTRUE;
      break;
    }
  }

  // cout << "doPassTrig" << doPassTrig << endl;

  if( !doPassTrig ) return muPair_dummy;

  vector< DYTool::OffMuPair > vec_muPair = DYTool::GetAllOffMuPairs(ntuple);

  // cout << "vec_muPair.size() = " << vec_muPair.size() << endl; 

  // -- medium ID + loose Isolation (as the efficiency of this combination is provided by Muon POG & enlarge statistics)
  vector< DYTool::OffMuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
  {
    if( muPair.first_.pt > 15               && muPair.second_.pt > 15 &&
        fabs(muPair.first_.eta) < 2.4       && fabs(muPair.second_.eta) < 2.4 &&
        muPair.first_.isMedium              && muPair.second_.isMedium &&
        muPair.first_.relPFIso_dBeta < 0.25 && muPair.second_.relPFIso_dBeta < 0.25 ) // -- loose PF isolation
    {
      vec_goodMuPair.push_back( muPair );
      // cout << "Find 1 pair passing condition" << endl;
    }

  }

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

DYTool::OffMuPair OffEventSelection_EGamma_TightIDISO(DYTool::DYTree *ntuple, Bool_t& doPass)
{
  doPass = kFALSE;

  DYTool::OffMuPair muPair_dummy;

  Bool_t doPassTrig = kFALSE;
  for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
  {
    TString tstr_firedTrig = firedTrigger;
    if( tstr_firedTrig.Contains("HLT_Ele32_WPTight_Gsf_v") || // -- collection of unprescaled trigger in EGamma dataset
        tstr_firedTrig.Contains("HLT_Ele115_CaloIdVT_GsfTrkIdT") ||
        tstr_firedTrig.Contains("HLT_Photon200") ||
        tstr_firedTrig.Contains("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL") ||
        tstr_firedTrig.Contains("HLT_DoubleEle25_CaloIdL_MW") ||
        tstr_firedTrig.Contains("HLT_DoublePhoton70") )
    {
      doPassTrig = kTRUE;
      break;
    }
  }

  // cout << "doPassTrig" << doPassTrig << endl;

  if( !doPassTrig ) return muPair_dummy;

  vector< DYTool::OffMuPair > vec_muPair = DYTool::GetAllOffMuPairs(ntuple);

  // cout << "vec_muPair.size() = " << vec_muPair.size() << endl; 

  // -- medium ID + loose Isolation (as the efficiency of this combination is provided by Muon POG & enlarge statistics)
  vector< DYTool::OffMuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
  {
    if( muPair.first_.pt > 15               && muPair.second_.pt > 15 &&
        fabs(muPair.first_.eta) < 2.4       && fabs(muPair.second_.eta) < 2.4 &&
        muPair.first_.isTight               && muPair.second_.isTight &&
        muPair.first_.relPFIso_dBeta < 0.15 && muPair.second_.relPFIso_dBeta < 0.15 ) // -- tight PF isolation
    {
      vec_goodMuPair.push_back( muPair );
      // cout << "Find 1 pair passing condition" << endl;
    }

  }

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