#! /bin/sh

#############################################
#
# fitterFormatting input arguements:
#   1st: input directory
#   2nd: output directory
#   3rd: type (sig, bkg, data)
#   4th: prompt (for duplicate removal)
#   5th: input filename 
#   6th: sample name
#   7th: outfile name
#   8th: category type 
#
# final files used for the fit are:
#   outdir/Output_MC.root
#   outdir/Output_Data.root
#
#############################################
# Sidenote: Safely ignore warning message "tab completion not implemented for this context" 
# This comes from the tabs included for making the inputs to fitterFormatting easier to read below 

#############################################
# 
#   Category Types
#   0 = No additional Sel. (i.e. Original Selection from Analayzer) w/ MET > 70
#   1 = OptSel1 : pt1/m > 0.50, pt2/m > 0.25, ptgg > 90, MET > 105
#   2 = OptSel2 : pt1/m > 0.55, pt2/m > 0.25, ptgg/MET > 0.5, MET > 95
#   3 = OptSel3 : pt1/m > 0.55, pt2/m > 0.25, ptgg > 85, MET > 50 
#   4 = OptSel4 : pt1/m > 0.45, pt2/m > 0.25, ptgg/MET > 0.2, MET > 70
#
#############################################

categType="1";
indir="../macro/data/25ns_v80X_v1/"
outdir="ntuples4fit_vtx0_OptSel$categType"

mkdir -p $outdir

root -l -b << EOF
.L fitterFormatting.cc++

fitterFormatting("$indir","$outdir","data",0,"DoubleEG.root",	"DoubleEG",	"Output_Data.root",$categType)


fitterFormatting("$indir","$outdir","sig",0,"2HDM_mZP600_mA0300.root",  "sig_2HDM_mZP600_mA0300",	"2HDM_mZP600_mA0300_new.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"2HDM_mZP800_mA0300.root",  "sig_2HDM_mZP800_mA0300",	"2HDM_mZP800_mA0300_new.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"2HDM_mZP1000_mA0300.root", "sig_2HDM_mZP1000_mA0300",	"2HDM_mZP1000_mA0300_new.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"2HDM_mZP1200_mA0300.root", "sig_2HDM_mZP1200_mA0300",	"2HDM_mZP1200_mA0300_new.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"2HDM_mZP1400_mA0300.root", "sig_2HDM_mZP1400_mA0300",	"2HDM_mZP1400_mA0300_new.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"2HDM_mZP1700_mA0300.root", "sig_2HDM_mZP1700_mA0300",	"2HDM_mZP1700_mA0300_new.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"2HDM_mZP2000_mA0300.root", "sig_2HDM_mZP2000_mA0300",	"2HDM_mZP2000_mA0300_new.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"2HDM_mZP2500_mA0300.root", "sig_2HDM_mZP2500_mA0300",	"2HDM_mZP2500_mA0300_new.root",$categType)

fitterFormatting("$indir","$outdir","sig",0,"BaryonicZp_mZP1000_mChi1000.root", "sig_ZpBaryonic_mZP1000_mChi1000",	"ZpBaryonic_mZP1000_mChi1000.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"BaryonicZp_mZP1000_mChi150.root",  "sig_ZpBaryonic_mZP1000_mChi150",	"ZpBaryonic_mZP1000_mChi150.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"BaryonicZp_mZP1000_mChi1.root",    "sig_ZpBaryonic_mZP1000_mChi1",		"ZpBaryonic_mZP1000_mChi1.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"BaryonicZp_mZP500_mChi500.root",   "sig_ZpBaryonic_mZP500_mChi500",	"ZpBaryonic_mZP500_mChi500.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"BaryonicZp_mZP500_mChi150.root",   "sig_ZpBaryonic_mZP500_mChi150",	"ZpBaryonic_mZP500_mChi150.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"BaryonicZp_mZP500_mChi1.root",     "sig_ZpBaryonic_mZP500_mChi1",		"ZpBaryonic_mZP500_mChi1.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"BaryonicZp_mZP100_mChi10.root",    "sig_ZpBaryonic_mZP100_mChi10",		"ZpBaryonic_mZP100_mChi10.root",$categType)
fitterFormatting("$indir","$outdir","sig",0,"BaryonicZp_mZP10_mChi10.root",     "sig_ZpBaryonic_mZP10_mChi10",		"ZpBaryonic_mZP10_mChi10.root",$categType)

fitterFormatting("$indir","$outdir","bkg",0,"VHToGG.root",	"VHToGG",	"VHToGG_new.root",$categType)
fitterFormatting("$indir","$outdir","bkg",0,"VBFHToGG.root",	"VBFHToGG",	"VBFHToGG_new.root",$categType)
fitterFormatting("$indir","$outdir","bkg",0,"ttHJetToGG.root",	"ttHJetToGG",	"ttHJetToGG_new.root",$categType)
fitterFormatting("$indir","$outdir","bkg",0,"GluGluHToGG.root",	"GluGluHToGG",	"GluGluHToGG_new.root",$categType)
fitterFormatting("$indir","$outdir","bkg",0,"DiPhoton.root",	"DiPhoton",	"DiPhoton_new.root",$categType)

.q

EOF
echo "Done"

echo "Adding MC Files Together"

hadd $outdir/Output_MC.root $outdir/ZpBaryonic* $outdir/2HDM_mZP* $outdir/GluGluHToGG_new.root $outdir/DiPhoton_new.root $outdir/V* $outdir/ttH* 

#hadd $outdir/Output_MC.root $outdir/2HDM_mZP* $outdir/GluGluHToGG_new.root $outdir/VH_new.root $outdir/QCD_new.root $outdir/GJets_new.root $outdir/DiPhoton_new.root $outdir/DYJetsToLL_new.root $outdir/VBFHToGG_new.root $outdir/ttHJetToGG_new.root $outdir/TGJets_new.root $outdir/TTGJets_new.root $outdir/WGToLNuG_new.root $outdir/ZGTo2LG_new.root $outdir/TTGG_0Jets_new.root $outdir/ZZTo2L2Nu_new.root 

#fitterFormatting("$indir","$outdir","sig",0,"2HDM_mZP600_mA0400.root",	"sig_2HDM_mZP600_mA0400",	"2HDM_mZP600_mA0400_new.root",$categType)
#fitterFormatting("$indir","$outdir","sig",0,"2HDM_mZP800_mA0400.root",	"sig_2HDM_mZP800_mA0400",	"2HDM_mZP800_mA0400_new.root",$categType)
#fitterFormatting("$indir","$outdir","bkg",0,"VH.root",		"VH",		"VH_new.root",$categType)
#fitterFormatting("$indir","$outdir","bkg",0,"TTGJets.root",	"TTGJets",	"TTGJets_new.root",$categType)
#fitterFormatting("$indir","$outdir","bkg",0,"TTGG_0Jets.root",	"TTGG_0Jets",	"TTGG_0Jets_new.root",$categType)
#fitterFormatting("$indir","$outdir","bkg",0,"TGJets.root",	"TGJets",	"TGJets_new.root",$categType)
#fitterFormatting("$indir","$outdir","bkg",0,"WGToLNuG.root",	"WGToLNuG",	"WGToLNuG_new.root",$categType)
#fitterFormatting("$indir","$outdir","bkg",0,"ZGTo2LG.root",	"ZGTo2LG",	"ZGTo2LG_new.root",$categType)
#fitterFormatting("$indir","$outdir","bkg",0,"ZZTo2L2Nu.root",	"ZZTo2L2Nu",	"ZZTo2L2Nu_new.root",$categType)
#fitterFormatting("$indir","$outdir","bkg",0,"ttHJetToGG.root",	"ttHJetToGG",	"ttHJetToGG_new.root",$categType)
#fitterFormatting("$indir","$outdir","bkg",0,"VBFHToGG.root",	"VBFHToGG",	"VBFHToGG_new.root",$categType)
#fitterFormatting("$indir","$outdir","bkg",2,"QCD.root",		"QCD",		"QCD_new.root",$categType)
#fitterFormatting("$indir","$outdir","bkg",1,"GJets.root",	"GJets",	"GJets_new.root",$categType)
#fitterFormatting("$indir","$outdir","bkg",0,"DYJetsToLL.root",	"DYJetsToLL",	"DYJetsToLL_new.root",$categType)



