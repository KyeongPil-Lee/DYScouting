import os

def GetCWD():
  cwd_fullPath = os.getcwd()
  analyzerPath = os.environ["DY_ANALYZER_PATH"]
  cwd = cwd_fullPath.split(analyzerPath+"/")[1] # -- start from the analyzer base path

  return cwd


from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobType = "HTCondor"
generator.jobName = "ControlPlot" # -- used for workspace name
generator.classCodePath = GetCWD()+"/MakeHist_Dimuon_noWeight.h"
generator.className = "HistProducer"

generator.luminosity = 1.0 # -- not used for now
generator.jsonName = "SampleInfo_v1p2.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  "ScoutingCaloMuon_Run2018A": 20,
  "ScoutingCaloMuon_Run2018B": 20,
  "ScoutingCaloMuon_Run2018C": 20,
  "ScoutingCaloMuon_Run2018D": 20,
  }

# generator.outputPath = "/scratch/kplee/DYScoutingJob"
generator.outputPath = "/data9/Users/kplee/DYScoutingJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()
