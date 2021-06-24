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

version = 'v1p6'
config.Data.outLFNDirBase = '/store/user/kplee/DYScoutingTree_%s' % version

config.JobType.allowUndistributedCMSSW = True

# 'MultiCRAB' part
if __name__ == '__main__':
    
    from CRABAPI.RawCommand import crabCommand

    # -- DY, M10-50 (Madgraph)
    config.General.requestName = 'DYJetsToLL_M10to50_Madgraph'
    config.Data.inputDataset = '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v2/AODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'DYJetsToLL_M10to50_Madgraph_ext'
    config.Data.inputDataset = '/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15_ext1-v1/AODSIM'
    crabCommand('submit', config = config)

    # -- DY, M50 (aMCNLO)
    config.General.requestName = 'DYJetsToLL_M50toInf_aMCNLO'
    config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v1/AODSIM'
    crabCommand('submit', config = config)

    config.General.requestName = 'DYJetsToLL_M50toInf_aMCNLO_ext'
    config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15_ext2-v1/AODSIM'
    crabCommand('submit', config = config)

    # # -- QCD, inclusive
    # config.General.requestName = 'QCDMuEnriched_Pt20toInf'
    # config.Data.inputDataset = '/QCD_Pt-20toInf_MuEnrichedPt15_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v1/AODSIM'
    # crabCommand('submit', config = config)

    # # -- QCD, pT-binned
    # config.General.requestName = 'QCDMuEnriched_Pt15to20'
    # config.Data.inputDataset = '/QCD_Pt-15to20_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v3/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt20to30'
    # config.Data.inputDataset = '/QCD_Pt-20to30_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v4/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt30to50'
    # config.Data.inputDataset = '/QCD_Pt-30to50_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v3/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt50to80'
    # config.Data.inputDataset = '/QCD_Pt-50to80_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v3/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt80to120'
    # config.Data.inputDataset = '/QCD_Pt-80to120_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v1/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt80to120_ext'
    # config.Data.inputDataset = '/QCD_Pt-80to120_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15_ext1-v2/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt120to170'
    # config.Data.inputDataset = '/QCD_Pt-120to170_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v1/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt120to170_ext'
    # config.Data.inputDataset = '/QCD_Pt-120to170_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15_ext1-v2/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt170to300'
    # config.Data.inputDataset = '/QCD_Pt-170to300_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v3/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt300to470'
    # config.Data.inputDataset = '/QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v3/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt300to470_ext'
    # config.Data.inputDataset = '/QCD_Pt-300to470_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15_ext3-v1/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt470to600'
    # config.Data.inputDataset = '/QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v1/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt470to600_ext'
    # config.Data.inputDataset = '/QCD_Pt-470to600_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15_ext1-v2/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt600to800'
    # config.Data.inputDataset = '/QCD_Pt-600to800_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v1/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt800to1000'
    # config.Data.inputDataset = '/QCD_Pt-800to1000_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15_ext3-v2/AODSIM'
    # crabCommand('submit', config = config)

    # config.General.requestName = 'QCDMuEnriched_Pt1000toInf'
    # config.Data.inputDataset = '/QCD_Pt-1000toInf_MuEnrichedPt5_TuneCP5_13TeV_pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v1/AODSIM'
    # crabCommand('submit', config = config)

    # # -- ttbar
    # config.General.requestName = 'TTTo2L2Nu_Powheg'
    # config.Data.inputDataset = '/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v1/AODSIM'
    # crabCommand('submit', config = config)

    # # -- W+jets (Madgraph)
    # config.General.requestName = 'WJetsToLNu_Madgraph'
    # config.Data.inputDataset = '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v2/AODSIM'
    # crabCommand('submit', config = config)
