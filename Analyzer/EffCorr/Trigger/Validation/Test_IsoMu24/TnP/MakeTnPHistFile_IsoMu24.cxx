#include <Include/SimplePlotTools.h>

// -- modify hist names
class TnPEffTool
{
public:
  TnPEffTool()
  {
    Init();
  }

  TnPEffTool( TString inputFileName ) : TnPEffTool()
  {
    inputFileName_ = inputFileName;
  }

  void SetCutAndCountRange(Double_t min, Double_t max)
  {
    minMass_ = min;
    maxMass_ = max;
  }

  TGraphAsymmErrors* CalcTnPEff_CutAndCount( TString varName )
  {
    TH1D* hEffTemp = PlotTool::Get_Hist( inputFileName_, "hEffTemplate_"+varName, "hEff_"+varName);
    Int_t nBin = hEffTemp->GetNbinsX();

    vector< TH1D* > vec_passHist;
    vector< TH1D* > vec_failHist;

    TString histNameBase = TString::Format("h_%s", varName.Data());

    for(Int_t i=0; i<nBin; i++)
    {
      TString binInfo = TString::Format("%02dbin", i);

      TString histNamePass = histNameBase + "pass_" + binInfo;
      TH1D* hTempPass = PlotTool::Get_Hist( inputFileName_, histNamePass );
      vec_passHist.push_back( hTempPass );

      TString histNameFail = histNameBase + "fail_" + binInfo;
      TH1D* hTempFail = PlotTool::Get_Hist( inputFileName_, histNameFail );
      vec_failHist.push_back( hTempFail );
    }

    TH1D* h_nPass = (TH1D*)hEffTemp->Clone();
    TH1D* h_nTotal = (TH1D*)hEffTemp->Clone();

    Double_t sum_nPass = 0;
    Double_t sum_nTotal = 0;

    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;

      Double_t nEventPass = CountEvent( vec_passHist[i] );
      if( nEventPass < 0 )
      {
        printf("\n[TnPEffTool::CalcTnPEff_CutAndCount] # pass events in %dth bin = %lf < 0 ... force it to be 0\n\n", i_bin, nEventPass);
        nEventPass = 0;
      }
      h_nPass->SetBinContent(i_bin, nEventPass );
      h_nPass->SetBinError( i_bin, sqrt(nEventPass) );

      Double_t nEventFail = CountEvent( vec_failHist[i] );
      Double_t nEventTotal = nEventPass + nEventFail;
      if( nEventTotal < 0 )
      {
        printf("[TnPEffTool::CalcTnPEff_CutAndCount] (# total events in %dth bin = %lf < 0 ... force it to be 0\n", i_bin, nEventTotal);
        nEventTotal = 0;
      }
      h_nTotal->SetBinContent(i_bin, nEventTotal );
      h_nTotal->SetBinError(i_bin, sqrt(nEventTotal) );

      sum_nPass += nEventPass;
      sum_nTotal += nEventTotal;
    }

    TEfficiency *TEff = new TEfficiency(*h_nPass, *h_nTotal);
    TGraphAsymmErrors *gEff = (TGraphAsymmErrors*)TEff->CreateGraph()->Clone();

    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t nEventPass  = h_nPass->GetBinContent(i_bin);
      Double_t nEventTotal = h_nTotal->GetBinContent(i_bin);

      Double_t binCenter, eff;
      gEff->GetPoint(i, binCenter, eff);

      Double_t absUncHigh_eff = gEff->GetErrorYhigh(i);
      Double_t relUncHigh_eff = (absUncHigh_eff / eff) * 100;
      Double_t absUncLow_eff  = gEff->GetErrorYlow(i);
      Double_t relUncLow_eff  = (absUncLow_eff / eff) * 100;

      printf("[%02d bin] (%6.1lf to %6.1lf): eff (%8.1lf / %8.1lf) = %.3lf + %.3lf(%.3lf%%) - %.3lf(%.3lf%%)\n",
              i_bin, hEffTemp->GetBinLowEdge(i_bin), hEffTemp->GetBinLowEdge(i_bin+1), nEventPass, nEventTotal, eff, absUncHigh_eff, relUncHigh_eff, absUncLow_eff, relUncLow_eff);
    }

    Calc_OverallEff( sum_nPass, sum_nTotal );

    cout << endl;

    return gEff;
  }

private:
  Double_t minMass_;
  Double_t maxMass_;
  TString inputFileName_;

  void Init()
  {
    inputFileName_ = "";
    minMass_ = 81;
    maxMass_ = 101;
    cout << "==========================================================" << endl;
    cout << "Default cut & count mass range: " << minMass_ << " < M < " << maxMass_ << " GeV" << endl;
    cout << "==========================================================" << endl;
  }

  Double_t CountEvent( TH1D* h )
  {
    Double_t nTotEvent = 0;

    Int_t nBin = h->GetNbinsX();
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      Double_t BinCenter = h->GetBinCenter(i_bin);
      if( minMass_ < BinCenter && BinCenter < maxMass_ )
      {
        Double_t nEvent = h->GetBinContent(i_bin);
        nTotEvent += nEvent;
      }
    }
    return nTotEvent;
  }

  void Calc_OverallEff( Double_t nPass, Double_t nTotal )
  {
    TH1D* h_singleBin = new TH1D("h_singleBin", "", 1, 0, 1 );
    TH1D* h_nPass = (TH1D*)h_singleBin->Clone();
    h_nPass->SetBinContent(1, nPass);
    h_nPass->SetBinError(1, sqrt(nPass));

    TH1D* h_nTotal = (TH1D*)h_singleBin->Clone();
    h_nTotal->SetBinContent(1, nTotal);
    h_nTotal->SetBinError(1, sqrt(nTotal));

    TEfficiency *TEff = new TEfficiency(*h_nPass, *h_nTotal);
    TGraphAsymmErrors *gEff = (TGraphAsymmErrors*)TEff->CreateGraph()->Clone();

    Double_t binCenter, eff;
    gEff->GetPoint(0, binCenter, eff);

    Double_t absUncHigh_eff = gEff->GetErrorYhigh(0);
    Double_t relUncHigh_eff = (absUncHigh_eff / eff) * 100;
    Double_t absUncLow_eff = gEff->GetErrorYlow(0);
    Double_t relUncLow_eff = (absUncLow_eff / eff) * 100;

    cout << "[Overall efficiency]" << endl;
    printf("eff (%.1lf / %.1lf) = %.3lf + %.3lf(%.3lf%%) - %.3lf(%.3lf%%)\n",
            nPass, nTotal, eff, absUncHigh_eff, relUncHigh_eff, absUncLow_eff, relUncLow_eff );
  }
};

void MakeTnPHistFile_IsoMu24()
{
  TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
  TString fileName = analyzerPath+"/EffCorr/Trigger/Validation/TnP/Validation/ROOTFile_MakeHist_IsoMu24_Offline_DYMuMu_M50toInf.root";

  TnPEffTool *tool = new TnPEffTool(fileName);
  TH1D* h_eff_pt = PlotTool::Convert_GraphToHist( tool->CalcTnPEff_CutAndCount( "highPt" ) );
  h_eff_pt->SetName("h_eff_pt");
  PlotTool::Print_Histogram(h_eff_pt);

  TFile* f_output = TFile::Open("ROOTFile_TnPEff_IsoMu24.root", "RECREATE");
  f_output->cd();
  h_eff_pt->Write();

  f_output->Close();
}