import sys, os
from optparse import OptionParser

def run():

  # option list
  parser = OptionParser("Usage: %prog [options]")
  parser.add_option("-f","--infile",action="store",dest="filename",type="string",
                    default="",help="Name of datacard to replace"),
  parser.add_option("-m","--mass",action="store",dest="mass",type="string",
                    default="",help="Mass"),
  (opts,args) = parser.parse_args()

  # open file
  infile  = open(opts.filename,'r')
  outname = opts.filename.replace('.txt','')
  outname += '_new.txt'
  outfile = open(outname,'w')
  # put mass in line 
  for line in infile:
    lineout = line.replace('$MASS',opts.mass) 
    outfile.write(lineout)
  print('Made cleaned datacard: %s' %outname)
 
if __name__ == "__main__":
  run()
