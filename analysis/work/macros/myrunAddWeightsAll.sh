#! /bin/sh

#path="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/"
path="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WeightTest2/"
#path="../../macro/data/25ns_v80X_v5/"
weight=$1

root -l -b << EOF
.L ../scripts/myaddWeights.C++

addWeights(${weight},"${path}","DoubleEG", 1);

addWeights(${weight},"${path}","GluGluHToGG", 0.089937);
addWeights(${weight},"${path}","VHToGG", 0.0066069);
addWeights(${weight},"${path}","VBFHToGG", 0.0107784);
addWeights(${weight},"${path}","ttHToGG", 0.00137673);

addWeights(${weight},"${path}","DiPhoton_M80", 87.62);
addWeights(${weight},"${path}","QCD_Pt30toInf", 240700.0);
addWeights(${weight},"${path}","QCD_Pt30to40", 24710.0);
addWeights(${weight},"${path}","QCD_Pt40toInf", 117700.0);
addWeights(${weight},"${path}","GJet_Pt20toInf", 3178.0);
addWeights(${weight},"${path}","GJet_Pt20to40", 233.3);
addWeights(${weight},"${path}","GJet_Pt40toInf", 871.4);
addWeights(${weight},"${path}","DYJetsToLL", 6467.0);
addWeights(${weight},"${path}","TTGG_0Jets", 0.01687);
addWeights(${weight},"${path}","TTGJets", 4.098);
addWeights(${weight},"${path}","TTJets", 744.0);

addWeights(${weight},"${path}","2HDM_mZP800_mA0300", 0.217);

.q

EOF

echo "Finished adding weights!"
