#! /bin/sh

#path="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/"
#path="../macro/data/25ns_v80X_v5/"
#path="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WrongWeightTest/Copy"
path="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WeightTest2"

root -l -b << EOF
.L ../scripts/skim.C++

skim("${path}","DoubleEG");

skim("${path}","GluGluHToGG");
skim("${path}","VHToGG");
skim("${path}","VBFHToGG");
skim("${path}","ttHJetToGG");

skim("${path}","DiPhoton");
skim("${path}","QCD");
skim("${path}","GJet");

skim("${path}","DYJetsToLL");
skim("${path}","TTGG_0Jets");
skim("${path}","TTGJets");
skim("${path}","TTJets");

skim("${path}","2HDM_mZP800_mA0300");

.q

EOF

echo "done skimming"
