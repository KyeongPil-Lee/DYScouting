import FWCore.ParameterSet.Config as cms

process = cms.Process("TreeProducer")

import sys
if len(sys.argv) < 2:
    print "usage: cmsRun ProduceTree.py <sample type>"
    print "   * available sample type: refer to DYScouting/TreeProducer/NtuplerArgument"
    sys.exit()

sampleType = sys.argv[1] # -- take
print "input sample type = ", sampleType

from DYScouting.TreeProducer.NtuplerArgument import GetArgument
theExampleFile, theGlobalTag, isMC = GetArgument( sampleType )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(theExampleFile), # -- @ KNU
    secondaryFileNames = cms.untracked.vstring(),
    # lumisToProcess = cms.untracked.VLuminosityBlockRange('258158:1-258158:1786'),
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = theGlobalTag

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("ntuple.root"),
  closeFileFast = cms.untracked.bool(False),
)

##--- l1 stage2 digis ---
process.load("EventFilter.L1TRawToDigi.gtStage2Digis_cfi")
process.gtStage2Digis.InputLabel = cms.InputTag( "hltFEDSelectorL1" )
process.load('PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff')

from DYScouting.TreeProducer.L1SeedList import GetL1SeedList

process.DYTree = cms.EDAnalyzer('DYTreeProducer',
  globalAlgBlk          = cms.untracked.InputTag("gtStage2Digis"),
  l1tAlgBlkInputTag     = cms.InputTag("gtStage2Digis"), # -- for L1TGlobalUtil
  l1tExtBlkInputTag     = cms.InputTag("gtStage2Digis"), # -- for L1TGlobalUtil
  ReadPrescalesFromFile = cms.bool( False ),             # -- for L1TGlobalUtil
  L1SeedList            = cms.untracked.vstring(GetL1SeedList()),
  triggerResults        = cms.untracked.InputTag("TriggerResults", "", "HLT"),

  scoutingVertex      = cms.untracked.InputTag("hltScoutingMuonPackerCalo",              "displacedVtx", "HLT"),
  pixelVertex         = cms.untracked.InputTag("hltScoutingPrimaryVertexPacker",         "primaryVtx",   "HLT"),
  pixelVertexNearMuon = cms.untracked.InputTag("hltScoutingPrimaryVertexPackerCaloMuon", "primaryVtx",   "HLT"),

  scoutingMuon   = cms.untracked.InputTag("hltScoutingMuonPackerCalo"),
  scoutingCaloJet = cms.untracked.InputTag("hltScoutingCaloPacker"),
  PUSummaryInfo  = cms.untracked.InputTag("addPileupInfo"),
  genEventInfo   = cms.untracked.InputTag("generator"),
  genParticle    = cms.untracked.InputTag("genParticles"),
  caloMETPhi     = cms.untracked.InputTag("hltScoutingCaloPacker", "caloMetPhi", "HLT"),
  caloMETPt      = cms.untracked.InputTag("hltScoutingCaloPacker", "caloMetPt", "HLT"),
  rho            = cms.untracked.InputTag("hltScoutingCaloPacker", "rho", "HLT"),
)

if isMC : 
    process.p = cms.Path(process.DYTree)
else : 
    process.p = cms.Path(process.gtStage2Digis + process.DYTree)