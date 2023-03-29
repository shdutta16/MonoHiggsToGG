#! /bin/sh

mA0=300

for mass in 600 800 1000 1200 1400 1700 2000 2500
  do 
    hadd higgsCombineTest.HybridNew.mH${mass}.mA0${mA0}.root MZP${mass}/higgsCombineTest.HybridNew.mH${mass}*
  done
