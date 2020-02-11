import os, sys

class TransferTool:
    def __init__(self):

        self.scriptName = ""

        self.SEPath = "" # -- SE = storage element
        self.destination = "" # -- e.g. kplee@147.47.50.161. It should be able to connect without ssh password
        self.portNumber = 22

        # -- individual settings for each tree
        self.crabDir = ""
        self.outputDirPath = ""

        # -- internal variables
        self.rsyncCMDs = []

    def Register(self):
        self._MakeRSyncCMD()
        
        # -- initialization
        self.crabDir = ""
        self.outputDirPath = ""

    def _MakeRSyncCMD(self):

        basePath = "%s/%s" % (self.SEPath, self.crabDir)

        subDirs = os.listdir(basePath)
        print "subDirs: ", subDirs
        if len(subDirs) != 1:
            print "More than 1 sub directories ... please check"
            for subDir in subDirs:
                print "%s/%s" % (basePath, subDir)
            sys.exit()

        rsyncCMD = """
ssh -p {port_} {destination_} "mkdir {outputDirPath_}"

rsync -avz --progress -e 'ssh -p {port_}' \\
{SEPath_}/{crabDir_}/*/*/*.root \\
{destination_}:{outputDirPath_}

echo "[{crabDir_}] is transferred"

""".format(port_=self.portNumber, SEPath_=self.SEPath, crabDir_=self.crabDir, destination_=self.destination, outputDirPath_=self.outputDirPath)
    
        print "cmd for rsync: \n", rsyncCMD
        self.rsyncCMDs.append( rsyncCMD )

    def GenScript(self):

        if self.scriptName in os.listdir("."):
            print "%s already exists in same directory ... please change the name" % self.scriptName
            sys.exit()

        f = open(self.scriptName, "w")

        f.write("#!bin/bash\n\n")

        for iTree in range(0, len(self.rsyncCMDs)):
            rsyncCMD = self.rsyncCMDs[iTree]
            f.write( rsyncCMD )

        f.write( 'echo "All jobs are finished"\n')

        f.close()

        os.system( "cat %s" % self.scriptName )

        print "*" * 100
        print "*" * 100
        print "script is generated: %s" % self.scriptName
        print "source %s >&%s.log&" % (self.scriptName, self.scriptName.split('.sh')[0])
        print "*" * 100
        print "*" * 100
