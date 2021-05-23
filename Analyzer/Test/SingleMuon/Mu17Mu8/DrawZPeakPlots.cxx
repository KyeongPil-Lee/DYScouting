#include <Test/SingleMuon/Mu17Mu8/ZPeakHistAttribute.h>
#include <Include/SimplePlotTools.h>
#include <Include/DYTool.h>

class PlotDrawer
{
public:
  HistAttribute histAttribute_;
  TString histName_;

  // -- histograms
  TH1D* h_data_;
  TH1D* h_DYTauTau_;
  TH1D* h_ttbar_;
  TH1D* h_WJets_;
  TH1D* h_QCDMuEnriched_;
  TH1D* h_DYMuMu_;

  PlotDrawer(HistAttribute histAttribute, TString selectionType)
  {
    histAttribute_ = histAttribute;
    histName_ = histAttribute_.histName_ + "_" + selectionType;
  }

  void Draw()
  {
    Init();
    Draw_StackedPlot();
  }

private:
  void Draw_StackedPlot()
  {
    // -- plots
    TString canvasName = histName_;
    canvasName.ReplaceAll("h_", "c_stack_");
    Bool_t isLogX, isLogY;
    isLogX = 0;
    isLogY = 1;

    PlotTool::HistStackCanvaswRatio *canvas = new PlotTool::HistStackCanvaswRatio(canvasName, isLogX, isLogY);

    canvas->RegisterData(h_data_, "2018Data (SingleMuon)", kBlack);
    canvas->SetTitle(histAttribute_.titleX_, "Entry", "data/MC");

    canvas->Register(h_DYTauTau_,      "Z/#gamma #rightarrow #tau#tau (M > 10 GeV)", DYTool::kDYTauTau);
    canvas->Register(h_ttbar_,         "t#bar{t} (leptonic)", DYTool::kTop);
    canvas->Register(h_WJets_,         "W+jets", DYTool::kWJets);
    canvas->Register(h_QCDMuEnriched_, "QCD (p_{T}(#mu) > 5 GeV, #mu-enriched)", DYTool::kQCD);
    canvas->Register(h_DYMuMu_,        "Z/#gamma #rightarrow #mu#mu (M > 10 GeV)", DYTool::kDY);

    canvas->SetLegendColumn(2);
    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);
    canvas->SetTitle(histAttribute_.titleX_, "Entry", "data/MC");

    if( histAttribute_.setRangeX_ ) canvas->SetRangeX( histAttribute_.minX_, histAttribute_.maxX_ );
    canvas->SetAutoRangeY();
    canvas->SetRangeRatio(0.8, 1.2);

    canvas->Latex_CMSInternal();
    canvas->RegisterLatex(0.75, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", DYTool::LUMI_2018/1000.0));

    canvas->ShowDataMCRatio();

    canvas->Draw();
  }

  void Init()
  {
    TString basePath = "./";
    TString baseFileName = "ROOTFile_MakeHist_OfflineZPeak";
    DYTool::HistGetter* histGetter = new DYTool::HistGetter(basePath, baseFileName);
    histGetter->DoNormalization("2018");
    histGetter->DoRemoveNegativeBin();

    TString tag_data  = "Offline_SingleMuon_Run2018All";
    TString tag_ttbar = "Offline_ttbar";
    TString tag_WJets = "Offline_WJets";

    vector<TString> vec_tag_DYMuMu   = { "Offline_DYMuMu_M10to50", "Offline_DYMuMu_M50toInf" };
    vector<TString> vec_tag_DYTauTau = { "Offline_DYTauTau_M10to50", "Offline_DYTauTau_M50toInf" };

    vector<TString> vec_tag_QCDMuEnriched = { 
      "Offline_QCDMuEnriched_Pt15to20", 
      "Offline_QCDMuEnriched_Pt20to30",
      "Offline_QCDMuEnriched_Pt30to50",
      "Offline_QCDMuEnriched_Pt50to80",
      "Offline_QCDMuEnriched_Pt80to120",
      "Offline_QCDMuEnriched_Pt120to170",
      "Offline_QCDMuEnriched_Pt170to300",
      "Offline_QCDMuEnriched_Pt300to470",
      "Offline_QCDMuEnriched_Pt470to600",
      "Offline_QCDMuEnriched_Pt600to800",
      "Offline_QCDMuEnriched_Pt800to1000",
      "Offline_QCDMuEnriched_Pt1000toInf"
    };

    Bool_t isMC = kFALSE;
    h_data_ = histGetter->Get_Histogram(histName_, tag_data, isMC);

    isMC = kTRUE;
    h_ttbar_ = histGetter->Get_Histogram(histName_, tag_ttbar, isMC);
    h_WJets_ = histGetter->Get_Histogram(histName_, tag_WJets, isMC);

    h_DYMuMu_        = histGetter->Get_Histogram(histName_, vec_tag_DYMuMu, isMC);
    h_DYTauTau_      = histGetter->Get_Histogram(histName_, vec_tag_DYTauTau, isMC);
    h_QCDMuEnriched_ = histGetter->Get_Histogram(histName_, vec_tag_QCDMuEnriched, isMC);
  }
};

void DrawZPeakPlots()
{
  vector<HistAttribute> vec_histAttribute = Get_ZPeakHistAttribute();
  vector<TString> vec_selectionType = {"IsoMu24Z", "L1", "DST", "HLT"};
  for(auto histAttribute: vec_histAttribute )
  {
    for(auto selectionType: vec_selectionType )
    {
      PlotDrawer* drawer = new PlotDrawer(histAttribute, selectionType);
      drawer->Draw();

      // break;
    }
  }
}