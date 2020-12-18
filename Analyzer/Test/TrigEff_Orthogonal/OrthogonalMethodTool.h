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

  if( !doPassTrig ) return muPair_dummy;

  vector< DYTool::OffMuPair > vec_muPair = DYTool::GetAllOffMuPairs(ntuple);

  // -- medium ID + loose Isolation (as the efficiency of this combination is provided by Muon POG & enlarge statistics)
  vector< DYTool::OffMuPair > vec_goodMuPair;
  for( auto& muPair : vec_muPair )
  {
    if( muPair.first_.isMedium         && muPair.second_.isMedium &&
        muPair.first_.relPFIso_dBeta < 0.25 && muPair.second_.relPFIso_dBeta < 0.25 ) // -- loose PF isolation
      vec_goodMuPair.push_back( muPair );
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