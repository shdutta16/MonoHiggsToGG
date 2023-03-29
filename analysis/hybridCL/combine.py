#!env python

import os,sys
from glob import glob
from subprocess import call, check_output
import re
import threading
import time

threads=[]

def drange(start, stop, step):
        ''' Return a floating range list. Start and stop are included in the list if possible.'''
        r = start
        while r <= stop:
                yield r
                r += step

from optparse import OptionParser, OptionGroup

usage = "usage: %prog [options]"
parser=OptionParser(usage=usage)
parser.add_option("-i","--input" ,dest='input',type='string',help="Input Datacard [Default=%default]",default="")
parser.add_option("-d","--dir" ,dest='dir',type='string',help="Directory where to write the configuration [Default=%default]",default="submit")
#parser.add_option("-n","--njobs" ,dest='njobs',type='int',help="Number of Job to submit [Default=%default]",default=50)
parser.add_option("-t","--ntoys" ,dest='ntoys',type='int',help="Number of Toys to submit [Default=%default]",default=1)
parser.add_option("","--begin" ,dest='begin',type='float',help="Begin Mass [Default=%default]",default=200)
parser.add_option("","--end" ,dest='end',type='float',help="End Mass [Default=%default]",default=900)
parser.add_option("","--step" ,dest='step',type='float',help="Step [Default=%default]",default=100)
parser.add_option("-q","--queue" ,dest='queue',type='string',help="Queue [Default=%default]",default="1nh")
parser.add_option("","--dryrun" ,dest='dryrun',action='store_true',help="Do not Submit [Default=%default]",default=False)
parser.add_option("-e","--expected" ,dest='exp',action='store_true',help="Run Expected [Default=%default]",default=True)
parser.add_option("-u","--unblind" ,dest='exp',action='store_false',help="Unblind Results")
parser.add_option("","--ncore",type='int',help="num. of core. [%default]",default=4)
parser.add_option("","--rmax",type='float',help="rmax. [%default]",default=1000000)
parser.add_option("","--onews",action='store_true',help="Only one ws. names do not depend on the higgs mass. [%default]",default=False)
parser.add_option("","--nosyst",action='store_true',help="No Syst.. [%default]",default=False)
parser.add_option("-M","--method" ,dest='method',type='string',help="Method [%default]",default="Asymptotic")
parser.add_option("","--doquantile",action='store_true',help="Run grid for a specific quantile [%default]", default=False) 
parser.add_option("","--quantile",dest='quantile',type='string',help="Choose a specific quantile [Default=%default]",default="0.5")
(opts,args)=parser.parse_args()

onews=0 ## for onews

EOS='/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select'

print "inserting in path cwd"
sys.path.insert(0,os.getcwd())
print "inserting in path cwd/python"
sys.path.insert(0,os.getcwd()+'/python')

print "inserting combine in path"
if not os.path.exists(os.path.expandvars('$CMSSW_BASE/bin/$SCRAM_ARCH/combine')):
        sys.exit('ERROR - CombinedLimit package must be installed')
if not os.path.exists(os.path.expandvars('$CMSSW_BASE/bin/$SCRAM_ARCH/text2workspace.py')):
        sys.exit('ERROR - CombinedLimit package must be installed')
if not os.path.exists(os.path.expandvars('$CMSSW_BASE/bin/$SCRAM_ARCH/combineCards.py')):
        sys.exit('ERROR - CombinedLimit package must be installed')


call("[ -d %s ] && rmdir %s"%(opts.dir,opts.dir),shell=True)
call("mkdir -p %s"%opts.dir,shell=True)

#for iJob in range(0,opts.njobs):
cmdFile = open("%s/cmdFile.sh"%opts.dir,"w")
iJob = -1

def parallel(text2ws, bsub=""):
	print "-> Parallel command :'"+text2ws+"'"
	if text2ws != "/bin/true":
		st = call(text2ws,shell=True);
	else: 
		st =0

	if st !=0 :
		print "ERROR in executing txt2ws"
		return 0
	if bsub !="":
		print "-> submitting",bsub
		call(bsub,shell=True)
	return 0 

rrange=[0.1, 0.10982,0.119639, 0.129459, 0.139279, 0.149098, 0.158918, 0.168737, 0.178557, 0.188377, 0.198196, 0.208016, 0.217836, 0.227655, 0.237475, 0.247295, 0.257114, 0.266934, 0.276754, 0.286573, 0.296393, 0.306212, 0.316032, 0.325852, 0.335671, 0.345491, 0.355311, 0.36513, 0.37495, 0.38477, 0.394589, 0.404409, 0.414228, 0.424048, 0.433868, 0.443687, 0.453507, 0.463327, 0.473146, 0.482966, 0.492786, 0.502605, 0.512425, 0.522244, 0.532064, 0.541884, 0.551703, 0.561523, 0.571343, 0.581162, 0.590982, 0.600802, 0.610621, 0.620441, 0.630261, 0.64008, 0.6499, 0.659719, 0.669539, 0.679359, 0.689178, 0.698998, 0.708818, 0.718637, 0.728457, 0.738277, 0.748096, 0.757916, 0.767735, 0.777555, 0.787375, 0.797194, 0.807014, 0.816834, 0.826653, 0.836473, 0.846293, 0.856112, 0.865932, 0.875752, 0.885571, 0.895391, 0.90521, 0.91503, 0.92485, 0.934669, 0.944489, 0.954309, 0.964128, 0.973948, 0.983768, 0.993587, 1.00341, 1.01323, 1.02305, 1.03287, 1.04269, 1.05251, 1.06232, 1.07214, 1.08196, 1.09178, 1.1016, 1.11142, 1.12124, 1.13106, 1.14088, 1.1507, 1.16052, 1.17034, 1.18016, 1.18998, 1.1998, 1.20962, 1.21944, 1.22926, 1.23908, 1.2489, 1.25872, 1.26854, 1.27836, 1.28818, 1.298, 1.30782, 1.31764, 1.32745, 1.33727, 1.34709, 1.35691, 1.36673, 1.37655, 1.38637, 1.39619, 1.40601, 1.41583, 1.42565, 1.43547, 1.44529, 1.45511, 1.46493, 1.47475, 1.48457, 1.49439, 1.50421, 1.51403, 1.52385, 1.53367, 1.54349, 1.55331, 1.56313, 1.57295, 1.58277, 1.59259, 1.6024, 1.61222, 1.62204, 1.63186, 1.64168, 1.6515, 1.66132, 1.67114, 1.68096, 1.69078, 1.7006, 1.71042, 1.72024, 1.73006, 1.73988, 1.7497, 1.75952, 1.76934, 1.77916, 1.78898, 1.7988, 1.80862, 1.81844, 1.82826, 1.83808, 1.8479, 1.85772, 1.86754, 1.87735, 1.88717, 1.89699, 1.90681, 1.91663, 1.92645, 1.93627, 1.94609, 1.95591, 1.96573, 1.97555, 1.98537, 1.99519, 2.00501, 2.01483, 2.02465, 2.03447, 2.04429, 2.05411, 2.06393, 2.07375, 2.08357, 2.09339, 2.10321, 2.11303, 2.12285, 2.13267, 2.14248, 2.1523, 2.16212, 2.17194, 2.18176, 2.19158, 2.2014, 2.21122, 2.22104, 2.23086, 2.24068, 2.2505, 2.26032, 2.27014, 2.27996, 2.28978, 2.2996, 2.30942, 2.31924, 2.32906, 2.33888, 2.3487, 2.35852, 2.36834, 2.37816, 2.38798, 2.3978, 2.40762, 2.41743, 2.42725, 2.43707, 2.44689, 2.45671, 2.46653, 2.47635, 2.48617, 2.49599, 2.50581, 2.51563, 2.52545, 2.53527, 2.54509, 2.55491, 2.56473, 2.57455, 2.58437, 2.59419, 2.60401, 2.61383, 2.62365, 2.63347, 2.64329, 2.65311, 2.66293, 2.67275, 2.68257, 2.69238, 2.7022, 2.71202, 2.72184, 2.73166, 2.74148, 2.7513, 2.76112, 2.77094, 2.78076, 2.79058, 2.8004, 2.81022, 2.82004, 2.82986, 2.83968, 2.8495, 2.85932, 2.86914, 2.87896, 2.88878, 2.8986, 2.90842, 2.91824, 2.92806, 2.93788, 2.9477, 2.95752, 2.96733, 2.97715, 2.98697, 2.99679, 3.00661, 3.01643, 3.02625, 3.03607, 3.04589, 3.05571, 3.06553, 3.07535, 3.08517, 3.09499, 3.10481, 3.11463, 3.12445, 3.13427, 3.14409, 3.15391, 3.16373, 3.17355, 3.18337, 3.19319, 3.20301, 3.21283, 3.22265, 3.23246, 3.24228, 3.2521, 3.26192, 3.27174, 3.28156, 3.29138, 3.3012, 3.31102, 3.32084, 3.33066, 3.34048, 3.3503, 3.36012, 3.36994, 3.37976, 3.38958, 3.3994, 3.40922, 3.41904, 3.42886, 3.43868, 3.4485, 3.45832, 3.46814, 3.47796, 3.48778, 3.4976, 3.50741, 3.51723, 3.52705, 3.53687, 3.54669, 3.55651, 3.56633, 3.57615, 3.58597, 3.59579, 3.60561, 3.61543, 3.62525, 3.63507, 3.64489, 3.65471, 3.66453, 3.67435, 3.68417, 3.69399, 3.70381, 3.71363, 3.72345, 3.73327, 3.74309, 3.75291, 3.76273, 3.77255, 3.78236, 3.79218, 3.802, 3.81182, 3.82164, 3.83146, 3.84128, 3.8511, 3.86092, 3.87074, 3.88056, 3.89038, 3.9002, 3.91002, 3.91984, 3.92966, 3.93948, 3.9493, 3.95912, 3.96894, 3.97876, 3.98858, 3.9984, 4.00822, 4.01804, 4.02786, 4.03768, 4.04749, 4.05731, 4.06713, 4.07695, 4.08677, 4.09659, 4.10641, 4.11623, 4.12605, 4.13587, 4.14569, 4.15551, 4.16533, 4.17515, 4.18497, 4.19479, 4.20461, 4.21443, 4.22425, 4.23407, 4.24389, 4.25371, 4.26353, 4.27335, 4.28317, 4.29299, 4.30281, 4.31263, 4.32244, 4.33226, 4.34208, 4.3519, 4.36172, 4.37154, 4.38136, 4.39118, 4.401, 4.41082, 4.42064, 4.43046, 4.44028, 4.4501, 4.45992, 4.46974, 4.47956, 4.48938, 4.4992, 4.50902, 4.51884, 4.52866, 4.53848, 4.5483, 4.55812, 4.56794, 4.57776, 4.58758, 4.59739, 4.60721, 4.61703, 4.62685, 4.63667, 4.64649, 4.65631, 4.66613, 4.67595, 4.68577, 4.69559, 4.70541, 4.71523, 4.72505, 4.73487, 4.74469, 4.75451, 4.76433, 4.77415, 4.78397, 4.79379, 4.80361, 4.81343, 4.82325, 4.83307, 4.84289, 4.85271, 4.86253, 4.87234, 4.88216, 4.89198, 4.9018, 4.91162, 4.92144, 4.93126, 4.94108, 4.9509, 4.96072, 4.97054, 4.98036, 4.99018, 5]
for ri in rrange:
		print ri

for mass in drange(opts.begin,opts.end,opts.step):
	
	for ri in rrange:
		print ri
		iJob += 1
		basedir=opts.dir
		if basedir[0] != '/': basedir=os.environ['PWD'] + "/" + opts.dir
		
		sh=open("%s/sub%d.sh"%(opts.dir,iJob),"w")
		
		call(["chmod","u+x","%s/sub%d.sh"%(opts.dir,iJob)])
		
	## Open File and write Preamble
		sh.write('#!/bin/bash\n')
		sh.write('[ "$WORKDIR" == "" ] && export WORKDIR="/tmp/%s/" \n'%(os.environ['USER']))
		sh.write('rm -v $WORKDIR/higgs*root\n')  ## make sure there is no residual in the WORKDIR
		sh.write('cd %s\n'%(os.getcwd() ) )
		sh.write('LD_LIBRARY_PATH=%s:$LD_LIBRARY_PATH\n'%os.getcwd())
		
	#print " WORK-AROUND COMBINE: FIXME!!! "
	#sh.write('cd ~amarini/work/ProductionJanuary2014/CMSSW_6_1_1_CategoryFull/src\n')
		sh.write('eval `scramv1 runtime -sh`\n') # cmsenv

	## Touch control files
		touch = "touch " + basedir + "/sub%d.pend"%iJob
		call(touch,shell=True)
		cmd = "rm " + basedir + "/sub%d.run 2>/dev/null >/dev/null"%iJob
		call(cmd,shell=True)
		cmd = "rm " + basedir + "/sub%d.done 2>/dev/null >/dev/null"%iJob
		call(cmd,shell=True)
		cmd = "rm " + basedir + "/sub%d.fail 2>/dev/null >/dev/null"%iJob
		call(cmd,shell=True)
		
		sh.write('date > %s/sub%d.run\n'%(basedir,iJob))
		sh.write('rm %s/sub%d.done\n'%(basedir,iJob))
		sh.write('rm %s/sub%d.pend\n'%(basedir,iJob))
		sh.write('rm %s/sub%d.fail\n'%(basedir,iJob))
		
		sh.write('cd $WORKDIR\n')

	## Construct Datacard
		datacard= re.sub( ".txt","_M%.0f.root"%mass,opts.input.split("/")[-1])
		cmd = "text2workspace.py -m " + str(mass) + " -o " + opts.dir + "/" + datacard +  " " + opts.input
		if opts.onews:
			datacard= re.sub( ".txt",".root",opts.input.split("/")[-1])
			cmd = "text2workspace.py -o "+ opts.dir+"/"+datacard +" " + opts.input
			if onews >0 : cmd="/bin/true"
			onews+=1

		if opts.ncore <2:
			if cmd != "/bin/true":
				print "-> Running command :'"+cmd+"'"
				call(cmd,shell=True)
	       	else:
			text2ws = cmd[:]
	
		## copy datacard in workdir
		sh.write('cp -v '+ basedir + "/"+  datacard + " ./ \n" )
		##Write combine line
	        #combine -M Asymptotic -m 200 -S 0 --run=expected --expectSignal=1 --expectSignalMass=200  cms_datacard_chhiggs_taunu.txt
		if opts.doquantile:
			combine= 'combine -M HybridNew --freq -m ' + str(mass) +' --fork 4 -T 500 --clsAcc 0 -v 0 -n 2HDM_mZP' + str(mass) +'_mA0300_' + str(ri)+' --saveHybridResult --expectedFromGrid=' +opts.quantile+' -s ' + str(iJob*1000) + ' -i 50 --singlePoint ' + str(ri)+' '
		else:
			combine= 'combine -M HybridNew --freq -m ' + str(mass) +' --fork 4 -T 500 --clsAcc 0 -v 0 -n 2HDM_mZP' + str(mass) +'_mA0300_' + str(ri)+' --saveHybridResult -s ' + str(iJob*1000) + ' -i 50 --singlePoint ' + str(ri)+' '
		
		combine += datacard
		print combine
		sh.write( combine + "\n" )
		sh.write('EXITCODE=$?\n')
		sh.write('[ $EXITCODE == 0 ] && touch %s/sub%d.done\n'%(basedir,iJob))
		sh.write('[ $EXITCODE != 0 ] && echo $EXITCODE > %s/sub%d.fail\n'%(basedir,iJob))
		
		sh.write('cp -v higgs*root %s/\n'%basedir)
		sh.write('rm %s/sub%d.run\n'%(basedir,iJob))
		
		cmdline = "bsub -q " + opts.queue + " -o %s/log%d.txt"%(basedir,iJob) + " -J " + "%s/Job_%d"%(opts.dir,iJob) + " %s/sub%d.sh"%(basedir,iJob)
		cmdFile.write(cmdline+"\n")

		bsub=""
		if not opts.dryrun: 
			if opts.ncore<2:
				print cmdline
				call(cmdline,shell=True)
			else:
				bsub=cmdline[:]

			if opts.ncore>=2:
				if not opts.onews:
					while threading.activeCount() >= opts.ncore:
						print "sleep ....",
						time.sleep(1)
						print "wake up"
				t= threading.Thread(target=parallel,args=(text2ws,bsub,) )
				t.start()
				threads.append(t)
					
				if opts.onews and text2ws !="/bin/true":
					print "-> for onews I'll wait the text2ws to end"
					t.join()
	for t in threads:
				t.join()
				
	print "Done"
