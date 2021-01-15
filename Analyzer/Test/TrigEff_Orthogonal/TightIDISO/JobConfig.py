import os

def GetCWD():
  cwd_fullPath = os.getcwd()
  analyzerPath = os.environ["DY_ANALYZER_PATH"]
  cwd = cwd_fullPath.split(analyzerPath+"/")[1] # -- start from the analyzer base path

  return cwd


from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobType = "HTCondor"
generator.jobName = "ZPeakHist_TrigEff" # -- used for workspace name
generator.classCodePath = GetCWD()+"/ZPeakHistProducer.h"
generator.className = "ZPeakHistProducer"

generator.luminosity = 59740.565202 # -- /pb
generator.jsonName = "SampleInfo_v1p4.json" # -- under DY_ANALYZER_PATH/Include
# generator.jsonName = "SampleInfo_v1p4_KNU.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
"Offline_EGamma_All_UL18": 100,
"Offline_DYMuMu_M50toInf_UL18":   10,
"Offline_DYEE_M50toInf_UL18": 10,
"Offline_DYTauTau_M50toInf_UL18":  10,
"Offline_WJets_UL18": 5,
"Offline_TTTo2L2Nu_UL18": 5,
"Offline_tW_UL18":     3,
"Offline_tbarW_UL18":  3,
"Offline_WW_UL18":  3,
"Offline_WZ_UL18":  3,
"Offline_ZZ_UL18":  3,
  }

# generator.outputPath = "/scratch/kplee/DYScoutingJob"
generator.outputPath = "/data9/Users/kplee/DYScoutingJob"
# generator.outputPath = "/u/user/kplee/scratch/DYScoutingJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()
