#!/usr/bin/python

import sys, getopt, re, os

def main(argv):
   inputfile = ''
   outputfile = ''
   outputdir = ''
   era = ''
   suffix = ''
   try:
      opts, args = getopt.getopt(argv,"hi:o:e:s:d:",["ifile=","ofile=","era=","suffix=","odir="])
   except getopt.GetoptError:
      print 'extractJSONS.py -i <inputfile> -o <outputfile> -e <era> -s <suffix> -d <outputdir>'
      sys.exit(1)
   options = []
   for opt, arg in opts:
      options.append(opt)
      if opt == '-h':
         print 'extractJSONS.py -i <inputfile> -o <outputfile> -e <era> -s <suffix> -d <outputdir>'
         sys.exit()
      elif opt in ("-i", "--ifile"):
         inputfile = arg
      elif opt in ("-o", "--ofile"):
         outputfile = arg
      elif opt in ("-e", "--era"):
         era = arg
      elif opt in ("-s", "--suffix"):
         suffix = arg
      elif opt in ("-d", "--odir"):
         outputdir = arg+'/'
   if "-s" not in options and "--suffix" not in options:
      suffix = outputfile
    

   print 'Input file is ', inputfile
   hand = open(inputfile) # open catalogue from Flashgg

   outputname = outputdir+suffix+'.json'
   #check that output directory exists and if not create it
   dir = os.path.dirname(outputname)
   if not os.path.exists(dir):
      os.makedirs(dir)   

   target = open(outputname,'w') #output file with the EOS file of dataset to be analyzed
          
   lines = hand.readlines()
   hand.close()

   #print out the line+next 2 (so name,nentries,weight)
   for i in range(0, len(lines)):
      line = lines[i]
      if outputfile in line and era in line and i+2 < len(lines) and i > 0:
         if 'true,' in lines[i-1]:
           line = re.sub('                "name": "', '', line)
           print 'Removing bad file ', line
	 else:
           target.write(lines[i])
           target.write(lines[i+1])
           target.write(lines[i+2])
           target.write(lines[i+3])

   target.close()        
   print 'Output file is ', outputname

if __name__ == "__main__":
   main(sys.argv[1:])
