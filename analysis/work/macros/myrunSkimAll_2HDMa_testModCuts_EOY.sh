#! /bin/sh

#path="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WrongWeightTest/Copy"
#path="/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/2HDMa_ma150/WithBranchingRatio/newSkim/"
#path2="/eos/user/d/dbhowmik/Analysis/PrivateSampleProd/2HDMa/2HDMa_ma150/WithBranchingRatio/newSkim/testModCuts"

path="/eos/cms/store/group/phys_egamma/shdutta/2018Analysis/WorkCopy/for_skim"


#path2="/eos/cms/store/group/phys_egamma/shdutta/2018Analysis/WorkCopy/for_skim/skimmed_LowMET_ptgg40"
path2="/eos/cms/store/group/phys_egamma/shdutta/2018Analysis/WorkCopy/for_skim/skimmed_HighMET"


root -l -b << EOF
.L ../scripts/skim_testModCuts.C

skim("${path}","${path2}","nTuple_DoubleEG");

skim("${path}","${path2}","nTuple_GluGluHToGG");
skim("${path}","${path2}","nTuple_VBFHToGG");
skim("${path}","${path2}","nTuple_VHToGG");
skim("${path}","${path2}","nTuple_ttHJetToGG");

skim("${path}","${path2}","nTuple_DiPhotonJetsBox");
skim("${path}","${path2}","nTuple_DYJetsToLL");

skim("${path}","${path2}","nTuple_QCD");
skim("${path}","${path2}","nTuple_GJet");

skim("${path}","${path2}","nTuple_TTGJets");
skim("${path}","${path2}","nTuple_TTGG_0Jets");
skim("${path}","${path2}","nTuple_TTJets");
skim("${path}","${path2}","nTuple_TGJets");

skim("${path}","${path2}","nTuple_WGToLNuG");
skim("${path}","${path2}","nTuple_TTWJetsToLNu");


skim("${path}","${path2}","nTuple_2017_2HDMa_mA200_ma150_EOY");
skim("${path}","${path2}","nTuple_2017_2HDMa_mA300_ma150_EOY");
skim("${path}","${path2}","nTuple_2017_2HDMa_mA400_ma150_EOY");
skim("${path}","${path2}","nTuple_2017_2HDMa_mA500_ma150_EOY");
skim("${path}","${path2}","nTuple_2017_2HDMa_mA600_ma150_EOY");


.q

EOF

echo "done skimming"
