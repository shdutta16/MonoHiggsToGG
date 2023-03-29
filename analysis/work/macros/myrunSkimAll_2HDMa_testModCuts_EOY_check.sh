#! /bin/sh

#path="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WrongWeightTest/Copy"
#path="/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/2HDMa_ma150/WithBranchingRatio/newSkim/"
#path2="/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/2HDMa_ma150/WithBranchingRatio/newSkim/testModCuts"

path="/eos/cms/store/group/phys_egamma/shdutta/2018Analysis/WorkCopy/for_skim/Debabrata"


#path2="/eos/cms/store/group/phys_egamma/shdutta/2018Analysis/WorkCopy/for_skim/skimmed_LowMET_ptgg40"
path2="/eos/cms/store/group/phys_egamma/shdutta/2018Analysis/WorkCopy/for_skim/Debabrata/skimmed"


root -l -b << EOF
.L ../scripts/skim_testModCuts.C

skim("${path}","${path2}","nTuple_2017_2HDMa_mA300_ma150_EOY");

.q

EOF

echo "done skimming"
