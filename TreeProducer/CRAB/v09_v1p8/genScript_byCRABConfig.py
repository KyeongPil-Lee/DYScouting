from DYScouting.TreeProducer.TransferTree_rSync import *

theDestination = "kplee@147.47.242.67"
thePortNumber = 1240
theDestinationBasePath = "/gv0/Users/kplee/DYScoutingTree/v1.7"
version = "v1p7"
theSEBasePath = "/pnfs/knu.ac.kr/data/cms/store/user/kplee/DYScoutingTree/%s" % version

list_crabConfig = []
list_crabConfig.append("crabcfg_2018MC_JPsi.py")

generator = ScriptGenerator()

# -- common settings
generator.list_crabConfig = list_crabConfig

generator.scriptName = "script_transferTree_JPsi.sh"
generator.destination = theDestination
generator.portNumber = thePortNumber

generator.destinationBasePath = theDestinationBasePath
generator.SEBasePath = theSEBasePath

generator.ignoreComment = True

generator.Generate()









