#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <Include/DYTool.h>

// -- common histogram for generator level distributions
class HistContainer
{
public:
  HistContainer()
  {
    InitHist();
  }

  // -- when GenParticle is given
  void FillHistogram( DYTool::GenParticle& genLepton1, DYTool::GenParticle& genLepton2, Double_t weight )
  {
    FillHistogram( genLepton1.vecP, genLepton2.vecP, weight );
  }

  // -- when momentum vector is given
  void FillHistogram( TLorentzVector& vecP_mu1, TLorentzVector& vecP_mu2, Double_t weight )
  {
    h_pt_->Fill( vecP_mu1.Pt(), weight );
    h_pt_->Fill( vecP_mu2.Pt(), weight );

    h_eta_->Fill( vecP_mu1.Eta(), weight );
    h_eta_->Fill( vecP_mu2.Eta(), weight );

    TLorentzVector vecP_diMu = (vecP_mu1 + vecP_mu2);

    h_diMuPt_->Fill( vecP_diMu.Pt(), weight );
    h_diMuRap_->Fill( vecP_diMu.Rapidity(), weight );
    h_diMuM_->Fill( vecP_diMu.M(), weight );

    h_diMuRapPt_->Fill( vecP_diMu.Rapidity(), vecP_diMu.Pt(), weight );

    h_sumWeight_->Fill( 0.5, weight ); // -- fill the first bin with the weight: eventually the first bin will save sum(weight)
  }

  void SaveHistogram( TFile *f_output )
  {
    h_pt_->Write();
    h_eta_->Write();
    h_diMuPt_->Write();
    h_diMuRap_->Write();
    h_diMuM_->Write();
    h_diMuRapPt_->Write();

    h_sumWeight_->Write();
  }

private:
  TH1D* h_pt_;
  TH1D* h_eta_;
  TH1D* h_diMuPt_;
  TH1D* h_diMuRap_;
  TH1D* h_diMuM_;
  TH2D* h_diMuRapPt_;

  TH1D* h_sumWeight_;

  void InitHist()
  {
    const Int_t nPtBin = 17;
    Double_t arr_ptBinEdge[nPtBin+1] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30, 40, 50, 1000};

    const Int_t nRapBin = 10;
    Double_t arr_rapBinEdge[nRapBin+1] = {-10, -4, -2, -1, -0.5, 0, 0.5, 1, 2, 4, 10};

    h_pt_  = new TH1D("h_pt", "", nPtBin, arr_ptBinEdge);
    h_eta_ = new TH1D("h_eta", "", nRapBin, arr_rapBinEdge);

    h_diMuPt_  = new TH1D("h_diMuPt", "", nPtBin, arr_ptBinEdge);
    h_diMuRap_ = new TH1D("h_diMuRap", "", nRapBin, arr_rapBinEdge);
    h_diMuM_   = new TH1D("h_diMuM", "",     10000, 0, 10000);

    h_diMuRapPt_ = new TH2D("h_diMuRapPt", "", nRapBin, arr_rapBinEdge, nPtBin, arr_ptBinEdge);

    h_sumWeight_ = new TH1D("h_sumWeight", "", 1, 0, 1);
  }
};


// -- hist containers per mass bin
class LargeHistContainer
{
public:
  vector<Double_t> vec_massBinEdge_;
  Int_t nMassBin_ = 0;

  vector<HistContainer*> vec_histContainer_;

  LargeHistContainer()
  {
    vector<Double_t> vec_temp = {0, 15, 20, 30, 40, 50};
    vec_massBinEdge_ = vec_temp;
    InitHist();
  }

  LargeHistContainer(vector<Double_t> vec_massBinEdge)
  {
    vec_massBinEdge_ = vec_massBinEdge;
    InitHist();
  }

  // -- when GenParticle is given
  void FillHistogram( DYTool::GenParticle& genLepton1, DYTool::GenParticle& genLepton2, Double_t weight )
  {
    FillHistogram( genLepton1.vecP, genLepton2.vecP, weight );
  }

  // -- when momentum vector is given
  void FillHistogram( TLorentzVector& vecP_mu1, TLorentzVector& vecP_mu2, Double_t weight )
  {
    TLorentzVector vecP_diMu = (vecP_mu1 + vecP_mu2);
    Double_t diMuM = vecP_diMu.M();

    Bool_t isFound = kFALSE;

    for(Int_t i=0; i<nMassBin_; i++)
    {
      Double_t minM = vec_massBinEdge_[i];
      Double_t maxM = vec_massBinEdge_[i+1];

      if( minM < diMuM && diMuM < maxM ) // -- if the mass bin is found
      {
        isFound = kTRUE;
        vec_histContainer_[i]->FillHistogram( vecP_mu1, vecP_mu2, weight );
        break;
      }
    }

    if( !isFound ) cout << "diMuM = " << diMuM << " event is out of the mass range ... not saved" << endl;
  }

  void SaveHistogram(TFile *f_output)
  {
    f_output->cd();
    for(Int_t i=0; i<nMassBin_; i++)
    {
      TString dirName = TString::Format("M%.0lfto%.0lf", vec_massBinEdge_[i], vec_massBinEdge_[i+1]);
      f_output->mkdir(dirName);
      f_output->cd(dirName);
      vec_histContainer_[i]->SaveHistogram(f_output);

      f_output->cd(); // -- back to root dir
    }
  }

private:
  void InitHist()
  {
    nMassBin_ = (Int_t)vec_massBinEdge_.size() - 1;
    for(Int_t i=0; i<nMassBin_; i++)
      vec_histContainer_.push_back( new HistContainer() );
  }


};