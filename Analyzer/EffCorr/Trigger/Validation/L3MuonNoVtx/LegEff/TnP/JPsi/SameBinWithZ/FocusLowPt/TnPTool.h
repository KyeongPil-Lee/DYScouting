#include <TEfficiency.h>

#include <Include/DYTool.h>
#include <Include/SimplePlotTools.h>

class TnPPairBase
{
public:
  DYTool::OffMuon tag_;
  DYTool::OffMuon probe_;
  DYTool::DYTree* ntuple_;

  Bool_t isValid_;
  Bool_t isPassingProbe_;

  Double_t mass_;
  Int_t nVtx_;

  TnPPairBase( DYTool::OffMuon tagCand, DYTool::OffMuon probeCand, DYTool::DYTree* ntuple ):
  tag_(tagCand),
  probe_(probeCand),
  ntuple_(ntuple)
  {
    isValid_ = kFALSE;
    isPassingProbe_ = kFALSE;
    mass_ = -999;
    nVtx_ = -999;
  }


  Bool_t IsValid()
  {
    if( PassTagCondition() && PassProbeCondition() )
    {
      mass_ = (tag_.vecP + probe_.vecP).M(); // -- used in PassPairCondition()

      if( PassPairCondition() )
      {
        isValid_ = kTRUE;
        nVtx_ = ntuple_->nOffVertex;

        if( PassPassingProbeCondition() ) isPassingProbe_ = kTRUE;
      }
    }

    return isValid_;
  }

  Double_t Mass()
  {
    if( !isValid_ )
    {
      cout << "Call mass value of invalid Tag & Probe pair! ... please check the code" << endl;
      return -999;
    }

    return mass_;
  }

  Double_t ProbePt() { return probe_.pt; }
  Double_t ProbeEta() { return probe_.eta; }
  Double_t ProbePhi() { return probe_.phi; }
  Int_t nVtx() { return nVtx_; }

  Bool_t IsPassingProbe() { return isPassingProbe_; }

  // -- user-defined tag condition
  virtual Bool_t PassTagCondition()
  {
    Bool_t flag = kFALSE;

    cout << "default PassTagCondition" << endl;

    return flag;
  }

  // -- user-defined probe condition
  virtual Bool_t PassProbeCondition()
  {
    Bool_t flag = kFALSE;

    cout << "default PassProbeCondition" << endl;

    return flag;
  }

  // -- user-defined probe condition
  virtual Bool_t PassPassingProbeCondition()
  {
    Bool_t flag = kFALSE;

    cout << "default PassPassingProbeCondition" << endl;

    return flag;
  }

  // -- could be re-defined
  virtual Bool_t PassPairCondition()
  {
    Bool_t flag = kFALSE;

    Double_t deltaR = tag_.vecP.DeltaR(probe_.vecP);

    if( mass_ > 49.9 && mass_ < 130.1 && deltaR > 0.3 )
      flag = kTRUE;

    return flag;
  }

};

class TnPHist
{
public:

  TnPHist()
  {
    cout << "Default constructor" << endl;
    cout << "Use TnPHist( TString binType, vector<Double_t> vec_BinEdge )" << endl;
  }

  TnPHist( TString binType, Double_t minPtCut, vector<Double_t> vec_BinEdge )
  {
    binType_ = binType;
    minPtCut_ = minPtCut;

    nBin_ = (Int_t)vec_BinEdge.size()-1; // -- # bins = # bin edges - 1

    minM_ = 50;
    maxM_ = 130;

    arr_binEdge_ = new Double_t[nBin_+1]; // -- dynamic allocation   
    for(Int_t i=0; i<nBin_+1; i++)
      arr_binEdge_[i] = vec_BinEdge[i];

    GenerateHists();
  }

  ~TnPHist()
  {
    delete arr_binEdge_;

    for( auto passHist : vec_passHist_ ) delete passHist;
    for( auto failHist : vec_failHist_ ) delete failHist;

    delete hEffTemplate_;
  }

  void Fill( TnPPairBase* pair, Double_t value, Double_t weight = 1.0 )
  {
    if( pair->ProbePt() < minPtCut_ ) return;

    Int_t i_hist = FindBinNumber(value);
    if( i_hist < 0 ) return; // -- out of bin range

    Double_t mass = pair->Mass();
    if( pair->IsPassingProbe() )
      vec_passHist_[i_hist]->Fill( mass, weight );
    else
      vec_failHist_[i_hist]->Fill( mass, weight );
  }

  void Save( TFile* f_output )
  {
    for(const auto& passHist : vec_passHist_ ) passHist->Write();
    for(const auto& failHist : vec_failHist_ ) failHist->Write();

    hEffTemplate_->Write();
  }

  TH1D* hEffTemplate() { return hEffTemplate_; }

private:
  TString binType_;
  Double_t minPtCut_;

  Int_t nBin_;
  Double_t* arr_binEdge_;

  // -- mass range for tag&probe mass distributino
  Double_t minM_;
  Double_t maxM_;

  vector<TH1D*> vec_passHist_;
  vector<TH1D*> vec_failHist_;

  // -- template histogram for the efficiency plot
  TH1D* hEffTemplate_;

  void GenerateHists()
  {
    Int_t nMassBin = (Int_t)(maxM_ - minM_); // -- bin size = 1 GeV -- //

    TString histNameBase = TString::Format("h_%s", binType_.Data());

    for(Int_t i=0; i<nBin_; i++)
    {
      TString binInfo = TString::Format("%02dbin", i);

      TString histNamePass = histNameBase + "_pass_" + binInfo;
      TH1D* hTempPass = new TH1D(histNamePass, "", nMassBin, minM_, maxM_ );
      vec_passHist_.push_back( hTempPass );

      TString histNameFail = histNameBase + "_fail_" + binInfo;
      TH1D* hTempFail = new TH1D(histNameFail, "", nMassBin, minM_, maxM_ );
      vec_failHist_.push_back( hTempFail );
    }

    hEffTemplate_ = new TH1D("hEffTemplate_"+binType_, "", nBin_, arr_binEdge_);
  }

  Int_t FindBinNumber(Double_t value)
  {
    Int_t i_return = -1;

    for(Int_t i=0; i<nBin_; i++)
    {
      if( arr_binEdge_[i] < value && value < arr_binEdge_[i+1] )
      {       
        i_return = i;
        break;
      }
    }

    return i_return;
  }

};

class TnPHist2D
{
public:
  TString binType_;
  Double_t minPtCut_;

  Int_t nBinX_;
  vector<Double_t> vec_binEdgeX_;
  vector<Double_t> vec_binEdgeY_;

  TnPHist2D( TString binType, Double_t minPtCut, vector<Double_t> vec_binEdgeX, vector<Double_t> vec_binEdgeY )
  {
    binType_ = binType;
    minPtCut_ = minPtCut;

    vec_binEdgeX_ = vec_binEdgeX;
    vec_binEdgeY_ = vec_binEdgeY;

    nBinX_ = (Int_t)vec_binEdgeX.size()-1; // -- # bins = # bin edges - 1

    Init();
  }

  ~TnPHist2D()
  {
    for(auto tnpHist : vec_TnPHist_ )
      delete tnpHist;
  }

  void Fill( TnPPairBase* pair, Double_t valueX, Double_t valueY, Double_t weight = 1.0 )
  {
    Int_t i_binX = FindBinNumberX(valueX);
    if( i_binX < 0 ) return; // -- not in x range

    vec_TnPHist_[i_binX]->Fill( pair, valueY, weight );
  }

  void Save( TFile* f_output )
  {
    for(auto tnpHist : vec_TnPHist_ )
      tnpHist->Save( f_output );
  }

private:
  vector<TnPHist*> vec_TnPHist_; // -- TnP mass histgoram for Y variable in each X bin

  void Init()
  {
    for(Int_t i_x=0; i_x<nBinX_; i_x++)
    {
      TString binType_binX = TString::Format("%s_%02dbinX", binType_.Data(), i_x);
      TnPHist* tnpHist_temp = new TnPHist(binType_binX, minPtCut_, vec_binEdgeY_ );
      vec_TnPHist_.push_back( tnpHist_temp );
    }
  }

  Int_t FindBinNumberX(Double_t valueX)
  {
    Int_t i_return = -1;

    for(Int_t i=0; i<nBinX_; i++)
    {
      if( vec_binEdgeX_[i] < valueX && valueX < vec_binEdgeX_[i+1] )
      {       
        i_return = i;
        break;
      }
    }

    return i_return;
  }

};

class TnPHistProducer
{
public:
  TnPHistProducer()
  {
    cout << "Default constructor: ptCut = 0! ... " << endl;
    Set_ptCut( 0 );    
    Init();
  }

  TnPHistProducer( Double_t ptCut )
  {
    Set_ptCut( ptCut );
    Init();
  }

  ~TnPHistProducer()
  {
    delete TnPHistPt_;
    delete TnPHistHighPt_;
    delete TnPHistEta_;
    delete TnPHistPhi_;
    delete TnPHistVtx_;
    delete TnPHist2DPtEta_;
  }

  void Fill( TnPPairBase* pair, Double_t weight = 1.0 )
  {
    TnPHistPt_->Fill( pair, pair->ProbePt(), weight );
    TnPHistHighPt_->Fill( pair, pair->ProbePt(), weight );
    TnPHistEta_->Fill( pair, pair->ProbeEta(), weight );
    TnPHistPhi_->Fill( pair, pair->ProbePhi(), weight );
    TnPHistVtx_->Fill( pair, pair->nVtx(), weight );
    TnPHist2DPtEta_->Fill( pair, pair->ProbePt(), pair->ProbeEta(), weight );
  }

  void Save( TFile *f_output )
  {
    TnPHistPt_->Save( f_output );
    TnPHistHighPt_->Save( f_output );
    TnPHistEta_->Save( f_output );
    TnPHistPhi_->Save( f_output );
    TnPHistVtx_->Save( f_output );
    TnPHist2DPtEta_->Save( f_output );
  }

private:
  Double_t ptCut_;
  TnPHist* TnPHistPt_;
  TnPHist* TnPHistHighPt_;
  TnPHist* TnPHistEta_;
  TnPHist* TnPHistPhi_;
  TnPHist* TnPHistVtx_;
  TnPHist2D* TnPHist2DPtEta_;

  void Init()
  {
    // -- binnig
    vector<Double_t> vec_PtBinEdge     = {0, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 10.5, 11, 11.5, 12, 12.5, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 30, 40, 50, 60, 120, 200, 500};
    vector<Double_t> vec_HighPtBinEdge = {2, 18, 22, 24, 26, 30, 40, 50 ,60 ,120 ,200, 300, 500}; // -- usual muon HLT (IsoMu24)
    vector<Double_t> vec_EtaBinEdge = {-2.4, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.4};
    Double_t degree15 = 3.141592 / 12.0;
    vector<Double_t> vec_PhiBinEdge =
    {
      (-1)*degree15*12, (-1)*degree15*11, (-1)*degree15*9, (-1)*degree15*7, (-1)*degree15*5, 
      (-1)*degree15*3, (-1)*degree15*1, degree15*1, degree15*3, degree15*5, 
      degree15*7, degree15*9, degree15*11, degree15*12
    };
    vector<Double_t> vec_VtxBinEdge = 
    {
      2.5, 4.5, 6.5, 8.5, 10.5, 12.5, 14.5, 16.5, 18.5, 20.5, 
      22.5, 24.5, 26.5, 28.5, 30.5, 32.5, 34.5, 36.5, 38.5, 40.5,
      42.5, 44.5, 46.5, 48.5, 50.5, 52.5, 54.5, 56.5, 58.5, 60.5
    };

    vector<Double_t> vec_ptEtaBinEdge_pt  = {5, 8, 9, 10, 11, 12, 500};
    vector<Double_t> vec_ptEtaBinEdge_eta = {-2.4, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.4};

    TnPHistPt_     = new TnPHist("pt",     0,      vec_PtBinEdge);
    TnPHistHighPt_ = new TnPHist("highPt", 0,      vec_HighPtBinEdge);
    TnPHistEta_    = new TnPHist("eta",    ptCut_, vec_EtaBinEdge);
    TnPHistPhi_    = new TnPHist("phi",    ptCut_, vec_PhiBinEdge);
    TnPHistVtx_    = new TnPHist("vtx",    ptCut_, vec_VtxBinEdge);
    TnPHist2DPtEta_ = new TnPHist2D("pteta", 0, vec_ptEtaBinEdge_pt, vec_ptEtaBinEdge_eta);
  }

  void Set_ptCut( Double_t ptCut ) { ptCut_ = ptCut; }
};

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

      TString histNamePass = histNameBase + "_pass_" + binInfo;
      TH1D* hTempPass = PlotTool::Get_Hist( inputFileName_, histNamePass );
      vec_passHist.push_back( hTempPass );

      TString histNameFail = histNameBase + "_fail_" + binInfo;
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
      h_nPass->SetBinContent(i_bin, nEventPass );
      h_nPass->SetBinError( i_bin, sqrt(nEventPass) );

      Double_t nEventFail = CountEvent( vec_failHist[i] );
      Double_t nEventTotal = nEventPass + nEventFail;
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

class TnP2DEffTool
{
public:
  TString fileName_;

  Double_t minMass_;
  Double_t maxMass_;

  Int_t nBinX_;
  vector<Double_t> vec_binEdgeX_;

  vector<TGraphAsymmErrors*> vec_effGraph_;



  TnP2DEffTool(TString fileName, vector<Double_t> vec_binEdgeX): 
  fileName_(fileName),
  vec_binEdgeX_(vec_binEdgeX)
  {
    Init();
  }

  void SetCutAndCountRange(Double_t min, Double_t max)
  {
    minMass_ = min;
    maxMass_ = max;
  }

  vector<TGraphAsymmErrors*> CalcTnPEff_CutAndCount( TString varName )
  {
    vec_effGraph_.clear();

    for(Int_t i_x=0; i_x<nBinX_; i_x++)
    {
      TString binType = TString::Format("%s_%02dbinX", varName.Data(), i_x);

      TnPEffTool* tool = new TnPEffTool(fileName_);
      tool->SetCutAndCountRange(minMass_, maxMass_);
      TGraphAsymmErrors* g_eff = tool->CalcTnPEff_CutAndCount( binType );

      vec_effGraph_.push_back( g_eff );
    }

    return vec_effGraph_;
  }

  TH2D* Get_2DEffMap()
  {
    if( vec_effGraph_.size() == 0 )
    {
      cout << "Run CalcTnPEff_CutAndCount(varName) first!" << endl;
      return nullptr;
    }

    vector<Double_t> vec_binEdgeY = Get_BinEdgeVector(vec_effGraph_[0]);
    Int_t nBinY = (Int_t)vec_binEdgeY.size()-1;

    Double_t* arr_binEdgeX = Convert_VectorToArray(vec_binEdgeX_);
    Double_t* arr_binEdgeY = Convert_VectorToArray(vec_binEdgeY);

    TH2D* h2D_eff = new TH2D("h2D_eff", "", nBinX_, arr_binEdgeX, nBinY, arr_binEdgeY);
    for(Int_t i_x=0; i_x<nBinX_; i_x++)
    {
      Int_t i_binX = i_x+1;

      TGraphAsymmErrors* g_eff = vec_effGraph_[i_x];

      Int_t nPoint = g_eff->GetN();
      for(Int_t i_y=0; i_y<nPoint; i_y++)
      {
        Int_t i_binY = i_y+1;

        Double_t temp;
        Double_t eff;
        g_eff->GetPoint(i_y, temp, eff);

        Double_t error_low  = g_eff->GetErrorYlow(i_y);
        Double_t error_high = g_eff->GetErrorYhigh(i_y);
        Double_t error = error_low > error_high ? error_low : error_high; // -- larger one

        h2D_eff->SetBinContent(i_binX, i_binY, eff);
        h2D_eff->SetBinError(i_binX, i_binY, error);
      }
    }

    return h2D_eff;
  }

  Double_t* Convert_VectorToArray(vector<Double_t> v)
  {
    Int_t nElement = (Int_t)v.size();
    Double_t* arr = new Double_t[nElement]; // -- dynamic allocation   
    for(Int_t i=0; i<nElement; i++)
      arr[i] = v[i];

    return arr;
  }

private:
  void Init()
  {
    vec_effGraph_.clear();

    minMass_ = 81;
    maxMass_ = 101;
    cout << "==========================================================" << endl;
    cout << "Default cut & count mass range: " << minMass_ << " < M < " << maxMass_ << " GeV" << endl;
    cout << "==========================================================" << endl;

    nBinX_ = (Int_t)vec_binEdgeX_.size()-1;
  }

  vector<Double_t> Get_BinEdgeVector(TGraphAsymmErrors* g)
  {
    vector<Double_t> vec_binEdge;

    Int_t nPoint = g->GetN();
    for(Int_t i_p=0; i_p<nPoint; i_p++)
    {
      Double_t x, y;
      g->GetPoint(i_p, x, y);
      Double_t errorX_low  = g->GetErrorXlow(i_p);
      Double_t binEdge = x - errorX_low;
      vec_binEdge.push_back( binEdge );

      if( i_p == nPoint - 1 ) // -- last point
      {
        Double_t errorX_high  = g->GetErrorXhigh(i_p);
        Double_t binEdge_last = x + errorX_high;
        vec_binEdge.push_back( binEdge_last );
      }
    }

    return vec_binEdge;
  }

};