#! /bin/sh

maindir="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/"

hadd ${maindir}DoubleEG.root ${maindir}Run2016*.root
hadd ${maindir}DiPhoton.root ${maindir}DiPhoton_M80.root ${maindir}DiPhoton_M40.root
hadd ${maindir}QCD.root      ${maindir}QCD_Pt*
hadd ${maindir}GJets.root    ${maindir}GJet_Pt*
