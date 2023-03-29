#! /bin/sh 
# this scripts creates a merged root file in the self-created mergedFinal

#mkdir -p data/mergedFinal

hadd data/25ns_v80X_v2/DoubleEG.root	data/25ns_v80X_v2/Run2016*
hadd data/25ns_v80X_v2/GJets.root	data/25ns_v80X_v2/GJet_Pt-20to40.root	data/25ns_v80X_v2/GJet_Pt-40toInf.root 
hadd data/25ns_v80X_v2/QCD.root		data/25ns_v80X_v2/QCD_Pt-40toInf.root	data/25ns_v80X_v2/QCD_Pt-30toInf.root	#data/25ns_v80X_v2/QCD_Pt-30to40.root
#hadd data/25ns_v80X_v2/ZJets.root	data/25ns_v80X_v2/ZJets_HT-100To200.root data/25ns_v80X_v2/ZJets_HT-200To400.root   data/25ns_v80X_v2/ZJets_HT-400To600.root   data/25ns_v80X_v2/ZJets_HT-600To800.root   data/25ns_v80X_v2/ZJets_HT-800To1200.root   data/25ns_v80X_v2/ZJets_HT-1200To2500.root   data/25ns_v80X_v2/ZJets_HT-2500ToInf.root  
