import os
from ROOT import *
from dep.pyapp import *
from dep.util import *
from optparse import OptionParser, make_option

class PlotMaker(pyapp):

  def __init__(self):
    super(PlotMaker,self).__init__(option_list =[
      make_option("--doblind",action="store",dest="doblind",
                  default=True,help="Run plots blinded [default = %default]"),
      make_option("--suffix",action="store",dest="suffix",type="string",
                  default="",help="Additional suffix [default = %default]"),
      ])

  global ROOT
  import ROOT
  from ROOT import gDirectory
  ROOT.gStyle.SetOptStat(0)
  ROOT.gStyle.SetOptTitle(0)

  def __call__(self,options,args):
    # --- Setup channels to run
    channels = []
    channels.append("Data2016")
    channels.append("Data2017")

    # --- Variables to compare
    var = []
    var.append("nvtx")
    var.append("pt1")
    var.append("pt2")
    var.append("eta1")
    var.append("eta2")
    var.append("phi1")
    var.append("phi2")
    var.append("r91")
    var.append("r92")
    var.append("chiso1")
    var.append("chiso2")
    var.append("neuiso1")
    var.append("neuiso2")
    var.append("phoiso1")
    var.append("phoiso2")
    var.append("sieie1")
    var.append("sieie2")
    var.append("hoe1")
    var.append("hoe2")
    var.append("mgg")
    var.append("ptgg")
    var.append("t1pfmetCorr")

    # --- Histogram format
    hform = {}
    #                   nbins,min,max
    hform["nvtx"]        = "80,0,80"
    hform["pt1"]         = "60,0,600"
    hform["pt2"]         = "60,0,600"
    hform["ptgg"]        = "60,0,600"
    hform["mgg"]         = "38,105,181"
    hform["phi1"]        = "20,-4,4"
    hform["phi2"]        = "20,-4,4"
    hform["eta1"]        = "20,-3,3"
    hform["eta2"]        = "20,-3,3"
    hform["r91"]         = "50,0,1.1"
    hform["r92"]         = "50,0,1.1"
    hform["hoe1"]        = "25,0.,0.025"
    hform["hoe2"]        = "25,0.,0.025"
    hform["sieie1"]      = "50,0.,0.03"
    hform["sieie2"]      = "50,0.,0.03"
    hform["chiso1"]      = "40,-5,15"
    hform["chiso2"]      = "40,-5,15"
    hform["neuiso1"]     = "40,-5,15"
    hform["neuiso2"]     = "40,-5,15"
    hform["phoiso1"]     = "40,-5,15"
    hform["phoiso2"]     = "40,-5,15"
    hform["t1pfmetCorr"] = "70,0,350"

    vname = {}
    vname["nvtx"]        = "# Vertices"
    vname["pt1"]         = "p_{T,#gamma1} [GeV]"
    vname["pt2"]         = "p_{T,#gamma2} [GeV]"
    vname["ptgg"]        = "p_{T,#gamma#gamma} [GeV]"
    vname["mgg"]         = "m_{#gamma#gamma} [GeV]"
    vname["phi1"]        = "#phi_{#gamma1}"
    vname["phi2"]        = "#phi_{#gamma2}"
    vname["eta1"]        = "#eta_{1}"
    vname["eta2"]        = "#eta_{2}"
    vname["r91"]         = "r9_{1}"
    vname["r92"]         = "r9_{2}"
    vname["hoe1"]        = "H/E (#gamma1)"
    vname["hoe2"]        = "H/E (#gamma2)"
    vname["sieie1"]      = "#sigma_{i#eta i#eta} (#gamma1)"
    vname["sieie2"]      = "#sigma_{i#eta i#eta} (#gamma2)"
    vname["chiso1"]      = "Charged Hadron Iso (#gamma1)"
    vname["chiso2"]      = "Charged Hadron Iso (#gamma2)"
    vname["neuiso1"]     = "Neutral Hadron Iso (#gamma1)"
    vname["neuiso2"]     = "Neutral Hadron Iso (#gamma2)"
    vname["phoiso1"]     = "Photon Iso (#gamma1)"
    vname["phoiso2"]     = "Photon Iso (#gamma2)"
    vname["t1pfmetCorr"] = "Type-1 PF corrected E_{T}^{miss} [GeV]"


    # --- Path to input files
    filenames = {}
    filenames["Data2017"] = "/afs/cern.ch/work/m/mzientek/public/Samples17_92x_v0/DoubleEG.root"
    filenames["Data2016"] = "/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/DoubleEG.root" 
    files = {}
    trees = {}
    for channel in channels:
      files[channel] = TFile(filenames[channel])
      trees[channel] = files[channel].Get("DiPhotonTree") 

    # --- Plot for each variable
    for v in var: self.plotVar(options,args,v,channels,trees,hform[v],vname[v])


  def plotVar(self,opts,args,var,channels,trees,hbins,vname):
    print("Making plot of : %s" %var)

    # --- Blinding
    cut = "t1pfmetCorr >= 0" # all
    if opts.doblind==True and var=="t1pfmetCorr": cut = "t1pfmetCorr < 100"
    if opts.doblind==True and var=="mgg":         cut = "mgg < 115 || mgg > 135"

    # --- Pickup histo format
    form = hbins.split(",")
    nbin = int(form[0])
    xmin = float(form[1])
    xmax = float(form[2])  

    # --- Get histo
    hist = {}
    for channel in channels:
      histname = "h_"+channel+"_"+var
      hist[channel] = ROOT.TH1F(histname,histname,nbin,xmin,xmax)
      if channel=="Data2017" and opts.doblind: 
        trees[channel].Draw(var+">>"+histname,"("+cut+")*weight") # apply blinding
      else:  
        trees[channel].Draw(var+">>"+histname,"weight")           # no blinding 
      hist[channel] = ROOT.gDirectory.Get(histname) 

    # --- Style plots
    names = {}
    names["Data2017"] = "Data (2017) 35.9/fb"
    names["Data2016"] = "Data (2016) 35.9/fb"
    color = {}
    color["Data2017"] = kAzure+2
    color["Data2016"] = kBlack
    style = {}
    style["Data2017"] = 21
    style["Data2016"] = 20 
 
    # --- Plot
    c = ROOT.TCanvas("c"+var,"",600,600)
    c.cd()
    c.SetLogy(1)
        
    for channel in channels:
      hist[channel].GetXaxis().SetTitle(vname)
      hist[channel].SetLineColor(color[channel])
      hist[channel].SetMarkerColor(color[channel])
      hist[channel].SetMarkerStyle(style[channel])
      if channel == channels[0]: hist[channel].Draw("PEL")
      else:                      hist[channel].Draw("PEL SAME")      

    # --- Latex
    texts = []
    texts.append(add_text(0.15,0.4,0.89,0.99,"#bf{CMS}  Preliminary"))

    # --- Legend
    l = ROOT.TLegend(0.55,0.80,0.80,0.88,"","brNDC")
    l.SetBorderSize(0)
    for channel in channels:
      l.AddEntry(hist[channel],names[channel],"PEL")
    l.Draw("SAME")

    # --- Save
    c.Print(opts.outdir+'plot_dataVdata_'+var+'.png')
    c.Print(opts.outdir+'plot_dataVdata_'+var+'.pdf')

# --- Call
if __name__ == "__main__":
  app = PlotMaker()
  app.run()
