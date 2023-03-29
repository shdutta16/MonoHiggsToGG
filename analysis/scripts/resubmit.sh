#!/bin/bash

# ---------------------------------------------------------
# Run by caling:
# ./resubmit.sh job_XXXXXXX_XX SampleName
# check size of output files: find DIRECTORY -type f -size -10k
# ---------------------------------------------------------

DIR=$1
NAME=$2

mkdir -p $DIR/$NAME/logOld
mv $DIR/$NAME/log/* $DIR/$NAME/logOld/

FILES=$DIR/$NAME/logOld/*
COUNTER=0
log="logOld"
logg="log"
src="src"
for i in $FILES
do
    #if grep -ir FallbackFileOpenError >/dev/null $i
    if grep -ir "exit code" >/dev/null $i
    #if grep -ir "Begin Fatal" >/dev/null $i
	then
	logfile=$i
	srcfile=$(echo $i | sed "s/$log/$src/g")
	srcfile=$(echo $srcfile | sed "s/"$NAME"_/submit_/g")
	srcfile=$(echo $srcfile | sed "s/$logg/$src/g")
	logfile=$(echo $logfile | sed "s/logOld/log/g")
	echo bsub -q 1nh -o $logfile source /afs/cern.ch/user/m/mzientek/private/MonoHgg_Moriond17/CMSSW_8_0_26_patch1/src/MonoHiggsToGG/analysis/scripts/$srcfile 
	bsub -q 1nh -o $logfile source /afs/cern.ch/user/m/mzientek/private/MonoHgg_Moriond17/CMSSW_8_0_26_patch1/src/MonoHiggsToGG/analysis/scripts/$srcfile 
	let COUNTER=COUNTER+1 
    fi
#, for src in list: system(bsub %s -q 2nd, src.replace(log,src)”
done

echo $COUNTER
