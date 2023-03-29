#!/usr/bin/python

import sys, getopt, re, os

def main(argv):
   inputfile = ''
   outputfile = ''
   outputdir = ''
   try:
      opts, args = getopt.getopt(argv,"hi:o:d:",["ifile=","ofile=","odir="])
   except getopt.GetoptError:
      print 'Separate_HiLoR9_Scaling.py -i <inputfile> -o <outputfile> -d <outputdir>'
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print 'python Separate_HiLoR9_Scaling.py -i <inputfile> -o <outputfile> -d <outputdir>'
         sys.exit()
      elif opt in ("-i", "--ifile"):
         inputfile = arg
      elif opt in ("-o", "--ofile"):
         outputfile = arg
      elif opt in ("-d", "--odir"):
         outputdir = arg
   print 'Input file is ', inputfile
   hand = open(inputfile) # open catalogue 

 
   outputname1 = outputdir+'/'+outputfile+'_highR9.txt'
   outputname2 = outputdir+'/'+outputfile+'_lowR9.txt'
   #check that output directory exists and if not create it
   dir = os.path.dirname(outputname1)
   if not os.path.exists(dir):
      os.makedirs(dir)   

   target1 = open(outputname1,'w') # output file for highR9
   target2 = open(outputname2,'w') # output file for lowR9
          
   lines = hand.readlines()
   hand.close()

   # separate the output into low and high R9 
   for i in range(0, len(lines)):
      line = lines[i]
      if 'highR9' in line:
        target1.write(lines[i])
      if 'lowR9' in line:
        target2.write(lines[i])
   target1.close()        
   target2.close()        
   print 'Output file is ', outputname1
   print 'Output file is ', outputname2

if __name__ == "__main__":
   main(sys.argv[1:])
