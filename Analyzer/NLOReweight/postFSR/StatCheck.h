#include <Include/SimplePlotTools.h>

class PlotProducer
{
public:
  TH2D* h_diMuRapPt_;
  Bool_t debug_ = kFALSE;

  vector<Int_t> vec_color = {
    kBlack, kGray, kRed, kGreen, kBlue,
    kYellow, kMagenta, kCyan, kOrange, kSpring, 
    kTeal, kAzure, kViolet, kPink};

  PlotProducer(TString fileName)
  {
    h_diMuRapPt_ = PlotTool::Get_Hist2D(fileName, "h_diMuRapPt" );
  }

  void SetDebug(Bool_t flag = kTRUE ) { debug_ = flag; }

  void Produce()
  {
    DrawPlot_SplitInto1D();
  }
private:
  void DrawPlot_SplitInto1D()
  {
    vector<Double_t> vec_rapBinEdge = GetVector_BinEdge(h_diMuRapPt_->GetXaxis());

    Int_t nHist = (Int_t)vec_rapBinEdge.size() - 1;

    vector<TH1D*> vec_hist;
    for(Int_t i_hist=0; i_hist<nHist; i_hist++)
    {
      TString histName = TString::Format("h_diMuRap_Y%.1lfto%.lf", vec_rapBinEdge[i_hist], vec_rapBinEdge[i_hist+1] );
      TH1D* h_temp = MakeHist_Split2DInto1D(h_diMuRapPt_, i_hist, histName);
      vec_hist.push_back( h_temp );
    }

    TString canvasName = "c_statCheck";
    PlotTool::HistCanvas* canvas = new PlotTool::HistCanvas(canvasName, 1, 1);
    canvas->SetTitle("p_{T}(#mu#mu) [GeV]", "# events");
    for(Int_t i_hist=0; i_hist<nHist; i_hist++)
    {
      TString legendName = TString::Format("%.1lf < Y(#mu#mu) < %.1lf", vec_rapBinEdge[i_hist], vec_rapBinEdge[i_hist+1]);
      Int_t color = vec_color[i_hist];
      canvas->Register(vec_hist[i_hist], legendName, color);
    }

    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{generator level (fromHardProcessFinalState)}}");
    canvas->Latex_CMSSim();

    canvas->SetLegendPosition(0.15, 0.15, 0.50, 0.50);

    canvas->SetRangeX(1, 1000);
    canvas->SetRangeY(1, 1e8);

    canvas->Draw();
  }

  vector<Double_t> GetVector_BinEdge(TAxis *axis)
  {
    vector<Double_t> vec_binEdge;

    Int_t nBin = axis->GetNbins();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t binEdge = axis->GetBinLowEdge(i_bin);

      vec_binEdge.push_back( binEdge );
    }

    vec_binEdge.push_back( axis->GetBinLowEdge(nBin+1) ); // -- last bin

    if( debug_ )
    {
      for( auto binEdge : vec_binEdge )
        printf("%.1lf\t", binEdge);

      printf("\n");
    }

    return vec_binEdge;
  }

  TH1D* MakeHist_Split2DInto1D(TH2D* h2D, Int_t i_hist, TString histName)
  {
    vector<Double_t> vec_binEdge = GetVector_BinEdge( h2D->GetYaxis() );

    TH1D* h_temp = MakeTH1D_FromVector(histName, vec_binEdge);

    Int_t i_binX = i_hist + 1; // -- x-axis: fixed
    Int_t nBinY = h2D->GetNbinsY();
    for(Int_t i=0; i<nBinY; i++)
    {
      Int_t i_binY = i+1;
      Double_t value = h2D->GetBinContent(i_binX, i_binY);
      Double_t error = h2D->GetBinError(i_binX, i_binY);

      h_temp->SetBinContent(i_binY, value);
      h_temp->SetBinError(i_binY, error);
    }

    return h_temp;
  }

  TH1D* MakeTH1D_FromVector(TString histName, vector<Double_t> vec_binEdge)
  {
    const Int_t nBin = (Int_t)vec_binEdge.size() - 1;
    Double_t *arr_binEdge = new Double_t[nBin+1];

    for(Int_t i=0; i<nBin+1; i++)
      arr_binEdge[i] = vec_binEdge[i];

    TH1D* h_temp = new TH1D(histName, "", nBin, arr_binEdge);

    delete[] arr_binEdge;

    return h_temp;
  }
};

class ComparisonPlotProducer
{
public:
  TH2D *h2D_LO_;
  TH2D *h2D_NLO_;
  ComparisonPlotProducer(TString fileName_LO, TString fileName_NLO)
  {
    h2D_LO_  = PlotTool::Get_Hist2D(fileName_LO,  "h_diMuRapPt" );
    h2D_NLO_ = PlotTool::Get_Hist2D(fileName_NLO, "h_diMuRapPt" );
  }

  void Produce()
  {
    ProduceComparisonPlot_All();
  }
private:
  Bool_t debug_ = kFALSE;

  void ProduceComparisonPlot_All()
  {
    vector<Double_t> vec_rapBinEdge = GetVector_BinEdge(h2D_LO_->GetXaxis());

    Int_t nRapBin = (Int_t)vec_rapBinEdge.size() - 1;
    for(Int_t i_rap=0; i_rap<nRapBin; i_rap++)
    {
      ProduceComparisonPlot_GivenRapIndex( i_rap, vec_rapBinEdge[i_rap], vec_rapBinEdge[i_rap+1] );
    }
  }

  void ProduceComparisonPlot_GivenRapIndex( Int_t i_rap, Double_t minRap, Double_t maxRap )
  {
    TString histName = TString::Format("h_diMuRap_Y%.1lfto%.lf", minRap, maxRap );
    TH1D* h_LO  = MakeHist_Split2DInto1D(h2D_LO_,  i_rap, histName+"_LO");
    TH1D* h_NLO = MakeHist_Split2DInto1D(h2D_NLO_, i_rap, histName+"_NLO");

    TString canvasName = TString::Format("c_statCheck_LOvsNLO_%dRapBin", i_rap);
    PlotTool::HistCanvas* canvas = new PlotTool::HistCanvas(canvasName, 1, 1);
    canvas->SetTitle("p_{T}(#mu#mu) [GeV]", "# events");

    canvas->Register(h_LO,  "LO",  kBlack);
    canvas->Register(h_NLO, "NLO", kBlue);

    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{generator level (fromHardProcessFinalState)}}");
    canvas->RegisterLatex(0.16, 0.87, TString::Format("#font[42]{#scale[0.6]{%.1lf < Y(#mu#mu) < %.1lf}}", minRap, maxRap));
    canvas->Latex_CMSSim();

    canvas->SetLegendPosition(0.15, 0.15, 0.50, 0.32);

    canvas->SetRangeX(1, 1000);
    canvas->SetRangeY(1, 1e8);

    canvas->Draw();
  }


  vector<Double_t> GetVector_BinEdge(TAxis *axis)
  {
    vector<Double_t> vec_binEdge;

    Int_t nBin = axis->GetNbins();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t binEdge = axis->GetBinLowEdge(i_bin);

      vec_binEdge.push_back( binEdge );
    }

    vec_binEdge.push_back( axis->GetBinLowEdge(nBin+1) ); // -- last bin

    if( debug_ )
    {
      for( auto binEdge : vec_binEdge )
        printf("%.1lf\t", binEdge);

      printf("\n");
    }

    return vec_binEdge;
  }

  TH1D* MakeHist_Split2DInto1D(TH2D* h2D, Int_t i_hist, TString histName)
  {
    vector<Double_t> vec_binEdge = GetVector_BinEdge( h2D->GetYaxis() );

    TH1D* h_temp = MakeTH1D_FromVector(histName, vec_binEdge);

    Int_t i_binX = i_hist + 1; // -- x-axis: fixed
    Int_t nBinY = h2D->GetNbinsY();
    for(Int_t i=0; i<nBinY; i++)
    {
      Int_t i_binY = i+1;
      Double_t value = h2D->GetBinContent(i_binX, i_binY);
      Double_t error = h2D->GetBinError(i_binX, i_binY);

      h_temp->SetBinContent(i_binY, value);
      h_temp->SetBinError(i_binY, error);
    }

    return h_temp;
  }

  TH1D* MakeTH1D_FromVector(TString histName, vector<Double_t> vec_binEdge)
  {
    const Int_t nBin = (Int_t)vec_binEdge.size() - 1;
    Double_t *arr_binEdge = new Double_t[nBin+1];

    for(Int_t i=0; i<nBin+1; i++)
      arr_binEdge[i] = vec_binEdge[i];

    TH1D* h_temp = new TH1D(histName, "", nBin, arr_binEdge);

    delete[] arr_binEdge;

    return h_temp;
  }

};