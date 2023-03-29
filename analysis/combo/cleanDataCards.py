import sys, os
from optparse import OptionParser

def run(opts):
  # input file
  fin  = open(opts.indir+opts.fname,"r")
  fout = open(opts.outdir+opts.fname,"w")
  

  # loop over file lines
  for line in fin:
    line = line.rstrip('\n')
    if 'hgg_BR_mq' in line: continue                          # remove mq unc.
    elif 'RealMet'   in line: continue                        # remove real MET unc. 
    elif 'lumi  lnN' in line:                                 # fix lumi unc.
      newline = line.replace('lumi','CMS_lumi')               # -- rename lumi to match tautau 
      newline = newline.replace('1.023','1.025')              # -- fix lumi value
      fout.write(newline+'\n')
    # ---- FIXME ----
    # Unc. below may need to be correlate/anti-correlate w/ tautau 
    # ---------------
    elif 'hgg_xsec_theo' in line:                             # fix pdf+alphas+xsec 
      newline = line.replace('hgg_xsec_theo','h_pdf_as_theo') # -- change to just pdf+alphas
      newline = newline.replace('0.8/1.2   ','0.961/1.039')   # -- change to just pdf+alphas
      fout.write(newline+'\n')
      fout.write('h_scale_theo lnN'.ljust(20))                # -- add qcd scale 
      fout.write(' -'.ljust(15))
      fout.write(' 0.933/1.046'.ljust(15))
      fout.write(' 0.997/1.004'.ljust(15))
      fout.write(' 0.969/1.038'.ljust(15))
      fout.write(' 0.908/1.058'.ljust(15))
      fout.write(' -'.ljust(15)) 
      fout.write(' -'.ljust(15))
      fout.write(' 0.933/1.046'.ljust(15))
      fout.write(' 0.997/1.004'.ljust(15))
      fout.write(' 0.969/1.038'.ljust(15))
      fout.write(' 0.908/1.058'.ljust(15))
      fout.write(' -'.ljust(15)) 
      fout.write('\n')
      fout.write('h_xsec_theo  lnN'.ljust(20))                # -- separate diff xsec unc.
      fout.write(' -'.ljust(15))
      fout.write(' 0.8/1.2'.ljust(15))
      fout.write(' -'.ljust(15))
      fout.write(' -'.ljust(15))
      fout.write(' -'.ljust(15))
      fout.write(' -'.ljust(15))
      fout.write(' -'.ljust(15))
      fout.write(' 0.8/1.2'.ljust(15))
      fout.write(' -'.ljust(15))
      fout.write(' -'.ljust(15))
      fout.write(' -'.ljust(15))
      fout.write(' -'.ljust(15))
      fout.write('\n')
    else: # else - just copy line
      fout.write(line+'\n') 


def init():
  # options
  parser = OptionParser("usage: %prog [options]")
  parser.add_option("-i",action="store",dest="indir",type="string",
                    default="",help="Input directory"),
  parser.add_option("-f",action="store",dest="fname",type="string",
                    default="",help="Input file"),
  parser.add_option("-o",action="store",dest="outdir",type="string",
                    default="",help="Out directory"),
  (options, args) = parser.parse_args()

  #run
  run(options)

if __name__=="__main__":
   init()
