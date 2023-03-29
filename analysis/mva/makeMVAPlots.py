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
      make_option("-i",action="store",dest="infile",type="string",
                  default="",help="Input file [default = %default]"),
      make_option("--indir",action="store",dest="indir",type="string",
                  default="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_MVA/",help="Indir [default = %default]"),
      make_option("--suffix",action="store",dest="suffix",type="string",
                  default="",help="Additional suffix [default = %default]"),
      make_option("--apply",action="store_true",dest="apply_mva",
                  default=False,help="Apply MVA"),
      make_option("--var",action="store_true",dest="do_var",
                  default=False,help="Plot variables after MVA cut"),
      make_option("--cut",action="store",dest="cut",type="string",
                  default="-100",help="Cut value [default = %default]"),
      ])
       
    global ROOT
    import sys, os, numpy, ROOT
    from ROOT import gDirectory
    from ROOT import *
    from optparse import OptionParser
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptTitle(0)

#--------------------------------------------------------------------------------------------------------------
  def __call__(self,options,args):
    print("Making plots for : %s" %options.mvatype)
    fin     = ROOT.TFile.Open(options.infile)
    fout    = ROOT.TFile.Open(options.outdir+'plotsMVAstudies_'+options.mvatype+''+options.suffix+'.root','RECREATE')
   
    addtree = ''
    if options.mvatype == 'DNN': addtree = 'DNN__BARY/'
    if options.mvatype == 'BDT': addtree = 'BDT'+options.suffix+'/'
 
    # --- Pick out different trees

    MVATree   = addtree+'Method_'+options.mvatype+'/'+options.mvatype+'/'
    TestTree  = fin.Get(addtree+'TestTree')
    TrainTree = fin.Get(addtree+'TrainTree')
  
    # --- Pick out the histograms
    print options.infile+'/'+addtree
    h_corr_sig  = ROOT.TH2F(fin.Get(addtree+'CorrelationMatrixS'))
    h_corr_bkg  = ROOT.TH2F(fin.Get(addtree+'CorrelationMatrixB'))
    h_train_ROC = ROOT.TH1D(fin.Get(MVATree+'MVA_'+options.mvatype+'_trainingRejBvsS'))
    h_test_ROC  = ROOT.TH1D(fin.Get(MVATree+'MVA_'+options.mvatype+'_rejBvsS'))
    # --- Make classifier histograms
    h_train_S   = ROOT.TH1D('h_train_S','S (Train)',80,-1.0,1.0)
    h_train_B   = ROOT.TH1D('h_train_B','B (Train)',80,-1.0,1.0)
    h_test_S    = ROOT.TH1D('h_test_S','S (Test)',80,-1.0,1.0)
    h_test_B    = ROOT.TH1D('h_test_B','B (Test)',80,-1.0,1.0)
    scut = 'classID==0'
    bcut = 'classID==1'
    TrainTree.Project("h_train_S",options.mvatype,scut)
    TrainTree.Project("h_train_B",options.mvatype,bcut)
    TrainTree.Project("h_test_S",options.mvatype,scut)
    TrainTree.Project("h_test_B",options.mvatype,bcut)  

    # --- Draw each plot with right styling 
    self.plotROCcurve(options,h_train_ROC,h_test_ROC)                  # ROC 
    self.plotCorrelations(options,h_corr_sig,'S')                      # CorrS
    self.plotCorrelations(options,h_corr_bkg,'B')                      # CorrB
    self.plotClassifier(options,h_train_S,h_train_B,h_test_S,h_test_B) # Classifier 
    mva_vars = []
    for nvar in range(1,h_corr_sig.GetXaxis().GetNbins()+1):           # Input Variables
      mva_var = h_corr_sig.GetXaxis().GetBinLabel(nvar)
      mva_vars.append(mva_var)
      h_var_sig = ROOT.TH1F(fin.Get(MVATree+mva_var+'__Signal'))
      h_var_bkg = ROOT.TH1F(fin.Get(MVATree+mva_var+'__Background'))
      self.plotVariables(options,mva_var,h_var_sig,h_var_bkg)
    if options.do_var:                                                 # Variables post-MVA cut
      for mva_var in mva_vars:
        self.plotAfterMVA(options,mva_var)
      # --- Plot MVA response (w/o any selection)
      options.cut = "-100"
      self.plotAfterMVA(options,"MVA_response")                        # MVA cut value

#--------------------------------------------------------------------------------------------------------------
  def plotROCcurve(self,options,h_train_ROC,h_test_ROC):
    c1 = ROOT.TCanvas("c1","",600,600)
    c1.cd()
  
    int_train = h_train_ROC.Integral()
    int_test  = h_test_ROC.Integral()
    rnd_train = "%3.3f" %int_train
    rnd_test  = "%3.3f" %int_test
  
    h_train_ROC.SetLineColor(2)
    h_train_ROC.SetLineStyle(2)
    h_test_ROC.SetLineColor(2)
    h_test_ROC.SetLineStyle(1)
    
    h_train_ROC.GetXaxis().SetTitle('Signal Efficiency')
    h_train_ROC.GetYaxis().SetTitle('Background Rejection (1-Eff)')
    h_train_ROC.SetMaximum(1.1)
    h_train_ROC.SetMinimum(0.0)
    h_train_ROC.GetXaxis().SetRangeUser(0.0,1.1)
    
    h_train_ROC.Draw('HIST')
    h_test_ROC.Draw('HIST SAME')
    l = ROOT.TLegend(0.2,0.2,0.6,0.32,"","brNDC")
    l.SetBorderSize(0)
    l.AddEntry(h_train_ROC,"Train Sample, AUC = "+str(rnd_train),"L")
    l.AddEntry(h_test_ROC,"Test Sample, AUC = "+str(rnd_test),"L")
    l.Draw('SAME')
    
    c1.Print(options.outdir+'plot_ROC_'+options.mvatype+''+options.suffix+'.png')
    c1.Print(options.outdir+'plot_ROC_'+options.mvatype+''+options.suffix+'.pdf')
    h_train_ROC.Write()
    h_test_ROC.Write()

#--------------------------------------------------------------------------------------------------------------
  def plotCorrelations(self,options,h_corr,which):
    c2 = ROOT.TCanvas("c2"+which,"",600,600)
    c2.cd()
    
    h_corr.Draw("COLZ")
    c2.Print(options.outdir+'plot_Corr'+which+'_'+options.mvatype+''+options.suffix+'.png') 
    c2.Print(options.outdir+'plot_Corr'+which+'_'+options.mvatype+''+options.suffix+'.pdf') 
    h_corr.Write()
   
#--------------------------------------------------------------------------------------------------------------
  def plotClassifier(self,options,h_train_S,h_train_B,h_test_S,h_test_B):
    c3 = ROOT.TCanvas("c3","",600,600)
    c3.cd()
  
    h_train_S.SetLineColor(2)
    h_train_S.SetMarkerColor(2)
    h_train_S.SetFillColor(2)
    h_train_S.SetFillStyle(3001)
    h_test_S.SetLineColor(2)
    h_test_S.SetMarkerColor(2)
    h_test_S.SetMarkerStyle(20)
  
    h_train_B.SetLineColor(4)
    h_train_B.SetMarkerColor(4)
    h_train_B.SetFillColor(4)
    h_train_B.SetFillStyle(3001) 
    h_test_B.SetLineColor(4)
    h_test_B.SetMarkerColor(4)
    h_test_B.SetMarkerStyle(20)
    
    h_train_S.GetXaxis().SetTitle('Classifier, '+options.mvatype)
    h_train_S.GetYaxis().SetTitle('Counts/Bin')
    ymax = max([h.GetMaximum() for h in [h_train_S,h_train_B,h_test_S,h_test_B] ])
    ymax *= 1.2
    h_train_S.SetMaximum(ymax)
  
    l3 = ROOT.TLegend(0.42,0.72,0.57,0.88,"","brNDC")
    l3.SetBorderSize(0)
    l3.AddEntry(h_train_S,"S (Train)","LF")
    l3.AddEntry(h_train_B,"B (Train)","LF")
    l3.AddEntry(h_test_S,"S (Test)","EPL")
    l3.AddEntry(h_test_B,"B (Test)","EPL")
  
    h_train_S.Draw('HIST')
    h_train_B.Draw('HIST SAME')
    h_test_S.Draw('EP SAME')
    h_test_B.Draw('EP SAME')
    l3.Draw('SAME')
    c3.Print(options.outdir+'plot_Classifier_'+options.mvatype+''+options.suffix+'.png')
    c3.Print(options.outdir+'plot_Classifier_'+options.mvatype+''+options.suffix+'.pdf')
    h_train_S.Write()
    h_train_B.Write()
    h_test_S.Write()
    h_test_B.Write()

#--------------------------------------------------------------------------------------------------------------
  def plotVariables(self,options,var,h_var_S,h_var_B):
    c4 = ROOT.TCanvas("c4"+var,"",600,600)
    c4.cd()

    h_var_S.SetLineColor(2)
    h_var_S.SetLineColor(2)
    h_var_S.SetMarkerColor(2)
    h_var_S.SetFillColor(2)
    h_var_S.SetFillStyle(3001)
  
    h_var_B.SetLineColor(4)
    h_var_B.SetMarkerColor(4)
    h_var_B.SetFillColor(4)
    h_var_B.SetFillStyle(3001) 

    int_S = h_var_S.Integral()
    int_B = h_var_B.Integral()
    h_var_S.Scale(1.0/int_S)
    h_var_B.Scale(1.0/int_B)
    ymax = max([h.GetMaximum() for h in [h_var_S,h_var_B] ])
    ymax *= 1.2
    h_var_S.SetMaximum(ymax)  
 
    h_var_S.GetXaxis().SetTitle(var)
    h_var_S.GetYaxis().SetTitle('')

    l4 = ROOT.TLegend(0.42,0.72,0.57,0.88,"","brNDC")
    l4.SetBorderSize(0)
    l4.AddEntry(h_var_S,"Signal Input","LF")
    l4.AddEntry(h_var_B,"Background Input","LF")
  
    h_var_S.Draw('HIST')
    h_var_B.Draw('HIST SAME')
    l4.Draw('SAME')
    c4.Print(options.outdir+'plot_'+var+'_'+options.mvatype+''+options.suffix+'.png')
    c4.Print(options.outdir+'plot_'+var+'_'+options.mvatype+''+options.suffix+'.pdf')
    h_var_S.Write()
    h_var_B.Write()

#--------------------------------------------------------------------------------------------------------------
  def plotAfterMVA(self,options,var):

    # --- Setup MVA cut
    cut = "MVA_response > "+options.cut

    # --- Setup sample names
    part1 = "OutputMVA_"+options.mvatype+'_'+options.suffix[6:]+'_'
    part2 = "_skim_woVetos.root"
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

    # --- Get files & trees & histos
    fsigs = {}
    fbkgs = {}
    fdata = {}
    tsigs = {}
    tbkgs = {}
    tdata = {}
    hsigs = {}
    hbkgs = {}
    hdata = {}
    x_min = 0.0
    x_max = 0.0
    for sig in sigs:
      htitle = "h_"+sig+"_"+var
      fsigs[sig] = ROOT.TFile.Open(options.indir+part1+sig+part2)
      tsigs[sig] = fsigs[sig].Get("DiPhotonTree")
      tsigs[sig].Draw(var+">>"+htitle,"("+cut+")*weight")
      hsigs[sig] = ROOT.gDirectory.Get(htitle)
      if sig==sigs[0]: x_min = hsigs[sig].GetXaxis().GetXmin()
      if sig==sigs[0]: x_max = hsigs[sig].GetXaxis().GetXmax()
      hsigs[sig].SetAxisRange(x_min,x_max,"X")
    for bkg in bkgs: 
      htitle = "h_"+bkg+"_"+var 
      fbkgs[bkg] = ROOT.TFile.Open(options.indir+part1+bkg+part2)
      tbkgs[bkg] = fbkgs[bkg].Get("DiPhotonTree")
      tbkgs[bkg].Draw(var+">>"+htitle,"("+cut+")*weight")
      hbkgs[bkg] = ROOT.gDirectory.Get(htitle)
      hbkgs[bkg].SetAxisRange(x_min,x_max,"X")
    for dat in data: 
      htitle = "h_"+dat+"_"+var 
      fdata[dat] = ROOT.TFile.Open(options.indir+part1+dat+part2)
      tdata[dat] = fdata[dat].Get("DiPhotonTree") 
      tdata[dat].Draw(var+">>"+htitle,"("+cut+")*weight")
      hdata[dat] = ROOT.gDirectory.Get(htitle)
      hdata[dat].SetAxisRange(x_min,x_max,"X")

    # --- Sum up histos
    for dat in data:
      if (dat == data[0]): h_sum_dat = hdata[dat].Clone()
      else:                h_sum_dat.Add(hdata[dat])

    i = 0
    for bkg in bkgs:
      if (bkg=="GluGluHToGG" or bkg=="VBFHToGG" or bkg=="ttHJetToGG" or bkg=="VHToGG"): continue
      if i==0: h_sum_nonres = hbkgs[bkg].Clone()
      else: h_sum_nonres.Add(hbkgs[bkg])  
      i = i+1

    #hgglist = ROOT.TList()
    ##hgglist.Add(hbkgs["GluGluHToGG"])
    #hgglist.Add(hbkgs["VBFHToGG"])
    #hgglist.Add(hbkgs["ttHJetToGG"])
    #h_sum_hgg = hbkgs["GluGluHToGG"].Clone()
    #h_sum_hgg.Reset()
    #h_sum_hgg.Merge(hgglist)
   
    h_sum_hgg = hbkgs["GluGluHToGG"].Clone()
    h_sum_hgg.Add(hbkgs["VBFHToGG"])
    h_sum_hgg.Add(hbkgs["ttHJetToGG"])
    h_sum_bkg = ROOT.THStack("h_stck_bkg_"+var,"")
    h_sum_bkg.Add(hbkgs["VHToGG"])
    h_sum_bkg.Add(h_sum_hgg)
    h_sum_bkg.Add(h_sum_nonres)

    # --- Setup colors
    color = {}
    for sig in sigs: 
      color[sig]      = kRed
    color["Data"]     = kBlack
    color["Bkg"]      = kTeal-1 
    color["SMHiggs"]  = kOrange-2
    color["VHToGG"]   = kRed-10
    for sig in sigs: 
      hsigs[sig].SetLineColor(color[sig])
      hsigs[sig].SetLineWidth(2)
    h_sum_dat.SetMarkerColor(color["Data"])
    h_sum_dat.SetMarkerStyle(8)
    h_sum_dat.SetLineColor(color["Data"])
    hbkgs["VHToGG"].SetFillColor(color["VHToGG"])
    h_sum_hgg.SetFillColor(color["SMHiggs"])
    h_sum_nonres.SetFillColor(color["Bkg"])

    # --- Plot
    c5 = ROOT.TCanvas("c5"+var,"",600,600)
    c5.cd()
    c5.SetLogy(1)  
 
    h_sum_dat.GetXaxis().SetTitle(var)
    h_sum_dat.Draw("PE")
    h_sum_bkg.Draw("HIST SAME")
    for sig in sigs: 
      hsigs[sig].Draw("HIST SAME")
    h_sum_dat.Draw("PE SAME") # redraw data
    gPad.RedrawAxis() 
 
    # --- Legend 
    l5 = ROOT.TLegend(0.55,0.65,0.80,0.88,"","brNDC")
    l5.SetBorderSize(0) 
    for dat in data:    l5.AddEntry(h_sum_dat,"Data","PEL")
    for sig in sigs:    l5.AddEntry(hsigs[sig],sig,"L")
    l5.AddEntry(h_sum_hgg,"SM H","F")
    l5.AddEntry(hbkgs["VHToGG"],"VH","F")
    l5.AddEntry(h_sum_nonres,"Non-res Bkg","F")
    l5.Draw('SAME')
    
    # --- Save
    if options.cut=="-100":
        c5.Print(options.outdir+'plot_'+var+'_afterMVA_'+options.mvatype+''+options.suffix+'.png')
        c5.Print(options.outdir+'plot_'+var+'_afterMVA_'+options.mvatype+''+options.suffix+'.pdf')
    else:
        c5.Print(options.outdir+'plot_'+var+'_afterMVA_'+str(options.cut)+'_'+options.mvatype+''+options.suffix+'.png')
        c5.Print(options.outdir+'plot_'+var+'_afterMVA_'+str(options.cut)+'_'+options.mvatype+''+options.suffix+'.pdf')

#--------------------------------------------------------------------------------------------------------------

if __name__ == "__main__": 
  app = PlotMaker()
  app.run()

