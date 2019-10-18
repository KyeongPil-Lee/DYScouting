from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobName = "sumWeight" # -- used for workspace name
generator.classCodePath = "SumWeight/MakeHist_SumWeight_DYTauTau.h"
generator.className = "HistProducer"

generator.luminosity = 1.0 # -- not used anyway
generator.jsonName = "SampleInfo_v1p0.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  "DYMuMu_M50toInf":    8,
  }

generator.outputPath = "/scratch/kplee/DYScoutingJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()