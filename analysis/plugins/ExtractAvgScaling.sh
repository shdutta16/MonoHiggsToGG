#! /bin/sh

# this runs the python scripts to get the average values of scaling over all runs 
# averages are done separately for bins of eta and r9

outdir="Tables_Scaling"
output1="Output"
output2="Parsed"
output3="ScalingValues"

# separate scaling in high and low R9
python Separate_HiLoR9_Scaling.py 	-i Scaling2016.dat 		-o $output1		-d $outdir

# separate scaling in all the eta bins now
python Separate_Eta_Scaling.py	-i $outdir/$output1"_highR9.txt"	-o $output2"_highR9" 	-d $outdir
python Separate_Eta_Scaling.py	-i $outdir/$output1"_lowR9.txt"		-o $output2"_lowR9" 	-d $outdir

# extract only the values of the scaling
awk '{print $5}' $outdir/$output2"_highR9_absEta_0_1.txt"  >> $outdir/$output3"_highR9_absEta_0_1.txt"
awk '{print $5}' $outdir/$output2"_lowR9_absEta_0_1.txt"   >> $outdir/$output3"_lowR9_absEta_0_1.txt"
awk '{print $5}' $outdir/$output2"_highR9_absEta_1_14.txt" >> $outdir/$output3"_highR9_absEta_1_14.txt"
awk '{print $5}' $outdir/$output2"_lowR9_absEta_1_14.txt"  >> $outdir/$output3"_lowR9_absEta_1_14.txt"
awk '{print $5}' $outdir/$output2"_highR9_absEta_15_2.txt" >> $outdir/$output3"_highR9_absEta_15_2.txt"
awk '{print $5}' $outdir/$output2"_lowR9_absEta_15_2.txt"  >> $outdir/$output3"_lowR9_absEta_15_2.txt"
awk '{print $5}' $outdir/$output2"_highR9_absEta_2_25.txt" >> $outdir/$output3"_highR9_absEta_2_25.txt"
awk '{print $5}' $outdir/$output2"_lowR9_absEta_2_25.txt"  >> $outdir/$output3"_lowR9_absEta_2_25.txt"

# average the results 
echo "Total Scaling for highR9 absEta_0_1 is " 
awk '{s+=$1} END {print s}' $outdir/$output3"_highR9_absEta_0_1.txt" 
echo "Total Scaling for lowR9 absEta_0_1 is " 
awk '{s+=$1} END {print s}' $outdir/$output3"_lowR9_absEta_0_1.txt"
echo "These are out of "
wc -l < $outdir/$output3"_highR9_absEta_0_1.txt"
wc -l < $outdir/$output3"_lowR9_absEta_0_1.txt"

echo "Total Scaling for highR9 absEta_1_14 is " 
awk '{s+=$1} END {print s}' $outdir/$output3"_highR9_absEta_1_14.txt"
echo "Total Scaling for lowR9 absEta_1_14 is " 
awk '{s+=$1} END {print s}' $outdir/$output3"_lowR9_absEta_1_14.txt"
echo "These are out of "
wc -l < $outdir/$output3"_highR9_absEta_1_14.txt"
wc -l < $outdir/$output3"_lowR9_absEta_1_14.txt"


echo "Total Scaling for highR9 absEta_15_2 is " 
awk '{s+=$1} END {print s}' $outdir/$output3"_highR9_absEta_15_2.txt"
echo "Total Scaling for lowR9 absEta_15_2 is " 
awk '{s+=$1} END {print s}' $outdir/$output3"_lowR9_absEta_15_2.txt"
echo "These are out of "
wc -l < $outdir/$output3"_highR9_absEta_15_2.txt"
wc -l < $outdir/$output3"_lowR9_absEta_15_2.txt"


echo "Total Scaling for highR9 absEta_2_25 is " 
awk '{s+=$1} END {print s}' $outdir/$output3"_highR9_absEta_2_25.txt"
echo "Total Scaling for lowR9 absEta_2_25 is " 
awk '{s+=$1} END {print s}' $outdir/$output3"_lowR9_absEta_2_25.txt"
echo "These are out of "
wc -l < $outdir/$output3"_highR9_absEta_2_25.txt"
wc -l < $outdir/$output3"_lowR9_absEta_2_25.txt"


#echo "These are out of "
#wc -l < $outdir/$output3"_highR9_absEta_0_1.txt"

rm $outdir/$output3"_highR9_absEta_0_1.txt"
rm $outdir/$output3"_lowR9_absEta_0_1.txt"
rm $outdir/$output3"_highR9_absEta_1_14.txt"
rm $outdir/$output3"_lowR9_absEta_1_14.txt"
rm $outdir/$output3"_highR9_absEta_15_2.txt"
rm $outdir/$output3"_lowR9_absEta_15_2.txt"
rm $outdir/$output3"_highR9_absEta_2_25.txt"
rm $outdir/$output3"_lowR9_absEta_2_25.txt"


