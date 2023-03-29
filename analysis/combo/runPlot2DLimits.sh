#!/bin/bash

outdir=$1
optdowgt=false
optdo90=false

addopt=''
if ${optdowgt}; then
  addopt=${addopt}' --dowgt'
fi
if ${optdo90}; then
  addopt=${addopt}' --do90'
fi

python plot2DlimitsAll.py -O ${outdir} ${addopt} -m BARY -w gg     
python plot2DlimitsAll.py -O ${outdir} ${addopt} -m BARY -w tt    
python plot2DlimitsAll.py -O ${outdir} ${addopt} -m BARY -w combo 
python plot2DlimitsAll.py -O ${outdir} ${addopt} -m 2HDM -w gg    
python plot2DlimitsAll.py -O ${outdir} ${addopt} -m 2HDM -w tt    --dosmooth
python plot2DlimitsAll.py -O ${outdir} ${addopt} -m 2HDM -w combo --dosmooth
