from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobType = "HTCondor"
generator.jobName = "MVAPlot" # -- used for workspace name
generator.classCodePath = "BDT/Trial/v04_AlmostNoCut/ROCCurve/MVAPlotProducer.h"
generator.className = "HistProducer"

generator.luminosity = 1.0 # -- not used for now
generator.jsonName = "SampleInfo_v1p2.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  "DYMuMu_M10to50":    20,
  "DYMuMu_M50toInf":   10,
  "QCDMuEnriched_Pt15to20":  3,
  "QCDMuEnriched_Pt20to30":  3,
  "QCDMuEnriched_Pt30to50":  3,
  "QCDMuEnriched_Pt50to80":  3,
  "QCDMuEnriched_Pt80to120": 3,
  "QCDMuEnriched_Pt120to170": 3,
  "QCDMuEnriched_Pt170to300": 3,
  "QCDMuEnriched_Pt300to470": 3,
  "QCDMuEnriched_Pt470to600": 3,
  "QCDMuEnriched_Pt600to800": 3,
  "QCDMuEnriched_Pt800to1000": 3,
  "QCDMuEnriched_Pt1000toInf": 3,
  }

# generator.outputPath = "/scratch/kplee/DYScoutingJob"
generator.outputPath = "/data9/Users/kplee/DYScoutingJob"
generator.bringOutput = True # -- bring output root file to the current working directory where JobConfig is executed

generator.Generate()
