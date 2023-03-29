import os
from ROOT import *
from dep.pyapp import *
from dep.util  import *
from optparse import OptionParser, make_option

class PlotMaker(pyapp):
 
  def __init__(self):
    super(PlotMaker,self).__init__(option_list =[
      make_option("--plotxsec",action="store_true",dest="do_xsec",
                  default=False,help="Plot limits in xsec-M plane [default = %default]"),
      make_option("--noext",action="store_false",dest="do_ext",
                  default=True,help="Extend plot to lowest mDM on plot [default = %default]"),
      make_option("--exp",action="store_true",dest="do_exp",
                  default=False,help="Plot expected limits as well [default = %default]"),
      make_option("--gg",action="store_true",dest="do_gg",
                  default=False,help="Add gg plot [default = %default]"),
      make_option("--bb",action="store_true",dest="do_bb",
                  default=False,help="Add bb plot [default = %default]"),
      make_option("--tt",action="store_true",dest="do_tt",
                  default=False,help="Add tautau plot [default = %default]"),
      make_option("--zz",action="store_true",dest="do_zz",
                  default=False,help="Add zz plot [default = %default]"),
      make_option("--ww",action="store_true",dest="do_ww",
                  default=False,help="Add ww plot [default = %default]"),
      make_option("--dd",action="store_true",dest="do_dd",
                  default=False,help="Add direct detection plots [default = %default]"),
      make_option("--cmb",action="store_true",dest="do_cmb",
                  default=False,help="Add combonation results [default = %default]"),
      make_option("--suffix",action="store",dest="suffix",type="string",
                  default="",help="Additional suffix [default = %default]"),
      ])

  def __extrapolate__(self,gin,channels,mlow):
    gout     = {}
    num_extr = 100
    for channel in channels: 
      gout[channel] = TGraph()
      mDM_ref  = Double(0)
      xsec_ref = Double(0)
      gin[channel].GetPoint(0,mDM_ref,xsec_ref)
      mR_ref = 0.939*mDM_ref/(0.939+mDM_ref)
      for i in range(0, num_extr):
        mDM_i  = mlow + i*(mDM_ref-mlow)/num_extr
        mR_i   = 0.939*mDM_i/(0.939+mDM_i)
        xsec_i = xsec_ref*(mR_i*mR_i)/(mR_ref*mR_ref)
        gout[channel].SetPoint(i,mDM_i,xsec_i)
        for i in range(0,gin[channel].GetN()):
          mDM  = Double(0)
          xsec = Double(0)
          gin[channel].GetPoint(i,mDM,xsec)
          gout[channel].SetPoint(i+num_extr,mDM,xsec)
      gin[channel] = gout[channel] 

  def __convert__(self,gin,gout): 
    c_SI = 6.9e-41*1e12
    for i in range(0,gin.GetN()): 
      mMed = Double(0)
      mDM  = Double(0)
      gin.GetPoint(i,mMed,mDM)
      mR   = Double(0.939*mDM)/(0.939+mDM)
      xsec = Double(c_SI*(mR*mR)/(mMed*mMed*mMed*mMed))
      gout.SetPoint(i,mDM,xsec)

  def __call__(self,options,args):

    # setup which channels to run   
    channels = [] 
    if options.do_gg:  channels.append("gg")
    if options.do_bb:  channels.append("bb")
    if options.do_tt:  channels.append("tt")
    if options.do_zz:  channels.append("zz")
    if options.do_ww:  channels.append("ww")
    if options.do_cmb: channels.append("cmb")
    print("Make plot for channels: %s" %channels)

    # setup direct detection files
    dd_channels = []
    if options.do_dd:
      print("Also plotting direct detection results") 
      dd_channels.append('Cresst')
      dd_channels.append('CDMSlite')
      dd_channels.append('PandaX')
      dd_channels.append('LUX')
      dd_channels.append('XENON1T')
      dd_channels.append('vFloor')

    # path to input files
    filepath   = {}
    filepath["cmb"]      = "~soffi/public/4MonoH/combo_inputs_90pCL.root"; 
    filepath["gg"]       = "~soffi/public/4MonoH/gg_inputs_90pCL.root";
    filepath["tt"]       = "~soffi/public/4MonoH/tt_inputs_90pCL.root";
    filepath["bb"]       = "";
    filepath["zz"]       = "";
    filepath["ww"]       = "";
    filepath["LUX"]      = "~mzientek/public/DD/LUX_SI_Combination_Oct2016.txt"
    filepath["PandaX"]   = "~mzientek/public/DD/pandax.txt"
    filepath["CDMSlite"] = "~mzientek/public/DD/cdmslite2015.txt"
    filepath["Cresst"]   = "~mzientek/public/DD/cresstii.txt"
    filepath["vFloor"]   = "~mzientek/public/DD/Neutrino_SI.txt"
    filepath["XENON1T"]  = "~mzientek/public/DD/xenon1t.txt"

    # style plots
    color = {}
    text  = {}
    color["cmb"]        = kViolet+3 
    color["gg"]         = kMagenta-4
    color["tt"]         = kBlue-4
    color["bb"]         = kOrange 
    color["zz"]         = kOrange+9
    color["ww"]         = kViolet+1
    color["vFloor"]     = kOrange+3
    color["Cresst"]     = kGreen+1
    color["CDMSlite"]   = kGreen+3
    color["PandaX"]     = kGreen+2
    color["LUX"]        = kGreen+4
    color["XENON1T"]    = kGreen-5

    text["gg"]         = "#bf{DM + h(#gamma#gamma)}"
    text["bb"]         = "#bf{DM + h(bb)}"
    text["tt"]         = "#bf{DM + h(#tau#tau)}"
    text["zz"]         = "#bf{DM + h(ZZ)}"
    text["ww"]         = "#bf{DM + h(WW)}"
    text["cmb"]        = "#bf{DM + h(#gamma#gamma + #tau#tau)}"
    text["vFloor"]     = "#nu floor (permeable)"
    text["LUX"]        = "#bf{LUX}"
    text["PandaX"]     = "#bf{PandaX-II}"
    text["CDMSlite"]   = "#bf{CDMSlite}"
    text["Cresst"]     = "#bf{CRESST-II}"
    text["XENON1T"]    = "#bf{XENON1T}"

    # pick up graphs
    tgraph_obs = {}
    tgraph_exp = {}
    for channel in channels:
      tgraph_obs[channel] = TFile(filepath[channel]).Get("observed_curve")
      tgraph_exp[channel] = TFile(filepath[channel]).Get("expected_curve")
    for dd_channel in dd_channels:
      tgraph_obs[dd_channel] = TGraph(filepath[dd_channel])

    # convert to mDM-xsec plane
    tgraph_obs_new = {}
    tgraph_exp_new = {}
    for channel in channels:
      tgraph_obs_new[channel] = TGraph()
      tgraph_exp_new[channel] = TGraph()
      if options.do_xsec:
        self.__convert__(tgraph_obs[channel],tgraph_obs_new[channel])
        self.__convert__(tgraph_exp[channel],tgraph_exp_new[channel])
      else:
        tgraph_obs_new[channel] = tgraph_obs[channel].Clone()
        tgraph_exp_new[channel] = tgraph_exp[channel].Clone()
    for dd_channel in dd_channels:
      tgraph_obs_new[dd_channel] = TGraph()
      tgraph_obs_new[dd_channel] = tgraph_obs[dd_channel].Clone()

    # extrapolate
    if options.do_ext and options.do_xsec:
      self.__extrapolate__(tgraph_obs_new, channels, 1)
      self.__extrapolate__(tgraph_exp_new, channels, 1)

    # canvas
    C = TCanvas("C","C",1000,600)
    C.Divide(2)
    C.cd(1).SetPad(0.0,0,0.75,0.99)
    C.cd(1).SetLeftMargin(0.15)
    C.cd(1).SetBottomMargin(0.15)
    if options.do_xsec: C.cd(1).SetLogy()
    if options.do_xsec: C.cd(1).SetLogx()
    if options.do_xsec: frame = C.cd(1).DrawFrame(1,1e-47,2000,2*1e-35)
    else:               frame = C.cd(1).DrawFrame(0,0,1500,500)
    C.cd(1).SetTickx()
    C.cd(1).SetTicky()
    if options.do_xsec: frame.SetXTitle("Dark matter mass m_{DM} [GeV]")
    else:               frame.SetXTitle("Mediator mass M_{ med} [GeV]")
    if options.do_xsec: frame.SetYTitle("#sigma^{SI}_{DM-nucleon} [cm^{2}]")
    else:               frame.SetYTitle("Dark matter mass m_{DM} [GeV]")
    frame.GetXaxis().SetTitleSize(0.045)
    frame.GetYaxis().SetTitleSize(0.045)
    frame.GetXaxis().SetTitleOffset(1.5)
    frame.GetYaxis().SetTitleOffset(1.5)
    
    # legends
    texts = []
    #texts.append(add_text(0.15,0.4,0.89,0.99,"#bf{CMS}            "))
    texts.append(add_text(0.15,0.4,0.89,0.99,"#bf{CMS} Preliminary")) #x1,x2,y1,y2
    texts.append(add_text(0.7,0.90,0.89,0.99,"35.9 fb^{-1} (13 TeV)"))

    #leg1 = C.BuildLegend(0.7,0.4,0.95,0.95)
    leg1 = C.BuildLegend(0.7,0.6,0.95,0.90)
    leg1.SetBorderSize(0)
    leg1.SetTextFont(42)
    leg1.SetTextSize(0.025)
    leg1.SetTextAlign(12)
    leg1.Clear()
    if options.do_exp: 
      leg1.SetHeader("#splitline{#bf{CMS exclusion 90% CL}}{Vector med., Dirac DM; g_{ q} = 0.25, g_{ DM} = 1.0}")
    else:  
      leg1.SetHeader("#splitline{#bf{CMS observed exclusion 90% CL}}{Vector med., Dirac DM; g_{ q} = 0.25, g_{ DM} = 1.0}") 

    #leg2 = C.BuildLegend(0.7,0.05,0.95,0.4)
    leg2 = C.BuildLegend(0.7,0.15,0.95,0.6)
    leg2.SetBorderSize(0)
    leg2.SetTextFont(42)
    leg2.SetTextSize(0.025)
    leg2.SetTextAlign(12)
    leg2.Clear()
    if options.do_dd and options.do_xsec: leg2.SetHeader("#bf{DD observed exclusion 90% CL}")

    for channel in channels:
      if options.do_exp: 
        leg1.AddEntry(tgraph_obs_new[channel],text[channel]+" Observed","L")
        leg1.AddEntry(tgraph_exp_new[channel],text[channel]+" Expected","FL")
      else:  
        leg1.AddEntry(tgraph_obs_new[channel],text[channel]+"","L")
    for dd_channel in dd_channels:
        if dd_channel == "LUX"        : leg2.AddEntry(tgraph_obs_new[dd_channel],"#splitline{"+text[dd_channel]+"}{#it{[arXiv:1608.07648]}}","L") 
        elif dd_channel == "PandaX"   : leg2.AddEntry(tgraph_obs_new[dd_channel],"#splitline{"+text[dd_channel]+"}{#it{[arXiv:1607.07400]}}","L")
        elif dd_channel == "CDMSlite" : leg2.AddEntry(tgraph_obs_new[dd_channel],"#splitline{"+text[dd_channel]+"}{#it{[arXiv:1509.02448]}}","L")
        elif dd_channel == "Cresst"   : leg2.AddEntry(tgraph_obs_new[dd_channel],"#splitline{"+text[dd_channel]+"}{#it{[arXiv:1509.01515]}}","L")
        elif dd_channel == "XENON1T"  : leg2.AddEntry(tgraph_obs_new[dd_channel],"#splitline{"+text[dd_channel]+"}{#it{[arXiv:1705.06655]}}","L")

    # draw
    C.cd(2).SetPad(0.75,0.0,1.0,1.0)
    C.Update()
    C.cd(1)
    C.Update()

    gStyle.SetHatchesLineWidth(2)

    # draw direct detection results
    for dd_channel in dd_channels:
      print dd_channel
      tgraph_obs_new[dd_channel].SetLineColor(color[dd_channel])
      if dd_channel=="vFloor":  
         tgraph_obs_new[dd_channel].SetLineWidth(-102)
         #tgraph_obs_new[dd_channel].Draw("same")
      else:
         tgraph_obs_new[dd_channel].SetFillColor(kWhite)
         tgraph_obs_new[dd_channel].SetFillStyle(4001)
         tgraph_obs_new[dd_channel].SetLineWidth(2)
         tgraph_obs_new[dd_channel].Draw("same")

    # draw monoH channels
    for channel in channels:
      tgraph_obs_new[channel].SetLineColor(color[channel])
      tgraph_obs_new[channel].SetFillColor(color[channel])
      tgraph_obs_new[channel].SetMarkerColor(color[channel])
      tgraph_obs_new[channel].SetFillStyle(3005)
      tgraph_obs_new[channel].SetLineWidth(203)
      tgraph_obs_new[channel].SetLineStyle(kSolid)
      tgraph_obs_new[channel].SetMarkerSize(0.1)
      tgraph_obs_new[channel].Draw("same")
    
      tgraph_exp_new[channel].SetLineColor(color[channel])
      tgraph_exp_new[channel].SetFillColor(color[channel])
      tgraph_exp_new[channel].SetMarkerColor(color[channel])
      tgraph_exp_new[channel].SetFillStyle(0)
      tgraph_exp_new[channel].SetLineWidth(4)
      tgraph_exp_new[channel].SetLineStyle(kDashed)
      if options.do_exp : tgraph_exp_new[channel].Draw("same")   

    #C.cd(1).RedrawAxis()
    whichChannels = ""
    for channel in channels: 
      whichChannels += "_"
      whichChannels += channel
    if options.do_xsec: addname = "XsecDM"
    else:               addname = "MmedDM"
    if options.do_exp:  addtxt  = ""
    else:               addtxt   ="_obs"
    C.SaveAs(options.outdir+"SpinIndepend_"+addname+"_MonoH"+whichChannels+addtxt+"_Summary.pdf")
    C.SaveAs(options.outdir+"SpinIndepend_"+addname+"_MonoH"+whichChannels+addtxt+"_Summary.png")

# --- Call
if __name__ == "__main__":
  app = PlotMaker()
  app.run()
