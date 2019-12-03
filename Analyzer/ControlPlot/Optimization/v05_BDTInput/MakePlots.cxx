#include <Include/SimplePlotTools.h>

#include <Include/SimplePlotTools.h>
#include <Include/DYTool.h>
#include <ControlPlot/DYHistInfo.h>

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

    // SetRebin(canvas);

    canvas->SetLegendColumn(2);
    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);
    canvas->SetTitle(histInfo_.titleX_, "Entry", "data/MC");

    if( histInfo_.setRangeX_ ) canvas->SetRangeX( histInfo_.minX_, histInfo_.maxX_ );
    canvas->SetAutoRangeY();

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

void MakePlots()
{
  vector<DYHistInfo> vec_histInfo = GetAllDYHist();
  // for(auto histInfo : vec_histInfo )
  //   cout << "histName: " << histInfo.histName_ << endl;

  for(auto histInfo : vec_histInfo )
  {
    // if( histInfo.histName_ != "h_caloMET_pt" ) continue;

    PlotProducer *producer = new PlotProducer(histInfo);
    producer->Produce();
  }
}