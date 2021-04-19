import os

def GetCWD():
  cwd_fullPath = os.getcwd()
  analyzerPath = os.environ["DY_ANALYZER_PATH"]
  cwd = cwd_fullPath.split(analyzerPath+"/")[1] # -- start from the analyzer base path

  return cwd


from Include.CondorJobGenerator import *

generator = MultiCondorJobGenerator()

generator.jobName = "AccEff" # -- used for workspace name
generator.ROOTCodePath = GetCWD()+"/MakeHist_AccEff.h"
generator.className = "AccEffProducer"

generator.luminosity = 1.0 # -- not used for now
generator.jsonName = "SampleInfo_v1p5.json" # -- under DY_ANALYZER_PATH/Include

# -- if "_skim" is added, it runs over skimmed ntuples
generator.dic_nJob = { 
    "Offline_DYMuMu_M10to50":    3, 
    "Offline_DYMuMu_M50toInf":   10,
  }

generator.baseWSPath = "/data9/Users/kplee/DYScoutingJob"

generator.Generate()
