from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobName = "ControlPlot" # -- used for workspace name
generator.classCodePath = "ControlPlot/MakeHist_Dimuon_noWeight.h"
generator.className = "HistProducer"

generator.luminosity = 1.0 # -- not used for now
generator.jsonName = "SampleInfo_v1p0.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  "DYMuMu_M10to50":  4,
  "DYMuMu_M50toInf": 4,
  "QCDMuEnriched_Pt20toInf": 4,
  "WJets": 4,
  "ttbar": 4,
  }

generator.outputPath = "/scratch/kplee/DYScoutingJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()