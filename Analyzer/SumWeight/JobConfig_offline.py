from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobType = "HTCondor"
generator.jobName = "sumWeight" # -- used for workspace name
generator.classCodePath = "SumWeight/MakeHist_SumWeight.h"
generator.className = "HistProducer"

generator.luminosity = 1.0 # -- not used anyway
generator.jsonName = "SampleInfo_v1p4_KNU.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  # "Offline_DYMuMu_M50toInf_UL18":   10,
  # "Offline_DYTauTau_M50toInf_UL18":  10,
  # "Offline_WJets_UL18": 5,
  # "Offline_ttbar_UL18": 5,
  # "Offline_tW_UL18":     3,
  # "Offline_tbarW_UL18":  3,
  # "Offline_WW_UL18":  3,
  # "Offline_WZ_UL18":  3,
  # "Offline_ZZ_UL18":  3,
  # "Offline_DYLL_M50toInf_UL18": 10,
  "Offline_DYEE_M50toInf_UL18": 10,
  }

# generator.outputPath = "/scratch/kplee/DYScoutingJob"
# generator.outputPath = "/data9/Users/kplee/DYScoutingJob"
generator.outputPath = "/u/user/kplee/scratch/DYScoutingJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()