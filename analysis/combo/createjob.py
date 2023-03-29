import sys, os

def run(model,mass):

  infile  = open('submitjob.tmpl','r')
  outfile = open('submitjob_'+model+'_'+mass+'.tmpl','w')
  for line in infile:
    lineout = line.replace('MODEL',model)
    lineout = lineout.replace('MASS',mass)
    outfile.write(lineout)

if __name__=="__main__":

  model=sys.argv[1]
  mass=sys.argv[2]
  run(model,mass)
