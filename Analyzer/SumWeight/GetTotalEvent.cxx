void GetTotalEvent()
{
  vector<TString> vec_sample = {
    "DYJetsToLL_M10to50_Madgraph_ext", "QCDMuEnriched_Pt20toInf", "TTTo2L2Nu_Powheg_Incompleted", "WJetsToLNu_Madgraph"
  };

  TString basePath = "/scratch/kplee/DYScoutingTree/v1.0";

  for(auto& sample : vec_sample )
  {

    cout << "Sample = " << sample << endl;

    TChain *chain = new TChain("DYTree/ntuple");
    chain->Add(basePath+"/"+sample+"/*.root");

    Int_t nEvent = chain->GetEntries();
    cout << "Total number of events: " << nEvent << endl;
    cout << endl;
  }
}