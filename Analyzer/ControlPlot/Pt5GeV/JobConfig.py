from Include.MultiJobGenerator import *

generator = MultiJobGenerator()

generator.jobType = "HTCondor"
generator.jobName = "ControlPlotPt5GeV" # -- used for workspace name
generator.classCodePath = "ControlPlot/Pt5GeV/MakeHist_Dimuon_noWeight.h"
generator.className = "HistProducer"

generator.luminosity = 1.0 # -- not used for now
generator.jsonName = "SampleInfo_v1p1.json" # -- under DY_ANALYZER_PATH/Include

generator.dic_nJob = { 
  "ScoutingCaloMuon_Run2018All": 50,
  "DYMuMu_M10to50":    5,
  "DYMuMu_M50toInf":   10,
  "DYTauTau_M10to50":  5,
  "DYTauTau_M50toInf": 10,
  "WJets": 5,
  "ttbar": 5,
  "QCDMuEnriched_Pt20toInf": 3,
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