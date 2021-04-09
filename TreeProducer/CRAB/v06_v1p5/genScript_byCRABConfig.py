from DYScouting.TreeProducer.TransferTree_rSync import *

theDestination = "kplee@147.47.242.67"
thePortNumber = 1240
theDestinationBasePath = "/gv0/Users/kplee/DYScoutingTree/v1.5/offline"
version = "v1p5"
theSEBasePath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYScoutingTree_%s" % version

list_crabConfig = []
list_crabConfig.append("crabConfig_miniAOD_MC.py")
list_crabConfig.append("crabConfig_miniAOD_DATA.py")

generator = ScriptGenerator()

# -- common settings
generator.list_crabConfig = list_crabConfig

generator.scriptName = "script_transferTree.sh"
generator.destination = theDestination
generator.portNumber = thePortNumber

generator.destinationBasePath = theDestinationBasePath
generator.SEBasePath = theSEBasePath

generator.Generate()









