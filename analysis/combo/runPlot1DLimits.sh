#!/bin/bash

outdir='~/www/Plots/13TeV_v80X_moriond17/PaperPlots/'

python plot1DlimitsAll.py ${outdir} BARY 1  
python plot1DlimitsAll.py ${outdir} 2HDM 300  
