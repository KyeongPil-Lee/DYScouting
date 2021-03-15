#include <vector>
#include <TObjString.h>
#include <Include/DYTool.h>

Bool_t CompareMuon_Pt( DYTool::Muon mu1, DYTool::Muon mu2 )
{
  return mu1.pt > mu2.pt; 
}

// -- reference: https://root.cern/doc/master/copytree3_8C.html
// -- reference2: https://root-forum.cern.ch/t/skim-events-from-a-tchain-into-a-new-file/9353
class SingleTreeSkimmer: public DYTool::ClassTemplate
{
public:
  SingleTreeSkimmer(): ClassTemplate()
  {

  }

  void Run()
  {
    CheckSampleInfo();
    StartTimer();

    vector<TString> vec_ntuplePath = GetVector_NtuplePath(sampleInfo_.ntuplePathFile);

    if( vec_ntuplePath.size() != 1 )
    {
      cout << "More than 1 root file is provided: This code only works for single input root file" << endl;
      return;
    }

    TString ntuplePath = vec_ntuplePath[0];
    TString outputPath = Get_OutputROOTFilePath(ntuplePath);

    cout << "input ntuple path:  " << ntuplePath << endl;
    cout << "output ntuple path: " << outputPath << endl;

    TChain *chain = new TChain("DYTree/ntuple");
    chain->Add(ntuplePath);
    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );

    // -- output TFile should be decleared before skimmedTree
    TFile *f_output = TFile::Open(outputPath, "RECREATE");
    f_output->cd();
    f_output->mkdir("DYTree");
    f_output->cd("DYTree");
    chain->LoadTree(0); // -- force 1st tree to be loaded
    TTree *skimmedTree = chain->GetTree()->CloneTree(0);

    Int_t nEvent = chain->GetEntries();
    for(Int_t i_ev=0; i_ev<nEvent; i_ev++)
    {
      ntuple->GetEvent(i_ev);

      if( PassSkimCondition(ntuple) ) skimmedTree->Fill();
    }

    printf("[# events] (input, output) = (%d, %lld)\n", nEvent, skimmedTree->GetEntries());

    skimmedTree->Write();
    f_output->Close();

    PrintRunTime();
  }

private:
  vector<TString> GetVector_NtuplePath(TString textFileName)
  {
    vector<TString> vec_ntuplePath;
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
        TString temp = line.data();
        vec_ntuplePath.push_back(temp);
      }
      openFile.close();
    }

    return vec_ntuplePath;
  }

  TString Get_OutputROOTFilePath(TString ntuplePath)
  {
    TObjArray *tokens = ntuplePath.Tokenize("/");
    TString ntupleName = ((TObjString*)tokens->Last())->GetString();
    // cout << "ntupleName = " << ntupleName << endl;
    TString dirPath = ntuplePath.ReplaceAll(ntupleName, "");
    // cout << "dirPath = " << dirPath << endl;

    TString outputPath = dirPath + "skim";
    if( gSystem->mkdir(outputPath, kTRUE) != -1 )
    {      
      gSystem->mkdir(outputPath, kTRUE);
      cout << "Directory is made: " << outputPath << endl;
    }

    TString outputNtupleName = ntupleName;
    outputNtupleName.ReplaceAll(".root", "_skim.root");

    outputPath = outputPath + "/" + outputNtupleName;

    return outputPath;
  }

  Bool_t PassSkimCondition(DYTool::DYTree* ntuple)
  {

    // -- L1 requirement is not added here to use same condition for 2017 and 2018 data
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

    // if( !doPassTrig )
    // {
    //   cout << "[" << i_ev << "th event] The scouting path is not fired. The full list of fired trigger is:" << endl;
    //   for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    //   {
    //     TString tstr_firedTrig = firedTrigger;
    //     cout << "  " << tstr_firedTrig << endl;
    //   }

    //   cout << endl;
    // }

    Bool_t haveTwoMuons = kFALSE;
    vector<DYTool::Muon> vec_muon = DYTool::GetAllMuons(ntuple);
    std::sort(vec_muon.begin(), vec_muon.end(), CompareMuon_Pt);
    if( vec_muon.size() >= 2 )
    {
      if( vec_muon[1].pt > 4.5 )
        haveTwoMuons = kTRUE; // -- at least 2 muons should have pT > 4.5 GeV
    }

    // cout << "[" << i_ev << "th event] (doPassTrig, haveTwoMuons) = (" << doPassTrig << ", " << haveTwoMuons << ")" << endl;

    return doPassTrig && haveTwoMuons;
  }

};

void Test()
{
  SingleTreeSkimmer* producer = new SingleTreeSkimmer();

  producer->sampleInfo_.type = "ScoutingCaloMuon_Run2018Av1_GoldenJSON_TEST";
  producer->sampleInfo_.ntuplePathFile = "/Users/kplee/Research/Analysis/DYScouting/Analyzer/TreeSkim/ntupleList_test.txt";
  producer->sampleInfo_.isMC = kFALSE;
  producer->sampleInfo_.xSec = -1;
  producer->sampleInfo_.sumWeight = -1;
  Double_t lumi = -1;
  producer->sampleInfo_.normFactor = -1;

  producer->Run();
}