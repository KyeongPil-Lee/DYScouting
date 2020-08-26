#include <Include/SimplePlotTools.h>

const Double_t lumi = 59740.565202; // -- 59.7  /fb

class PlotProducer
{
public:
  TString fileName_LO_;
  TString fileName_NLO_;
  TString fileName_LOWeighted_;

  TH1D* h_M50toInf_;

  PlotProducer( TString fileName_LO, TString fileName_NLO, TString fileName_LOWeighted )
  {
    fileName_LO_ = fileName_LO;
    fileName_NLO_ = fileName_NLO;
    fileName_LOWeighted_ = fileName_LOWeighted;
  }

  void Set_M50toInf(TString fileName, TString histName)
  {
    h_M50toInf_ = PlotTool::Get_Hist(fileName, histName);
    Double_t normFactor = (lumi * xSec("M50toInf") ) / SumWeight("M50toInf");
    h_M50toInf_->Scale( normFactor );
  }

  void Produce()
  {
    ProducePlot_M10to50("h_diMuM");
    ProducePlot_M10to50("h_diMuPt");
    ProducePlot_M10to50("h_diMuRap");
    // ProducePlot_M10to50("h_diMuM", kTRUE);
    // ProducePlot_M10to50("h_diMuPt", kTRUE);
    // ProducePlot_M10to50("h_diMuRap", kTRUE);
  }

private:
  void ProducePlot_M10to50(TString histName, Bool_t doNormTo1 = kFALSE)
  {
    TH1D* h_LO  = PlotTool::Get_Hist(fileName_LO_,  histName);
    TH1D* h_NLO = PlotTool::Get_Hist(fileName_NLO_, histName);
    TH1D* h_LOWeighted = PlotTool::Get_Hist(fileName_LOWeighted_, histName);

    if( !doNormTo1 )
    {
      Double_t normFactor_LO         = (lumi * xSec("M10to50") ) / SumWeight("LO");
      Double_t normFactor_NLO        = (lumi * xSec("M10to50") ) / SumWeight("NLO");
      Double_t normFactor_LOWeighted = (lumi * xSec("M10to50") ) / SumWeight("LOWeighted");

      h_LO->Scale( normFactor_LO );
      h_NLO->Scale( normFactor_NLO );
      h_LOWeighted->Scale( normFactor_LOWeighted );
    }
    else
    {
      h_LO->Scale( 1.0 / h_LO->Integral() );
      h_NLO->Scale( 1.0 / h_NLO->Integral() );
      h_LOWeighted->Scale( 1.0 / h_LOWeighted->Integral() );
    }

    TString canvasName = histName;
    canvasName.ReplaceAll("h_", "c_");

    Bool_t isLogX = kFALSE;
    if( histName == "h_diMuPt" ) isLogX = kTRUE;
    PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio(canvasName, isLogX, 1);

    TString titleX = "";
    if( histName == "h_diMuM" )   titleX = "m(#mu#mu) [GeV]";
    if( histName == "h_diMuPt" )  titleX = "p_{T}(#mu#mu) [GeV]";
    if( histName == "h_diMuRap" ) titleX = "Y(#mu#mu)";
    canvas->SetTitle(titleX, "# events", "ratio to Red");

    // canvas->Register( h_DYMuMu_merged, "M10-50 + M50-inf", kBlack);
    canvas->Register( h_LO,         "10 < M < 50 GeV (LO)",  kRed);
    canvas->Register( h_NLO,        "10 < M < 50 GeV (NLO)", kGreen+2);
    canvas->Register( h_LOWeighted, "10 < M < 50 GeV (LO, reweighted to NLO)",  kViolet);

    if( histName == "h_diMuM" )
    {
      canvas->SetRangeX( 10, 50 );
      canvas->SetRangeY( 1e5, 1e9 );
      canvas->SetRangeRatio(0.8, 1.2);
    }

    if( histName == "h_diMuPt" )
    {
      canvas->SetRangeX( 0, 1000 );
      canvas->SetRangeY( 1e6, 1e9 );
      canvas->SetRangeRatio(0.7, 1.5);
    }

    if( histName == "h_diMuRap" )
    {
      canvas->SetRangeX( -10, 10 );
      canvas->SetRangeY( 1e6, 1e9 );
      canvas->SetRangeRatio(0.7, 1.5);
    }

    if( doNormTo1 )
      canvas->SetRangeY( 1e-5, 10 );

    canvas->SetLegendPosition(0.15, 0.75, 0.80, 0.90);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{generator level (fromHardProcessFinalState)}}");
    canvas->Latex_CMSSim();

    canvas->Draw();
  }

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

};

void ClosureTest()
{
  TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
  TString fileName_LO  = "../../../LO/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";
  TString fileName_NLO = "../../../NLO/ROOTFile_MakeGenHist_DiMuPtRap_NLO_DYMuMu_M10to50_2016.root"; // -- 2016 sample
  TString fileName_LOWeighted = "./ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";
  PlotProducer* producer = new PlotProducer(fileName_LO, fileName_NLO, fileName_LOWeighted);
  producer->Produce();

}