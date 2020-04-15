#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TString.h>
#include <TColor.h>

#include "DYTool.h"
#include "TMVAClassification_BDTG.class.C" // -- for BDT classification

class HistProducer
{
public:
  TString dataType_;
  TString ntuplePath_;
  TString isMC_;

  HistProducer( TString dataType, TString ntuplePath, Bool_t isMC )
  {
    dataType_   = dataType;
    ntuplePath_ = ntuplePath;
    isMC_ = isMC;
  }

  void Produce()
  {
    // -- histogram definition
    TH1D* h_diMuM = new TH1D("h_diMuM", "", 50, 10, 60);

    // -- setup the ntuple
    TChain *chain = new TChain("DYTree/ntuple");
    chain->Add( ntuplePath_ );

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );

    // -- setup for PU weighting tool
    TString fileName_PUReweight = "ROOTFile_PUReweighting_2018.root";
    TString histName_PUReweight = "h_ratio";
    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool(fileName_PUReweight, histName_PUReweight);

    // -- setup for BDT classification
    vector<std::string> vec_inputVarName = {
      "diMuNormVtxChi2", "diMuDEta", "diMuDPhi", 
      "caloMET_pt", 
      "nMuonHit_lead", "nMatchedStation_lead", "nPixelHit_lead", "nTrackerLayer_lead", "normChi2_lead", "relTrkIso_lead", 
      "nMuonHit_sub", "nMatchedStation_sub", "nPixelHit_sub", "nTrackerLayer_sub", "normChi2_sub", "relTrkIso_sub"
    };

    ReadBDTG* BDTClassifier = new ReadBDTG(vec_inputVarName);

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    for(Int_t i=0; i<nEvent; i++)
    {
      // -- print progress bar
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      // -- event weight
      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      Double_t totWeight = genWeight;
      if( isMC_ ) totWeight = genWeight * PUTool->Weight( ntuple->truePU );

      // -- if it is DY sample (DYMuMu_*): separating DY->mumu events from DY->ll events by checking the generator information
      // -- else: return 1
      // -- The function is defined in DYTool.h
      if( DYTool::SelectGenEventBySampleType(dataType_, ntuple) )
      {
        // -- select events passing preselection used for BDT training
        Bool_t passPreSel = kFALSE;
        DYTool::MuPair DYMuPair = DYTool::EventSelection_BDTInput_noOS(ntuple, passPreSel);
        if( passPreSel )
        {
          // -- BDT classification

          // -- input variables
          vector<Double_t> vec_BDTInputVar = {
            DYMuPair.normVtxChi2, DYMuPair.dEta, DYMuPair.dPhi,
            ntuple->caloMET_pt,
            (Double_t)DYMuPair.first_.nMuonHit, (Double_t)DYMuPair.first_.nMatchedStation, (Double_t)DYMuPair.first_.nPixelHit, (Double_t)DYMuPair.first_.nTrackerLayer, DYMuPair.first_.normChi2, DYMuPair.first_.relTrkIso,
            (Double_t)DYMuPair.second_.nMuonHit, (Double_t)DYMuPair.second_.nMatchedStation, (Double_t)DYMuPair.second_.nPixelHit, (Double_t)DYMuPair.second_.nTrackerLayer, DYMuPair.second_.normChi2, DYMuPair.second_.relTrkIso
          };

          Double_t mvaValue = BDTClassifier->GetMvaValue( vec_BDTInputVar );
          Double_t WP = 0.466016; // -- 78% signal efficiency with 90% background rejection

          // -- event passing BDT classification
          if( mvaValue > WP )
          {
            if( DYMuPair.isOS ) // -- signal region: pass BDT classificaiton + opposite sign requirement (same sign: used for control region in background estimation)
            {
              h_diMuM->Fill( DYMuPair.mass, totWeight );
            }
          } // -- end of BDT classification

        } // -- end of preselection

      } // -- end of DYTool::SelectGenEventBySampleType

    } // -- end of event iteration

    TFile *f_output = TFile::Open("ROOTFile_DimuonMass.root", "RECREATE");
    f_output->cd();
    h_diMuM->Write();
    f_output->Close();
    cout << "Output file is created: " << f_output->GetName() << endl;

  }
private:

};

void ExampleAnalyzer()
{
  TString dataType = "DYMuMu_M10to50";
  TString ntuplePath = "/Users/KyeongPil_Lee/Physics/DYScouting/Analyzer/Include/Example/ntuple_MC_DY_M10to50_v1p2.root";
  Bool_t isMC = kTRUE;

  HistProducer* producer = new HistProducer(dataType, ntuplePath, isMC);
  producer->Produce();
}