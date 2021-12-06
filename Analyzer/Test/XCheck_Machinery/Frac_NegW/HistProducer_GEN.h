// -- compare the distributions
// ---- 1) select the events with only exact two muons
// ---- 2) select the dimuon candidate with best vertex chi2

#include <Include/DYTool.h>

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

    // -- define the histogram
    TH1D* h_mu_pt_posW  = new TH1D("h_mu_pt_posW",  "", 10000, 0, 10000);
    TH1D* h_mu_eta_posW = new TH1D("h_mu_eta_posW", "", 200, -10, 10);
    TH1D* h_mu_phi_posW = new TH1D("h_mu_phi_posW", "", 80, -4, 4);
    TH1D* h_diMu_mass_posW = new TH1D("h_diMu_mass_posW", "", 10000, 0, 10000);
    TH1D* h_diMu_pt_posW   = new TH1D("h_diMu_pt_posW",   "", 10000, 0, 10000);
    TH1D* h_diMu_rap_posW  = new TH1D("h_diMu_rap_posW",  "", 200, -10, 10);

    TH1D* h_mu_pt_negW  = new TH1D("h_mu_pt_negW",  "", 10000, 0, 10000);
    TH1D* h_mu_eta_negW = new TH1D("h_mu_eta_negW", "", 200, -10, 10);
    TH1D* h_mu_phi_negW = new TH1D("h_mu_phi_negW", "", 80, -4, 4);
    TH1D* h_diMu_mass_negW = new TH1D("h_diMu_mass_negW", "", 10000, 0, 10000);
    TH1D* h_diMu_pt_negW   = new TH1D("h_diMu_pt_negW",   "", 10000, 0, 10000);
    TH1D* h_diMu_rap_negW  = new TH1D("h_diMu_rap_negW",  "", 200, -10, 10);

    TH1D* h_mu_pt_allW  = new TH1D("h_mu_pt_allW",  "", 10000, 0, 10000);
    TH1D* h_mu_eta_allW = new TH1D("h_mu_eta_allW", "", 200, -10, 10);
    TH1D* h_mu_phi_allW = new TH1D("h_mu_phi_allW", "", 80, -4, 4);
    TH1D* h_diMu_mass_allW = new TH1D("h_diMu_mass_allW", "", 10000, 0, 10000);
    TH1D* h_diMu_pt_allW   = new TH1D("h_diMu_pt_allW",   "", 10000, 0, 10000);
    TH1D* h_diMu_rap_allW  = new TH1D("h_diMu_rap_allW",  "", 200, -10, 10);

    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );
    // ntuple->TurnOnBranches_OffMuon();

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      // -- do not normalize to the integrated luminosity; just pure statistics
      // Double_t totWeight;
      // if( sampleInfo_.isMC ) totWeight = genWeight;
      // else                   totWeight = 1.0;

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        vector<DYTool::GenParticle> vec_genLepton = DYTool::GetAllGenLeptons(ntuple, 13, "fromHardProcessFinalState");

        for( auto& genLepton: vec_genLepton ) {
          TLorentzVector vecP_diMu = vec_genLepton[0].vecP + vec_genLepton[1].vecP;

          h_mu_pt_allW->Fill( vec_genLepton[0].pt, genWeight );
          h_mu_eta_allW->Fill( vec_genLepton[0].eta, genWeight );
          h_mu_phi_allW->Fill( vec_genLepton[0].phi, genWeight );
          h_mu_pt_allW->Fill( vec_genLepton[1].pt, genWeight );
          h_mu_eta_allW->Fill( vec_genLepton[1].eta, genWeight );
          h_mu_phi_allW->Fill( vec_genLepton[1].phi, genWeight );

          h_diMu_mass_allW->Fill( vecP_diMu.M(), genWeight );
          h_diMu_pt_allW->Fill( vecP_diMu.Pt(), genWeight );
          h_diMu_rap_allW->Fill( vecP_diMu.Rapidity(), genWeight );

          if( genWeight > 0 ) {
            h_mu_pt_posW->Fill( vec_genLepton[0].pt, genWeight );
            h_mu_eta_posW->Fill( vec_genLepton[0].eta, genWeight );
            h_mu_phi_posW->Fill( vec_genLepton[0].phi, genWeight );
            h_mu_pt_posW->Fill( vec_genLepton[1].pt, genWeight );
            h_mu_eta_posW->Fill( vec_genLepton[1].eta, genWeight );
            h_mu_phi_posW->Fill( vec_genLepton[1].phi, genWeight );

            h_diMu_mass_posW->Fill( vecP_diMu.M(), genWeight );
            h_diMu_pt_posW->Fill( vecP_diMu.Pt(), genWeight );
            h_diMu_rap_posW->Fill( vecP_diMu.Rapidity(), genWeight );
          }
          else {
            h_mu_pt_negW->Fill( vec_genLepton[0].pt, genWeight );
            h_mu_eta_negW->Fill( vec_genLepton[0].eta, genWeight );
            h_mu_phi_negW->Fill( vec_genLepton[0].phi, genWeight );
            h_mu_pt_negW->Fill( vec_genLepton[1].pt, genWeight );
            h_mu_eta_negW->Fill( vec_genLepton[1].eta, genWeight );
            h_mu_phi_negW->Fill( vec_genLepton[1].phi, genWeight );

            h_diMu_mass_negW->Fill( vecP_diMu.M(), genWeight );
            h_diMu_pt_negW->Fill( vecP_diMu.Pt(), genWeight );
            h_diMu_rap_negW->Fill( vecP_diMu.Rapidity(), genWeight );
          }
        } // -- end of loop over genLepton

      } // -- sample type selection

    } // -- end of event iteration

    TString outputName = TString::Format("ROOTFile_HistProducer_GEN_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    f_output->cd();

    h_mu_pt_allW->Write();    
    h_mu_eta_allW->Write();
    h_mu_phi_allW->Write();
    h_diMu_mass_allW->Write();
    h_diMu_pt_allW->Write();
    h_diMu_rap_allW->Write();

    h_mu_pt_posW->Write();    
    h_mu_eta_posW->Write();
    h_mu_phi_posW->Write();
    h_diMu_mass_posW->Write();
    h_diMu_pt_posW->Write();
    h_diMu_rap_posW->Write();

    h_mu_pt_negW->Write();    
    h_mu_eta_negW->Write();
    h_mu_phi_negW->Write();
    h_diMu_mass_negW->Write();
    h_diMu_pt_negW->Write();
    h_diMu_rap_negW->Write();

    f_output->Close();

    PrintRunTime();
  }

private:

};