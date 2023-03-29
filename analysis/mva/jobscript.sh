#!/bin/sh

ANA_DIR=/afs/cern.ch/work/m/mzientek/private/MonoHgg_2017/CMSSW_9_2_8/src/
RUN_DIR=/afs/cern.ch/work/m/mzientek/private/MonoHgg_2017/CMSSW_9_2_8/src/MonoHiggsToGG/analysis/mva/

cd $ANA_DIR
eval `scram runtime -sh`
cd $RUN_DIR

echo "run job!"
python runMVAtest.py --mva BDT
echo "done running"

echo "removing potential core dump file!"
rm core.*

echo "all done successfully!"
