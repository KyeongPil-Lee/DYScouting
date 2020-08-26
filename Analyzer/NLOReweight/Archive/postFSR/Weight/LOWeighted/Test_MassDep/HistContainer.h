#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <Include/DYTool.h>

// -- common histogram for generator level distribution
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
    h_diMuM_   = new TH1D("h_diMuM", "", 10000, 0, 10000);

    h_diMuRapPt_ = new TH2D("h_diMuRapPt", "", nRapBin, arr_rapBinEdge, nPtBin, arr_ptBinEdge);

    h_sumWeight_ = new TH1D("h_sumWeight", "", 1, 0, 1);
  }
};