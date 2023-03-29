import sys
import os
import numpy
import ROOT
from ROOT import *
from ROOT import TObject 
from ROOT import RooWorkspace
from ROOT import RooRealVar
from ROOT import RooArgList
from ROOT import RooArgSet
from ROOT import RooAbsData
from ROOT import RooCmdArg
from ROOT import RooDataSet
from ROOT import RooDataHist
from ROOT import RooAbsReal
from optparse import OptionParser, make_option

def comparePDF():

  # option list
  parser = OptionParser("usage: %prog [options]")
  parser.add_option("--lowMET",action="store_true",dest="doLowMET",
          default=False,help="Run lowMET"),
  parser.add_option("--label",action="store",dest="label",type="string",
          default="",help="Additional label"),
  parser.add_option("--signame",action="store",dest="signame",type="string",
          default="ZpBaryonic_mZP10_mChi1",help="Sig name"),
  parser.add_option("--dir1",action="store",type="string",
          default="/afs/cern.ch/work/m/mzientek/private/FitDirectory/CMSSW_8_0_28/src/diphotons/Analysis/macros/ntuples4fit_cic_default_shapes_lumi_35.9/",
          help="Path to dir1 to compare"),
  parser.add_option("--dir2",action="store",type="string",
          default="~soffi/public/4Margaret/ntuples4fit_pho_allMC_woLepVetos_met50_met130_cic_default_shapes_lumi_35.9_PARAMETRIC/",
          help="Path to dir2 to compare"),
  parser.add_option("-O",action="store",dest="outdir",type="string",
          default="~/www/Plots/13TeV_v80X_moriond17/FitResults/ntuples4fit/ntuples4fit_cic_default_shapes_lumi_35.9/",
          help="Outdir default = %default"),
  parser.add_option("--data",action="store_true",dest="do_dat",
          default=False,help="Compare data"),
  parser.add_option("--bin",action="store_true",dest="do_bin",
          default=False,help="Compare binned data"),
  parser.add_option("--bkg",action="store_true",dest="do_bkg",
          default=True,help="Compare bkg"),
  parser.add_option("--sig",action="store_true",dest="do_sig",
          default=True,help="Compare sig"),
  parser.add_option("--ggh",action="store_true",dest="do_ggh",
          default=False,help="Compare ggh"),
  parser.add_option("--vbf",action="store_true",dest="do_vbf",
          default=False,help="Compare vbf"),
  parser.add_option("--tth",action="store_true",dest="do_tth",
          default=False,help="Compare tth"),
  parser.add_option("--vh",action="store_true",dest="do_vh",
          default=False,help="Compare vh"),

  # end option list 
  (options, args) = parser.parse_args()

  # setup label
  if options.label!="": options.label = options.label+"_"

  # which PDFs to compare
  items = []
  if options.do_dat: items.append("dat")
  if options.do_bin: items.append("bin")
  if options.do_bkg: items.append("bkg")
  if options.do_sig: items.append("sig")
  if options.do_ggh: items.append("ggh")
  if options.do_vbf: items.append("vbf")
  if options.do_tth: items.append("tth")
  if options.do_vh : items.append("vh")

  # setup files
  path1 = options.dir1 
  path2 = options.dir2
  filename = {} 
  bkgname1 = "ntuples4fit_cic_default_shapes_lumi_35.9_MonoHgg"
  bkgname2 = "ntuples4fit_pho_allMC_woLepVetos_met50_met130_cic_default_shapes_lumi_35.9_MonoHgg"
  filename["dat"] = bkgname1
  filename["bin"] = bkgname1
  filename["bkg"] = bkgname1
  filename["sig"] = "sig_"+options.signame+"_13TeV"
  filename["ggh"] = "GluGluHToGG_13TeV"
  filename["vbf"] = "VBFHToGG_13TeV"
  filename["tth"] = "ttHJetToGG_13TeV"
  filename["vh"]  = "VHToGG_13TeV"
  files1 = {}
  files2 = {}
  for item in items:
    if item=="bkg" or item=="dat" or item=="bin": 
      fname1 = path1+bkgname1+".root"
      fname2 = path2+bkgname2+".root"
    else: 
      fname1 = path1+filename[item]+".root"
      fname2 = path2+filename[item]+".root"
    #print("fname1: %s"%fname1)
    #print("fname2: %s"%fname2)
    files1[item] = ROOT.TFile(fname1)
    files2[item] = ROOT.TFile(fname2)

  # get workspaces
  workspaces1 = {}
  workspaces2 = {}
  for item in items:
    workspaces1[item] = files1[item].Get("wtemplates")#ROOT.RooWorkspace(files1[item].wtemplates)
    workspaces2[item] = files1[item].Get("wtemplates")#ROOT.RooWorkspace(files2[item].wtemplates)
 
  if workspaces1[item]==None: print"Worskpace 1 NOT found"
  if workspaces2[item]==None: print"Workspace 2 NOT found"
 
  # get pdfs
  pdfnames = {}
  pdfnames["dat"] = "data"
  pdfnames["bin"] = "binned_data"
  pdfnames["bkg"] = "model_bkg"
  pdfnames["sig"] = "model_signal_"+filename["sig"]
  pdfnames["ggh"] = "model_higgs_"+filename["ggh"]
  pdfnames["vbf"] = "model_higgs_"+filename["vbf"]
  pdfnames["tth"] = "model_higgs_"+filename["tth"]
  pdfnames["vh"]  = "model_higgs_"+filename["vh"]
  pdfs1 = {}
  pdfs2 = {}
  norm1 = {}
  norm2 = {}
  cat = "_met130"
  if options.doLowMET: cat = "_met0-130"
 
  for item in items:
    if item=="dat" or item=="bin":
      pdfs1[item] = workspaces1[item].data(pdfnames[item]+cat)
      pdfs2[item] = workspaces2[item].data(pdfnames[item]+cat)
      norm1[item] = 1.0
      norm2[item] = 1.0
    else:
      pdfs1[item] = workspaces1[item].pdf(pdfnames[item]+cat)
      pdfs2[item] = workspaces2[item].pdf(pdfnames[item]+cat)
      norm1[item] = workspaces1[item].var(pdfnames[item]+cat+"_norm").getValV()
      norm2[item] = workspaces2[item].var(pdfnames[item]+cat+"_norm").getValV()
    print(item+" -------- norm (new): "+str(norm1[item])+"\t v. norm (old): "+str(norm2[item]))
    if pdfs1[item]==None: 
      print"RooDataHist 1 NOT found"
      sys.exit(1)
    if pdfs2[item]==None: 
      print"RooDataHist 2 NOT found"
      sys.exit(1)
 
  # canvas & frames
  c = {}
  f = {}
  l = {}
  for item in items:
    c[item] = TCanvas("c_"+item,"c_"+item,600,600)
    c[item].cd()
    f[item] = ROOT.RooPlot(workspaces1[item].var('mgg'),105,181,38)
    l[item] = TLegend(0.7,0.7,0.85,0.85)
    curve1opts = [RooFit.LineColor(ROOT.kRed),RooFit.Normalization(norm1[item],ROOT.RooAbsReal.NumEvent)]
    curve2opts = [RooFit.LineColor(ROOT.kBlue),RooFit.Normalization(norm2[item],ROOT.RooAbsReal.NumEvent)]
    data1opts  = [RooFit.MarkerColor(ROOT.kRed),RooFit.LineColor(ROOT.kRed)]
    data2opts  = [RooFit.MarkerColor(ROOT.kBlue),RooFit.LineColor(ROOT.kBlue)]
    if item=="dat" or item=="bin":
      pdfs2[item].plotOn(f[item],*(data2opts)) # old pdf
      pdfs1[item].plotOn(f[item],*(data1opts)) # new pdf
    else:
      pdfs2[item].plotOn(f[item],*(curve2opts)) # old pdf
      pdfs1[item].plotOn(f[item],*(curve1opts)) # new pdf
    l[item].AddEntry(f[item].getObject(int(f[item].numItems()-2)),"Old fit","lpe")
    l[item].AddEntry(f[item].getObject(int(f[item].numItems()-1)),"New fit","lpe")
    f[item].Draw() 
    l[item].Draw("same")
    print("Outfile: %scomparePDFs_%s%s%s.png"%(options.outdir,options.label,item,cat))
    c[item].SaveAs(options.outdir+"comparePDFs_"+options.label+item+cat+".png")
    c[item].SaveAs(options.outdir+"comparePDFs_"+options.label+item+cat+".pdf")

if __name__ == "__main__":
  print("Running...")
  comparePDF()
  print("Done")
