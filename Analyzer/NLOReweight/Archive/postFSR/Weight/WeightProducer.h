#include <Include/SimplePlotTools.h>

class NLOWeightProducer
{
public:
  TH2D *h_diMuRapPt_LO_;
  TH2D *h_diMuRapPt_NLO_;

  Double_t sumWeight_LO_;
  Double_t sumWeight_NLO_;

  TH2D* h_diMuRapPt_weight_;

  // Bool_t debug_ = kTRUE;
  Bool_t debug_ = kFALSE;

  NLOWeightProducer(TString fileName_LO, TString fileName_NLO)
  {
    h_diMuRapPt_LO_  = PlotTool::Get_Hist2D(fileName_LO,  "h_diMuRapPt" );
    h_diMuRapPt_NLO_ = PlotTool::Get_Hist2D(fileName_NLO, "h_diMuRapPt" );

    sumWeight_LO_  = GetValue_OneBinHist( fileName_LO, "h_sumWeight" );
    sumWeight_NLO_ = GetValue_OneBinHist( fileName_NLO, "h_sumWeight" );

    if( debug_ )
      printf("[Sum of weight] LO = %.0lf, NLO = %.0lf\n", sumWeight_LO_, sumWeight_NLO_);
  }

  void Set_Debug(Bool_t flag = kTRUE ) { debug_ = flag; }

  void Produce()
  {
    Calc_Weight();
    ProduceComparisonXSecPlot_All();

    ProduceWeightPlot1D_All();
    ProduceWeightPlot2D(h_diMuRapPt_weight_);
  }

  void Save(TString outputFileName)
  {
    TFile *f_output = TFile::Open(outputFileName, "RECREATE");
    f_output->cd();

    h_diMuRapPt_LO_->SetName("h_diMuRapPt_LO");
    h_diMuRapPt_NLO_->SetName("h_diMuRapPt_NLO");

    h_diMuRapPt_LO_->Write();
    h_diMuRapPt_NLO_->Write();
    h_diMuRapPt_weight_->Write();

    f_output->Close();
  }

private:
  void ProduceWeightPlot1D_All()
  {
    vector<Double_t> vec_rapBinEdge = GetVector_BinEdge(h_diMuRapPt_LO_->GetXaxis());

    Int_t nRapBin = (Int_t)vec_rapBinEdge.size() - 1;

    vector<TH1D*> vec_hist;
    for(Int_t i_rap=0; i_rap<nRapBin; i_rap++)
    {
      TString histName = TString::Format("h_diMuRap_Y%.1lfto%.lf", vec_rapBinEdge[i_rap], vec_rapBinEdge[i_rap+1] );
      TH1D* h_temp = MakeHist_Split2DInto1D(h_diMuRapPt_weight_, i_rap, histName);
      vec_hist.push_back( h_temp );
    }

    // -- for colors
    vector<Int_t> vec_color = {
      kBlack, kGray, kRed, kGreen, kBlue,
      kYellow, kMagenta, kCyan, kOrange, kViolet, 
      kSpring, kTeal, kAzure, kPink};

    // -- canvas
    TString canvasName = "c_weight";
    PlotTool::HistCanvas* canvas = new PlotTool::HistCanvas(canvasName, 1, 0);
    canvas->SetTitle("p_{T}(#mu#mu) [GeV]", "weight (NLO/LO)");
    for(Int_t i_rap=0; i_rap<nRapBin; i_rap++)
    {
      TString legendName = TString::Format("%.1lf < Y(#mu#mu) < %.1lf", vec_rapBinEdge[i_rap], vec_rapBinEdge[i_rap+1]);
      Int_t color = vec_color[i_rap];
      canvas->Register(vec_hist[i_rap], legendName, color);
    }

    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{generator level (fromHardProcessFinalState)}}");
    canvas->Latex_CMSSim();

    canvas->SetLegendPosition(0.15, 0.50, 0.50, 0.90);

    canvas->SetRangeX(0.1, 1000);
    canvas->SetRangeY(0, 2.5);

    canvas->Draw();
  }

  void ProduceWeightPlot2D(TH2D* h2D)
  {
    gStyle->SetPalette(1);

    TCanvas *c = new TCanvas("c_weight2D", "", 800, 800);
    c->cd();
    c->SetTopMargin(0.05);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.13);
    c->SetBottomMargin(0.13);
    // c->SetLogx();
    c->SetLogy();

    h2D->SetTitle("");
    h2D->SetStats(kFALSE);
    h2D->GetXaxis()->SetTitle( "Y(#mu#mu)" );
    h2D->GetXaxis()->SetLabelSize(0.04);
    h2D->GetXaxis()->SetTitleOffset(1.1);
    h2D->GetXaxis()->SetTitleSize(0.05);
    h2D->GetXaxis()->SetNoExponent(kFALSE);
    h2D->GetXaxis()->SetMoreLogLabels(kFALSE);

    h2D->GetYaxis()->SetTitle( "p_{T}(#mu#mu) [GeV]" );
    h2D->GetYaxis()->SetTitleOffset(1.3);
    h2D->GetYaxis()->SetTitleSize(0.05);
    h2D->GetYaxis()->SetLabelSize(0.045);
    h2D->GetYaxis()->SetNoExponent(kFALSE);
    h2D->GetYaxis()->SetMoreLogLabels(kFALSE);

    h2D->GetZaxis()->SetRangeUser(0.5, 2.0);

    h2D->Draw("COLZTEXT");

    c->Update(); // -- without this, palette is not made...
    TPaletteAxis *palette = (TPaletteAxis*)h2D->GetListOfFunctions()->FindObject("palette");
    palette->SetX1NDC(0.88);
    palette->SetX2NDC(0.92);
    palette->SetY1NDC(0.13);
    palette->SetY2NDC(0.95);

    TLatex latex;
    latex.DrawLatexNDC(0.13, 0.96, "#font[62]{CMS#font[42]{#it{#scale[0.8]{ Simulation}}}}");
    latex.DrawLatexNDC(0.16, 0.91, "#font[42]{#scale[0.7]{NLO/LO k-factor}}");

    c->SaveAs(".pdf");
  }

  void ProduceComparisonXSecPlot_All()
  {
    vector<Double_t> vec_rapBinEdge = GetVector_BinEdge(h_diMuRapPt_LO_->GetXaxis());

    Int_t nRapBin = (Int_t)vec_rapBinEdge.size() - 1;
    for(Int_t i_rap=0; i_rap<nRapBin; i_rap++)
      ProduceComparisonPlot_GivenRapIndex( i_rap, vec_rapBinEdge[i_rap], vec_rapBinEdge[i_rap+1] );
  }

  void ProduceComparisonPlot_GivenRapIndex( Int_t i_rap, Double_t minRap, Double_t maxRap )
  {
    TString histName = TString::Format("h_diMuRap_Y%.1lfto%.lf", minRap, maxRap );
    TH1D* h_LO  = MakeHist_Split2DInto1D(h_diMuRapPt_LO_,  i_rap, histName+"_LO");
    TH1D* h_NLO = MakeHist_Split2DInto1D(h_diMuRapPt_NLO_, i_rap, histName+"_NLO");

    // -- change to the differential cross section
    h_LO  = PlotTool::DivideEachBin_ByBinWidth(h_LO);
    h_NLO = PlotTool::DivideEachBin_ByBinWidth(h_NLO);

    TString canvasName = TString::Format("c_xSec_LOvsNLO_%dRapBin", i_rap);
    PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio(canvasName, 1, 1);
    canvas->SetTitle("p_{T}(#mu#mu) [GeV]", "d#sigma/dp_{T} [pb/GeV]", "NLO/LO");

    canvas->Register(h_LO,  "LO",  kBlack);
    canvas->Register(h_NLO, "NLO", kBlue);

    canvas->RegisterLatex(0.16, 0.91, "#font[42]{#scale[0.6]{generator level (fromHardProcessFinalState)}}");
    canvas->RegisterLatex(0.16, 0.87, TString::Format("#font[42]{#scale[0.6]{%.1lf < Y(#mu#mu) < %.1lf}}", minRap, maxRap));
    canvas->Latex_CMSSim();

    canvas->SetLegendPosition(0.15, 0.32, 0.50, 0.50);

    canvas->SetRangeX(0.1, 1000);
    canvas->SetRangeY(5e-5, 1e4);
    canvas->SetRangeRatio(0.5, 2.0);

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

  void Calc_Weight()
  {
    // -- init
    h_diMuRapPt_weight_ = (TH2D*)h_diMuRapPt_LO_->Clone("h_diMuRapPt_weight");

    // -- normalization: each bin = x-section
    Double_t xSec = 6203.3333; // -- 18610 / 3.0: DY, 10 < M < 50 GeV
    h_diMuRapPt_LO_->Scale( xSec / sumWeight_LO_ );
    h_diMuRapPt_NLO_->Scale( xSec / sumWeight_NLO_ );

    Int_t nBinX = h_diMuRapPt_LO_->GetNbinsX();
    Int_t nBinY = h_diMuRapPt_LO_->GetNbinsY();

    for(Int_t i_x=0; i_x<nBinX; i_x++)
    {
      Int_t i_binX = i_x + 1;
      for(Int_t i_y=0; i_y<nBinY; i_y++)
      {
        Int_t i_binY = i_y + 1;

        Double_t nEvent_LO  = h_diMuRapPt_LO_->GetBinContent(i_binX, i_binY);
        Double_t nEvent_NLO = h_diMuRapPt_NLO_->GetBinContent(i_binX, i_binY);

        Double_t unc_LO  = h_diMuRapPt_LO_->GetBinError(i_binX, i_binY);
        Double_t unc_NLO = h_diMuRapPt_NLO_->GetBinError(i_binX, i_binY);

        Double_t weight = nEvent_NLO / nEvent_LO;

        Double_t relUnc_LO  = unc_LO / nEvent_LO;
        Double_t relUnc_NLO = unc_NLO / nEvent_NLO;
        Double_t relUnc_weight = sqrt( relUnc_LO*relUnc_LO + relUnc_NLO*relUnc_NLO );
        Double_t unc_weight = weight * relUnc_weight; 

        h_diMuRapPt_weight_->SetBinContent(i_binX, i_binY, weight);
        h_diMuRapPt_weight_->SetBinError(i_binX, i_binY, unc_weight);
      }
    }
  }

  Double_t GetValue_OneBinHist(TString fileName, TString histName)
  {
    TH1D* h = PlotTool::Get_Hist(fileName, histName);
    return h->GetBinContent(1);
  }

};