#! /bin/sh

mA0=300
for mass in 600 800 1000 1200 1400 1700 2000 2500 
  do
    for quantile in 0.025 0.16 0.84 0.975 0.5
      do 
        echo $mass
	echo $quantile
        python combine.py -i DataCard_2HDM_mZP${mass}_mA0${mA0}.txt -d /afs/cern.ch/work/m/mzientek/public/HybridNewCL/2HDM_A0${mA0}/MZP${mass}_${quantile} -q 1nh -M HybridNew --onews --begin ${mass} --end ${mass} -t 100 --doquantile --quantile ${quantile}
      done	
  done

