#include <Include/SimplePlotTools.h>

// const Double_t lumi = 59740.565202; // -- 59.7  /fb

class PlotProducer
{
public:
  TString fileName_LO_;
  TString fileName_NLO_;
  TString fileName_LOWeighted_;

  PlotProducer( TString fileName_LO, TString fileName_NLO, TString fileName_LOWeighted )
  {
    fileName_LO_ = fileName_LO;
    fileName_NLO_ = fileName_NLO;
    fileName_LOWeighted_ = fileName_LOWeighted;
  }

  void Produce()
  {
    ProducePlot_M10to50("h_diMuM");
    ProducePlot_M10to50("h_diMuPt");
    ProducePlot_M10to50("h_diMuRap");

    // ProducePlot_CombinedWithM50Sample();
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
    canvas->SetTitle(titleX, titleY, "ratio to NLO");

    // canvas->Register( h_DYMuMu_merged, "M10-50 + M50-inf", kBlack);
    canvas->Register( h_NLO,        "10 < m < 50 GeV (NLO)", kGreen+2);
    canvas->Register( h_LO,         "10 < m < 50 GeV (LO)",  kRed);
    canvas->Register( h_LOWeighted, "10 < m < 50 GeV (LO, reweighted to NLO)",  kViolet);

    if( histName == "h_diMuM" )
    {
      canvas->SetRangeX( 10, 50 );
      canvas->SetRangeY( 1, 1e4 );
      canvas->SetRangeRatio(0.84, 1.16);

      if( doNorm ) canvas->SetRangeY( 0, 0.25 );
    }

    if( histName == "h_diMuPt" )
    {
      canvas->SetRangeX( 0, 1000 );
      canvas->SetRangeY( 5e-3, 1e7 );
      canvas->SetRangeRatio(0.4, 1.5);
    }

    if( histName == "h_diMuRap" )
    {
      canvas->SetRangeX( -10, 10 );
      canvas->SetRangeY( 5e1, 1e4 );
      canvas->SetRangeRatio(0.84, 1.16);
    }

    canvas->SetLegendPosition(0.15, 0.75, 0.80, 0.90);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{generator level (fromHardProcessFinalState)}}");
    canvas->Latex_CMSSim();

    canvas->Draw();
  }

  // -- it should use isHardProcess distribution
  // void ProducePlot_CombinedWithM50Sample()
  // {
  //   TH1D* h_LO  = PlotTool::Get_Hist(fileName_LO_,  "h_diMuM");
  //   TH1D* h_NLO = PlotTool::Get_Hist(fileName_NLO_, "h_diMuM");
  //   TH1D* h_LOWeighted = PlotTool::Get_Hist(fileName_LOWeighted_, "h_diMuM");

  //   Double_t normFactor_LO         = ( xSec("M10to50") ) / SumWeight("LO");
  //   Double_t normFactor_NLO        = ( xSec("M10to50") ) / SumWeight("NLO");
  //   Double_t normFactor_LOWeighted = ( xSec("M10to50") ) / SumWeight("LOWeighted");

  //   h_LO->Scale( normFactor_LO );
  //   h_NLO->Scale( normFactor_NLO );
  //   h_LOWeighted->Scale( normFactor_LOWeighted );

  //   TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
  //   TH1D* h_M50toInf = PlotTool::Get_Hist(analyzerPath+"/ControlPlot/GenLevel/ROOTFile_MakeGenHist_Dimuon_noWeight_DYMuMu_M50toInf.root", "h_diMuM");
  //   Double_t normFactor_M50toInf = ( 2009.41 ) / SumWeight("M50toInf");
  //   h_M50toInf->Scale( normFactor_M50toInf );

  //   h_LO         = PlotTool::DivideEachBin_ByBinWidth(h_LO);
  //   h_NLO        = PlotTool::DivideEachBin_ByBinWidth(h_NLO);
  //   h_LOWeighted = PlotTool::DivideEachBin_ByBinWidth(h_LOWeighted);
  //   h_M50toInf   = PlotTool::DivideEachBin_ByBinWidth(h_M50toInf);

  //   TString canvasName = "c_mass_combinedWithM50Sample";
  //   PlotTool::HistCanvas* canvas = new PlotTool::HistCanvas(canvasName, 0, 1);
  //   canvas->SetTitle("m [GeV]", "d#sigma/dm [pb/GeV]" );

  //   // canvas->Register( h_DYMuMu_merged, "M10-50 + M50-inf", kBlack);
  //   canvas->Register( h_LO,          "M10-50, LO",                   kGreen+2);
  //   canvas->Register( h_NLO,         "M10-50, NLO",                  kRed);
  //   canvas->Register( h_LOWeighted,  "M10-50, LO (weighted to NLO)", kViolet);
  //   canvas->Register( h_M50toInf, "M50-inf, NLO", kBlue);

  //   canvas->SetRangeX( 10, 110 );
  //   // canvas->SetRangeY( 1e5, 1e9 );

  //   canvas->SetLegendPosition(0.60, 0.82, 0.95, 0.95);
  //   canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{generator level (isHardProcess)}}");
  //   // canvas->RegisterLatex(0.16, 0.88, "#font[42]{#scale[0.6]{aMC@NLO samples for both mass range}}");
  //   canvas->Latex_CMSSim();

  //   canvas->Draw();

  //   // -- Zoom-in plot
  //   canvas->SetCanvasName(canvasName+"_zoomIn");
  //   canvas->SetLogXY(0, 0);
  //   canvas->SetRangeX( 45, 55 );
  //   // canvas->SetRangeY( 1e5, 1e6 );
    
  //   canvas->Draw();
  // }

  Double_t SumWeight(TString type)
  {
    Double_t sumWeight = 0;

    if( type == "LO" )  sumWeight = 28985097.0;
    if( type == "NLO" ) sumWeight = 33278866.0; // -- 2016 sample
    // if( type == "NLO" ) sumWeight = 7506956.0; // -- 2015 sample
    if( type == "LOWeighted" ) sumWeight = 28985097.0; // -- same with LO
    if( type == "M50toInf") sumWeight = 44318555.0; 

    return sumWeight;
  }

  Double_t xSec(TString type)
  {
    Double_t xSec = 0;
    Double_t kFactor_M40to50_NNLOtoNLO = 1.0684240278;

    // if( tag == "DYMuMu_M10to50" )                 xSec = 18610.0 / 3.0; // -- NLO
    if( type == "M10to50" )  xSec = (18610.0 / 3.0) * kFactor_M40to50_NNLOtoNLO; // -- NNLO
    if( type == "M50toInf" ) xSec = 2009.41;

    return xSec;
  }

  Double_t GetValue_OneBinHist(TString fileName, TString histName)
  {
    TH1D* h = PlotTool::Get_Hist(fileName, histName);
    return h->GetBinContent(1);
  }

};

void ClosureTest()
{
  TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
  TString fileName_LO  = "./Distribution/LO/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";
  TString fileName_NLO = "./Distribution/NLO/ROOTFile_MakeGenHist_DiMuPtRap_NLO_DYMuMu_M10to50_2016.root"; // -- 2016 sample
  TString fileName_LOWeighted = "./Distribution/LOWeighted/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";
  PlotProducer* producer = new PlotProducer(fileName_LO, fileName_NLO, fileName_LOWeighted);
  producer->Produce();

}