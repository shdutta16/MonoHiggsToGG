import sys, os, numpy, ROOT
from ROOT import TObjString
from ROOT import RooWorkspace
from ROOT import TProcessID
from ROOT import RooRealVar
from ROOT import RooArgList
from ROOT import RooArgSet
from ROOT import RooDataSet
from ROOT import RooDataHist
from ROOT import RooAbsReal

def getEffBary(cat,mZp,mDM):
  eff = 1.0
  if cat==1: #highMET
    eff=(0.00474*mZp+0.0397*mDM-1.4e-07*mZp*mZp-1.19e-05*mDM*mDM-2.95e-06*mZp*mDM+6.1583)
  if cat==0: #lowMET
    eff=(-3e-04*mZp-4e-03*mDM-1.3e-08*mZp*mZp-4.4e-07*mDM*mDM-3.8e-07*mZp*mDM+6.5448)
  return eff

def getEff2HDM(cat,mZp,mDM):
  eff = 1.0
  if cat==1: #highMET
    if (mZp > 2500): mZp = 2500 # flatten efficiency after mZp=2500
    eff=(0.0556*mZp-0.03818*mDM-1.67e-05*mZp*mZp+4.66e-06*mDM*mDM+1.59e-05*mZp*mDM+7.50587)
  if cat==0: #lowMET
    eff=(-9.50e-04*mZp+5.52e-04*mDM+2.67e-07*mZp*mZp+1.44e-07*mDM*mDM-3.16e-07*mZp*mDM+0.810386)
  if (eff < 0): eff = 0.001 # make sure no negative efficiencies
  return eff

def makeCard(TYPE,mZp,mChi,DIR):

  # Setup input and output files
  indir = DIR
  old_str = ""
  new_str = "" 
  if (TYPE=="BARY"):
    old_str = "sig_ZpBaryonic_mZP10_mChi1"
    new_str = "sig_ZpBaryonic_mZP"+mZp+"_mChi"+mChi
  if (TYPE=="2HDM"):
    old_str = "sig_2HDM_mZP600_mA0300"
    new_str = "sig_2HDM_mZP"+mZp+"_mA0"+mChi
  fin  = open(indir+"dataCard_"+old_str+"_13TeV.txt","r")
  fout = open(indir+"dataCard_"+new_str+"_13TeV.txt","w") 
  rin  = ROOT.TFile(indir+old_str+"_13TeV.root") 
  rout = ROOT.TFile(indir+new_str+"_13TeV.root","RECREATE")

  # Copy the datacard for new mass point
  for line in fin:
    if old_str in line:
      line = line.replace(old_str,new_str)
    fout.write(line) 

  # Get the old and new eff
  mZ  = float(mZp)
  mDM = float(mChi)
  old_efflowMET  = 1.0
  old_effhighMET = 1.0
  new_efflowMET  = 1.0
  new_effhighMET = 1.0
  if (TYPE=="BARY"):
    old_efflowMET  = getEffBary(0,10,1)
    old_effhighMET = getEffBary(1,10,1)
    new_efflowMET  = getEffBary(0,mZ,mDM)
    new_effhighMET = getEffBary(1,mZ,mDM)
  if (TYPE=="2HDM"):
    old_efflowMET  = getEff2HDM(0,600,300)
    old_effhighMET = getEff2HDM(1,600,300)
    new_efflowMET  = getEff2HDM(0,mZ,mDM)
    new_effhighMET = getEff2HDM(1,mZ,mDM)
  scale_lowMET   = new_efflowMET/old_efflowMET
  scale_highMET  = new_effhighMET/old_effhighMET
  #print("Old eff: low = %f, high = %f" %(old_efflowMET,old_effhighMET)) 
  #print("New eff: low = %f, high = %f" %(new_efflowMET,new_effhighMET))
  #print("Scale:   low = %f, high = %f" %(scale_lowMET,scale_highMET)) 

  # Copy the input file
  in_TObjString    = TObjString(rin.cfg)
  out_TObjString   = in_TObjString.Clone()
  in_RooWorkspace  = RooWorkspace(rin.wtemplates)
  #in_RooWorkspace.Print() # print obj in input rooWorkspace
  w1 = ROOT.RooWorkspace("wtemplates")
  w1.rooImport = getattr(w1,'import')

  var1 = in_RooWorkspace.var('mgg')
  var2 = in_RooWorkspace.var('model_signal_'+old_str+'_13TeV_met0-130_norm')
  var3 = in_RooWorkspace.var('model_signal_'+old_str+'_13TeV_met130_norm')
  # multiply old normalization by new scale
  valnorm_lowMET  = scale_lowMET*var2.getValV()
  valnorm_highMET = scale_highMET*var3.getValV()
  norm1 = RooRealVar("model_signal_"+new_str+"_13TeV_met0-130_norm","model_signal"+new_str+"13TeV_met0-130_norm",valnorm_lowMET)
  norm2 = RooRealVar("model_signal_"+new_str+"_13TeV_met130_norm","model_signal"+new_str+"13TeV_met130_norm",valnorm_highMET)
  varlist = ROOT.RooArgList(var1,norm1,norm2)
  #print("%f * %f" %(scale_lowMET,var2.getValV()))
  #print("%f" %valnorm_lowMET)
  #print("%f" %norm1.getValV())

  # get old pdfs and change names
  pdf1 = in_RooWorkspace.pdf('model_signal_'+old_str+'_13TeV_met0-130')
  pdf2 = in_RooWorkspace.pdf('model_signal_'+old_str+'_13TeV_met0-130_energyScalemet0-130Down')
  pdf3 = in_RooWorkspace.pdf('model_signal_'+old_str+'_13TeV_met0-130_energyScalemet0-130Up')
  pdf4 = in_RooWorkspace.pdf('model_signal_'+old_str+'_13TeV_met130')
  pdf5 = in_RooWorkspace.pdf('model_signal_'+old_str+'_13TeV_met130_energyScalemet130Down')
  pdf6 = in_RooWorkspace.pdf('model_signal_'+old_str+'_13TeV_met130_energyScalemet130Up')
  pdf1new = ROOT.RooHistPdf(pdf1,"model_signal_"+new_str+"_13TeV_met0-130") 
  pdf2new = ROOT.RooHistPdf(pdf2,"model_signal_"+new_str+"_13TeV_met0-130_energyScalemet0-130Down") 
  pdf3new = ROOT.RooHistPdf(pdf3,"model_signal_"+new_str+"_13TeV_met0-130_energyScalemet0-130Up") 
  pdf4new = ROOT.RooHistPdf(pdf4,"model_signal_"+new_str+"_13TeV_met130") 
  pdf5new = ROOT.RooHistPdf(pdf5,"model_signal_"+new_str+"_13TeV_met130_energyScalemet130Down") 
  pdf6new = ROOT.RooHistPdf(pdf6,"model_signal_"+new_str+"_13TeV_met130_energyScalemet130Up") 

  # these are null pointers -- probably don't have right classes (missing from dipho analysis) to read them
  # but they are also not needed for running higgs combine so left out for now 
  dat1 = in_RooWorkspace.data('signal_'+old_str+'_13TeV_met130')
  dat2 = in_RooWorkspace.data('signalforPdf_'+old_str+'_13TeV_met130')
  dat3 = in_RooWorkspace.data('signal_'+old_str+'_13TeV_met0-130')
  dat4 = in_RooWorkspace.data('signalforPdf_'+old_str+'_13TeV_met0-130')
  #print("%f" %dat1.sumEntries())

  # Write to output file
  #out_TObjString.Write()
  w1.rooImport(var1)
  w1.rooImport(norm1)
  w1.rooImport(norm2)

  w1.rooImport(pdf1new)
  w1.rooImport(pdf2new)
  w1.rooImport(pdf3new)
  w1.rooImport(pdf4new)
  w1.rooImport(pdf5new)
  w1.rooImport(pdf6new)

  #w1.Print() # print contents of workspace
  w1.Write()
  rout.Close()

if __name__ == "__main__":
  filename = ""
  DIR = ""

  if len(sys.argv) < 4: 
    print("Invalid usage! Correct usage is: python makeGenericDataCard.py [mZp] [mChi] [DIR] [type]")
    sys.exit()
  else:
    MZP  = sys.argv[1]
    MCHI = sys.argv[2]
    DIR  = sys.argv[3]
    TYPE = sys.argv[4] 
  if TYPE=="BARY" and MZP=="10" and MCHI=="1": 
    print("Bary: Can't use mZp = 10 and mChi = 1. That is the input! Can't be the output!")
    sys.exit()
  if TYPE=="2HDM" and MZP=="600" and MCHI=="300":
    print("2HDM: Can't use mZp = 600 and mA = 300. That is the input! Can't be the output!")
    sys.exit()
    
  print("Making datacard (%s) for mZp = %s , mChi = %s" %(TYPE, MZP, MCHI))
  makeCard(TYPE,MZP,MCHI,DIR)
  print("Finished")
