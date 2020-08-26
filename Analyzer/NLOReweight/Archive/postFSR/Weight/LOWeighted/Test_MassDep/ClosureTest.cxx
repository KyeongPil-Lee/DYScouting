#include <Include/SimplePlotTools.h>

const Double_t lumi = 59740.565202; // -- 59.7  /fb

class PlotProducer
{
public:
  TString massRange_;
  TString fileName_LO_;
  TString fileName_NLO_;
  TString fileName_LOWeighted_;

  TH1D* h_M50toInf_;

  PlotProducer( TString massRange, TString fileName_LO, TString fileName_NLO, TString fileName_LOWeighted )
  {
    massRange_ = massRange;

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
    TH1D* h_LO  = PlotTool::Get_Hist(fileName_LO_,  massRange_+"/"+histName);
    TH1D* h_NLO = PlotTool::Get_Hist(fileName_NLO_, massRange_+"/"+histName);
    TH1D* h_LOWeighted = PlotTool::Get_Hist(fileName_LOWeighted_, massRange_+"/"+histName);

    Double_t normFactor_LO         = (lumi * xSec("M10to50") ) / SumWeight("LO");
    Double_t normFactor_NLO        = (lumi * xSec("M10to50") ) / SumWeight("NLO");
    Double_t normFactor_LOWeighted = (lumi * xSec("M10to50") ) / SumWeight("LOWeighted");

    h_LO->Scale( normFactor_LO );
    h_NLO->Scale( normFactor_NLO );
    h_LOWeighted->Scale( normFactor_LOWeighted );

    TString canvasName = histName;
    canvasName.ReplaceAll("h_", "c_"+massRange_+"_");

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

    canvas->SetLegendPosition(0.15, 0.75, 0.80, 0.87);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{generator level (fromHardProcessFinalState)}}");

    TString massRangeInfo = "";
    if( massRange_ == "below30GeV" ) massRangeInfo = "10 < m(#mu#mu) < 30 GeV";
    if( massRange_ == "above30GeV" ) massRangeInfo = "30 < m(#mu#mu) < 60 GeV";
    canvas->RegisterLatex(0.16, 0.87, "#font[42]{#scale[0.6]{"+massRangeInfo+"}}");

    canvas->Latex_CMSSim();

    canvas->Draw();
  }

  // Double_t SumWeight(TString type)
  // {
  //   Double_t sumWeight = 0;

  //   if( type == "LO" )  sumWeight = 28985097.0;
  //   if( type == "NLO" ) sumWeight = 33278866.0; // -- 2016 sample
  //   // if( type == "NLO" ) sumWeight = 7506956.0; // -- 2015 sample
  //   if( type == "LOWeighted" ) sumWeight = 28985097.0; // -- same with LO
  //   if( type == "M50toInf") sumWeight = 44318555.0; 

  //   return sumWeight;
  // }

  Double_t SumWeight(TString type)
  {
    Double_t sumWeight = 0;
    if( type != "M50toInf" )
    {
      TString fileName = "";
      if( type == "LO" )         fileName = fileName_LO_;
      if( type == "NLO" )        fileName = fileName_NLO_;
      if( type == "LOWeighted" ) fileName = fileName_LOWeighted_;

      TH1D* h_sumWeight = PlotTool::Get_Hist(fileName, massRange_+"/h_sumWeight");
      sumWeight = h_sumWeight->GetBinContent(1);
    }
    else
      sumWeight = 44318555.0;

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
  TString fileName_LO  = "./LO/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";
  TString fileName_NLO = "./NLO/ROOTFile_MakeGenHist_DiMuPtRap_NLO_DYMuMu_M10to50_2016.root"; // -- 2016 sample
  TString fileName_LOWeighted = "./LOWeighted/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";


  PlotProducer* producer_below30GeV = new PlotProducer("below30GeV", fileName_LO, fileName_NLO, fileName_LOWeighted);
  producer_below30GeV->Produce();

  PlotProducer* producer_above30GeV = new PlotProducer("above30GeV", fileName_LO, fileName_NLO, fileName_LOWeighted);
  producer_above30GeV->Produce();

}