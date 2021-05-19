from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''
config.General.workArea = 'CRABDir'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../../test/ProduceTree.py'
# config.JobType.pyCfgParams = ['sampleType=Data2018UL_MINIAOD']

config.Data.inputDataset = ''

config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 20
config.Data.publication = False

config.Site.storageSite = 'T3_KR_KNU'

config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/ReReco/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt'

version = 'v1p5p4'
config.Data.outLFNDirBase = '/store/user/kplee/DYScoutingTree_%s' % version

config.JobType.allowUndistributedCMSSW = True

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    config.General.requestName = 'SingleMuon_Run2018A'
    config.Data.inputDataset = '/SingleMuon/Run2018A-17Sep2018-v2/MINIAOD'
    config.JobType.pyCfgParams = ['sampleType=Data2018_MINIAOD']
    crabCommand('submit', config = config)

    config.General.requestName = 'SingleMuon_Run2018B'
    config.Data.inputDataset = '/SingleMuon/Run2018B-17Sep2018-v1/MINIAOD'
    config.JobType.pyCfgParams = ['sampleType=Data2018_MINIAOD']
    crabCommand('submit', config = config)

    config.General.requestName = 'SingleMuon_Run2018C'
    config.Data.inputDataset = '/SingleMuon/Run2018C-17Sep2018-v1/MINIAOD'
    config.JobType.pyCfgParams = ['sampleType=Data2018_MINIAOD']
    crabCommand('submit', config = config)

    # config.General.requestName = 'SingleMuon_Run2018D'
    # config.Data.inputDataset = '/SingleMuon/Run2018D-PromptReco-v2/MINIAOD'
    # config.JobType.pyCfgParams = ['sampleType=Data2018Prompt_MINIAOD']
    # crabCommand('submit', config = config)
