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
                  False, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.bool,           # string, int, float, bool
                  "Bool isMC")
options.register ('useVtx0',
                  True, # default value
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

# Pick up trigger info
if options.isMC: 
   bit  = 'TriggerResults::HLT'
   flag = 'TriggerResults::PAT'
else:
   bit  = 'TriggerResults::HLT'
   flag = 'TriggerResults::RECO' 



process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1000 )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('/store/group/phys_higgs/cmshgg/musella/flashgg/ExoPhys14_v2/diphotonsPhys14V2/RSGravToGG_kMpl001_M_5000_Tune4C_13TeV_pythia8/ExoPhys14_v2-diphotonsPhys14V2-v0-Phys14DR-PU20bx25_PHYS14_25_V1-v1/150128_133931/0000/myOutputFile_1.root'
                           )                                   

if (options.isMC==False):
    print "applying 2017 json"                                
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())  
    JSONfile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PromptReco/Cert_294927-305636_13TeV_PromptReco_Collisions17_JSON.txt' 
    myLumis = LumiList.LumiList(filename = JSONfile).getCMSSWString().split(',')  
    process.source.lumisToProcess.extend(myLumis)  

process.load("flashgg/MicroAOD/flashggPhotons_cfi")
process.load("flashgg/MicroAOD/flashggDiPhotons_cfi")

process.TFileService = cms.Service("TFileService",fileName = cms.string("OUTPUT"))

process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
)

# Get vtx0 diphoton collection
if options.useVtx0:
    from flashgg.MicroAOD.flashggDiPhotons_cfi import flashggDiPhotonsLite
    process.flashggDiPhotonsVtx0 = flashggDiPhotonsLite.clone(VertexSelectorName="FlashggZerothVertexSelector",whichVertex=cms.uint32(0))
    sourcediphotons = "flashggDiPhotonsVtx0"
else: sourcediphotons = "flashggDiPhotons" # BDT photons

# Make jets   
from flashgg.MicroAOD.flashggJets_cfi import flashggBTag, maxJetCollections
process.flashggUnpackedJets = cms.EDProducer("FlashggVectorVectorJetUnpacker",  
                                             JetsTag = cms.InputTag("flashggFinalJets"),          
                                             NCollections = cms.uint32(maxJetCollections) 
                                             )               
UnpackedJetCollectionVInputTag = cms.VInputTag()       
for i in range(0,maxJetCollections):    
    UnpackedJetCollectionVInputTag.append(cms.InputTag('flashggUnpackedJets',str(i)))  

##===========================================================================================================================#
process.diPhoAna = cms.EDAnalyzer('DiPhoAnalyzer_Moriond17', ## important -- have to keep format xx = xx (no tabs) for reading in batchjobs
                                  VertexTag = cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
				  METTag = cms.untracked.InputTag('flashggMets::FLASHggMicroAOD'),
				  pfcands = cms.InputTag("packedPFCandidates"),
                                  inputTagJets = UnpackedJetCollectionVInputTag,  
                                  ElectronTag = cms.InputTag('flashggSelectedElectrons'),
                                  MuonTag = cms.InputTag('flashggSelectedMuons'), 
                                  bTag = cms.untracked.string(flashggBTag),      
				  RhoTag = cms.InputTag('fixedGridRhoAll'),
                                  genPhotonExtraTag = cms.InputTag("flashggGenPhotonsExtra"),   
                                  DiPhotonTag = cms.untracked.InputTag(sourcediphotons),
                                  DiPhotonBDTVtxTag = cms.untracked.InputTag('flashggDiPhotons'),
                                  PileUpTag = cms.untracked.InputTag('slimmedAddPileupInfo'),
                                  generatorInfo = cms.InputTag('generator'),
				  bits = cms.InputTag(bit),
                                  flags = cms.InputTag(flag),
                                  dopureweight = PU, 
                                  sampleIndex = SI,
                                  puWFileName = weights,
                                  SSRewFileName = cms.string('/afs/cern.ch/user/s/soffi/public/transformation_Moriond17_AfterPreApr_v1.root'), 
                                  corrFileName = cms.string('EgammaAnalysis/ElectronTools/data/Moriond17_74x_pho'),
                                  xsec = XS,
                                  kfac = KF,
                                  sumDataset = SDS,
                                  )

if options.useVtx0: process.p = cms.Path( process.flashggDiPhotonsVtx0*process.flashggUnpackedJets*process.diPhoAna )
else:               process.p = cms.Path( process.flashggUnpackedJets*process.diPhoAna )
