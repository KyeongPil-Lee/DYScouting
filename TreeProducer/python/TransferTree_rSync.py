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

class ScriptGenerator:
    def __init__(self):

        self.list_crabConfig = "" # -- input crab configurations to read
        self.destinationBasePath = ""
        self.SEBasePath = ""

        # -- TransferTool variables
        self.scriptName = ""
        self.destination = "" # -- e.g. kplee@147.47.50.161. It should be able to connect without ssh password
        self.portNumber = 22

        # -- internal variables
        self.tool = TransferTool()

        self.dic_crab_dataset = {} # -- key: crab directory name, value: dataset name (used to find the corresponding directory in SE)


    def Generate(self):
        print "list_crabConfig = ", self.list_crabConfig

        print "scriptName = %s" % self.scriptName
        print "destination = %s" % self.destination
        print "portNumber = %s\n" % self.portNumber

        print "SEBasePath = %s" % self.SEBasePath
        print "destinationBasePath = %s" % self.destinationBasePath

        self.tool.scriptName = self.scriptName
        self.tool.destination = self.destination
        self.tool.portNumber = self.portNumber

        for crabConfig in self.list_crabConfig:
            self.GetDatasetNamePerCRABJob(crabConfig)

        for crabJobName in self.dic_crab_dataset.keys():
            dataset = self.dic_crab_dataset[crabJobName]

            self.tool.SEPath        = "%s/%s" % (self.SEBasePath, dataset)
            self.tool.crabDir       = "crab_%s" % crabJobName
            self.tool.outputDirPath = "%s/%s" % (self.destinationBasePath, crabJobName)
            self.tool.Register()

            print "crabJobName   = %s" % self.tool.crabDir
            print "SEPath        = %s" % self.tool.SEPath
            print "outputDirPath = %s" % self.tool.outputDirPath
            print "\n"

        self.tool.GenScript()



    def GetDatasetNamePerCRABJob(self, crabConfig):
        list_crabCommand = []

        f_crabConfig = open(crabConfig)
        list_line = f_crabConfig.readlines()

        list_i_commandLine = []
        i_APIStartLine = 0
        for i in range(0, len(list_line)):
            line = list_line[i]

            if "from CRABAPI.RawCommand" in line:
                i_APIStartLine = i

            if "crabCommand" in line and "submit" in line:
                list_i_commandLine.append(i)


        list_command = []
        for i in range(0, len(list_i_commandLine)):
            i_startline = 0
            i_endline = 0

            if i == 0:
                i_startLine = i_APIStartLine
                i_endLine = list_i_commandLine[i]
            else:
                i_startLine = list_i_commandLine[i-1]
                i_endLine = list_i_commandLine[i]

            string_temp = ""
            for j in range(0, len(list_line)):
                if i_startLine < j and j < i_endLine:
                    string_temp = string_temp + list_line[j]

            list_command.append(string_temp)

        for command in list_command:
            list_commandLine = command.split("\n")

            crabJobName = ""
            dataset = ""
            for commandLine in list_commandLine:
                if "config.General.requestName" in commandLine:
                    crabJobName = commandLine.split("=")[-1].strip().strip('"').strip("'")
                    # print crabJobName
                elif "config.Data.inputDataset" in commandLine:
                    dataset = commandLine.split("/")[1]
                    # print dataset

        self.dic_crab_dataset[crabJobName] = dataset













