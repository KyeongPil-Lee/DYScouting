#include <Include/DYTool.h>
#include <NLOReweight/NLOWeightTool.h>

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

    TH1D* h_nFilterObj       = new TH1D("h_nFilterObj",       "", 100, 0, 100);
    TH1D* h_nL3MuonNoVtx_pt3 = new TH1D("h_nL3MuonNoVtx_pt3", "", 100, 0, 100);

    TH1D* h_pt_filterObj   = new TH1D("h_pt_filterObj",   "", 10000, 0, 10000);
    TH1D* h_pt_L3MuonNoVtx = new TH1D("h_pt_L3MuonNoVtx", "", 10000, 0, 10000);

    TH1D* h_eta_filterObj   = new TH1D("h_eta_filterObj",   "", 60, -3, 3);
    TH1D* h_eta_L3MuonNoVtx = new TH1D("h_eta_L3MuonNoVtx", "", 60, -3, 3);

    TH1D* h_phi_filterObj   = new TH1D("h_phi_filterObj",   "", 80, -4, 4);
    TH1D* h_phi_L3MuonNoVtx = new TH1D("h_phi_L3MuonNoVtx", "", 80, -4, 4);

    Int_t i_event = 0;
    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      Double_t totWeight = sampleInfo_.normFactor * genWeight;
      // Double_t totWeight = genWeight;
      if( sampleInfo_.isMC ) totWeight *= PUTool->Weight( ntuple->truePU );

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        // -- DY low mass sample: apply NLO/LO k-factor
        if( sampleInfo_.type.Contains("DYMuMu_M10to50"))
          totWeight *= nloWeightTool->GetWeight(ntuple);

        vector<DYTool::GenParticle> vec_genMuon = DYTool::GetAllGenLeptons(ntuple, 13, "fromHardProcessFinalState");
        if( vec_genMuon.size() != 2 ) continue;

        // i_event++;

        // Bool_t isL1Fired  = IsFired_L1(ntuple);
        // Bool_t isHLTFired = IsFired_DoubleMu3(ntuple);
        // printf("[%d event] (L1, HLT) = (%d, %d)\n", i_event, isL1Fired, isHLTFired);
        // printf("  [1st gen-muon] (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)\n", vec_genMuon[0].pt, vec_genMuon[0].eta, vec_genMuon[0].phi);
        // printf("  [2nd gen-muon] (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)\n", vec_genMuon[1].pt, vec_genMuon[1].eta, vec_genMuon[1].phi);

        // vector<DYTool::HLTObj>      vec_filterObj       = DYTool::GetAllHLTObj(ntuple, "hltDoubleMu3L3FilteredNoVtx");
        // vector<DYTool::L3MuonNoVtx> vec_L3MuonNoVtx_pt3 = DYTool::GetAllL3MuonNoVtx(ntuple, 3.0);

        // vector<DYTool::L1Muon> vec_L1Muon = DYTool::GetAllL1Muon(ntuple, 8.0);
        // printf("[L1 muons]\n");
        // for(auto& L1Muon : vec_L1Muon )
        //   printf("  (pt, eta, phi, quality) = (%.1lf, %.3lf, %.3lf, %.0lf)\n", L1Muon.pt, L1Muon.eta, L1Muon.phi, L1Muon.quality);

        // printf("[L3MuonCandidateNoVtx object with pT > 3 GeV]\n");
        // for(auto& L3MuonNoVtx_pt3 : vec_L3MuonNoVtx_pt3 )
        //   printf("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)\n", L3MuonNoVtx_pt3.pt, L3MuonNoVtx_pt3.eta, L3MuonNoVtx_pt3.phi);

        // printf("[Filter(hltDoubleMu3L3FilteredNoVtx) object]\n");
        // for(auto& filterObj : vec_filterObj )
        //   printf("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)\n", filterObj.pt, filterObj.eta, filterObj.phi);

        // printf("\n");
        // if( i_event > 100 ) break;

        vector<DYTool::HLTObj> vec_filterObj_temp = DYTool::GetAllHLTObj(ntuple, "hltDoubleMu3L3FilteredNoVtx");
        vector<DYTool::HLTObj> vec_filterObj = RemoveDuplicatedObject( vec_filterObj_temp );

        vector<DYTool::L3MuonNoVtx> vec_L3MuonNoVtx_pt3;
        Bool_t doPass_customFilter = CustomFilter_DoubleMu3(ntuple, vec_L3MuonNoVtx_pt3);

        Int_t nFilterObj       = (Int_t)vec_filterObj.size();
        Int_t nL3MuonNoVtx_pt3 = (Int_t)vec_L3MuonNoVtx_pt3.size();

        h_nFilterObj->Fill( nFilterObj, totWeight );
        h_nL3MuonNoVtx_pt3->Fill( nL3MuonNoVtx_pt3, totWeight );

        for(auto& filterObj : vec_filterObj )
        {
          h_pt_filterObj->Fill( filterObj.pt, totWeight );
          h_eta_filterObj->Fill( filterObj.eta, totWeight );
          h_phi_filterObj->Fill( filterObj.phi, totWeight );
        }

        for(auto& L3MuonNoVtx : vec_L3MuonNoVtx_pt3 )
        {
          h_pt_L3MuonNoVtx->Fill( L3MuonNoVtx.pt, totWeight );
          h_eta_L3MuonNoVtx->Fill( L3MuonNoVtx.eta, totWeight );
          h_phi_L3MuonNoVtx->Fill( L3MuonNoVtx.phi, totWeight );
        }

        if( nFilterObj != nL3MuonNoVtx_pt3 )
        {
          TString basePath = "/data9/Users/kplee/Physics/DYScouting/Analyzer/EffCorr/Trigger/Validation/L3MuonNoVtx/Comp_FilterObj";
          ofstream logEvent(basePath+"/Event_differentNumberOfObject.txt", std::ios_base::out | std::ios_base::app);

          logEvent << "======================================" << endl;
          logEvent << "[The full L3MuonCandidateNoVtx object]" << endl;
          vector<DYTool::L3MuonNoVtx> vec_L3MuonNoVtx = DYTool::GetAllL3MuonNoVtx(ntuple, -1.0);
          for( L3MuonNoVtx : vec_L3MuonNoVtx )
            logEvent << TString::Format("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)", L3MuonNoVtx.pt, L3MuonNoVtx.eta, L3MuonNoVtx.phi) << endl;
          logEvent << endl;

          logEvent << "[Custom filter object (SingleMu3 + >=2 muons)]" << endl;
          for(auto& L3MuonNoVtx_pt3 : vec_L3MuonNoVtx_pt3 )
          {
            TString str_temp = TString::Format("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)", L3MuonNoVtx_pt3.pt, L3MuonNoVtx_pt3.eta, L3MuonNoVtx_pt3.phi);
            logEvent << str_temp << endl;
          }
          logEvent << endl;

          logEvent << "[Filter(hltDoubleMu3L3FilteredNoVtx) object]" << endl;
          for(auto& filterObj : vec_filterObj )
          {
            TString str_temp = TString::Format("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)", filterObj.pt, filterObj.eta, filterObj.phi);
            logEvent << str_temp << endl;
          }
          logEvent << "======================================" << endl;
          logEvent << endl;

          logEvent.close();
        }

      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("Comp_L3MuonNoVtx_FilterObj");
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    h_nFilterObj->Write();
    h_nL3MuonNoVtx_pt3->Write();

    h_pt_filterObj->Write();
    h_eta_filterObj->Write();
    h_phi_filterObj->Write();

    h_pt_L3MuonNoVtx->Write();
    h_eta_L3MuonNoVtx->Write();
    h_phi_L3MuonNoVtx->Write();

    f_output->Close();

    PrintRunTime();
  }

private:
  Bool_t IsFired_L1(DYTool::DYTree* ntuple)
  {
    Bool_t doPass_L1 = kFALSE;
    // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPass_L1 = kTRUE; // -- no L1 w/ mass cut
    if( ntuple->vec_L1Bit->at(4) ) doPass_L1 = kTRUE; // -- no L1 w/ mass cut

    return doPass_L1;
  }

  Bool_t IsFired_DoubleMu3(DYTool::DYTree* ntuple)
  {
    Bool_t doPass_HLT = kFALSE;
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("DST_DoubleMu3_noVtx_CaloScouting_v") ) doPass_HLT = kTRUE;

      if( doPass_HLT ) break;
    }

    return doPass_HLT;
  }

  Bool_t CustomFilter_DoubleMu3(DYTool::DYTree *ntuple, vector<DYTool::L3MuonNoVtx> &vec_filterObj)
  {
    Bool_t flag = kFALSE;

    vector<DYTool::L3MuonNoVtx> vec_filterObj_singleMu;
    Bool_t flag_singleMu = DYTool::CustomSingleMuFilter_MimicDoubleMu3Leg(ntuple, vec_filterObj_singleMu);

    if( vec_filterObj_singleMu.size() >= 2 )
    {
      flag = kTRUE;
      vec_filterObj = vec_filterObj_singleMu;
    }

    return flag;
  }

  vector<DYTool::HLTObj> RemoveDuplicatedObject( vector<DYTool::HLTObj> vec_obj )
  {
    vector<DYTool::HLTObj> vec_obj_cleaned;
    vec_obj_cleaned.clear();

    for( auto obj : vec_obj )
    {
      Double_t pt = obj.pt;
      Double_t eta = obj.eta;
      Double_t phi = obj.phi;

      Bool_t isDuplicated = kFALSE;
      for( auto obj_cleaned : vec_obj_cleaned )
      {
        if(  pt == obj_cleaned.pt && eta == obj_cleaned.eta && phi == obj_cleaned.phi )
        {
          isDuplicated = kTRUE;
          break;
        }
      }

      if( !isDuplicated )
        vec_obj_cleaned.push_back( obj );
    }

    return vec_obj_cleaned;
  }

};