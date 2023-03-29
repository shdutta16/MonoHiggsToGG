import sys, numpy, ROOT
from ROOT import *
from optparse import OptionParser

def run(opts):

  # input files
  fin1 = open(opts.fin1,"r")
  fin2 = open(opts.fin2,"r")
  # output files
  fout = open("CommonEvents.txt","w")

  # setup arrays  
  run1  = []
  lumi1 = []
  even1 = [] 
  run2  = []
  lumi2 = []
  even2 = [] 
 
  # read fin1
  for line1 in fin1:
    line1 = line1.rstrip('\n')
    line1 = line1.split('\t')
    if (line1[0]=="Event:"):
      run1.append(line1[1])
      lumi1.append(line1[2])
      even1.append(line1[3]) 

  # read fin2
  for line2 in fin2:
    line2 = line2.rstrip('\n')
    line2 = line2.split('*')
    if (line2[0]==''):
      run2.append(line2[2].strip())
      lumi2.append(line2[3].strip())
      even2.append(line2[4].strip())
 
  # loop over events
  for ev1 in range(0,len(run1)): 
    for ev2 in range(0,len(run2)):
      if (run1[ev1]==run2[ev2] and lumi1[ev1]==lumi2[ev2] and even1[ev1]==even2[ev2]):
        print "Same event!"
        fout.write("----------------------------------- \n")
        fout.write("Event(1) %s\t%s\t%s \n"%(run1[ev1],lumi1[ev1],even1[ev1]))
        fout.write("Event(2) %s\t%s\t%s \n"%(run2[ev2],lumi2[ev2],even2[ev2]))
        fout.write("----------------------------------- \n")
      

def init():
  # options
  parser = OptionParser("usage: %prog [options]")
  parser.add_option("--f1",action="store",dest="fin1",type="string",
                    default="",help="File1"),
  parser.add_option("--f2",action="store",dest="fin2",type="string",
                    default="",help="File2"),
  (options, args) = parser.parse_args()

  #run
  run(options)

if __name__=="__main__":
  init()
