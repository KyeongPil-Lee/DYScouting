#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TString fileName_LO_;
  TString fileName_NLO_;

  PlotProducer(TString fileName_LO, TString fileName_NLO)
  {
    fileName_LO_ = fileName_LO;
    fileName_NLO_ = fileName_NLO;
  }

  void Produce()
  {
    DrawPlot("diMuM");
    DrawPlot("diMuPt");
    DrawPlot("diMuRap");
  }

private:
  void DrawPlot(TString var)
  {
    TString histName = "h_"+var;
    TH1D* h_LO  = PlotTool::Get_Hist(fileName_LO_, histName);
    TH1D* h_NLO = PlotTool::Get_Hist(fileName_NLO_, histName);

    h_LO->Scale(  1.0 / h_LO->Integral() );
    h_NLO->Scale( 1.0 / h_NLO->Integral() );

    RemoveNegativeBin(h_LO, "LO");
    RemoveNegativeBin(h_NLO, "NLO");

    TString canvasName = TString::Format("c_comp_%s", var.Data());
    PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio(canvasName, 0, 1);

    TString titleX = "";
    if( var == "diMuM" )   titleX = "m(#mu#mu) [GeV]";
    if( var == "diMuPt" )  titleX = "p_{T}(#mu#mu) [GeV]";
    if( var == "diMuRap" ) titleX = "Y(#mu#mu)";

    canvas->SetTitle(titleX, "Normalized to 1", "NLO/LO" );

    canvas->Register( h_LO,  "LO sample (Madgraph MLM)",  kBlack);
    canvas->Register( h_NLO, "NLO sample (aMC@NLO FxFx)", kBlue);

    // canvas->SetAutoRangeY();

    if( var == "diMuM" )   canvas->SetRangeX(0, 60);
    if( var == "diMuPt" )  canvas->SetRangeX(0, 20);
    if( var == "diMuRap" ) canvas->SetRangeX(-10, 10);

    if( var == "diMuRap" ) canvas->SetRangeY(1e-8, 1e-1);

    if( var == "diMuM" )  canvas->SetRangeRatio(0.8, 1.2);
    if( var == "diMuRap" ) canvas->SetRangeRatio(0.5, 1.5);

    canvas->SetLegendPosition(0.40, 0.80, 0.95, 0.91);
    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{generator level (isHardProcess)}}");
    canvas->Latex_CMSSim();

    canvas->Draw();
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

};

void ComparisonPlot()
{
  TString fileName_LO  = "./LO/ROOTFile_MakeGenHist_DiMuPtRap_DYMuMu_M10to50.root";
  TString fileName_NLO = "./NLO/ROOTFile_MakeGenHist_DiMuPtRap_NLO_DYMuMu_M10to50_2016.root";
  PlotProducer* producer = new PlotProducer(fileName_LO, fileName_NLO);
  producer->Produce();
}
