#!/bin/bash

target=$1 && shift
nm_suffix=$1 && shift
outDir=$PWD

set -x
for mDM in 150; do
    ./mylimit_plots_MonoHgg_2HDMa.py --do-limits -M AsymptoticLimits --input-dir $target --model 2HDMa -O $outDir/ --label MonoHgg_2HDMa --lumi 59.69 --use-fb --mDM $mDM --suffix "" --blind --addName $nm_suffix --saveas root #--saveas pdf,png,root
done
set +x
