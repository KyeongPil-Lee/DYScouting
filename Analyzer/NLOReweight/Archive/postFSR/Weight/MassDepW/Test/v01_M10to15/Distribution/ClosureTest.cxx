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
  }

private:
  void ProducePlot_M10to50(TString histName)
  {
    TH1D* h_LO  = PlotTool::Get_Hist(fileName_LO_,  histName);
    TH1D* h_NLO = PlotTool::Get_Hist(fileName_NLO_, histName);
    TH1D* h_LOWeighted = PlotTool::Get_Hist(fileName_LOWeighted_, histName);

    Double_t normFactor_LO         = (lumi * xSec("M10to50") ) / SumWeight("LO");
    Double_t normFactor_NLO        = (lumi * xSec("M10to50") ) / SumWeight("NLO");
    Double_t normFactor_LOWeighted = (lumi * xSec("M10to50") ) / SumWeight("LOWeighted");

    // Double_t normFactor_LO         = (lumi * xSec("M10to50") ) / 28985097.0;
    // Double_t normFactor_NLO        = (lumi * xSec("M10to50") ) / 33278866.0;
    // Double_t normFactor_LOWeighted = (lumi * xSec("M10to50") ) / 28985096.1;

    h_LO->Scale( normFactor_LO );
    h_NLO->Scale( normFactor_NLO );
    h_LOWeighted->Scale( normFactor_LOWeighted );

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
    canvas->Register( h_LO,         "LO",  kRed);
    canvas->Register( h_NLO,        "NLO", kGreen+2);
    canvas->Register( h_LOWeighted, "LO, reweighted to NLO",  kViolet);

    if( histName == "h_diMuM" )
    {
      canvas->SetRangeX( 10, 15 );
      // canvas->SetRangeY( 1e5, 1e9 );
      canvas->SetRangeRatio(0.8, 1.2);
    }

    if( histName == "h_diMuPt" )
    {
      canvas->SetRangeX( 0, 1000 );
      // canvas->SetRangeY( 1e6, 1e9 );
      canvas->SetRangeRatio(0.7, 1.5);
    }

    if( histName == "h_diMuRap" )
    {
      canvas->SetRangeX( -10, 10 );
      // canvas->SetRangeY( 1e6, 1e9 );
      canvas->SetRangeRatio(0.7, 1.5);
    }

    canvas->SetLegendPosition(0.15, 0.75, 0.80, 0.87);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{generator level (fromHardProcessFinalState)}}");

    canvas->Latex_CMSSim();

    canvas->Draw();
  }

  Double_t SumWeight(TString type)
  {
    Double_t sumWeight = 0;
    if( type != "M50toInf" )
    {
      TString fileName = "";
      if( type == "LO" )         fileName = fileName_LO_;
      if( type == "NLO" )        fileName = fileName_NLO_;
      if( type == "LOWeighted" ) fileName = fileName_LOWeighted_;

      TH1D* h_sumWeight = PlotTool::Get_Hist(fileName, "h_sumWeight");
      sumWeight = h_sumWeight->GetBinContent(1);
    }
    else
      sumWeight = 44318555.0;

    printf("[PlotProducer::SumWeight] Type = %s ---> sum(weight) = %.1lf\n", type.Data(), sumWeight);

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
  // TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
  TString fileName_LO         = "./LO/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";
  TString fileName_NLO        = "./NLO/ROOTFile_MakeGenHist_DiMuPtRap_NLO_DYMuMu_M10to50_2016.root"; // -- 2016 sample
  TString fileName_LOWeighted = "./LOWeighted/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";

  PlotProducer* producer = new PlotProducer(fileName_LO, fileName_NLO, fileName_LOWeighted);
  producer->Produce();
}