#!/bin/env python

from math import sqrt
from ROOT import *
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
      make_option("--indir",action="store",dest="indir",type="string",
                  default="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_MVA/",help="Indir [default = %default]"),
      make_option("--suffix",action="store",dest="suffix",type="string",
                  default="",help="Suffix [default = %default]"),
      ])
       
    global ROOT
    import sys, os, numpy, ROOT

    from ROOT import gDirectory
    from optparse import OptionParser
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptTitle(0)
 
# ----------------------------------------------------------------
  def __call__(self,options,args):
    # --- Setup samples
    part1 = 'OutputMVA_'+options.mvatype+'_'+options.suffix+'_'
    part2 = '_skim_woVetos.root'
    sigs  = []
    bkgs  = [] 
    data  = []
    sigs.append("BaryonicZp_mZP10_mChi1")
    bkgs.append("NonResBkg")
    bkgs.append("EwkBkg")
    bkgs.append("GluGluHToGG")
    bkgs.append("VBFHToGG")
    bkgs.append("ttHJetToGG")
    bkgs.append("VHToGG")
    data.append("DoubleEG") 


    # --- Define dictionaries
    fsigs = {}
    fbkgs = {}
    fdata = {}
    tsigs = {}
    tbkgs = {}
    tdata = {}
    hsigs = {}
    hbkgs = {}
    hdata = {}
    initsig = {}
    initbkg = {}
    initdat = {}
    endsig  = {} 
    endbkg  = {}
    enddat  = {}
    effsig  = {}
    errsig  = {}
    effbkg  = {}
    errbkg  = {}
    effdat  = {}
    errdat  = {}
    numsig  = {}
    numbkg  = {}
    numdat  = {}

    # --- Get files & trees & histos
    for sig in sigs:
      fsigs[sig]   = ROOT.TFile.Open(options.indir+part1+sig+part2)
      tsigs[sig]   = fsigs[sig].Get("DiPhotonTree")
      hsigs[sig]   = fsigs[sig].Get("h_selection")
      initsig[sig] = hsigs[sig].GetBinContent(1)
      endsig[sig]  = self.applyCut(tsigs[sig],initsig[sig])
      numsig[sig]  = endsig[sig][0]
      effsig[sig]  = endsig[sig][1]
      errsig[sig]  = endsig[sig][2]
    for bkg in bkgs: 
      fbkgs[bkg]   = ROOT.TFile.Open(options.indir+part1+bkg+part2)
      tbkgs[bkg]   = fbkgs[bkg].Get("DiPhotonTree")
      hbkgs[bkg]   = fbkgs[bkg].Get("h_selection") 
      initbkg[bkg] = hbkgs[bkg].GetBinContent(1) 
      endbkg[bkg]  = self.applyCut(tbkgs[bkg],initbkg[bkg])
      numbkg[bkg]  = endbkg[bkg][0]
      effbkg[bkg]  = endbkg[bkg][1]
      errbkg[bkg]  = endbkg[bkg][2]
    for dat in data: 
      fdata[dat]   = ROOT.TFile.Open(options.indir+part1+dat+part2)
      tdata[dat]   = fdata[dat].Get("DiPhotonTree")
      hdata[dat]   = fdata[dat].Get("h_selection")
      initdat[dat] = hdata[dat].GetBinContent(1)
      enddat[dat]  = self.applyCut(tdata[dat],initdat[dat])
      numdat[dat]  = enddat[dat][0] 
      effdat[dat]  = enddat[dat][1]
      errdat[dat]  = enddat[dat][2]

    totbkg  = []
    totdat  = []
    for bkg in bkgs: totbkg += numbkg[bkg]
    for dat in data: totdat += numdat[dat]

    num_mva_cuts = len(numsig[sigs[0]])  

    tmparray1 = []
    tmparray2 = []
    sigmasigs = {}
    sigersigs = {}
    for sig in sigs:
      sigmasigs[sig] = []
      sigersigs[sig] = []
      for n in range(0,num_mva_cuts):
        sigma = effsig[sig][n]/(2.5+sqrt(totbkg[n]))
        error = errsig[sig][n]/(2.5+sqrt(totbkg[n])) - effsig[sig][n]/((2.5+sqrt(totbkg[n]))*(2.5+sqrt(totbkg[n])))
        tmparray1.append(sigma)
        tmparray2.append(error)
        sigmasigs[sig] = tmparray1
        sigersigs[sig] = tmparray2
      tmparray1 = []
      tmparray2 = []

    # --- Print to file  
    for sig in sigs: 
      self.writeFile(options,sig,effsig[sig],errsig[sig],sigmasigs[sig],sigersigs[sig],totbkg,totdat,num_mva_cuts)

# ----------------------------------------------------------------
  def writeFile(self,options,sig,eff,err,sigma,sigerr,totbkg,totdat,ncuts):  
    outfile = options.outdir+'Efficiency_MVA_'+options.mvatype+'_'+options.suffix+'_'+sig+'.txt'  
    print 'Writing efficiency to : '+outfile 
    fout = open(outfile,'w')

    fout.write("\\documentclass[a4paper,landscape]{article} \n")
    fout.write("\\usepackage[paperheight=8.0in,paperwidth=11.0in,margin=1.0in,headheight=0.0in,footskip=0.5in,includehead,includefoot]{geometry} \n")
    fout.write("\\begin{document} \n")
    fout.write("\n")
    fout.write("\\begin{table}[bthp] \n")
    fout.write("\\centering \n")
    fout.write("\\begin{tabular}{|c|c|c|c|c|} \n")
    fout.write("\\hline \n")

    fout.write("% Signal: "+sig+" \n")
    sigstr = sig.replace("_"," ")
    fout.write("\\multicolumn{5}{|c|}{ "+sigstr+" } \\\\ \n")
    fout.write("\\hline \n")
    fout.write("\\hline \n")
    fout.write("Cut & Eff & $\sigma$ & nBkg & nData \\\\   \n")
    fout.write("\\hline \n")
    fout.write("\\hline \n")
    for n in range(0,ncuts):
      cutval = -0.20 + n*0.01
      fout.write(str(cutval)+' & ')
      fout.write('%1.2f $\pm$ %1.2f & ' %(eff[n]*100,err[n]*100))
      fout.write('%1.6f $\pm$ %1.6f & ' %(sigma[n],sigerr[n]))
      fout.write(str(totbkg[n])+' & ')
      fout.write(str(totdat[n])+' \\\\ \n')
    fout.write("\\hline \n")
    fout.write("\\end{tabular} \n")
    fout.write("\\end{table} \n")
    fout.write("\n")
    fout.write("\\end{document} \n")

# ----------------------------------------------------------------
  def applyCut(self,tree,init):
     output = []
     eff    = [] 
     err    = []
 
     for i in range(0,40):
       # --- Setup MVA cut
       cutval = -0.20 + i*0.01
       cut = '(MVA_response > '+str(cutval)+')'
  
       # --- Open tree   
       tree.Draw('mgg>>h_tmp',cut+'*weight')
       htmp = ROOT.gDirectory.Get('h_tmp')
 
       # --- Values
       value = htmp.Integral()
       effic = value/init
       output.append(value) 
       if effic <= 0: effic = 0.0
       eff.append(effic)
       err.append(sqrt(effic*(1.0-effic)/init))

       # --- Clear
       htmp.Clear()
 
     return output,eff,err

# ----------------------------------------------------------------
if __name__ == "__main__": 
  app = PlotMaker()
  app.run()
