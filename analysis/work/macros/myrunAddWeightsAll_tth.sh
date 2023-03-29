#! /bin/sh

#path="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/"
path="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WeightTest2/"
#path="../../macro/data/25ns_v80X_v5/"
weight=$1


root -l -b << EOF
.L ../scripts/myaddWeights.C++


addWeights(${weight},"${path}","ttHJetToGG", 0.00137673);

.q

EOF

echo "Finished adding weights!"
