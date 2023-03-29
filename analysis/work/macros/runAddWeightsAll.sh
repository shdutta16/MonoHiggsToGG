#! /bin/sh

path="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/"
#path="../../macro/data/25ns_v80X_v5/"
weight=$1


root -l -b << EOF
.L ../scripts/addWeights.C++

addWeights(${weight},"${path}","Run2016B");
addWeights(${weight},"${path}","Run2016C");
addWeights(${weight},"${path}","Run2016D");
addWeights(${weight},"${path}","Run2016E");
addWeights(${weight},"${path}","Run2016F");
addWeights(${weight},"${path}","Run2016G");
addWeights(${weight},"${path}","Run2016H_ver2");
addWeights(${weight},"${path}","Run2016H_ver3");

addWeights(${weight},"${path}","GluGluHToGG");
addWeights(${weight},"${path}","VHToGG");
addWeights(${weight},"${path}","VBFHToGG");
addWeights(${weight},"${path}","ttHToGG");

addWeights(${weight},"${path}","DiPhoton_M40");
addWeights(${weight},"${path}","DiPhoton_M80");
addWeights(${weight},"${path}","QCD_Pt-30toInf");
addWeights(${weight},"${path}","QCD_Pt-30to40");
addWeights(${weight},"${path}","QCD_Pt-40toInf");
addWeights(${weight},"${path}","GJet_Pt-20toInf");
addWeights(${weight},"${path}","GJet_Pt-20to40");
addWeights(${weight},"${path}","GJet_Pt-40toInf");
addWeights(${weight},"${path}","DYJetsToLL");
addWeights(${weight},"${path}","TGJets");
addWeights(${weight},"${path}","TTGG_0Jets");
addWeights(${weight},"${path}","TTGJets");
addWeights(${weight},"${path}","TTJets");
addWeights(${weight},"${path}","WGToLNuG");
addWeights(${weight},"${path}","WJetsToLNu");
addWeights(${weight},"${path}","WZTo2L2Q");
addWeights(${weight},"${path}","ZGTo2LG");
addWeights(${weight},"${path}","ZGTo2NuG");
addWeights(${weight},"${path}","ZZTo2L2Q");

addWeights(${weight},"${path}","ScalarZp_mZP10000_mChi1000");
addWeights(${weight},"${path}","ScalarZp_mZP10000_mChi10");
addWeights(${weight},"${path}","ScalarZp_mZP10000_mChi1");
addWeights(${weight},"${path}","ScalarZp_mZP10000_mChi150");
addWeights(${weight},"${path}","ScalarZp_mZP10000_mChi500");
addWeights(${weight},"${path}","ScalarZp_mZP10000_mChi50");
addWeights(${weight},"${path}","ScalarZp_mZP1000_mChi1000");
addWeights(${weight},"${path}","ScalarZp_mZP1000_mChi1");
addWeights(${weight},"${path}","ScalarZp_mZP1000_mChi150");
addWeights(${weight},"${path}","ScalarZp_mZP100_mChi10");
addWeights(${weight},"${path}","ScalarZp_mZP100_mChi1");
addWeights(${weight},"${path}","ScalarZp_mZP10_mChi1000");
addWeights(${weight},"${path}","ScalarZp_mZP10_mChi10");
addWeights(${weight},"${path}","ScalarZp_mZP10_mChi1");
addWeights(${weight},"${path}","ScalarZp_mZP10_mChi150");
addWeights(${weight},"${path}","ScalarZp_mZP10_mChi500");
addWeights(${weight},"${path}","ScalarZp_mZP10_mChi50");
addWeights(${weight},"${path}","ScalarZp_mZP15_mChi10");
addWeights(${weight},"${path}","ScalarZp_mZP200_mChi1");
addWeights(${weight},"${path}","ScalarZp_mZP200_mChi150");
addWeights(${weight},"${path}","ScalarZp_mZP200_mChi50");
addWeights(${weight},"${path}","ScalarZp_mZP20_mChi1");
addWeights(${weight},"${path}","ScalarZp_mZP295_mChi150");
addWeights(${weight},"${path}","ScalarZp_mZP300_mChi1");
addWeights(${weight},"${path}","ScalarZp_mZP300_mChi50");
addWeights(${weight},"${path}","ScalarZp_mZP500_mChi1");
addWeights(${weight},"${path}","ScalarZp_mZP500_mChi150");
addWeights(${weight},"${path}","ScalarZp_mZP500_mChi500");
addWeights(${weight},"${path}","ScalarZp_mZP50_mChi10");
addWeights(${weight},"${path}","ScalarZp_mZP50_mChi1");
addWeights(${weight},"${path}","ScalarZp_mZP50_mChi50");
addWeights(${weight},"${path}","ScalarZp_mZP95_mChi50");
addWeights(${weight},"${path}","ScalarZp_mZP995_mChi500");

addWeights(${weight},"${path}","BaryonicZp_mZP10000_mChi1000");
addWeights(${weight},"${path}","BaryonicZp_mZP10000_mChi10");
addWeights(${weight},"${path}","BaryonicZp_mZP10000_mChi150");
addWeights(${weight},"${path}","BaryonicZp_mZP10000_mChi1");
addWeights(${weight},"${path}","BaryonicZp_mZP10000_mChi500");
addWeights(${weight},"${path}","BaryonicZp_mZP10000_mChi50");
addWeights(${weight},"${path}","BaryonicZp_mZP1000_mChi1000");
addWeights(${weight},"${path}","BaryonicZp_mZP1000_mChi150");
addWeights(${weight},"${path}","BaryonicZp_mZP1000_mChi1");
addWeights(${weight},"${path}","BaryonicZp_mZP100_mChi10");
addWeights(${weight},"${path}","BaryonicZp_mZP100_mChi1");
addWeights(${weight},"${path}","BaryonicZp_mZP10_mChi1000");
addWeights(${weight},"${path}","BaryonicZp_mZP10_mChi10");
addWeights(${weight},"${path}","BaryonicZp_mZP10_mChi1");
addWeights(${weight},"${path}","BaryonicZp_mZP10_mChi50");
addWeights(${weight},"${path}","BaryonicZp_mZP15_mChi10");
addWeights(${weight},"${path}","BaryonicZp_mZP1995_mChi1000");
addWeights(${weight},"${path}","BaryonicZp_mZP2000_mChi1");
addWeights(${weight},"${path}","BaryonicZp_mZP2000_mChi500");
addWeights(${weight},"${path}","BaryonicZp_mZP200_mChi150");
addWeights(${weight},"${path}","BaryonicZp_mZP200_mChi1");
addWeights(${weight},"${path}","BaryonicZp_mZP200_mChi50");
addWeights(${weight},"${path}","BaryonicZp_mZP20_mChi1");
addWeights(${weight},"${path}","BaryonicZp_mZP295_mChi150");
addWeights(${weight},"${path}","BaryonicZp_mZP300_mChi50");
addWeights(${weight},"${path}","BaryonicZp_mZP500_mChi150");
addWeights(${weight},"${path}","BaryonicZp_mZP500_mChi1");
addWeights(${weight},"${path}","BaryonicZp_mZP500_mChi500");
addWeights(${weight},"${path}","BaryonicZp_mZP50_mChi10");
addWeights(${weight},"${path}","BaryonicZp_mZP50_mChi1");
addWeights(${weight},"${path}","BaryonicZp_mZP995_mChi500");

addWeights(${weight},"${path}","2HDM_mZP1000_mA0300");
addWeights(${weight},"${path}","2HDM_mZP1000_mA0400");
addWeights(${weight},"${path}","2HDM_mZP1000_mA0500");
addWeights(${weight},"${path}","2HDM_mZP1000_mA0600");
addWeights(${weight},"${path}","2HDM_mZP1000_mA0700");
addWeights(${weight},"${path}","2HDM_mZP1000_mA0800");
addWeights(${weight},"${path}","2HDM_mZP1200_mA0300");
addWeights(${weight},"${path}","2HDM_mZP1200_mA0400");
addWeights(${weight},"${path}","2HDM_mZP1200_mA0500");
addWeights(${weight},"${path}","2HDM_mZP1200_mA0600");
addWeights(${weight},"${path}","2HDM_mZP1200_mA0700");
addWeights(${weight},"${path}","2HDM_mZP1200_mA0800");
addWeights(${weight},"${path}","2HDM_mZP1400_mA0300");
addWeights(${weight},"${path}","2HDM_mZP1400_mA0400");
addWeights(${weight},"${path}","2HDM_mZP1400_mA0500");
addWeights(${weight},"${path}","2HDM_mZP1400_mA0600");
addWeights(${weight},"${path}","2HDM_mZP1400_mA0700");
addWeights(${weight},"${path}","2HDM_mZP1400_mA0800");
addWeights(${weight},"${path}","2HDM_mZP1700_mA0300");
addWeights(${weight},"${path}","2HDM_mZP1700_mA0400");
addWeights(${weight},"${path}","2HDM_mZP1700_mA0500");
addWeights(${weight},"${path}","2HDM_mZP1700_mA0600");
addWeights(${weight},"${path}","2HDM_mZP1700_mA0700");
addWeights(${weight},"${path}","2HDM_mZP1700_mA0800");
addWeights(${weight},"${path}","2HDM_mZP2000_mA0300");
addWeights(${weight},"${path}","2HDM_mZP2000_mA0400");
addWeights(${weight},"${path}","2HDM_mZP2000_mA0500");
addWeights(${weight},"${path}","2HDM_mZP2000_mA0600");
addWeights(${weight},"${path}","2HDM_mZP2000_mA0700");
addWeights(${weight},"${path}","2HDM_mZP2000_mA0800");
addWeights(${weight},"${path}","2HDM_mZP2500_mA0300");
addWeights(${weight},"${path}","2HDM_mZP2500_mA0400");
addWeights(${weight},"${path}","2HDM_mZP2500_mA0500");
addWeights(${weight},"${path}","2HDM_mZP2500_mA0600");
addWeights(${weight},"${path}","2HDM_mZP2500_mA0700");
addWeights(${weight},"${path}","2HDM_mZP2500_mA0800");
addWeights(${weight},"${path}","2HDM_mZP600_mA0300");
addWeights(${weight},"${path}","2HDM_mZP600_mA0400");
addWeights(${weight},"${path}","2HDM_mZP800_mA0300");
addWeights(${weight},"${path}","2HDM_mZP800_mA0400");
addWeights(${weight},"${path}","2HDM_mZP800_mA0500");
addWeights(${weight},"${path}","2HDM_mZP800_mA0600");

.q

EOF

echo "Finished adding weights!"
