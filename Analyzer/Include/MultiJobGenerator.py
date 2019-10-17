import os
import sys
import json

class MultiJobGenerator:
    def __init__(self):
        self.jobName = ""
        self.classCodePath = ""
        self.className = ""
        self.jsonName = ""
        self.luminosity = -1
        self.outputPath = ""
        self.dic_nJob = {} # -- nJob for each sample type

        # -- if it is true, then the output root file after hadd is copied to the current directory (where job configuration is executed)
        self.bringOutput = False

        # -- internal variable
        self.WSPath = ""

    def Generate(self):
        self.CheckOptions()
        self.PrintOptions()

        self.CreateWorkspace()

        for sampleType in self.dic_nJob.keys():
            jobGenerator = JobGenerator()
            jobGenerator.classCodePath = self.classCodePath
            jobGenerator.className = self.className
            jobGenerator.jsonName = self.jsonName
            jobGenerator.luminosity = self.luminosity

            # -- under main workspace
            jobGenerator.outputPath = self.WSPath

            jobGenerator.sampleType = sampleType
            jobGenerator.nJob = self.dic_nJob[sampleType]
            jobGenerator.bringOutput = self.bringOutput

            jobGenerator.Generate()

        self.GenerateFullScript()
        self.GenerateFullHADDScript()

    def CreateWorkspace(self):
        i = 1
        while "%s_v%02d" % (self.jobName, i) in os.listdir(self.outputPath):
            i = i+1

        WSName = "%s_v%02d" % (self.jobName, i)
        self.WSPath = "%s/%s" % (self.outputPath, WSName)
        os.mkdir(self.WSPath)

    # -- to submit all jobs from each sample
    def GenerateFullScript(self):
        scriptPath = "%s/submit_allSamples.sh" % self.WSPath
        f_script = open(scriptPath, "w")
        f_script.write("#!/bin/bash\n")

        for sampleType in self.dic_nJob.keys():
            cmd = "source %s/%s/submit_all.sh" % (self.WSPath, sampleType)
            f_script.write(cmd+"\n")

        f_script.close()

        print "[MultiJobGenerator] Submit all jobs in backgrounds"
        print "source %s\n" % scriptPath

    def GenerateFullHADDScript(self):
        scriptPath = "%s/hadd_allSamples.sh" % self.WSPath
        f_script = open(scriptPath, "w")
        f_script.write("#!/bin/bash\n")

        for sampleType in self.dic_nJob.keys():
            cmd = "source %s/%s/hadd_all.sh" % (self.WSPath, sampleType)
            f_script.write(cmd+"\n")

        f_script.close()

        print "[MultiJobGenerator] Do hadd for all samples"
        print "source %s\n" % scriptPath


    def CheckOptions(self):
        if self.jobName == "":
            print "jobName is not set"
            sys.exit()

        if self.classCodePath == "":
            print "classCodePath is not set"
            sys.exit()

        if self.className == "":
            print "className is not set"
            sys.exit()

        if self.jsonName == "":
            print "jsonName is not set"
            sys.exit()

        if self.luminosity == "":
            print "luminosity is not set"
            sys.exit()

        if self.outputPath == "":
            print "outputPath is not set"
            sys.exit()

        if len(self.dic_nJob) == 0:
            print "dic_nJob is not set"
            sys.exit()

    def PrintOptions(self):
        print "[MultiJobGenerator::Options]"
        print "  jobName:       ", self.jobName
        print "  classCodePath: ", self.classCodePath
        print "  className:     ", self.className
        print "  jsonName:      ", self.jsonName
        print "  luminosity:    ", self.luminosity
        print "  outputPath:    ", self.outputPath
        print "  dic_nJob"
        for sampleType in self.dic_nJob.keys():
            print "    %s -> nJob = %d" % (sampleType, self.dic_nJob[sampleType])

        print ""

class JobGenerator:
    def __init__(self):

        # -- mandatory variables
        self.classCodePath = ""
        self.className = ""
        self.jsonName = ""
        self.sampleType = ""
        self.luminosity = -1
        self.nJob = -1
        self.outputPath = ""

        # -- if it is true, then the output root file after hadd is copied to the current directory (where job configuration is executed)
        self.bringOutput = False

        # -- internal variables
        self.ntuplePathBase = os.getenv("DY_NTUPLE_PATH")

        self.isMC = False
        self.xSec = -1
        self.sumWeight = -1
        self.list_file = []

        self.nFilePerJob = -1

        self.WSPath = ""

        self.f_fullRunScript = ""

    def Generate(self):
        self.CheckOptions()
        self.PrintOptions()
        self.FillSampleInfo()

        self.CreateWorkspace()

    # -- fill isMC, xSec, sumWeight and list_file
    def FillSampleInfo(self):
        analyzerPath = os.getenv("DY_ANALYZER_PATH")
        jsonPath = "%s/Include/%s" % (analyzerPath, self.jsonName)
        with open(jsonPath) as fullSampleInfo_file:
            fullSampleInfo = json.load(fullSampleInfo_file)

        isFound = False
        for sample in fullSampleInfo["Sample"]:
            if sample["tag"] == self.sampleType:
                isFound = True
                print "Comment on sample: ", sample["comment"]

                self.isMC = sample["isMC"]
                self.xSec = sample["xSec"]
                self.sumWeight = sample["sumWeight"]

                for ntupleDir in sample["list_dir"]:
                    ntupleDirPath = "%s/%s" % (self.ntuplePathBase, ntupleDir)
                    list_file = os.listdir(ntupleDirPath)
                    for fileName in list_file:
                        if ".root" in fileName:
                            ntuplePath = "%s/%s" % (ntupleDirPath, fileName)
                            self.list_file.append( ntuplePath )

                break

        self.list_file.sort()

        if not isFound:
            print "No corresponding information to %s in %s" % (self.sampleType, self.jsonName)
            sys.exit()

        if len(self.list_file) == 0:
            print "No available root file in the directory"
            sys.exit()

    def CreateWorkspace(self):
        self.WSPath = "%s/%s" % (self.outputPath, self.sampleType)
        os.mkdir(self.WSPath)

        # -- script with all execution commands for sub jobs
        qsubScriptName = "submit_all.sh"
        qsubScriptPath = "%s/%s" % (self.WSPath, qsubScriptName)
        self.f_fullRunScript = open(qsubScriptPath, "w")
        self.f_fullRunScript.write("#!bin/bash\n")

        self.CreateSubWorkSpaces()

        self.f_fullRunScript.write('echo "All jobs for %s are submitted"\n' % self.sampleType)
        self.f_fullRunScript.close()

        self.GenerateHADDScript()


    def CreateSubWorkSpaces(self):
        nFile = len(self.list_file)
        if self.nJob > nFile:
            print "nJob > nFile -> nJob is set as nFile"
            self.nJob = nFile

        self.nFilePerJob = int( float(nFile) / float(self.nJob) )
        print "nJob = %d, nFile = %d -> nFilePerJob = %d\n" % (self.nJob, nFile, self.nFilePerJob)

        for i in range(0, self.nJob):
            # -- make sub workspace directory
            dirName = "Job_v%03d" % i
            subWSPath = "%s/%s" % (self.WSPath, dirName)
            os.mkdir( subWSPath )

            # -- make .txt file with ntuple paths
            ntupleListName = self.GenerateNtupleListPerJob( i, subWSPath )

            # -- make .cxx file to run
            self.GenerateROOTCode(ntupleListName, subWSPath)

            # -- make shell script to run .cxx
            self.CreateScriptPerJob( subWSPath )
            self.WriteCommandInFullScript( i, subWSPath )

    def GenerateNtupleListPerJob(self, i, path):
        list_filePerJob = []
        if i == self.nJob-1:
            list_filePerJob = self.list_file[int(i*self.nFilePerJob):]
        else:
            list_filePerJob = self.list_file[int(i*self.nFilePerJob):int((i+1)*self.nFilePerJob)]

        fileListName = "NtupleList_v%03d.txt" % i
        fileListPath = "%s/%s" % (path, fileListName)
        f_fileList = open(fileListPath, "w")
        for fileName in list_filePerJob:
            f_fileList.write( fileName + "\n" )
        f_fileList.close()

        return fileListName

    def CreateScriptPerJob( self, path ):

        cmd = "root -l -b -q Run.cxx++ >&Run.log"

        scriptName = "Run.sh"
        scriptPath = "%s/%s" % (path, scriptName)
        f_script = open(scriptPath, "w")
        f_script.write(
"""#!/bin/bash

export SCRAM_ARCH=slc6_amd64_gcc630
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh

# -- CMSSW setup (to be in consistent enviornment)
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_10_1_9
# cmsenv: does not work
eval `scramv1 runtime -sh`

cd {workingDir_}
{cmd_}


""".format(workingDir_=path, cmd_=cmd )
        )

        f_script.close()

    def GenerateROOTCode(self, ntupleListName, path):
        isMCStr = ""
        normFactorStr = ""
        if self.isMC:
            isMCStr = "kTRUE"
            normFactorStr = "( lumi * {xSec_} ) / {sumWeight_}".format(xSec_=self.xSec, sumWeight_=self.sumWeight)
        else:
            isMCStr = "kFALSE"
            normFactorStr = "1.0"

        str_codes = \
"""#include <{classCodePath_}>

void Run()
{{
  {className_}* producer = new {className_}();

  producer->sampleInfo_.type = "{sampleType_}";
  producer->sampleInfo_.ntuplePathFile = "{ntupleListName_}";
  producer->sampleInfo_.isMC = {isMCStr_};  
  producer->sampleInfo_.xSec = {xSec_};
  producer->sampleInfo_.sumWeight = {sumWeight_};
  Double_t lumi = {luminosity_};
  producer->sampleInfo_.normFactor = {normFactorStr_};

  producer->Run();
}}

""".format(classCodePath_=self.classCodePath, className_=self.className,
           sampleType_=self.sampleType, ntupleListName_=ntupleListName, isMCStr_=isMCStr,
           xSec_=self.xSec, sumWeight_=self.sumWeight, luminosity_=self.luminosity, normFactorStr_=normFactorStr)

        rootCodePath = "%s/Run.cxx" % path
        f_cxx = open(rootCodePath, "w")
        f_cxx.write(str_codes)
        f_cxx.close()


    def WriteCommandInFullScript(self, i, path ):
        scriptPath = "%s/Run.sh" % path

        cmd = "source %s &" % scriptPath
        self.f_fullRunScript.write(cmd+"\n")

    def GenerateHADDScript(self):
        scriptPath = "%s/hadd_all.sh" % self.WSPath
        f_script = open(scriptPath, "w")

        classCodeName = self.classCodePath.split("/")[-1].split(".h")[0]
        mergedFileName = "ROOTFile_%s_%s.root" % (classCodeName, self.sampleType)
        f_script.write("hadd %s/%s \\\n" % (self.WSPath, mergedFileName))

        for i in range(0, self.nJob):
            dirName = "Job_v%03d" % i
            dirPath = "%s/%s" % (self.WSPath, dirName)
            cmd = ""
            if i == self.nJob-1:
                cmd = "%s/*.root" % dirPath
            else:
                cmd = "%s/*.root \\" % dirPath

            f_script.write(cmd+"\n")

        f_script.write("\n")
        f_script.write('echo "[%s] hadd is finished"\n' % self.sampleType)

        if self.bringOutput:
            CWD = os.getcwd()
            cmd_cp = "cp %s/%s %s" % (self.WSPath, mergedFileName, CWD)
            f_script.write(cmd_cp+"\n")
            f_script.write( 'echo "copy: %s -> %s"' % (mergedFileName, CWD) )
            f_script.write("\n")

        f_script.close()

    def CheckOptions(self):
        if self.classCodePath == "":
            print 'classCodePath: not set'
            sys.exit()

        if self.className == "":
            print 'className: not set'
            sys.exit()

        if self.jsonName == "": 
            print 'jsonName: not set'
            sys.exit()

        if self.sampleType == "": 
            print 'sampleType: not set'
            sys.exit()

        if self.luminosity == -1: 
            print 'luminosity: not set'
            sys.exit()

        if self.nJob == -1: 
            print 'nJob: not set'
            sys.exit()

        if self.outputPath == -1: 
            print 'outputPath: not set'
            sys.exit()

    def PrintOptions(self):
        print "[JobGenerator::Options]"
        print "   classCodePath = ", self.classCodePath
        print "   className     = ", self.className
        print "   jsonName      = ", self.jsonName
        print "   sampleType    = ", self.sampleType
        print "   luminosity    = ", self.luminosity
        print "   nJob          = ", self.nJob
        print "   outputPath    = ", self.outputPath
        print ""