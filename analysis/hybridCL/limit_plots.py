#!/bin/env python

from diphotons.Utils.pyrapp import *
from optparse import OptionParser, make_option
from copy import deepcopy as copy
import os, sys, glob, json

from auto_plotter import getObjects

def guessLabel(name):
    if "EBEB" in name: return "EBEB"
    elif "EBEE" in name: return "EBEE"
    else: return "Combined"
    

def scaleGraph(graph,scale):
    graph = graph.Clone()
    graph.GetListOfFunctions().Clear()
    ## graph.Print()
    
    xvals = graph.GetX()
    yvals = graph.GetY()
    yerrl = graph.GetEYlow()
    yerrh = graph.GetEYhigh()
    for ip in xrange(graph.GetN()):
        scl = scale(xvals[ip]) 
        ## print scl
        graph.SetPoint( ip, xvals[ip], yvals[ip]*scl )
        try:
            graph.SetPointEYlow( ip, yerrl[ip]*scl )
            graph.SetPointEYhigh( ip, yerrh[ip]*scl )
        except:
            pass
    
    ## graph.Print()
    
    return graph

def fitFunc(graph,func):
    
    ## func = func.Clone()
    graph.Fit(func)
    return func.Clone()

def addCmsLumi(canv,period,pos,extraText=None):
    if extraText:
        ROOT.writeExtraText = True
        if type(extraText) == str and extraText != "":
            ROOT.extraText = extraText
    ROOT.CMS_lumi(canv,period,pos)

# -----------------------------------------------------------------------------------------------------------
class LimitPlot(PlotApp):

    def __init__(self):
        super(LimitPlot,self).__init__(option_list=[
                make_option("--do-limits",action="store_true", dest="do_limits", 
                            default=False),
                make_option("--do-pvalues",action="store_true", dest="do_pvalues", 
                            default=False),
                make_option("--do-comparison",action="store_true", dest="do_comparison", 
                            default=False),
                make_option("--compare-expected",action="store_true", dest="compare_expected", 
                            default=False),
                make_option("--compare-file","--compare-files",dest="compare_files",action="callback",type="string", callback=optpars_utils.ScratchAppend(str),
                            default=[]),
                make_option("--compare-label","--compare-labels",dest="compare_labels",action="callback",type="string", callback=optpars_utils.ScratchAppend(str),
                            default=[]),
                make_option("--asimov-expected",action="store_true", dest="asimov_expected", 
                            default=True),
                make_option("--toys-expected",action="store_false", dest="asimov_expected", 
                            ),
		make_option("--mA0",action="store",dest="mA0",default=""),
                make_option("-n","--label",action="store", dest="label", 
                            default=""),                
                make_option("-M","--method",action="store", dest="method", 
                            default="Asymptotic"),                
                make_option("-U","--unblind",action="store_true", dest="unblind", 
                            default=False),                
                make_option("-B","--blind",action="store_false", dest="unblind", 
                            ),                
                make_option("-k","--couplings",action="callback", dest="couplings", type="string", callback=optpars_utils.ScratchAppend(str),
                            default=[]),                
                make_option("-x","--x-sections",action="callback", dest="x_sections", type="string", callback=optpars_utils.Load(),
                            default={}),                
                make_option("--fixed-x-section",action="store", dest="fixed_x_section", type="float", 
                            default=None), 
                make_option("--use-fb",dest="use_fb", action="store_true", 
                            default=False), 
                
            ])
        
        global ROOT, style_utils, RooFit
        import ROOT
        from ROOT import RooFit
        from ROOT import RooAbsData
        import diphotons.Utils.pyrapp.style_utils as style_utils


    def __call__(self,options,args):
        self.loadRootStyle()
        
        # ROOT.gSystem.AddIncludePath( "$ROOTSYS/include" )
        ROOT.gROOT.LoadMacro( "$CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/plotting/bandUtils.cxx+" )
        
        self.loadXsections(options.x_sections)

        if options.do_comparison:
            if len(options.compare_labels) > 0: assert( len(options.compare_labels) == len(options.compare_files) )
            else: options.compare_labels = map(guessLabel, options.compare_files)
            self.compare = map(lambda x: (getObjects([self.open(x[0])]),x[1]), zip(options.compare_files,options.compare_labels) )
            self.plotComparisons(options)
            return
        if options.compare_expected:
            self.compareExpectedLivia(options, True, False) #orig
            #self.compareExpectedLivia(options, False, False) #boost
            self.compareExpectedLivia(options, False, True) #chiara
            return

        print options.couplings
        #if len(options.couplings) == 0:
        #    flist = glob.glob("%s/higgsCombine%s*.%s.root" % (options.input_dir, options.label, options.method) )
        #else:
        #flist = ["%s/higgsCombineTest.%s.mA0300.root" %(options.input_dir, options.method)]
        flist = [ "%s/higgsCombine%sCombined.%s.mH125_2HDM.root" % (options.input_dir, options.label, options.method)]
        #flist = [ "%s/higgsCombine%sCombined.%s.mH125_2HDM.root" % (options.input_dir, options.label, options.method)]
        print options.input_dir, flist
            
        tflist = {}
        for fname in flist:
            bname = os.path.basename(fname)
            coup = bname.split("_",1)
            print coup
            coup = coup[1].split(".")
            print coup
            coup = coup[0].replace("k","")
            print coup
            ## coup = bname.split("_",1)[1].split(".")[0].replace("k","")
            tfin = self.open(fname)
            if not tfin: 
                print ("unable to open %s" % fname)
                sys.exit(-1)
            tree = tfin.Get("limit")
            if not tree: 
                print ("unable to find limit tree in %s" % fname)
                sys.exit(-1)
        
            tflist[coup] = tfin
        
        self.graphs = []
        for coup,tfile in tflist.iteritems():

            if options.do_limits:
                print coup, tfile
                self.plotLimit(options,coup,tfile)
            if options.do_pvalues:
                self.plotPval(options,coup,tfile)
                
        self.autosave()

        if len(options.couplings) == 0:
            graphs = self.open("%s/graphs_%s.root" % (options.input_dir,options.method),"recreate")
        else:
            graphs = self.open("%s/graphs_%s_%s.root" % (options.input_dir,"_".join(options.couplings),options.method),"recreate")
        graphs.cd()
        for gr in self.graphs: gr.Write()
        graphs.Close()
   
    def compareExpectedLivia(self,options, isOrigSel, isChiara):
        #original selection
       
        tfile_boostchiaramet80_H=self.open("ntuples4fit_newboostchiara_met80_cic_default_shapes_lumi_2.245/graphs_Asymptotic.root")
        tfile_boostchiaramet80_noH=self.open("ntuples4fit_newboostchiara_met80_noH_cic_default_shapes_lumi_2.246/graphs_Asymptotic.root")
        tfile_boostchiaramet80_nosyst=self.open("ntuples4fit_newboostchiara_met80_noSyst_cic_default_shapes_lumi_2.246/graphs_Asymptotic.root")

        #74X
        tfile_opt3_74X_H=self.open("ntuples4fit_vtx0_OptSel3_74X_cic_default_shapes_lumi_2.246/graphs_Asymptotic.root")
        tfile_opt4_74X_H=self.open("ntuples4fit_vtx0_OptSel4_74X_cic_default_shapes_lumi_2.246/graphs_Asymptotic.root")
        tfile_ABCD_opt1_74X=self.open("ABCD_74X_OptSel1/graphs_Asymptotic.root")
        tfile_ABCD_opt2_74X=self.open("ABCD_74X_OptSel2/graphs_Asymptotic.root")
        tfile_CC_opt2_74X=self.open("ChiaraCC_74X_OptSel2/graphs_Asymptotic.root")
        tfile_CC_opt1_74X=self.open("ChiaraCC_74X_OptSel1/graphs_Asymptotic.root")
        
        #76X
        tfile_opt3_76X_H=self.open("ntuples4fit_vtx0_OptSel3_76X_cic_default_shapes_lumi_2.3/graphs_Asymptotic.root")
        tfile_opt4_76X_H=self.open("ntuples4fit_vtx0_OptSel4_76X_cic_default_shapes_lumi_2.3/graphs_Asymptotic.root")
        tfile_ABCD_opt1_76X=self.open("ABCD_76X_OptSel1/graphs_Asymptotic.root")
        tfile_ABCD_opt2_76X=self.open("ABCD_76X_OptSel2/graphs_Asymptotic.root")
        tfile_ABCD_opt3_76X=self.open("ABCD_76X_OptSel3/graphs_Asymptotic.root")
        tfile_CC_opt2_76X=self.open("ChiaraCC_76X_OptSel2/graphs_Asymptotic.root")
        tfile_CC_opt1_76X=self.open("ChiaraCC_76X_OptSel1/graphs_Asymptotic.root")
        tfile_LS_opt1_76X=self.open("Livia_76X_OptSel1/graphs_Asymptotic.root")
        tfile_LS_opt1_76X_noSyst=self.open("Livia_76X_OptSel1/graphs_Asymptotic.root")
      
        #76X hybridNew
        tfile_LS_opt1_76X_HybridNew=self.open("hybNew.root")
    
        gr_boostchiaramet80_H=tfile_boostchiaramet80_H.Get("expected_2HDM")
        gr_boostchiaramet80_noH=tfile_boostchiaramet80_noH.Get("expected_2HDM")
        gr_boostchiaramet80_nosyst=tfile_boostchiaramet80_nosyst.Get("expected_2HDM")


        gr_opt3_74X_H=tfile_opt3_74X_H.Get("expected_2HDM")
        gr_opt4_74X_H=tfile_opt4_74X_H.Get("expected_2HDM")
        gr_ABCD_opt1_74X = tfile_ABCD_opt1_74X.Get("expected_2HDM")
        gr_ABCD_opt2_74X = tfile_ABCD_opt2_74X.Get("expected_2HDM")
        gr_CC_opt2_74X = tfile_CC_opt2_74X.Get("expected_2HDM")
        gr_CC_opt1_74X = tfile_CC_opt1_74X.Get("expected_2HDM")
      
        gr_opt3_76X_H=tfile_opt3_76X_H.Get("expected_2HDM")
        gr_opt4_76X_H=tfile_opt4_76X_H.Get("expected_2HDM")
        gr_ABCD_opt1_76X = tfile_ABCD_opt1_76X.Get("expected_2HDM")
        gr_ABCD_opt2_76X = tfile_ABCD_opt2_76X.Get("expected_2HDM")
        gr_ABCD_opt3_76X = tfile_ABCD_opt3_76X.Get("expected_2HDM")
        gr_CC_opt2_76X = tfile_CC_opt2_76X.Get("expected_2HDM")
        gr_CC_opt1_76X = tfile_CC_opt1_76X.Get("expected_2HDM")
        gr_LS_opt1_76X = tfile_LS_opt1_76X.Get("expected_2HDM")
        gr_LS_opt1_76X_noSyst = tfile_LS_opt1_76X_noSyst.Get("expected_2HDM")
      
        
        gr_LS_opt1_76X_HybridNew= tfile_LS_opt1_76X_HybridNew.Get("Graph")

        unit = "fb" if options.use_fb else "pb"
        basicStyle = [["SetMarkerSize",0.6],["SetLineWidth",3],
              ["SetTitle",";M_{Z`} (GeV);95%% C.L. #sigma(pp#rightarrow Z`#rightarrowA_{0}H #rightarrow#chi#chi#gamma#gamma ) (%s)" % unit]]
        expectedStyle = [["SetMarkerSize",0]]+basicStyle
    
     
        style_utils.apply( gr_boostchiaramet80_H, [["colors",ROOT.kRed],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_boostchiaramet80_noH, [["colors",ROOT.kBlue],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_boostchiaramet80_nosyst, [["colors",ROOT.kOrange],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )


        style_utils.apply( gr_opt3_74X_H, [["colors",ROOT.kRed],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_opt4_74X_H, [["colors",ROOT.kBlue],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_ABCD_opt1_74X, [["colors",ROOT.kMagenta],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_ABCD_opt2_74X, [["colors",ROOT.kOrange],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_CC_opt2_74X, [["colors",ROOT.kSpring+9],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_CC_opt1_74X, [["colors",ROOT.kAzure+7],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )

        style_utils.apply( gr_opt3_76X_H, [["colors",ROOT.kRed],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_opt4_76X_H, [["colors",ROOT.kBlue],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_ABCD_opt1_76X, [["colors",ROOT.kMagenta],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_ABCD_opt2_76X, [["colors",ROOT.kOrange],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_ABCD_opt3_76X, [["colors",ROOT.kBlack],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_CC_opt2_76X, [["colors",ROOT.kSpring+9],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_CC_opt1_76X, [["colors",ROOT.kAzure+7],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_LS_opt1_76X, [["colors",ROOT.kAzure+7],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_LS_opt1_76X_HybridNew, [["colors",ROOT.kMagenta],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )
        style_utils.apply( gr_LS_opt1_76X_noSyst, [["colors",ROOT.kBlue],["SetLineStyle",7],["SetName","gr_pho"]]+expectedStyle )

      
        isH=True

      
        title=ROOT.TString()
        title="Comparison_"
      
        isNewSel=False

        legend = ROOT.TLegend(0.19,0.7,0.79,0.9)
        

        #show the effect of the resonant bkg
#        legend.AddEntry(gr_boostchiaramet80_H,"Adding the Higgs resonant background","l")
 #       legend.AddEntry(gr_boostchiaramet80_noH,"Fitting only the non-resonant background","l")
        
        #show the effect of the syst
        #legend.AddEntry(gr_boostchiaramet80_H,"With systematic uncertainty","l")
        #legend.AddEntry(gr_boostchiaramet80_nosyst,"W/O systematic uncertainty","l")
       
        is74X=False
       
        isHremoval=False
        isSyst=False
        isOpts=True
        isRels=False
        isABCD=True
        #compare opt3 and opt4
        if is74X and isOpts:
            legend.AddEntry(gr_ABCD_opt1_74X,"ABCD Option 1 74X","l")
          #  legend.AddEntry(gr_ABCD_opt2_74X,"ABCD Option 2 74X","l")      
            legend.AddEntry(gr_CC_opt1_74X,"C&C Option 1 74X","l")      
          #  legend.AddEntry(gr_CC_opt2_74X,"C&C Option 2 74X","l")      
            legend.AddEntry(gr_opt3_74X_H,"Option 3 74X","l")
          #  legend.AddEntry(gr_opt4_74X_H,"Option 4 74X","l")

        if not is74X and isOpts:
          #  legend.AddEntry(gr_opt3_76X_H,"FIT 76X","l")
          #  legend.AddEntry(gr_ABCD_opt1_76X,"ABCD 76X","l")
           # legend.AddEntry(gr_ABCD_opt2_76X,"Option 2 76X","l") 
            legend.AddEntry(gr_LS_opt1_76X,"Asymptotic","l")           
            legend.AddEntry(gr_LS_opt1_76X_HybridNew,"Full CLs","l")           
          #  legend.AddEntry(gr_LS_opt1_76X_noSyst,"Mass Sidebands 76X w/o syst","l")           
        
           # legend.AddEntry(gr_ABCD_opt3_76X,"ABCD Option 3 76X","l")
           # legend.AddEntry(gr_opt4_76X_H,"Option 4 76X","l")

        if isRels and isABCD:
            legend.AddEntry(gr_ABCD_opt1_74X,"ABCD Option 1 74X","l")
#            legend.AddEntry(gr_ABCD_opt1_76X,"ABCD Option 1 76X","l")  

            legend.AddEntry(gr_ABCD_opt2_74X,"ABCD Option 2 74X","l")      
#            legend.AddEntry(gr_ABCD_opt2_76X,"ABCD Option 2 76X","l") 
            legend.AddEntry(gr_CC_opt1_74X,"C&C Option 1 74X","l")               
            legend.AddEntry(gr_CC_opt2_74X,"C&C Option 2 74X","l")               
            
        if isRels and not isABCD:
            legend.AddEntry(gr_opt3_74X_H,"Option 3 74X","l")
#            legend.AddEntry(gr_opt3_76X_H,"Option 3 76X","l")
            legend.AddEntry(gr_opt4_74X_H,"Option 4 74X","l")
#            legend.AddEntry(gr_opt4_76X_H,"Option 4 76X","l")

       
        if isOrigSel:
            title+="limits_pho"
        if not isOrigSel and not isChiara:
            title+="limits_boost"
        if not isOrigSel and  isChiara:
            title+="limits_boostchiara"
        if not isOrigSel and  isChiara and isHremoval:
            title+="limits_boostchiara_HremovalCheck"
        if not isOrigSel and  isChiara and isSyst:
            title+="limits_boostchiara_SYSTremovalCheck"
        if not isOrigSel and  isChiara and isOpts and is74X:
            title+="limits_OptsCheck_74X_wCC"
        if not isOrigSel and  isChiara and isOpts and not is74X:
            title+="limits_OptsCheck_76X_Allsel_HybridNew"
        if not isOrigSel and  isChiara and isRels and isABCD:
            title+="limits_RelsCheck_ABCD_74"
        if not isOrigSel and  isChiara and isRels and not isABCD:
            title+="limits_RelsCheck_FIT_74"
        
        
        canv  = ROOT.TCanvas(title,title)
        canv.SetLogx()            
  
       # if isOrigSel and not hasMetCut and not lookAtmet:
         
       # if isOrigSel and  hasMetCut and not lookAtmet:
          
        if not isOrigSel and  isChiara and isH:
            gr_boostchiaramet80_H.Draw("APL")
            gr_boostchiaramet80_H.GetYaxis().SetRangeUser(2.,12.)
            if isHremoval:
                gr_boostchiaramet80_noH.Draw("PLsame")
            if isSyst:
                gr_boostchiaramet80_nosyst.Draw("PLsame")
           
        if not isOrigSel and  isChiara and isOpts and is74X:
            gr_opt3_74X_H.Draw("APL")
            gr_opt3_74X_H.GetYaxis().SetRangeUser(0.,12.)
            gr_ABCD_opt1_74X.Draw("PLsame")
          #  gr_ABCD_opt2_74X.Draw("PLsame")
          #  gr_opt4_74X_H.Draw("PLsame")
            gr_CC_opt1_74X.Draw("PLsame")
#            gr_CC_opt2_74X.Draw("PLsame")
        
        if not isOrigSel and  isChiara and isOpts and not is74X:
           # gr_opt3_76X_H.Draw("APL")
           # gr_opt3_76X_H.GetYaxis().SetRangeUser(0.,12.)
            #gr_ABCD_opt3_76X.Draw("PLsame")
           # gr_ABCD_opt1_76X.Draw("PLsame")
           # gr_CC_opt1_76X.Draw("PLsame")
           # gr_ABCD_opt2_76X.Draw("PLsame")
           # gr_opt4_76X_H.Draw("PLsame")
            gr_LS_opt1_76X.Draw("APL")
            gr_LS_opt1_76X_HybridNew.Draw("PLsame")
          #  gr_LS_opt1_76X_noSyst.Draw("PLsame")
            gr_LS_opt1_76X.GetYaxis().SetRangeUser(0.,12.)
       

        if not isOrigSel and  isChiara and isRels and not isABCD:
            gr_opt3_74X_H.Draw("APL")
            gr_opt3_74X_H.GetYaxis().SetRangeUser(0.,12.)
#            gr_opt3_76X_H.Draw("PLsame")
#            gr_opt4_76X_H.Draw("PLsame")
            gr_opt4_74X_H.Draw("PLsame")
       
        if not isOrigSel and  isChiara and isRels and isABCD:
         #   gr_ABCD_opt1_74X.Draw("APL")
            gr_LS_opt1_76X.Draw("APL")
           
          #  gr_LS_opt1_76X_noSyst.Draw("PLsame")
          #  gr_ABCD_opt1_74X.GetYaxis().SetRangeUser(0.,12.)
         #   gr_CC_opt1_74X.Draw("PLsame")
         #   gr_CC_opt2_74X.Draw("PLsame")
#            gr_ABCD_opt1_76X.Draw("PLsame")
         #   gr_ABCD_opt2_74X.Draw("PLsame")
#            gr_ABCD_opt2_76X.Draw("PLsame")
          

        addCmsLumi(canv,4, 1, "Simulation")
        
        self.keep(legend,True)
        #if not isH: 
        legend.Draw()
        self.keep( [canv,gr_opt4_74X_H] )
        self.format(canv,options.postproc)
      #  self.format(canv, options)
      
    def plotLimit(self,options,coup,tfile):
        ## TGraphAsymmErrors *theBand(TFile *file, int doSyst, int whichChannel, BandType type, double width=0.68) {
        tfile_xsecTheo=self.open("Signal_crossSections.root")
        tfile_gz08=self.open("gz08_fbU.root")
        theo = tfile_xsecTheo.Get("xsec_2HDM")
        gz08 = tfile_gz08.Get("xsec_2HDM")
        

        if options.asimov_expected:
            ROOT.use_precomputed_quantiles = True
            bandType = ROOT.Median 
        else:
            bandType = ROOT.Median
        expected68 = ROOT.theBand( tfile, 1, 0, bandType, 0.68 )
        expected95 = ROOT.theBand( tfile, 1, 0, bandType, 0.95 )
        observed = ROOT.theBand( tfile, 1, 0, ROOT.Observed, 0.95 )
#        print expected68.GetN()
 #       print expected95.GetN()
        
        unit = "fb" if options.use_fb else "pb"
        basicStyle = [["SetMarkerSize",0.6],["SetLineWidth",3],
                       ["SetTitle",";M_{Z`} (GeV);95%% C.L. #sigma(pp#rightarrow Z`#rightarrowA_{0}H #rightarrow#chi#chi#gamma#gamma ) (%s)" % unit]]
        commonStyle = [[self.scaleByXsec,coup],"Sort"]+basicStyle
        ## expectedStyle = commonStyle+[["SetMarkerStyle",ROOT.kOpenCircle]]
        expectedStyle = commonStyle+[["SetMarkerSize",0]]
        observedStyle = commonStyle+[["SetMarkerStyle",ROOT.kFullCircle]]
        theoStyle = [["SetMarkerSize",0]]+basicStyle

        style_utils.apply( theo, [["colors",ROOT.kRed],["SetLineStyle",5],["SetName","theo"]]+theoStyle )
        style_utils.apply( gz08, [["colors",ROOT.kBlue],["SetLineStyle",5],["SetName","theo"]]+theoStyle )
        style_utils.apply( expected68, [["colors",ROOT.kYellow],["SetName","expected68_%s"%coup]]+expectedStyle )
        style_utils.apply( expected95, [["colors",ROOT.kGreen],["SetName","expected95_%s"%coup]]+expectedStyle )
        
        expected = ROOT.TGraph(expected68)
        style_utils.apply( expected, [["colors",ROOT.kBlack],["SetLineStyle",7],["SetName","expected_%s"%coup]])
        
        style_utils.apply(observed,[["SetName","observed_%s"%coup]]+observedStyle)
      
        canv  = ROOT.TCanvas("limits_k%s_mA0%s"%(coup,options.mA0),"limits_k%s_mA0%s"%(coup,options.mA0))
        canv.SetLogx()
        canv.SetLogy()
       
        canv.SetGridx()
        canv.SetGridy()
        legend = ROOT.TLegend(0.2,0.67,0.8,0.9)
        expected95.Draw("APE3")        
        expected95.GetXaxis().SetRangeUser(600,2550)
        expected95.GetYaxis().SetRangeUser(0.008,200)
        expected95.GetXaxis().SetMoreLogLabels()
        expected68.Draw("E3PL")
        expected.Draw("PL")
        theo.Draw("PL")
        gz08.Draw("PL")
        #kappa = "0."+coup[1:]
        #legend.AddEntry(None,"#tilde{#kappa} = %s" % kappa,"")
	legend.SetHeader("Limits for 2HDM with mA0 = %s GeV"%options.mA0)
        legend.AddEntry(theo,"2HDM - g_{Z} constrained","l") 
        legend.AddEntry(gz08,"2HDM - g_{Z} = 0.8","l") 
        legend.AddEntry(expected,"Expected limit","l")
        legend.AddEntry(expected68," \pm 1 \sigma","f")
        legend.AddEntry(expected95," \pm 2 \sigma","f")
        if options.unblind:
            observed.Draw("PL")
            ## observed.Draw("L")
            legend.AddEntry(observed,"Observed limit","l")
        #if coup in self.xsections_:
        #    grav = self.xsections_[coup]
        #    style_utils.apply( grav, basicStyle+[["SetLineStyle",9],["colors",ROOT.myColorB2]] )
        #    grav.Draw("L")
        #    legend.AddEntry(grav,"Z`#rightarrowA_{0}H #rightarrow#chi#chi#gamma#gamma","l").SetLineStyle(0)
        canv.RedrawAxis()   
        self.keep(legend,True)
        legend.Draw()
        
        self.graphs.extend([gz08,theo,observed,expected,expected68,expected95])
        
        self.keep( [gz08,theo,canv,observed,expected,expected68,expected95] )
        self.format(canv,options.postproc)

    def plotComparisons(self,options):
        ## if options.compare_expected:
        #if options.compare_expected:
         #   observed = map(lambda x: (filter(lambda y: "expected" in y.GetName(), x[0]),x[1]), self.compare)
          #  coups = set(map(lambda x: x.GetName().replace("expected_",""), reduce(lambda x,y: x+y, map(lambda z: z[0], observed), [])))
      #  else:
        observed = map(lambda x: (filter(lambda y: "observed" in y.GetName(), x[0]),x[1]), self.compare)
        coups = set(map(lambda x: x.GetName().replace("observed_",""), reduce(lambda x,y: x+y, map(lambda z: z[0], observed), [])))
        ## observed = map(lambda x: (filter(lambda y: "expected" in y.GetName(), x[0]),x[1]), self.compare)
        ## coups = set(map(lambda x: x.GetName().replace("expected_",""), reduce(lambda x,y: x+y, map(lambda z: z[0], observed), [])))
        coups = ["001"]
        map(lambda x: self.plotComparison(options,x,observed), coups)

    def plotComparison(self,options,coup,observed):
        
        cobserved = map(lambda x: (filter(lambda y: y.GetName().ends, x[0])[0],x[1]), observed)
        print cobserved
        
        styles = [ [["colors",ROOT.kBlue]], [["colors",ROOT.kOrange]], [["colors",ROOT.kGreen+1]] ]
        map(lambda x: style_utils.apply(x[0],[["SetMarkerSize",0.5],["SetLineWidth",1]]+styles.pop(0)), cobserved)
    
        canv = ROOT.TCanvas("comparison_%s" % coup,"comparison_%s" % coup)
        legend = ROOT.TLegend(0.55,0.51,0.85,0.76)
        legend.SetFillStyle(0)
        kappa = "0."+coup[1:]
        legend.AddEntry(None,"#tilde{#kappa} = %s" % kappa,"")
        
        g0 = cobserved[0][0]
        g0.Draw("apl")
        for gr,nam in cobserved:
            legend.AddEntry(gr,nam,"l")
        for gr,nam in reversed(cobserved):
            gr.Draw("pl")
        legend.Draw("same")
        
        g0.GetXaxis().SetRangeUser(450,5000)
        g0.GetXaxis().SetMoreLogLabels()
        canv.SetLogx()
        if options.do_pvalues:
            canv.SetLogy()
            g0.GetYaxis().SetRangeUser(1e-3,0.55)
            self.drawLines(g0)
        
        self.keep([canv,legend])
        self.format(canv,options.postproc)
        
    def drawLines(self,ref,xmin=450,xmax=5000):
        
        spots = filter(lambda x: x>ref.GetYaxis().GetXmin(),  map(lambda x: (x,ROOT.RooStats.SignificanceToPValue(x)), xrange(1,5) ) )
        
        lines = map( lambda y: ROOT.TLine(xmin,y[1],xmax,y[1]), spots )
        map( lambda x: style_utils.apply(x,[["SetLineColor",ROOT.kGray+3],["SetLineStyle",7]]), lines )
        
        labels = map( lambda y: ROOT.TLatex(xmax*1.01,y[1]*0.9,"#color[%d]{%d #sigma}" % (ROOT.kGray+2,y[0])), spots )
        map( lambda x: style_utils.apply(x,[["SetTextSize",0.05]]), labels )

        map( lambda x: x.Draw("same"), lines+labels )
        self.keep(lines+labels)
        
    def plotPval(self,options,coup,tfile):
        observed = ROOT.theBand( tfile, 1, 0, ROOT.Observed, 0.95 )
        basicStyle = [["SetMarkerSize",0.6],["SetLineWidth",3],
                       ["SetTitle",";M_{G} (GeV);p_{0}"]]
        commonStyle = ["Sort"]+basicStyle
        observedStyle = commonStyle+[["SetMarkerStyle",ROOT.kFullCircle],["colors",ROOT.kBlue]]
        
        style_utils.apply(observed,[["SetName","observed_%s"%coup]]+observedStyle)
      
        
        canv  = ROOT.TCanvas("pvalues_k%s"%coup,"pvalues_k%s"%coup)
        canv.SetLogy()
        canv.SetLogx()
        legend = ROOT.TLegend(0.5,0.6,0.8,0.75)
        legend.SetFillStyle(0)
        kappa = "0."+coup[1:]
        observed.Draw("apl")
        ## observed.Draw("al")
        ## observed.GetYaxis().SetRangeUser(1e-5,0.55)
        observed.GetYaxis().SetRangeUser(1e-3,0.55)
        observed.GetXaxis().SetRangeUser(450,5000)
        observed.GetXaxis().SetMoreLogLabels()
        
        ## xmin,xmax=observed.GetXaxis().GetXmin(),observed.GetXaxis().GetXmax()
        xmin,xmax=450,5000
        spots = filter(lambda x: x>observed.GetYaxis().GetXmin(),  map(lambda x: (x,ROOT.RooStats.SignificanceToPValue(x)), xrange(1,5) ) )
        
        lines = map( lambda y: ROOT.TLine(xmin,y[1],xmax,y[1]), spots )
        map( lambda x: style_utils.apply(x,[["SetLineColor",ROOT.kGray+3],["SetLineStyle",7]]), lines )

        labels = map( lambda y: ROOT.TLatex(xmax*1.01,y[1]*0.9,"#color[%d]{%d #sigma}" % (ROOT.kGray+2,y[0])), spots )
        map( lambda x: style_utils.apply(x,[["SetTextSize",0.05]]), labels )

        map( lambda x: x.Draw("same"), lines+labels )
        self.keep(lines+labels)
        
        legend.AddEntry(None,"#tilde{#kappa} = %s" % kappa,"")
        legend.AddEntry(observed,"Observed p_{0}","l")
        
        self.keep(legend,True)
        legend.Draw()
        
        self.graphs.extend([observed])
        
        self.keep( [canv,observed] )
        self.format(canv,options.postproc)

    def scaleByXsec(self,graph,coup):
        if self.options.fixed_x_section:
            scale = self.options.fixed_x_section
            if self.options.use_fb: scale *= 1e+3
        xvals = graph.GetX()
        yvals = graph.GetY()
        yerrl = graph.GetEYlow()
        yerrh = graph.GetEYhigh()
        if not self.options.fixed_x_section:
            if not coup in self.xsections_:
                print("Cross section for k = %s not found" % coup)
                xsec=1
                #sys.exit(-1)
            else:
                xsec = self.xsections_[coup]
        for ip in range(graph.GetN()):
            if not self.options.fixed_x_section:
                #scale = xsec.Eval(xvals[ip])
                scale=1
            graph.SetPoint( ip, xvals[ip], yvals[ip]*scale )
            graph.SetPointEYlow( ip, yerrl[ip]*scale )
            graph.SetPointEYhigh( ip, yerrh[ip]*scale )
        
    def loadXsections(self,inmap):
        self.xsections_ = {}
        scl = 1e+3 if self.options.use_fb else 1.
        for name,val in inmap.iteritems():
            if name.startswith("RSGravToGG") or name.startswith("RSGravitonToGG"):
                coup,mass = name.split("kMpl")[1].split("_Tune")[0].replace("_","").replace("-","").split("M")
                mass = float(mass)
                if not coup in self.xsections_:
                    self.xsections_[coup] = ROOT.TGraph()
                self.xsections_[coup].SetPoint(self.xsections_[coup].GetN(),mass,val["xs"]*scl)
        for name,val in self.xsections_.iteritems():
            val.Sort()

    def plotXsections(self):
        coups = sorted( map( lambda x: (float("0."+x[0][1:]),x[1]), self.xsections_.iteritems() ), key=lambda x: x[0] )
        ## minc = min( map( lambda x: x[0], coups) )
        refc = coups[-4]
        print refc, coups
        scaled = map( lambda x: (x[0],scaleGraph(x[1], lambda y: refc[0]*refc[0]/((x[0]*x[0])*refc[1].Eval(y)))), coups )
        
        mypol = ROOT.TF1("mypol","[0]+[1]*(x-[2])**2")
        fit = map( lambda x: (x[0],x[1],fitFunc(x[1],mypol)),  scaled )
        
        rescaled = map( lambda x: (x[0],scaleGraph(x[1], lambda y: 1./(x[2].Eval(y)) )), fit )

        canv = ROOT.TCanvas("xsections","xsections")
        scaled[0][1].Draw("apl")
        # scaled[0].GetYaxis().SetRange(0,5)
        for g in scaled[1:]: g[1].Draw("pl")
        print scaled
        self.keep( list(scaled) )
        self.keep(canv)

        recanv = ROOT.TCanvas("xsections_rescaled","xsections_rescaled")
        rescaled[0][1].Draw("apl")
        # scaled[0].GetYaxis().SetRange(0,5)
        for g in rescaled[1:]: g[1].Draw("pl")
        print rescaled
        self.keep( list(rescaled) )
        self.keep(recanv)

        params = map( lambda x: (x[0], x[2].GetParameter(0), x[2].GetParameter(1), x[2].GetParameter(2)), fit  )
        
        param_graphs = ROOT.TGraph(), ROOT.TGraph(), ROOT.TGraph()
        map( lambda x: (param_graphs[0].SetPoint(param_graphs[0].GetN(),x[0],x[1]),param_graphs[1].SetPoint(param_graphs[1].GetN(),x[0],x[2]),param_graphs[2].SetPoint(param_graphs[2].GetN(),x[0],x[3])), params )
        for ip, gr in enumerate(param_graphs):
            gr.Sort()
            pcanv = ROOT.TCanvas("p%d"%ip,"p%d"%ip)
            gr.Draw()
            self.keep( [gr,pcanv] )

        p0 = ROOT.TF1("p0","pol2")
        p0.SetParameters(1.09141,-0.0977154,-0.670345)

        p1 = ROOT.TF1("p1","pol2")
        p1.SetParameters(-3.44266e-08,5.194e-08,2.02169e-07)

        p2 = ROOT.TF1("p2","pol2")
        p2.SetParameters(2718.59,69.1401,-772.539)
        
        ## refc[0] = 3
        equalized = map( lambda x: (x[0],scaleGraph(x[1], lambda y: refc[0]*refc[0]/((x[0]*x[0])*(p0.Eval(x[0]) + p1.Eval(x[0])*(y-p2.Eval(x[0]))**2)) )), coups )        

        eqcanv = ROOT.TCanvas("xsections_equalized","xsections_equalized")
        ## equalized[0][1].Draw("apl")
        ## # scaled[0].GetYaxis().SetRange(0,5)
        ## for g in equalized[1:]: g[1].Draw("pl")
        ## self.keep( list(equalized) )
        ## self.keep(eqcanv)

        sumg = {}
        for gr in equalized:
            gr = gr[1]
            xvals = gr.GetX()
            yvals = gr.GetY()
            for ip in xrange(gr.GetN()):
                x,y = xvals[ip],yvals[ip]
                if not x in sumg: sumg[x] = [0.,0]
                sumg[x][0] += y
                sumg[x][1] += 1
        averaged = ROOT.TGraph()
        for x,y in sumg.iteritems():
            averaged.SetPoint(averaged.GetN(),x,y[0]/y[1])
        averaged.Sort()
        averaged.Draw("apl")
        self.keep(averaged)
        self.keep(eqcanv)
        
        xsec = {
            "ref" : refc[0],
            "p0"  : [ p0.GetParameter(0), p0.GetParameter(1), p0.GetParameter(2) ],
            "p1"  : [ p1.GetParameter(0), p1.GetParameter(1), p1.GetParameter(2) ],
            "p2"  : [ p2.GetParameter(0), p2.GetParameter(1), p2.GetParameter(2) ],
            "xsec" : [ (averaged.GetX()[i],averaged.GetY()[i]) for i in xrange(averaged.GetN()) ]
            }
        
        with open("xsecions.json","w+") as xsec_file:
            xsec_file.write(json.dumps(xsec))
            xsec_file.close()


       
# -----------------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    app = LimitPlot()
    app.run()
