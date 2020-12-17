from CRABClient.UserUtilities import config
config = config()

config.General.requestName = ''
config.General.workArea = 'CRABDir'

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../../test/ProduceTree.py'
config.JobType.pyCfgParams = ['sampleType=Data2018UL_MINIAOD']

config.Data.inputDataset = ''

config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 20
config.Data.publication = False

config.Site.storageSite = 'T3_KR_KNU'

config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/Legacy_2018/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt'

version = 'v1p4'
config.Data.outLFNDirBase = '/store/user/kplee/DYScoutingTree_%s' % version

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    config.General.requestName = 'EGamma_Run2018A_UL18'
    config.Data.inputDataset = '/EGamma/Run2018A-12Nov2019_UL2018-v2/MINIAOD'
    crabCommand('submit', config = config)

    config.General.requestName = 'EGamma_Run2018B_UL18'
    config.Data.inputDataset = '/EGamma/Run2018B-12Nov2019_UL2018-v2/MINIAOD'
    crabCommand('submit', config = config)

    config.General.requestName = 'EGamma_Run2018C_UL18'
    config.Data.inputDataset = '/EGamma/Run2018C-12Nov2019_UL2018-v2/MINIAOD'
    crabCommand('submit', config = config)

    config.General.requestName = 'EGamma_Run2018D_UL18'
    config.Data.inputDataset = '/EGamma/Run2018D-12Nov2019_UL2018-v4/MINIAOD'
    crabCommand('submit', config = config)