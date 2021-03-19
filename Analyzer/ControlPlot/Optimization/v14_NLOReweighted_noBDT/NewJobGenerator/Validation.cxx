#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TString sample_;
  TString histName_;

  Bool_t isLogX_;
  Bool_t isLogY_;
  TString titleX_;
  Double_t minX_;
  Double_t maxX_;


  PlotProducer(TString sample, TString histName)
  {
    sample_   = sample;
    histName_ = histName;
  }

  void Produce()
  {
    Set_IndividualSetting();

    TString fileName_base = TString::Format("ROOTFile_MakeHist_Dimuon_noWeight_%s.root", sample_.Data());
    TString fileName_ref = "../"+fileName_base;
    TString fileName_target = fileName_base;

    TH1D* h_ref    = PlotTool::Get_Hist(fileName_ref, histName_);
    TH1D* h_target = PlotTool::Get_Hist(fileName_target, histName_);

    TString outputDir = sample_;
    if (gSystem->mkdir(outputDir, kTRUE) != -1)
      gSystem->mkdir(outputDir, kTRUE);

    TString canvasName = histName_;
    canvasName.ReplaceAll("h_", "c_");
    canvasName.ReplaceAll("/", "_");
    canvasName = sample_ + "/" + canvasName;

    PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio(canvasName, isLogX_, isLogY_);
    canvas->SetTitle(titleX_, "Entry", "target/ref");

    canvas->Register(h_ref,    "Reference", kBlack);
    canvas->Register(h_target, "Target",    kRed);

    canvas->SetRangeX(minX_, maxX_);
    canvas->SetAutoRangeY();
    canvas->SetRangeRatio(0.96, 1.04);
    canvas->SetLegendPosition(0.55, 0.82, 0.95, 0.95);

    canvas->Latex_CMSInternal();

    canvas->Draw();
  }

private:
  void Set_IndividualSetting()
  {
    if( histName_ == "h_diMuM" )
    {
      isLogX_ = kFALSE;
      isLogY_ = kTRUE;
      minX_ = 0;
      maxX_ = 1000;
      titleX_ = "m [GeV]";
    }

    if( histName_ == "M10to60/h_diMuM" )
    {
      isLogX_ = kFALSE;
      isLogY_ = kTRUE;
      minX_ = 10;
      maxX_ = 60;
      titleX_ = "m [GeV]";
    }

    if( histName_ == "ZPeak/h_diMuM" )
    {
      isLogX_ = kFALSE;
      isLogY_ = kTRUE;
      minX_ = 60;
      maxX_ = 120;
      titleX_ = "m [GeV]";
    }

    if( histName_ == "h_diMuPt" )
    {
      isLogX_ = kFALSE;
      isLogY_ = kTRUE;
      minX_ = 0;
      maxX_ = 200;
      titleX_ = "p_{T}(#mu#mu) [GeV]";
    }

    if( histName_ == "h_diMuRap" )
    {
      isLogX_ = kFALSE;
      isLogY_ = kTRUE;
      minX_ = -2.5;
      maxX_ = 2.5;
      titleX_ = "Y(#mu#mu)";
    }

    if( histName_ == "h_pt" )
    {
      isLogX_ = kFALSE;
      isLogY_ = kTRUE;
      minX_ = 0;
      maxX_ = 200;
      titleX_ = "p_{T}(#mu) [GeV]";
    }

    if( histName_ == "h_eta" )
    {
      isLogX_ = kFALSE;
      isLogY_ = kTRUE;
      minX_ = -2.5;
      maxX_ = 2.5;
      titleX_ = "#eta(#mu)";
    }

    if( histName_ == "h_phi" )
    {
      isLogX_ = kFALSE;
      isLogY_ = kTRUE;
      minX_ = -2.5;
      maxX_ = 2.5;
      titleX_ = "#phi(#mu)";
    }

  }

};


void Validation()
{
  vector<TString> vec_sample = {
    "DYMuMu_M10to50", "DYMuMu_M50toInf", "DYTauTau_M10to50", "DYTauTau_M50toInf", 
    "QCDMuEnriched_Pt15to20", 
    "QCDMuEnriched_Pt20to30",
    "QCDMuEnriched_Pt30to50",
    "QCDMuEnriched_Pt50to80",
    "QCDMuEnriched_Pt80to120",
    "QCDMuEnriched_Pt120to170",
    "QCDMuEnriched_Pt170to300",
    "QCDMuEnriched_Pt300to470",
    "QCDMuEnriched_Pt470to600",
    "QCDMuEnriched_Pt600to800",
    "QCDMuEnriched_Pt800to1000",
    "QCDMuEnriched_Pt1000toInf",
    "ScoutingCaloMuon_Run2018All", "ttbar", "WJets"};
    
  vector<TString> vec_histName = {
    "h_diMuM", "M10to60/h_diMuM", "ZPeak/h_diMuM", 
    "h_diMuPt", "h_diMuRap",
    "h_pt", "h_eta", "h_phi"};

  for(auto sample : vec_sample )
  {
    for(auto histName : vec_histName)
    {
       PlotProducer* producer = new PlotProducer(sample, histName);
       producer->Produce();
    }
  }
}