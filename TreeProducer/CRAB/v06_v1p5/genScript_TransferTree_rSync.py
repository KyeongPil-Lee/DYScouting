from DYScouting.TreeProducer.TransferTree_rSync import *

tool = TransferTool()
# -- common settings
tool.scriptName = "script_transferDYTree_rSync_offline_v01.sh"
# tool.scriptName = "script_transferDYTree_v02.sh"
tool.destination = "kplee@147.47.242.67"
tool.portNumber = 1240

# -- individual settings
basePath = "/gv0/Users/kplee/DYScoutingTree/v1.5/offline"

version = "v1p5"
SEBasePath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYScoutingTree_%s" % version

def OutputDirName(crabDirName):
    return crabDirName.split("crab_")[-1]

# -- data
tool.SEPath = SEBasePath+"/SingleMuon"
tool.crabDir = "crab_SingleMuon_Run2018A"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/SingleMuon"
tool.crabDir = "crab_SingleMuon_Run2018B"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/SingleMuon"
tool.crabDir = "crab_SingleMuon_Run2018C"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = SEBasePath+"/SingleMuon"
tool.crabDir = "crab_SingleMuon_Run2018D"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

# -- MC
# tool.SEPath = SEBasePath+"/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8"
# tool.crabDir = "crab_DYJetsToLL_M10to50_Madgraph"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()

# tool.SEPath = SEBasePath+"/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8"
# tool.crabDir = "crab_DYJetsToLL_M10to50_Madgraph_ext"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()

# tool.SEPath = SEBasePath+"/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8"
# tool.crabDir = "crab_DYJetsToLL_M50toInf_aMCNLO"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()

# tool.SEPath = SEBasePath+"/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8"
# tool.crabDir = "crab_DYJetsToLL_M50toInf_aMCNLO_ext"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()


# tool.SEPath = SEBasePath+"/TTTo2L2Nu_Powheg"
# tool.crabDir = "crab_DYJetsToLL_M50toInf_aMCNLO_ext"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()


# tool.SEPath = SEBasePath+"/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"
# tool.crabDir = "crab_WJetsToLNu_Madgraph"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()


tool.GenScript()