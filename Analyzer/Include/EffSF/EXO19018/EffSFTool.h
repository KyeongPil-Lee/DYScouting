
class EffSFTool
{
public:
  // -- efficiency SF vs. dimuon mass for the events with pT(sub) > 13 GeV
  TH1D* h_effSF_1D_;

  // -- collection efficiency SF vs. dimuon mass & pT(sub) for the events with pT(sub) < 13 GeV.
  Int_t nDRBin_ = 0;
  vector<Double_t> vec_dRBinEdge_ = {0, 1.5, 2.5, 9999};
  vector<TH2D*> vec_hist_effSF_2D_;

  Int_t nBin1D_ = 0;
  Int_t nBin2D_X_ = 0;
  Int_t nBin2D_Y_ = 0;
  EffSFTool()
  {
    cout << "[EffSFTool] use the efficiency SF from EXO-19-018" << endl;
    
    MakeHist_EffSF_1D();
    MakeHist_EffSF_3D();

    nDRBin_ = (Int_t)vec_dRBinEdge_.size() - 1;
    nBin1D_ = h_effSF_1D_->GetNbinsX();
    nBin2D_X_ = vec_hist_effSF_2D_[0]->GetNbinsX();
    nBin2D_Y_ = vec_hist_effSF_2D_[0]->GetNbinsY();
  }

  Double_t GetSF(Double_t pt_sub, Double_t diMuM, Double_t diMuDR)
  {
    Double_t theSF = -1;
    if( pt_sub > 13 )
    {
      Int_t theBin = h_effSF_1D_->GetXaxis()->FindBin(diMuM);

      // -- under/overflow: extrapolation
      if( theBin == 0 )         theBin = 1;
      if( theBin == nBin1D_+1 ) theBin = nBin1D_;

      theSF = h_effSF_1D_->GetBinContent(theBin);
    }
    else
    {
      Int_t dRBinIndex = -1;    
      for(Int_t i_dR=0; i_dR<nDRBin_; i_dR++)
      {
        Double_t minDR = vec_dRBinEdge_[i_dR];
        Double_t maxDR = vec_dRBinEdge_[i_dR+1];
        if( minDR < diMuDR && diMuDR < maxDR )
        {
          dRBinIndex = i_dR;
          break;
        }
      }

      // -- x-axis: dimuon mass
      Int_t theBinX = vec_hist_effSF_2D_[dRBinIndex]->GetXaxis()->FindBin(diMuM);

      // -- under/overflow: extrapolation
      if( theBinX == 0 )          theBinX = 1;
      if (theBinX == nBin2D_X_+1) theBinX = nBin2D_X_;

      // -- y-axis: sub-leading muon pT
      Int_t theBinY = vec_hist_effSF_2D_[dRBinIndex]->GetYaxis()->FindBin(pt_sub);

      // -- under/overflow: extrapolation
      if( theBinY == 0 )          theBinY = 1;
      if (theBinY == nBin2D_Y_+1) theBinY = nBin2D_Y_;

      theSF = vec_hist_effSF_2D_[dRBinIndex]->GetBinContent(theBinX, theBinY);
      if( theSF == 0 )
      {
        printf("[SF == 0!] (pt_sub, diMuM, diMuDR) = (%.2lf, %.2lf, %.4lf) ... SF is forced to be 1\n", pt_sub, diMuM, diMuDR);
        theSF = 1.0;
      }
    }

    // -- for validation
    // printf("(pt_sub, diMuM, diMuDR) = (%.2lf, %.2lf, %.4lf) -> SF = %lf\n", pt_sub, diMuM, diMuDR, theSF);

    return theSF;
  }

  void Save(TFile *f_output)
  {
    f_output->cd();
    h_effSF_1D_->Write();
    for(auto hist : vec_hist_effSF_2D_ )
      hist->Write();
  }

private:
  // -- https://indico.cern.ch/event/837514/contributions/3512003/attachments/1885324/3113203/20190730_DarkPhoton2lApproval_EXO_schhibra.pdf (S12)
  void MakeHist_EffSF_1D()
  {
    const Int_t nBin = 11;
    Double_t arr_binEdge[nBin+1] = {5, 6, 7, 8.5, 10.5, 12.5, 15, 18, 21.5, 26, 31, 50};
    Double_t arr_binContent[nBin] = {0.92, 1.31, 0.94, 0.92, 0.96, 0.96, 0.965, 0.963, 0.98, 1.01, 0.975};

    h_effSF_1D_ = new TH1D("h_effSF_1D", "", nBin, arr_binEdge);
    for(Int_t i=0; i<nBin; i++)
    {
      Int_t i_bin = i+1;
      h_effSF_1D_->SetBinContent(i_bin, arr_binContent[i]);
      h_effSF_1D_->SetBinError(i_bin, 0);
    }
  }

  void MakeHist_EffSF_3D()
  {
    const Int_t nBinX = 9;
    const Int_t nBinY = 4;
    // Double_t arr_binEdgeX[nBinX+1] = {5, 6, 8, 10, 15, 20, 25, 30, 40, 50};
    Double_t arr_binEdgeX[nBinX+1] = {10, 12, 14, 16, 18, 20, 25, 30, 40, 50};
    Double_t arr_binEdgeY[nBinY+1] = {4, 5, 6, 10, 13};

    // -- dR < 1.5
    // -- https://jsalfeld.web.cern.ch/jsalfeld/darkphoton/2dEfficiencies/l1_corrCuts_eff2D_ratio_2018Both_DR0-1p5_Final.png
    Double_t arr_binContent_dR1[nBinY][nBinX] = {
      {0.87, 0.97, 0.91, 1.09, 1.33, 0, 0, 0, 0},
      {0.89, 0.89, 0.95, 1.03, 1.07, 0.91, 0, 0, 0},
      {0.92, 0.93, 0.97, 0.94, 0.92, 0.93, 0.92, 1.09, 0},
      {0.94, 0.94, 0.97, 0.98, 0.98, 0.94, 0.94, 1.06, 0.82}
    };

    // -- 1.5 < dR < 2.5
    // -- https://jsalfeld.web.cern.ch/jsalfeld/darkphoton/2dEfficiencies/l1_corrCuts_eff2D_ratio_2018Both_DR1p5-2p5_Final.png
    Double_t arr_binContent_dR2[nBinY][nBinX] = {
      {0.76, 0.81, 0.97, 1.20, 1.17, 0, 0, 0, 0},
      {0.86, 0.88, 0.92, 0.87, 0.90, 0.86, 0, 0, 0},
      {0.90, 0.94, 0.98, 1.04, 0.97, 0.87, 0.89, 0.98, 0},
      {0, 0, 1.25, 1.04, 0.94, 0.99, 0.95, 0.97, 0.96}
    };

    // -- dR > 2.5
    // -- https://jsalfeld.web.cern.ch/jsalfeld/darkphoton/2dEfficiencies/l1_corrCuts_eff2D_ratio_2018Both_DR2p5-inf_Final.png
    Double_t arr_binContent_dR3[nBinY][nBinX] = {
      {0.70, 0.75, 0.85, 1.37, 1.86, 0, 0, 0, 0},
      {0.78, 0.86, 0.92, 1.12, 1.43, 1.32, 0, 0, 0},
      {0.94, 0.90, 1.06, 1.19, 1.20, 1.06, 1.06, 1.05, 0},
      {0, 0, 0, 0, 1.11, 0.91, 0.98, 0.99, 0.97}
    };

    // Double_t arr_binContentX_ptSub1_dR1 = {0.87, 0.97, 0.91, 1.09, 1.33, 0, 0, 0, 0};
    // Double_t arr_binContentX_ptSub2_dR1 = {0.89, 0.89, 0.95, 1.03, 1.07, 0.91, 0, 0, 0};
    // Double_t arr_binContentX_ptSub3_dR1 = {0.92, 0.93, 0.97, 0.94, 0.92, 0.93, 0.92, 1.09, 0};
    // Double_t arr_binContentX_ptSub4_dR1 = {0.94, 0.94, 0.97, 0.98, 0.98, 0.94, 0.94, 1.06, 0.82};
    TH2D* h_effSF_2D_dR1 = new TH2D("h_effSF_2D_dR1", "", nBinX, arr_binEdgeX, nBinY, arr_binEdgeY);
    TH2D* h_effSF_2D_dR2 = new TH2D("h_effSF_2D_dR2", "", nBinX, arr_binEdgeX, nBinY, arr_binEdgeY);
    TH2D* h_effSF_2D_dR3 = new TH2D("h_effSF_2D_dR3", "", nBinX, arr_binEdgeX, nBinY, arr_binEdgeY);
    for(Int_t i_y=0; i_y<nBinY; i_y++)
    {
      Int_t i_binY = i_y+1;

      for(Int_t i_x=0; i_x<nBinX; i_x++)
      {
        Int_t i_binX = i_x+1;

        h_effSF_2D_dR1->SetBinContent(i_binX, i_binY, arr_binContent_dR1[i_y][i_x] );
        h_effSF_2D_dR2->SetBinContent(i_binX, i_binY, arr_binContent_dR2[i_y][i_x] );
        h_effSF_2D_dR3->SetBinContent(i_binX, i_binY, arr_binContent_dR3[i_y][i_x] );
      }
    }

    vec_hist_effSF_2D_.push_back(h_effSF_2D_dR1);
    vec_hist_effSF_2D_.push_back(h_effSF_2D_dR2);
    vec_hist_effSF_2D_.push_back(h_effSF_2D_dR3);
  }
};

void Test_EffSFTool()
{
  EffSFTool* tool = new EffSFTool();
  TFile *f_output = TFile::Open("ROOTFile_EffSF_EXO19018.root", "RECREATE");
  tool->Save(f_output);
  f_output->Close();
}