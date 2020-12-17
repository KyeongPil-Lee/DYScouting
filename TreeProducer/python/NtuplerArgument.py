

def GetArgument(sampleType):
    exampleEDMFile = ''
    globalTag = ''
    isMC = ''
    isMiniAOD = ''

    if sampleType == "ScoutingMuon2018":
        exampleEDMFile = 'file:/u/user/kplee/scratch/Physics/RAW_ScoutingCaloMuon_Run2018C_v2.root'
        globalTag = '101X_dataRun2_HLT_v7'
        isMC = False
        isMiniAOD = False


    elif sampleType == "MC2018":
         exampleEDMFile = 'file:/u/user/kplee/scratch/Physics/AODSIM_Autumn18_DY10to50_MadgraphMLM.root'
         globalTag = '102X_upgrade2018_realistic_v15'
         isMC = True
         isMiniAOD = False

    elif sampleType == "MC2018UL_MINIAOD":
         exampleEDMFile = 'file:/afs/cern.ch/work/k/kplee/private/ROOTFile_Test/106X/UL18/MINIAODSIM_DYLL_M50_aMCNLO_Summer19UL18.root' # -- lxplus
         globalTag = '106X_upgrade2018_realistic_v11_L1v1'
         isMC = True
         isMiniAOD = True

    elif sampleType == "Data2018UL_MINIAOD":
         exampleEDMFile = '' # -- lxplus
         globalTag = '106X_dataRun2_v28'
         isMC = False
         isMiniAOD = True

    else:
        print "[GetArgument] type = %s is not available sample type" % (sampleType)


    return exampleEDMFile, globalTag, isMC, isMiniAOD