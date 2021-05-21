import os

def GetCWD():
  cwd_fullPath = os.getcwd()
  analyzerPath = os.environ["DY_ANALYZER_PATH"]
  cwd = cwd_fullPath.split(analyzerPath+"/")[1] # -- start from the analyzer base path

  return cwd


from Include.CondorJobGenerator import *

generator = MultiCondorJobGenerator()

generator.jobName = "DataMCRatio" # -- used for workspace name
generator.ROOTCodePath = GetCWD()+"/MCDimuonMass.h"
generator.className = "HistProducer"

generator.luminosity = 1.0 # -- not used for now
generator.jsonName = "SampleInfo_v1p5.json" # -- under DY_ANALYZER_PATH/Include

# -- if "_skim" is added, it runs over skimmed ntuples
generator.dic_nJob = { 
  # "Offline_SingleMuon_Run2018All": 300,
  "Offline_DYMuMu_M10to50":    3, 
  "Offline_DYMuMu_M50toInf":   10,
  "Offline_DYTauTau_M10to50":  3, 
  "Offline_DYTauTau_M50toInf": 10, 
  "Offline_WJets": 2,
  "Offline_ttbar": 10,
  "Offline_QCDMuEnriched_Pt15to20":  2,
  "Offline_QCDMuEnriched_Pt20to30":  2,
  "Offline_QCDMuEnriched_Pt30to50":  2,
  "Offline_QCDMuEnriched_Pt50to80":  2,
  "Offline_QCDMuEnriched_Pt80to120": 2,
  "Offline_QCDMuEnriched_Pt120to170": 2,
  "Offline_QCDMuEnriched_Pt170to300": 2,
  "Offline_QCDMuEnriched_Pt300to470": 2,
  "Offline_QCDMuEnriched_Pt470to600": 2,
  "Offline_QCDMuEnriched_Pt600to800": 2,
  "Offline_QCDMuEnriched_Pt800to1000": 2,
  "Offline_QCDMuEnriched_Pt1000toInf": 2,
  }

generator.baseWSPath = "/data9/Users/kplee/DYScoutingJob"

generator.Generate()
