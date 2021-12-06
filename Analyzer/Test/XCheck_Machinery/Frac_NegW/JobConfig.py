import os

def GetCWD():
  cwd_fullPath = os.getcwd()
  analyzerPath = os.environ["DY_ANALYZER_PATH"]
  cwd = cwd_fullPath.split(analyzerPath+"/")[1] # -- start from the analyzer base path

  return cwd


from Include.CondorJobGenerator import *

generator = MultiCondorJobGenerator()

generator.jobName = "ControlPlot" # -- used for workspace name
generator.ROOTCodePath = GetCWD()+"/HistProducer_GEN.h"
generator.className = "HistProducer"

generator.luminosity = 1.0 # -- not used for now
generator.jsonName = "SampleInfo_v1p5.json" # -- under DY_ANALYZER_PATH/Include

# -- if "_skim" is added, it runs over skimmed ntuples
generator.dic_nJob = { 
  "Offline_DYMuMu_M50toInf": 30,
  }

generator.baseWSPath = "/data9/Users/kplee/DYScoutingJob"

generator.Generate()
