#!/bin/sh

mva="BDT"
opt=3
suffix="_AllBkgs"
path="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_MVA/"
addname="OutputMVA_${mva}_opt${opt}${suffix}_"
endname="_skim_woVetos"

python fitFormatting.py -i ${path}${addname} --ext ${endname} -o ntuples4fit_${mva}_opt${opt}${suffix}/ -f names.txt  
