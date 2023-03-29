from CRABClient.UserUtilities import config
config = config()

config.General.requestName  = 'ggTomonoH_aa_sinp0p35_tanb1p0_MXd10_MH3_300_MH4_200'
config.General.transferLogs = True
config.JobType.pluginName   = 'Analysis'

config.JobType.inputFiles  = ['input_files_2018/PU_Reweight_2018.root', 'input_files_2018/Run2018_Step2Closure_CoarseEtaR9Gain_v2_smearings.dat', 'input_files_2018/Run2018_Step2Closure_CoarseEtaR9Gain_v2_scales.dat']

# Name of the CMSSW configuration file                                                                        
 
config.JobType.psetName    = 'diPhoAna_2018.py'

#config.Data.inputDataset = '/SingleElectron/Run2016F-HcalCalIsoTrkFilter-PromptReco-v1/ALCARECO'                                                                      
#config.Data.inputDataset = '/DoublePion_E-50/PhaseIFall16DR-FlatPU20to50RECO_81X_upgrade2017_realistic_v26-v1/GEN-SIM-RECO'            
                               
#config.Data.inputDataset = '/DoubleEG/sethzenz-RunIIFall17-2_7_7-2_7_7-v0-Run2017C-17Nov2017-v1-07c3a951e9177a6fb1470ee3c7814a6f/USER'
#config.Data.inputDataset = '/DoubleEG/sethzenz-RunIIFall17-2_7_5-2_7_5-v0-Run2017C-PromptReco-v1-55dd8a3831767cdcb092d16987c90615/USER'
#config.Data.inputDataset = '/DoubleEG/sethzenz-RunIIFall17-3_0_1-3_0_1-v0-Run2017B-17Nov2017-v1-a18f46d96a188391b5acca7d030a860e/USER'
#config.Data.inputDataset = '/DoubleEG/sethzenz-RunIIFall17-3_1_0-3_1_0-v0-Run2017B-31Mar2018-v1-aa3a4880c1f02b5f7421114b3c220c90/USER'
#config.Data.inputDataset = '/GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8/spigazzi-Era2017_RR-31Mar2018_v2-legacyRun2FullV1-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2-881967903231519e7d04858a35c22266/USER'
config.Data.inputDataset = '/GluGluHToGG_M125_TuneCP5_13TeV-amcatnloFXFX-pythia8/spigazzi-Era2018_RR-17Sep2018_v2-legacyRun2FullV2-v0-RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1-a56b881212d7e1373ea9751571b84b54/USER'

#config.Data.inputDataset = '/ggTomonoH_aa_sinp0p35_tanb1p0_MXd10_MH3_300_MH4_200_TuneCP3_madgraph-pythia/phys_higgs-monoH_Fall17_12Apr2018_94X_mc2017_realistic_v14-v2-RunIIFall18-4_0_0-465-g807f3606-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2-7b5bde4b08ed54bb0040817619f76064/USER'


config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSReader'
#config.Data.inputDBS = 'phys03'

#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt'
#config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt'

config.Data.splitting = 'FileBased'                                                                                                                                    
#config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 2

#config.Data.outLFNDirBase = '/store/user/%s/MHgg/SignalDEG_2017Data' % (getUsernameFromSiteDB())
#config.Data.outLFNDirBase = '/store/group/phys_egamma/shdutta/2018Analysis/MC_bkg/GluGluHToGG/'
config.Data.outLFNDirBase = '/store/user/shdutta/2018Analysis_reloaded/MC_sig/ggTomonoH_aa_sinp0p35_tanb1p0_MXd10_MH3_300_MH4_200/'
config.Data.publication = False

#config.Data.ignoreLocality = True
#config.Site.whitelist = ['T2_CH_CERN']
# Where the output files will be transmitted to                                                                                                                        
 
#config.Site.storageSite = 'T3_US_FNALLPC'
#config.Site.storageSite = 'T2_CH_CERN'
config.Site.storageSite = 'T2_IN_TIFR'
