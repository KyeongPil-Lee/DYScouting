class DYHistInfo
{
public:
  TString histName_;
  TString titleX_;

  Bool_t setRangeX_;
  Double_t minX_;
  Double_t maxX_;

  DYHistInfo()
  {
    histName_ = "";
    titleX_ = "";

    setRangeX_ = kFALSE;
    minX_ = 0;
    maxX_ = 0;
  }

  void SetHistName(TString histName) {histName_ = histName; }

  void SetTitleX(TString titleX) {titleX_ = titleX; }

  void SetRangeX(Double_t minX, Double_t maxX)
  {
    setRangeX_ = kTRUE;
    minX_ = minX;
    maxX_ = maxX;
  }
};

vector<DYHistInfo> GetAllDYHist()
{
  vector<DYHistInfo> vec_histInfo;

  DYHistInfo histInfo_diMuM;
  histInfo_diMuM.SetHistName( "h_diMuM" );
  histInfo_diMuM.SetTitleX( "m(#mu#mu) [GeV]" );
  histInfo_diMuM.SetRangeX( 10, 120 );
  vec_histInfo.push_back( histInfo_diMuM );

  DYHistInfo histInfo_diMuM_lowM_fineBin;
  histInfo_diMuM_lowM_fineBin.SetHistName( "h_diMuM_lowM_fineBin" );
  histInfo_diMuM_lowM_fineBin.SetTitleX( "m(#mu#mu) [GeV]" );
  histInfo_diMuM_lowM_fineBin.SetRangeX( 7, 13 );
  vec_histInfo.push_back( histInfo_diMuM_lowM_fineBin );

  // DYHistInfo histInfo_diMuM_Upsilon;
  // histInfo_diMuM_Upsilon.SetHistName( "h_diMuM_Upsilon" );
  // histInfo_diMuM_Upsilon.SetTitleX( "m(#mu#mu) [GeV]" );
  // histInfo_diMuM_Upsilon.SetRangeX( 7, 13 );
  // vec_histInfo.push_back( histInfo_diMuM_Upsilon );

  DYHistInfo histInfo_diMuRap;
  histInfo_diMuRap.SetHistName( "h_diMuRap" );
  histInfo_diMuRap.SetTitleX( "y(#mu#mu)" );
  vec_histInfo.push_back( histInfo_diMuRap );

  DYHistInfo histInfo_diMuPt;
  histInfo_diMuPt.SetHistName( "h_diMuPt" );
  histInfo_diMuPt.SetTitleX( "p_{T}(#mu#mu) [GeV]" );
  histInfo_diMuPt.SetRangeX( 0, 200 );
  vec_histInfo.push_back( histInfo_diMuPt );

  DYHistInfo histInfo_pt;
  histInfo_pt.SetHistName( "h_pt" );
  histInfo_pt.SetTitleX( "p_{T}(#mu) [GeV]" );
  histInfo_pt.SetRangeX( 0, 150 );
  vec_histInfo.push_back( histInfo_pt );

  DYHistInfo histInfo_eta;
  histInfo_eta.SetHistName( "h_eta" );
  histInfo_eta.SetTitleX( "#eta(#mu)" );
  vec_histInfo.push_back( histInfo_eta );

  DYHistInfo histInfo_phi;
  histInfo_phi.SetHistName( "h_phi" );
  histInfo_phi.SetTitleX( "#phi(#mu)" );
  vec_histInfo.push_back( histInfo_phi );

  DYHistInfo histInfo_nMuonHit;
  histInfo_nMuonHit.SetHistName( "h_nMuonHit" );
  histInfo_nMuonHit.SetTitleX( "# muon hits" );
  histInfo_nMuonHit.SetRangeX( 0, 80 );
  vec_histInfo.push_back( histInfo_nMuonHit );

  DYHistInfo histInfo_nMatchedStation;
  histInfo_nMatchedStation.SetHistName( "h_nMatchedStation" );
  histInfo_nMatchedStation.SetTitleX( "# matched stations" );
  histInfo_nMatchedStation.SetRangeX( 0, 5 );
  vec_histInfo.push_back( histInfo_nMatchedStation );

  DYHistInfo histInfo_nPixelHit;
  histInfo_nPixelHit.SetHistName( "h_nPixelHit" );
  histInfo_nPixelHit.SetTitleX( "# valid pixel hits" );
  vec_histInfo.push_back( histInfo_nPixelHit );

  DYHistInfo histInfo_nTrackerLayer;
  histInfo_nTrackerLayer.SetHistName( "h_nTrackerLayer" );
  histInfo_nTrackerLayer.SetTitleX( "# tracker layers" );
  vec_histInfo.push_back( histInfo_nTrackerLayer );

  DYHistInfo histInfo_normChi2;
  histInfo_normChi2.SetHistName( "h_normChi2" );
  histInfo_normChi2.SetTitleX( "Normalized #chi^{2}" );
  histInfo_normChi2.SetRangeX( 0, 12 );
  vec_histInfo.push_back( histInfo_normChi2 );

  DYHistInfo histInfo_relTrkIso;
  histInfo_relTrkIso.SetHistName( "h_relTrkIso" );
  histInfo_relTrkIso.SetTitleX( "Tracker isolation / p_{T}" );
  histInfo_relTrkIso.SetRangeX( 0, 0.20 );
  vec_histInfo.push_back( histInfo_relTrkIso );

  DYHistInfo histInfo_diMuOS;
  histInfo_diMuOS.SetHistName( "h_diMuOS" );
  histInfo_diMuOS.SetTitleX( "Opposite sign" );
  vec_histInfo.push_back( histInfo_diMuOS );

  DYHistInfo histInfo_diMuDEta;
  histInfo_diMuDEta.SetHistName( "h_diMuDEta" );
  histInfo_diMuDEta.SetTitleX( "#Delta#eta(#mu_{1}, #mu_{2})" );
  vec_histInfo.push_back( histInfo_diMuDEta );

  DYHistInfo histInfo_diMuDPhi;
  histInfo_diMuDPhi.SetHistName( "h_diMuDPhi" );
  histInfo_diMuDPhi.SetTitleX( "#Delta#phi(#mu_{1}, #mu_{2})" );
  vec_histInfo.push_back( histInfo_diMuDPhi );

  DYHistInfo histInfo_diMuAngle3D;
  histInfo_diMuAngle3D.SetHistName( "h_diMuAngle3D" );
  histInfo_diMuAngle3D.SetTitleX( "3D angle(#mu_{1}, #mu_{2})" );
  vec_histInfo.push_back( histInfo_diMuAngle3D );

  DYHistInfo histInfo_diMuDEtaCM;
  histInfo_diMuDEtaCM.SetHistName( "h_diMuDEtaCM" );
  histInfo_diMuDEtaCM.SetTitleX( "#Delta#eta(#mu_{1}, #mu_{2}) (CM frame)" );
  vec_histInfo.push_back( histInfo_diMuDEtaCM );

  DYHistInfo histInfo_diMuDPhiCM;
  histInfo_diMuDPhiCM.SetHistName( "h_diMuDPhiCM" );
  histInfo_diMuDPhiCM.SetTitleX( "#Delta#phi(#mu_{1}, #mu_{2}) (CM frame)" );
  vec_histInfo.push_back( histInfo_diMuDPhiCM );

  DYHistInfo histInfo_diMuAngle3DCM;
  histInfo_diMuAngle3DCM.SetHistName( "h_diMuAngle3DCM" );
  histInfo_diMuAngle3DCM.SetTitleX( "3D angle(#mu_{1}, #mu_{2}) (CM frame)" );
  vec_histInfo.push_back( histInfo_diMuAngle3DCM );

  DYHistInfo histInfo_diMuHasVertex;
  histInfo_diMuHasVertex.SetHistName( "h_diMuHasVertex" );
  histInfo_diMuHasVertex.SetTitleX( "has dimuon vertex" );
  vec_histInfo.push_back( histInfo_diMuHasVertex );

  DYHistInfo histInfo_diMuNormVtxChi2;
  histInfo_diMuNormVtxChi2.SetHistName( "h_diMuNormVtxChi2" );
  histInfo_diMuNormVtxChi2.SetTitleX( "Normalized vertex #chi^{2}" );
  histInfo_diMuNormVtxChi2.SetRangeX( 0, 15 );
  vec_histInfo.push_back( histInfo_diMuNormVtxChi2 );

  DYHistInfo histInfo_caloMET_pt;
  histInfo_caloMET_pt.SetHistName( "h_caloMET_pt" );
  histInfo_caloMET_pt.SetTitleX( "Calo MET [GeV]" );
  histInfo_caloMET_pt.SetRangeX( 0, 200 );
  vec_histInfo.push_back( histInfo_caloMET_pt );

  DYHistInfo histInfo_caloMET_phi;
  histInfo_caloMET_phi.SetHistName( "h_caloMET_phi" );
  histInfo_caloMET_phi.SetTitleX( "#phi(Calo MET)" );
  vec_histInfo.push_back( histInfo_caloMET_phi );

  DYHistInfo histInfo_diMuVtx_x;
  histInfo_diMuVtx_x.SetHistName( "h_diMuVtx_x" );
  histInfo_diMuVtx_x.SetTitleX( "x (dimuon vertex) [cm]" );
  histInfo_diMuVtx_x.SetRangeX( -0.5, 0.5 );
  vec_histInfo.push_back( histInfo_diMuVtx_x );

  DYHistInfo histInfo_diMuVtx_y;
  histInfo_diMuVtx_y.SetHistName( "h_diMuVtx_y" );
  histInfo_diMuVtx_y.SetTitleX( "y (dimuon vertex) [cm]" );
  histInfo_diMuVtx_y.SetRangeX( -0.5, 0.5 );
  vec_histInfo.push_back( histInfo_diMuVtx_y );

  DYHistInfo histInfo_diMuVtx_z;
  histInfo_diMuVtx_z.SetHistName( "h_diMuVtx_z" );
  histInfo_diMuVtx_z.SetTitleX( "z (dimuon vertex) [cm]" );
  histInfo_diMuVtx_z.SetRangeX( -0.5, 0.5 );
  vec_histInfo.push_back( histInfo_diMuVtx_z );

  DYHistInfo histInfo_diMuVtx_r;
  histInfo_diMuVtx_r.SetHistName( "h_diMuVtx_r" );
  histInfo_diMuVtx_r.SetTitleX( "#sqrt{x^{2}+y^{2}} (dimuon vertex) [cm]" );
  histInfo_diMuVtx_r.SetRangeX( 0, 1 );
  vec_histInfo.push_back( histInfo_diMuVtx_r );

  // DYHistInfo histInfo_diMuVtx_R;
  // histInfo_diMuVtx_R.SetHistName( "h_diMuVtx_R" );
  // histInfo_diMuVtx_R.SetTitleX( "#sqrt{x^{2}+y^{2}+z^{2}} (dimuon vertex) [cm]" );
  // histInfo_diMuVtx_r.SetRangeX( 0, 5 );
  // vec_histInfo.push_back( histInfo_diMuVtx_R );

  DYHistInfo histInfo_nVtx;
  histInfo_nVtx.SetHistName( "h_nVtx" );
  histInfo_nVtx.SetTitleX( "# dimuon vertex" );
  histInfo_nVtx.SetRangeX( 0, 10 );
  vec_histInfo.push_back( histInfo_nVtx );

  DYHistInfo histInfo_nPixelVtx;
  histInfo_nPixelVtx.SetHistName( "h_nPixelVtx" );
  histInfo_nPixelVtx.SetTitleX( "# pixel vertex (HLT tracking)" );
  histInfo_nPixelVtx.SetRangeX( 0, 30 );
  vec_histInfo.push_back( histInfo_nPixelVtx );

  DYHistInfo histInfo_nPixelVtxMu;
  histInfo_nPixelVtxMu.SetHistName( "h_nPixelVtxMu" );
  histInfo_nPixelVtxMu.SetTitleX( "# pixel vertex near HLT muons" );
  histInfo_nPixelVtxMu.SetRangeX( 0, 30 );
  vec_histInfo.push_back( histInfo_nPixelVtxMu );

  DYHistInfo histInfo_dxy;
  histInfo_dxy.SetHistName( "h_dxy" );
  histInfo_dxy.SetTitleX( "dxy [cm]" );
  vec_histInfo.push_back( histInfo_dxy );

  DYHistInfo histInfo_dz;
  histInfo_dz.SetHistName( "h_dz" );
  histInfo_dz.SetTitleX( "dz [cm]" );
  vec_histInfo.push_back( histInfo_dz );

  // -- Z peak histograms
  vector<DYHistInfo>  vec_histInfo_ZPeak;
  for(auto &histInfo : vec_histInfo)
  {
    DYHistInfo histInfo_ZPeak = histInfo;
    if( histInfo_ZPeak.histName_ == "h_diMuM") histInfo_ZPeak.SetRangeX( 60, 120 );

    TString histName = histInfo_ZPeak.histName_;
    histInfo_ZPeak.SetHistName( "ZPeak/"+histName );
    vec_histInfo_ZPeak.push_back( histInfo_ZPeak );
  }

  // -- M10-60 histograms
  vector<DYHistInfo>  vec_histInfo_M10to60;
  for(auto &histInfo : vec_histInfo)
  {
    DYHistInfo histInfo_M10to60 = histInfo;
    if( histInfo_M10to60.histName_ == "h_diMuM") histInfo_M10to60.SetRangeX( 10, 60 );

    TString histName = histInfo_M10to60.histName_;
    histInfo_M10to60.SetHistName( "M10to60/"+histName );
    vec_histInfo_M10to60.push_back( histInfo_M10to60 );
  }

  // -- M10-60, OS, histograms
  vector<DYHistInfo>  vec_histInfo_M10to60_OS;
  for(auto &histInfo : vec_histInfo)
  {
    DYHistInfo histInfo_M10to60_OS = histInfo;
    if( histInfo_M10to60_OS.histName_ == "h_diMuM")
    {
        histInfo_M10to60_OS.SetRangeX( 10, 60 );
        histInfo_M10to60_OS.SetTitleX( "m(#mu#mu) [GeV] (OS)" );
    }

    TString histName = histInfo_M10to60_OS.histName_;
    histInfo_M10to60_OS.SetHistName( "M10to60_OS/"+histName );
    vec_histInfo_M10to60_OS.push_back( histInfo_M10to60_OS );
  }

  // -- M10-60, SS, histograms
  vector<DYHistInfo>  vec_histInfo_M10to60_SS;
  for(auto &histInfo : vec_histInfo)
  {
    DYHistInfo histInfo_M10to60_SS = histInfo;
    if( histInfo_M10to60_SS.histName_ == "h_diMuM")
    {
        histInfo_M10to60_SS.SetRangeX( 10, 60 );
        histInfo_M10to60_SS.SetTitleX( "m(#mu#mu) [GeV] (SS)" );
    }

    TString histName = histInfo_M10to60_SS.histName_;
    histInfo_M10to60_SS.SetHistName( "M10to60_SS/"+histName );
    vec_histInfo_M10to60_SS.push_back( histInfo_M10to60_SS );
  }


  // -- combine
  for(auto &histInfo : vec_histInfo_ZPeak )
    vec_histInfo.push_back( histInfo );

  for(auto &histInfo : vec_histInfo_M10to60 )
    vec_histInfo.push_back( histInfo );

  for(auto &histInfo : vec_histInfo_M10to60_OS )
    vec_histInfo.push_back( histInfo );

  for(auto &histInfo : vec_histInfo_M10to60_SS )
    vec_histInfo.push_back( histInfo );


  return vec_histInfo;
}


