import sys, os, re

model = "2HDM"
which = "tt"
indir = 'json/'+which+'_'+model+'_results/' 
if model=="2HDM": xsecfile = open('crosssectionZp2HDM.txt','r')
if model=="BARY": xsecfile = open('crosssectionZpBaryonic.txt','r')
limitpts = os.listdir(indir)
outdir   = which+'_'+model+'_weighted_results'
if not(os.path.isdir(outdir)): os.mkdir(outdir) 

# pick up limit files
regex = re.compile('Zprime.*json')
lfile = filter(regex.match,limitpts)
#print lfile

m_Z  = []
m_A  = []
xsec = []
mass = []
xsecs = {}

# read in xsecs
for line in xsecfile:
  line = line.rstrip('\n')
  line = line.split(' ')
  m_Z.append(line[0])
  m_A.append(line[1])
  xsec.append(line[2]) 
  massstr = line[0]+'_'+line[1]
  mass.append(massstr)
  xsecs[massstr] = line[2] 

# loop over jsons
for l in lfile: 
  name = l.replace('Zprime','')
  name = name.replace('.json','')
  name = name.split('A')
  zp   = name[0]
  a0   = name[1]
  findmass = zp+'_'+a0
  if findmass not in mass: continue # don't make json for pts that don't have xsec
  xsecwgt  = xsecs[findmass] 

  # open new file
  json_orig = open(indir+l,'r')
  json_new  = open(outdir+'/'+l,'w')

  # loop over 
  for limitline in json_orig:
    limitline = limitline.rstrip('\n')
    limitline = limitline.rstrip(',')
    if ':' in limitline and '{' not in limitline:
      values = limitline.split(':')
      value  = values[1]
      numval = float(value)
      wgtval = numval/float(xsecwgt)
      #print wgtval
      if 'obs' in limitline:  json_new.write(values[0]+': '+str(wgtval)+'\n')
      else:                   json_new.write(values[0]+': '+str(wgtval)+',\n')
    else:
      json_new.write(limitline+'\n') 

