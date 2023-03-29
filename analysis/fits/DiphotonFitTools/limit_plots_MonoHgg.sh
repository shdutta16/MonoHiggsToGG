#!/bin/bash

target=$1

for mDM in 1; do
  ./limit_plots_MonoHgg.py --do-limits -M Asymptotic --input-dir $target --model BARY -O ~/www/Plots/13TeV_v80X_moriond17/FitResults/ --label MonoHgg_ZpBaryonic --lumi 35.9 --use-fb --mDM $mDM --suffix "" --unblind --saveas pdf,png,root
done

#for mDM in 300; do
#  ./limit_plots_MonoHgg.py --do-limits -M Asymptotic --input-dir $target --model 2HDM -O ~/www/Plots/13TeV_v80X_moriond17/FitResults/ --label MonoHgg_2HDM --lumi 35.9 --use-fb --mDM $mDM --suffix "" --unblind --saveas pdf,png,root
#done
