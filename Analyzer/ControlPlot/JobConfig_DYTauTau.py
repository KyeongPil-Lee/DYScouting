from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobName = "ControlPlot_DYTauTau" # -- used for workspace name
generator.classCodePath = "ControlPlot/MakeHist_Dimuon_noWeight_DYTauTau.h"
generator.className = "HistProducer"

generator.luminosity = 1.0 # -- not used for now
generator.jsonName = "SampleInfo_v1p0.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  "DYTauTau_M10to50":  8,
  "DYTauTau_M50toInf": 8,
  }

generator.outputPath = "/scratch/kplee/DYScoutingJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()