#!/bin/sh

cp -r /afs/cern.ch/work/d/dbhowmik/public/Analysis/MHgg/2017Analysis/CMSSW_10_6_8/src/* .
./flashgg/setup_flashgg.sh 
cmsenv
mv MonoHiggsToGG/analysis/test/MC_bkg/GluGluHToGG/DiPhoAnalyzer_Legacy18.cc MonoHiggsToGG/analysis/test/MC_bkg/GluGluHToGG/DiPhoAnalyzer_Legacy18_cc
scram b -j 10
