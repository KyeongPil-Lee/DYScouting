from DYScouting.TreeProducer.TransferTree_rSync import *

tool = TransferTool()
# -- common settings
tool.scriptName = "script_transferDYTree_rSync_offline_v01.sh"
# tool.scriptName = "script_transferDYTree_v02.sh"
tool.destination = "kplee@147.47.242.67"
tool.portNumber = 1240

# -- individual settings
basePath = "/gv0/Users/kplee/DYScoutingTree/v1.4/offline"

version = "v1p4"
SEBasePath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYScoutingTree_%s" % version

def OutputDirName(crabDirName):
    return crabDirName.split("crab_")[-1]

tool.SEPath = SEBasePath+"/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8"
tool.crabDir = "crab_DYLL_M50_aMCNLO_UL18"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/EGamma"
tool.crabDir = "crab_EGamma_Run2018A_UL18"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/EGamma"
tool.crabDir = "crab_EGamma_Run2018B_UL18"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/EGamma"
tool.crabDir = "crab_EGamma_Run2018C_UL18"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/EGamma"
tool.crabDir = "crab_EGamma_Run2018D_UL18"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8"
tool.crabDir = "crab_ST_tW_antitop_Powheg_UL18"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8"
tool.crabDir = "crab_ST_tW_top_Powheg_UL18"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"
tool.crabDir = "crab_TTTo2L2Nu_Powheg_UL18"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"
tool.crabDir = "crab_WJetsToLNu_MadgraphMLM_UL18"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/WW_TuneCP5_13TeV-pythia8"
tool.crabDir = "crab_WW_Pythia8_UL18"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/WZ_TuneCP5_13TeV-pythia8"
tool.crabDir = "crab_WZ_Pythia8_UL18"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/ZZ_TuneCP5_13TeV-pythia8"
tool.crabDir = "crab_ZZ_Pythia8_UL18"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()


tool.GenScript()