#! /bin/sh

#path="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/"
#path="../macro/data/25ns_v80X_v5/"
path="/eos/user/d/dbhowmik/Analysis/Bkg/newID_rightPU/WrongWeightTest/Copy"

root -l -b << EOF
.L ../scripts/skim.C++

skim("${path}","DoubleEG");

skim("${path}","GluGluHToGG");
skim("${path}","VHToGG");
skim("${path}","VBFHToGG");
skim("${path}","ttHJetToGG");

skim("${path}","DiPhoton");
skim("${path}","QCD");
skim("${path}","GJets");
skim("${path}","DYJetsToLL");
skim("${path}","NonResBkg");
skim("${path}","EWKBkg");
skim("${path}","TGJets");
skim("${path}","TTGG_0Jets");
skim("${path}","TTGJets");
skim("${path}","TTJets");
skim("${path}","WGToLNuG");
skim("${path}","ZGTo2LG");
skim("${path}","ZGTo2NuG");
skim("${path}","ZZTo2L2Q");
skim("${path}","WJetsToLNu");
skim("${path}","WZTo2L2Q");

skim("${path}","BaryonicZp_mZP10000_mChi1000");
skim("${path}","BaryonicZp_mZP10000_mChi10");
skim("${path}","BaryonicZp_mZP10000_mChi150");
skim("${path}","BaryonicZp_mZP10000_mChi1");
skim("${path}","BaryonicZp_mZP10000_mChi500");
skim("${path}","BaryonicZp_mZP10000_mChi50");
skim("${path}","BaryonicZp_mZP1000_mChi1000");
skim("${path}","BaryonicZp_mZP1000_mChi150");
skim("${path}","BaryonicZp_mZP1000_mChi1");
skim("${path}","BaryonicZp_mZP100_mChi10");
skim("${path}","BaryonicZp_mZP100_mChi1");
skim("${path}","BaryonicZp_mZP10_mChi1000");
skim("${path}","BaryonicZp_mZP10_mChi10");
skim("${path}","BaryonicZp_mZP10_mChi1");
skim("${path}","BaryonicZp_mZP10_mChi50");
skim("${path}","BaryonicZp_mZP15_mChi10");
skim("${path}","BaryonicZp_mZP1995_mChi1000");
skim("${path}","BaryonicZp_mZP2000_mChi1");
skim("${path}","BaryonicZp_mZP2000_mChi500");
skim("${path}","BaryonicZp_mZP200_mChi150");
skim("${path}","BaryonicZp_mZP200_mChi1");
skim("${path}","BaryonicZp_mZP200_mChi50");
skim("${path}","BaryonicZp_mZP20_mChi1");
skim("${path}","BaryonicZp_mZP295_mChi150");
skim("${path}","BaryonicZp_mZP300_mChi50");
skim("${path}","BaryonicZp_mZP500_mChi150");
skim("${path}","BaryonicZp_mZP500_mChi1");
skim("${path}","BaryonicZp_mZP500_mChi500");
skim("${path}","BaryonicZp_mZP50_mChi10");
skim("${path}","BaryonicZp_mZP50_mChi1");
skim("${path}","BaryonicZp_mZP995_mChi500");

skim("${path}","ScalarZp_mZP10000_mChi1");
skim("${path}","ScalarZp_mZP10000_mChi10");
skim("${path}","ScalarZp_mZP10000_mChi1000");
skim("${path}","ScalarZp_mZP10000_mChi150");
skim("${path}","ScalarZp_mZP10000_mChi50");
skim("${path}","ScalarZp_mZP10000_mChi500");
skim("${path}","ScalarZp_mZP1000_mChi1");
skim("${path}","ScalarZp_mZP1000_mChi150");
skim("${path}","ScalarZp_mZP1000_mChi1000");
skim("${path}","ScalarZp_mZP100_mChi1");
skim("${path}","ScalarZp_mZP100_mChi10");
skim("${path}","ScalarZp_mZP10_mChi1");
skim("${path}","ScalarZp_mZP10_mChi10");
skim("${path}","ScalarZp_mZP10_mChi1000");
skim("${path}","ScalarZp_mZP10_mChi150");
skim("${path}","ScalarZp_mZP10_mChi500");
skim("${path}","ScalarZp_mZP10_mChi50");
skim("${path}","ScalarZp_mZP15_mChi10");
skim("${path}","ScalarZp_mZP200_mChi1");
skim("${path}","ScalarZp_mZP200_mChi150");
skim("${path}","ScalarZp_mZP200_mChi50");
skim("${path}","ScalarZp_mZP20_mChi1");
skim("${path}","ScalarZp_mZP295_mChi150");
skim("${path}","ScalarZp_mZP300_mChi1");
skim("${path}","ScalarZp_mZP300_mChi50");
skim("${path}","ScalarZp_mZP500_mChi1");
skim("${path}","ScalarZp_mZP500_mChi150");
skim("${path}","ScalarZp_mZP500_mChi500");
skim("${path}","ScalarZp_mZP50_mChi1");
skim("${path}","ScalarZp_mZP50_mChi10");
skim("${path}","ScalarZp_mZP50_mChi50");
skim("${path}","ScalarZp_mZP95_mChi50");
skim("${path}","ScalarZp_mZP995_mChi500");

skim("${path}","2HDM_mZP1000_mA0300");
skim("${path}","2HDM_mZP1000_mA0400");
skim("${path}","2HDM_mZP1000_mA0500");
skim("${path}","2HDM_mZP1000_mA0600");
skim("${path}","2HDM_mZP1000_mA0700");
skim("${path}","2HDM_mZP1000_mA0800");
skim("${path}","2HDM_mZP1200_mA0300");
skim("${path}","2HDM_mZP1200_mA0400");
skim("${path}","2HDM_mZP1200_mA0500");
skim("${path}","2HDM_mZP1200_mA0600");
skim("${path}","2HDM_mZP1200_mA0700");
skim("${path}","2HDM_mZP1200_mA0800");
skim("${path}","2HDM_mZP1400_mA0300");
skim("${path}","2HDM_mZP1400_mA0400");
skim("${path}","2HDM_mZP1400_mA0500");
skim("${path}","2HDM_mZP1400_mA0600");
skim("${path}","2HDM_mZP1400_mA0700");
skim("${path}","2HDM_mZP1400_mA0800");
skim("${path}","2HDM_mZP1700_mA0300");
skim("${path}","2HDM_mZP1700_mA0400");
skim("${path}","2HDM_mZP1700_mA0500");
skim("${path}","2HDM_mZP1700_mA0600");
skim("${path}","2HDM_mZP1700_mA0700");
skim("${path}","2HDM_mZP1700_mA0800");
skim("${path}","2HDM_mZP2000_mA0300");
skim("${path}","2HDM_mZP2000_mA0400");
skim("${path}","2HDM_mZP2000_mA0500");
skim("${path}","2HDM_mZP2000_mA0600");
skim("${path}","2HDM_mZP2000_mA0700");
skim("${path}","2HDM_mZP2000_mA0800");
skim("${path}","2HDM_mZP2500_mA0300");
skim("${path}","2HDM_mZP2500_mA0400");
skim("${path}","2HDM_mZP2500_mA0500");
skim("${path}","2HDM_mZP2500_mA0600");
skim("${path}","2HDM_mZP2500_mA0700");
skim("${path}","2HDM_mZP2500_mA0800");
skim("${path}","2HDM_mZP600_mA0300");
skim("${path}","2HDM_mZP600_mA0400");
skim("${path}","2HDM_mZP800_mA0300");
skim("${path}","2HDM_mZP800_mA0400");
skim("${path}","2HDM_mZP800_mA0500");
skim("${path}","2HDM_mZP800_mA0600");
.q

EOF

echo "done skimming"
