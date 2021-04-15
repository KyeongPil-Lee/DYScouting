#include <Include/SimplePlotTools.h>

#include <Include/DYTool.h>
#include <ControlPlot/DYHistInfo.h>

class PlotProducer
{
public:
  DYHistInfo histInfo_;

  TString histName_;

  // -- histograms
  TH1D* h_data_;
  TH1D* h_DYTauTau_;
  TH1D* h_ttbar_;
  TH1D* h_WJets_;
  TH1D* h_QCDMuEnriched_;
  TH1D* h_DYMuMu_;

  PlotProducer(DYHistInfo histInfo)
  {
    histInfo_ = histInfo;
    histName_ = histInfo_.histName_;
  }

  void Produce()
  {
    Init();

    Produce_StackedPlot();
    Produce_FractionPlot();
    Produce_NormHist(); // -- should be produced at the last step (as it normalized the original histograms)
  }

private:
  void Init()
  {
    h_data_ = nullptr;
    h_DYTauTau_ = nullptr;
    h_ttbar_ = nullptr;
    h_WJets_ = nullptr;
    h_QCDMuEnriched_ = nullptr;
    h_DYMuMu_ = nullptr;

    TString basePath = "./";
    TString baseFileName = "ROOTFile_MakeHist_OfflineDistribution";
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

    SetRebin();
  }

  void Produce_FractionPlot()
  {
    TH1D* h_totMC = (TH1D*)h_DYTauTau_->Clone();
    h_totMC->Add( h_ttbar_ );
    h_totMC->Add( h_WJets_ );
    h_totMC->Add( h_DYMuMu_ );
    h_totMC->Add( h_QCDMuEnriched_ );

    // for(Int_t i=0; i<h_totMC->GetNbinsX(); i++)
    // {
    //   Int_t i_bin = i+1;
    //   printf("[%d bin] (DYTauTau, ttbar, WJets, DYMuMu, QCDMuEnriched, totMC) = (%.1lf, %.1lf, %.1lf, %.1lf, %.1lf, %.1lf)\n",
    //      i_bin,
    //      h_DYTauTau_->GetBinContent(i_bin),
    //      h_ttbar_->GetBinContent(i_bin),
    //      h_WJets_->GetBinContent(i_bin),
    //      h_DYMuMu_->GetBinContent(i_bin),
    //      h_QCDMuEnriched_->GetBinContent(i_bin),
    //      h_totMC->GetBinContent(i_bin) );
    // }

    TH1D* h_frac_DYTauTau = (TH1D*)h_DYTauTau_->Clone();
    h_frac_DYTauTau->Divide( h_DYTauTau_, h_totMC );

    TH1D* h_frac_ttbar = (TH1D*)h_ttbar_->Clone();
    h_frac_ttbar->Divide( h_ttbar_, h_totMC );

    TH1D* h_frac_WJets = (TH1D*)h_WJets_->Clone();
    h_frac_WJets->Divide( h_WJets_, h_totMC );

    TH1D* h_frac_DYMuMu = (TH1D*)h_DYMuMu_->Clone();
    h_frac_DYMuMu->Divide( h_DYMuMu_, h_totMC );

    TH1D* h_frac_QCDMuEnriched = (TH1D*)h_QCDMuEnriched_->Clone();
    h_frac_QCDMuEnriched->Divide( h_QCDMuEnriched_, h_totMC );

    // -- plots
    TString canvasName = histName_;
    canvasName.ReplaceAll("h_", "c_frac_");

    Bool_t isLogX, isLogY;
    GetLogXY(isLogX, isLogY);
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, isLogX, 0);

    canvas->Register(h_frac_DYTauTau,      "Z/#gamma #rightarrow #tau#tau (M > 10 GeV)", DYTool::kDYTauTau);
    canvas->Register(h_frac_ttbar,         "t#bar{t} (leptonic)", DYTool::kTop);
    canvas->Register(h_frac_WJets,         "W+jets", DYTool::kWJets);
    canvas->Register(h_frac_DYMuMu,        "Z/#gamma #rightarrow #mu#mu (M > 10 GeV)", DYTool::kDY);
    canvas->Register(h_frac_QCDMuEnriched, "QCD (p_{T}(#mu) > 5 GeV, #mu-enriched)", DYTool::kQCD);

    canvas->SetLegendColumn(2);
    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);
    canvas->SetTitle(histInfo_.titleX_, "Fraction w.r.t. total MC");
    
    if( histInfo_.setRangeX_ ) canvas->SetRangeX( histInfo_.minX_, histInfo_.maxX_ );

    canvas->SetRangeY(0, 1.1);
    canvas->Latex_CMSSim();
    canvas->Draw("HISTLP");
  }

  void Produce_StackedPlot()
  {
    // -- plots
    TString canvasName = histName_;
    canvasName.ReplaceAll("h_", "c_stack_");
    Bool_t isLogX, isLogY;
    GetLogXY(isLogX, isLogY);
    PlotTool::HistStackCanvaswRatio *canvas = new PlotTool::HistStackCanvaswRatio(canvasName, isLogX, isLogY);

    canvas->RegisterData(h_data_, "2018Data (SingleMuon)", kBlack);

    if( histName_.Contains("SS") )
    {
      canvas->Register(h_DYTauTau_,      "Z/#gamma #rightarrow #tau#tau (M > 10 GeV)", DYTool::kDYTauTau);
      canvas->Register(h_ttbar_,         "t#bar{t} (leptonic)", DYTool::kTop);
      canvas->Register(h_DYMuMu_,        "Z/#gamma #rightarrow #mu#mu (M > 10 GeV)", DYTool::kDY);
      canvas->Register(h_WJets_,         "W+jets", DYTool::kWJets);
      canvas->Register(h_QCDMuEnriched_, "QCD (p_{T}(#mu) > 5 GeV, #mu-enriched)", DYTool::kQCD);
    }
    else
    {
      canvas->Register(h_DYTauTau_,      "Z/#gamma #rightarrow #tau#tau (M > 10 GeV)", DYTool::kDYTauTau);
      canvas->Register(h_ttbar_,         "t#bar{t} (leptonic)", DYTool::kTop);
      canvas->Register(h_WJets_,         "W+jets", DYTool::kWJets);
      if( histName_.Contains("ZPeak") || histName_.Contains("M10to60_OS") ) // -- DY is dominant ... it should come at the last step
      {
        canvas->Register(h_QCDMuEnriched_, "QCD (p_{T}(#mu) > 5 GeV, #mu-enriched)", DYTool::kQCD);
        canvas->Register(h_DYMuMu_,        "Z/#gamma #rightarrow #mu#mu (M > 10 GeV)", DYTool::kDY);
      }
      else
      {
        canvas->Register(h_DYMuMu_,        "Z/#gamma #rightarrow #mu#mu (M > 10 GeV)", DYTool::kDY);
        canvas->Register(h_QCDMuEnriched_, "QCD (p_{T}(#mu) > 5 GeV, #mu-enriched)", DYTool::kQCD);
      }
    }

    canvas->SetLegendColumn(2);
    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);
    canvas->SetTitle(histInfo_.titleX_, "Entry", "data/MC");

    if( histInfo_.setRangeX_ ) canvas->SetRangeX( histInfo_.minX_, histInfo_.maxX_ );
    canvas->SetAutoRangeY();

    canvas->SetRangeRatio(0.4, 1.6);
    if( histName_.Contains("h_diMuM") ) canvas->SetRangeRatio(0.7, 1.1);

    canvas->Latex_CMSPre();
    canvas->RegisterLatex(0.75, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", DYTool::LUMI_2018/1000.0));

    canvas->ShowDataMCRatio();

    canvas->Draw();
  }

  void Produce_NormHist()
  {
    // -- normalization
    h_DYTauTau_->Scale( 1.0 / h_DYTauTau_->Integral() );
    h_ttbar_->Scale( 1.0 / h_ttbar_->Integral() );
    h_WJets_->Scale( 1.0 / h_WJets_->Integral() );
    h_DYMuMu_->Scale( 1.0 / h_DYMuMu_->Integral() );
    h_QCDMuEnriched_->Scale( 1.0 / h_QCDMuEnriched_->Integral() );

    // -- plots
    TString canvasName = histName_;
    canvasName.ReplaceAll("h_", "c_norm_");

    Bool_t isLogX, isLogY;
    GetLogXY(isLogX, isLogY);
    PlotTool::HistCanvas *canvas = new PlotTool::HistCanvas(canvasName, isLogX, 0);

    canvas->Register(h_DYTauTau_,      "Z/#gamma #rightarrow #tau#tau (M > 10 GeV)", DYTool::kDYTauTau);
    canvas->Register(h_ttbar_,         "t#bar{t} (leptonic)", DYTool::kTop);
    canvas->Register(h_WJets_,         "W+jets", DYTool::kWJets);
    canvas->Register(h_DYMuMu_,        "Z/#gamma #rightarrow #mu#mu (M > 10 GeV)", DYTool::kDY);
    canvas->Register(h_QCDMuEnriched_, "QCD (p_{T}(#mu) > 5 GeV, #mu-enriched)", DYTool::kQCD);

    canvas->SetLegendColumn(2);
    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);
    canvas->SetTitle(histInfo_.titleX_, "Entry (norm. to 1)");
    
    if( histInfo_.setRangeX_ ) canvas->SetRangeX( histInfo_.minX_, histInfo_.maxX_ );

    canvas->SetAutoRangeY();
    canvas->Latex_CMSSim();
    canvas->Draw("HISTLP");
  }

  void GetLogXY(Bool_t &isLogX, Bool_t &isLogY)
  {
    isLogX = 0;
    isLogY = 1;
    // if( histName_.Contains("h_diMuM") ) isLogY = 1;
  }

  void SetRebin(PlotTool::HistCanvas* canvas )
  {
    if( histName_.Contains("h_diMuPt") )   canvas->SetRebin(5);
    if( histName_.Contains("h_pt") )       canvas->SetRebin(5);
    if( histName_.Contains("h_normChi2") ) canvas->SetRebin(5);
    if( histName_.Contains("h_vtx_x") ) canvas->SetRebin(10);
    if( histName_.Contains("h_vtx_y") ) canvas->SetRebin(10);
    if( histName_.Contains("h_vtx_z") ) canvas->SetRebin(10);
    if( histName_.Contains("h_vtx_r") ) canvas->SetRebin(10);
    if( histName_.Contains("h_vtx_R") ) canvas->SetRebin(10);
    if( histName_.Contains("h_caloMET_pt") ) canvas->SetRebin(5);
  }

  void SetRebin()
  {
    if( histName_.Contains("h_diMuPt") )     DoRebin(5);
    if( histName_.Contains("h_pt") )         DoRebin(5);
    if( histName_.Contains("h_normChi2") )   DoRebin(5);
    if( histName_.Contains("h_vtx_x") )      DoRebin(10);
    if( histName_.Contains("h_vtx_y") )      DoRebin(10);
    if( histName_.Contains("h_vtx_z") )      DoRebin(10);
    if( histName_.Contains("h_vtx_r") )      DoRebin(10);
    if( histName_.Contains("h_vtx_R") )      DoRebin(10);
    if( histName_.Contains("h_caloMET_pt") ) DoRebin(5);
  }

  void DoRebin(Int_t nRebin )
  {
    h_data_->Rebin( nRebin );
    h_DYTauTau_->Rebin( nRebin );
    h_ttbar_->Rebin( nRebin );
    h_WJets_->Rebin( nRebin );
    h_QCDMuEnriched_->Rebin( nRebin );
    h_DYMuMu_->Rebin( nRebin );
  }

};

void MakeSubDir()
{
  vector<TString> vec_subDir = {"./ZPeak", "./M10to60", "./M10to60_OS", "./M10to60_SS"};
  for(const auto subDir : vec_subDir)
  {
    if( gSystem->mkdir(subDir, kTRUE) != -1 )
    {
      gSystem->mkdir(subDir, kTRUE);
      cout << subDir << " directory is created" << endl;
    }
  }
}


void DrawPlots()
{
  MakeSubDir();

  vector<DYHistInfo> vec_histInfo = GetAllDYHist();

  for(auto histInfo : vec_histInfo )
  {
    if( histInfo.histName_.Contains("h_diMuDEtaCM") ) continue;
    if( histInfo.histName_.Contains("h_diMuDPhiCM") ) continue;
    if( histInfo.histName_.Contains("h_diMuAngle3DCM") ) continue;
    if( histInfo.histName_.Contains("h_diMuHasVertex") ) continue;
    if( histInfo.histName_.Contains("h_diMuVtx") ) continue;
    if( histInfo.histName_.Contains("h_nVtx") ) continue;
    if( histInfo.histName_.Contains("h_nPixelVtx") ) continue;
    if( histInfo.histName_.Contains("h_nPixelVtxMu") ) continue;
    if( histInfo.histName_.Contains("h_caloMET") ) continue;
    // if( histInfo.histName_.Contains("h_nPixelVtx") ) continue;
    // if( histInfo.histName_.Contains("h_nPixelVtx") ) continue;
    // if( histInfo.histName_.Contains("h_nPixelVtx") ) continue;
    // if( histInfo.histName_.Contains("h_nPixelVtx") ) continue;

    PlotProducer *producer = new PlotProducer(histInfo);
    producer->Produce();
  }
}