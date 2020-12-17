from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''
config.General.workArea = 'CRABDir'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../../test/ProduceTree.py'
config.JobType.pyCfgParams = ['sampleType=MC2018UL_MINIAOD']

config.Data.inputDataset = ''

config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 5
config.Data.publication = False

config.Site.storageSite = 'T3_KR_KNU'

version = 'v1p4'
config.Data.outLFNDirBase = '/store/user/kplee/DYScoutingTree_%s' % version

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    config.General.requestName = 'DYLL_M50_aMCNLO_UL18'
    config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'TTTo2L2Nu_Powheg_UL18'
    config.Data.inputDataset = '/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'WJetsToLNu_MadgraphMLM_UL18'
    config.Data.inputDataset = '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'ST_tW_antitop_Powheg_UL18'
    config.Data.inputDataset = '/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'ST_tW_top_Powheg_UL18'
    config.Data.inputDataset = '/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v1/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'WW_Pythia8_UL18'
    config.Data.inputDataset = '/WW_TuneCP5_13TeV-pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'WZ_Pythia8_UL18'
    config.Data.inputDataset = '/WZ_TuneCP5_13TeV-pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'ZZ_Pythia8_UL18'
    config.Data.inputDataset = '/ZZ_TuneCP5_13TeV-pythia8/RunIISummer19UL18MiniAOD-106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
    crabCommand('submit', config = config)
