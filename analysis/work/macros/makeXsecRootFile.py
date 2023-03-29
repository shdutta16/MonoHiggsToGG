import sys, numpy, ROOT
from ROOT import TH2F
from optparse import OptionParser

def run(opts):
  # input file
  if (opts.sig=="2HDM"): infile = 'xsecZp2HDM'
  if (opts.sig=="BARY"): infile = 'xsecZpBARY'
  fin_name  = opts.indir+infile+".txt"
  print "Analyzing : "+fin_name
  fin = open(fin_name,"r")

  # output file
  fout_name = opts.outdir+"Output_"+infile+".root"
  print "Writing to : "+fout_name
  fout = ROOT.TFile(fout_name,"RECREATE")
  
  # output histo
  bins = []
  if (opts.sig=="2HDM"): bins = [4050,0,4050,1000,0,1000]
  if (opts.sig=="BARY"): bins = [1000,0,1000,1000,0,1000]
  xbins = []
  if (opts.sig=="2HDM"): 
    xbins = [0,20,40,60] 
    ybins = [0,20,40,60,100]
  if (opts.sig=="BARY"):
    xbins = [] 
    ybins = []
  nxbins = len(xbins) 
  nybins = len(ybins)

  # declare histo
  if (opts.do_var): hist = ROOT.TH2F('h2_'+opts.sig+'_XSEC_pb','h2_'+opts.sig+'_XSEC_pb',nxbins,xbins,nybins,ybins)
  else:             hist = ROOT.TH2F('h2_'+opts.sig+'_XSEC_pb','h2_'+opts.sig+'_XSEC_pb',bins[0],bins[1],bins[2],bins[3],bins[4],bins[5])
 
  m_X = 1.0 # mZP for BARY, mZP for 2HDM
  m_Y = 1.0 # mDM for BARY, mA0 for 2HDM
  xsec = 1.0
  
  # read in values
  for line in fin:
    line = line.rstrip('\n')
    line = line.split(' ')
    m_X  = float(line[0])
    m_Y  = float(line[1])
    xsec = float(line[2])
    #print("%i %i : %f" %(m_X,m_Y,xsec))
    
    # fill histo
    ROOT.gStyle.SetOptStat(0)
    hist.Fill(m_X,m_Y,xsec)
    hist.SetMarkerStyle(7)
  
  # save histo
  hist.Write()
  fout.Close() 

#----------------------------------
def init():
  parser = OptionParser("usage: %prog [options]")
  parser.add_option("--sig",action="store",dest="sig",type="string",
                    default="2HDM",help="Which sig [default = %default]"),
  parser.add_option("--indir",action="store",dest="indir",type="string",
                    default="",help="Indir [default = %default]"),
  parser.add_option("--outdir",action="store",dest="outdir",type="string",
                    default="",help="Outdir [default = %default]"),
  parser.add_option("--do_var",action="store_true",dest="do_var",
                    default=False,help="Do variable binnign [default = %default]"),

  # end option list 
  (options, args) = parser.parse_args()

  # run 
  run(options)

if __name__ == "__main__":
  init()
