import os

def GetCWD():
  cwd_fullPath = os.getcwd()
  analyzerPath = os.environ["DY_ANALYZER_PATH"]
  cwd = cwd_fullPath.split(analyzerPath+"/")[1] # -- start from the analyzer base path

  return cwd


from Include.CondorJobGenerator import *

generator = MultiCondorJobGenerator()

generator.jobName = "EffCorr" # -- used for workspace name
generator.ROOTCodePath = GetCWD()+"/MakeHist_MCTruthLegEff.h"
generator.className = "HistProducer"

generator.luminosity = 59740.565202 # -- 2018
generator.jsonName = "SampleInfo_v1p7.json" # -- under DY_ANALYZER_PATH/Include

# -- if "_skim" is added, it runs over skimmed ntuples
generator.dic_nJob = { 
  "DYMuMu_M10to50":    50, 
  "DYMuMu_M50toInf":   100,
  }

generator.baseWSPath = "/data9/Users/kplee/DYScoutingJob"

generator.Generate()
