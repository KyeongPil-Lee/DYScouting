#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TEfficiency.h>

#include <vector>

#include <Include/DYTool.h>
#include <BDT/Trial/v06_noOS/dataset/weights/TMVAClassification_BDTG.class.C>
#include <NLOReweight/NLOWeightTool.h>
#include <ControlPlot/HistContainer.h>

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

    // -- cut-flow histogram setup
    Int_t nCut = 7;
    TH1D* h_nEvent_cutFlow = new TH1D("h_nEvent_cutFlow", "", nCut, 0, nCut);

    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");
    NLOWeightTool* nloWeightTool = new NLOWeightTool();

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
        // -- DY low mass sample: apply NLO/LO k-factor
        if( sampleInfo_.type.Contains("DYMuMu_M10to50"))
          totWeight *= nloWeightTool->GetWeight(ntuple);

        if( IsDenominator(ntuple) )
        {
          h_nEvent_cutFlow->Fill(0.5, totWeight); // -- 0.5 = 1st bin

          if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) )
          {
            h_nEvent_cutFlow->Fill(1.5, totWeight); // -- 1.5 = 2nd bin

            if( IsHLTFired(ntuple) )
            {
              h_nEvent_cutFlow->Fill(2.5, totWeight); // -- 2.5 = 3rd bin

              // -- dimuon selection
              vector< DYTool::MuPair > vec_allMuPair = DYTool::GetAllMuPairs(ntuple);

              if( AtLeastOnePair_PassAcc(vec_allMuPair)  )
              {
                h_nEvent_cutFlow->Fill(3.5, totWeight); // -- 3.5 = 4th bin

                if( AtLeastOnePair_PassAccGoodMuPair(vec_allMuPair) )
                {
                  h_nEvent_cutFlow->Fill(4.5, totWeight); // -- 4.5 = 5th bin

                  DYTool::MuPair DYMuPair;
                  if( AtLeastOnePair_PassAccGoodMuPairVertex(vec_allMuPair, ntuple, DYMuPair) )
                  {
                    h_nEvent_cutFlow->Fill(5.5, totWeight); // -- 5.5 = 6th bin

                    vector<Double_t> vec_BDTInputVar = 
                    {
                      DYMuPair.normVtxChi2, DYMuPair.dEta, DYMuPair.dPhi,
                      ntuple->caloMET_pt,
                      (Double_t)DYMuPair.first_.nMuonHit, (Double_t)DYMuPair.first_.nMatchedStation, (Double_t)DYMuPair.first_.nPixelHit, (Double_t)DYMuPair.first_.nTrackerLayer, DYMuPair.first_.normChi2, DYMuPair.first_.relTrkIso,
                      (Double_t)DYMuPair.second_.nMuonHit, (Double_t)DYMuPair.second_.nMatchedStation, (Double_t)DYMuPair.second_.nPixelHit, (Double_t)DYMuPair.second_.nTrackerLayer, DYMuPair.second_.normChi2, DYMuPair.second_.relTrkIso
                    };

                    Double_t mvaValue = BDTClassifier->GetMvaValue( vec_BDTInputVar );
                    Double_t WP = 0.466016; // -- 78% signal efficiency with 90% background rejection

                    if( mvaValue > WP )
                    {
                      h_nEvent_cutFlow->Fill(6.5, totWeight); // -- 6.5 = 7th bin
                    } // -- BDT

                  } // -- common vertex

                } // -- good muon pair

              } // -- acceptance

            } // -- HLT

          } // -- L1

        } // -- isDenominator()

      } // -- sample type

    } // -- event iteration

    TString outputName = TString::Format("ROOTFile_MakeHist_Dimuon_%s.root", sampleInfo_.type.Data());
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    h_nEvent_cutFlow->Write();

    f_output->Close();

    PrintRunTime();
  }

private:

  // -- acceptance & within mass range at the generator level
  Bool_t IsDenominator(DYTool::DYTree* ntuple)
  {
    Bool_t doPass = kFALSE;

    vector<DYTool::GenParticle> vec_genMuon = DYTool::GetAllGenLeptons(ntuple, 13, "fromHardProcessFinalState");
    if( vec_genMuon.size() != 2) return kFALSE;

    if( vec_genMuon[0].pt > 5 && fabs(vec_genMuon[0].eta) < 2.4 && 
        vec_genMuon[1].pt > 5 && fabs(vec_genMuon[1].eta) < 2.4  )
    {
      Double_t diMuM = (vec_genMuon[0].vecP + vec_genMuon[1].vecP).M();

      if( 10.0 < diMuM && diMuM < 60.0)
        doPass = kTRUE;
    }

    return doPass;
  }

  Bool_t IsHLTFired(DYTool::DYTree* ntuple)
  {
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

    return doPassTrig;
  }

  Bool_t AtLeastOnePair_PassAcc(vector< DYTool::MuPair > vec_allMuPair)
  {
    Bool_t flag = kFALSE;

    for(auto& muPair : vec_allMuPair )
    {
      if( DoPassAcc(muPair) )
      {
        flag = kTRUE;
        break;
      }
    }

    return flag;
  }

  Bool_t AtLeastOnePair_PassAccGoodMuPair(vector< DYTool::MuPair > vec_allMuPair)
  {
    Bool_t flag = kFALSE;

    for(auto& muPair : vec_allMuPair )
    {
      if( DoPassAcc(muPair) && IsGoodMuonPair(muPair) )
      {
        flag = kTRUE;
        break;
      }
    }

    return flag;
  }

  Bool_t AtLeastOnePair_PassAccGoodMuPairVertex(vector< DYTool::MuPair > vec_allMuPair, DYTool::DYTree *ntuple, DYTool::MuPair& selectedPair)
  {
    Bool_t flag = kFALSE;

    vector<DYTool::MuPair> vec_selectedPair;
    for(auto& muPair : vec_allMuPair )
    {
      if( DoPassAcc(muPair) && IsGoodMuonPair(muPair) )
      {
        muPair.CheckVertex(ntuple);

        if( muPair.hasVertex )
          vec_selectedPair.push_back( muPair );
      }
    }

    if( vec_selectedPair.size() > 0 )
    {
      flag = kTRUE;

      std::sort(vec_selectedPair.begin(), vec_selectedPair.end(), DYTool::CompareMuonPair_SmallerVtxChi2);
      selectedPair = vec_selectedPair[0]; // -- pair with smallest vertex chi2 value
    }

    return flag;
  }

  Bool_t DoPassAcc(DYTool::MuPair& muPair)
  {
    Bool_t flag = kFALSE;

    if( muPair.first_.pt > 5  && fabs(muPair.first_.eta) < 2.4 &&
        muPair.second_.pt > 5 && fabs(muPair.second_.eta) < 2.4 &&
        10.0 < muPair.mass && muPair.mass < 60.0 )
      flag = kTRUE;

    return flag;
  }

  Bool_t IsGoodMuonPair(DYTool::MuPair& muPair)
  {
    Bool_t isGoodMuon = kFALSE;
    if( muPair.first_.nPixelHit > 0  && muPair.first_.nTrackerLayer > 5  && muPair.first_.normChi2 < 10  && muPair.first_.relTrkIso < 0.15 &&
        muPair.second_.nPixelHit > 0 && muPair.second_.nTrackerLayer > 5 && muPair.second_.normChi2 < 10 && muPair.second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    return isGoodMuon;
  }

};

void Test()
{
  HistProducer* producer = new HistProducer();

  producer->sampleInfo_.type = "DYMuMu_M10to50_NLOXSec_TEST";
  TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
  producer->sampleInfo_.ntuplePathFile = analyzerPath+"/Include/Example/ntuplePath_example.txt";
  producer->sampleInfo_.xSec = 10000;
  producer->sampleInfo_.sumWeight = 10000;
  Double_t lumi = 999;
  producer->sampleInfo_.normFactor = (lumi * 10000.0 ) / 10000.0;

  producer->Run();
}