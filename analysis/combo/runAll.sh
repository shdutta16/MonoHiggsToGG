#!/bin/bash

model=$1 # 2HDM or BARY

if [[ $model == "2HDM" ]]; then
  vals_mA=($(seq 400 25 700))
fi
if [[ $model == "BARY" ]]; then
  vals_mA=($(seq 0 25 900))
fi

for mA in ${vals_mA[@]}; do
  ./launch_job.sh $model $mA
done 
