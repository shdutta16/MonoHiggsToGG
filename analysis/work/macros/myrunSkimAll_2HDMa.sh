#! /bin/sh

#path="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WrongWeightTest/Copy"
path="/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/2HDMa_ma150/WithBranchingRatio/newSkim/"

root -l -b << EOF
.L ../scripts/skim.C++

skim("${path}","nTUple_2HDMa_mA200_ma150");
skim("${path}","nTUple_2HDMa_mA300_ma150");
skim("${path}","nTUple_2HDMa_mA400_ma150");
skim("${path}","nTUple_2HDMa_mA500_ma150");
skim("${path}","nTUple_2HDMa_mA600_ma150");


.q

EOF

echo "done skimming"
