

def GetArgument(self, sampleType):
    exampleEDMFile = ''
    globalTag = ''
    isMC = ''

    if sampleType == "ScoutingMuon2018":
        exampleEDMFile = 'file:/u/user/kplee/scratch/Physics/RAW_ScoutingCaloMuon_Run2018C_v2.root'
        globalTag = '101X_dataRun2_HLT_v7'
        isMC = False

    elif sampleType == "MC2018":
        exampleEDMFile = 'file:/u/user/kplee/scratch/Physics/AODSIM_Autumn18_DY10to50_MadgraphMLM.root'
        globalTag = '102X_upgrade2018_realistic_v15'
        isMC = True


    return exampleEDMFile, globalTag, isMC