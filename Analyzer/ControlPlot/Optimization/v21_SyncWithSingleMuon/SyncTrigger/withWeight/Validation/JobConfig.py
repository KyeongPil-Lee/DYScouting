import os

def GetCWD():
  cwd_fullPath = os.getcwd()
  analyzerPath = os.environ["DY_ANALYZER_PATH"]
  cwd = cwd_fullPath.split(analyzerPath+"/")[1] # -- start from the analyzer base path

  return cwd


from Include.CondorJobGenerator import *

generator = MultiCondorJobGenerator()

generator.jobName = "ControlPlot" # -- used for workspace name
generator.ROOTCodePath = GetCWD()+"/MakeHist_Dimuon.h"
generator.className = "HistProducer"

generator.luminosity = 59740.565202
generator.jsonName = "SampleInfo_v1p5.json" # -- under DY_ANALYZER_PATH/Include

# -- if "_skim" is added, it runs over skimmed ntuples
generator.dic_nJob = { 
    "ScoutingCaloMuon_Run2018All_skim": 300,
    "DYMuMu_M10to50_skim":    3, 
    "DYMuMu_M50toInf_skim":   10,
    "DYTauTau_M10to50_skim":  3, 
    "DYTauTau_M50toInf_skim": 10, 
    "WJets_skim": 2,
    "ttbar_skim": 10,
    "QCDMuEnriched_Pt15to20_skim":  2,
    "QCDMuEnriched_Pt20to30_skim":  2,
    "QCDMuEnriched_Pt30to50_skim":  2,
    "QCDMuEnriched_Pt50to80_skim":  2,
    "QCDMuEnriched_Pt80to120_skim": 2,
    "QCDMuEnriched_Pt120to170_skim": 2,
    "QCDMuEnriched_Pt170to300_skim": 2,
    "QCDMuEnriched_Pt300to470_skim": 2,
    "QCDMuEnriched_Pt470to600_skim": 2,
    "QCDMuEnriched_Pt600to800_skim": 2,
    "QCDMuEnriched_Pt800to1000_skim": 2,
    "QCDMuEnriched_Pt1000toInf_skim": 2,
  }

generator.baseWSPath = "/data9/Users/kplee/DYScoutingJob"

generator.Generate()
