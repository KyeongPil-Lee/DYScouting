from DYScouting.TreeProducer.TrasnferTree import *

tool = TransferTool()
# -- common settings
# tool.scriptName = "script_transferDYTree_v01.sh"
tool.scriptName = "script_transferDYTree_v02.sh"
tool.destination = "kplee@147.47.50.161"
tool.portNumber = 50001

# -- individual settings
basePath = "/scratch/kplee/DYScoutingTree/v1.0"

# # -- test
# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8"

# tool.crabDir = "crab_DYTree_v20191016_DYJetsToLL_M10to50_aMCNLO"
# tool.outputDirPath = "%s/DYJetsToLL_M50toInf_aMCNLO" % basePath
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-1000toInf_MuEnrichedPt5_TuneCP5_13TeV_pythia8"

# tool.crabDir = "crab_DYTree_v20191016_QCDMuEnriched_Pt1000toInf"
# tool.outputDirPath = "%s/QCDMuEnriched_Pt1000toInf" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-20toInf_MuEnrichedPt15_TuneCP5_13TeV_pythia8"

# tool.crabDir = "crab_DYTree_v20191016_QCDMuEnriched_Pt20toInf"
# tool.outputDirPath = "%s/QCDMuEnriched_Pt20toInf" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8"

# tool.crabDir = "crab_DYTree_v20191016_QCDMuEnriched_Pt300to470"
# tool.outputDirPath = "%s/QCDMuEnriched_Pt300to470" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8"

# tool.crabDir = "crab_DYTree_v20191016_QCDMuEnriched_Pt300to470_ext"
# tool.outputDirPath = "%s/QCDMuEnriched_Pt300to470_ext" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-30to50_MuEnrichedPt5_TuneCP5_13TeV_pythia8"

# tool.crabDir = "crab_DYTree_v20191016_QCDMuEnriched_Pt30to50"
# tool.outputDirPath = "%s/QCDMuEnriched_Pt30to50" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8"

# tool.crabDir = "crab_DYTree_v20191016_QCDMuEnriched_Pt470to600"
# tool.outputDirPath = "%s/QCDMuEnriched_Pt470to600" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8"

# tool.crabDir = "crab_DYTree_v20191016_QCDMuEnriched_Pt470to600_ext"
# tool.outputDirPath = "%s/QCDMuEnriched_Pt470to600" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-80to120_MuEnrichedPt5_TuneCP5_13TeV_pythia8"

# tool.crabDir = "crab_DYTree_v20191016_QCDMuEnriched_Pt80to120"
# tool.outputDirPath = "%s/QCDMuEnriched_Pt80to120" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-80to120_MuEnrichedPt5_TuneCP5_13TeV_pythia8"

# tool.crabDir = "crab_DYTree_v20191016_QCDMuEnriched_Pt80to120_ext"
# tool.outputDirPath = "%s/QCDMuEnriched_Pt80to120" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"

# tool.crabDir = "crab_DYTree_v20191016_WJetsToLNu_Madgraph"
# tool.outputDirPath = "%s/WJetsToLNu_Madgraph" % basePath
# tool.Register()

#############
# -- v02 -- #
#############
# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8"

# tool.crabDir = "crab_DYTree_v20191016_DYJetsToLL_M50toInf_aMCNLO_ext"
# tool.outputDirPath = "%s/DYJetsToLL_M50toInf_aMCNLO_ext" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-50to80_MuEnrichedPt5_TuneCP5_13TeV_pythia8"

# tool.crabDir = "crab_DYTree_v20191016_QCDMuEnriched_Pt50to80"
# tool.outputDirPath = "%s/QCDMuEnriched_Pt50to80" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-800to1000_MuEnrichedPt5_TuneCP5_13TeV_pythia8"

# tool.crabDir = "crab_DYTree_v20191016_QCDMuEnriched_Pt800to1000"
# tool.outputDirPath = "%s/QCDMuEnriched_Pt800to1000" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/ScoutingCaloMuon"

# tool.crabDir = "crab_DYTree_v20191016_ScoutingCaloMuon_Run2018Av1_GoldenJSON"
# tool.outputDirPath = "%s/ScoutingCaloMuon_Run2018Av1_GoldenJSON_Incompleted" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/ScoutingCaloMuon"

# tool.crabDir = "crab_DYTree_v20191016_ScoutingCaloMuon_Run2018Bv1_GoldenJSON"
# tool.outputDirPath = "%s/ScoutingCaloMuon_Run2018Bv1_GoldenJSON_Incompleted" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/ScoutingCaloMuon"

# tool.crabDir = "crab_DYTree_v20191016_ScoutingCaloMuon_Run2018Cv1_GoldenJSON"
# tool.outputDirPath = "%s/ScoutingCaloMuon_Run2018Cv1_GoldenJSON_Incompleted" % basePath
# tool.Register()


# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"

# tool.crabDir = "crab_DYTree_v20191016_TTTo2L2Nu_Powheg"
# tool.outputDirPath = "%s/TTTo2L2Nu_Powheg_Incompleted" % basePath
# tool.Register()



tool.GenScript()