import os
import sys
import math
import ROOT
#from ROOT import TColor
from array import array
from optparse import OptionParser
from CMS_lumi import CMS_lumi
import plotting_interp as plot
import re
import json
import types
import numpy

def run(opts):
  
  # --- read in options
  model   = opts.model
  which   = opts.which
  outdir  = opts.outdir
  do90    = opts.do90
  dowgt   = opts.dowgt
  dosmth  = opts.dosmth
  smthfnc = opts.smthfnc
  #if dosmth: addtxt = '_smth'
 
  # --- read in files
  indir = '/eos/cms/store/group/phys_exotica/MonoHgg/MonoH-COMBO-2016/'+model+'_jsons/'
  if dowgt: wfile = ''
  else:     wfile = '_weighted'
  if do90:  indir += which+'_'+model+wfile+'_results_90CL/' 
  else:     indir += which+'_'+model+wfile+'_results/'

  # --- options for plot averaging
  doFillAvgLow   = True  # do averaging below mMed = 2*mDM line
  doFillAvgHigh  = True  # do averaging above mMed = 2*mDM line
  if model=="2HDM": doFillAvgLow  = False
  if model=="2HDM": doFillAvgHigh = False
  doFillAvgRest  = True  # do averaging at line or average normally
  doFillAvgAll   = True  # do averaging for all plots not just observed

  # --- setup general style 
  ROOT.gROOT.SetBatch(ROOT.kTRUE)
  ROOT.gStyle.SetOptStat(0)
  plot.ModTDRStyle()

  canv = ROOT.TCanvas()
  canv.SetLogz()
  canv.SetTicks()
  canv.SetRightMargin(0.16) # allow enough space for z axis label
  canv.cd()

  # --- setup palette 
  ROOT.gStyle.SetPalette(57)          # palette normal
  InvertPalette()                     # palette inverted
  ROOT.gStyle.SetNumberContours(255)

  A=[]; Z=[]
  # --- mass points
  if model=="2HDM":
    A=[300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675]
    Z=[450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950]
  if model=="BARY":
    A=[1,35,100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,875,900,925,950,975,1000]
    Z=[10,50,100,200,250,300,350,400,450,500,550,600,675,750,800,850,900,950,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000]
  if model=="BARY" and which=='combo':
    A=[1,35,100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700,725,750,775,800,825,850,900,925,950,975,1000]
    Z=[10,50,100,200,250,300,350,400,450,500,550,600,650,700,900,950,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000]

  # --- binning for BARY model
  # Y axis
  BinningA = [0.5,1.5] 
  BinAAxis = [1.0,47.5]
  for i in range(1, len(A)-1):
      BinningA.append( (A[i] + A[i+1])/2.0 )
      BinAAxis.append( (A[i] + A[i+1])/2.0 )
  BinningA.append( (A[-1] + A[-1] - ((A[-1] + A[-2])/2.0)) )
  BinAAxis.append( (A[-1] + A[-1] - ((A[-1] + A[-2])/2.0)) )
  # X axis
  BinningZ = [9,11]
  BinZAxis = [10,75]
  for i in range(1, len(Z)-1):
      BinningZ.append( (Z[i] + Z[i+1])/2.0 )
      BinZAxis.append( (Z[i] + Z[i+1])/2.0 )
  BinningZ.append( (Z[-1] + Z[-1] - ((Z[-1] + Z[-2])/2.0)) )
  BinZAxis.append( (Z[-1] + Z[-1] - ((Z[-1] + Z[-2])/2.0)) )

  # --- setup histograms (different models have different binning)
  if model=="2HDM":
    limitPlotAxis  = ROOT.TH2F("lplotAxis",  "lplotAxis",  len(Z), Z[0],    Z[-1]+50, len(A), A[0],      A[-1]+25)
    limitPlot      = ROOT.TH2F("lplot",      "lplot",      len(Z), Z[0]-25, Z[-1]+50, len(A), A[0]-12.5, A[-1]+25)
    limitPlotObs   = ROOT.TH2F("lplotObs",   "lplotObs",   len(Z), Z[0]-25, Z[-1]+50, len(A), A[0]-12.5, A[-1]+25)
    limitPlotUp    = ROOT.TH2F("lplotU",     "lplotU",     len(Z), Z[0]-25, Z[-1]+50, len(A), A[0]-12.5, A[-1]+25)
    limitPlotDown  = ROOT.TH2F("lplotDown",  "lplotDown",  len(Z), Z[0]-25, Z[-1]+50, len(A), A[0]-12.5, A[-1]+25)
    limitPlotUp2   = ROOT.TH2F("lplotU2",    "lplotU2",    len(Z), Z[0]-25, Z[-1]+50, len(A), A[0]-12.5, A[-1]+25)
    limitPlotDown2 = ROOT.TH2F("lplotDown2", "lplotDown2", len(Z), Z[0]-25, Z[-1]+50, len(A), A[0]-12.5, A[-1]+25)
  if model=="BARY": # variable binning
    limitPlotAxis  = ROOT.TH2F("lplotAxis",  "lplotAxis",  len(Z)-1,        array('d',BinZAxis), len(A)-1,        array('d',BinAAxis))
    limitPlot      = ROOT.TH2F("lplot",      "lplot",      len(BinningZ)-1, array('d',BinningZ), len(BinningA)-1, array('d',BinningA))
    limitPlotObs   = ROOT.TH2F("lplotObs",   "lplotObs",   len(BinningZ)-1, array('d',BinningZ), len(BinningA)-1, array('d',BinningA))
    limitPlotUp    = ROOT.TH2F("lplotU",     "lplotU",     len(BinningZ)-1, array('d',BinningZ), len(BinningA)-1, array('d',BinningA))
    limitPlotDown  = ROOT.TH2F("lplotDown",  "lplotDown",  len(BinningZ)-1, array('d',BinningZ), len(BinningA)-1, array('d',BinningA))
    limitPlotUp2   = ROOT.TH2F("lplotU2",    "lplotU2",    len(BinningZ)-1, array('d',BinningZ), len(BinningA)-1, array('d',BinningA))
    limitPlotDown2 = ROOT.TH2F("lplotDown2", "lplotDown2", len(BinningZ)-1, array('d',BinningZ), len(BinningA)-1, array('d',BinningA))

  # --- read in json files
  for a in A:
    for z in Z:
      data = {}
      filename = indir+'Zprime'+str(z)+'A'+str(a)+'.json'
      if which=='gg' and model=='BARY': # BARY gg ONLY has DM instead of A in filename 
        filename = indir+'Zprime'+str(z)+'DM'+str(a)+'.json' 
      scale = 1.
      if dowgt: scale = scaleXS(model,z,a)
      if os.path.isfile(filename) and scale != "99999":
         with open(filename) as jsonfile:
          data = json.load(jsonfile)
          for key in data: # fill plots from json
            limitPlot.SetBinContent(limitPlot.GetXaxis().FindBin(float(z)),limitPlot.GetYaxis().FindBin(float(a)),float(scale)*data[key][u'exp0'])
            limitPlotUp.SetBinContent(limitPlotUp.GetXaxis().FindBin(float(z)),limitPlot.GetYaxis().FindBin(float(a)),float(scale)*data[key][u'exp+1'])
            limitPlotDown.SetBinContent(limitPlotDown.GetXaxis().FindBin(float(z)),limitPlot.GetYaxis().FindBin(float(a)),float(scale)*data[key][u'exp-1'])
            limitPlotUp2.SetBinContent(limitPlotUp2.GetXaxis().FindBin(float(z)),limitPlot.GetYaxis().FindBin(float(a)),float(scale)*data[key][u'exp+2'])
            limitPlotDown2.SetBinContent(limitPlotDown2.GetXaxis().FindBin(float(z)),limitPlot.GetYaxis().FindBin(float(a)),float(scale)*data[key][u'exp-2'])
            limitPlotObs.SetBinContent(limitPlotObs.GetXaxis().FindBin(float(z)),limitPlot.GetYaxis().FindBin(float(a)),float(scale)*data[key][u'obs'])

  # --- average plots to make smooth contours
  fillAvg(limitPlotObs, A, Z, doFillAvgLow, False,  False)
  fillAvg(limitPlotObs, A, Z, False, doFillAvgHigh, False)
  fillAvg(limitPlotObs, A, Z, False, False, doFillAvgRest)
  if doFillAvgAll: 
    fillAvg(limitPlot,      A, Z, doFillAvgLow, False,  False)
    fillAvg(limitPlotUp,    A, Z, doFillAvgLow, False,  False)
    fillAvg(limitPlotDown,  A, Z, doFillAvgLow, False,  False)
    fillAvg(limitPlotUp2,   A, Z, doFillAvgLow, False,  False)
    fillAvg(limitPlotDown2, A, Z, doFillAvgLow, False,  False)
    fillAvg(limitPlot,      A, Z, False, doFillAvgHigh, False)
    fillAvg(limitPlotUp,    A, Z, False, doFillAvgHigh, False)
    fillAvg(limitPlotDown,  A, Z, False, doFillAvgHigh, False)
    fillAvg(limitPlotUp2,   A, Z, False, doFillAvgHigh, False)
    fillAvg(limitPlotDown2, A, Z, False, doFillAvgHigh, False)
    fillAvg(limitPlot,      A, Z, False, False, doFillAvgRest)
    fillAvg(limitPlotUp,    A, Z, False, False, doFillAvgRest)
    fillAvg(limitPlotDown,  A, Z, False, False, doFillAvgRest)
    fillAvg(limitPlotUp2,   A, Z, False, False, doFillAvgRest)
    fillAvg(limitPlotDown2, A, Z, False, False, doFillAvgRest)

  # --- axis labels 
  limitPlotAxis.GetXaxis().SetTitle("m_{Z'} [GeV]")
  limitPlotObs.GetZaxis().SetTitle("#sigma_{95% CL}/#sigma_{th}")
  if model=="2HDM": limitPlotAxis.GetYaxis().SetTitle("m_{A} [GeV]")
  if model=="BARY": limitPlotAxis.GetYaxis().SetTitle("m_{DM} [GeV]")

  # --- clone obs to get contour 
  limitPlotObsCopy = limitPlotObs.Clone()

  # --- set up min and max of z axis
  limitPlotObs.SetMaximum(100)
  limitPlotObs.SetMinimum(0.3)
  # --- set range of x and y axis
  if model=="BARY": limitPlotObs.GetXaxis().SetRangeUser(10,2001)
  if model=="BARY": limitPlotObs.GetYaxis().SetRangeUser(1,1001)
  if model=="2HDM": limitPlotObs.GetXaxis().SetRangeUser(450,2000)
  if model=="2HDM": limitPlotObs.GetYaxis().SetRangeUser(300,700)

  # --- style plot
  limitPlotObs.GetYaxis().SetTitleOffset(0.95) # format axis labels 
  limitPlotObs.GetZaxis().SetTitleOffset(0.95) 
  limitPlotObs.GetXaxis().SetLabelSize(0.035) # format axis ticks
  limitPlotObs.GetYaxis().SetLabelSize(0.035)
  if model=="2HDM": limitPlotAxis.GetXaxis().SetNdivisions(9)
  if model=="2HDM": limitPlotAxis.GetYaxis().SetNdivisions(8)
  if model=="BARY": limitPlotAxis.GetXaxis().SetNdivisions(10)
  if model=="BARY": limitPlotAxis.GetYaxis().SetNdivisions(16)

  # --- smooth 
  if dosmth:
    limitPlot.GetXaxis().SetRange(3,limitPlot.GetNbinsX())
    limitPlot.Smooth(1,smthfnc)
    limitPlotObsCopy.GetXaxis().SetRange(3,limitPlotObsCopy.GetNbinsX())
    limitPlotObsCopy.Smooth(1,smthfnc)
    limitPlotUp.GetXaxis().SetRange(3,limitPlotUp.GetNbinsX())
    limitPlotUp.Smooth(1,smthfnc)
    limitPlotDown.GetXaxis().SetRange(3,limitPlotDown.GetNbinsX())
    limitPlotDown.Smooth(1,smthfnc)
    limitPlot.GetXaxis().SetRange(0,limitPlot.GetNbinsX())
    limitPlotObsCopy.GetXaxis().SetRange(0,limitPlotObsCopy.GetNbinsX())
    limitPlotUp.GetXaxis().SetRange(0,limitPlotUp.GetNbinsX())
    limitPlotDown.GetXaxis().SetRange(0,limitPlotDown.GetNbinsX())
    #limitPlot.Smooth(1,smthfnc)
    #limitPlotObsCopy.Smooth(1,smthfnc)
    #limitPlotUp.Smooth(1,smthfnc)
    #limitPlotDown.Smooth(1,smthfnc)
 
  # --- get and style each contour
  # 1 sigma up 
  limitPlotUp.SetMinimum(1)
  limitPlotUp.SetContour(1)
  limitPlotUp.SetLineWidth(1)
  # 1 sigma down
  limitPlotDown.SetMinimum(1)
  limitPlotDown.SetContour(1)
  limitPlotDown.SetLineWidth(1)
  # observed
  limitPlotObs.SetLineWidth(3)
  limitPlotObs.SetLineColor(2)
  limitPlotObsCopy.SetMinimum(1)
  limitPlotObsCopy.SetContour(1)
  limitPlotObsCopy.SetLineWidth(3)
  limitPlotObsCopy.SetLineColor(2)
  # expected
  limitPlot.SetMinimum(1)
  limitPlot.SetContour(1)
  limitPlot.SetLineStyle(7)
  limitPlot.SetLineWidth(3)

  # --- draw plots
  limitPlotAxis.Draw("COLZ") 
  limitPlotObs.Draw("COLZ SAME")
  limitPlotUp.Draw("CONT3 SAME")
  limitPlotDown.Draw("CONT3 SAME")
  limitPlotObsCopy.Draw("CONT3 SAME")
  limitPlot.Draw("CONT3 SAME")

  # --- legend and extra text box
  x1 = 0.18
  y1 = 0.55
  x2 = x1+0.42
  y2 = y1+0.35
  # --- add white box below bary
  white = ROOT.TPaveText(x1,y1,x2,y2,"NDC")
  white.AddText("")
  white.SetFillColor(0)
  white.Draw("SAME")
  # --- latex
  if model=="2HDM":  txt1 = "#bf{Z'-2HDM}"
  if model=="BARY":  txt1 = "#bf{Baryonic Z'}"
  txt1 += "#bf{, Z' #rightarrow DM + h"
  if which=='gg':    txt1 += "(#gamma#gamma)}"
  if which=='tt':    txt1 += "(#tau#tau)} "
  if which=='combo': txt1 += "(#gamma#gamma + #tau#tau)}"
  if model=="2HDM":  txt2 = "#bf{Dirac DM, m_{DM} = 100 GeV}"
  if model=="BARY":  txt2 = "#bf{Dirac DM, g_{q} = 0.25, g_{DM} = 1.0 }"
  if model=="2HDM":  txt3 = "#bf{g_{Z'} = 0.8, g_{DM} = 1.0}"
  if model=="BARY":  txt3 = "" 
  txt = ROOT.TPaveText(x1,y1+0.15,x2,y2,"NDC")
  txt.AddText(txt1)
  txt.AddText(txt2)
  txt.AddText(txt3)
  txt.SetTextAlign(12)
  txt.SetTextSize(0.04)
  txt.Draw("SAME")
  # --- legend
  if model=="2HDM": leg = ROOT.TLegend(x1,y1,x2,y1+0.15)
  if model=="BARY": leg = ROOT.TLegend(x1,y1+0.05,x2,y1+0.2)
  leg.SetBorderSize(0)
  leg.SetTextFont(42)
  leg.SetTextSize(0.040)
  leg.AddEntry(limitPlotObs,"Observed 95% CL","L")
  leg.AddEntry(limitPlot,"Expected 95% CL","L")
  leg.AddEntry(limitPlotUp,"#pm 1 s.d.","L")
  leg.Draw()
  
  canv.cd()
  canv.Update()
  CMS_lumi(canv,4,0)
  canv.RedrawAxis()
  canv.Update()
  
  # --- save
  outname = outdir+'contours_'
  if do90: outname += '90CL'
  else:    outname += '95CL'
  outname += '_'+model+'_'+which+'.root'
  outfile = ROOT.TFile(outname,'RECREATE')
  outfile.cd()
  limitPlot.Write()
  limitPlotObs.Write()
  if do90:
    canv.Print(outdir+'limits2D_'+model+'_'+which+'_90CL.pdf')
    canv.Print(outdir+'limits2D_'+model+'_'+which+'_90CL.png')
  else: 
    canv.Print(outdir+'limits2D_'+model+'_'+which+'.pdf')
    canv.Print(outdir+'limits2D_'+model+'_'+which+'.png')

def fillAvg(limitPlot,A,Z,doL,doH,doR):
  # --- ordering for each option
  irange = range(1,limitPlot.GetNbinsY()+1)
  jrange = range(1,limitPlot.GetNbinsX()+1)
  if doL: jrange = list(reversed(range(1,limitPlot.GetNbinsX()+1)))
  if doH: irange = list(reversed(range(1,limitPlot.GetNbinsY()+1)))

  # --- average over 4 adjacent bins
  for i in irange:
    for j in jrange:
      aVal = A[i-1]
      zVal = Z[j-1]
      binVal = str(limitPlot.GetBinContent(j,i))
      # --- only if bin is 0 do averaging
      if binVal == "0.0" and ((doL and 2*float(aVal) < float(zVal)) or (doH and 2*float(aVal) > float(zVal)) or (doR)):
        avg = 0.0
        div = 0.0
        back = limitPlot.GetBinContent(j-1,i)
        forw = limitPlot.GetBinContent(j+1,i)
        down = limitPlot.GetBinContent(j,i-1)
        abov = limitPlot.GetBinContent(j,i+1)
        if back != 0.0 and ((doL and back < 50.) or (doH and back > 50.) or (doR)):
          avg += back
          div += 1
        if forw != 0.0 and ((doL and forw < 50.) or (doH and forw > 50.) or (doR)):
          avg += forw 
          div += 1
        if down != 0.0 and ((doL and down < 50.) or (doH and down > 50.) or (doR)):
          avg += down
          div += 1
        if abov != 0.0 and ((doL and abov < 50.) or (doH and abov > 50.) or (doR)):
          avg += abov
          div += 1
        if div != 0:
          avg = avg/div
          limitPlot.SetBinContent(j,i,avg)

def InvertPalette():
   # --- Function to make inverted kBird palette
   alpha=1
   stops = array('d', ([ 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000]) )
   red   = array('d', ([ 0.2082, 0.0592, 0.0780, 0.0232, 0.1802, 0.5301, 0.8186, 0.9956, 0.9764]) )
   green = array('d', ([ 0.1664, 0.3599, 0.5041, 0.6419, 0.7178, 0.7492, 0.7328, 0.7862, 0.9832]) )
   blue  = array('d', ([ 0.5293, 0.8684, 0.8385, 0.7914, 0.6425, 0.4662, 0.3499, 0.1968, 0.0539]) )
   invred   = numpy.asarray(list(reversed(red)))
   invgreen = numpy.asarray(list(reversed(green)))
   invblue  = numpy.asarray(list(reversed(blue)))
   ROOT.TColor.CreateGradientColorTable(9, stops, invred, invgreen, invblue, 255, alpha);

def scaleXS(model,Z,A):
    # --- Function to scale point by 1/xsec
    if model=="2HDM": xsRef = open("crosssectionZp2HDM.txt")
    if model=="BARY": xsRef = open("crosssectionZpBaryonic.txt")
    returnString = "99999"
    for line in xsRef:
        if (str(line.split(' ')[0]) == str(Z) and str(line.split(' ')[1]) == str(A)):
            returnString = str(1./float(line.split(' ')[2]))
            print returnString
    return returnString

def init():

  # options
  parser = OptionParser("usage: %prog [options]")
  parser.add_option("-O",action="store",dest="outdir",type="string",
                    default="",help="Output directory [default = %default]"),
  parser.add_option("-m",action="store",dest="model",type="string",
                    default="",help="Which model (2HDM or BARY)"),
  parser.add_option("-w",action="store",dest="which",type="string",
                    default="",help="Which channel (gg, tt, combo)"),
  parser.add_option("--dowgt",action="store_true",dest="dowgt",
                    default=False,help="Weight by 1/xsec (if not already done) [default = %default]"),
  parser.add_option("--do90",action="store_true",dest="do90",
                    default=False,help="Store 90%CL root file [default = %default]"),
  parser.add_option("--dosmooth",action="store_true",dest="dosmth",
                    default=False,help="Smooth TH2 after filling and avg. [default = %default]"),
  parser.add_option("--smth",action="store",dest="smthfnc",type="string",
                    default="k5a",help="Smoothing function to apply [default = %default]"),
  (options, args) = parser.parse_args()

  if options.model!="2HDM" and options.model!="BARY": 
    print "Model "+options.model+" is NOT a valid option." 
    sys.exit()
  if options.which!="gg" and options.which!="tt" and options.which!="combo":
    print "Channel "+options.which+" is NOT a valid option."
    sys.exit()

  #run
  print "Making 2D limit plot for: "+options.model+" "+options.which
  if options.do90:   print "Using 90CL limits"
  if options.dowgt:  print "Weighting by 1/xsec"
  if options.dosmth: print "Smoothing applied"
  run(options)

if __name__=="__main__":
  init()
