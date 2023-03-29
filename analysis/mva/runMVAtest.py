#!/bin/env python

from utils.pyapp import * 
from optparse import OptionParser, make_option

# ----------------------------------------------------------------
# --- Setup main options
# ----------------------------------------------------------------
class PlotMaker(pyapp):

  def __init__(self):
    # --- Additional options
    super(PlotMaker,self).__init__(option_list=[
      make_option("--mva",action="store",dest="mvatype",type="string",
                  default="SVM",help="Input type of MVA [default = %default]"),
      make_option("-s","--sig",action="store",dest="sig",type="string",
                  default="BARY",help="Signal [default = %default]"),
      make_option("--indir",action="store",dest="indir",type="string",
                  default="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_MVA/",help="Indir [default = %default]"),
      make_option("--skimname",action="store",dest="skimname",type="string",
                  default="_skim_woVetos",help="Skim name [default = %default]"),
      make_option("--suffix",action="store",dest="suffix",type="string",
                  default="",help="Additional suffix [default = %default]"),
      make_option("--apply",action="store_true",dest="apply_mva",
                  default=False,help="Apply MVA"),
      ])  
    
    global ROOT
    import sys, os, numpy, ROOT
    from ROOT import gDirectory
    from ROOT import TMVA
    from ROOT.TMVA import Factory
    from optparse import OptionParser
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptTitle(0)

#--------------------------------------------------------------------------------------------------------------
  def __call__(self,options,args):
    print("Apply MVA: %s" %options.mvatype)
    
    # --- Input files
    signame = ""
    if options.sig=="BARY": signame = "BaryonicZp_mZP10_mChi1"
    if options.sig=="2HDM": signame = "2HDM_mZP600_mA0300"
    fsig = ROOT.TFile.Open(options.indir+signame+options.skimname+".root")
    bkgs = []
    bkgs.append("NonResBkg")
    bkgs.append("EwkBkg")
    bkgs.append("GluGluHToGG")
    bkgs.append("VHToGG")
    bkgs.append("ttHJetToGG")
    bkgs.append("VBFHToGG")
    fbkg = {}
    for bkg in bkgs: fbkg[bkg] = ROOT.TFile.Open(options.indir+bkg+options.skimname+".root") 
    
    # --- Input trees 
    tsig = fsig.Get('DiPhotonTree')
    tbkg = {}
    for bkg in bkgs: tbkg[bkg] = fbkg[bkg].Get('DiPhotonTree')
    
    # --- Ouput file
    if options.suffix!="": options.suffix = "_"+options.suffix
    fileoutname = "TMVA_"+options.mvatype+"_"+options.sig+options.suffix
    fileout     = ROOT.TFile(fileoutname+".root","RECREATE")
    
    # --- Create TMVA factory
    ROOT.TMVA.Tools.Instance()
    factory = ROOT.TMVA.Factory("TMVAClassification", fileout, "!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification")
    loader  = ROOT.TMVA.DataLoader(options.mvatype+"_"+options.sig+options.suffix)
 
    # --- Parse suffix
    suff  = options.suffix
    split = suff.split('_')
    opt = 0
    for s in split: 
      if 'opt' in s: opt = int(s[3])

    # --- Add the variables to the TMVA factory as floats
    var = []
    var.append('dphiggmet')
    var.append('ptgg')
    if opt >= 2: var.append('t1pfmetCorr')
    if opt >= 3: var.append('nJets30')
    if opt >= 3: var.append('nEle')
    if opt >= 3: var.append('nMuons')
    if opt >= 4: var.append('phigg')
    if opt >= 5: var.append('mgg')
    print var
    for v in var:
      if v[0]=="n": loader.AddVariable(v,'I') # integer variables 
      else:         loader.AddVariable(v,'F') # float variables
    
    # --- Link signal and background to root ntuple
    loader.AddSignalTree(tsig)
    for bkg in bkgs: loader.AddBackgroundTree(tbkg[bkg])
    
    # --- Cuts defining the signal and background sample
    sigCut = ROOT.TCut("sampleID > 100")
    bkgCut = ROOT.TCut("sampleID <= 100")
    cut = ROOT.TCut("")

    # --- Prepare the training/testing signal/background
    if options.mvatype=="SVM": loader.PrepareTrainingAndTestTree(cut,10000,50000,10000,50000,"SplitMode=Random:!V") 
    if options.mvatype=="BDT": loader.PrepareTrainingAndTestTree(sigCut,bkgCut,"nTrain_Signal=10000:nTrain_Background=50000:nTest_Signal=10000:nTest_Background=50000:SplitMode=Random:NormMode=NumEvents:!V")
    if options.mvatype=="DNN": loader.PrepareTrainingAndTestTree(sigCut,bkgCut,"nTrain_Signal=10000:nTrain_Background=50000:nTest_Signal=10000:nTest_Background=50000:SplitMode=Random:NormMode=NumEvents:!V") 
 
    # --- Extra options
    if options.mvatype=="SVM": 
      mvaoptions = "C=1.0:Gamma=0.005:Tol=0.001:VarTransform=None"
    if options.mvatype=="BDT": 
      mvaoptions = "!H:!V:NTrees=1000:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20"
    if options.mvatype=="DNN": 
      mvaoptions = "!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=G:WeightInitialization=XAVIERUNIFORM"
      mvaoptions += ":Layout=TANH|100,TANH|50,TANH|10,LINEAR:"
      train0 = "LearningRate=le-l,Momentum=0.0,Repetitions=1,ConvergenceSteps=300,BatchSize=20,TestRepetitions=15,WeightDecay=0.001,Regularization=NONE,DropConfig=0.0+0.5+0.5+0.5,DropRepetitions=1,Multithreading=True"
      train1 = "LearningRate=1e-2,Momentum=0.5,Repetitions=1,ConvergenceSteps=300,BatchSize=30,TestRepetitions=7,WeightDecay=0.001,Regularization=L2,Multithreading=True,DropConfig=0.0+0.1+0.1+0.1,DropRepetitions=1"
      train2 = "LearningRate=1e-2,Momentum=0.3,Repetitions=1,ConvergenceSteps=300,BatchSize=40,TestRepetitions=7,WeightDecay=0.0001,Regularization=L2,Multithreading=True"
      train3 = "LearningRate=1e-3,Momentum=0.1,Repetitions=1,ConvergenceSteps=200,BatchSize=70,TestRepetitions=7,WeightDecay=0.0001,Regularization=NONE,Multithreading=True"
      trainingString = train0+"|"+train1+"|"+train2+"|"+train3
      mvaoptions += trainingString 
 
    # --- Book the method
    if options.mvatype=="SVM": method = factory.BookMethod( loader, ROOT.TMVA.Types.kSVM, "SVM", mvaoptions)
    if options.mvatype=="BDT": method = factory.BookMethod( loader, ROOT.TMVA.Types.kBDT, "BDT", mvaoptions)
    if options.mvatype=="DNN": method = factory.BookMethod( loader, ROOT.TMVA.Types.kDNN, "DNN", mvaoptions)

   
    # --- Training
    factory.TrainAllMethods()
    # --- Testing
    factory.TestAllMethods()
    # --- Evaluate and compare
    factory.EvaluateAllMethods()

    print("---> Wrote root file: %s.root" %fileoutname)
    print("---> TMVA is done!")  

#--------------------------------------------------------------------------------------------------------------

if __name__ == "__main__":
  app = PlotMaker()
  app.run()
 
