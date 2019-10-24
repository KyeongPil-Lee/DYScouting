from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobType = "HTCondor"
generator.jobName = "sumWeight" # -- used for workspace name
generator.classCodePath = "SumWeight/MakeHist_SumWeight.h"
generator.className = "HistProducer"

generator.luminosity = 1.0 # -- not used anyway
generator.jsonName = "SampleInfo_v1p1.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  "WJets":    5,
  }

generator.outputPath = "/scratch/kplee/DYScoutingJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()