//-------------------------------------------------
//
//   Class: DYTreeProducer
//
//   Description: flat ntuple producer for DY analysis w/ scouting data
//
//
//   Author:
//   K.P.Lee           Seoul National University
// 
//  Reference: https://github.com/schhibra/DarkPhotonAnalysis/blob/master/CMSSW_9_4_14/src/ScoutingAnalysis/TreeMaker/plugins/TreeMaker.cc (By Simranjit)
//--------------------------------------------------



// -- system include files
#include <memory>


// -- frameworks
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

// -- triggers
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "L1Trigger/L1TGlobal/interface/L1TGlobalUtil.h"
#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

// -- scouting
#include "DataFormats/Scouting/interface/ScoutingMuon.h"
#include "DataFormats/Scouting/interface/ScoutingCaloJet.h"
#include "DataFormats/Scouting/interface/ScoutingVertex.h"

// -- GEN info
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

// -- offline objects
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
// #include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
// #include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Scalers/interface/LumiScalers.h"

// -- for the extrapolation of the offline muon to the 2nd muon station
#include "MuonAnalysis/MuonAssociators/interface/PropagateToMuon.h"

// -- tracking
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"

// -- others
#include "CommonTools/UtilAlgos/interface/TFileService.h" 

// -- ROOT includes
#include <TTree.h>
#include <TLorentzVector.h>

using namespace std;
using namespace reco;
using namespace edm;

class DYTreeProducer : public edm::EDAnalyzer
{
public:
  explicit DYTreeProducer(const edm::ParameterSet&);
  ~DYTreeProducer();

private:
  virtual void analyze( const edm::Event&, const edm::EventSetup& );
  virtual void beginJob();
  virtual void endJob();
  virtual void beginRun( const edm::Run&, const edm::EventSetup& );
  virtual void endRun( const edm::Run&, const edm::EventSetup& );

  void Init();
  void Make_Branch();

  void Fill_L1( const edm::Event&, const edm::EventSetup& );
  void Fill_HLT( const edm::Event & );
  void Fill_L3MuonNoVtx( const edm::Event& );
  void Fill_ScoutingVertex( const edm::Event& );
  void Fill_PixelVertex( const edm::Event& );
  void Fill_PixelVertexNearMuon( const edm::Event& );
  void Fill_ScoutingMuon( const edm::Event& );
  void Fill_ScoutingCaloJet( const edm::Event& );
  void Fill_GenParticle( const edm::Event& );
  void Fill_OfflineMuon( const edm::Event&, const edm::EventSetup& );


  bool SavedTriggerCondition( std::string& );
  bool SavedFilterCondition( std::string& );


  // -- tokens
  edm::EDGetTokenT< l1t::MuonBxCollection >          t_L1Muon_;

  edm::EDGetTokenT< BXVector<GlobalAlgBlk> >         t_globalAlgBlk_;

  edm::EDGetTokenT< edm::TriggerResults >            t_triggerResults_;
  edm::EDGetTokenT< trigger::TriggerEvent >          t_triggerEvent_;

  edm::EDGetTokenT<std::vector<ScoutingVertex> >     t_scoutingVertex_;
  edm::EDGetTokenT<std::vector<ScoutingVertex> >     t_pixelVertex_;
  edm::EDGetTokenT<std::vector<ScoutingVertex> >     t_pixelVertexNearMuon_;

  edm::EDGetTokenT<std::vector<ScoutingMuon> >       t_scoutingMuon_;
  edm::EDGetTokenT<std::vector<ScoutingCaloJet> >    t_scoutingCaloJet_;

  edm::EDGetTokenT< std::vector<PileupSummaryInfo> > t_PUSummaryInfo_;
  
  edm::EDGetTokenT< GenEventInfoProduct >            t_genEventInfo_;
  edm::EDGetTokenT< reco::GenParticleCollection >    t_genParticle_;

  // const edm::EDGetTokenT<LHEEventProduct>         t_LHEEventProduct_; // -- it will be added later (if needed)

  edm::EDGetTokenT< double >  t_caloMETPhi_;
  edm::EDGetTokenT< double >  t_caloMETPt_;
  edm::EDGetTokenT< double >  t_rho_;


  // -- variable for L1 information
  vector< std::string> vec_L1Seed_;
  l1t::L1TGlobalUtil   *L1GtUtils_;

  // -- IterL3MuonCandidateNoVtx object
  edm::EDGetTokenT< std::vector<pat::TriggerObjectStandAlone> > t_trigObj_L3MuonNoVtx_;

  // -- isMiniAOD?
  bool isMiniAOD_;
  edm::EDGetTokenT< std::vector<pat::TriggerObjectStandAlone> > t_trigObj_miniAOD_;
  edm::EDGetTokenT< reco::VertexCollection >                    t_offlineVertex_;
  edm::EDGetTokenT< edm::View<reco::Muon> >                     t_offlineMuon_;

  // -- propagator
  PropagateToMuon propagatorToMuon_;

  // -- debugging switch
  bool debug_ = false;

  // -- tree
  TTree* ntuple_;

  // -- array size
  static const int arrSize_ = 2000;

  // --  event informations
  int runNum_;
  int lumiBlockNum_;
  unsigned long long eventNum_;

  double genWeight_;

  int truePU_;

  // -- trigger information
  vector< bool > vec_L1Bit_;
  vector< int >  vec_L1Prescale_;
  vector< std::string > vec_firedTrigger_;
  vector< std::string > vec_filterName_;
  vector< double > vec_HLTObj_pt_;
  vector< double > vec_HLTObj_eta_;
  vector< double > vec_HLTObj_phi_;

  // -- dimuon vertex information (@ HLT)
  int nVtx_;
  double vtx_x_[arrSize_];
  double vtx_y_[arrSize_];
  double vtx_z_[arrSize_];
  double vtx_xErr_[arrSize_];
  double vtx_yErr_[arrSize_];
  double vtx_zErr_[arrSize_];
  double vtx_chi2_[arrSize_];
  int    vtx_nDOF_[arrSize_];
  int    vtx_isValid_[arrSize_];

  // -- pixel vertex information from full tracking @ HLT
  int nPixelVtx_;
  double pixelVtx_x_[arrSize_];
  double pixelVtx_y_[arrSize_];
  double pixelVtx_z_[arrSize_];
  double pixelVtx_xErr_[arrSize_];
  double pixelVtx_yErr_[arrSize_];
  double pixelVtx_zErr_[arrSize_];
  double pixelVtx_chi2_[arrSize_];
  int    pixelVtx_nDOF_[arrSize_];
  int    pixelVtx_isValid_[arrSize_];

  // -- pixel vertex made by tracks near L3 muons @ HLT
  int nPixelVtxMu_;
  double pixelVtxMu_x_[arrSize_];
  double pixelVtxMu_y_[arrSize_];
  double pixelVtxMu_z_[arrSize_];
  double pixelVtxMu_xErr_[arrSize_];
  double pixelVtxMu_yErr_[arrSize_];
  double pixelVtxMu_zErr_[arrSize_];
  double pixelVtxMu_chi2_[arrSize_];
  int    pixelVtxMu_nDOF_[arrSize_];
  int    pixelVtxMu_isValid_[arrSize_];

  // -- muon information (scouting)
  int nMuon_;
  double muon_pt_[arrSize_];
  double muon_eta_[arrSize_];
  double muon_phi_[arrSize_];
  double muon_px_[arrSize_];
  double muon_py_[arrSize_];
  double muon_pz_[arrSize_];
  double muon_charge_[arrSize_];

  int muon_nPixelHit_[arrSize_];
  int muon_nStripHit_[arrSize_];
  int muon_nTrackerLayer_[arrSize_];
  int muon_nMuonHit_[arrSize_];
  int muon_nMatchedStation_[arrSize_];
  double muon_normChi2_[arrSize_];
  double muon_dxy_[arrSize_];
  double muon_dz_[arrSize_];
  double muon_trkIso_[arrSize_];
  vector< vector<int> > muon_vtxIndex_;

  // -- jet information
  int nCaloJet_;
  double caloJet_pt_[arrSize_];
  double caloJet_eta_[arrSize_];
  double caloJet_phi_[arrSize_];
  double caloJet_m_[arrSize_];
  double caloJet_jetArea_[arrSize_];
  double caloJet_maxEInEmTowers_[arrSize_];
  double caloJet_maxEInHadTowers_[arrSize_];
  double caloJet_hadEnergyInHB_[arrSize_];
  double caloJet_hadEnergyInHE_[arrSize_];
  double caloJet_hadEnergyInHF_[arrSize_];
  double caloJet_emEnergyInEB_[arrSize_];
  double caloJet_emEnergyInEE_[arrSize_];
  double caloJet_emEnergyInHF_[arrSize_];
  double caloJet_towersArea_[arrSize_];
  double caloJet_mvaDiscriminator_[arrSize_];
  double caloJet_btagDiscriminator_[arrSize_];


  // -- generator level information
  int nGenParticle_;
  int genParticle_ID_[arrSize_];
  int genParticle_status_[arrSize_];
  int genParticle_mother_[arrSize_];

  double genParticle_pt_[arrSize_];
  double genParticle_eta_[arrSize_];
  double genParticle_phi_[arrSize_];
  double genParticle_px_[arrSize_];
  double genParticle_py_[arrSize_];
  double genParticle_pz_[arrSize_];
  double genParticle_energy_[arrSize_];
  double genParticle_charge_[arrSize_];

  int genParticle_isPrompt_[arrSize_];
  int genParticle_isPromptFinalState_[arrSize_];
  int genParticle_isTauDecayProduct_[arrSize_];
  int genParticle_isPromptTauDecayProduct_[arrSize_];
  int genParticle_isDirectPromptTauDecayProductFinalState_[arrSize_];
  int genParticle_isHardProcess_[arrSize_];
  int genParticle_isLastCopy_[arrSize_];
  int genParticle_isLastCopyBeforeFSR_[arrSize_];
  int genParticle_isPromptDecayed_[arrSize_];
  int genParticle_isDecayedLeptonHadron_[arrSize_];
  int genParticle_fromHardProcessBeforeFSR_[arrSize_];
  int genParticle_fromHardProcessDecayed_[arrSize_];
  int genParticle_fromHardProcessFinalState_[arrSize_];
  int genParticle_isMostlyLikePythia6Status3_[arrSize_];

  // -- MET, Calo info.
  double caloMET_phi_;
  double caloMET_pt_;
  double rho_;

  // -- offline objects
  int nVertex_;

  // -- offline muon: offMuon_* (to distinguish with scouting muons)
  int nOffMuon_;

  double offMuon_pt_[arrSize_];
  double offMuon_eta_[arrSize_];
  double offMuon_phi_[arrSize_];
  double offMuon_px_[arrSize_];
  double offMuon_py_[arrSize_];
  double offMuon_pz_[arrSize_];
  double offMuon_dB_[arrSize_];
  double offMuon_charge_[arrSize_];
  int offMuon_isGLB_[arrSize_];
  int offMuon_isSTA_[arrSize_];
  int offMuon_isTRK_[arrSize_];
  int offMuon_isPF_[arrSize_];
  int offMuon_isTight_[arrSize_];
  int offMuon_isMedium_[arrSize_];
  int offMuon_isLoose_[arrSize_];
  int offMuon_isHighPt_[arrSize_];
  int offMuon_isSoft_[arrSize_];

  double offMuon_iso03_sumPt_[arrSize_];
  double offMuon_iso03_hadEt_[arrSize_];
  double offMuon_iso03_emEt_[arrSize_];

  double offMuon_PFIso03_charged_[arrSize_];
  double offMuon_PFIso03_neutral_[arrSize_];
  double offMuon_PFIso03_photon_[arrSize_];
  double offMuon_PFIso03_sumPU_[arrSize_];

  double offMuon_PFIso04_charged_[arrSize_];
  double offMuon_PFIso04_neutral_[arrSize_];
  double offMuon_PFIso04_photon_[arrSize_];
  double offMuon_PFIso04_sumPU_[arrSize_];

  double offMuon_PFCluster03_ECAL_[arrSize_];
  double offMuon_PFCluster03_HCAL_[arrSize_];

  double offMuon_PFCluster04_ECAL_[arrSize_];
  double offMuon_PFCluster04_HCAL_[arrSize_];

  double offMuon_normChi2_global_[arrSize_];
  int offMuon_nTrackerHit_global_[arrSize_];
  int offMuon_nTrackerLayer_global_[arrSize_];
  int offMuon_nPixelHit_global_[arrSize_];
  int offMuon_nMuonHit_global_[arrSize_];

  double offMuon_normChi2_inner_[arrSize_];
  int offMuon_nTrackerHit_inner_[arrSize_];
  int offMuon_nTrackerLayer_inner_[arrSize_];
  int offMuon_nPixelHit_inner_[arrSize_];

  double offMuon_pt_tuneP_[arrSize_];
  double offMuon_ptError_tuneP_[arrSize_];

  double offMuon_dxyVTX_best_[arrSize_];
  double offMuon_dzVTX_best_[arrSize_];

  int offMuon_nMatchedStation_[arrSize_];
  int offMuon_nMatchedRPCLayer_[arrSize_];
  int offMuon_stationMask_[arrSize_];

  double offMuon_pt_inner_[arrSize_];

  double offMuon_propEta_[arrSize_];
  double offMuon_propPhi_[arrSize_];

  std::vector<double> offMuon_vtxTrkChi2_;
  std::vector<double> offMuon_vtxTrkProb_;
  std::vector<double> offMuon_vtxTrkNdof_;
  std::vector<double> offMuon_vtxTrkPt1_;
  std::vector<double> offMuon_vtxTrkPt2_;

  int nL1Muon_;
  double L1Muon_pt_[arrSize_];
  double L1Muon_eta_[arrSize_];
  double L1Muon_phi_[arrSize_];
  double L1Muon_charge_[arrSize_];
  double L1Muon_quality_[arrSize_];

  int nL3MuonNoVtx_;
  double L3MuonNoVtx_pt_[arrSize_];
  double L3MuonNoVtx_eta_[arrSize_];
  double L3MuonNoVtx_phi_[arrSize_];
};

DYTreeProducer::DYTreeProducer(const edm::ParameterSet& iConfig):
t_L1Muon_              ( consumes< l1t::MuonBxCollection  >         (iConfig.getUntrackedParameter<edm::InputTag>("L1Muon")) ),
t_globalAlgBlk_        ( consumes< BXVector< GlobalAlgBlk > >       (iConfig.getUntrackedParameter<edm::InputTag>("globalAlgBlk")) ),
t_triggerResults_      ( consumes< edm::TriggerResults >            (iConfig.getUntrackedParameter<edm::InputTag>("triggerResults")) ),
t_triggerEvent_        ( mayConsume< trigger::TriggerEvent >        (iConfig.getUntrackedParameter<edm::InputTag>("triggerEvent")) ), // -- only for AOD (even not in RAW) -- //
t_scoutingVertex_      ( consumes< std::vector<ScoutingVertex> >    (iConfig.getUntrackedParameter<edm::InputTag>("scoutingVertex")) ),
t_pixelVertex_         ( consumes< std::vector<ScoutingVertex> >    (iConfig.getUntrackedParameter<edm::InputTag>("pixelVertex")) ),
t_pixelVertexNearMuon_ ( consumes< std::vector<ScoutingVertex> >    (iConfig.getUntrackedParameter<edm::InputTag>("pixelVertexNearMuon")) ),
t_scoutingMuon_        ( consumes< std::vector<ScoutingMuon> >      (iConfig.getUntrackedParameter<edm::InputTag>("scoutingMuon")) ),
t_scoutingCaloJet_     ( consumes< std::vector<ScoutingCaloJet> >   (iConfig.getUntrackedParameter<edm::InputTag>("scoutingCaloJet")) ),
t_PUSummaryInfo_       ( consumes< std::vector<PileupSummaryInfo> > (iConfig.getUntrackedParameter<edm::InputTag>("PUSummaryInfo")) ),
t_genEventInfo_        ( consumes< GenEventInfoProduct >            (iConfig.getUntrackedParameter<edm::InputTag>("genEventInfo")) ),
t_genParticle_         ( consumes< reco::GenParticleCollection >    (iConfig.getUntrackedParameter<edm::InputTag>("genParticle")) ),
t_caloMETPhi_          ( consumes< double >                         (iConfig.getUntrackedParameter<edm::InputTag>("caloMETPhi")) ),
t_caloMETPt_           ( consumes< double >                         (iConfig.getUntrackedParameter<edm::InputTag>("caloMETPt")) ),
t_rho_                 ( consumes< double >                         (iConfig.getUntrackedParameter<edm::InputTag>("rho")) ),
t_trigObj_L3MuonNoVtx_ ( consume< std::vector<pat::TriggerObjectStandAlone> >    (iConfig.getUntrackedParameter<edm::InputTag>("triggerObject_L3MuonNoVtx")) ), // -- only for miniAOD -- //
t_trigObj_miniAOD_     ( mayConsume< std::vector<pat::TriggerObjectStandAlone> > (iConfig.getUntrackedParameter<edm::InputTag>("triggerObject_miniAOD")) ), // -- only for miniAOD -- //
t_offlineVertex_       ( mayConsume< reco::VertexCollection >                    (iConfig.getUntrackedParameter<edm::InputTag>("offlineVertex")) ),
t_offlineMuon_         ( mayConsume< edm::View<reco::Muon> >                     (iConfig.getUntrackedParameter<edm::InputTag>("offlineMuon")) ),
propagatorToMuon_(iConfig)
{
   // usesResource("TFileService");
  vec_L1Seed_ = iConfig.getUntrackedParameter<std::vector<std::string> >("L1SeedList");
  L1GtUtils_  = new l1t::L1TGlobalUtil(iConfig, consumesCollector());

  isMiniAOD_ = iConfig.getUntrackedParameter<bool>("isMiniAOD");
  if( debug_ ) cout << "isMiniAOD_ = " << isMiniAOD_ << endl;
}

DYTreeProducer::~DYTreeProducer()
{

}

void DYTreeProducer::analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup)
{
  Init();

  // -- event information
  bool isRealData = iEvent.isRealData();

  runNum_       = iEvent.id().run();
  lumiBlockNum_ = iEvent.id().luminosityBlock();
  eventNum_     = iEvent.id().event();

  // -- True PU info: only for MC -- //
  if( !isRealData )
  {
    edm::Handle<std::vector< PileupSummaryInfo > > h_PUSummaryInfo;

    if( iEvent.getByToken(t_PUSummaryInfo_,h_PUSummaryInfo) )
    {
      std::vector<PileupSummaryInfo>::const_iterator PVI;
      for(PVI = h_PUSummaryInfo->begin(); PVI != h_PUSummaryInfo->end(); ++PVI)
      {
        if(PVI->getBunchCrossing()==0)
        {
          truePU_ = PVI->getTrueNumInteractions();
          continue;
        }
      } // -- end of PU iteration -- //
    } // -- end of if ( token exists )
  } // -- end of isMC -- //

  // -- offline vertex, if available
  edm::Handle<reco::VertexCollection> h_offlineVertex;
  if( iEvent.getByToken(t_offlineVertex_, h_offlineVertex) )
  {
    int nGoodVtx = 0;
    for(reco::VertexCollection::const_iterator it = h_offlineVertex->begin(); it != h_offlineVertex->end(); ++it)
      if( it->isValid() ) nGoodVtx++;

    nVertex_ = nGoodVtx;
  }

  // -- fill each object
  Fill_L1(iEvent, iSetup);
  Fill_HLT(iEvent);
  Fill_L3MuonNoVtx(iEvent);
  Fill_ScoutingVertex(iEvent);
  Fill_PixelVertex(iEvent);
  Fill_PixelVertexNearMuon(iEvent);
  Fill_ScoutingMuon(iEvent);
  Fill_ScoutingCaloJet(iEvent);
  Fill_OfflineMuon(iEvent, iSetup);
  if( !isRealData ) Fill_GenParticle(iEvent);


  // -- MET variables
  edm::Handle<double> h_caloMETPhi;
  iEvent.getByToken(t_caloMETPhi_, h_caloMETPhi);
  if( h_caloMETPhi.isValid() ) caloMET_phi_ = *(h_caloMETPhi.product());

  edm::Handle<double> h_caloMETPt;
  iEvent.getByToken(t_caloMETPt_, h_caloMETPt);
  if( h_caloMETPt.isValid() ) caloMET_pt_ = *(h_caloMETPt.product());

  edm::Handle<double> h_rho;
  iEvent.getByToken(t_rho_, h_rho);
  if( h_rho.isValid() ) rho_ = *(h_rho.product());

  // cout << "[MET] (phi, pt) = (" << caloMET_phi_ << ", " << caloMET_pt_ << ")" << endl;

  ntuple_->Fill();
}

void DYTreeProducer::beginJob()
{
  edm::Service<TFileService> fs;
  ntuple_ = fs->make<TTree>("ntuple","ntuple");

  Make_Branch();
}

void DYTreeProducer::Init()
{
  // --  event informations
  runNum_ = -999;
  lumiBlockNum_ = -999;
  eventNum_ = 0;

  genWeight_ = 0; // -- not -999; for analyzer code

  truePU_ = -999;

  // -- trigger information
  vec_L1Bit_.clear();
  vec_L1Prescale_.clear();
  vec_firedTrigger_.clear();
  vec_filterName_.clear();
  vec_HLTObj_pt_.clear();
  vec_HLTObj_eta_.clear();
  vec_HLTObj_phi_.clear();

  // -- vertex information (@ HLT)
  nVtx_ = -999;
  nPixelVtx_ = -999;
  nPixelVtxMu_ = -999;
  for(Int_t i=0; i<arrSize_; i++)
  {
    vtx_x_[i] = -999;
    vtx_y_[i] = -999;
    vtx_z_[i] = -999;
    vtx_xErr_[i] = -999;
    vtx_yErr_[i] = -999;
    vtx_zErr_[i] = -999;
    vtx_chi2_[i] = -999;
    vtx_nDOF_[i] = -999;
    vtx_isValid_[i] = 0;

    pixelVtx_x_[i] = -999;
    pixelVtx_y_[i] = -999;
    pixelVtx_z_[i] = -999;
    pixelVtx_xErr_[i] = -999;
    pixelVtx_yErr_[i] = -999;
    pixelVtx_zErr_[i] = -999;
    pixelVtx_chi2_[i] = -999;
    pixelVtx_nDOF_[i] = -999;
    pixelVtx_isValid_[i] = 0;

    pixelVtxMu_x_[i] = -999;
    pixelVtxMu_y_[i] = -999;
    pixelVtxMu_z_[i] = -999;
    pixelVtxMu_xErr_[i] = -999;
    pixelVtxMu_yErr_[i] = -999;
    pixelVtxMu_zErr_[i] = -999;
    pixelVtxMu_chi2_[i] = -999;
    pixelVtxMu_nDOF_[i] = -999;
    pixelVtxMu_isValid_[i] = 0;
  }

  // -- muon information
  nMuon_ = -999;
  for(Int_t i=0; i<arrSize_; i++)
  {
    muon_pt_[i] = -999;
    muon_eta_[i] = -999;
    muon_phi_[i] = -999;
    muon_px_[i] = -999;
    muon_py_[i] = -999;
    muon_pz_[i] = -999;
    muon_charge_[i] = -999;

    muon_nPixelHit_[i] = -999;
    muon_nStripHit_[i] = -999;
    muon_nTrackerLayer_[i] = -999;
    muon_nMuonHit_[i] = -999;
    muon_nMatchedStation_[i] = -999;
    muon_normChi2_[i] = -999;
    muon_dxy_[i] = -999;
    muon_dz_[i] = -999;
    muon_trkIso_[i] = -999;
  }
  muon_vtxIndex_.clear();

  // -- jet information
  nCaloJet_ = -999;
  for(Int_t i=0; i<arrSize_; i++)
  {
    caloJet_pt_[i] = -999;
    caloJet_eta_[i] = -999;
    caloJet_phi_[i] = -999;
    caloJet_m_[i] = -999;
    caloJet_jetArea_[i] = -999;
    caloJet_maxEInEmTowers_[i] = -999;
    caloJet_maxEInHadTowers_[i] = -999;
    caloJet_hadEnergyInHB_[i] = -999;
    caloJet_hadEnergyInHE_[i] = -999;
    caloJet_hadEnergyInHF_[i] = -999;
    caloJet_emEnergyInEB_[i] = -999;
    caloJet_emEnergyInEE_[i] = -999;
    caloJet_emEnergyInHF_[i] = -999;
    caloJet_towersArea_[i] = -999;
    caloJet_mvaDiscriminator_[i] = -999;
    caloJet_btagDiscriminator_[i] = -999;
  }

  // -- generator level information
  nGenParticle_ = -999;
  for(Int_t i=0; i<arrSize_; i++)
  {
    genParticle_ID_[i] = -999;
    genParticle_status_[i] = -999;
    genParticle_mother_[i] = -999;

    genParticle_pt_[i]  = -999;
    genParticle_eta_[i] = -999;
    genParticle_phi_[i] = -999;
    genParticle_px_[i]  = -999;
    genParticle_py_[i]  = -999;
    genParticle_pz_[i]  = -999;
    genParticle_energy_[i] = -999;
    genParticle_charge_[i] = -999;

    genParticle_isPrompt_[i] = 0;
    genParticle_isPromptFinalState_[i] = 0;
    genParticle_isTauDecayProduct_[i] = 0;
    genParticle_isPromptTauDecayProduct_[i] = 0;
    genParticle_isDirectPromptTauDecayProductFinalState_[i] = 0;
    genParticle_isHardProcess_[i] = 0;
    genParticle_isLastCopy_[i] = 0;
    genParticle_isLastCopyBeforeFSR_[i] = 0;
    genParticle_isPromptDecayed_[i] = 0;
    genParticle_isDecayedLeptonHadron_[i] = 0;
    genParticle_fromHardProcessBeforeFSR_[i] = 0;
    genParticle_fromHardProcessDecayed_[i] = 0;
    genParticle_fromHardProcessFinalState_[i] = 0;
    genParticle_isMostlyLikePythia6Status3_[i] = 0;
  }

  caloMET_phi_ = -999;
  caloMET_pt_ = -999;
  rho_ = -999;

  nVertex_ = 0;

  nOffMuon_ = 0;
  for( int i=0; i<arrSize_; i++)
  {
    offMuon_pt_[i] = -999;
    offMuon_eta_[i] = -999;
    offMuon_phi_[i] = -999;
    offMuon_px_[i] = -999;
    offMuon_py_[i] = -999;
    offMuon_pz_[i] = -999;
    offMuon_dB_[i] = -999;
    offMuon_charge_[i] = -999;
    offMuon_isGLB_[i] = 0;
    offMuon_isSTA_[i] = 0;
    offMuon_isTRK_[i] = 0;
    offMuon_isPF_[i] = 0;
    offMuon_isTight_[i] = 0;
    offMuon_isMedium_[i] = 0;
    offMuon_isLoose_[i] = 0;
    offMuon_isHighPt_[i] = 0;
    offMuon_isSoft_[i] = 0;

    offMuon_iso03_sumPt_[i] = -999;
    offMuon_iso03_hadEt_[i] = -999;
    offMuon_iso03_emEt_[i] = -999;

    offMuon_PFIso03_charged_[i] = -999;
    offMuon_PFIso03_neutral_[i] = -999;
    offMuon_PFIso03_photon_[i] = -999;
    offMuon_PFIso03_sumPU_[i] = -999;

    offMuon_PFIso04_charged_[i] = -999;
    offMuon_PFIso04_neutral_[i] = -999;
    offMuon_PFIso04_photon_[i] = -999;
    offMuon_PFIso04_sumPU_[i] = -999;

    offMuon_PFCluster03_ECAL_[i] = -999;
    offMuon_PFCluster03_HCAL_[i] = -999;

    offMuon_PFCluster04_ECAL_[i] = -999;
    offMuon_PFCluster04_HCAL_[i] = -999;

    offMuon_normChi2_global_[i] = -999;
    offMuon_nTrackerHit_global_[i] = -999;
    offMuon_nTrackerLayer_global_[i] = -999;
    offMuon_nPixelHit_global_[i] = -999;
    offMuon_nMuonHit_global_[i] = -999;

    offMuon_normChi2_inner_[i] = -999;
    offMuon_nTrackerHit_inner_[i] = -999;
    offMuon_nTrackerLayer_inner_[i] = -999;
    offMuon_nPixelHit_inner_[i] = -999;

    offMuon_pt_tuneP_[i] = -999;
    offMuon_ptError_tuneP_[i] = -999;

    offMuon_dxyVTX_best_[i] = -999;
    offMuon_dzVTX_best_[i] = -999;

    offMuon_nMatchedStation_[i] = -999;
    offMuon_nMatchedRPCLayer_[i] = -999;
    offMuon_stationMask_[i] = -999;

    offMuon_pt_inner_[i] = -999;

    offMuon_propEta_[i] = -999;
    offMuon_propPhi_[i] = -999;
  }

  offMuon_vtxTrkChi2_.clear();
  offMuon_vtxTrkProb_.clear();
  offMuon_vtxTrkNdof_.clear();
  offMuon_vtxTrkPt1_.clear();
  offMuon_vtxTrkPt2_.clear();

  nL1Muon_ = -999;
  for( int i=0; i<arrSize_; i++)
  {
    L1Muon_pt_[i] = -999;
    L1Muon_eta_[i] = -999;
    L1Muon_phi_[i] = -999;
    L1Muon_charge_[i] = -999;
    L1Muon_quality_[i] = -999;
  }

  nL3MuonNoVtx_ = -999;
  for( int i=0; i<arrSize_; i++)
  {
    L3MuonNoVtx_pt_[i] = -999;
    L3MuonNoVtx_eta_[i] = -999;
    L3MuonNoVtx_phi_[i] = -999;
  }

}

void DYTreeProducer::Make_Branch()
{
  ntuple_->Branch("runNum",&runNum_,"runNum/I");
  ntuple_->Branch("lumiBlockNum",&lumiBlockNum_,"lumiBlockNum/I");
  ntuple_->Branch("eventNum",&eventNum_,"eventNum/l"); // -- unsigned long long -- //
  ntuple_->Branch("truePU", &truePU_, "truePU/I");

  ntuple_->Branch("vec_L1Bit",        &vec_L1Bit_);
  ntuple_->Branch("vec_L1Prescale",   &vec_L1Prescale_);
  ntuple_->Branch("vec_firedTrigger", &vec_firedTrigger_);
  ntuple_->Branch("vec_filterName",   &vec_filterName_);
  ntuple_->Branch("vec_HLTObj_pt",    &vec_HLTObj_pt_);
  ntuple_->Branch("vec_HLTObj_eta",   &vec_HLTObj_eta_);
  ntuple_->Branch("vec_HLTObj_phi",   &vec_HLTObj_phi_);


  ntuple_->Branch("nVtx", &nVtx_, "nVtx/I");
  ntuple_->Branch("vtx_x", &vtx_x_, "vtx_x[nVtx]/D");
  ntuple_->Branch("vtx_y", &vtx_y_, "vtx_y[nVtx]/D");
  ntuple_->Branch("vtx_z", &vtx_z_, "vtx_z[nVtx]/D");
  ntuple_->Branch("vtx_xErr", &vtx_xErr_, "vtx_xErr[nVtx]/D");
  ntuple_->Branch("vtx_yErr", &vtx_yErr_, "vtx_yErr[nVtx]/D");
  ntuple_->Branch("vtx_zErr", &vtx_zErr_, "vtx_zErr[nVtx]/D");
  ntuple_->Branch("vtx_chi2", &vtx_chi2_, "vtx_chi2[nVtx]/D");
  ntuple_->Branch("vtx_nDOF", &vtx_nDOF_, "vtx_nDOF[nVtx]/I");
  ntuple_->Branch("vtx_isValid", &vtx_isValid_, "vtx_isValid[nVtx]/I");


  ntuple_->Branch("nPixelVtx", &nPixelVtx_, "nPixelVtx/I");
  ntuple_->Branch("pixelVtx_x", &pixelVtx_x_, "pixelVtx_x[nPixelVtx]/D");
  ntuple_->Branch("pixelVtx_y", &pixelVtx_y_, "pixelVtx_y[nPixelVtx]/D");
  ntuple_->Branch("pixelVtx_z", &pixelVtx_z_, "pixelVtx_z[nPixelVtx]/D");
  ntuple_->Branch("pixelVtx_xErr", &pixelVtx_xErr_, "pixelVtx_xErr[nPixelVtx]/D");
  ntuple_->Branch("pixelVtx_yErr", &pixelVtx_yErr_, "pixelVtx_yErr[nPixelVtx]/D");
  ntuple_->Branch("pixelVtx_zErr", &pixelVtx_zErr_, "pixelVtx_zErr[nPixelVtx]/D");
  ntuple_->Branch("pixelVtx_chi2", &pixelVtx_chi2_, "pixelVtx_chi2[nPixelVtx]/D");
  ntuple_->Branch("pixelVtx_nDOF", &pixelVtx_nDOF_, "pixelVtx_nDOF[nPixelVtx]/I");
  ntuple_->Branch("pixelVtx_isValid", &pixelVtx_isValid_, "pixelVtx_isValid[nPixelVtx]/I");


  ntuple_->Branch("nPixelVtxMu", &nPixelVtxMu_, "nPixelVtxMu/I");
  ntuple_->Branch("pixelVtxMu_x", &pixelVtxMu_x_, "pixelVtxMu_x[nPixelVtxMu]/D");
  ntuple_->Branch("pixelVtxMu_y", &pixelVtxMu_y_, "pixelVtxMu_y[nPixelVtxMu]/D");
  ntuple_->Branch("pixelVtxMu_z", &pixelVtxMu_z_, "pixelVtxMu_z[nPixelVtxMu]/D");
  ntuple_->Branch("pixelVtxMu_xErr", &pixelVtxMu_xErr_, "pixelVtxMu_xErr[nPixelVtxMu]/D");
  ntuple_->Branch("pixelVtxMu_yErr", &pixelVtxMu_yErr_, "pixelVtxMu_yErr[nPixelVtxMu]/D");
  ntuple_->Branch("pixelVtxMu_zErr", &pixelVtxMu_zErr_, "pixelVtxMu_zErr[nPixelVtxMu]/D");
  ntuple_->Branch("pixelVtxMu_chi2", &pixelVtxMu_chi2_, "pixelVtxMu_chi2[nPixelVtxMu]/D");
  ntuple_->Branch("pixelVtxMu_nDOF", &pixelVtxMu_nDOF_, "pixelVtxMu_nDOF[nPixelVtxMu]/I");
  ntuple_->Branch("pixelVtxMu_isValid", &pixelVtxMu_isValid_, "pixelVtxMu_isValid[nPixelVtxMu]/I");


  ntuple_->Branch("nMuon", &nMuon_, "nMuon/I");
  ntuple_->Branch("muon_pt", &muon_pt_, "muon_pt[nMuon]/D");
  ntuple_->Branch("muon_eta", &muon_eta_, "muon_eta[nMuon]/D");
  ntuple_->Branch("muon_phi", &muon_phi_, "muon_phi[nMuon]/D");
  ntuple_->Branch("muon_px", &muon_px_, "muon_px[nMuon]/D");
  ntuple_->Branch("muon_py", &muon_py_, "muon_py[nMuon]/D");
  ntuple_->Branch("muon_pz", &muon_pz_, "muon_pz[nMuon]/D");
  ntuple_->Branch("muon_charge", &muon_charge_, "muon_charge[nMuon]/D");

  ntuple_->Branch("muon_normChi2", &muon_normChi2_, "muon_normChi2[nMuon]/D");
  ntuple_->Branch("muon_nTrackerLayer", &muon_nTrackerLayer_, "muon_nTrackerLayer[nMuon]/I");
  ntuple_->Branch("muon_nPixelHit", &muon_nPixelHit_, "muon_nPixelHit[nMuon]/I");
  ntuple_->Branch("muon_nStripHit", &muon_nStripHit_, "muon_nStripHit[nMuon]/I");
  ntuple_->Branch("muon_nMuonHit", &muon_nMuonHit_, "muon_nMuonHit[nMuon]/I");
  ntuple_->Branch("muon_nMatchedStation", &muon_nMatchedStation_, "muon_nMatchedStation[nMuon]/I");
  ntuple_->Branch("muon_dxy", &muon_dxy_, "muon_dxy[nMuon]/D");
  ntuple_->Branch("muon_dz",  &muon_dz_,  "muon_dz[nMuon]/D");
  ntuple_->Branch("muon_trkIso", &muon_trkIso_, "muon_trkIso[nMuon]/D");
  ntuple_->Branch("muon_vtxIndex", &muon_vtxIndex_);


  ntuple_->Branch("nCaloJet", &nCaloJet_, "nCaloJet/I");
  ntuple_->Branch("caloJet_pt",  &caloJet_pt_,  "caloJet_pt[nCaloJet]/D");
  ntuple_->Branch("caloJet_eta", &caloJet_eta_, "caloJet_eta[nCaloJet]/D");
  ntuple_->Branch("caloJet_phi", &caloJet_phi_, "caloJet_phi[nCaloJet]/D");
  ntuple_->Branch("caloJet_m",   &caloJet_m_,   "caloJet_m[nCaloJet]/D");
  ntuple_->Branch("caloJet_jetArea",        &caloJet_jetArea_,           "caloJet_jetArea[nCaloJet]/D");
  ntuple_->Branch("caloJet_maxEInEmTowers", &caloJet_maxEInEmTowers_,    "caloJet_maxEInEmTowers[nCaloJet]/D");
  ntuple_->Branch("caloJet_maxEInHadTowers", &caloJet_maxEInHadTowers_, "caloJet_maxEInHadTowers_[nCaloJet]/D");
  ntuple_->Branch("caloJet_hadEnergyInHB",   &caloJet_hadEnergyInHB_,   "caloJet_hadEnergyInHB_[nCaloJet]/D");
  ntuple_->Branch("caloJet_hadEnergyInHE",   &caloJet_hadEnergyInHE_,   "caloJet_hadEnergyInHE_[nCaloJet]/D");
  ntuple_->Branch("caloJet_hadEnergyInHF",   &caloJet_hadEnergyInHF_,   "caloJet_hadEnergyInHF_[nCaloJet]/D");
  ntuple_->Branch("caloJet_emEnergyInEB",    &caloJet_emEnergyInEB_,    "caloJet_emEnergyInEB_[nCaloJet]/D");
  ntuple_->Branch("caloJet_emEnergyInEE",    &caloJet_emEnergyInEE_,    "caloJet_emEnergyInEE_[nCaloJet]/D");
  ntuple_->Branch("caloJet_emEnergyInHF",    &caloJet_emEnergyInHF_,    "caloJet_emEnergyInHF_[nCaloJet]/D");
  ntuple_->Branch("caloJet_towersArea",      &caloJet_towersArea_,      "caloJet_towersArea_[nCaloJet]/D");
  ntuple_->Branch("caloJet_mvaDiscriminator",  &caloJet_mvaDiscriminator_,  "caloJet_mvaDiscriminator_[nCaloJet]/D");
  ntuple_->Branch("caloJet_btagDiscriminator", &caloJet_btagDiscriminator_, "caloJet_btagDiscriminator_[nCaloJet]/D");



  ntuple_->Branch("genWeight", &genWeight_, "genWeight/D");
  ntuple_->Branch("nGenParticle", &nGenParticle_, "nGenParticle/I");
  ntuple_->Branch("genParticle_ID", &genParticle_ID_, "genParticle_ID[nGenParticle]/I");
  ntuple_->Branch("genParticle_status", &genParticle_status_, "genParticle_status[nGenParticle]/I");
  ntuple_->Branch("genParticle_mother", &genParticle_mother_, "genParticle_mother[nGenParticle]/I");
  ntuple_->Branch("genParticle_pt", &genParticle_pt_, "genParticle_pt[nGenParticle]/D");
  ntuple_->Branch("genParticle_eta", &genParticle_eta_, "genParticle_eta[nGenParticle]/D");
  ntuple_->Branch("genParticle_phi", &genParticle_phi_, "genParticle_phi[nGenParticle]/D");
  ntuple_->Branch("genParticle_px", &genParticle_px_, "genParticle_px[nGenParticle]/D");
  ntuple_->Branch("genParticle_py", &genParticle_py_, "genParticle_py[nGenParticle]/D");
  ntuple_->Branch("genParticle_pz", &genParticle_pz_, "genParticle_pz[nGenParticle]/D");
  ntuple_->Branch("genParticle_energy", &genParticle_energy_, "genParticle_energy[nGenParticle]/D");
  ntuple_->Branch("genParticle_charge", &genParticle_charge_, "genParticle_charge[nGenParticle]/D");
  ntuple_->Branch("genParticle_isPrompt", &genParticle_isPrompt_, "genParticle_isPrompt[nGenParticle]/I");
  ntuple_->Branch("genParticle_isPromptFinalState", &genParticle_isPromptFinalState_, "genParticle_isPromptFinalState[nGenParticle]/I");
  ntuple_->Branch("genParticle_isTauDecayProduct", &genParticle_isTauDecayProduct_, "genParticle_isTauDecayProduct[nGenParticle]/I");
  ntuple_->Branch("genParticle_isPromptTauDecayProduct", &genParticle_isPromptTauDecayProduct_, "genParticle_isPromptTauDecayProduct[nGenParticle]/I");
  ntuple_->Branch("genParticle_isDirectPromptTauDecayProductFinalState", &genParticle_isDirectPromptTauDecayProductFinalState_, "genParticle_isDirectPromptTauDecayProductFinalState[nGenParticle]/I");
  ntuple_->Branch("genParticle_isHardProcess", &genParticle_isHardProcess_, "genParticle_isHardProcess[nGenParticle]/I");
  ntuple_->Branch("genParticle_isLastCopy", &genParticle_isLastCopy_, "genParticle_isLastCopy[nGenParticle]/I");
  ntuple_->Branch("genParticle_isLastCopyBeforeFSR", &genParticle_isLastCopyBeforeFSR_, "genParticle_isLastCopyBeforeFSR[nGenParticle]/I");
  ntuple_->Branch("genParticle_isPromptDecayed", &genParticle_isPromptDecayed_, "genParticle_isPromptDecayed[nGenParticle]/I");
  ntuple_->Branch("genParticle_isDecayedLeptonHadron", &genParticle_isDecayedLeptonHadron_, "genParticle_isDecayedLeptonHadron[nGenParticle]/I");
  ntuple_->Branch("genParticle_fromHardProcessBeforeFSR", &genParticle_fromHardProcessBeforeFSR_, "genParticle_fromHardProcessBeforeFSR[nGenParticle]/I");
  ntuple_->Branch("genParticle_fromHardProcessDecayed", &genParticle_fromHardProcessDecayed_, "genParticle_fromHardProcessDecayed[nGenParticle]/I");
  ntuple_->Branch("genParticle_fromHardProcessFinalState", &genParticle_fromHardProcessFinalState_, "genParticle_fromHardProcessFinalState[nGenParticle]/I");
  ntuple_->Branch("genParticle_isMostlyLikePythia6Status3", &genParticle_isMostlyLikePythia6Status3_, "genParticle_isMostlyLikePythia6Status3[nGenParticle]/I");

  ntuple_->Branch("caloMET_phi", &caloMET_phi_, "caloMET_phi/D");
  ntuple_->Branch("caloMET_pt",  &caloMET_pt_,  "caloMET_pt/D");
  ntuple_->Branch("rho",         &rho_,         "rho/D");

  ntuple_->Branch("nVertex", &nVertex_, "nVertex/I");

  ntuple_->Branch("nOffMuon", &nOffMuon_, "nOffMuon/I");
  ntuple_->Branch("offMuon_pt", &offMuon_pt_, "offMuon_pt[nOffMuon]/D");
  ntuple_->Branch("offMuon_eta", &offMuon_eta_, "offMuon_eta[nOffMuon]/D");
  ntuple_->Branch("offMuon_phi", &offMuon_phi_, "offMuon_phi[nOffMuon]/D");
  ntuple_->Branch("offMuon_px", &offMuon_px_, "offMuon_px[nOffMuon]/D");
  ntuple_->Branch("offMuon_py", &offMuon_py_, "offMuon_py[nOffMuon]/D");
  ntuple_->Branch("offMuon_pz", &offMuon_pz_, "offMuon_pz[nOffMuon]/D");
  ntuple_->Branch("offMuon_dB", &offMuon_dB_, "offMuon_dB[nOffMuon]/D");
  ntuple_->Branch("offMuon_charge", &offMuon_charge_, "offMuon_charge[nOffMuon]/D");
  ntuple_->Branch("offMuon_isGLB", &offMuon_isGLB_, "offMuon_isGLB[nOffMuon]/I");
  ntuple_->Branch("offMuon_isSTA", &offMuon_isSTA_, "offMuon_isSTA[nOffMuon]/I");
  ntuple_->Branch("offMuon_isTRK", &offMuon_isTRK_, "offMuon_isTRK[nOffMuon]/I");
  ntuple_->Branch("offMuon_isPF", &offMuon_isPF_, "offMuon_isPF[nOffMuon]/I");
  ntuple_->Branch("offMuon_isTight", &offMuon_isTight_, "offMuon_isTight[nOffMuon]/I");
  ntuple_->Branch("offMuon_isMedium", &offMuon_isMedium_, "offMuon_isMedium[nOffMuon]/I");
  ntuple_->Branch("offMuon_isLoose", &offMuon_isLoose_, "offMuon_isLoose[nOffMuon]/I");
  ntuple_->Branch("offMuon_isHighPt", &offMuon_isHighPt_, "offMuon_isHighPt[nOffMuon]/I");
  ntuple_->Branch("offMuon_isSoft", &offMuon_isSoft_, "offMuon_isSoft[nOffMuon]/I");

  ntuple_->Branch("offMuon_iso03_sumPt", &offMuon_iso03_sumPt_, "offMuon_iso03_sumPt[nOffMuon]/D");
  ntuple_->Branch("offMuon_iso03_hadEt", &offMuon_iso03_hadEt_, "offMuon_iso03_hadEt[nOffMuon]/D");
  ntuple_->Branch("offMuon_iso03_emEt", &offMuon_iso03_emEt_, "offMuon_iso03_emEt[nOffMuon]/D");
  ntuple_->Branch("offMuon_PFIso03_charged", &offMuon_PFIso03_charged_, "offMuon_PFIso03_charged[nOffMuon]/D");
  ntuple_->Branch("offMuon_PFIso03_neutral", &offMuon_PFIso03_neutral_, "offMuon_PFIso03_neutral[nOffMuon]/D");
  ntuple_->Branch("offMuon_PFIso03_photon", &offMuon_PFIso03_photon_, "offMuon_PFIso03_photon[nOffMuon]/D");
  ntuple_->Branch("offMuon_PFIso03_sumPU", &offMuon_PFIso03_sumPU_, "offMuon_PFIso03_sumPU[nOffMuon]/D");
  ntuple_->Branch("offMuon_PFIso04_charged", &offMuon_PFIso04_charged_, "offMuon_PFIso04_charged[nOffMuon]/D");
  ntuple_->Branch("offMuon_PFIso04_neutral", &offMuon_PFIso04_neutral_, "offMuon_PFIso04_neutral[nOffMuon]/D");
  ntuple_->Branch("offMuon_PFIso04_photon", &offMuon_PFIso04_photon_, "offMuon_PFIso04_photon[nOffMuon]/D");
  ntuple_->Branch("offMuon_PFIso04_sumPU", &offMuon_PFIso04_sumPU_, "offMuon_PFIso04_sumPU[nOffMuon]/D");

  ntuple_->Branch("offMuon_PFCluster03_ECAL", &offMuon_PFCluster03_ECAL_, "offMuon_PFCluster03_ECAL[nOffMuon]/D");
  ntuple_->Branch("offMuon_PFCluster03_HCAL", &offMuon_PFCluster03_HCAL_, "offMuon_PFCluster03_HCAL[nOffMuon]/D");
  ntuple_->Branch("offMuon_PFCluster04_ECAL", &offMuon_PFCluster04_ECAL_, "offMuon_PFCluster04_ECAL[nOffMuon]/D");
  ntuple_->Branch("offMuon_PFCluster04_HCAL", &offMuon_PFCluster04_HCAL_, "offMuon_PFCluster04_HCAL[nOffMuon]/D");
  ntuple_->Branch("offMuon_normChi2_global", &offMuon_normChi2_global_, "offMuon_normChi2_global[nOffMuon]/D");
  ntuple_->Branch("offMuon_nTrackerHit_global", &offMuon_nTrackerHit_global_, "offMuon_nTrackerHit_global[nOffMuon]/I");
  ntuple_->Branch("offMuon_nTrackerLayer_global", &offMuon_nTrackerLayer_global_, "offMuon_nTrackerLayer_global[nOffMuon]/I");
  ntuple_->Branch("offMuon_nPixelHit_global", &offMuon_nPixelHit_global_, "offMuon_nPixelHit_global[nOffMuon]/I");
  ntuple_->Branch("offMuon_nMuonHit_global", &offMuon_nMuonHit_global_, "offMuon_nMuonHit_global[nOffMuon]/I");
  ntuple_->Branch("offMuon_normChi2_inner", &offMuon_normChi2_inner_, "offMuon_normChi2_inner[nOffMuon]/D");
  ntuple_->Branch("offMuon_nTrackerHit_inner", &offMuon_nTrackerHit_inner_, "offMuon_nTrackerHit_inner[nOffMuon]/I");
  ntuple_->Branch("offMuon_nTrackerLayer_inner", &offMuon_nTrackerLayer_inner_, "offMuon_nTrackerLayer_inner[nOffMuon]/I");
  ntuple_->Branch("offMuon_nPixelHit_inner", &offMuon_nPixelHit_inner_, "offMuon_nPixelHit_inner[nOffMuon]/I");
  ntuple_->Branch("offMuon_pt_tuneP", &offMuon_pt_tuneP_, "offMuon_pt_tuneP[nOffMuon]/D");
  ntuple_->Branch("offMuon_ptError_tuneP", &offMuon_ptError_tuneP_, "offMuon_ptError_tuneP[nOffMuon]/D");
  ntuple_->Branch("offMuon_dxyVTX_best", &offMuon_dxyVTX_best_, "offMuon_dxyVTX_best[nOffMuon]/D");
  ntuple_->Branch("offMuon_dzVTX_best", &offMuon_dzVTX_best_, "offMuon_dzVTX_best[nOffMuon]/D");
  ntuple_->Branch("offMuon_nMatchedStation", &offMuon_nMatchedStation_, "offMuon_nMatchedStation[nOffMuon]/I");
  ntuple_->Branch("offMuon_nMatchedRPCLayer", &offMuon_nMatchedRPCLayer_, "offMuon_nMatchedRPCLayer[nOffMuon]/I");
  ntuple_->Branch("offMuon_stationMask", &offMuon_stationMask_, "offMuon_stationMask[nOffMuon]/I");

  ntuple_->Branch("offMuon_pt_inner", &offMuon_pt_inner_, "offMuon_pt_inner[nOffMuon]/D");

  ntuple_->Branch("offMuon_propEta", &offMuon_propEta_, "offMuon_propEta[nOffMuon]/D");
  ntuple_->Branch("offMuon_propPhi", &offMuon_propPhi_, "offMuon_propPhi[nOffMuon]/D");

  ntuple_->Branch("offMuon_vtxTrkChi2", &offMuon_vtxTrkChi2_);
  ntuple_->Branch("offMuon_vtxTrkProb", &offMuon_vtxTrkProb_);
  ntuple_->Branch("offMuon_vtxTrkNdof", &offMuon_vtxTrkNdof_);
  ntuple_->Branch("offMuon_vtxTrkPt1",  &offMuon_vtxTrkPt1_);
  ntuple_->Branch("offMuon_vtxTrkPt2",  &offMuon_vtxTrkPt2_);

  ntuple_->Branch("nL1Muon",        &nL1Muon_,        "nL1Muon/I");
  ntuple_->Branch("L1Muon_pt",      &L1Muon_pt_,      "L1Muon_pt[nL1Muon]/D");
  ntuple_->Branch("L1Muon_eta",     &L1Muon_eta_,     "L1Muon_eta[nL1Muon]/D");
  ntuple_->Branch("L1Muon_phi",     &L1Muon_phi_,     "L1Muon_phi[nL1Muon]/D");
  ntuple_->Branch("L1Muon_charge",  &L1Muon_charge_,  "L1Muon_charge[nL1Muon]/D");
  ntuple_->Branch("L1Muon_quality", &L1Muon_quality_, "L1Muon_quality[nL1Muon]/D");

  ntuple_->Branch("nL3MuonNoVtx",        &nL3MuonNoVtx_,        "nL3MuonNoVtx/I");
  ntuple_->Branch("L3MuonNoVtx_pt",      &L3MuonNoVtx_pt_,      "L3MuonNoVtx_pt[nL3MuonNoVtx]/D");
  ntuple_->Branch("L3MuonNoVtx_eta",     &L3MuonNoVtx_eta_,     "L3MuonNoVtx_eta[nL3MuonNoVtx]/D");
  ntuple_->Branch("L3MuonNoVtx_phi",     &L3MuonNoVtx_phi_,     "L3MuonNoVtx_phi[nL3MuonNoVtx]/D");
}


void DYTreeProducer::Fill_GenParticle(const edm::Event &iEvent)
{
  // -- Gen-weight info -- //
  edm::Handle<GenEventInfoProduct> h_genEventInfo;
  iEvent.getByToken(t_genEventInfo_, h_genEventInfo);
  genWeight_ = h_genEventInfo->weight();

  // -- Gen-particle info -- //
  edm::Handle<reco::GenParticleCollection> h_genParticle;
  iEvent.getByToken(t_genParticle_, h_genParticle);

  int _nGenParticle = 0;
  for( size_t i=0; i< h_genParticle->size(); ++i)
  {
    const reco::GenParticle &parCand = (*h_genParticle)[i];

    if( abs(parCand.pdgId()) == 11 || abs(parCand.pdgId()) == 13 || abs(parCand.pdgId()) == 15 ) // -- only leptons for now -- //
    {
      genParticle_ID_[_nGenParticle]     = parCand.pdgId();
      genParticle_status_[_nGenParticle] = parCand.status();
      genParticle_mother_[_nGenParticle] = parCand.mother(0)->pdgId();

      genParticle_pt_[_nGenParticle]  = parCand.pt();
      genParticle_eta_[_nGenParticle] = parCand.eta();
      genParticle_phi_[_nGenParticle] = parCand.phi();
      genParticle_px_[_nGenParticle]  = parCand.px();
      genParticle_py_[_nGenParticle]  = parCand.py();
      genParticle_pz_[_nGenParticle]  = parCand.pz();
      genParticle_energy_[_nGenParticle] = parCand.energy();
      genParticle_charge_[_nGenParticle] = parCand.charge();

      if( parCand.statusFlags().isPrompt() )                genParticle_isPrompt_[_nGenParticle] = 1;
      if( parCand.statusFlags().isTauDecayProduct() )       genParticle_isTauDecayProduct_[_nGenParticle] = 1;
      if( parCand.statusFlags().isPromptTauDecayProduct() ) genParticle_isPromptTauDecayProduct_[_nGenParticle] = 1;
      if( parCand.statusFlags().isDecayedLeptonHadron() )   genParticle_isDecayedLeptonHadron_[_nGenParticle] = 1;

      if( parCand.isPromptFinalState() ) genParticle_isPromptFinalState_[_nGenParticle] = 1;
      if( parCand.isDirectPromptTauDecayProductFinalState() ) genParticle_isDirectPromptTauDecayProductFinalState_[_nGenParticle] = 1;
      if( parCand.isHardProcess() ) genParticle_isHardProcess_[_nGenParticle] = 1;
      if( parCand.isLastCopy() ) genParticle_isLastCopy_[_nGenParticle] = 1;
      if( parCand.isLastCopyBeforeFSR() ) genParticle_isLastCopyBeforeFSR_[_nGenParticle] = 1;

      if( parCand.isPromptDecayed() )           genParticle_isPromptDecayed_[_nGenParticle] = 1;
      if( parCand.fromHardProcessBeforeFSR() )  genParticle_fromHardProcessBeforeFSR_[_nGenParticle] = 1;
      if( parCand.fromHardProcessDecayed() )    genParticle_fromHardProcessDecayed_[_nGenParticle] = 1;
      if( parCand.fromHardProcessFinalState() ) genParticle_fromHardProcessFinalState_[_nGenParticle] = 1;
      // if( parCand.isMostlyLikePythia6Status3() ) this->genParticle_isMostlyLikePythia6Status3[_nGenParticle] = 1;

      _nGenParticle++;
    }
  }
  nGenParticle_ = _nGenParticle;
}


void DYTreeProducer::Fill_HLT(const edm::Event &iEvent)
{
  edm::Handle<edm::TriggerResults>  h_triggerResults;
  iEvent.getByToken(t_triggerResults_, h_triggerResults);

  // -- save trigger bit
  edm::TriggerNames triggerNames = iEvent.triggerNames(*h_triggerResults);
  for(unsigned int i_trig=0; i_trig<triggerNames.size(); ++i_trig)
  {
    LogDebug("triggers") << triggerNames.triggerName(i_trig);

    if( h_triggerResults->accept(i_trig) )
    {
      std::string pathName = triggerNames.triggerName(i_trig);
      if( SavedTriggerCondition(pathName) ) vec_firedTrigger_.push_back( pathName );
    } // -- end of if fired -- //

  } // -- end of iteration over all trigger names -- //

  // -- save trigger object (if available)
  if( isMiniAOD_ )
  {
    edm::Handle< std::vector<pat::TriggerObjectStandAlone> > h_triggerObject;
    iEvent.getByToken(t_trigObj_miniAOD_, h_triggerObject);

    for( pat::TriggerObjectStandAlone triggerObj : *h_triggerObject)
    {
      triggerObj.unpackNamesAndLabels(iEvent, *h_triggerResults);

      for( size_t i_filter = 0; i_filter < triggerObj.filterLabels().size(); ++i_filter )
      {
        // if( debug_ ) cout << "i_filter = " << i_filter << endl;
        std::string filterName = triggerObj.filterLabels()[i_filter];
        if( SavedFilterCondition(filterName) )
        {
          // if( debug_ ) cout << "  pass SavedFilterCondition" << endl;
          // if( debug_ ) cout << "  --> Trigger object: (pt, eta, phi) = (" << triggerObj.pt() << ", " << triggerObj.eta() << ", " << triggerObj.phi() << ")" << endl;

          vec_filterName_.push_back( filterName );
          vec_HLTObj_pt_.push_back( triggerObj.pt() );
          vec_HLTObj_eta_.push_back( triggerObj.eta() );
          vec_HLTObj_phi_.push_back( triggerObj.phi() );
        }

      } // -- loop over filters

    } // -- loop over trigger objects
  }
  else // -- AOD (reco data or MC) or RAW (scouting data) case
  {
    edm::Handle<trigger::TriggerEvent> h_triggerEvent;
    if( iEvent.getByToken(t_triggerEvent_, h_triggerEvent) ) // -- run only when TriggerEvent content is available (e.g. RAW tier (Scouting data) doesn't have TriggerEvent so below lines will not run. But it will run for usual data or MC in AOD format)
    {
      const trigger::size_type nFilter(h_triggerEvent->sizeFilters());
      for( trigger::size_type i_filter=0; i_filter<nFilter; i_filter++)
      {
        std::string filterName = h_triggerEvent->filterTag(i_filter).encode();

        if( SavedFilterCondition(filterName) )
        {
          trigger::Keys objectKeys = h_triggerEvent->filterKeys(i_filter);
          const trigger::TriggerObjectCollection& triggerObjects(h_triggerEvent->getObjects());

          for( trigger::size_type i_key=0; i_key<objectKeys.size(); i_key++)
          {
            trigger::size_type objKey = objectKeys.at(i_key);
            const trigger::TriggerObject& triggerObj(triggerObjects[objKey]);

            vec_filterName_.push_back( filterName );
            vec_HLTObj_pt_.push_back( triggerObj.pt() );
            vec_HLTObj_eta_.push_back( triggerObj.eta() );
            vec_HLTObj_phi_.push_back( triggerObj.phi() );
          }
        } // -- end of if( muon filters )-- //
      } // -- end of filter iteration -- //
    } // -- end of if(token is available)
  }
  
}

void DYTreeProducer::Fill_L3MuonNoVtx( const edm::Event& iEvent )
{
  Handle<std::vector<pat::TriggerObjectStandAlone> > h_L3MuonNoVtx;
  iEvent.getByToken(t_L3MuonNoVtx_, h_L3MuonNoVtx);

  if( h_L3MuonNoVtx.isValid() )
  {
    int _nL3MuonNoVtx = 0;
    for(unsigned int i_mu=0; i_mu<h_L3MuonNoVtx->size(); ++i_mu)
    {
      const pat::TriggerObjectStandAlone &triggerObj = (*h_L3MuonNoVtx)[i_mu];
      L3MuonNoVtx_pt_[i_mu] = triggerObj.pt();
      L3MuonNoVtx_eta_[i_mu] = triggerObj.eta();
      L3MuonNoVtx_phi_[i_mu] = triggerObj.phi();

      _nL3MuonNoVtx++;
    } // -- loop over trigger objects
  } // -- handle: valid?

  nL3MuonNoVtx_ = _nL3MuonNoVtx;
}


void DYTreeProducer::Fill_ScoutingVertex( const edm::Event& iEvent )
{
  Handle<std::vector<ScoutingVertex> > h_scoutingVertex;
  iEvent.getByToken(t_scoutingVertex_, h_scoutingVertex);

  int _nVtx = 0;
  if( h_scoutingVertex.isValid() )
  {
    // cout << "h_scoutingVertex->size() = " << h_scoutingVertex->size() << endl;

    for(unsigned int i_vtx=0; i_vtx<h_scoutingVertex->size(); ++i_vtx)
    {
      const ScoutingVertex &scoutingVertex = (*h_scoutingVertex)[i_vtx];

      vtx_x_[i_vtx]    = scoutingVertex.x();
      vtx_y_[i_vtx]    = scoutingVertex.y();
      vtx_z_[i_vtx]    = scoutingVertex.z();
      vtx_xErr_[i_vtx]    = scoutingVertex.zError();
      vtx_yErr_[i_vtx]    = scoutingVertex.yError();
      vtx_zErr_[i_vtx]    = scoutingVertex.zError();
      vtx_chi2_[i_vtx]    = scoutingVertex.chi2();
      vtx_nDOF_[i_vtx]    = scoutingVertex.ndof();
      vtx_isValid_[i_vtx] = scoutingVertex.isValidVtx();

      _nVtx++;
    }

    nVtx_ = _nVtx;
  }

}

void DYTreeProducer::Fill_PixelVertex( const edm::Event& iEvent )
{
  Handle<std::vector<ScoutingVertex> > h_pixelVertex;
  iEvent.getByToken(t_pixelVertex_, h_pixelVertex);

  int _nPixelVtx = 0;
  if( h_pixelVertex.isValid() )
  {
    for(unsigned int i_vtx=0; i_vtx<h_pixelVertex->size(); ++i_vtx)
    {
      const ScoutingVertex &scoutingVertex = (*h_pixelVertex)[i_vtx];

      pixelVtx_x_[i_vtx]    = scoutingVertex.x();
      pixelVtx_y_[i_vtx]    = scoutingVertex.y();
      pixelVtx_z_[i_vtx]    = scoutingVertex.z();
      pixelVtx_xErr_[i_vtx]    = scoutingVertex.zError();
      pixelVtx_yErr_[i_vtx]    = scoutingVertex.yError();
      pixelVtx_zErr_[i_vtx]    = scoutingVertex.zError();
      pixelVtx_chi2_[i_vtx]    = scoutingVertex.chi2();
      pixelVtx_nDOF_[i_vtx]    = scoutingVertex.ndof();
      pixelVtx_isValid_[i_vtx] = scoutingVertex.isValidVtx();

      _nPixelVtx++;
    }

    nPixelVtx_ = _nPixelVtx;
  }

}

void DYTreeProducer::Fill_PixelVertexNearMuon( const edm::Event& iEvent )
{
  Handle<std::vector<ScoutingVertex> > h_pixelVertexNearMuon;
  iEvent.getByToken(t_pixelVertexNearMuon_, h_pixelVertexNearMuon);

  int _nPixelVtxMu = 0;
  if( h_pixelVertexNearMuon.isValid() )
  {
    for(unsigned int i_vtx=0; i_vtx<h_pixelVertexNearMuon->size(); ++i_vtx)
    {
      const ScoutingVertex &scoutingVertex = (*h_pixelVertexNearMuon)[i_vtx];

      pixelVtxMu_x_[i_vtx]    = scoutingVertex.x();
      pixelVtxMu_y_[i_vtx]    = scoutingVertex.y();
      pixelVtxMu_z_[i_vtx]    = scoutingVertex.z();
      pixelVtxMu_xErr_[i_vtx]    = scoutingVertex.zError();
      pixelVtxMu_yErr_[i_vtx]    = scoutingVertex.yError();
      pixelVtxMu_zErr_[i_vtx]    = scoutingVertex.zError();
      pixelVtxMu_chi2_[i_vtx]    = scoutingVertex.chi2();
      pixelVtxMu_nDOF_[i_vtx]    = scoutingVertex.ndof();
      pixelVtxMu_isValid_[i_vtx] = scoutingVertex.isValidVtx();

      _nPixelVtxMu++;
    }

    nPixelVtxMu_ = _nPixelVtxMu;
  }

}

void DYTreeProducer::Fill_ScoutingMuon( const edm::Event& iEvent )
{
  Handle<std::vector<ScoutingMuon> > h_scoutingMuon;
  iEvent.getByToken(t_scoutingMuon_, h_scoutingMuon);

  int _nMuon = 0;
  if( h_scoutingMuon.isValid() )
  {
    // cout << "h_scoutingMuon->size() = " << h_scoutingMuon->size() << endl;

    for(unsigned int i_mu=0; i_mu<h_scoutingMuon->size(); ++i_mu)
    {
      const ScoutingMuon& muon = (*h_scoutingMuon)[i_mu];

      // muon_px_[i_mu]  = muon.px();
      // muon_py_[i_mu]  = muon.py();
      // muon_pz_[i_mu]  = muon.pz();
      muon_pt_[i_mu]  = muon.pt();
      muon_eta_[i_mu] = muon.eta();
      muon_phi_[i_mu] = muon.phi();
      muon_charge_[i_mu] = muon.charge();

      muon_nPixelHit_[i_mu]       = muon.nValidPixelHits();
      muon_nStripHit_[i_mu]       = muon.nValidStripHits();
      muon_nTrackerLayer_[i_mu]   = muon.nTrackerLayersWithMeasurement();
      muon_nMuonHit_[i_mu]        = muon.nValidMuonHits();
      muon_nMatchedStation_[i_mu] = muon.nMatchedStations();
      muon_normChi2_[i_mu]        = muon.ndof() > 0. ? muon.chi2() / muon.ndof() : 1e4;

      muon_dxy_[i_mu] = muon.dxy();
      muon_dz_[i_mu]  = muon.dz();

      muon_trkIso_[i_mu] = muon.trackIso();

      muon_vtxIndex_.push_back( muon.vtxIndx() );

      // cout << "[Scouting muon: isolation] (ECAL, HCAL) = (" << muon.ecalIso() << ", " << muon.hcalIso() << ")" << endl; 

      _nMuon++;
    }

    nMuon_ = _nMuon;
  }

}

void DYTreeProducer::Fill_ScoutingCaloJet( const edm::Event& iEvent )
{
  Handle<std::vector<ScoutingCaloJet> > h_scoutingCaloJet;
  iEvent.getByToken(t_scoutingCaloJet_, h_scoutingCaloJet);

  int _nCaloJet = 0;
  if( h_scoutingCaloJet.isValid() )
  {
    for(unsigned int i_jet=0; i_jet<h_scoutingCaloJet->size(); ++i_jet)
    {
      const ScoutingCaloJet& caloJet = (*h_scoutingCaloJet)[i_jet];

      caloJet_pt_[i_jet]   = caloJet.pt();
      caloJet_eta_[i_jet]  = caloJet.eta();
      caloJet_phi_[i_jet]  = caloJet.phi();
      caloJet_m_[i_jet]    = caloJet.m();

      caloJet_jetArea_[i_jet]         = caloJet.jetArea();
      caloJet_maxEInEmTowers_[i_jet]  = caloJet.maxEInEmTowers();
      caloJet_maxEInHadTowers_[i_jet] = caloJet.maxEInHadTowers();
      caloJet_hadEnergyInHB_[i_jet]   = caloJet.hadEnergyInHB();
      caloJet_hadEnergyInHE_[i_jet]   = caloJet.hadEnergyInHE();
      caloJet_hadEnergyInHF_[i_jet]   = caloJet.hadEnergyInHF();
      caloJet_emEnergyInEB_[i_jet]    = caloJet.emEnergyInEB();
      caloJet_emEnergyInEE_[i_jet]    = caloJet.emEnergyInEE();
      caloJet_emEnergyInHF_[i_jet]    = caloJet.emEnergyInHF();
      caloJet_towersArea_[i_jet]      = caloJet.towersArea();

      caloJet_mvaDiscriminator_[i_jet]  = caloJet.mvaDiscriminator();
      caloJet_btagDiscriminator_[i_jet] = caloJet.btagDiscriminator();

      _nCaloJet++;
    }

    nCaloJet_ = _nCaloJet;
  }

}

void DYTreeProducer::Fill_L1( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  // -- seed information
  L1GtUtils_->retrieveL1(iEvent, iSetup, t_globalAlgBlk_);

  for(unsigned int i_seed=0; i_seed<vec_L1Seed_.size(); i_seed++)
  {
    bool isFired = false;
    L1GtUtils_->getFinalDecisionByName(string(vec_L1Seed_[i_seed]), isFired);

    int L1Prescale = -999;
    L1GtUtils_->getPrescaleByName(string(vec_L1Seed_[i_seed]), L1Prescale );

    // cout << "L1 seed: " << vec_L1Seed_[i_seed] << endl;
    // cout << "--> decision = " << isFired << ", prescale = " << L1Prescale << endl;

    vec_L1Bit_.push_back( isFired );
    vec_L1Prescale_.push_back( L1Prescale );
  }

  // -- L1 muon information
  edm::Handle<l1t::MuonBxCollection> h_L1Muon;
  if( iEvent.getByToken(t_L1Muon_, h_L1Muon) )
  {
    int _nL1Muon = 0;
    for(int ibx = h_L1Muon->getFirstBX(); ibx<=h_L1Muon->getLastBX(); ++ibx)
    {
      if(ibx != 0) continue; // -- only take when ibx == 0 -- //
      for(auto it=h_L1Muon->begin(ibx); it!=h_L1Muon->end(ibx); it++)
      {
        l1t::MuonRef ref_L1Mu(h_L1Muon, distance(h_L1Muon->begin(h_L1Muon->getFirstBX()), it) );

        L1Muon_pt_[_nL1Muon]      = ref_L1Mu->pt();
        L1Muon_eta_[_nL1Muon]     = ref_L1Mu->eta();
        L1Muon_phi_[_nL1Muon]     = ref_L1Mu->phi();
        L1Muon_charge_[_nL1Muon]  = ref_L1Mu->charge();
        L1Muon_quality_[_nL1Muon] = ref_L1Mu->hwQual();

        _nL1Muon++;
      }
    }
    nL1Muon_ = _nL1Muon;
  }

}


bool DYTreeProducer::SavedTriggerCondition( std::string& pathName )
{
  bool flag = false;

  // -- muon triggers
  if( pathName.find("DST_DoubleMu") != std::string::npos ||
      pathName.find("HLT_IsoMu")    != std::string::npos ||
      pathName.find("HLT_Mu8")      != std::string::npos ||
      pathName.find("HLT_Mu17")     != std::string::npos ||
      pathName.find("HLT_Mu50")     != std::string::npos ||
      pathName.find("HLT_OldMu")    != std::string::npos ||
      pathName.find("HLT_TkMu")     != std::string::npos ||
      pathName.find("HLT_IsoTkMu")  != std::string::npos ||
      pathName.find("HLT_DoubleMu") != std::string::npos ||
      pathName.find("HLT_Ele32_WPTight_Gsf")                  != std::string::npos || // -- for Egamma, unprescaled triggers
      pathName.find("HLT_Ele115_CaloIdVT_GsfTrkIdT")          != std::string::npos ||
      pathName.find("HLT_Photon200")                          != std::string::npos ||
      pathName.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL") != std::string::npos ||
      pathName.find("HLT_DoubleEle25_CaloIdL_MW")             != std::string::npos ||
      pathName.find("HLT_DoublePhoton70")                     != std::string::npos ) flag = true;

  return flag;
}


bool DYTreeProducer::SavedFilterCondition( std::string& filterName )
{
  bool flag = false;

  // // -- muon filters (single muon)
  // if( (filterName.find("sMu") != std::string::npos || filterName.find("SingleMu") != std::string::npos) &&
  //      filterName.find("Tau")      == std::string::npos &&
  //      filterName.find("EG")       == std::string::npos &&
  //      filterName.find("MultiFit") == std::string::npos ) flag = true;

  // // -- muon filters (double muon)
  // if( (filterName.find("Dimuon") != std::string::npos || filterName.find("DoubleMu") != std::string::npos) &&
  //      filterName.find("Tau")      == std::string::npos &&
  //      filterName.find("EG")       == std::string::npos &&
  //      filterName.find("MultiFit") == std::string::npos ) flag = true;

  // if( (filterName.find("sMu")    != std::string::npos || filterName.find("SingleMu") != std::string::npos || 
  //      filterName.find("Dimuon") != std::string::npos || filterName.find("DoubleMu") != std::string::npos) ) flag = true;

  // -- keep only relavent trigger objects to reduce tree size
  // ---- DST_DoubleMu3_noVtx_CaloScouting_v*
  // ---- HLT_IsoMu24_v*
  // ---- HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v*
  // ---- HLT_Mu50_v*
  // ---- CAVEAT: the filter names are for 2018 setup; it needs to be checked (and updated if needed) if you want to run on 2016 or 17!
  if( filterName.find("hltL1sDoubleMuIorTripleMuIorQuadMu") != std::string::npos || 
      filterName.find("hltDimuon3L1Filtered0")              != std::string::npos ||
      filterName.find("hltDimuon3L2PreFiltered0")           != std::string::npos || 
      filterName.find("hltDoubleMu3L3FilteredNoVtx")        != std::string::npos || 
      filterName.find("hltL1sSingleMu22")                                            != std::string::npos || 
      filterName.find("hltL1fL1sMu22L1Filtered0")                                    != std::string::npos || 
      filterName.find("hltL3fL1sSingleMu22L1f0L2f10QL3Filtered24Q")                  != std::string::npos || 
      filterName.find("hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07") != std::string::npos || 
      filterName.find("hltL1sDoubleMu125to157")                        != std::string::npos || 
      filterName.find("hltL1fL1sDoubleMu155L1Filtered0")               != std::string::npos || 
      filterName.find("hltL3fL1DoubleMu155fPreFiltered8")              != std::string::npos || 
      filterName.find("hltL3fL1DoubleMu155fFiltered17")                != std::string::npos || 
      filterName.find("hltDiMuon178RelTrkIsoFiltered0p4")              != std::string::npos || 
      filterName.find("hltDiMuon178RelTrkIsoFiltered0p4DzFiltered0p2") != std::string::npos || 
      filterName.find("hltDiMuon178Mass3p8Filtered")                   != std::string::npos || 
      filterName.find("hltL1sSingleMu22or25")                     != std::string::npos || 
      filterName.find("hltL1fL1sMu22or25L1Filtered0")             != std::string::npos || 
      filterName.find("hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q") != std::string::npos ) flag = true;

  return flag;
}

void DYTreeProducer::Fill_OfflineMuon(const edm::Event &iEvent, const edm::EventSetup& iSetup)
{
  // -- initialize the propagator
  propagatorToMuon_.init(iSetup);

  // -- for the dimuon vertex variable
  // iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theTTBuilder);
  ESHandle<MagneticField> B;
  iSetup.get<IdealMagneticFieldRecord>().get(B);

  edm::Handle< edm::View<reco::Muon> > h_offlineMuon;
  iEvent.getByToken(t_offlineMuon_, h_offlineMuon);

  if( debug_ ) cout << "h_offlineMuon.isValid() = " << h_offlineMuon.isValid() << endl;

  if( h_offlineMuon.isValid() ) // -- only when the dataset has offline muon collection (e.g. AOD or miniAOD) -- //
  {
    edm::Handle<reco::VertexCollection> h_offlineVertex;
    iEvent.getByToken(t_offlineVertex_, h_offlineVertex);
    const reco::Vertex & pv = h_offlineVertex->at(0);

    int _nOffMuon = 0;

    // for(auto mu=h_offlineMuon->begin(); mu!=h_offlineMuon->end(); ++mu)
    for(unsigned int i_mu=0; i_mu<h_offlineMuon->size(); ++i_mu)
    {
      const auto& mu = (*h_offlineMuon)[i_mu]; // -- & is important; an error occurs without &

      offMuon_pt_[_nOffMuon]  = mu.pt();
      offMuon_eta_[_nOffMuon] = mu.eta();
      offMuon_phi_[_nOffMuon] = mu.phi();
      offMuon_px_[_nOffMuon]  = mu.px();
      offMuon_py_[_nOffMuon]  = mu.py();
      offMuon_pz_[_nOffMuon]  = mu.pz();
      offMuon_charge_[_nOffMuon] = mu.charge();
      // if( isMiniAOD_ ) offMuon_dB_[_nOffMuon] = mu.dB(); // -- dB is only availabe in pat::Muon -- //

      if( mu.isGlobalMuon() )     offMuon_isGLB_[_nOffMuon] = 1;
      if( mu.isStandAloneMuon() ) offMuon_isSTA_[_nOffMuon] = 1;
      if( mu.isTrackerMuon() )    offMuon_isTRK_[_nOffMuon] = 1;
      if( mu.isPFMuon() )         offMuon_isPF_[_nOffMuon] = 1;

      // -- defintion of ID functions: http://cmsdoxygen.web.cern.ch/cmsdoxygen/CMSSW_9_4_0/doc/html/da/d18/namespacemuon.html#ac122b2516e5711ce206256d7945473d2 -- //
      if( muon::isTightMuon( mu, pv ) )  offMuon_isTight_[_nOffMuon] = 1;
      if( muon::isMediumMuon( mu ) )     offMuon_isMedium_[_nOffMuon] = 1;
      if( muon::isLooseMuon( mu ) )      offMuon_isLoose_[_nOffMuon] = 1;
      if( muon::isHighPtMuon( mu, pv ) ) offMuon_isHighPt_[_nOffMuon] = 1;

      // -- bool muon::isSoftMuon(const reco::Muon& muon, const reco::Vertex& vtx, bool run2016_hip_mitigation)
      // -- it is different under CMSSW_8_0_29: bool muon::isSoftMuon(const reco::Muon& muon, const reco::Vertex& vtx)
      // -- Remove this part to avoid compile error (and soft muon would not be used for now) - need to be fixed at some point
      // if( muon::isSoftMuon( mu, pv, 0) ) offMuon_isSoft_[_nOffMuon] = 1;

      offMuon_iso03_sumPt_[_nOffMuon] = mu.isolationR03().sumPt;
      offMuon_iso03_hadEt_[_nOffMuon] = mu.isolationR03().hadEt;
      offMuon_iso03_emEt_[_nOffMuon]  = mu.isolationR03().emEt;

      offMuon_PFIso03_charged_[_nOffMuon] = mu.pfIsolationR03().sumChargedHadronPt;
      offMuon_PFIso03_neutral_[_nOffMuon] = mu.pfIsolationR03().sumNeutralHadronEt;
      offMuon_PFIso03_photon_[_nOffMuon]  = mu.pfIsolationR03().sumPhotonEt;
      offMuon_PFIso03_sumPU_[_nOffMuon]   = mu.pfIsolationR03().sumPUPt;

      offMuon_PFIso04_charged_[_nOffMuon] = mu.pfIsolationR04().sumChargedHadronPt;
      offMuon_PFIso04_neutral_[_nOffMuon] = mu.pfIsolationR04().sumNeutralHadronEt;
      offMuon_PFIso04_photon_[_nOffMuon]  = mu.pfIsolationR04().sumPhotonEt;
      offMuon_PFIso04_sumPU_[_nOffMuon]   = mu.pfIsolationR04().sumPUPt;

      reco::TrackRef globalTrk = mu.globalTrack();
      if( globalTrk.isNonnull() )
      {
        offMuon_normChi2_global_[_nOffMuon] = globalTrk->normalizedChi2();

        const reco::HitPattern & globalTrkHit = globalTrk->hitPattern();
        offMuon_nTrackerHit_global_[_nOffMuon]   = globalTrkHit.numberOfValidTrackerHits();
        offMuon_nTrackerLayer_global_[_nOffMuon] = globalTrkHit.trackerLayersWithMeasurement();
        offMuon_nPixelHit_global_[_nOffMuon]     = globalTrkHit.numberOfValidPixelHits();
        offMuon_nMuonHit_global_[_nOffMuon]   = globalTrkHit.numberOfValidMuonHits();
      }

      reco::TrackRef innerTrk = mu.innerTrack();
      if( innerTrk.isNonnull() )
      {
        offMuon_pt_inner_[_nOffMuon] = innerTrk->pt();
        offMuon_normChi2_inner_[_nOffMuon] = innerTrk->normalizedChi2();

        const reco::HitPattern & innerTrkHit = innerTrk->hitPattern();
        offMuon_nTrackerHit_inner_[_nOffMuon]   = innerTrkHit.numberOfValidTrackerHits();
        offMuon_nTrackerLayer_inner_[_nOffMuon] = innerTrkHit.trackerLayersWithMeasurement();
        offMuon_nPixelHit_inner_[_nOffMuon]     = innerTrkHit.numberOfValidPixelHits();
      }

      reco::TrackRef tunePTrk = mu.tunePMuonBestTrack();
      if( tunePTrk.isNonnull() )
      {
        offMuon_pt_tuneP_[_nOffMuon]      = tunePTrk->pt();
        offMuon_ptError_tuneP_[_nOffMuon] = tunePTrk->ptError();
      }

      offMuon_dxyVTX_best_[_nOffMuon] = mu.muonBestTrack()->dxy( pv.position() );
      offMuon_dzVTX_best_[_nOffMuon]  = mu.muonBestTrack()->dz( pv.position() );

      offMuon_nMatchedStation_[_nOffMuon] = mu.numberOfMatchedStations();
      offMuon_nMatchedRPCLayer_[_nOffMuon] = mu.numberOfMatchedRPCLayers();
      offMuon_stationMask_[_nOffMuon] = mu.stationMask();

      // -- propagation to the 2nd station and get eta and phi value (for the matching with L1 muons)
      TrajectoryStateOnSurface prop = propagatorToMuon_.extrapolate( *(mu.muonBestTrack()) );
      if( prop.isValid() )
      {
        offMuon_propEta_[_nOffMuon] = prop.globalPosition().eta();
        offMuon_propPhi_[_nOffMuon] = prop.globalPosition().phi();
        // printf("[Propagation: suceeded (isGLB, isTRK, isSTA, isPF) = (%d, %d, %d, %d)]\n", offMuon_isGLB_[_nOffMuon], offMuon_isTRK_[_nOffMuon], offMuon_isSTA_[_nOffMuon], offMuon_isPF_[_nOffMuon]);
        // printf("  (eta, propagated eta) = (%lf, %lf)\n", offMuon_eta_[_nOffMuon], offMuon_propEta_[_nOffMuon]);
        // printf("  (phi, propagated phi) = (%lf, %lf)\n", offMuon_phi_[_nOffMuon], offMuon_propPhi_[_nOffMuon]);
      }

      _nOffMuon++;

      // -- dimuon variables
      for(unsigned int j_mu=0; j_mu<h_offlineMuon->size(); j_mu++)
      {
        if( i_mu >= j_mu ) continue; // -- avoid double counting

        const auto& mu2 = h_offlineMuon->at(j_mu);

        reco::TrackRef InnerTrk  = mu.innerTrack();
        reco::TrackRef InnerTrk2 = mu2.innerTrack();

        if( InnerTrk.isNonnull() && InnerTrk2.isNonnull() )
        {
          reco::TransientTrack muTransient1(InnerTrk, B.product());
          reco::TransientTrack muTransient2(InnerTrk2, B.product());

          vector<reco::TransientTrack> dimuonTracksTrk;
          dimuonTracksTrk.push_back(muTransient1);
          dimuonTracksTrk.push_back(muTransient2);

          KalmanVertexFitter KalmanFitterTrk(true);
          CachingVertex<5> vertexTrk;

          bool isVertexTrk = true;
          try 
          { 
            vertexTrk = KalmanFitterTrk.vertex(dimuonTracksTrk);
          }
          catch(exception & err)
          { 
            isVertexTrk = false;
          }

          if( isVertexTrk && vertexTrk.isValid() )
          {
            offMuon_vtxTrkPt1_.push_back(InnerTrk->pt());
            offMuon_vtxTrkPt2_.push_back(InnerTrk2->pt());
            offMuon_vtxTrkChi2_.push_back(vertexTrk.totalChiSquared());
            offMuon_vtxTrkNdof_.push_back(vertexTrk.degreesOfFreedom());
            offMuon_vtxTrkProb_.push_back( TMath::Prob(vertexTrk.totalChiSquared(),(int)vertexTrk.degreesOfFreedom()) );
          }
        } // -- end of if( InnerTrk.isNonnull() && InnerTrk2.isNonnull() )
      } // -- end of j-th muon iteration

    } // -- end of i-th muon iteration
    nOffMuon_ = _nOffMuon;

  } // -- end of if( h_offlineMuon.isValid() )
}

void DYTreeProducer::endJob() {}
void DYTreeProducer::beginRun(const edm::Run &iRun, const edm::EventSetup &iSetup) {}
void DYTreeProducer::endRun(const edm::Run &iRun, const edm::EventSetup &iSetup) {}

DEFINE_FWK_MODULE(DYTreeProducer);
