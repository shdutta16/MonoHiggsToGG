#!/bin/env python

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.PythonUtilities.LumiList as LumiList  
import FWCore.ParameterSet.Types as CfgTypes  
import FWCore.ParameterSet.VarParsing as VarParsing


process = cms.Process("diPhoAna")
process.load("FWCore.MessageService.MessageLogger_cfi")

# Setup input variables
options = VarParsing.VarParsing('analysis')
options.register ('isMC',
                  True, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.bool,           # string, int, float, bool
                  "Bool isMC")
options.register ('useVtx0',
                  False, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.bool,           # string, int, float, bool
                  "Bool useVtx0")
options.parseArguments()

if options.isMC: print'Sample is MC'
else:            print'Sample is Data'

# Pick up the GlobalTag
from Configuration.AlCa.GlobalTag import GlobalTag

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")

from Configuration.AlCa.autoCond import autoCond

if options.isMC:
   process.GlobalTag = GlobalTag(process.GlobalTag, autoCond['run2_mc'].replace("::All","") )
else:
   process.GlobalTag = GlobalTag(process.GlobalTag, autoCond['run2_data'].replace("::All","") )

# Do pileup reweighting
if options.isMC: do_pu = 1
else:            do_pu = 0

# Set sample index (testing only)
if options.isMC: index = 101
else:            index = 10001

# Pick up trigger info
if options.isMC: 
   bit  = 'TriggerResults::HLT'
   flag = 'TriggerResults::PAT'
else:
   bit  = 'TriggerResults::HLT'
   flag = 'TriggerResults::RECO' 

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
print 'Now trying to get input file'

process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(
	#"file:myMicroAODOutputFile.root"
        # 2017 test
        #"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall17-2_7_5/2_7_5/DoubleEG/RunIIFall17-2_7_5-2_7_5-v0-Run2017C-PromptReco-v1/171030_113309/0000/myMicroAODOutputFile_1.root",
        #"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall17-3_0_0/3_0_0/DoubleEG/RunIIFall17-3_0_0-3_0_0-v0-Run2017D-17Nov2017-v1/180325_171732/0000/myMicroAODOutputFile_1.root"
        #"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall17-3_1_0/3_1_0/DoubleEG/RunIIFall17-3_1_0-3_1_0-v0-Run2017B-31Mar2018-v1/180606_155530/0000/myMicroAODOutputFile_1.root"
        #"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall17-3_1_0/3_1_0/GluGluHToGG_M100_13TeV_amcatnloFXFX_pythia8/RunIIFall17-3_1_0-3_1_0-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/180609_081858/0000/myMicroAODOutputFile_1.root"
        #"/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall17-3_1_0/3_1_0/GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8/RunIIFall17-3_1_0-3_1_0-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/180605_202403/0000/myMicroAODOutputFile_8.root"
        #"/store/user/soffi/RunIISummer16-2_4_1-25ns_Moriond17_DMHiggs_2HDM_8026/2_4_1/ZprimeToA0hToA0chichihAA_2HDM_MZp-2000_MA0-300_13TeV-madgraph-pythia8/RunIISummer16-2_4_1-25ns_Moriond17_DMHiggs_2HDM_8026-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/170220_104239/0000/myMicroAODOutputFile_2.root"

        #"/store/user/soffi/RunIISummer16-2_4_2-25ns_Moriond17_DMHiggs_2HDM/2_4_1/ZprimeToA0hToA0chichihAA_2HDM_MZp-800_MA0-300_13TeV-madgraph-pythia8/RunIISummer16-2_4_2-25ns_Moriond17_DMHiggs_2HDM-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/170207_230803/0000/myMicroAODOutputFile_2.root",
        #"/store/user/soffi/RunIISummer16-2_4_2-25ns_Moriond17_DMHiggs_2HDM/2_4_1/ZprimeToA0hToA0chichihAA_2HDM_MZp-800_MA0-300_13TeV-madgraph-pythia8/RunIISummer16-2_4_2-25ns_Moriond17_DMHiggs_2HDM-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/170207_230803/0000/myMicroAODOutputFile_1.root"
        #"file:/afs/cern.ch/work/d/dbhowmik/public/Analysis/MHgg/2017Analysis/MCproduction/MiniAODtoMicroAOD/CMSSW_10_5_0/src/flashgg/myMicroAOD_Yutaro.root"
        #"file:/afs/cern.ch/work/d/dbhowmik/public/Analysis/MHgg/2017Analysis/MCproduction/MiniAODtoMicroAOD/CMSSW_9_4_9/src/flashgg/myMicroAOD_Yutaro_flashgg_9_4_9.root"
        #"file:/afs/cern.ch/work/d/dbhowmik/public/Analysis/MHgg/2017Analysis/MCproduction/MiniAODtoMicroAOD/CMSSW_10_5_0/src/flashgg/myMicroAODOutputFile_test.root"
        #"/store/user/soffi/RunIISummer16-2_4_2-25ns_Moriond17_DMHiggs_2HDM/2_4_1/ZprimeToA0hToA0chichihAA_2HDM_MZp-2500_MA0-300_13TeV-madgraph-pythia8/RunIISummer16-2_4_2-25ns_Moriond17_DMHiggs_2HDM-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/170207_230320/0000/myMicroAODOutputFile_3.root"
        #"file:/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/myMicroAODOutputFile_1.root",
        #"file:/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/myMicroAODOutputFile_2.root",
        #"file:/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/myMicroAODOutputFile_3.root",
        #"file:/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/myMicroAODOutputFile_4.root",
        #"file:/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/myMicroAODOutputFile_5.root"
        #"file:myMicroAODOutputFile_67.root"
        #"/store/user/spigazzi/flashgg/Era2017_RR-31Mar2018_v2/legacyRun2FullV1/DoubleEG/Era2017_RR-31Mar2018_v2-legacyRun2FullV1-v0-Run2017D-31Mar2018-v1/190606_095246/0001/myMicroAODOutputFile_1060.root"
        #"/store/user/spigazzi/flashgg/Era2017_RR-31Mar2018_v2/legacyRun2FullV1/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/Era2017_RR-31Mar2018_v2-legacyRun2FullV1-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/190606_094541/0000/myMicroAODOutputFile_625.root"
        #"/store/user/spigazzi/flashgg/Era2018_RR-17Sep2018_v2/legacyRun2FullV2/GluGluHToGG_M125_TuneCP5Down_13TeV-amcatnloFXFX-pythia8/Era2018_RR-17Sep2018_v2-legacyRun2FullV2-v0-RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/190710_092644/0000/myMicroAODOutputFile_2.root"
        #"/store/group/phys_higgs/cmshgg/asroy/flashgg/monoH_Fall17_12Apr2018_94X_mc2017_realistic_v14-v2/RunIIFall18-4_0_0-465-g807f3606/ggTomonoH_aa_sinp0p35_tanb1p0_MXd10_MH3_300_MH4_200_TuneCP3_madgraph-pythia/monoH_Fall17_12Apr2018_94X_mc2017_realistic_v14-v2-RunIIFall18-4_0_0-465-g807f3606-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/230316_201523/0000/myMicroAODOutputFile_1.root"
        #"/store/user/spigazzi/flashgg/Era2018_RR-17Sep2018_v2/legacyRun2FullV2/TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8/Era2018_RR-17Sep2018_v2-legacyRun2FullV2-v0-RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/190717_094233/0000/myMicroAODOutputFile_1.root"
        "/store/group/phys_higgs/cmshgg/asroy/flashgg/monoH_Fall17_12Apr2018_94X_mc2017_realistic_v14-v2/RunIIFall18-4_0_0-465-g807f3606/ggTomonoH_aa_sinp0p35_tanb1p0_MXd10_MH3_300_MH4_200_TuneCP3_madgraph-pythia/monoH_Fall17_12Apr2018_94X_mc2017_realistic_v14-v2-RunIIFall18-4_0_0-465-g807f3606-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/230316_201523/0000/myMicroAODOutputFile_1.root"

      	)
    )

print process.source.fileNames

if (options.isMC==False):
    print "applying 2018 json"                                
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange()) 
    #JSONfile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PromptReco/Cert_294927-305636_13TeV_PromptReco_Collisions17_JSON.txt' 
    #JSONfile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt'
    #JSONfile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt'
    JSONfile ='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/Legacy_2018/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt'
    myLumis = LumiList.LumiList(filename = JSONfile).getCMSSWString().split(',')  
    process.source.lumisToProcess.extend(myLumis)                              
    print myLumis 

process.load("flashgg/MicroAOD/flashggPhotons_cfi")
process.load("flashgg/MicroAOD/flashggDiPhotons_cfi")

process.TFileService = cms.Service("TFileService",fileName = cms.string("diPhotons_legacy2018.root"))

process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.MessageLogger.categories.extend(["GetManyWithoutRegistration","GetByLabelWithoutRegistration"])
_messageSettings = cms.untracked.PSet(
                reportEvery = cms.untracked.int32(1),
                            optionalPSet = cms.untracked.bool(True),
                            limit = cms.untracked.int32(10000000)
                        )

process.MessageLogger.cerr.GetManyWithoutRegistration = _messageSettings
process.MessageLogger.cerr.GetByLabelWithoutRegistration = _messageSettings

# get vtx0 diphoton collection

'''
if options.useVtx0:
    from flashgg.MicroAOD.flashggDiPhotons_cfi import flashggDiPhotonsLite
    process.flashggDiPhotonsVtx0 = flashggDiPhotonsLite.clone(VertexSelectorName="FlashggZerothVertexSelector",whichVertex=cms.uint32(0))
    sourcediphotons = "flashggDiPhotonsVtx0"
else: sourcediphotons = "flashggDiPhotons" # BDT photons
'''

#sourcediphotons = "flashggDiPhotons" # BDT photons
if options.useVtx0:
    from flashgg.MicroAOD.flashggDiPhotons_cfi import flashggDiPhotons
    process.flashggDiPhotonsVtx0 = flashggDiPhotons.clone(VertexSelectorName="FlashggZerothVertexSelector",whichVertex=cms.uint32(0))
    sourcediphotons = "flashggDiPhotonsVtx0"
else: sourcediphotons = "flashggDiPhotons" # BDT photons

print sourcediphotons
# to make jets
from flashgg.MicroAOD.flashggJets_cfi import flashggBTag, maxJetCollections
process.flashggUnpackedJets = cms.EDProducer("FlashggVectorVectorJetUnpacker",  
                                             JetsTag = cms.InputTag("flashggFinalJets"),          
                                             NCollections = cms.uint32(maxJetCollections) 
                                             )               

UnpackedJetCollectionVInputTag = cms.VInputTag()       
for i in range(0,maxJetCollections):    
    UnpackedJetCollectionVInputTag.append(cms.InputTag('flashggUnpackedJets',str(i)))  

## check the event content 
#process.content = cms.EDAnalyzer("EventContentAnalyzer")

#For reference related to jets
#https://github.com/cms-analysis/flashgg/blob/dev_legacy_runII/Taggers/python/flashggVBFMVA_cff.py
pujidPtBin1_loose  = [-0.97, -0.68, -0.53, -0.47]
pujidPtBin2_loose  = [-0.89, -0.52, -0.38, -0.30]

pujidPtBin1_medium = [0.18, -0.55, -0.42, -0.36]
pujidPtBin2_medium = [0.61, -0.35, -0.23, -0.17]

pujidPtBin1_tight  = [0.69, -0.35, -0.26, -0.21]
pujidPtBin2_tight  = [0.86, -0.10, -0.05, -0.01]

if options.isMC:
   METxcorr1 = 0.296713
   METxcorr2 = -0.141506
   METycorr1 = 0.115685
   METycorr2 = 0.0128193
else:
   METxcorr1 = 0.492083
   METxcorr2 = -2.93552
   METycorr1 = 0.17874
   METycorr2 = -0.786844

process.diPhoAna = cms.EDAnalyzer('DiPhoAnalyzer_Legacy18',
                                  VertexTag 		= cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
				  METTag		= cms.untracked.InputTag('flashggMets::FLASHggMicroAOD'),#
				  pfcands		= cms.InputTag("packedPFCandidates"),
                                  inputTagJets		= UnpackedJetCollectionVInputTag,            
                                  ElectronTag		= cms.InputTag('flashggSelectedElectrons'),    
                                  MuonTag		= cms.InputTag('flashggSelectedMuons'),            
                                  bTag 			= cms.untracked.string(flashggBTag),                
				  RhoTag		= cms.InputTag('fixedGridRhoAll'),
                                  genPhotonExtraTag 	= cms.InputTag("flashggGenPhotonsExtra"),    
                                  DiPhotonTag 		= cms.untracked.InputTag(sourcediphotons),
                                  DiPhotonBDTVtxTag     = cms.untracked.InputTag('flashggDiPhotons'),
                                  PileUpTag 		= cms.untracked.InputTag('slimmedAddPileupInfo'),
                                  generatorInfo 	= cms.InputTag('generator'),
                                  dopureweight 		= cms.untracked.int32(do_pu),
                                  bits         		= cms.InputTag(bit),
                                  flags        		= cms.InputTag(flag),
				  sampleIndex  		= cms.untracked.int32(index),
                                  #puWFileName  		= cms.string('pileupWeights_moriond17_v2.root'),
                                  puWFileName  		= cms.string('./input_files_2018/PU_Reweight_2018.root'),
                                  #puWFileName  		= cms.string('pu_EOY_GluGluHToGG.root'),
                                  #SSRewFileName         = cms.string('transformation_Moriond17_AfterPreApr_v1.root'), 
                                  #corrFileName		= cms.string('EgammaAnalysis/ElectronTools/data/Moriond17_74x_pho'),
                                  #corrFileName		= cms.string('MonoHiggsToGG/analysis/python/Run2017_17Nov2017_v1_ele'),
                                  #corrFileName		= cms.string('EgammaAnalysis/ElectronTools/data/Run2017_17Nov2017_v1_ele'),
                                  corrFileName		= cms.string('EgammaAnalysis/ElectronTools/data/ScalesSmearings/Run2018_Step2Closure_CoarseEtaR9Gain_v2'),
                                  #corrFileName		= cms.string('EgammaAnalysis/ElectronTools/data/ScalesSmearings/Run2017_24Feb2020_runEtaR9Gain_v2'),
                                  kfac         		= cms.untracked.double(1.),
                                  sumDataset   		= cms.untracked.double(1.0), 
                                  JetIDLevel		= cms.string('Tight2018'),
                                  pujidWpPtBin1 = cms.vdouble(pujidPtBin1_tight), ## WP for 20 < pT < 30 
                                  pujidWpPtBin2 = cms.vdouble(pujidPtBin2_tight), ## WP for 30 < pT < 50
                                  rmsforwardCut = cms.double(3.0), # default was 0.03 , running on loose pujid
                                  photonFileName = cms.FileInPath("flashgg/Taggers/data/L1prefiring_photonpt_2017BtoF.root"),
                                  #photonFileName = cms.FileInPath(""),
                                  photonHistName = cms.untracked.string("L1prefiring_photonpt_2017BtoF"),      #https://github.com/cms-analysis/flashgg/blob/dev_legacy_runII/Taggers/python/flashggPrefireDiPhotons_cff.py
                                  jetFileName = cms.FileInPath("flashgg/Taggers/data/L1prefiring_jetpt_2017BtoF.root"),
                                  jetHistName = cms.untracked.string("L1prefiring_jetpt_2017BtoF"),
                                  isRelevant = cms.bool(False),
                                  correctHEM2018 = cms.bool(True),
                                  #applyToCentral = cms.bool(False),
                                  METphiXcorr1 = cms.double(METxcorr1),
                                  METphiXcorr2 = cms.double(METxcorr2),
                                  METphiYcorr1 = cms.double(METycorr1),
                                  METphiYcorr2 = cms.double(METycorr2),
                                  )


if options.useVtx0: process.p = cms.Path( process.flashggDiPhotonsVtx0*process.flashggUnpackedJets*process.diPhoAna )    
else:               process.p = cms.Path( process.flashggUnpackedJets*process.diPhoAna )   

#process.p = cms.Path( process.flashggUnpackedJets*process.diPhoAna )   


