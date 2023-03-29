#!/bin/bash

pathdir="/afs/cern.ch/user/m/mzientek/private/MonoHggCMSSW/CMSSW_7_6_3_patch2/src/MonoHiggsToGG/analysis/macro/diPhoPlots/"
indir1="25ns_v76X_v2_OptSel"

sel="1"
outdir="76X_OptSel"${sel}

mkdir -p ${outdir} 

cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP600_Sel${sel}_1_met105.txt  $outdir/DataCard_2HDM_mZP600.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP800_Sel${sel}_2_met165.txt  $outdir/DataCard_2HDM_mZP800.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP1000_Sel${sel}_3_met220.txt $outdir/DataCard_2HDM_mZP1000.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP1200_Sel${sel}_4_met245.txt $outdir/DataCard_2HDM_mZP1200.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP1400_Sel${sel}_5_met255.txt $outdir/DataCard_2HDM_mZP1400.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP1700_Sel${sel}_6_met285.txt $outdir/DataCard_2HDM_mZP1700.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP2000_Sel${sel}_7_met285.txt $outdir/DataCard_2HDM_mZP2000.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP2500_Sel${sel}_7_met285.txt $outdir/DataCard_2HDM_mZP2500.txt

sel="2"
outdir="76X_OptSel"${sel}

mkdir -p ${outdir} 

cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP600_Sel${sel}_1_met95.txt   $outdir/DataCard_2HDM_mZP600.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP800_Sel${sel}_2_met170.txt  $outdir/DataCard_2HDM_mZP800.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP1000_Sel${sel}_3_met170.txt $outdir/DataCard_2HDM_mZP1000.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP1200_Sel${sel}_4_met230.txt $outdir/DataCard_2HDM_mZP1200.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP1400_Sel${sel}_5_met255.txt $outdir/DataCard_2HDM_mZP1400.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP1700_Sel${sel}_6_met285.txt $outdir/DataCard_2HDM_mZP1700.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP2000_Sel${sel}_7_met285.txt $outdir/DataCard_2HDM_mZP2000.txt
cp $pathdir/$indir1$sel/ABCD/DataCard_2HDM_mZP2500_Sel${sel}_7_met285.txt $outdir/DataCard_2HDM_mZP2500.txt


#for mass in 600 800 1000 1200 1400 1700 2000 2500 #1 10 100 1000
#  do
#    echo $mass 
#    combine Opt${sel}/DataCard_2HDM_mZP${mass}.txt -M Asymptotic -m ${mass} --run=blind    
#  done
