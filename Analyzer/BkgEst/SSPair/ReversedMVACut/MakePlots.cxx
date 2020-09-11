#include <Include/SimplePlotTools.h>

#include <Include/SimplePlotTools.h>
#include <Include/DYTool.h>
#include <ControlPlot/DYHistInfo.h>

class HistNormTool
{
public:
  DYTool::SimpleSampleInfo* sampleInfo_;

  TString type_;
  Double_t sumWeight_;
  Double_t xSec_;

  TString fileName_;

  HistNormTool( TString type )
  {
    sampleInfo_ = new DYTool::SimpleSampleInfo(type);

    type_ = type;
    Set_FileName();
  }

  TH1D* Get_NormalizedHist(TString histName)
  {
    TH1D* h = PlotTool::Get_Hist(fileName_, histName);
    h->Sumw2();

    Double_t normFactor = sampleInfo_->NormFactorToLumi("2018");
    h->Scale( normFactor );

    // cout << type_ << " -> " << h->Integral() << endl;
    RemoveNegativeBin(h);

    return h;
  }

  void RemoveNegativeBin(TH1D* h)
  {
    Int_t nBin = h->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t binContent = h->GetBinContent(i_bin);
      if( binContent < 0 )
      {
        cout << "[" << h->GetName() << ", " << type_ << "] binContent (" << i_bin << " bin) = " << binContent << " < 0 ... -> set it as 0" << endl;
        h->SetBinContent(i_bin, 0);
      }
    }
  }

private:
  void Set_FileName()
  {
    if (type_ != "" )
      fileName_ = "ROOTFile_MakeHist_Dimuon_noWeight_"+type_+".root";
    else
    {
      cout << "no information for type_ = " << type_ << endl;
      fileName_ = "";
    }
  }

};

class PlotProducer
{
public:
  TString histName_;
  TString titleX_;

  PlotTool::HistInfo histInfo_data_;
  vector<PlotTool::HistInfo> vec_MCHistInfo_;

  // -- histograms
  TH1D* h_data_;
  TH1D* h_DYTauTau_;
  TH1D* h_ttbar_;
  TH1D* h_WJets_;
  TH1D* h_QCDMuEnriched_;
  TH1D* h_DYMuMu_;

  DYHistInfo histInfo_;

  PlotProducer(TString histName )
  {
    histName_ = histName;
  }

  PlotProducer(DYHistInfo histInfo )
  {
    histInfo_ = histInfo;
    histName_ = histInfo_.histName_;

    cout << "histName: " << histName_ << endl;
  }

  void Produce()
  {
    InitHistograms();
    SetupHistograms();

    Produce_StackedPlot();
    Produce_FractionPlot();
    Produce_NormHist(); // -- should be produced at the last step (as it normalized the original histograms)
  }

  void Produce_noPlot()
  {
    InitHistograms();
    SetupHistograms();
  }

private:
  void InitHistograms()
  {
    h_data_ = nullptr;
    h_DYTauTau_ = nullptr;
    h_ttbar_ = nullptr;
    h_WJets_ = nullptr;
    h_QCDMuEnriched_ = nullptr;
    h_DYMuMu_ = nullptr;
  }

  void SetupHistograms()
  {
    // -- setup the histograms
    HistNormTool* histNormTool_DYMuMu_M10to50  = new HistNormTool("DYMuMu_M10to50");
    HistNormTool* histNormTool_DYMuMu_M50toInf = new HistNormTool("DYMuMu_M50toInf");

    HistNormTool* histNormTool_DYTauTau_M10to50  = new HistNormTool("DYTauTau_M10to50");
    HistNormTool* histNormTool_DYTauTau_M50toInf = new HistNormTool("DYTauTau_M50toInf");

    HistNormTool* histNormTool_ttbar = new HistNormTool("ttbar");
    HistNormTool* histNormTool_WJets = new HistNormTool("WJets");
    HistNormTool* histNormTool_QCDMuEnriched_Pt15to20    = new HistNormTool("QCDMuEnriched_Pt15to20");
    HistNormTool* histNormTool_QCDMuEnriched_Pt20to30    = new HistNormTool("QCDMuEnriched_Pt20to30");
    HistNormTool* histNormTool_QCDMuEnriched_Pt30to50    = new HistNormTool("QCDMuEnriched_Pt30to50");
    HistNormTool* histNormTool_QCDMuEnriched_Pt50to80    = new HistNormTool("QCDMuEnriched_Pt50to80");
    HistNormTool* histNormTool_QCDMuEnriched_Pt80to120   = new HistNormTool("QCDMuEnriched_Pt80to120");
    HistNormTool* histNormTool_QCDMuEnriched_Pt120to170  = new HistNormTool("QCDMuEnriched_Pt120to170");
    HistNormTool* histNormTool_QCDMuEnriched_Pt170to300  = new HistNormTool("QCDMuEnriched_Pt170to300");
    HistNormTool* histNormTool_QCDMuEnriched_Pt300to470  = new HistNormTool("QCDMuEnriched_Pt300to470");
    HistNormTool* histNormTool_QCDMuEnriched_Pt470to600  = new HistNormTool("QCDMuEnriched_Pt470to600");
    HistNormTool* histNormTool_QCDMuEnriched_Pt600to800  = new HistNormTool("QCDMuEnriched_Pt600to800");
    HistNormTool* histNormTool_QCDMuEnriched_Pt800to1000 = new HistNormTool("QCDMuEnriched_Pt800to1000");
    HistNormTool* histNormTool_QCDMuEnriched_Pt1000toInf = new HistNormTool("QCDMuEnriched_Pt1000toInf");


    // -- DY
    TH1D* h_DYMuMu_M10to50  = histNormTool_DYMuMu_M10to50->Get_NormalizedHist(histName_);
    TH1D* h_DYMuMu_M50toInf = histNormTool_DYMuMu_M50toInf->Get_NormalizedHist(histName_);

    h_DYMuMu_ = (TH1D*)h_DYMuMu_M10to50->Clone();
    h_DYMuMu_->Add( h_DYMuMu_M50toInf );

    // -- DYTauTau
    TH1D* h_DYTauTau_M10to50  = histNormTool_DYTauTau_M10to50->Get_NormalizedHist(histName_);
    TH1D* h_DYTauTau_M50toInf = histNormTool_DYTauTau_M50toInf->Get_NormalizedHist(histName_);

    h_DYTauTau_ = (TH1D*)h_DYTauTau_M10to50->Clone();
    h_DYTauTau_->Add( h_DYTauTau_M50toInf );

    // -- QCD
    TH1D* h_QCDMuEnriched_Pt15to20    = histNormTool_QCDMuEnriched_Pt15to20->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt20to30    = histNormTool_QCDMuEnriched_Pt20to30->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt30to50    = histNormTool_QCDMuEnriched_Pt30to50->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt50to80    = histNormTool_QCDMuEnriched_Pt50to80->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt80to120   = histNormTool_QCDMuEnriched_Pt80to120->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt120to170  = histNormTool_QCDMuEnriched_Pt120to170->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt170to300  = histNormTool_QCDMuEnriched_Pt170to300->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt300to470  = histNormTool_QCDMuEnriched_Pt300to470->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt470to600  = histNormTool_QCDMuEnriched_Pt470to600->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt600to800  = histNormTool_QCDMuEnriched_Pt600to800->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt800to1000 = histNormTool_QCDMuEnriched_Pt800to1000->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt1000toInf = histNormTool_QCDMuEnriched_Pt1000toInf->Get_NormalizedHist(histName_);

    h_QCDMuEnriched_ = (TH1D*)h_QCDMuEnriched_Pt15to20->Clone();
    h_QCDMuEnriched_->Add( h_QCDMuEnriched_Pt20to30 );
    h_QCDMuEnriched_->Add( h_QCDMuEnriched_Pt30to50 );
    h_QCDMuEnriched_->Add( h_QCDMuEnriched_Pt50to80 );
    h_QCDMuEnriched_->Add( h_QCDMuEnriched_Pt80to120 );
    h_QCDMuEnriched_->Add( h_QCDMuEnriched_Pt120to170 );
    h_QCDMuEnriched_->Add( h_QCDMuEnriched_Pt170to300 );
    h_QCDMuEnriched_->Add( h_QCDMuEnriched_Pt300to470 );
    h_QCDMuEnriched_->Add( h_QCDMuEnriched_Pt470to600 );
    h_QCDMuEnriched_->Add( h_QCDMuEnriched_Pt600to800 );
    h_QCDMuEnriched_->Add( h_QCDMuEnriched_Pt800to1000 );
    h_QCDMuEnriched_->Add( h_QCDMuEnriched_Pt1000toInf );

    // -- others
    h_ttbar_ = histNormTool_ttbar->Get_NormalizedHist(histName_);
    h_WJets_ = histNormTool_WJets->Get_NormalizedHist(histName_);

    h_data_ = PlotTool::Get_Hist("ROOTFile_MakeHist_Dimuon_noWeight_ScoutingCaloMuon_Run2018All.root", histName_); h_data_->Sumw2();

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

    // SetRebin(canvas);

    canvas->SetLegendColumn(2);
    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);
    canvas->SetTitle(histInfo_.titleX_, "Fraction w.r.t. total MC");
    
    if( histInfo_.setRangeX_ ) canvas->SetRangeX( histInfo_.minX_, histInfo_.maxX_ );

    canvas->SetRangeY(0, 1.1);
    canvas->Latex_CMSSim();
    // canvas->RegisterLatex(0.75, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", lumi/1000.0));
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

    canvas->RegisterData(h_data_, "Data (Run2018A-D)", kBlack);

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

    // SetRebin(canvas);

    canvas->SetLegendColumn(2);
    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);
    canvas->SetTitle(histInfo_.titleX_, "Entry", "data/MC");

    if( histInfo_.setRangeX_ ) canvas->SetRangeX( histInfo_.minX_, histInfo_.maxX_ );
    canvas->SetAutoRangeY();

    // canvas->SetRangeRatio(0.88, 1.12);
    // canvas->SetRangeRatio(0.2, 1.4);
    // canvas->SetRangeRatio(0.7, 1.3);
    canvas->SetRangeRatio(0.4, 1.6);
    // if( !histName_.Contains("ZPeak") ) canvas->SetRangeRatio(0.2, 1.4);

    canvas->Latex_CMSPre();
    canvas->RegisterLatex(0.75, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", DYTool::LUMI_2018/1000.0));

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

    // SetRebin(canvas);

    canvas->SetLegendColumn(2);
    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);
    canvas->SetTitle(histInfo_.titleX_, "Entry (norm. to 1)");
    
    if( histInfo_.setRangeX_ ) canvas->SetRangeX( histInfo_.minX_, histInfo_.maxX_ );

    canvas->SetAutoRangeY();
    // canvas->SetRangeY(0, 0.1);
    canvas->Latex_CMSSim();
    // canvas->RegisterLatex(0.75, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", lumi/1000.0));
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

void MakePlots()
{
  MakeSubDir();

  vector<DYHistInfo> vec_histInfo = GetAllDYHist();
  // for(auto histInfo : vec_histInfo )
  //   cout << "histName: " << histInfo.histName_ << endl;

  for(auto histInfo : vec_histInfo )
  {
    if( histInfo.histName_.Contains("h_diMuM_lowM_fineBin") ) continue;

    PlotProducer *producer = new PlotProducer(histInfo);
    producer->Produce();
  }
}