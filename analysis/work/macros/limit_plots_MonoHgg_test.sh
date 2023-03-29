#!/bin/bash
./limit_plots.py --do-limits -M Asymptotic --input-dir ntuples4fit_mDM1 -O ~/www/Plots/13TeV_v80X_moriond17/TestLimits/ --label MonoHgg_ZpBaryonic --lumi 35.9 --use-fb --mDM 1 --suffix "" --unblind --saveas pdf,png,root
#./limit_plots.py --do-limits -M Asymptotic --input-dir ntuples4fit_orig --addName _orig -O ~/www/Plots/13TeV_v80X_moriond17/TestLimits/ --label MonoHgg_ZpBaryonic --lumi 35.9 --use-fb --mDM 1 --suffix "" --unblind --saveas pdf,png,root
#./limit_plots_MonoHgg_ZpBaryonic.py --do-limits -M Asymptotic --input-dir ntuples4fit_mDM1 -O ~/www/Plots/13TeV_v80X_moriond17/TestLimits/ --label MonoHgg_ZpBaryonic --lumi 35.9 --use-fb --mDM 1 --suffix ""
