#! /bin/sh

#path="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WrongWeightTest/Copy"
path="/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/2HDMa_ma150/WithBranchingRatio/newSkim/"
path2="/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/2HDMa_ma150/WithBranchingRatio/newSkim/testModCuts"

root -l -b << EOF
.L ../scripts/skim_testModCuts.C++

skim("${path}","${path2}","nTUple_2HDMa_mA200_ma150");
skim("${path}","${path2}","nTUple_2HDMa_mA300_ma150");
skim("${path}","${path2}","nTUple_2HDMa_mA400_ma150");
skim("${path}","${path2}","nTUple_2HDMa_mA500_ma150");
skim("${path}","${path2}","nTUple_2HDMa_mA600_ma150");


.q

EOF

echo "done skimming"
