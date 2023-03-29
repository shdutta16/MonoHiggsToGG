import os

# input directory
indir  = 'combo_2HDM_bash_clean/'
outdir = 'combo_2HDM_results/' 

# range of inputs
mZp = range(600,2050,50)
mA  = range(300,725,25)

# setup name
rename = []
diname = {}
zpname = {}
a0name = {}
for z in mZp:
  for a in mA:
    rename.append('Zprime'+str(z)+'A'+str(a))
for n in rename:
    breakname = n.replace('Zprime','')
    breakname = breakname.split('A')
    zpname[n] = breakname[0]
    a0name[n] = breakname[1]
    diname[n] = indir+'combo_'+model+'_m'+a0name[n]+'/results_'+a0name[n]+'/'

for n in rename:
   cmd = "combineTool.py -M CollectLimits "+n+" "+diname[n]+"*.limit.*"+zpname[n]+"* -o "+outdir+n+".json"
   os.system(cmd)
