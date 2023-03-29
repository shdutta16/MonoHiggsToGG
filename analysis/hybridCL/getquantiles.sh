#! /bin/sh

mA0=300
path="/afs/cern.ch/work/m/mzientek/public/HybridNewCL_REDO/2HDM_A0300_SinglePoint_m600Sel_UpdatedSys/"

for mass in 600 800 1000 1200 1400 1700 2000 2500
  do
    for quantile in 0.5 0.025 0.16 0.84 0.975 
      do
        echo ${mass} 
	echo ${quantile} 
        combine DataCard_2HDM_mZP${mass}_mA0${mA0}.txt -m ${mass} -M HybridNew --freq --grid=${path}higgsCombineTest.HybridNew.mH${mass}.mA0${mA0}.root --expectedFromGrid=${quantile} -v 3 
      done
    echo ${mass} 
    echo "Observed limit" 
    combine DataCard_2HDM_mZP${mass}_mA0${mA0}.txt -m ${mass} -M HybridNew --freq --grid=${path}higgsCombineTest.HybridNew.mH${mass}.mA0${mA0}.root
  done
