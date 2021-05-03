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

version = 'v1p5p1'
config.Data.outLFNDirBase = '/store/user/kplee/DYScoutingTree_%s' % version

config.JobType.allowUndistributedCMSSW = True

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    config.General.requestName = 'QCDMuEnriched_Pt170to300'
    config.Data.inputDataset = '/QCD_Pt-170to300_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v3/AODSIM'
    crabCommand('submit', config = config)
