#include <Include/SimplePlotTools.h>

#include <Include/SimplePlotTools.h>
#include <Include/DYTool.h>

const Double_t lumi = 61300; // -- 61.3 /fb

class SampleInfo
{
public:
  TString type_;
  Double_t sumWeight_;
  Double_t xSec_;

  TString fileName_;

  SampleInfo( TString type )
  {
    type_ = type;
    Init();
  }

  TH1D* Get_NormalizedHist(TString histName)
  {
    TH1D* h = PlotTool::Get_Hist(fileName_, histName);
    h->Sumw2();

    Double_t normFactor = (lumi * xSec_) / sumWeight_;

    h->Scale( normFactor );

    // cout << type_ << " -> " << h->Integral() << endl;

    return h;
  }

private:
  void Init()
  {
    Set_SumWeight();
    Set_xSec();
    Set_FileName();
  }

  void Set_SumWeight()
  {
    if( type_ == "DYMuMu_M10to50" )                 sumWeight_ = 28985097.0;
    else if( type_ == "DYMuMu_M50toInf" )           sumWeight_ = 44318555.0;
    else if( type_ == "DYTauTau_M10to50" )          sumWeight_ = 28843598.0;
    else if( type_ == "DYTauTau_M50toInf" )         sumWeight_ = 44279551.0;
    else if( type_ == "ttbar" )                     sumWeight_ = 63811310.0;
    else if( type_ == "WJets" )                     sumWeight_ = 71061720.0;
    else if( type_ == "QCDMuEnriched_Pt20toInf" )   sumWeight_ = 22165320.0;
    else if( type_ == "QCDMuEnriched_Pt15to20" )    sumWeight_ = 4572168.0;
    else if( type_ == "QCDMuEnriched_Pt20to30" )    sumWeight_ = 26549827.0;
    else if( type_ == "QCDMuEnriched_Pt30to50" )    sumWeight_ = 30431342.0;
    else if( type_ == "QCDMuEnriched_Pt50to80" )    sumWeight_ = 20360155.0;
    else if( type_ == "QCDMuEnriched_Pt80to120" )   sumWeight_ = 25652280.0;
    else if( type_ == "QCDMuEnriched_Pt120to170" )  sumWeight_ = 21315922.0;
    else if( type_ == "QCDMuEnriched_Pt170to300" )  sumWeight_ = 36372547.0;
    else if( type_ == "QCDMuEnriched_Pt300to470" )  sumWeight_ = 29488563.0;
    else if( type_ == "QCDMuEnriched_Pt470to600" )  sumWeight_ = 20542857.0;
    else if( type_ == "QCDMuEnriched_Pt600to800" )  sumWeight_ = 16891461.0;
    else if( type_ == "QCDMuEnriched_Pt800to1000" ) sumWeight_ = 16749914.0;
    else if( type_ == "QCDMuEnriched_Pt1000toInf" ) sumWeight_ = 11039499.0;
    else
    {
      cout << "no information for type_ = " << type_ << endl;
      sumWeight_ = 0;
    }
  }

  void Set_xSec()
  {
    if( type_ == "DYMuMu_M10to50" )                 xSec_ = 6270.0;
    else if( type_ == "DYMuMu_M50toInf" )           xSec_ = 2009.41;
    else if( type_ == "DYTauTau_M10to50" )          xSec_ = 6270.0;
    else if( type_ == "DYTauTau_M50toInf" )         xSec_ = 2009.41;
    else if( type_ == "ttbar" )                     xSec_ = 88.29;
    else if( type_ == "WJets" )                     xSec_ = 61526.7;
    else if( type_ == "QCDMuEnriched_Pt20toInf" )   xSec_ = 239400.0;
    else if( type_ == "QCDMuEnriched_Pt15to20" )    xSec_ = 2799000.0;
    else if( type_ == "QCDMuEnriched_Pt20to30" )    xSec_ = 2526000.0;
    else if( type_ == "QCDMuEnriched_Pt30to50" )    xSec_ = 1362000.0;
    else if( type_ == "QCDMuEnriched_Pt50to80" )    xSec_ = 376600.0;
    else if( type_ == "QCDMuEnriched_Pt80to120" )   xSec_ = 88930.0;
    else if( type_ == "QCDMuEnriched_Pt120to170" )  xSec_ = 21230.0;
    else if( type_ == "QCDMuEnriched_Pt170to300" )  xSec_ = 7055.0;
    else if( type_ == "QCDMuEnriched_Pt300to470" )  xSec_ = 797.3; // -- TuneCUETP8M1
    else if( type_ == "QCDMuEnriched_Pt470to600" )  xSec_ = 59.24;
    else if( type_ == "QCDMuEnriched_Pt600to800" )  xSec_ = 25.25; // -- TuneCUETP8M1
    else if( type_ == "QCDMuEnriched_Pt800to1000" ) xSec_ = 4.723; // -- TuneCUETP8M1
    else if( type_ == "QCDMuEnriched_Pt1000toInf" ) xSec_ = 1.613; // -- TuneCUETP8M1
    else
    {
      cout << "no information for type_ = " << type_ << endl;
      sumWeight_ = 0;
    }
  }

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

  PlotProducer(TString histName )
  {
    histName_ = histName;
  }

  void Produce()
  {
    InitHistograms();
    SetupHistograms();

    Produce_StackedPlot();
    Produce_FractionPlot();
    Produce_NormHist(); // -- should be produced at the last step (as it normalized the original histograms)
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
    SampleInfo* sampleInfo_DYMuMu_M10to50  = new SampleInfo("DYMuMu_M10to50");
    SampleInfo* sampleInfo_DYMuMu_M50toInf = new SampleInfo("DYMuMu_M50toInf");

    SampleInfo* sampleInfo_DYTauTau_M10to50  = new SampleInfo("DYTauTau_M10to50");
    SampleInfo* sampleInfo_DYTauTau_M50toInf = new SampleInfo("DYTauTau_M50toInf");

    SampleInfo* sampleInfo_ttbar = new SampleInfo("ttbar");
    SampleInfo* sampleInfo_WJets = new SampleInfo("WJets");
    SampleInfo* sampleInfo_QCDMuEnriched_Pt15to20    = new SampleInfo("QCDMuEnriched_Pt15to20");
    SampleInfo* sampleInfo_QCDMuEnriched_Pt20to30    = new SampleInfo("QCDMuEnriched_Pt20to30");
    SampleInfo* sampleInfo_QCDMuEnriched_Pt30to50    = new SampleInfo("QCDMuEnriched_Pt30to50");
    SampleInfo* sampleInfo_QCDMuEnriched_Pt50to80    = new SampleInfo("QCDMuEnriched_Pt50to80");
    SampleInfo* sampleInfo_QCDMuEnriched_Pt80to120   = new SampleInfo("QCDMuEnriched_Pt80to120");
    SampleInfo* sampleInfo_QCDMuEnriched_Pt120to170  = new SampleInfo("QCDMuEnriched_Pt120to170");
    SampleInfo* sampleInfo_QCDMuEnriched_Pt170to300  = new SampleInfo("QCDMuEnriched_Pt170to300");
    SampleInfo* sampleInfo_QCDMuEnriched_Pt300to470  = new SampleInfo("QCDMuEnriched_Pt300to470");
    SampleInfo* sampleInfo_QCDMuEnriched_Pt470to600  = new SampleInfo("QCDMuEnriched_Pt470to600");
    SampleInfo* sampleInfo_QCDMuEnriched_Pt600to800  = new SampleInfo("QCDMuEnriched_Pt600to800");
    SampleInfo* sampleInfo_QCDMuEnriched_Pt800to1000 = new SampleInfo("QCDMuEnriched_Pt800to1000");
    SampleInfo* sampleInfo_QCDMuEnriched_Pt1000toInf = new SampleInfo("QCDMuEnriched_Pt1000toInf");


    // -- DY
    TH1D* h_DYMuMu_M10to50  = sampleInfo_DYMuMu_M10to50->Get_NormalizedHist(histName_);
    TH1D* h_DYMuMu_M50toInf = sampleInfo_DYMuMu_M50toInf->Get_NormalizedHist(histName_);

    h_DYMuMu_ = (TH1D*)h_DYMuMu_M10to50->Clone();
    h_DYMuMu_->Add( h_DYMuMu_M50toInf );

    // -- DYTauTau
    TH1D* h_DYTauTau_M10to50  = sampleInfo_DYTauTau_M10to50->Get_NormalizedHist(histName_);
    TH1D* h_DYTauTau_M50toInf = sampleInfo_DYTauTau_M50toInf->Get_NormalizedHist(histName_);

    h_DYTauTau_ = (TH1D*)h_DYTauTau_M10to50->Clone();
    h_DYTauTau_->Add( h_DYTauTau_M50toInf );

    // -- QCD
    TH1D* h_QCDMuEnriched_Pt15to20    = sampleInfo_QCDMuEnriched_Pt15to20->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt20to30    = sampleInfo_QCDMuEnriched_Pt20to30->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt30to50    = sampleInfo_QCDMuEnriched_Pt30to50->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt50to80    = sampleInfo_QCDMuEnriched_Pt50to80->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt80to120   = sampleInfo_QCDMuEnriched_Pt80to120->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt120to170  = sampleInfo_QCDMuEnriched_Pt120to170->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt170to300  = sampleInfo_QCDMuEnriched_Pt170to300->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt300to470  = sampleInfo_QCDMuEnriched_Pt300to470->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt470to600  = sampleInfo_QCDMuEnriched_Pt470to600->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt600to800  = sampleInfo_QCDMuEnriched_Pt600to800->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt800to1000 = sampleInfo_QCDMuEnriched_Pt800to1000->Get_NormalizedHist(histName_);
    TH1D* h_QCDMuEnriched_Pt1000toInf = sampleInfo_QCDMuEnriched_Pt1000toInf->Get_NormalizedHist(histName_);

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
    h_ttbar_ = sampleInfo_ttbar->Get_NormalizedHist(histName_);
    h_WJets_ = sampleInfo_WJets->Get_NormalizedHist(histName_);


    // TH1D* h_data_RunA = PlotTool::Get_Hist("ROOTFile_MakeHist_Dimuon_noWeight_ScoutingCaloMuon_Run2018A.root", histName_); h_data_RunA->Sumw2();
    // TH1D* h_data_RunB = PlotTool::Get_Hist("ROOTFile_MakeHist_Dimuon_noWeight_ScoutingCaloMuon_Run2018B.root", histName_); h_data_RunB->Sumw2();
    // TH1D* h_data_RunC = PlotTool::Get_Hist("ROOTFile_MakeHist_Dimuon_noWeight_ScoutingCaloMuon_Run2018C.root", histName_); h_data_RunC->Sumw2();
    // TH1D* h_data_RunD = PlotTool::Get_Hist("ROOTFile_MakeHist_Dimuon_noWeight_ScoutingCaloMuon_Run2018D.root", histName_); h_data_RunD->Sumw2();

    // h_data_RunA->Scale( -1.0 );
    // h_data_RunB->Scale( -1.0 );
    // h_data_RunC->Scale( -1.0 );
    // h_data_RunD->Scale( -1.0 );

    // TH1D* h_data = (TH1D*)h_data_RunA->Clone();
    // h_data->Add( h_data_RunB );
    // h_data->Add( h_data_RunC );
    // h_data->Add( h_data_RunD );

    h_data_ = PlotTool::Get_Hist("ROOTFile_MakeHist_Dimuon_noWeight_ScoutingCaloMuon_Run2018All.root", histName_); h_data_->Sumw2();
  }

  void Produce_FractionPlot()
  {
    TH1D* h_totMC = (TH1D*)h_DYTauTau_->Clone();
    h_totMC->Add( h_ttbar_ );
    h_totMC->Add( h_WJets_ );
    h_totMC->Add( h_DYMuMu_ );
    h_totMC->Add( h_QCDMuEnriched_ );

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

    SetRebin(canvas);

    canvas->SetLegendColumn(2);
    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);
    TString titleX = GetTitleX();
    canvas->SetTitle(titleX, "Fraction w.r.t. total MC");
    
    SetRangeX(canvas);
    canvas->SetRangeY(0, 1.1);
    canvas->Latex_CMSSim();
    // canvas->RegisterLatex(0.75, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", lumi/1000.0));
    canvas->Draw("HISTLP");
  }

  void Produce_StackedPlot()
  {
    // -- plots
    TString canvasName = histName_;
    canvasName.ReplaceAll("h_", "c_");
    Bool_t isLogX, isLogY;
    GetLogXY(isLogX, isLogY);
    PlotTool::HistStackCanvaswRatio *canvas = new PlotTool::HistStackCanvaswRatio(canvasName, isLogX, isLogY);

    canvas->RegisterData(h_data_, "Data (Run2018A-D)", kBlack);

    canvas->Register(h_DYTauTau_,      "Z/#gamma #rightarrow #tau#tau (M > 10 GeV)", DYTool::kDYTauTau);
    canvas->Register(h_ttbar_,         "t#bar{t} (leptonic)", DYTool::kTop);
    canvas->Register(h_WJets_,         "W+jets", DYTool::kWJets);
    if( histName_.Contains("ZPeak") ) // -- DY is dominant ... it should come at the last step
    {
      canvas->Register(h_QCDMuEnriched_, "QCD (p_{T}(#mu) > 5 GeV, #mu-enriched)", DYTool::kQCD);
      canvas->Register(h_DYMuMu_,        "Z/#gamma #rightarrow #mu#mu (M > 10 GeV)", DYTool::kDY);
    }
    else
    {
      canvas->Register(h_DYMuMu_,        "Z/#gamma #rightarrow #mu#mu (M > 10 GeV)", DYTool::kDY);
      canvas->Register(h_QCDMuEnriched_, "QCD (p_{T}(#mu) > 5 GeV, #mu-enriched)", DYTool::kQCD);
    }

    SetRebin(canvas);

    canvas->SetLegendColumn(2);
    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);
    TString titleX = GetTitleX();
    canvas->SetTitle(titleX, "Entry", "data/MC");

    SetRangeX(canvas);
    // canvas->SetRangeRatio(0.88, 1.12);
    canvas->SetRangeRatio(0.2, 1.4);
    if( !histName_.Contains("ZPeak") ) canvas->SetRangeRatio(0.2, 1.4);

    canvas->Latex_CMSPre();
    canvas->RegisterLatex(0.75, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", lumi/1000.0));

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

    SetRebin(canvas);

    canvas->SetLegendColumn(2);
    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);
    TString titleX = GetTitleX();
    canvas->SetTitle(titleX, "Entry (norm. to 1)");
    
    SetRangeX(canvas);
    canvas->SetRangeY(0, 0.1);
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
  }

  void SetRangeX( PlotTool::CanvasBase* canvas )
  { 
    if( histName_.Contains("h_diMuM") )        canvas->SetRangeX( 10, 120 );
    if( histName_.Contains("ZPeak/h_diMuM") )  canvas->SetRangeX( 60, 120 );
    if( histName_.Contains("h_diMuPt") )       canvas->SetRangeX( 0, 200 );
    if( histName_.Contains("h_pt") )           canvas->SetRangeX( 0, 150 );
    if( histName_.Contains("h_normChi2") )     canvas->SetRangeX( 0, 12 );
    if( histName_.Contains("h_nMuonHit") )     canvas->SetRangeX( 0, 80 );
    if( histName_.Contains("h_nMatchedStation") ) canvas->SetRangeX( 0, 5 );
    if( histName_.Contains("h_relTrkIso") ) canvas->SetRangeX( 0, 0.20 );
  }

  TString GetTitleX()
  {
    TString titleX = "undefined";
    if( histName_.Contains("h_diMuM") )   titleX = "m(#mu#mu) [GeV]";
    if( histName_.Contains("h_diMuRap") ) titleX = "y(#mu#mu)";
    if( histName_.Contains("h_diMuPt") )  titleX = "p_{T}(#mu#mu) [GeV]";
    if( histName_.Contains("h_diMuOS") )  titleX = "Opposite sign";
    if( histName_.Contains("h_diMuDEta") )  titleX = "#Delta#eta(#mu_{1}, #mu_{2})";
    if( histName_.Contains("h_diMuDPhi") )  titleX = "#Delta#phi(#mu_{1}, #mu_{2})";
    if( histName_.Contains("h_diMuAngle3D") )    titleX = "3D angle(#mu_{1}, #mu_{2})";
    if( histName_.Contains("h_diMuDEtaCM") )     titleX = "#Delta#eta(#mu_{1}, #mu_{2}) (CM frame)";
    if( histName_.Contains("h_diMuDPhiCM") )     titleX = "#Delta#phi(#mu_{1}, #mu_{2}) (CM frame)";
    if( histName_.Contains("h_diMuAngle3DCM") )  titleX = "3D angle(#mu_{1}, #mu_{2}) (CM frame)";


    if( histName_.Contains("h_pt") )  titleX  = "p_{T}(#mu) [GeV]";
    if( histName_.Contains("h_eta") ) titleX = "#eta(#mu)";
    if( histName_.Contains("h_phi") ) titleX = "#phi(#mu)";

    if( histName_.Contains("h_nMuonHit") )         titleX  = "# muon hits";
    if( histName_.Contains("h_nMatchedStation") )  titleX  = "# matched muon station";
    if( histName_.Contains("h_nPixelHit") )        titleX  = "# pixel hits";

    if( histName_.Contains("h_nTrackerLayer") )   titleX  = "# tracker layers";
    if( histName_.Contains("h_normChi2") )        titleX  = "Normalized #chi^{2}";
    if( histName_.Contains("h_dxy") )             titleX  = "dxy [mm]";
    if( histName_.Contains("h_dz") )              titleX  = "dz [mm]";

    if( histName_.Contains("h_relTrkIso") )   titleX  = "Tracker isolation / p_{T}";

    return titleX;
  }

};

void MakePlots()
{
  vector<TString> vec_histName = {
    "ZPeak/h_diMuM", "ZPeak/h_diMuRap", "ZPeak/h_diMuPt",
    "ZPeak/h_pt", "ZPeak/h_eta", "ZPeak/h_phi", 
    "ZPeak/h_nMuonHit", "ZPeak/h_nMatchedStation", "ZPeak/h_nPixelHit", 
    "ZPeak/h_nTrackerLayer", "ZPeak/h_normChi2", "ZPeak/h_relTrkIso",
    "ZPeak/h_diMuOS", "ZPeak/h_diMuDEta", "ZPeak/h_diMuDPhi", "ZPeak/h_diMuAngle3D", 
    "ZPeak/h_diMuDEtaCM", "ZPeak/h_diMuDPhiCM", "ZPeak/h_diMuAngle3DCM",

    "M10to60/h_diMuM", "M10to60/h_diMuRap", "M10to60/h_diMuPt",
    "M10to60/h_pt", "M10to60/h_eta", "M10to60/h_phi", 
    "M10to60/h_nMuonHit", "M10to60/h_nMatchedStation", "M10to60/h_nPixelHit", 
    "M10to60/h_nTrackerLayer", "M10to60/h_normChi2", "M10to60/h_relTrkIso",
    "M10to60/h_diMuOS", "M10to60/h_diMuDEta", "M10to60/h_diMuDPhi", "M10to60/h_diMuAngle3D", 
    "M10to60/h_diMuDEtaCM", "M10to60/h_diMuDPhiCM", "M10to60/h_diMuAngle3DCM",

    "h_diMuM", "h_diMuRap", "h_diMuPt",
    "h_pt", "h_eta", "h_phi", 
    "h_nMuonHit", "h_nMatchedStation", "h_nPixelHit", 
    "h_nTrackerLayer", "h_normChi2", "h_relTrkIso",
    "h_diMuOS", "h_diMuDEta", "h_diMuDPhi", "h_diMuAngle3D",
    "h_diMuDEtaCM", "h_diMuDPhiCM", "h_diMuAngle3DCM"
  };

  for(const auto& histName : vec_histName )
  {
    PlotProducer *producer = new PlotProducer(histName);
    producer->Produce();
  }

}