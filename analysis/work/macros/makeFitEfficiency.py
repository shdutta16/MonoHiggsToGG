import os
from ROOT import *
from dep.pyapp import *
from dep.util  import *
from optparse  import OptionParser, make_option

class FitMaker(pyapp):

  def __init__(self):
    super(FitMaker,self).__init__(option_list =[
      make_option("--deg1",action="store_true",dest="fitPolDeg1",
                  default=False,help="Run fit with polynomial degree 1"),
      make_option("--deg3",action="store_true",dest="fitPolDeg3",
                  default=False,help="Run fit with polynomial degree 3"),
      make_option("--fitHist",action="store_true",dest="fitHist",
                  default=False,help="Run fit on hist (rather than TGraph) [default=%default]"),
      make_option("--lowMet",action="store_true",dest="doLowMet",
                  default=False,help="Do fit for lowMet range [default = %default]"),
      make_option("--range",action="store",dest="fitrange",
                  default=None,help="Range [default= %default]"),
      make_option("--xmin",action="store",dest="xmin",type="int",
                  default=550,help="Min X of fit [default = %default]"),
      make_option("--xmax",action="store",dest="xmax",type="int",
                  default=2550,help="Max X of fit [default = %default]"),
      make_option("--ymin",action="store",dest="ymin",type="int",
                  default=250,help="Min Y of fit [default = %default]"),
      make_option("--ymax",action="store",dest="ymax",type="int",
                  default=850,help="Max Y of fit [default = %default]"),
      make_option("--opts",action="store",dest="fitopts",type="string",
                  default="",help="Fit options [default = %default]"),
      make_option("--model",action="store",dest="model",type="string",
                  default="2HDM",help="Which model to run [default = %default]"),
      ])
 
  def __call__(self,options,args):
    
    metregion = 'HighMET'
    if (options.doLowMet): metregion = 'LowMET'
    print("Running fit for model: %s in %s region" %(options.model,metregion))

    # pick up input graphs
    filepath  = "EffandYieldsTables/Output_Eff.root"
    graphname = options.model+"_Eff_"+metregion
    infile    = TFile(filepath)
    histoeff  = infile.Get("h_"+graphname)
    histoerr  = infile.Get("h_"+graphname+"_err")
    if (options.fitHist): histofit = TH2D("histofit","",2500,500,3000,700,200,900)
    else:                 histofit = infile.Get("graph_"+graphname)

    # output graph
    outfilename = options.outdir+'Output_EfficiencyFit_'+options.model+'_'+metregion+'.root'
    print("Making output root file: %s" %outfilename)
    outfile = TFile.Open(outfilename,'RECREATE')
  
    # setup histo for fit
    if (options.fitHist):
      bincon = Double(0)
      binerr = Double(0)
      for x in range(0,histoeff.GetNbinsX()):
        for y in range(0,histoeff.GetNbinsY()):
          bincon = histoeff.GetBinContent(x,y)
          binerr = histoerr.GetBinError(x,y)
          histofit.SetBinContent(x,y,bincon)
          histofit.SetBinError(x,y,binerr) 
 
    # setup the fit
    eqn1 = "([0] + [1]*x + [2]*y)"
    eqn2 = "([0] + [1]*x + [2]*y + [3]*x*x + [4]*y*y + [5]*x*y)"
    eqn3 = "([0] + [1]*x + [2]*y + [3]*x*x + [4]*y*y + [5]*x*y + [6]*x*x*x + [7]*y*y*y + [8]*x*x*y + [9]*x*y*y)"

    if options.fitrange:
      options.fitrange = options.fitrange.split(",")
      fit_min_x = int(options.fitrange[0])
      fit_max_x = int(options.fitrange[1])
      fit_min_y = int(options.fitrange[2])
      fit_max_y = int(options.fitrange[3])
    else: 
      fit_min_x = int(options.xmin)
      fit_max_x = int(options.xmax)
      fit_min_y = int(options.ymin)
      fit_max_y = int(options.ymax)
    print("Fitting in the range: X [%i,%i] & Y[%i,%i]" %(fit_min_x,fit_max_x,fit_min_y,fit_max_y))
    if options.fitPolDeg1:   polN = TF2("polN",eqn1,fit_min_x,fit_max_x,fit_min_y,fit_max_y) 
    elif options.fitPolDeg3: polN = TF2("polN",eqn3,fit_min_x,fit_max_x,fit_min_y,fit_max_y) 
    else:                    polN = TF2("polN",eqn2,fit_min_x,fit_max_x,fit_min_y,fit_max_y) 
  
    # set parameters
    if not options.doLowMet: # high MET parameters
      polN.SetParameters(-2.17340, 9.40028e-02, -7.98430e-02, -3.82316e-05, 1.19600e-05, 4.27385e-05)
      polN.SetParameters(11.0200, 5.54198e-02, -5.38791e-02, -1.65226e-05, 2.26265e-05, 1.52306e-05)
      polN.SetParameters(7.50591, 5.56292e-02, -3.81799e-02, -1.66711e-05, 4.65969e-06, 1.58854e-05)
    else: # low MET parameters
      polN.SetParameters(2.51404e-01, -3.87093e-04, 8.38014e-04, 1.93552e-07, 2.79338e-06, -1.29031e-06)
      polN.SetParameters(6.13016e-01, -7.62181e-04, 7.34239e-04, 2.15066e-07, -1.10133e-07, -3.06113e-07)
      polN.SetParameters(8.10386e-01, -9.50248e-04, 5.52061e-04, 2.66917e-07, 1.43855e-07, -3.15942e-07)
      if options.fitPolDeg3:
        polN.SetParameters(2.65565e+00, -5.38159e-03, 4.19025e-03, 3.60743e-06, 3.74387e-06, -6.35321e-06,
                           -7.37046e-10, -1.88211e-09, 1.61315e-09, -1.49700e-10)

    # do fit
    histofit.Fit("polN",options.fitopts)

    # results  
    chi2 = polN.GetChisquare()
    print("Chi2 = %d" %chi2)
    fit = histofit.FindObject("polN") 
    #p = [1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0] 
    nparam = 6
    if options.fitPolDeg1 : nparam = 3
    if options.fitPolDeg3 : nparam = 10
    p = nparam*[1.0] 
    for i in range(0,nparam):
      p[i] = polN.GetParameter(i) 
      #print("p[%i] = %f" %(i,p[i])) 

    # final fitted equation
    #if gMinuit.fStatus:
    for x in (600,800,1000,1200):#,1400,1700,2000,2500,3000,3500,4000):
      for y in (300,400,500,600,700,800,900,1000):
        if (x-y <= 100) : continue
        #if (x==600 and (y>=500)) : continue
        #if (x==800 and (y>=700)) : continue
        origeff = histoeff.GetBinContent(histoeff.FindBin(x,y))
        # which pol
        if options.fitPolDeg1: eff = p[0] + p[1]*x + p[2]*y
        elif options.fitPolDeg3: eff = p[0] + p[1]*x + p[2]*y + p[3]*x*x + p[4]*y*y + p[5]*x*y + p[6]*x*x*x + p[7]*y*y*y + p[8]*x*x*y + p[9]*x*y*y
        else: eff = p[0] + p[1]*x + p[2]*y + p[3]*x*x + p[4]*y*y + p[5]*x*y
        if origeff==0: print("Calculated Eff for: mZ = %i , mA = %i  ------ %f (-)" %(x,y,eff))
        else: print("Calculated Eff for: mZ = %i , mA = %i  ------ %f (Orig = %f)" %(x,y,eff,origeff))

    # make output plot
    c = TCanvas("c","",1000,600)
    c.cd()
    histofit.SetMarkerStyle(20)
    histofit.SetMarkerSize(0.5)
    histofit.Draw("PE")
    fit.Draw("lego same")
    
    # save output
    c.Update()
    c.Write()
    c.SaveAs(options.outdir+"OutputFit_"+options.model+"_"+metregion+".png") 
    c.SaveAs(options.outdir+"OutputFit_"+options.model+"_"+metregion+".pdf") 
    

# --- Call
if __name__ == "__main__":
  app = FitMaker()
  app.run() 
 
