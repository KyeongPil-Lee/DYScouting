import os

def GetCWD():
  cwd_fullPath = os.getcwd()
  analyzerPath = os.environ["DY_ANALYZER_PATH"]
  cwd = cwd_fullPath.split(analyzerPath+"/")[1] # -- start from the analyzer base path

  return cwd


from Include.CondorJobGenerator import *

generator = MultiCondorJobGenerator()

generator.jobName = "ControlPlot" # -- used for workspace name
generator.ROOTCodePath = GetCWD()+"/MakeHist_Dimuon_noWeight.h"
generator.className = "HistProducer"

generator.luminosity = 1.0 # -- not used for now
generator.jsonName = "SampleInfo_v1p5.json" # -- under DY_ANALYZER_PATH/Include

# -- if "_skim" is added, it runs over skimmed ntuples
generator.dic_nJob = { 
  "ScoutingCaloMuon_Run2018All": 300,
  "DYMuMu_M10to50":    3, 
  "DYMuMu_M50toInf":   10,
  "DYTauTau_M10to50":  3, 
  "DYTauTau_M50toInf": 10, 
  "WJets": 2,
  "ttbar": 10,
  "QCDMuEnriched_Pt15to20":  2,
  "QCDMuEnriched_Pt20to30":  2,
  "QCDMuEnriched_Pt30to50":  2,
  "QCDMuEnriched_Pt50to80":  2,
  "QCDMuEnriched_Pt80to120": 2,
  "QCDMuEnriched_Pt120to170": 2,
  "QCDMuEnriched_Pt170to300": 2,
  "QCDMuEnriched_Pt300to470": 2,
  "QCDMuEnriched_Pt470to600": 2,
  "QCDMuEnriched_Pt600to800": 2,
  "QCDMuEnriched_Pt800to1000": 2,
  "QCDMuEnriched_Pt1000toInf": 2,
  }

generator.baseWSPath = "/data9/Users/kplee/DYScoutingJob"

generator.Generate()
