import os
import sys
import json

class MultiCondorJobGenerator:
    def __init__(self):
        self.jobName = "" # -- used for workspace name
        self.ROOTCodePath = "" # -- relative to DY_ANALYZER_PATH
        self.className = ""
        self.jsonName = ""
        self.luminosity = -1.0
        self.baseWSPath = ""
        self.dic_nJob = {} # -- nJob for each sample type

        self.WSPath = ""


    def Generate(self):
        self.CreateWorkspace()

        name_haddScript = ""
        name_condorScript = ""
        instruction_testRun = ""
        isDone_TestRunCMD = False
        for sampleType in self.dic_nJob.keys():
            jobGenerator = SingleJobGenerator()
            jobGenerator.ROOTCodePath = self.ROOTCodePath
            jobGenerator.className    = self.className
            jobGenerator.jsonName     = self.jsonName
            jobGenerator.luminosity   = self.luminosity

            # -- under main workspace
            jobGenerator.baseWSPath = self.WSPath

            jobGenerator.sampleType = sampleType
            jobGenerator.nJob = self.dic_nJob[sampleType]

            jobGenerator.Generate()

            name_haddScript   = jobGenerator.haddScriptName
            name_condorScript = jobGenerator.condorScriptName

            if not isDone_TestRunCMD:
                instruction_testRun = jobGenerator.CMDForTestRun()
                isDone_TestRunCMD = True

        self.GenerateFullCondorSubmitScript(name_condorScript)
        self.GenerateFullHaddScript(name_haddScript)

        print instruction_testRun

    def CreateWorkspace(self):
        i = 1
        while "%s_v%02d" % (self.jobName, i) in os.listdir(self.baseWSPath):
            i = i+1

        WSName = "%s_v%02d" % (self.jobName, i)
        self.WSPath = "%s/%s" % (self.baseWSPath, WSName)
        os.mkdir(self.WSPath)

    def GenerateFullCondorSubmitScript(self, name_condorScript):
        scriptPath = "%s/submit_allSamples.sh" % self.WSPath
        f_script = open(scriptPath, "w")
        f_script.write("#!/bin/bash\n")

        for sampleType in self.dic_nJob.keys():
            sampleDirName = sampleType
            if "_skim" in sampleType:
                sampleDirName = sampleType.split("_skim")[0]
            cmd = "condor_submit %s/%s/%s" % (self.WSPath, sampleDirName, name_condorScript)
            f_script.write(cmd+"\n")

        f_script.close()

        print "[MultiJobGenerator] Submit all jobs:"
        print "source %s\n" % scriptPath

    def GenerateFullHaddScript(self, name_haddScript):
        scriptPath = "%s/hadd_allSamples.sh" % self.WSPath
        f_script = open(scriptPath, "w")
        f_script.write("#!/bin/bash\n")

        for sampleType in self.dic_nJob.keys():
            sampleDirName = sampleType
            if "_skim" in sampleType:
                sampleDirName = sampleType.split("_skim")[0]
            cmd = "source %s/%s/%s" % (self.WSPath, sampleDirName, name_haddScript)
            f_script.write(cmd+"\n")

        f_script.close()

        print "[CondorJobGenerator] Do hadd for all samples:"
        print "source %s\n" % scriptPath



# -- 1) generate multiple .txt files containing ntuple paths
# -- 2) generate Run.cxx (simple ROOT macro) (whose argument is the .txt file) to run the class on the ntuples in each text file
# -- 3) generate script_(self.className).sh which runs Run.cxx
# -- 4) generate condorscript_submit.jds to submit all jobs
# -- 5) generate script_hadd_transfer.sh to merge all output ROOT file and copy to CWD (where SingleJobGenerator is executed)
class SingleJobGenerator:
    def __init__(self):
        # -- mandatory options
        self.ROOTCodePath = ""
        self.className = ""
        self.jsonName = ""
        self.sampleType = ""
        self.luminosity = -1
        self.nJob = -1
        self.baseWSPath = ""  # -- base path where the .txt file and the output root files are saved

        # -- internal variables
        self.ntuplePathBase = os.getenv("DY_NTUPLE_PATH")

        self.isMC = False
        self.xSec = -1
        self.sumWeight = -1
        self.list_ntuplePath = []

        self.nFilePerJob = -1

        self.WSPath = ""

        # -- path to the directory containing all text files with the ntuple paths: will be set in MakeDir_TextFile()
        self.textFileDirPath = ""

        # -- base name of the text file with the ntuple path lists (full name: baseName_ntupleList_$(Process).txt)
        self.baseName_ntupleList = "ntuplePathList"

        # -- directory name that contains the text file with ntuple paths
        self.textFileDirName = "textFiles"

        # -- common ROOT marco to run the ROOT class
        self.commonROOTMarcoName = "Run.cxx"

        # -- shell script name to run (executable in the condor script)
        self.scriptNameToRun = "script_condorRun.sh"

        # -- condor script name
        self.condorScriptName = "condorscript_submit.jds"

        # -- script name for hadd and transfer to the submit directory
        self.haddScriptName = "script_hadd_transfer.sh"

    def Generate(self):
        self.FillSampleInfo()
        self.CreateWorkspace()


    # -- fill isMC, xSec, sumWeight and list_ntuplePath
    def FillSampleInfo(self):
        analyzerPath = os.getenv("DY_ANALYZER_PATH")
        jsonPath = "%s/Include/%s" % (analyzerPath, self.jsonName)
        with open(jsonPath) as fullSampleInfo_file:
            fullSampleInfo = json.load(fullSampleInfo_file)

        isSkimTree = False
        if "_skim" in self.sampleType:
            isSkimTree = True
            self.sampleType = self.sampleType.split("_skim")[0]

        isFound = False
        for sample in fullSampleInfo["Sample"]:
            if sample["tag"] == self.sampleType:
                isFound = True
                # print "Comment on sample: ", sample["comment"]
                print "Comment on sample(%s): %s" % (sample["tag"], sample["comment"])
                if isSkimTree:
                    print "  --> skimmed version is used. It should be used only when the skimming condition doesn't affect results of the analyzer"

                self.isMC = sample["isMC"]
                self.xSec = sample["xSec"]
                self.sumWeight = sample["sumWeight"]

                for ntupleDir in sample["list_dir"]:
                    ntupleDirPath = "%s/%s" % (self.ntuplePathBase, ntupleDir)
                    if isSkimTree:
                        ntupleDirPath = ntupleDirPath + "/skim"

                    list_file = os.listdir(ntupleDirPath)
                    for fileName in list_file:
                        if ".root" in fileName:
                            ntuplePath = "%s/%s" % (ntupleDirPath, fileName)
                            self.list_ntuplePath.append( ntuplePath )

                break

        self.list_ntuplePath.sort()

        if not isFound:
            print "No corresponding information to %s in %s" % (self.sampleType, self.jsonName)
            sys.exit()

        if len(self.list_ntuplePath) == 0:
            print "No available root file in the directory"
            sys.exit()


    def CreateWorkspace(self):
        self.WSPath = "%s/%s" % (self.baseWSPath, self.sampleType)
        os.mkdir(self.WSPath)

        self.GenerateTextFile_NtuplePathList()

        self.GenerateCommonROOTMarco()
        self.GenerateCommonShellScript()
        self.GenerateCondorScript()
        self.GenerateHaddScript()


    def GenerateTextFile_NtuplePathList(self):        
        nFile = len(self.list_ntuplePath)
        if self.nJob > nFile:
            print "nJob > nFile -> nJob is set as nFile"
            self.nJob = nFile

        self.nFilePerJob = int( float(nFile) / float(self.nJob) )
        print "nJob = %d, nFile = %d -> nFilePerJob = %d\n" % (self.nJob, nFile, self.nFilePerJob)

        self.MakeDir_TextFile()

        for i_job in range(0, self.nJob):
            list_ntuplePath_iJob = self.GetNtupleListPerJob(i_job)

            textFilePath = "%s/%s_%d.txt" % (self.textFileDirPath, self.baseName_ntupleList, i_job)
            f_textFile = open(textFilePath, "w")
            for fileName in list_ntuplePath_iJob:
                f_textFile.write( fileName + "\n" )
            f_textFile.close()

    def MakeDir_TextFile(self):
        self.textFileDirPath = "%s/%s" % (self.WSPath, self.textFileDirName)
        if self.textFileDirName not in os.listdir(self.WSPath):
            os.mkdir(self.textFileDirPath)
        else:
            print "[SingleJobGenerator] %s exists in %s directory... please check" % (self.textFileDirName, self.WSPath)
            sys.exit()

    def GetNtupleListPerJob(self, i_job):
        list_filePerJob = []
        if i_job == self.nJob-1:
            list_filePerJob = self.list_ntuplePath[int(i_job*self.nFilePerJob):]
        else:
            list_filePerJob = self.list_ntuplePath[int(i_job*self.nFilePerJob):int((i_job+1)*self.nFilePerJob)]

        return list_filePerJob

    def GenerateCommonROOTMarco(self):
        isMCStr = ""
        normFactorStr = ""
        if self.isMC:
            isMCStr = "kTRUE"
            normFactorStr = "( lumi * {xSec_} ) / {sumWeight_}".format(xSec_=self.xSec, sumWeight_=self.sumWeight)
        else:
            isMCStr = "kFALSE"
            normFactorStr = "1.0"

        str_codes = \
"""#include <{ROOTCodePath_}>

void {commonMarcoName_}(TString textFileName_ntupleList)
{{
  {className_}* producer = new {className_}();

  producer->sampleInfo_.type = "{sampleType_}";
  producer->sampleInfo_.ntuplePathFile = textFileName_ntupleList;
  producer->sampleInfo_.isMC = {isMCStr_};  
  producer->sampleInfo_.xSec = {xSec_};
  producer->sampleInfo_.sumWeight = {sumWeight_};
  Double_t lumi = {luminosity_};
  producer->sampleInfo_.normFactor = {normFactorStr_};

  producer->Run();
}}

""".format(commonMarcoName_=self.commonROOTMarcoName.split(".cxx")[0], 
           ROOTCodePath_=self.ROOTCodePath, className_=self.className,
           sampleType_=self.sampleType, isMCStr_=isMCStr,
           xSec_=self.xSec, sumWeight_=self.sumWeight, luminosity_=self.luminosity, normFactorStr_=normFactorStr)

        rootCodePath = "%s/%s" % (self.WSPath, self.commonROOTMarcoName)
        f_cxx = open(rootCodePath, "w")
        f_cxx.write(str_codes)
        f_cxx.close()

    def GenerateCommonShellScript(self):

        scriptPath = "%s/%s" % (self.WSPath, self.scriptNameToRun)
        f_script = open(scriptPath, "w")
        f_script.write(
"""#!/bin/bash

fileName_ntuplePathList=$1

root -l -b -q '{commonROOTMarcoName_}++("'$fileName_ntuplePathList'")'

echo "finished"

""".format(commonROOTMarcoName_ = self.commonROOTMarcoName)
        )

        f_script.close()


    def GenerateCondorScript(self):

        # -- directory for condor log files
        condorDirName = "condor"
        condorDirPath = "%s/%s" % (self.WSPath, condorDirName)
        if condorDirName not in os.listdir(self.WSPath):
            os.mkdir(condorDirPath)

        commonROOTMarcoPath = "%s/%s" % (self.WSPath, self.commonROOTMarcoName)

        condorScriptPath = "%s/%s" % (self.WSPath, self.condorScriptName)
        f_script = open(condorScriptPath, "w")
        # -- $(Process): 0, 1, 2, ... nJob-1
        f_script.write(
"""
executable = {WSPath_}/{scriptNameToRun_}
universe   = vanilla
log        = {condorDirPath_}/condor.log
output     = {condorDirPath_}/condor_output_$(Process).log
error      = {condorDirPath_}/condor_error_$(Process).log
getenv     = True
should_transfer_files = YES
when_to_transfer_output = ON_EXIT

accounting_group=group_cms

# -- to set the location for the output files
initialdir = {WSPath_}

transfer_input_files = {textFileDirPath_}/{baseName_ntupleList_}_$(Process).txt,{commonROOTMarcoPath_}
Arguments            = "{baseName_ntupleList_}_$(Process).txt"


queue {nJob_}

""".format(WSPath_=self.WSPath, scriptNameToRun_ = self.scriptNameToRun, textFileDirPath_=self.textFileDirPath, 
           baseName_ntupleList_=self.baseName_ntupleList, commonROOTMarcoPath_=commonROOTMarcoPath, 
           condorDirPath_=condorDirPath, nJob_=self.nJob)
        )

        f_script.close()

    def GenerateHaddScript(self):
        scriptPath = "%s/%s" % (self.WSPath, self.haddScriptName)
        f_script = open(scriptPath, "w")

        submitDir = os.getcwd()

        ROOTCodeName = self.ROOTCodePath.split("/")[-1].split(".h")[0]
        mergedFileName = "ROOTFile_%s_%s.root" % (ROOTCodeName, self.sampleType)
        mergedFilePath = "%s/%s" % (submitDir, mergedFileName)

        cmd_hadd = "hadd %s %s/*.root" % (mergedFilePath, self.WSPath)

        f_script.write("#!bin/bash\n\n")
        f_script.write(cmd_hadd+"\n")

        f_script.close()


    def CMDForTestRun(self):
        testTextFilePath = "%s/%s_0.txt" % (self.textFileDirName, self.baseName_ntupleList)
        
        instruction = """If you want to run a test job:
cd {WSPath_}
source {scriptNameToRun_} {testTextFilePath_} >&TestRun.log&
cd {cwd_}
tail -f {WSPath_}/TestRun.log
""".format(WSPath_=self.WSPath, scriptNameToRun_=self.scriptNameToRun, 
           testTextFilePath_=testTextFilePath, cwd_=os.getcwd())
        
        return instruction
