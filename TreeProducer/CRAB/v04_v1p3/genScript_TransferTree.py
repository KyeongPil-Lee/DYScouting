from DYScouting.TreeProducer.TrasnferTree import *

tool = TransferTool()
# -- common settings
tool.scriptName = "script_transferDYTree_v01.sh"
# tool.scriptName = "script_transferDYTree_v02.sh"
tool.destination = "kplee@147.47.242.67"
tool.portNumber = 1240

# -- individual settings
basePath = "/gv0/Users/kplee/DYScoutingTree/v1.3"
version = "v20200204"

def OutputDirName(crabDirName):
    return crabDirName.split("_"+version+"_")[-1]

# # -- DY, M10-50
version = "v20200210"
tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8"
tool.crabDir = "crab_DYTree_"+version+"_DYJetsToLL_M10to50_Madgraph"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()
version = "v20200204"

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8"
# tool.crabDir = "crab_DYTree_"+version+"_DYJetsToLL_M10to50_Madgraph_ext"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()

# # -- DY, M50
tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8"
tool.crabDir = "crab_DYTree_"+version+"_DYJetsToLL_M50toInf_aMCNLO"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8"
# tool.crabDir = "crab_DYTree_"+version+"_DYJetsToLL_M50toInf_aMCNLO_ext"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()

# # -- QCD, inclusive
tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-20toInf_MuEnrichedPt15_TuneCP5_13TeV_pythia8"
tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt20toInf"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()


# # -- QCD, jet pT-binned
tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-15to20_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt15to20"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-20to30_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt20to30"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()

tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-30to50_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt30to50"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-50to80_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt50to80"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-80to120_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt80to120"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-80to120_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt80to120_ext"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-120to170_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt120to170"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-120to170_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt120to170_ext"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-170to300_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt170to300"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()

tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt300to470"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt300to470_ext"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()

tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt470to600"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
# tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt470to600_ext"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()

tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-600to800_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt600to800"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-800to1000_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt800to1000"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/QCD_Pt-1000toInf_MuEnrichedPt5_TuneCP5_13TeV_pythia8"
tool.crabDir = "crab_DYTree_"+version+"_QCDMuEnriched_Pt1000toInf"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()


# # -- W+jets
version = "v20200210"
tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"
tool.crabDir = "crab_DYTree_"+version+"_WJetsToLNu_Madgraph"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()
version = "v20200204"

# # -- ttbar
tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"
tool.crabDir = "crab_DYTree_"+version+"_TTTo2L2Nu_Powheg"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()

# # -- scouting muon data
# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/ScoutingCaloMuon"
# tool.crabDir = "crab_DYTree_"+version+"_ScoutingCaloMuon_Run2018Av1_GoldenJSON"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/ScoutingCaloMuon"
# tool.crabDir = "crab_DYTree_"+version+"_ScoutingCaloMuon_Run2018Bv1_GoldenJSON"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()

# tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/ScoutingCaloMuon"
# tool.crabDir = "crab_DYTree_"+version+"_ScoutingCaloMuon_Run2018Cv1_GoldenJSON"
# tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
# tool.Register()

tool.SEPath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/ScoutingCaloMuon"
tool.crabDir = "crab_DYTree_"+version+"_ScoutingCaloMuon_Run2018Dv1_GoldenJSON"
tool.outputDirPath = "%s/%s" % (basePath, OutputDirName(tool.crabDir))
tool.Register()



tool.GenScript()