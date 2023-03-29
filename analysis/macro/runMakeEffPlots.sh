#! /bin/sh

#inputDir="data/25ns_v1-1-0_ReReco_wAllJetInfo/"
inputDir="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_v3/"
outputDir="diPhoPlots/25ns_v80X_v3/Eff/"
ftype="png"

# pick up MC metCorr
#px = t1pfmet*cos(t1pfmetPhi) -(-1.33055 + -0.00700742 * t1pfmetSumEt)
#py = t1pfmet*sin(t1pfmetPhi) -(0.267872 + -0.00486139 * t1pfmetSumEt)
#MetCorr={-1.33055,-0.00700742,0.267872,-0.00486139}
MetCorr={-0.91,-0.0024,-0.71,0.0013}


mkdir -p $outputDir

root -l -b <<EOF
.L makeEffPlots.cc++

doEff("$inputDir","$outputDir","$ftype",34.7,$MetCorr);

.q

EOF
