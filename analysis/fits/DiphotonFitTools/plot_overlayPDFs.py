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
  parser.add_option("--signame",action="store",dest="signame",type="string",
          default="ZpBaryonic_mZP10_mChi1",help="Sig name"),
  parser.add_option("--dir1",action="store",type="string",
          default="/afs/cern.ch/work/m/mzientek/private/FitDirectory/CMSSW_8_0_28/src/diphotons/Analysis/macros/ntuples4fit_cic_default_shapes_lumi_35.9/",
          help="Path to dir1 to compare"),
  parser.add_option("-O",action="store",dest="outdir",type="string",
          default="~/www/Plots/13TeV_v80X_moriond17/FitResults/ntuples4fit/ntuples4fit_cic_default_shapes_lumi_35.9/",
          help="Outdir default = %default"),
  parser.add_option("--label",action="store",dest="label",type="string",
          default="",help="Additional label"),
  parser.add_option("--data",action="store_true",dest="do_dat",
          default=False,help="Compare data"),
  parser.add_option("--scale",action="store_true",dest="do_scale",
          default=False,help="Compare data w/ Scale up&down"),
  parser.add_option("--bin",action="store_true",dest="do_bin",
          default=False,help="Compare binned data"),
  parser.add_option("--bkg",action="store_true",dest="do_bkg",
          default=False,help="Compare bkg"),
  parser.add_option("--sig",action="store_true",dest="do_sig",
          default=False,help="Compare sig"),
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
  if options.do_dat:   items.append("dat")
  if options.do_bin:   items.append("bin")
  if options.do_bkg:   items.append("bkg")
  if options.do_sig:   items.append("sig")
  if options.do_ggh:   items.append("ggh")
  if options.do_vbf:   items.append("vbf")
  if options.do_tth:   items.append("tth")
  if options.do_vh :   items.append("vh")
  if options.do_scale:
    items.append("scup")
    items.append("scdo")
  print items

  # setup names
  names = {}
  names["dat"]  = "Data"
  names["bin"]  = "Data"
  names["bkg"]  = "Non-res Bkg" 
  names["sig"]  = "Signal"
  names["ggh"]  = "GGH"
  names["vbf"]  = "VBF"
  names["tth"]  = "ttH"
  names["vh"]   = "Vh"
  names["scup"] = "Scale Up"
  names["scdo"] = "Scale Down"

  # setup colors
  color = {}
  color["dat"]  = ROOT.kBlack
  color["bin"]  = ROOT.kBlack
  color["bkg"]  = ROOT.kBlue 
  color["sig"]  = ROOT.kRed
  color["ggh"]  = ROOT.kMagenta
  color["vbf"]  = ROOT.kCyan
  color["tth"]  = ROOT.kGreen
  color["vh"]   = ROOT.kBlue
  if options.do_scale: 
    color[items[0]] = ROOT.kBlack
  color["scup"] = ROOT.kRed
  color["scdo"] = ROOT.kMagenta

  # setup files
  path1 = options.dir1 
  filename = {} 
  bkgname1 = "ntuples4fit_cic_default_shapes_lumi_35.9_MonoHgg"
  filename["dat"]  = bkgname1
  filename["bin"]  = bkgname1
  filename["bkg"]  = bkgname1
  filename["sig"]  = "sig_"+options.signame+"_13TeV"
  filename["ggh"]  = "GluGluHToGG_13TeV"
  filename["vbf"]  = "VBFHToGG_13TeV"
  filename["tth"]  = "ttHJetToGG_13TeV"
  filename["vh"]   = "VHToGG_13TeV"
  filename["scup"] = filename[items[0]] 
  filename["scdo"] = filename[items[0]]
  files1 = {}
  for item in items:
    fname1 = path1+filename[item]+".root"
    #if item=="bkg" or item=="dat" or item=="bin": fname1 = path1+bkgname1+".root"
    #else: fname1 = path1+filename[item]+".root"
    #print("fname1: %s"%fname1)
    files1[item] = ROOT.TFile(fname1)

  # get workspaces
  workspaces1 = {}
  for item in items:
    workspaces1[item] = files1[item].Get("wtemplates")#ROOT.RooWorkspace(files1[item].wtemplates)
  if workspaces1[item]==None: print"Worskpace 1 NOT found"
 
  # which met cat
  cat = "_met130"
  if options.doLowMET: cat = "_met0-130"

  # get pdfs
  pdfnames = {}
  pdfnames["dat"]  = "data"+cat
  pdfnames["bin"]  = "binned_data"+cat
  pdfnames["bkg"]  = "model_bkg"+cat
  pdfnames["sig"]  = "model_signal_"+filename["sig"]+cat
  pdfnames["ggh"]  = "model_higgs_"+filename["ggh"]+cat
  pdfnames["vbf"]  = "model_higgs_"+filename["vbf"]+cat
  pdfnames["tth"]  = "model_higgs_"+filename["tth"]+cat
  pdfnames["vh"]   = "model_higgs_"+filename["vh"]+cat
  pdfnames["scup"] = pdfnames[items[0]]+"_energyScale"+cat[1:]+"Up" 
  pdfnames["scdo"] = pdfnames[items[0]]+"_energyScale"+cat[1:]+"Down" 

  pdfs1 = {}
  norm1 = {}
  for item in items:
    if item=="dat" or item=="bin":
      pdfs1[item] = workspaces1[item].data(pdfnames[item])
      norm1[item] = 1.0
    elif item=="scup" or item=="scdo":
      pdfs1[item] = workspaces1[item].pdf(pdfnames[item])
      norm1[item] = workspaces1[item].var(pdfnames[items[0]]+"_norm").getValV()
    else:
      pdfs1[item] = workspaces1[item].pdf(pdfnames[item])
      norm1[item] = workspaces1[item].var(pdfnames[item]+"_norm").getValV()

    #print(item+" -------- norm (new): "+str(norm1[item]))
    if pdfs1[item]==None: 
      print("RooDataHist (%s) NOT found" %item)
      sys.exit(1)
 
  # canvas & frames
  c = TCanvas("c_"+item,"c_"+item,600,600)
  c.cd()
  l = TLegend(0.7,0.7,0.85,0.85)
  f = ROOT.RooPlot(workspaces1[item].var('mgg'),105,181,38)
  allitem = ""
  for item in items:
    curve1opts = [RooFit.LineColor(color[item]),RooFit.Normalization(norm1[item],ROOT.RooAbsReal.NumEvent)]
    data1opts  = [RooFit.MarkerColor(color[item]),RooFit.LineColor(color[item])]
    if item=="dat" or item=="bin": pdfs1[item].plotOn(f,*(data1opts))
    else:                          pdfs1[item].plotOn(f,*(curve1opts)) 
    l.AddEntry(f.getObject(int(f.numItems()-1)),names[item],"lpe")
    f.Draw() 
    l.Draw("same")
    allitem += item
    allitem += "_" 

  print("Outfile: %sdrawPDFs_%s%s%s.png"%(options.outdir,options.label,allitem,cat))
  c.SaveAs(options.outdir+"drawPDFs_"+options.label+allitem+cat+".png")
  c.SaveAs(options.outdir+"drawPDFs_"+options.label+allitem+cat+".pdf")

if __name__ == "__main__":
  print("Running...")
  comparePDF()
  print("Done")
