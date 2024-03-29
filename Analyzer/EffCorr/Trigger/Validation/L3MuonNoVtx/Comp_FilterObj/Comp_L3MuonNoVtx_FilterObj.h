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

    TH1D* h_nFilterObj_tot   = new TH1D("h_nFilterObj_tot",   "", 100, 0, 100);
    TH1D* h_nFilterObj_type1 = new TH1D("h_nFilterObj_type1", "", 100, 0, 100);
    TH1D* h_nFilterObj_type2 = new TH1D("h_nFilterObj_type2", "", 100, 0, 100);
    TH1D* h_nFilterObj_type3 = new TH1D("h_nFilterObj_type3", "", 100, 0, 100);

    TH1D* h_nL3MuonNoVtx_tot   = new TH1D("h_nL3MuonNoVtx_tot",   "", 100, 0, 100);
    TH1D* h_nL3MuonNoVtx_type1 = new TH1D("h_nL3MuonNoVtx_type1", "", 100, 0, 100);
    TH1D* h_nL3MuonNoVtx_type2 = new TH1D("h_nL3MuonNoVtx_type2", "", 100, 0, 100);
    TH1D* h_nL3MuonNoVtx_type3 = new TH1D("h_nL3MuonNoVtx_type3", "", 100, 0, 100);

    // -- # events of its type according to the generator level variables
    // -- nFilterObj == nL3MuonNoVtx
    // -- nFilterObj > nL3MuonNoVtx: my custom filter rejects more
    // -- nFilterObj < nL3MuonNoVtx: my custom filter couldn't reject the muons
    TH1D* h_nEvent_diMuPt_tot   = new TH1D("h_nEvent_diMuPt_tot",   "", 10000, 0, 10000);
    TH1D* h_nEvent_diMuPt_type1 = new TH1D("h_nEvent_diMuPt_type1", "", 10000, 0, 10000);
    TH1D* h_nEvent_diMuPt_type2 = new TH1D("h_nEvent_diMuPt_type2", "", 10000, 0, 10000);
    TH1D* h_nEvent_diMuPt_type3 = new TH1D("h_nEvent_diMuPt_type3", "", 10000, 0, 10000);

    TH1D* h_nEvent_diMuM_tot   = new TH1D("h_nEvent_diMuM_tot",   "", 10000, 0, 10000);
    TH1D* h_nEvent_diMuM_type1 = new TH1D("h_nEvent_diMuM_type1", "", 10000, 0, 10000);
    TH1D* h_nEvent_diMuM_type2 = new TH1D("h_nEvent_diMuM_type2", "", 10000, 0, 10000);
    TH1D* h_nEvent_diMuM_type3 = new TH1D("h_nEvent_diMuM_type3", "", 10000, 0, 10000);

    TH1D* h_nEvent_diMuRap_tot   = new TH1D("h_nEvent_diMuRap_tot",   "", 200, -10, 10);
    TH1D* h_nEvent_diMuRap_type1 = new TH1D("h_nEvent_diMuRap_type1", "", 200, -10, 10);
    TH1D* h_nEvent_diMuRap_type2 = new TH1D("h_nEvent_diMuRap_type2", "", 200, -10, 10);
    TH1D* h_nEvent_diMuRap_type3 = new TH1D("h_nEvent_diMuRap_type3", "", 200, -10, 10);


    // -- histogram for tot
    TH1D* h_pt_filterObj_tot   = new TH1D("h_pt_filterObj_tot",   "", 10000, 0, 10000);
    TH1D* h_pt_L3MuonNoVtx_tot = new TH1D("h_pt_L3MuonNoVtx_tot", "", 10000, 0, 10000);

    TH1D* h_eta_filterObj_tot   = new TH1D("h_eta_filterObj_tot",   "", 60, -3, 3);
    TH1D* h_eta_L3MuonNoVtx_tot = new TH1D("h_eta_L3MuonNoVtx_tot", "", 60, -3, 3);

    TH1D* h_phi_filterObj_tot   = new TH1D("h_phi_filterObj_tot",   "", 80, -4, 4);
    TH1D* h_phi_L3MuonNoVtx_tot = new TH1D("h_phi_L3MuonNoVtx_tot", "", 80, -4, 4);

    // -- histogram for type1
    TH1D* h_pt_filterObj_type1   = new TH1D("h_pt_filterObj_type1",   "", 10000, 0, 10000);
    TH1D* h_pt_L3MuonNoVtx_type1 = new TH1D("h_pt_L3MuonNoVtx_type1", "", 10000, 0, 10000);

    TH1D* h_eta_filterObj_type1   = new TH1D("h_eta_filterObj_type1",   "", 60, -3, 3);
    TH1D* h_eta_L3MuonNoVtx_type1 = new TH1D("h_eta_L3MuonNoVtx_type1", "", 60, -3, 3);

    TH1D* h_phi_filterObj_type1   = new TH1D("h_phi_filterObj_type1",   "", 80, -4, 4);
    TH1D* h_phi_L3MuonNoVtx_type1 = new TH1D("h_phi_L3MuonNoVtx_type1", "", 80, -4, 4);

    // -- histogram for type2
    TH1D* h_pt_filterObj_type2   = new TH1D("h_pt_filterObj_type2",   "", 10000, 0, 10000);
    TH1D* h_pt_L3MuonNoVtx_type2 = new TH1D("h_pt_L3MuonNoVtx_type2", "", 10000, 0, 10000);

    TH1D* h_eta_filterObj_type2   = new TH1D("h_eta_filterObj_type2",   "", 60, -3, 3);
    TH1D* h_eta_L3MuonNoVtx_type2 = new TH1D("h_eta_L3MuonNoVtx_type2", "", 60, -3, 3);

    TH1D* h_phi_filterObj_type2   = new TH1D("h_phi_filterObj_type2",   "", 80, -4, 4);
    TH1D* h_phi_L3MuonNoVtx_type2 = new TH1D("h_phi_L3MuonNoVtx_type2", "", 80, -4, 4);

    // -- histogram for type3
    TH1D* h_pt_filterObj_type3   = new TH1D("h_pt_filterObj_type3",   "", 10000, 0, 10000);
    TH1D* h_pt_L3MuonNoVtx_type3 = new TH1D("h_pt_L3MuonNoVtx_type3", "", 10000, 0, 10000);

    TH1D* h_eta_filterObj_type3   = new TH1D("h_eta_filterObj_type3",   "", 60, -3, 3);
    TH1D* h_eta_L3MuonNoVtx_type3 = new TH1D("h_eta_L3MuonNoVtx_type3", "", 60, -3, 3);

    TH1D* h_phi_filterObj_type3   = new TH1D("h_phi_filterObj_type3",   "", 80, -4, 4);
    TH1D* h_phi_L3MuonNoVtx_type3 = new TH1D("h_phi_L3MuonNoVtx_type3", "", 80, -4, 4);

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

        // -- apply acceptance at the generator level: let's consider the events within the detector acceptance only
        Bool_t flag_passGenAcc = kFALSE;
        if( vec_genMuon[0].pt > 5 && fabs(vec_genMuon[0].eta) < 2.4 &&
            vec_genMuon[1].pt > 5 && fabs(vec_genMuon[1].eta) < 2.4 ) flag_passGenAcc = kTRUE;

        if( !flag_passGenAcc ) continue;

        TLorentzVector vecP_diMu_gen = vec_genMuon[0].vecP + vec_genMuon[1].vecP;

        Double_t diMuPt_gen  = vecP_diMu_gen.Pt();
        Double_t diMuM_gen   = vecP_diMu_gen.M();
        Double_t diMuRap_gen = vecP_diMu_gen.Rapidity();

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

        Int_t nFilterObj   = (Int_t)vec_filterObj.size();
        Int_t nL3MuonNoVtx = (Int_t)vec_L3MuonNoVtx_pt3.size();

        h_nEvent_diMuPt_tot->Fill( diMuPt_gen, totWeight );
        h_nEvent_diMuM_tot->Fill( diMuM_gen, totWeight );
        h_nEvent_diMuRap_tot->Fill( diMuRap_gen, totWeight );

        Bool_t isType1 = kFALSE;
        Bool_t isType2 = kFALSE;
        Bool_t isType3 = kFALSE;

        if( nFilterObj == nL3MuonNoVtx )      isType1 = kTRUE;
        else if( nFilterObj > nL3MuonNoVtx )  isType2 = kTRUE;
        else if( nFilterObj < nL3MuonNoVtx )  isType3 = kTRUE;

        h_nFilterObj_tot->Fill( nFilterObj, totWeight );
        h_nL3MuonNoVtx_tot->Fill( nL3MuonNoVtx, totWeight );

        if( isType1 )
        {
          h_nFilterObj_type1->Fill( nFilterObj, totWeight );
          h_nL3MuonNoVtx_type1->Fill( nL3MuonNoVtx, totWeight );
        }
        if( isType2 )
        {
          h_nFilterObj_type2->Fill( nFilterObj, totWeight );
          h_nL3MuonNoVtx_type2->Fill( nL3MuonNoVtx, totWeight );
        }
        if( isType3 )
        {
          h_nFilterObj_type3->Fill( nFilterObj, totWeight );
          h_nL3MuonNoVtx_type3->Fill( nL3MuonNoVtx, totWeight );
        }

        if( isType1 )
        {
          h_nEvent_diMuPt_type1->Fill( diMuPt_gen, totWeight );
          h_nEvent_diMuM_type1->Fill( diMuM_gen, totWeight );
          h_nEvent_diMuRap_type1->Fill( diMuRap_gen, totWeight );
        }
        if( isType2 )
        {
          h_nEvent_diMuPt_type2->Fill( diMuPt_gen, totWeight );
          h_nEvent_diMuM_type2->Fill( diMuM_gen, totWeight );
          h_nEvent_diMuRap_type2->Fill( diMuRap_gen, totWeight );
        }
        if( isType3 )
        {
          h_nEvent_diMuPt_type3->Fill( diMuPt_gen, totWeight );
          h_nEvent_diMuM_type3->Fill( diMuM_gen, totWeight );
          h_nEvent_diMuRap_type3->Fill( diMuRap_gen, totWeight );
        }

        // -- kinematics for the filter objects
        for(auto& filterObj : vec_filterObj )
        {
          h_pt_filterObj_tot->Fill( filterObj.pt, totWeight );
          h_eta_filterObj_tot->Fill( filterObj.eta, totWeight );
          h_phi_filterObj_tot->Fill( filterObj.phi, totWeight );

          if( isType1 )
          {
            h_pt_filterObj_type1->Fill( filterObj.pt, totWeight );
            h_eta_filterObj_type1->Fill( filterObj.eta, totWeight );
            h_phi_filterObj_type1->Fill( filterObj.phi, totWeight );
          }
          if( isType2 )
          {
            h_pt_filterObj_type2->Fill( filterObj.pt, totWeight );
            h_eta_filterObj_type2->Fill( filterObj.eta, totWeight );
            h_phi_filterObj_type2->Fill( filterObj.phi, totWeight );
          }
          if( isType3 )
          {
            h_pt_filterObj_type3->Fill( filterObj.pt, totWeight );
            h_eta_filterObj_type3->Fill( filterObj.eta, totWeight );
            h_phi_filterObj_type3->Fill( filterObj.phi, totWeight );
          }
        }

        for(auto& L3MuonNoVtx : vec_L3MuonNoVtx_pt3 )
        {
          h_pt_L3MuonNoVtx_tot->Fill( L3MuonNoVtx.pt, totWeight );
          h_eta_L3MuonNoVtx_tot->Fill( L3MuonNoVtx.eta, totWeight );
          h_phi_L3MuonNoVtx_tot->Fill( L3MuonNoVtx.phi, totWeight );

          if( isType1 )
          {
            h_pt_L3MuonNoVtx_type1->Fill( L3MuonNoVtx.pt, totWeight );
            h_eta_L3MuonNoVtx_type1->Fill( L3MuonNoVtx.eta, totWeight );
            h_phi_L3MuonNoVtx_type1->Fill( L3MuonNoVtx.phi, totWeight );
          }
          if( isType2 )
          {
            h_pt_L3MuonNoVtx_type2->Fill( L3MuonNoVtx.pt, totWeight );
            h_eta_L3MuonNoVtx_type2->Fill( L3MuonNoVtx.eta, totWeight );
            h_phi_L3MuonNoVtx_type2->Fill( L3MuonNoVtx.phi, totWeight );
          }
          if( isType3 )
          {
            h_pt_L3MuonNoVtx_type3->Fill( L3MuonNoVtx.pt, totWeight );
            h_eta_L3MuonNoVtx_type3->Fill( L3MuonNoVtx.eta, totWeight );
            h_phi_L3MuonNoVtx_type3->Fill( L3MuonNoVtx.phi, totWeight );
          }
        }


        // -- logging
        if( nFilterObj != nL3MuonNoVtx )
        {
          TString basePath = "/data9/Users/kplee/Physics/DYScouting/Analyzer/EffCorr/Trigger/Validation/L3MuonNoVtx/Comp_FilterObj";
          ofstream logEvent(basePath+"/Event_differentNumberOfObject.txt", std::ios_base::out | std::ios_base::app);

          logEvent << "======================================" << endl;
          logEvent << "[The full L3MuonCandidateNoVtx object]" << endl;
          vector<DYTool::L3MuonNoVtx> vec_L3MuonNoVtx = DYTool::GetAllL3MuonNoVtx(ntuple, -1.0);
          for( auto& L3MuonNoVtx : vec_L3MuonNoVtx )
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
          logEvent << endl;

          // -- when the custom filter reject more than the real filter: trace the rejected objects to understand the reason
          if( nFilterObj > nL3MuonNoVtx )
          {
            vector<DYTool::L3MuonNoVtx> vec_filterObj_singleMu_debug;
            Bool_t flag_singleMu = CustomSingleMuFilter_MimicDoubleMu3Leg_Debug(ntuple, vec_filterObj_singleMu_debug, logEvent);
          }

          logEvent << "======================================" << endl;
          logEvent << endl;

          logEvent.close();
        }

      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("Comp_L3MuonNoVtx_FilterObj");
    TFile *f_output = TFile::Open(outputName, "RECREATE");

    h_nFilterObj_tot->Write();
    h_nL3MuonNoVtx_tot->Write();

    h_nFilterObj_type1->Write();
    h_nL3MuonNoVtx_type1->Write();

    h_nFilterObj_type2->Write();
    h_nL3MuonNoVtx_type2->Write();

    h_nFilterObj_type3->Write();
    h_nL3MuonNoVtx_type3->Write();


    h_nEvent_diMuPt_tot->Write();
    h_nEvent_diMuPt_type1->Write();
    h_nEvent_diMuPt_type2->Write();
    h_nEvent_diMuPt_type3->Write();

    h_nEvent_diMuM_tot->Write();
    h_nEvent_diMuM_type1->Write();
    h_nEvent_diMuM_type2->Write();
    h_nEvent_diMuM_type3->Write();

    h_nEvent_diMuRap_tot->Write();
    h_nEvent_diMuRap_type1->Write();
    h_nEvent_diMuRap_type2->Write();
    h_nEvent_diMuRap_type3->Write();


    h_pt_filterObj_tot->Write();
    h_eta_filterObj_tot->Write();
    h_phi_filterObj_tot->Write();

    h_pt_filterObj_type1->Write();
    h_eta_filterObj_type1->Write();
    h_phi_filterObj_type1->Write();

    h_pt_filterObj_type2->Write();
    h_eta_filterObj_type2->Write();
    h_phi_filterObj_type2->Write();

    h_pt_filterObj_type3->Write();
    h_eta_filterObj_type3->Write();
    h_phi_filterObj_type3->Write();


    h_pt_L3MuonNoVtx_tot->Write();
    h_eta_L3MuonNoVtx_tot->Write();
    h_phi_L3MuonNoVtx_tot->Write();

    h_pt_L3MuonNoVtx_type1->Write();
    h_eta_L3MuonNoVtx_type1->Write();
    h_phi_L3MuonNoVtx_type1->Write();

    h_pt_L3MuonNoVtx_type2->Write();
    h_eta_L3MuonNoVtx_type2->Write();
    h_phi_L3MuonNoVtx_type2->Write();

    h_pt_L3MuonNoVtx_type3->Write();
    h_eta_L3MuonNoVtx_type3->Write();
    h_phi_L3MuonNoVtx_type3->Write();

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

  Bool_t CustomSingleMuFilter_MimicDoubleMu3Leg_Debug(DYTool::DYTree *ntuple, vector<DYTool::L3MuonNoVtx> &vec_filterObj, ofstream& logEvent)
  {
    logEvent << "[CustomSingleMuFilter_MimicDoubleMu3Leg_Debug]" << endl;

    vec_filterObj.clear();
    
    Bool_t flag = kFALSE;

    vector<DYTool::L3MuonNoVtx> vec_L3MuonNoVtx = DYTool::GetAllL3MuonNoVtx(ntuple, -1.0);
    for( auto& L3MuonNoVtx : vec_L3MuonNoVtx )
    {
      Bool_t isMatched_L1 = dRMatching_HLTObj_Debug(L3MuonNoVtx.vecP, ntuple, "hltDimuon3L1Filtered0",    0.3, logEvent);
      Bool_t isMatched_L2 = dRMatching_HLTObj_Debug(L3MuonNoVtx.vecP, ntuple, "hltDimuon3L2PreFiltered0", 0.5, logEvent);

      Bool_t isMatched_prevCand = isMatched_L1 || isMatched_L2;

      Bool_t doPass_PtCut = L3MuonNoVtx.pt > 3.0;

      if( isMatched_prevCand && doPass_PtCut ) vec_filterObj.push_back( L3MuonNoVtx );

      logEvent << TString::Format("  (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)", L3MuonNoVtx.pt, L3MuonNoVtx.eta, L3MuonNoVtx.phi) << endl;
      Bool_t flag_pass = isMatched_prevCand && doPass_PtCut;
      if( flag_pass )
        logEvent << "   ---> pass" << endl;
      else
        logEvent << "   ---> fail: " << TString::Format("(isMatched_L1, isMatched_L2, doPass_PtCut) = (%d, %d, %d)", isMatched_L1, isMatched_L2, doPass_PtCut) << endl;
    }
    logEvent << endl;

    if( vec_filterObj.size() >= 1 ) flag = kTRUE;

    return flag;
  }

  Bool_t dRMatching_HLTObj_Debug( TLorentzVector vecP_ref, DYTool::DYTree* ntuple, TString filterName, Double_t minDR, ofstream& logEvent )
  {
    logEvent << "   [dRMatching_HLTObj_Debug] filterName = " << filterName << endl;
    logEvent << TString::Format("   reference momentum (pt, eta, phi) = (%.1lf, %.3lf, %.3lf)", vecP_ref.Pt(), vecP_ref.Eta(), vecP_ref.Phi()) << endl;

    vector<DYTool::HLTObj> vec_HLTObj = DYTool::GetAllHLTObj(ntuple, filterName);
    vector<TLorentzVector> vec_vecP_HLTObj;
    for(const auto& HLTObj : vec_HLTObj )
      vec_vecP_HLTObj.push_back( HLTObj.vecP );

    return dRMatching_Debug( vecP_ref, vec_vecP_HLTObj, minDR, logEvent );
  }

  Bool_t dRMatching_Debug( TLorentzVector vecP_ref, vector<TLorentzVector> vec_vecP, Double_t minDR, ofstream& logEvent )
  {
    bool flag = kFALSE;

    Int_t nObj = (Int_t)vec_vecP.size();
    for(const auto& vecP_target: vec_vecP )
    {
      Double_t dR = vecP_ref.DeltaR( vecP_target );

      logEvent << TString::Format("      -> dR with (pt, eta, phi) = (%.1lf, %.3lf, %.3lf): %.3lf", vecP_target.Pt(), vecP_target.Eta(), vecP_target.Phi(), dR) << endl;

      if( dR < minDR )
      {
        flag = kTRUE;
        break;
      }
    }

    return flag;
  }

};