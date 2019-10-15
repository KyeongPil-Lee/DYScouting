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
#include "L1Trigger/L1TGlobal/interface/L1TGlobalUtil.h"
#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"

// -- scouting
#include "DataFormats/Scouting/interface/ScoutingMuon.h"
#include "DataFormats/Scouting/interface/ScoutingVertex.h"

// -- GEN info
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

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

  void Fill_HLT( const edm::Event & );
  void Fill_ScoutingVertex( const edm::Event& );
  void Fill_ScoutingMuon( const edm::Event& );
  void Fill_GenParticle( const edm::Event& );

  bool SavedTriggerCondition( std::string& );
  bool SavedFilterCondition( std::string& );

  // -- tokens
  edm::EDGetTokenT< edm::TriggerResults >            t_triggerResults_;
  // edm::EDGetTokenT< trigger::TriggerEvent >          t_triggerEvent_;

  edm::EDGetTokenT<std::vector<ScoutingVertex> >     t_scoutingVertex_;
  edm::EDGetTokenT<std::vector<ScoutingMuon> >       t_scoutingMuon_;

  edm::EDGetTokenT< std::vector<PileupSummaryInfo> > t_PUSummaryInfo_;
  
  edm::EDGetTokenT< GenEventInfoProduct >            t_genEventInfo_;
  edm::EDGetTokenT< reco::GenParticleCollection >    t_genParticle_;

  // const edm::EDGetTokenT<LHEEventProduct>         t_LHEEventProduct_; // -- it will be added later (if needed)

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
  vector< std::string > vec_firedTrigger_;
  vector< std::string > vec_filterName_;
  vector< double > vec_HLTObj_pt_;
  vector< double > vec_HLTObj_eta_;
  vector< double > vec_HLTObj_phi_;

  // -- vertex information (@ HLT)
  int nVtx_;
  double vtx_x_[arrSize_];
  double vtx_y_[arrSize_];
  double vtx_z_[arrSize_];
  double vtx_chi2_[arrSize_];

  // -- muon information
  int nMuon_;
  double muon_pt_[arrSize_];
  double muon_eta_[arrSize_];
  double muon_phi_[arrSize_];
  double muon_px_[arrSize_];
  double muon_py_[arrSize_];
  double muon_pz_[arrSize_];
  double muon_charge_[arrSize_];

  int muon_nPixelHit_[arrSize_];
  int muon_nTrackerLayer_[arrSize_];
  int muon_nMuonHit_[arrSize_];
  int muon_nMatchedStation_[arrSize_];
  double muon_normChi2_[arrSize_];
  double muon_dxy_[arrSize_];
  double muon_dz_[arrSize_];
  double muon_trkIso_[arrSize_];

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
};

DYTreeProducer::DYTreeProducer(const edm::ParameterSet& iConfig):
t_triggerResults_ ( consumes< edm::TriggerResults >            (iConfig.getUntrackedParameter<edm::InputTag>("triggerResults")) ),
// t_triggerEvent_   ( consumes< trigger::TriggerEvent >          (iConfig.getUntrackedParameter<edm::InputTag>("triggerEvent")) ),
t_scoutingVertex_ ( consumes< std::vector<ScoutingVertex> >    (iConfig.getUntrackedParameter<edm::InputTag>("scoutingVertex")) ),
t_scoutingMuon_   ( consumes< std::vector<ScoutingMuon> >      (iConfig.getUntrackedParameter<edm::InputTag>("scoutingMuon")) ),
t_PUSummaryInfo_  ( consumes< std::vector<PileupSummaryInfo> > (iConfig.getUntrackedParameter<edm::InputTag>("PUSummaryInfo")) ),
t_genEventInfo_   ( consumes< GenEventInfoProduct >            (iConfig.getUntrackedParameter<edm::InputTag>("genEventInfo")) ),
t_genParticle_    ( consumes< reco::GenParticleCollection >    (iConfig.getUntrackedParameter<edm::InputTag>("genParticle")) )
{
   // usesResource("TFileService");
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

  // -- fill each object
  Fill_HLT(iEvent);
  Fill_ScoutingVertex(iEvent);
  Fill_ScoutingMuon(iEvent);
  if( !isRealData ) Fill_GenParticle(iEvent);

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

  genWeight_ = -999;

  truePU_ = -999;

  // -- trigger information
  vec_firedTrigger_.clear();
  vec_filterName_.clear();
  vec_HLTObj_pt_.clear();
  vec_HLTObj_eta_.clear();
  vec_HLTObj_phi_.clear();

  // -- vertex information (@ HLT)
  nVtx_ = -999;
  for(Int_t i=0; i<arrSize_; i++)
  {
    vtx_x_[i] = -999;
    vtx_y_[i] = -999;
    vtx_z_[i] = -999;
    vtx_chi2_[i] = -999;
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
    muon_nTrackerLayer_[i] = -999;
    muon_nMuonHit_[i] = -999;
    muon_nMatchedStation_[i] = -999;
    muon_normChi2_[i] = -999;
    muon_dxy_[i] = -999;
    muon_dz_[i] = -999;
    muon_trkIso_[i] = -999;
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
}

void DYTreeProducer::Make_Branch()
{
  ntuple_->Branch("runNum",&runNum_,"runNum/I");
  ntuple_->Branch("lumiBlockNum",&lumiBlockNum_,"lumiBlockNum/I");
  ntuple_->Branch("eventNum",&eventNum_,"eventNum/l"); // -- unsigned long long -- //
  ntuple_->Branch("truePU", &truePU_, "truePU/I");

  ntuple_->Branch("vec_firedTrigger", &vec_firedTrigger_);
  ntuple_->Branch("vec_filterName", &vec_filterName_);
  ntuple_->Branch("vec_HLTObj_pt", &vec_HLTObj_pt_);
  ntuple_->Branch("vec_HLTObj_eta", &vec_HLTObj_eta_);
  ntuple_->Branch("vec_HLTObj_phi", &vec_HLTObj_phi_);


  ntuple_->Branch("nVtx", &nVtx_, "nVtx/I");
  ntuple_->Branch("vtx_x", &vtx_x_, "vtx_x[nVtx]/D");
  ntuple_->Branch("vtx_y", &vtx_y_, "vtx_y[nVtx]/D");
  ntuple_->Branch("vtx_z", &vtx_z_, "vtx_z[nVtx]/D");
  ntuple_->Branch("vtx_chi2", &vtx_chi2_, "vtx_chi2[nVtx]/D");


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
  ntuple_->Branch("muon_nMuonHit", &muon_nMuonHit_, "muon_nMuonHit[nMuon]/I");
  ntuple_->Branch("muon_nMatchedStation", &muon_nMatchedStation_, "muon_nMatchedStation[nMuon]/I");
  ntuple_->Branch("muon_dxy", &muon_dxy_, "muon_dxy[nMuon]/D");
  ntuple_->Branch("muon_dz",  &muon_dz_,  "muon_dz[nMuon]/D");
  ntuple_->Branch("muon_trkIso", &muon_trkIso_, "muon_trkIso[nMuon]/D");


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

    if( abs(parCand.pdgId()) == 13 ) // -- only muons -- //
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
  // edm::Handle<trigger::TriggerEvent> h_triggerEvent;

  iEvent.getByToken(t_triggerResults_, h_triggerResults);
  // iEvent.getByToken(t_triggerEvent_,   h_triggerEvent);

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

  // const trigger::size_type nFilter(h_triggerEvent->sizeFilters());
  // for( trigger::size_type i_filter=0; i_filter<nFilter; i_filter++)
  // {
  //   std::string filterName = h_triggerEvent->filterTag(i_filter).encode();

  //   if( SavedFilterCondition(filterName) )
  //   {
  //     trigger::Keys objectKeys = h_triggerEvent->filterKeys(i_filter);
  //     const trigger::TriggerObjectCollection& triggerObjects(h_triggerEvent->getObjects());

  //     for( trigger::size_type i_key=0; i_key<objectKeys.size(); i_key++)
  //     {
  //       trigger::size_type objKey = objectKeys.at(i_key);
  //       const trigger::TriggerObject& triggerObj(triggerObjects[objKey]);

  //       vec_filterName_.push_back( filterName );
  //       vec_HLTObj_pt_.push_back( triggerObj.pt() );
  //       vec_HLTObj_eta_.push_back( triggerObj.eta() );
  //       vec_HLTObj_phi_.push_back( triggerObj.phi() );
  //     }
  //   } // -- end of if( muon filters )-- //
  // } // -- end of filter iteration -- //
  
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
      vtx_chi2_[i_vtx] = scoutingVertex.chi2();

      _nVtx++;
    }

    nVtx_ = _nVtx;
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
      muon_nTrackerLayer_[i_mu]   = muon.nTrackerLayersWithMeasurement();
      muon_nMuonHit_[i_mu]        = muon.nValidMuonHits();
      muon_nMatchedStation_[i_mu] = muon.nMatchedStations();
      muon_normChi2_[i_mu]        = muon.ndof() > 0. ? muon.chi2() / muon.ndof() : 1e4;

      muon_dxy_[i_mu] = muon.dxy();
      muon_dz_[i_mu]  = muon.dz();

      muon_trkIso_[i_mu] = muon.trackIso();

      _nMuon++;
    }

    nMuon_ = _nMuon;
  }

}


bool DYTreeProducer::SavedTriggerCondition( std::string& pathName )
{
  bool flag = false;

  // -- muon triggers
  if( pathName.find("DST_DoubleMu") != std::string::npos ||
      pathName.find("HLT_Mu")       != std::string::npos ||
      pathName.find("HLT_OldMu")    != std::string::npos ||
      pathName.find("HLT_TkMu")     != std::string::npos ||
      pathName.find("HLT_IsoTkMu")  != std::string::npos ||
      pathName.find("HLT_DoubleMu") != std::string::npos ||
      pathName.find("HLT_Mu8_T")    != std::string::npos ) flag = true;

  return flag;
}


bool DYTreeProducer::SavedFilterCondition( std::string& filterName )
{
  bool flag = false;

  // -- muon filters (single muon)
  if( (filterName.find("sMu") != std::string::npos || filterName.find("SingleMu") != std::string::npos) &&
       filterName.find("Tau")      == std::string::npos &&
       filterName.find("EG")       == std::string::npos &&
       filterName.find("MultiFit") == std::string::npos ) flag = true;

  // -- muon filters (double muon)
  if( (filterName.find("Dimuon") != std::string::npos || filterName.find("DoubleMu") != std::string::npos) &&
       filterName.find("Tau")      == std::string::npos &&
       filterName.find("EG")       == std::string::npos &&
       filterName.find("MultiFit") == std::string::npos ) flag = true;

  return flag;
}

void DYTreeProducer::endJob() {}
void DYTreeProducer::beginRun(const edm::Run &iRun, const edm::EventSetup &iSetup) {}
void DYTreeProducer::endRun(const edm::Run &iRun, const edm::EventSetup &iSetup) {}

DEFINE_FWK_MODULE(DYTreeProducer);
