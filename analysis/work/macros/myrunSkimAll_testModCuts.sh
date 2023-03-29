#! /bin/sh

#path="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/"
#path="../macro/data/25ns_v80X_v5/"
#path="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WrongWeightTest/Copy"
path="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WeightTest2"
path2="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WeightTest2/testModCuts"
root -l -b << EOF
.L ../scripts/skim_testModCuts.C++

skim("${path}","${path2}","DoubleEG");

skim("${path}","${path2}","GluGluHToGG");
skim("${path}","${path2}","VHToGG");
skim("${path}","${path2}","VBFHToGG");
skim("${path}","${path2}","ttHJetToGG");

skim("${path}","${path2}","DiPhoton");
skim("${path}","${path2}","QCD");
skim("${path}","${path2}","GJet");

skim("${path}","${path2}","DYJetsToLL");
skim("${path}","${path2}","TTGG_0Jets");
skim("${path}","${path2}","TTGJets");
skim("${path}","${path2}","TTJets");

.q

EOF

echo "done skimming"
