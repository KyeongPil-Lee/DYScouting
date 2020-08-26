class PlotProducer
{
public:
  TString fileName_M10to12_;
  TString fileName_M10to11_;
  TString fileName_M11to12_;

  PlotProducer(TString fileName_M10to12, TString fileName_M10to11, TString fileName_M11to12)
  {
    fileName_M10to12_ = fileName_M10to12;
    fileName_M10to11_ = fileName_M10to11;
    fileName_M11to12_ = fileName_M11to12;
  }

  void Produce()
  {

  }

private:
  void ProducePlot_M10to50(TString histName, Bool_t doNorm = kFALSE)
  {
    TH1D* h_LO  = PlotTool::Get_Hist(fileName_LO_,  histName);
    TH1D* h_NLO = PlotTool::Get_Hist(fileName_NLO_, histName);
    TH1D* h_LOWeighted = PlotTool::Get_Hist(fileName_LOWeighted_, histName);

    // Double_t sumWeight_LO_M10to50       = SumWeight("LO");
    // Double_t sumWeight_LO_subMassRange  = GetValue_OneBinHist( fileName_LO_, "h_sumWeight" );
    // Double_t frac_sumWeight_LO          = (sumWeight_LO_subMassRange / sumWeight_LO_M10to50);
    // Double_t xSec_subMassRange_LO       = xSec("M10to50") * frac_sumWeight_LO;

    // Double_t sumWeight_NLO_M10to50       = SumWeight("NLO");
    // Double_t sumWeight_NLO_subMassRange  = GetValue_OneBinHist( fileName_NLO_, "h_sumWeight" );
    // Double_t frac_sumWeight_NLO          = (sumWeight_NLO_subMassRange / sumWeight_NLO_M10to50);
    // Double_t xSec_subMassRange_NLO       = xSec("M10to50") * frac_sumWeight_NLO;

    // Double_t sumWeight_LOWeighted_M10to50       = SumWeight("LOWeighted");
    // Double_t sumWeight_LOWeighted_subMassRange  = GetValue_OneBinHist( fileName_LOWeighted_, "h_sumWeight" );
    // Double_t frac_sumWeight_LOWeighted          = (sumWeight_LOWeighted_subMassRange / sumWeight_LOWeighted_M10to50);
    // Double_t xSec_subMassRange_LOWeighted       = xSec("M10to50") * frac_sumWeight_LOWeighted;

    // Double_t normFactor_LO         = ( xSec_subMassRange_LO )         / sumWeight_LO_subMassRange;
    // Double_t normFactor_NLO        = ( xSec_subMassRange_NLO )        / sumWeight_NLO_subMassRange;
    // Double_t normFactor_LOWeighted = ( xSec_subMassRange_LOWeighted ) / sumWeight_LOWeighted_subMassRange;

    Double_t normFactor_LO         = ( xSec("M10to50") ) / SumWeight("LO");
    Double_t normFactor_NLO        = ( xSec("M10to50") ) / SumWeight("NLO");
    Double_t normFactor_LOWeighted = ( xSec("M10to50") ) / SumWeight("LOWeighted");

    h_LO->Scale( normFactor_LO );
    h_NLO->Scale( normFactor_NLO );
    h_LOWeighted->Scale( normFactor_LOWeighted );

    // -- differential cross section
    h_LO         = PlotTool::DivideEachBin_ByBinWidth(h_LO);
    h_NLO        = PlotTool::DivideEachBin_ByBinWidth(h_NLO);
    h_LOWeighted = PlotTool::DivideEachBin_ByBinWidth(h_LOWeighted);

    TString canvasName = histName;
    canvasName.ReplaceAll("h_", "c_");
    if( doNorm ) canvasName = canvasName + "_norm";

    Bool_t isLogX = kFALSE;
    if( histName == "h_diMuPt" ) isLogX = kTRUE;

    Bool_t isLogY = kTRUE;
    if( doNorm ) isLogY = kFALSE;

    PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio(canvasName, isLogX, isLogY);

    TString titleX = "";
    if( histName == "h_diMuM" )   titleX = "m(#mu#mu) [GeV]";
    if( histName == "h_diMuPt" )  titleX = "p_{T}^{#mu#mu} [GeV]";
    if( histName == "h_diMuRap" ) titleX = "y^{#mu#mu}";

    TString titleY = "";
    if( histName == "h_diMuM" )   titleY = "d#sigma/dm [pb/GeV]";
    if( histName == "h_diMuPt" )  titleY = "d#sigma/dp_{T}^{#mu#mu} [pb/GeV]";
    if( histName == "h_diMuRap" ) titleY = "d#sigma/dy^{#mu#mu} [pb]";
    canvas->SetTitle(titleX, titleY, "ratio to Red");

    // canvas->Register( h_DYMuMu_merged, "M10-50 + M50-inf", kBlack);
    canvas->Register( h_LO,         "10 < m < 12 GeV (LO)",  kRed);
    canvas->Register( h_NLO,        "10 < m < 12 GeV (NLO)", kGreen+2);
    canvas->Register( h_LOWeighted, "10 < m < 12 GeV (LO, reweighted to NLO)",  kViolet);

    if( histName == "h_diMuM" )
    {
      canvas->SetRangeX( 10, 50 );
      canvas->SetRangeY( 1e2, 1e4 );
      canvas->SetRangeRatio(0.8, 1.2);

      if( doNorm ) canvas->SetRangeY( 0, 0.25 );
    }

    if( histName == "h_diMuPt" )
    {
      canvas->SetRangeX( 0, 1000 );
      canvas->SetRangeY( 1e-3, 1e5 );
      canvas->SetRangeRatio(0.7, 1.8);
    }

    if( histName == "h_diMuRap" )
    {
      canvas->SetRangeX( -10, 10 );
      canvas->SetRangeY( 1e1, 1e3 );
      canvas->SetRangeRatio(0.7, 1.8);
    }

    canvas->SetLegendPosition(0.15, 0.75, 0.80, 0.90);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{generator level (fromHardProcessFinalState)}}");
    canvas->Latex_CMSSim();

    canvas->Draw();
  }

};

void ProducePlot()
{
  TString fileName_M10to12 = "";
  TString fileName_M10to11 = "";
  TString fileName_M11to12 = "";

  PlotProducer *producer = new PlotProducer(fileName_M10to12, fileName_M10to11, fileName_M11to12);
  producer->Produce();
}