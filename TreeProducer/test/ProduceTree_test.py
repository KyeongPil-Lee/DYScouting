import FWCore.ParameterSet.Config as cms

# isMC = True
isMC = False

if isMC:
  fileName = 'file:/u/user/kplee/scratch/Physics/AODSIM_Autumn18_DY10to50_MadgraphMLM.root'
  theGT = '102X_upgrade2018_realistic_v15'
else:
  fileName = 'file:/u/user/kplee/scratch/Physics/RAW_ScoutingCaloMuon_Run2018C_v2.root'
  theGT = '101X_dataRun2_HLT_v7'

process = cms.Process("TreeProducer")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(fileName), # -- @ KNU
    secondaryFileNames = cms.untracked.vstring(),
    # lumisToProcess = cms.untracked.VLuminosityBlockRange('258158:1-258158:1786'),
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(5000))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = theGT

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("ntuple.root"),
  closeFileFast = cms.untracked.bool(False),
)

process.DYTree = cms.EDAnalyzer('DYTreeProducer',
  triggerResults = cms.untracked.InputTag("TriggerResults", "", "HLT"),
  scoutingVertex = cms.untracked.InputTag("hltScoutingMuonPackerCalo", "displacedVtx", "HLT"),
  scoutingMuon   = cms.untracked.InputTag("hltScoutingMuonPackerCalo"),
  PUSummaryInfo  = cms.untracked.InputTag("addPileupInfo"),
  genEventInfo   = cms.untracked.InputTag("generator"),
  genParticle    = cms.untracked.InputTag("genParticles"), 
)

process.mypath = cms.EndPath(process.DYTree)
