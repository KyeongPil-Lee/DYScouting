import os

def GetCWD():
  cwd_fullPath = os.getcwd()
  analyzerPath = os.environ["DY_ANALYZER_PATH"]
  cwd = cwd_fullPath.split(analyzerPath+"/")[1] # -- start from the analyzer base path

  return cwd


from Include.CondorJobGenerator import *

generator = MultiCondorJobGenerator()

generator.jobName = "TreeSkim" # -- used for workspace name
generator.ROOTCodePath = GetCWD()+"/SingleTreeSkimmer.h"
generator.className = "SingleTreeSkimmer"

generator.luminosity = 1.0 # -- not used for now
generator.jsonName = "SampleInfo_v1p2.json" # -- under DY_ANALYZER_PATH/Include

# --jobs: set it as a large number so that all jobs has only 1 root file per job
# -- as the ROOT code works only for a single input root file
generator.dic_nJob = { 
  # "ScoutingCaloMuon_Run2018All": 1e10,
  "DYMuMu_M10to50":    1e10,
  # "DYMuMu_M50toInf":   1e10,
  # "DYTauTau_M10to50":  1e10,
  # "DYTauTau_M50toInf": 1e10,
  # "WJets": 1e10,
  # "ttbar": 1e10,
  # "QCDMuEnriched_Pt20toInf": 1e10,
  # "QCDMuEnriched_Pt15to20":  1e10,
  # "QCDMuEnriched_Pt20to30":  1e10,
  # "QCDMuEnriched_Pt30to50":  1e10,
  # "QCDMuEnriched_Pt50to80":  1e10,
  # "QCDMuEnriched_Pt80to120": 1e10,
  # "QCDMuEnriched_Pt120to170": 1e10,
  # "QCDMuEnriched_Pt170to300": 1e10,
  # "QCDMuEnriched_Pt300to470": 1e10,
  # "QCDMuEnriched_Pt470to600": 1e10,
  # "QCDMuEnriched_Pt600to800": 1e10,
  # "QCDMuEnriched_Pt800to1000": 1e10,
  # "QCDMuEnriched_Pt1000toInf": 1e10,
  }

generator.baseWSPath = "/data9/Users/kplee/DYScoutingJob"

generator.Generate()
