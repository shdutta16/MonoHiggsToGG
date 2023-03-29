#! /bin/sh

#############################################
# fitterFormatting_METcat input arguements:
#   1st: whichDphi cuts to apply
#   2nd: which met cut to apply
#   3rd: input directory
#   4th: output directory
#   5th: type (sig, bkg, data)
#   6th: prompt (for duplicate removal)
#   7th: input filename 
#   8th: sample name
#############################################
#############################################
# final files used for the fit are:
#   outdir/Output_MC.root
#   outdir/Output_Data.root
#############################################

which_dphi_cuts=3 #0 = none, 1 = ggMET, 2 = jMET, 3 = both
addname=""
#if [ "$which_dphi_cuts" -eq 0 ] then 
#  $addname="_dphiless"
#elif [ "$which_dphi_cuts" -eq 1 ] then
#  $addname="_wDphiGGmet"
#elif [ "$which_dphi_cuts" -eq 2 ] then
#  $addname="_wDphiJmet"
#fi

echo $addname

indir="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v2/"
metcut=$1
outdir="ntuples4fit_pho_newSig_test${addname}_met0_met"${metcut}

mkdir -p $outdir

# Sidenote: Safely ignore warning message "tab completion not implemented for this context" 
# This comes from the tabs included for making the inputs to fitterFormatting easier to read below 

root -l -b << EOF
.L fitterFormatting_METcat.cc++

fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP600_mA0300_skimmedtree.root",	"sig_2HDM_mZP600_mA0300")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP800_mA0300_skimmedtree.root",	"sig_2HDM_mZP800_mA0300")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1000_mA0300_skimmedtree.root",	"sig_2HDM_mZP1000_mA0300")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1200_mA0300_skimmedtree.root",	"sig_2HDM_mZP1200_mA0300")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1400_mA0300_skimmedtree.root",	"sig_2HDM_mZP1400_mA0300")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1700_mA0300_skimmedtree.root",	"sig_2HDM_mZP1700_mA0300")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP2000_mA0300_skimmedtree.root",	"sig_2HDM_mZP2000_mA0300")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP2500_mA0300_skimmedtree.root",	"sig_2HDM_mZP2500_mA0300")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP600_mA0400_skimmedtree.root",	"sig_2HDM_mZP600_mA0400")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP800_mA0400_skimmedtree.root",	"sig_2HDM_mZP800_mA0400")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1000_mA0400_skimmedtree.root",	"sig_2HDM_mZP1000_mA0400")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1200_mA0400_skimmedtree.root",	"sig_2HDM_mZP1200_mA0400")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1400_mA0400_skimmedtree.root",	"sig_2HDM_mZP1400_mA0400")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1700_mA0400_skimmedtree.root",	"sig_2HDM_mZP1700_mA0400")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP2000_mA0400_skimmedtree.root",	"sig_2HDM_mZP2000_mA0400")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP2500_mA0400_skimmedtree.root",	"sig_2HDM_mZP2500_mA0400")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP800_mA0500_skimmedtree.root",	"sig_2HDM_mZP800_mA0500")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1000_mA0500_skimmedtree.root",	"sig_2HDM_mZP1000_mA0500")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1200_mA0500_skimmedtree.root",	"sig_2HDM_mZP1200_mA0500")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1400_mA0500_skimmedtree.root",	"sig_2HDM_mZP1400_mA0500")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1700_mA0500_skimmedtree.root",	"sig_2HDM_mZP1700_mA0500")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP2000_mA0500_skimmedtree.root",	"sig_2HDM_mZP2000_mA0500")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP2500_mA0500_skimmedtree.root",	"sig_2HDM_mZP2500_mA0500")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP800_mA0600_skimmedtree.root",	"sig_2HDM_mZP800_mA0600")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1000_mA0600_skimmedtree.root",	"sig_2HDM_mZP1000_mA0600")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1200_mA0600_skimmedtree.root",	"sig_2HDM_mZP1200_mA0600")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1400_mA0600_skimmedtree.root",	"sig_2HDM_mZP1400_mA0600")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1700_mA0600_skimmedtree.root",	"sig_2HDM_mZP1700_mA0600")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP2000_mA0600_skimmedtree.root",	"sig_2HDM_mZP2000_mA0600")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP2500_mA0600_skimmedtree.root",	"sig_2HDM_mZP2500_mA0600")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1000_mA0700_skimmedtree.root",	"sig_2HDM_mZP1000_mA0700")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1200_mA0700_skimmedtree.root",	"sig_2HDM_mZP1200_mA0700")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1400_mA0700_skimmedtree.root",	"sig_2HDM_mZP1400_mA0700")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1700_mA0700_skimmedtree.root",	"sig_2HDM_mZP1700_mA0700")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP2000_mA0700_skimmedtree.root",	"sig_2HDM_mZP2000_mA0700")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP2500_mA0700_skimmedtree.root",	"sig_2HDM_mZP2500_mA0700")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1000_mA0800_skimmedtree.root",	"sig_2HDM_mZP1000_mA0800")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1200_mA0800_skimmedtree.root",	"sig_2HDM_mZP1200_mA0800")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1400_mA0800_skimmedtree.root",	"sig_2HDM_mZP1400_mA0800")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP1700_mA0800_skimmedtree.root",	"sig_2HDM_mZP1700_mA0800")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP2000_mA0800_skimmedtree.root",	"sig_2HDM_mZP2000_mA0800")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "2HDM_mZP2500_mA0800_skimmedtree.root",	"sig_2HDM_mZP2500_mA0800")

fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP10000_mChi1000_skimmedtree.root", "sig_ZpBaryonic_mZP10000_mChi1000")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP10000_mChi10_skimmedtree.root"  , "sig_ZpBaryonic_mZP10000_mChi10")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP10000_mChi150_skimmedtree.root" , "sig_ZpBaryonic_mZP10000_mChi150")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP10000_mChi1_skimmedtree.root"   , "sig_ZpBaryonic_mZP10000_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP10000_mChi500_skimmedtree.root" , "sig_ZpBaryonic_mZP10000_mChi500")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP10000_mChi50_skimmedtree.root"  , "sig_ZpBaryonic_mZP10000_mChi50")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP1000_mChi1000_skimmedtree.root" , "sig_ZpBaryonic_mZP1000_mChi1000")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP1000_mChi150_skimmedtree.root"  , "sig_ZpBaryonic_mZP1000_mChi150")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP1000_mChi1_skimmedtree.root"    , "sig_ZpBaryonic_mZP1000_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP100_mChi10_skimmedtree.root"    , "sig_ZpBaryonic_mZP100_mChi10")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP100_mChi1_skimmedtree.root"     , "sig_ZpBaryonic_mZP100_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP10_mChi1000_skimmedtree.root"   , "sig_ZpBaryonic_mZP10_mChi1000")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP10_mChi10_skimmedtree.root"     , "sig_ZpBaryonic_mZP10_mChi10")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP10_mChi1_skimmedtree.root"      , "sig_ZpBaryonic_mZP10_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP10_mChi50_skimmedtree.root"     , "sig_ZpBaryonic_mZP10_mChi50")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP15_mChi10_skimmedtree.root"     , "sig_ZpBaryonic_mZP15_mChi10")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP1995_mChi1000_skimmedtree.root" , "sig_ZpBaryonic_mZP1995_mChi1000")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP2000_mChi1_skimmedtree.root"    , "sig_ZpBaryonic_mZP2000_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP2000_mChi500_skimmedtree.root"  , "sig_ZpBaryonic_mZP2000_mChi500")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP200_mChi150_skimmedtree.root"   , "sig_ZpBaryonic_mZP200_mChi150")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP200_mChi1_skimmedtree.root"     , "sig_ZpBaryonic_mZP200_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP200_mChi50_skimmedtree.root"    , "sig_ZpBaryonic_mZP200_mChi50")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP20_mChi1_skimmedtree.root"      , "sig_ZpBaryonic_mZP20_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP295_mChi150_skimmedtree.root"   , "sig_ZpBaryonic_mZP295_mChi150")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP300_mChi50_skimmedtree.root"    , "sig_ZpBaryonic_mZP300_mChi50")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP500_mChi150_skimmedtree.root"   , "sig_ZpBaryonic_mZP500_mChi150")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP500_mChi1_skimmedtree.root"     , "sig_ZpBaryonic_mZP500_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP500_mChi500_skimmedtree.root"   , "sig_ZpBaryonic_mZP500_mChi500")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP50_mChi10_skimmedtree.root"     , "sig_ZpBaryonic_mZP50_mChi10")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP50_mChi1_skimmedtree.root"      , "sig_ZpBaryonic_mZP50_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "BaryonicZp_mZP995_mChi500_skimmedtree.root"   , "sig_ZpBaryonic_mZP995_mChi500")

fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP10000_mChi1000_skimmedtree.root"  , "sig_ScalarZp_mZP10000_mChi1000")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP10000_mChi10_skimmedtree.root"    , "sig_ScalarZp_mZP10000_mChi10")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP10000_mChi150_skimmedtree.root"   , "sig_ScalarZp_mZP10000_mChi150")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP10000_mChi1_skimmedtree.root"     , "sig_ScalarZp_mZP10000_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP10000_mChi500_skimmedtree.root"   , "sig_ScalarZp_mZP10000_mChi500")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP10000_mChi50_skimmedtree.root"    , "sig_ScalarZp_mZP10000_mChi50")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP1000_mChi1000_skimmedtree.root"   , "sig_ScalarZp_mZP1000_mChi1000")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP1000_mChi150_skimmedtree.root"    , "sig_ScalarZp_mZP1000_mChi150")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP1000_mChi1_skimmedtree.root"      , "sig_ScalarZp_mZP1000_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP100_mChi10_skimmedtree.root"      , "sig_ScalarZp_mZP100_mChi10")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP100_mChi1_skimmedtree.root"       , "sig_ScalarZp_mZP100_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP10_mChi1000_skimmedtree.root"     , "sig_ScalarZp_mZP10_mChi1000")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP10_mChi10_skimmedtree.root"       , "sig_ScalarZp_mZP10_mChi10")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP10_mChi150_skimmedtree.root"      , "sig_ScalarZp_mZP10_mChi150")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP10_mChi1_skimmedtree.root"        , "sig_ScalarZp_mZP10_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP10_mChi500_skimmedtree.root"      , "sig_ScalarZp_mZP10_mChi500")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP10_mChi50_skimmedtree.root"       , "sig_ScalarZp_mZP10_mChi50")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP15_mChi10_skimmedtree.root"       , "sig_ScalarZp_mZP15_mChi10")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP200_mChi150_skimmedtree.root"     , "sig_ScalarZp_mZP200_mChi150")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP200_mChi1_skimmedtree.root"       , "sig_ScalarZp_mZP200_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP200_mChi50_skimmedtree.root"      , "sig_ScalarZp_mZP200_mChi50")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP20_mChi1_skimmedtree.root"        , "sig_ScalarZp_mZP20_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP295_mChi150_skimmedtree.root"     , "sig_ScalarZp_mZP295_mChi150")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP300_mChi1_skimmedtree.root"       , "sig_ScalarZp_mZP300_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP300_mChi50_skimmedtree.root"      , "sig_ScalarZp_mZP300_mChi50")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP500_mChi150_skimmedtree.root"     , "sig_ScalarZp_mZP500_mChi150")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP500_mChi1_skimmedtree.root"       , "sig_ScalarZp_mZP500_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP500_mChi500_skimmedtree.root"     , "sig_ScalarZp_mZP500_mChi500")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP50_mChi10_skimmedtree.root"       , "sig_ScalarZp_mZP50_mChi10")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP50_mChi1_skimmedtree.root"        , "sig_ScalarZp_mZP50_mChi1")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP50_mChi50_skimmedtree.root"       , "sig_ScalarZp_mZP50_mChi50")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP95_mChi50_skimmedtree.root"       , "sig_ScalarZp_mZP95_mChi50")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","sig",0, "ScalarZp_mZP995_mChi500_skimmedtree.root"     , "sig_ScalarZp_mZP995_mChi500")

fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","bkg",0, "VHToGG_skimmedtree.root",	"VHToGG")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","bkg",0, "VBFHToGG_skimmedtree.root",	"VBFHToGG")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","bkg",0, "ttHJetToGG_skimmedtree.root",	"ttHJetToGG")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","bkg",0, "GluGluHToGG_skimmedtree.root",	"GluGluHToGG")

fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","bkg",0, "DiPhoton_skimmedtree.root",	"DiPhoton")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","bkg",0, "QCD_skimmedtree.root",		"QCD")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","bkg",0, "GJets_skimmedtree.root",		"GJets")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","bkg",0, "DYJetsToLL_skimmedtree.root",	"DYJetsToLL")
fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","bkg",0, "NonResBkg_skimmedtree.root",	"NonResBkg")

fitterFormatting($which_dphi_cuts,$metcut,"$indir","$outdir","data",0,"DoubleEG_skimmedtree.root",	"Output_Data")

.q

EOF
echo "Done"

echo "Adding MC Files Together"
hadd $outdir/Output_MC.root $outdir/sig* $outdir/GluGluHToGG* $outdir/DiPhoton* $outdir/V* $outdir/ttH* $outdir/QCD* $outdir/DY* $outdir/GJets* $outdir/NonResBkg*  



