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

 
   outputname1 = outputdir+'/'+outputfile+'_absEta_0_1.txt'
   outputname2 = outputdir+'/'+outputfile+'_absEta_1_14.txt'
   outputname3 = outputdir+'/'+outputfile+'_absEta_15_2.txt'
   outputname4 = outputdir+'/'+outputfile+'_absEta_2_25.txt'
   #check that output directory exists and if not create it
   dir = os.path.dirname(outputname1)
   if not os.path.exists(dir):
      os.makedirs(dir)   

   target1 = open(outputname1,'w') # output file for eta 0-1 
   target2 = open(outputname2,'w') # output file for eta 1-1.4
   target3 = open(outputname3,'w') # output file for eta 1.5-2
   target4 = open(outputname4,'w') # output file for eta 2-2.5
          
   lines = hand.readlines()
   hand.close()

   # separate the output into low and high R9 
   for i in range(0, len(lines)):
      line = lines[i]
      if 'absEta_0_1' in line:
        target1.write(lines[i])
      if 'absEta_1_1' in line:
        target2.write(lines[i])
      if '566_2' in line:
        target3.write(lines[i])
      if 'absEta_2_2' in line:
        target4.write(lines[i])
   target1.close()        
   target2.close()        
   target3.close()        
   target4.close()        
   print 'Output file is ', outputname1
   print 'Output file is ', outputname2
   print 'Output file is ', outputname3
   print 'Output file is ', outputname4

if __name__ == "__main__":
   main(sys.argv[1:])
