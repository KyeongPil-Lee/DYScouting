from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = ''
config.General.workArea = 'CRABDir'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../../test/ProduceTree_102X_upgrade2018_realistic_v15.py'

config.Data.inputDataset = ''

config.Data.inputDBS = 'global'
# config.Data.splitting = 'Automatic'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 5
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = False

# config.Site.storageSite = 'T3_KR_KISTI'
config.Site.storageSite = 'T2_KR_KNU'

# config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
# FirstRun = 271036
# LastRun = 284044
# config.Data.runRange = '%d-%d' % (FirstRun, LastRun)


version = '_v20191017_'
# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    # -- DY, M10-50 (Madgraph)
    config.General.requestName = 'DYTree'+version+'DYJetsToLL_M10to50_Madgraph'
    config.Data.inputDataset = '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v2/AODSIM'
    config.Data.ignoreLocality = True
    config.Site.whitelist = ['T2_CH_*', 'T2_FR_*', 'T2_DE_*', 'T2_IT_*', 'T1_RU_*', 'T2_RU_*'] # -- mandatory for ignoreLocality option
    crabCommand('submit', config = config)

    config.General.requestName = 'DYTree'+version+'DYJetsToLL_M10to50_Madgraph_ext'
    config.Data.inputDataset = '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15_ext1-v1/AODSIM'
    config.Data.ignoreLocality = True
    config.Site.whitelist = ['T2_CH_*', 'T2_FR_*', 'T2_DE_*', 'T2_IT_*', 'T1_RU_*', 'T2_RU_*'] # -- mandatory for ignoreLocality option
    crabCommand('submit', config = config)

    # config.General.requestName = ''
    # config.Data.inputDataset = ''
    # crabCommand('submit', config = config)