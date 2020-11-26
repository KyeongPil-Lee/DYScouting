#include <Include/DYTool.h>
#include <BDT/Trial/v06_noOS/dataset/weights/TMVAClassification_BDTG.class.C>
#include <Test/ZPeakSF/ZPeakHistContainer.h>

class ZPeakMCHistProducer: public DYTool::ClassTemplate
{
public:
  ZPeakMCHistProducer(): ClassTemplate()
  {

  }

  void Run()
  {
    Setup();
    CheckSampleInfo();
    StartTimer();

    // -- define the histogram
    ZPeakHistContainer* hist = new ZPeakHistContainer("");

    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");

    vector<std::string> vec_inputVarName = {
      "diMuNormVtxChi2", "diMuDEta", "diMuDPhi", 
      "caloMET_pt", 
      "nMuonHit_lead", "nMatchedStation_lead", "nPixelHit_lead", "nTrackerLayer_lead", "normChi2_lead", "relTrkIso_lead", 
      "nMuonHit_sub", "nMatchedStation_sub", "nPixelHit_sub", "nTrackerLayer_sub", "normChi2_sub", "relTrkIso_sub"
    };

    ReadBDTG* BDTClassifier = new ReadBDTG(vec_inputVarName);

    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      // Double_t totWeight = sampleInfo_.normFactor * genWeight;
      Double_t totWeight = genWeight;
      if( sampleInfo_.isMC ) totWeight = genWeight * PUTool->Weight( ntuple->truePU );

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        Bool_t isDYEvent = kFALSE;
        DYTool::MuPair DYMuPair = DYTool::EventSelection_BDTInput_noOS(ntuple, isDYEvent);
        if( isDYEvent )
        {
          hist->Fill( DYMuPair, totWeight );

          // -- if BDT needs to be applied, comment "on" below lines
          // vector<Double_t> vec_BDTInputVar = {
          //   DYMuPair.normVtxChi2, DYMuPair.dEta, DYMuPair.dPhi,
          //   ntuple->caloMET_pt,
          //   (Double_t)DYMuPair.first_.nMuonHit, (Double_t)DYMuPair.first_.nMatchedStation, (Double_t)DYMuPair.first_.nPixelHit, (Double_t)DYMuPair.first_.nTrackerLayer, DYMuPair.first_.normChi2, DYMuPair.first_.relTrkIso,
          //   (Double_t)DYMuPair.second_.nMuonHit, (Double_t)DYMuPair.second_.nMatchedStation, (Double_t)DYMuPair.second_.nPixelHit, (Double_t)DYMuPair.second_.nTrackerLayer, DYMuPair.second_.normChi2, DYMuPair.second_.relTrkIso
          // };

          // Double_t mvaValue = BDTClassifier->GetMvaValue( vec_BDTInputVar );
          // Double_t WP = 0.466016; // -- 78% signal efficiency with 90% background rejection

          // if (mvaValue > WP )
          // {

          // }
        }
      } // -- sample type selection

    } // -- end of event iteration

    TString outputName = TString::Format("ROOTFile_ZPeakMCHistProducer_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    f_output->cd();

    hist->Write( f_output );

    f_output->Close();

    PrintRunTime();
  }

private:
};