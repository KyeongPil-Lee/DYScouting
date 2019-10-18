#include <Include/SimplePlotTools.h>

void MakePUPlot()
{
  TH1D* h_data = PlotTool::Get_Hist("PileupHistogram-goldenJSON-13tev-2018-69200ub-99bins.root", "pileup");
  h_data->Scale( 1.0 / h_data->Integral() );
  TH1D* h_MC   = PlotTool::Get_Hist("PileupHistogram_MC.root", "h_PU_MC_2018");

  PlotTool::Print_Histogram(h_data);
  PlotTool::Print_Histogram(h_MC);

  PlotTool::HistCanvaswRatio* canvas = new PlotTool::HistCanvaswRatio("c_PU", 0, 0);
  canvas->SetTitle("Pileup", "Normalized to 1", "data/MC");
  canvas->Register(h_MC, "MC", kRed);
  canvas->Register(h_data, "Data", kBlack);

  canvas->SetRangeRatio(0, 2.5);
  canvas->SetRangeY(0, 0.05);

  canvas->Draw("HISTP");


  TH1D* h_ratio = (TH1D*)h_data->Clone();
  h_ratio->Divide(h_data, h_MC);

  PlotTool::HistCanvas *canvas_PU = new PlotTool::HistCanvas("c_PUWeight", 0, 1);
  canvas_PU->SetTitle("Pileup", "Weight");
  canvas_PU->Register(h_ratio, "Weight", kBlue);
  canvas_PU->SetLegendPosition(0.75, 0.82, 0.95, 0.95);
  canvas_PU->Draw("HISTP");


  TFile *f_output = TFile::Open("ROOTFile_PUReweighting_2018.root", "RECREATE");
  f_output->cd();

  h_data->SetName("h_data");
  h_MC->SetName("h_MC");
  h_ratio->SetName("h_ratio");

  h_data->Write();
  h_MC->Write();
  h_ratio->Write();

  f_output->Close();
}