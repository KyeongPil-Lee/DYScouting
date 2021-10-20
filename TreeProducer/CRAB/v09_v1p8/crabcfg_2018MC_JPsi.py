from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''
config.General.workArea = 'CRABDir'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../../test/ProduceTree.py'
config.JobType.pyCfgParams = ['sampleType=MC2018']

config.Data.inputDataset = ''

config.Data.inputDBS = 'global'
# config.Data.splitting = 'Automatic'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 5
config.Data.publication = False

config.Site.storageSite = 'T3_KR_KNU'

version = 'v1p8'
config.Data.outLFNDirBase = '/store/user/kplee/DYScoutingTree/%s' % version

config.JobType.allowUndistributedCMSSW = True

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    # # -- DY, M10-50 (Madgraph)
    # config.General.requestName = 'DYJetsToLL_M10to50_Madgraph'
    # config.Data.inputDataset = '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v2/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'DYJetsToLL_M10to50_Madgraph_ext'
    # config.Data.inputDataset = '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15_ext1-v1/AODSIM'
    # crabCommand('submit', config = config)

    # # -- DY, M50 (aMCNLO)
    # config.General.requestName = 'DYJetsToLL_M50toInf_aMCNLO'
    # config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v1/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'DYJetsToLL_M50toInf_aMCNLO_ext'
    # config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15_ext2-v1/AODSIM'
    # crabCommand('submit', config = config)

    # -- J/Psi (for tag and probe test)
    config.General.requestName = 'JPsiToMuMu_JPsiPt8_Pythia8'
    config.Data.inputDataset = '/JpsiToMuMu_JpsiPt8_TuneCP5_13TeV-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v1/AODSIM'
    crabCommand('submit', config = config)

