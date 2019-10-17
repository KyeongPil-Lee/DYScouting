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
        self.scpCMDs = []

    def Register(self):
        self._MakeScpCMD()
        
        # -- initialization
        self.crabDir = ""
        self.outputDirPath = ""

    def _MakeScpCMD(self):
        # -- mkdir
        destination2 = self.destination
        if "-P" in destination2:
            destination2 = destination2.replace("-P", "-p")

        cmd = 'ssh -p %d %s "mkdir %s"\n\n' % (self.portNumber, self.destination, self.outputDirPath)

        rootFilePaths = []

        basePath = "%s/%s" % (self.SEPath, self.crabDir)

        subDirs = os.listdir(basePath)
        print "subDirs: ", subDirs
        if len(subDirs) != 1:
            print "More than 1 sub directories ... please check"
            for subDir in subDirs:
                print "%s/%s" % (basePath, subDir)
            sys.exit()

        extendedPath = "%s/%s" % (basePath, subDirs[0])

        subsubDirs = os.listdir(extendedPath)
        print "subsubDirs: ", subsubDirs

        for subsubDir in subsubDirs:
            rootFilePath = "%s/%s" % (extendedPath, subsubDir)
            rootFilePaths.append( rootFilePath+"/*.root" )

        print "rootFilePaths: ", rootFilePaths

        cmd = cmd + "scp -P %s \\\n" % (self.portNumber)
        for rootFilePath in rootFilePaths:
            cmd = cmd + "%s \\\n" % rootFilePath

        cmd = cmd + "%s:%s\n\n" % (self.destination, self.outputDirPath)

        cmd = cmd + 'echo "[%s] is trasnferred"\n' % self.crabDir

        print "cmd for scp: \n", cmd
        self.scpCMDs.append( cmd )

    def GenScript(self):

        if self.scriptName in os.listdir("."):
            print "%s already exists in same directory ... please change the name" % self.scriptName
            sys.exit()

        f = open(self.scriptName, "w")

        f.write("#!bin/bash\n\n")

        for iTree in range(0, len(self.scpCMDs)):
            scpCMD = self.scpCMDs[iTree]
            f.write( scpCMD  + "\n\n\n" )

        f.write( 'echo "All jobs are finished"\n')

        f.close()

        os.system( "cat %s" % self.scriptName )

        print "*" * 100
        print "*" * 100
        print "script is generated: %s" % self.scriptName
        print "source %s >&%s.log&" % (self.scriptName, self.scriptName.split('.sh')[0])
        print "Check list before running script"
        print "1) ROOT: available? (for hadd)"
        print "*" * 100
        print "*" * 100
