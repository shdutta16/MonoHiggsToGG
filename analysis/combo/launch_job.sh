#!/bin/bash

model=$1
mass=$2

echo "Mass ${mass}"
python createjob.py $model $mass 

chmod 755 submitjob_${model}_${mass}.tmpl

echo "bsub -q 1nd submitjob_${model}_${mass}.tmpl"
bsub -q 1nd submitjob_${model}_${mass}.tmpl
