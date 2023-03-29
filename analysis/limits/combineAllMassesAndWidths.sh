#!/bin/bash

for mass in 600 800 1000 1200 1400 1700 2500 #1 10 100 1000
  do
    echo $mass 
    combine DataCard_2HDM_mZP${mass}_met80.txt -M Asymptotic -m ${mass} --run=blind    
  done
