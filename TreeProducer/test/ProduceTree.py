import FWCore.ParameterSet.Config as cms

"The HLT objects saved in the tree are the ones matched with the filter names from 2018 HLT menu"
"DYTreeProducer::SavedFilterCondition should be updated if you want to run it on 2016 or 17!"

# -- usage: usage: cmsRun ProduceTree.py sampleType=<sample type>

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.register('sampleType',
                  "none", # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,         # string, int, or float
                  "Sample type defined in DYScouting/TreeProducer/NtuplerArgument.py")

options.parseArguments()

print "input sample type = ", options.sampleType

from DYScouting.TreeProducer.NtuplerArgument import GetArgument
theExampleFile, theGlobalTag, isMC, isMiniAOD = GetArgument( options.sampleType )

print "   [example file] ", theExampleFile
print "   [global tag]   ", theGlobalTag
print "   [isMC]         ", isMC
print "   [isMiniAOD]    ", isMiniAOD
print ""

process = cms.Process("TreeProducer")

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

# -- for the extrapolation of offlie muon to 2nd muon station
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi")
process.load("TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi")

##--- l1 stage2 digis ---
process.load("EventFilter.L1TRawToDigi.gtStage2Digis_cfi")
process.gtStage2Digis.InputLabel = cms.InputTag( "hltFEDSelectorL1" )
process.load('PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff')

from DYScouting.TreeProducer.L1SeedList import GetL1SeedList

process.DYTree = cms.EDAnalyzer('DYTreeProducer',
  L1Muon                = cms.untracked.InputTag("gmtStage2Digis", "Muon", "RECO"),
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

  isMiniAOD              = cms.untracked.bool(isMiniAOD),
  triggerObject_miniAOD  = cms.untracked.InputTag("notUsed"),
  offlineMuon            = cms.untracked.InputTag("muons"), # -- will be skipped if the collection is not available (e.g. RAW)
  offlineVertex          = cms.untracked.InputTag("offlinePrimaryVertices"), # -- will be skipped if the collection is not available (e.g. RAW)

  # -- for the extrapolation of offlie muon to 2nd muon station
  preselection = cms.string("gmtMuonCand.quality > 1"), # FIXME: maybe exclude CSC-only region?
  useTrack     = cms.string("tracker"),
  useState     = cms.string("atVertex"),
  maxDeltaR    = cms.double(1.5),             ## FIXME: to be tuned
  maxDeltaEta  = cms.double(0.3),             ## FIXME: to be tuned
  l1PhiOffset  = cms.double(1.25 * 3.14159265359/180.),
  useSimpleGeometry = cms.bool(True),
  fallbackToME1     = cms.bool(True),
)

if isMiniAOD:
  print "[ProduceTree.py] run with miniAOD mode"
  process.DYTree.isMiniAOD             = cms.untracked.bool(True)
  process.DYTree.genParticle           = cms.untracked.InputTag("prunedGenParticles")
  process.DYTree.PUSummaryInfo         = cms.untracked.InputTag("slimmedAddPileupInfo")
  process.DYTree.triggerObject_miniAOD = cms.untracked.InputTag("slimmedPatTrigger")
  process.DYTree.offlineMuon           = cms.untracked.InputTag("slimmedMuons")
  process.DYTree.offlineVertex         = cms.untracked.InputTag("offlineSlimmedPrimaryVertices")

if not isMC and not isMiniAOD: # -- if it is DATA and RAW tier, RAWtoDigi step is needed to retrieve L1 information
  process.p = cms.Path(process.gtStage2Digis + process.DYTree)
else: # -- else (MC or DATA with miniAOD format)
  process.p = cms.Path(process.DYTree)
