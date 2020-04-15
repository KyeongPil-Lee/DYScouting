
#include <Include/DYTool.h>
// #include <ControlPlot/DYHistInfo.h>
#include <Include/SimplePlotTools.h>

const Double_t lumi = 61300; // -- 61.3 /fb

class HistContainer
{
public:
  TString basePath_; // -- directory where root files exist
  TString histName_;

  // -- histograms
  TH1D* h_data_;
  TH1D* h_DYTauTau_;
  TH1D* h_ttbar_;
  TH1D* h_WJets_;
  TH1D* h_QCDMuEnriched_;
  TH1D* h_DYMuMu_;

  TString tag_data_;
  TString tag_ttbar_;
  TString tag_WJets_;

  vector<TString> vec_tag_DYMuMu_;
  vector<TString> vec_tag_DYTauTau_;
  vector<TString> vec_tag_QCDMuEnriched_;


  HistContainer(TString basePath, TString histName)
  {
    basePath_ = basePath;
    histName_ = histName;

    Init();
  }

  TH1D* Get_Histogram(TString type)
  {
    if( type == "data" )               return (TH1D*)h_data_->Clone();
    else if( type == "DYTauTau" )      return (TH1D*)h_DYTauTau_->Clone();
    else if( type == "ttbar" )         return (TH1D*)h_ttbar_->Clone();
    else if( type == "WJets" )         return (TH1D*)h_WJets_->Clone();
    else if( type == "QCDMuEnriched" ) return (TH1D*)h_QCDMuEnriched_->Clone();
    else if( type == "DYMuMu" )        return (TH1D*)h_DYMuMu_->Clone();
    else
      cout << "no corresponding histogram for " << type << " is found" << endl;

    return nullptr;
  }

private:
  void Init()
  {
    h_data_     = nullptr;
    h_DYTauTau_ = nullptr;
    h_ttbar_    = nullptr;
    h_WJets_    = nullptr;
    h_QCDMuEnriched_ = nullptr;
    h_DYMuMu_   = nullptr;

    tag_data_  = "ScoutingCaloMuon_Run2018All";
    tag_ttbar_ = "ttbar";
    tag_WJets_ = "WJets";

    vec_tag_DYMuMu_   = { "DYMuMu_M10to50", "DYMuMu_M50toInf" };
    vec_tag_DYTauTau_ = { "DYTauTau_M10to50", "DYTauTau_M50toInf" };
    vec_tag_QCDMuEnriched_ = { 
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
      "QCDMuEnriched_Pt1000toInf"
    };

    Init_Histogram(h_data_,  tag_data_);
    Init_Histogram(h_ttbar_, tag_ttbar_);
    Init_Histogram(h_WJets_, tag_WJets_);

    Init_MergedHistogram(h_DYMuMu_,        vec_tag_DYMuMu_);
    Init_MergedHistogram(h_DYTauTau_,      vec_tag_DYTauTau_);
    Init_MergedHistogram(h_QCDMuEnriched_, vec_tag_QCDMuEnriched_);
  }

  void Init_Histogram(TH1D*& h, TString tag)
  {
    TString fileName = basePath_+"/ROOTFile_MakeHist_Dimuon_noWeight_"+tag+".root";
    h = PlotTool::Get_Hist(fileName, histName_);
    h->Sumw2();
    RemoveNegativeBin(h, tag);

    if( tag != tag_data_ ) // -- MC
    {
      Double_t xSec      = Get_xSec(tag);
      Double_t sumWeight = Get_SumWeight(tag);
      Double_t normFactor = (lumi * xSec ) / sumWeight;
      h->Scale( normFactor );
    }
  }

  void RemoveNegativeBin(TH1D* h, TString tag)
  {
    Int_t nBin = h->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t binContent = h->GetBinContent(i_bin);
      if( binContent < 0 )
      {
        cout << "[" << h->GetName() << ", " << tag << "] binContent (" << i_bin << " bin) = " << binContent << " < 0 ... -> set it as 0" << endl;
        h->SetBinContent(i_bin, 0);
      }
    }
  }

  void Init_MergedHistogram(TH1D*& h, vector<TString> vec_tag)
  {
    for(auto tag : vec_tag)
    {
      TH1D* h_temp = nullptr;
      Init_Histogram( h_temp, tag );

      if( h == nullptr ) { h = (TH1D*)h_temp->Clone(); h->Sumw2(); }
      else               h->Add( h_temp );
    }
  }

  Double_t Get_SumWeight(TString tag)
  {
    Double_t sumWeight = 0;

    if( tag == "DYMuMu_M10to50" )                 sumWeight = 28985097.0;
    else if( tag == "DYMuMu_M50toInf" )           sumWeight = 44318555.0;
    else if( tag == "DYTauTau_M10to50" )          sumWeight = 28843598.0;
    else if( tag == "DYTauTau_M50toInf" )         sumWeight = 44279551.0;
    else if( tag == "ttbar" )                     sumWeight = 63811310.0;
    else if( tag == "WJets" )                     sumWeight = 71061720.0;
    else if( tag == "QCDMuEnriched_Pt20toInf" )   sumWeight = 22165320.0;
    else if( tag == "QCDMuEnriched_Pt15to20" )    sumWeight = 4572168.0;
    else if( tag == "QCDMuEnriched_Pt20to30" )    sumWeight = 26549827.0;
    else if( tag == "QCDMuEnriched_Pt30to50" )    sumWeight = 30431342.0;
    else if( tag == "QCDMuEnriched_Pt50to80" )    sumWeight = 20360155.0;
    else if( tag == "QCDMuEnriched_Pt80to120" )   sumWeight = 25652280.0;
    else if( tag == "QCDMuEnriched_Pt120to170" )  sumWeight = 21315922.0;
    else if( tag == "QCDMuEnriched_Pt170to300" )  sumWeight = 36372547.0;
    else if( tag == "QCDMuEnriched_Pt300to470" )  sumWeight = 29488563.0;
    else if( tag == "QCDMuEnriched_Pt470to600" )  sumWeight = 20542857.0;
    else if( tag == "QCDMuEnriched_Pt600to800" )  sumWeight = 16891461.0;
    else if( tag == "QCDMuEnriched_Pt800to1000" ) sumWeight = 16749914.0;
    else if( tag == "QCDMuEnriched_Pt1000toInf" ) sumWeight = 11039499.0;
    else
    {
      cout << "no information for tag = " << tag << endl;
      sumWeight = 0;
    }

    return sumWeight;
  }

  Double_t Get_xSec(TString tag)
  {
    Double_t xSec = 0;

    if( tag == "DYMuMu_M10to50" )                 xSec = 6270.0;
    else if( tag == "DYMuMu_M50toInf" )           xSec = 2009.41;
    else if( tag == "DYTauTau_M10to50" )          xSec = 6270.0;
    else if( tag == "DYTauTau_M50toInf" )         xSec = 2009.41;
    else if( tag == "ttbar" )                     xSec = 88.29;
    else if( tag == "WJets" )                     xSec = 61526.7;
    else if( tag == "QCDMuEnriched_Pt20toInf" )   xSec = 239400.0;
    else if( tag == "QCDMuEnriched_Pt15to20" )    xSec = 2799000.0;
    else if( tag == "QCDMuEnriched_Pt20to30" )    xSec = 2526000.0;
    else if( tag == "QCDMuEnriched_Pt30to50" )    xSec = 1362000.0;
    else if( tag == "QCDMuEnriched_Pt50to80" )    xSec = 376600.0;
    else if( tag == "QCDMuEnriched_Pt80to120" )   xSec = 88930.0;
    else if( tag == "QCDMuEnriched_Pt120to170" )  xSec = 21230.0;
    else if( tag == "QCDMuEnriched_Pt170to300" )  xSec = 7055.0;
    else if( tag == "QCDMuEnriched_Pt300to470" )  xSec = 797.3; // -- TuneCUETP8M1
    else if( tag == "QCDMuEnriched_Pt470to600" )  xSec = 59.24;
    else if( tag == "QCDMuEnriched_Pt600to800" )  xSec = 25.25; // -- TuneCUETP8M1
    else if( tag == "QCDMuEnriched_Pt800to1000" ) xSec = 4.723; // -- TuneCUETP8M1
    else if( tag == "QCDMuEnriched_Pt1000toInf" ) xSec = 1.613; // -- TuneCUETP8M1
    else
    {
      cout << "no information for tag = " << tag << endl;
      xSec = 0;
    }

    return xSec;
  }
};

class QCDEstimator
{
public:
  // -- in the region with nominal MVA cut (signal-dominant in OS)
  HistContainer* hists_R1OS_;
  HistContainer* hists_R1SS_;

  // -- in the region with reversed MVA cut (background-dominant)
  HistContainer* hists_R2OS_;
  HistContainer* hists_R2SS_;

  TH1D* h_data_R2OS_EWKSub_;
  TH1D* h_data_R2SS_EWKSub_;

  TH1D* h_ratio_R2_OSSS_;

  TH1D* h_data_R1SS_;
  TH1D* h_QCD_dataDriven_;

  // -- for validation
  TH1D* h_ratio_R2_OSSS_QCDMC_;
  TH1D* h_ratio_R1_OSSS_;
  TH1D* h_ratio_R1_OSSS_QCDMC_;

  // -- Upsilon
  Bool_t UpsilonAvailable_ = kFALSE;
  TH1D* h_diMuM_Upsilon_R1OS_;
  TH1D* h_diMuM_Upsilon_R2OS_;

  Bool_t useMCDrivenFactor_ = kFALSE;


  QCDEstimator(TString basePath_R1, TString basePath_R2)
  {
    hists_R1OS_ = new HistContainer(basePath_R1, "M10to60_OS/h_diMuM");
    hists_R1SS_ = new HistContainer(basePath_R1, "M10to60_SS/h_diMuM");

    hists_R2OS_ = new HistContainer(basePath_R2, "M10to60_OS/h_diMuM");
    hists_R2SS_ = new HistContainer(basePath_R2, "M10to60_SS/h_diMuM");
  }

  void Set_Upsilon(TString fileName_R1OS, TString histName_R1OS, TString fileName_R2OS, TString histName_R2OS)
  {
    UpsilonAvailable_ = kTRUE;
    h_diMuM_Upsilon_R1OS_ = PlotTool::Get_Hist(fileName_R1OS, histName_R1OS);
    h_diMuM_Upsilon_R2OS_ = PlotTool::Get_Hist(fileName_R2OS, histName_R2OS);
  }

  void Estimate()
  {
    h_data_R2OS_EWKSub_ = SubtractEWKProcess(hists_R2OS_, "R2OS");
    h_data_R2SS_EWKSub_ = SubtractEWKProcess(hists_R2SS_, "R2SS");

    h_ratio_R2_OSSS_ = (TH1D*)h_data_R2OS_EWKSub_->Clone();
    h_ratio_R2_OSSS_->Divide( h_data_R2OS_EWKSub_, h_data_R2SS_EWKSub_ );

    h_data_R1SS_ = hists_R1SS_->Get_Histogram("data");
    if( useMCDrivenFactor_ )
    {
      Double_t ratio_OSSS_QCDMC = 1.591; // -- flat
      h_QCD_dataDriven_ = (TH1D*)h_data_R1SS_->Clone();
      h_QCD_dataDriven_->Scale( ratio_OSSS_QCDMC );
    }
    else
      h_QCD_dataDriven_ = MultiplyConversionFactor_BinByBin(h_data_R1SS_, h_ratio_R2_OSSS_);

    Produce_ValidationPlots();
  }

  void Produce_ValidationPlots()
  {
    ProducePlot_Stacked(hists_R1OS_, "diMuM_R1OS");
    ProducePlot_Stacked(hists_R1SS_, "diMuM_R1SS");

    ProducePlot_Stacked(hists_R2OS_, "diMuM_R2OS");
    ProducePlot_Stacked(hists_R2SS_, "diMuM_R2SS");

    ComparisonPlot_beforeAfterSubtract( hists_R2OS_->Get_Histogram("data"), h_data_R2OS_EWKSub_, "R2OS" );
    ComparisonPlot_beforeAfterSubtract( hists_R2SS_->Get_Histogram("data"), h_data_R2SS_EWKSub_, "R2SS" );

    // -- for validation: check the ratio from QCD MC
    TH1D* h_QCD_R2OS = hists_R2OS_->Get_Histogram("QCDMuEnriched");
    TH1D* h_QCD_R2SS = hists_R2SS_->Get_Histogram("QCDMuEnriched");
    h_ratio_R2_OSSS_QCDMC_ = (TH1D*)h_QCD_R2OS->Clone();
    h_ratio_R2_OSSS_QCDMC_->Divide( h_QCD_R2OS, h_QCD_R2SS );

    // -- for validation: check the ratio from R1 data
    TH1D* h_data_R1OS_EWKSub = SubtractEWKProcess(hists_R1OS_, "R1OS");
    cout << "EWK subtracted R1OS: Remove negative bins" << endl;
    RemoveNegativeBin(h_data_R1OS_EWKSub, "R1OS");

    TH1D* h_data_R1SS_EWKSub = SubtractEWKProcess(hists_R1SS_, "R1SS");

    ComparisonPlot_beforeAfterSubtract( hists_R1OS_->Get_Histogram("data"), h_data_R1OS_EWKSub, "R1OS_ForVal" );
    ComparisonPlot_beforeAfterSubtract( hists_R1SS_->Get_Histogram("data"), h_data_R1SS_EWKSub, "R1SS_ForVal" );

    h_ratio_R1_OSSS_ = (TH1D*)h_data_R1OS_EWKSub->Clone();
    h_ratio_R1_OSSS_->Divide( h_data_R1OS_EWKSub, h_data_R1SS_EWKSub );

    // -- for validation: check the ratio from R1 QCD MC
    TH1D* h_QCD_R1OS = hists_R1OS_->Get_Histogram("QCDMuEnriched");
    TH1D* h_QCD_R1SS = hists_R1SS_->Get_Histogram("QCDMuEnriched");
    h_ratio_R1_OSSS_QCDMC_ = (TH1D*)h_QCD_R1OS->Clone();
    h_ratio_R1_OSSS_QCDMC_->Divide( h_QCD_R1OS, h_QCD_R1SS );

    ComparisonPlot_Ratio();

    ComparisonPlot_QCD_MC_DataDriven(hists_R1OS_->Get_Histogram("QCDMuEnriched"), h_QCD_dataDriven_);

    StackedPlot_UseDataDrivenQCD(h_QCD_dataDriven_);
  }

private:
  TH1D* SubtractEWKProcess(HistContainer* hists, TString type)
  {
    TH1D* h_data          = hists->Get_Histogram("data");
    TH1D* h_ttbar         = hists->Get_Histogram("ttbar");
    TH1D* h_WJets         = hists->Get_Histogram("WJets");
    TH1D* h_DYTauTau      = hists->Get_Histogram("DYTauTau");
    TH1D* h_DYMuMu        = hists->Get_Histogram("DYMuMu");
    TH1D* h_QCDMuEnriched = hists->Get_Histogram("QCDMuEnriched");

    TH1D* h_data_sub = (TH1D*)h_data->Clone();
    h_data_sub->Add( h_ttbar,    -1.0 );
    h_data_sub->Add( h_WJets,    -1.0 );
    h_data_sub->Add( h_DYTauTau, -1.0 );
    h_data_sub->Add( h_DYMuMu,   -1.0 );

    if( UpsilonAvailable_ )
    {
      if( type == "R1OS" ) h_data_sub->Add( h_diMuM_Upsilon_R1OS_, -1.0 );
      if( type == "R2OS" ) h_data_sub->Add( h_diMuM_Upsilon_R2OS_, -1.0 );
    }

    return h_data_sub;
  }

  void ComparisonPlot_Ratio()
  {
    TString canvasName = "c_comp_ratio";

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 0, 0);
    canvas->SetTitle( "m (#mu#mu) [GeV]", "Ratio (OS/SS)", "ratio to black");

    canvas->Register( h_ratio_R2_OSSS_,       "Region 2, data", kBlack);
    canvas->Register( h_ratio_R2_OSSS_QCDMC_, "Region 2, QCD #mu-enriched MC", DYTool::kQCD);
    // canvas->Register( h_ratio_R1_OSSS_,       "Region 1, data", kRed);
    canvas->Register( h_ratio_R1_OSSS_QCDMC_, "Region 1, QCD #mu-enriched MC", kBlue);

    canvas->Latex_CMSPre();
    canvas->RegisterLatex(0.75, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", lumi/1000.0));

    canvas->SetLegendPosition(0.16, 0.75, 0.60, 0.95);

    canvas->SetRangeRatio( 0.7, 1.3 );
    canvas->SetRangeX(10, 60);
    canvas->SetRangeY(0, 10);

    canvas->Draw();
  }

  void ComparisonPlot_beforeAfterSubtract(TH1D* h_data, TH1D* h_data_subtracted, TString type)
  {
    TString canvasName = "c_comp_beforeAfterSubtraction_"+type;

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 0, 1);
    canvas->SetTitle( "m (#mu#mu) [GeV]", "# events", "after/before");

    canvas->Register( h_data,            "Before EWK subtraction", kBlack);
    canvas->Register( h_data_subtracted, "After EWK subtraction",  kBlue);

    canvas->Latex_CMSPre();
    canvas->RegisterLatex(0.75, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", lumi/1000.0));

    canvas->SetRangeRatio( 0.7, 1.01);
    canvas->SetRangeX(10, 60);
    // canvas->SetAutoRangeY();
    canvas->SetRangeY(0.5, 1e9);
    // canvas->SetRangeY(-1e6, 1e6);

    TString regionInfo = Get_RegionInfo(type);
    canvas->RegisterLatex(0.16, 0.91, TString::Format("#font[42]{#scale[0.7]{%s}}", regionInfo.Data()));

    canvas->Draw();
  }

  void ProducePlot_Stacked(HistContainer* hists, TString type)
  {
    TH1D* h_data          = hists->Get_Histogram("data");
    TH1D* h_ttbar         = hists->Get_Histogram("ttbar");
    TH1D* h_WJets         = hists->Get_Histogram("WJets");
    TH1D* h_QCDMuEnriched = hists->Get_Histogram("QCDMuEnriched");
    TH1D* h_DYTauTau      = hists->Get_Histogram("DYTauTau");
    TH1D* h_DYMuMu        = hists->Get_Histogram("DYMuMu");

    TString canvasName = "c_stacked_"+type;

    Bool_t isLogX, isLogY;
    if( type.Contains("diMuM") ) {isLogX = kFALSE; isLogY = kTRUE; }
    else                         {isLogX = kFALSE; isLogY = kFALSE; }

    PlotTool::HistStackCanvaswRatio *canvas = new PlotTool::HistStackCanvaswRatio(canvasName, isLogX, isLogY);

    canvas->RegisterData(h_data, "Data (Run2018A-D)", kBlack);

    if( type.Contains("SS") )
    {
      canvas->Register(h_DYTauTau,      "Z/#gamma #rightarrow #tau#tau (M > 10 GeV)", DYTool::kDYTauTau);
      canvas->Register(h_ttbar,         "t#bar{t} (leptonic)", DYTool::kTop);
      canvas->Register(h_DYMuMu,        "Z/#gamma #rightarrow #mu#mu (M > 10 GeV)", DYTool::kDY);
      canvas->Register(h_WJets,         "W+jets", DYTool::kWJets);
      canvas->Register(h_QCDMuEnriched, "QCD (p_{T}(#mu) > 5 GeV, #mu-enriched)", DYTool::kQCD);
    }
    else if( type.Contains("R2OS") )
    {
      canvas->Register(h_DYTauTau,      "Z/#gamma #rightarrow #tau#tau (M > 10 GeV)", DYTool::kDYTauTau);
      canvas->Register(h_ttbar,         "t#bar{t} (leptonic)", DYTool::kTop);
      canvas->Register(h_WJets,         "W+jets", DYTool::kWJets);
      canvas->Register(h_DYMuMu,        "Z/#gamma #rightarrow #mu#mu (M > 10 GeV)", DYTool::kDY);
      canvas->Register(h_QCDMuEnriched, "QCD (p_{T}(#mu) > 5 GeV, #mu-enriched)", DYTool::kQCD);
      if( UpsilonAvailable_ )
        canvas->Register(h_diMuM_Upsilon_R2OS_, "Y(1,2,3S) #rightarrow #mu#mu", kRed-9);
    }
    else if( type.Contains("OS") )
    {
      canvas->Register(h_DYTauTau,      "Z/#gamma #rightarrow #tau#tau (M > 10 GeV)", DYTool::kDYTauTau);
      canvas->Register(h_ttbar,         "t#bar{t} (leptonic)", DYTool::kTop);
      canvas->Register(h_WJets,         "W+jets", DYTool::kWJets);
      canvas->Register(h_QCDMuEnriched, "QCD (p_{T}(#mu) > 5 GeV, #mu-enriched)", DYTool::kQCD);
      canvas->Register(h_DYMuMu,        "Z/#gamma #rightarrow #mu#mu (M > 10 GeV)", DYTool::kDY);
      if( UpsilonAvailable_ )
        canvas->Register(h_diMuM_Upsilon_R1OS_, "Y(1,2,3S) #rightarrow #mu#mu", kRed-9);
    }


    canvas->SetLegendColumn(2);
    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);

    if( type.Contains("diMuM") )
    {
      canvas->SetTitle("m [GeV]", "Entry", "data/MC");
      canvas->SetRangeX(10, 60);
    }

    canvas->SetAutoRangeY();
    canvas->SetRangeRatio(0.2, 1.4);

    canvas->Latex_CMSPre();
    canvas->RegisterLatex(0.75, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", lumi/1000.0));

    TString regionInfo = Get_RegionInfo(type);
    canvas->RegisterLatex(0.16, 0.91, TString::Format("#font[42]{#scale[0.7]{%s}}", regionInfo.Data()));

    canvas->Draw();

    Double_t nEvent_data = GetNEvent_MassRange(h_data,          10, 60);
    Double_t nEvent_DY   = GetNEvent_MassRange(h_DYMuMu,        10, 60);
    Double_t nEvent_QCD  = GetNEvent_MassRange(h_QCDMuEnriched, 10, 60);
    printf("[%s] # events in 10 < M < 60 GeV: (data, DY, QCD) = (%.0lf, %.0lf, %.0lf)\n\n", type.Data(), nEvent_data, nEvent_DY, nEvent_QCD);
  }

  void ComparisonPlot_QCD_MC_DataDriven(TH1D* h_MC, TH1D* h_dataDriven)
  {
    TString canvasName = "c_comp_QCD_MC_vs_dataDriven";

    PlotTool::HistCanvaswRatio *canvas = new PlotTool::HistCanvaswRatio(canvasName, 0, 1);
    canvas->SetTitle( "m (#mu#mu) [GeV]", "# events", "data-driven/MC");

    canvas->Register( h_MC,         "#mu-enriched QCD MC", kBlack);
    canvas->Register( h_dataDriven, "QCD estimated by data-driven method",  kBlue);

    canvas->Latex_CMSPre();
    canvas->RegisterLatex(0.75, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", lumi/1000.0));

    canvas->SetRangeRatio(0, 2.5);
    canvas->SetRangeX(10, 60);
    // canvas->SetAutoRangeY();
    canvas->SetRangeY(1e3, 1e6);
    // canvas->SetRangeY(-1e6, 1e6);

    // TString regionInfo = Get_RegionInfo(type);
    // canvas->RegisterLatex(0.16, 0.91, TString::Format("#font[42]{#scale[0.7]{%s}}", regionInfo.Data()));

    canvas->Draw();

  }

  void StackedPlot_UseDataDrivenQCD(TH1D* h_QCD_dataDriven)
  {    
    
    TH1D* h_data          = hists_R1OS_->Get_Histogram("data");
    TH1D* h_ttbar         = hists_R1OS_->Get_Histogram("ttbar");
    TH1D* h_WJets         = hists_R1OS_->Get_Histogram("WJets");
    // TH1D* h_QCDMuEnriched = hists_R1OS_->Get_Histogram("QCDMuEnriched");
    TH1D* h_DYTauTau      = hists_R1OS_->Get_Histogram("DYTauTau");
    TH1D* h_DYMuMu        = hists_R1OS_->Get_Histogram("DYMuMu");

    TString canvasName = "c_diMuM_dataDrivenQCD";

    PlotTool::HistStackCanvaswRatio *canvas = new PlotTool::HistStackCanvaswRatio(canvasName, 0, 1);

    canvas->RegisterData(h_data, "Data (Run2018A-D)", kBlack);

    canvas->Register(h_DYTauTau,      "Z/#gamma #rightarrow #tau#tau (M > 10 GeV)", DYTool::kDYTauTau);
    canvas->Register(h_ttbar,         "t#bar{t} (leptonic)", DYTool::kTop);
    canvas->Register(h_WJets,         "W+jets", DYTool::kWJets);
    canvas->Register(h_QCD_dataDriven, "QCD (data-driven)", DYTool::kQCD);
    canvas->Register(h_DYMuMu,        "Z/#gamma #rightarrow #mu#mu (M > 10 GeV)", DYTool::kDY);
    if( UpsilonAvailable_ )
      canvas->Register(h_diMuM_Upsilon_R1OS_, "Y(1,2,3S) #rightarrow #mu#mu", kRed-9);

    canvas->SetLegendColumn(2);
    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.95);

    canvas->SetTitle("m [GeV]", "Entry", "data/MC");
    canvas->SetRangeX(10, 60);

    canvas->SetAutoRangeY();
    canvas->SetRangeRatio(0.2, 1.4);

    canvas->Latex_CMSPre();
    canvas->RegisterLatex(0.75, 0.96, TString::Format("#font[42]{#scale[0.6]{%.1lf fb^{-1} (13 TeV)}}", lumi/1000.0));

    // TString regionInfo = Get_RegionInfo(type);
    // canvas->RegisterLatex(0.16, 0.91, TString::Format("#font[42]{#scale[0.7]{%s}}", regionInfo.Data()));

    canvas->Draw();
  }

  TString Get_RegionInfo(TString type)
  {
    TString regionInfo = "";

    if( type.Contains("R1") ) regionInfo = "Region 1";
    if( type.Contains("R2") ) regionInfo = "Region 2";

    if( type.Contains("OS") ) regionInfo = regionInfo + " (opposite sign)";
    if( type.Contains("SS") ) regionInfo = regionInfo + " (same sign)";

    return regionInfo;
  }

  void RemoveNegativeBin(TH1D* h, TString type)
  {
    Int_t nBin = h->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t binContent = h->GetBinContent(i_bin);
      if( binContent < 0 )
      {
        cout << "[" << h->GetName() << ", " << type << "] binContent (" << i_bin << " bin) = " << binContent << " < 0 ... -> set it as 0" << endl;
        h->SetBinContent(i_bin, 0);
      }
    }
  }

  TH1D* MultiplyConversionFactor_BinByBin(TH1D* h_data_SS, TH1D* h_factor)
  {
    TH1D* h_result = (TH1D*)h_data_SS->Clone();
    Int_t nBin = h_result->GetNbinsX();

    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t data_SS = h_data_SS->GetBinContent(i_bin);
      Double_t err_data_SS = h_data_SS->GetBinError(i_bin);
      Double_t factor = h_factor->GetBinContent(i_bin);

      h_result->SetBinContent(i_bin, data_SS * factor );
      h_result->SetBinError(i_bin, err_data_SS * factor );
    }

    return h_result;
  }

  Double_t GetNEvent_MassRange(TH1D* h, Double_t min, Double_t max)
  {
    Double_t sum = 0;

    Int_t nBin = h->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t binCenter = h->GetBinCenter(i_bin);
      if( min < binCenter && binCenter < max )
        sum = sum + h->GetBinContent(i_bin);
    }

    return sum;
  }
};

void EstimateQCD()
{
  TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
  TString basePath_R1 = analyzerPath+"/ControlPlot/Optimization/v10_BDT_noOS"; // -- in the region with nominal MVA cut (signal-dominant in OS)
  TString basePath_R2 = analyzerPath+"/BkgEst/SSPair/ReversedMVACut"; // -- in the region with reversed MVA cut (background-dominant)

  QCDEstimator* estimator = new QCDEstimator(basePath_R1, basePath_R2);
  estimator->Set_Upsilon( analyzerPath+"/BkgEst/Upsilon/ROOTFile_Upsilon_R1OS.root", "h_diMuM_Upsilon",
                          analyzerPath+"/BkgEst/Upsilon/ROOTFile_Upsilon_R2OS.root", "h_diMuM_Upsilon" );
  // estimator->useMCDrivenFactor_ = kTRUE;
  estimator->Estimate();
}