from DYScouting.TreeProducer.TrasnferTree import *

tool = TransferTool()
# -- common settings
# tool.scriptName = "script_transferDYTree_v01.sh"
tool.scriptName = "script_transferDYTree_v02.sh"
tool.destination = "kplee@147.47.242.67"
tool.portNumber = 1240

# -- individual settings
basePath = "/gv0/Users/kplee/DYScoutingTree/v1.1"

# # -- DY, M10-50
# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8"
# tool.crabDir = "crab_DYTree_v20191018_DYJetsToLL_M10to50_Madgraph"
# tool.outputDirPath = "%s/DYJetsToLL_M10to50_Madgraph" % basePath
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8"
# tool.crabDir = "crab_DYTree_v20191018_DYJetsToLL_M10to50_Madgraph_ext"
# tool.outputDirPath = "%s/DYJetsToLL_M10to50_Madgraph_ext" % basePath
# tool.Register()

# # -- DY, M50
# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8"
# tool.crabDir = "crab_DYTree_v20191018_DYJetsToLL_M50toInf_aMCNLO"
# tool.outputDirPath = "%s/DYJetsToLL_M50toInf_aMCNLO" % basePath
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8"
# tool.crabDir = "crab_DYTree_v20191018_DYJetsToLL_M50toInf_aMCNLO_ext"
# tool.outputDirPath = "%s/DYJetsToLL_M50toInf_aMCNLO_ext" % basePath
# tool.Register()

# # -- QCD, inclusive
# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-20toInf_MuEnrichedPt15_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt20toInf"
# tool.outputDirPath = "%s/QCDMuEnriched_Pt20toInf" % basePath
# tool.Register()


# # -- QCD, jet pT-binned
# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-15to20_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt15to20"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-20to30_MuEnrichedPt5_TuneCP5_13TeV_pythia8"

tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt20to30"
tool.outputDirPath = "%s/%s_Incompleted" % (basePath, tool.crabDir.split("_v20191018_")[-1])
tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-30to50_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt30to50"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-50to80_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt50to80"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-80to120_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt80to120"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-80to120_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt80to120_ext"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-120to170_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt120to170"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-120to170_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt120to170_ext"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-170to300_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt170to300"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt300to470"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt300to470_ext"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt470to600"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt470to600_ext"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-600to800_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt600to800"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-800to1000_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt800to1000"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-1000toInf_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_v20191018_QCDMuEnriched_Pt1000toInf"
# tool.outputDirPath = "%s/%s" % (basePath, tool.crabDir.split("_v20191018_")[-1])
# tool.Register()


# # -- W+jets
# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"
# tool.crabDir = "crab_DYTree_v20191018_WJetsToLNu_Madgraph"
# tool.outputDirPath = "%s/WJetsToLNu_Madgraph" % basePath
# tool.Register()

# # -- ttbar
# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"
# tool.crabDir = "crab_DYTree_v20191018_TTTo2L2Nu_Powheg"
# tool.outputDirPath = "%s/TTTo2L2Nu_Powheg" % basePath
# tool.Register()

# -- scouting muon data
tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/ScoutingCaloMuon"
tool.crabDir = "crab_DYTree_v20191018_ScoutingCaloMuon_Run2018Av1_GoldenJSON"
tool.outputDirPath = "%s/ScoutingCaloMuon_Run2018Av1_GoldenJSON_Incompleted" % basePath # -- failed job: 1132
tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/ScoutingCaloMuon"
# tool.crabDir = "crab_DYTree_v20191018_ScoutingCaloMuon_Run2018Bv1_GoldenJSON"
# tool.outputDirPath = "%s/ScoutingCaloMuon_Run2018Bv1_GoldenJSON" % basePath
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/ScoutingCaloMuon"
# tool.crabDir = "crab_DYTree_v20191018_ScoutingCaloMuon_Run2018Cv1_GoldenJSON"
# tool.outputDirPath = "%s/ScoutingCaloMuon_Run2018Cv1_GoldenJSON" % basePath
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/ScoutingCaloMuon"
# tool.crabDir = "crab_DYTree_v20191018_ScoutingCaloMuon_Run2018Dv1_GoldenJSON"
# tool.outputDirPath = "%s/ScoutingCaloMuon_Run2018Dv1_GoldenJSON" % basePath
# tool.Register()



tool.GenScript()