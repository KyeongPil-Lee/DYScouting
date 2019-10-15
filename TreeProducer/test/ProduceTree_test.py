import FWCore.ParameterSet.Config as cms

process = cms.Process("TreeProducer")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/u/user/kplee/scratch/Physics/RAW_ScoutingCaloMuon_Run2018C.root'), # -- Data @ KNU
    # fileNames = cms.untracked.vstring('file:/u/user/kplee/scratch/Physics/AODSIM_Autumn18_DY10to50_MadgraphMLM.root'), # -- MC @ KNU
    secondaryFileNames = cms.untracked.vstring(),
    # lumisToProcess = cms.untracked.VLuminosityBlockRange('258158:1-258158:1786'),
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = '101X_dataRun2_HLT_v7'

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("ntuple.root"),
  closeFileFast = cms.untracked.bool(False),
)

process.DYTree = cms.EDAnalyzer('DYTreeProducer',
  triggerResults = cms.InputTag("TriggerResults"),
  scoutingVertex = cms.InputTag("hltScoutingMuonPackerCalo", "displacedVtx", "HLT"),
  scoutingMuon   = cms.InputTag("hltScoutingMuonPackerCalo"),
  PUSummaryInfo  = cms.InputTag("addPileupInfo"),
  genEventInfo   = cms.InputTag("generator"),
  genParticle    = cms.InputTag("genParticles"), 
)

process.mypath = cms.EndPath(process.DYTree)
