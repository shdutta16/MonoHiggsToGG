#! /bin/sh

#path="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/"
path="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WrongWeightTest"
#path="/afs/cern.ch/user/m/mzientek/private/MonoHggCMSSW80X/CMSSW_8_0_8_patch1/src/MonoHiggsToGG/analysis/"
#path="~/private/MonoHggCMSSW80X/CMSSW_8_0_8_patch1/src/MonoHiggsToGG/analysis/work/macros/"
sample=$1

echo "Running sample: " ${sample}
root -l -b -q "../scripts/skim.C("\"${path}\"","\"${sample}\"")"

echo "done with skimming"
