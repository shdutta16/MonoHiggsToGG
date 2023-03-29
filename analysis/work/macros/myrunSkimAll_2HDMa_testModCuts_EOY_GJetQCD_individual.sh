#! /bin/sh

#path="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WrongWeightTest/Copy"
#path="/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/2HDMa_ma150/WithBranchingRatio/newSkim/"
#path2="/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/2HDMa_ma150/WithBranchingRatio/newSkim/testModCuts"

path="/eos/user/d/dbhowmik/Analysis/nTuple_2017_EOY/copy/for_skim"
#path2="/eos/user/d/dbhowmik/Analysis/nTuple_2017_EOY/copy/for_skim/skimmed_LowMET"
#path2="/eos/user/d/dbhowmik/Analysis/nTuple_2017_EOY/copy/for_skim/skimmed_HighMET"

path2="/eos/user/d/dbhowmik/Analysis/nTuple_2017_EOY/copy/for_skim/skimmed_LowMETHighMET_combinedTest/GJet_QCD_individual"

#.L ../scripts/skim_testModCuts.C++

root -l -b << EOF
.L ../scripts/skim_testModCuts_combinedMETtest.C

skim("${path}","${path2}","nTuple_QCD_Pt-30to40_EOY");
skim("${path}","${path2}","nTuple_QCD_Pt-40toInf_EOY");
skim("${path}","${path2}","nTuple_QCD_Pt-30toInf_EOY");

skim("${path}","${path2}","nTuple_GJet_Pt-20to40_EOY");
skim("${path}","${path2}","nTuple_GJet_Pt-40toInf_EOY");
skim("${path}","${path2}","nTuple_GJet_Pt-20toInf_EOY");

.q

EOF

echo "done skimming"
