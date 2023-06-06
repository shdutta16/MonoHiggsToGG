#!/bin/sh

addname="optimized_allBins"

#indir="/eos/cms/store/group/phys_egamma/shdutta/2018Analysis_reloaded/WorkCopy/for_skim/DNN_skim/"
indir="/eos/cms/store/group/phys_egamma/shdutta/2018Analysis_reloaded/WorkCopy/for_skim/paraDNN_skim_DNNbins/"

#str_dnnBins=0p0_0p9_bin0p1_0p90_0p99_bin0p01_0p990_1p0_bin0p001


#dnnBins_mA200="0.3,0.56,0.8,0.98,0.99176,0.9964,1"
#dnnBins_mA300="0.2,0.4,0.69,0.89,0.97,0.99073,1"
#dnnBins_mA400="0.75,0.91,0.98,0.99004,0.99513,0.99786,1"
#dnnBins_mA500="0.86,0.94,0.99086,0.99595,0.99794,0.9991,1"
#dnnBins_mA600="0.93,0.98,0.99442,0.99799,0.99928,0.9999,1"


dnnBins_mA200="0,0.06,0.1,0.18,0.3,0.56,0.8,0.93,0.98,0.99176,0.9964,1"
dnnBins_mA300="0,0.02,0.03,0.06,0.13,0.2,0.4,0.69,0.89,0.97,0.99073,1"
dnnBins_mA400="0,0.06,0.2,0.31,0.56,0.75,0.91,0.98,0.99004,0.99513,0.99786,1"
dnnBins_mA500="0,0.08,0.18,0.24,0.57,0.86,0.94,0.99086,0.99595,0.99794,0.9991,1"
dnnBins_mA600="0,0.07,0.18,0.41,0.78,0.93,0.98,0.99442,0.99799,0.99928,0.9999,1"


#dnnBins_mA200="0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.90,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.990,0.991,0.992,0.993,0.994,0.995,0.996,0.997,0.998,0.999,1.0"
#dnnBins_mA300="0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.90,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.990,0.991,0.992,0.993,0.994,0.995,0.996,0.997,0.998,0.999,1.0"
#dnnBins_mA400="0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.90,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.990,0.991,0.992,0.993,0.994,0.995,0.996,0.997,0.998,0.999,1.0"
#dnnBins_mA500="0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.90,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.990,0.991,0.992,0.993,0.994,0.995,0.996,0.997,0.998,0.999,1.0"
#dnnBins_mA600="0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.90,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.990,0.991,0.992,0.993,0.994,0.995,0.996,0.997,0.998,0.999,1.0"


#outdir="paraDNN_ntuples4fit_pho_newSig_${addname}_dnnBins_"${str_dnnBins}
outdir="paraDNN_ntuples4fit_${addname}_dnnBins"

mkdir -p $outdir

root -l -b << EOF
.L fitterFormatting_DNNcat_array.cc++


// For parametric DNN skimmed trees

fitterFormatting("$dnnBins_mA200","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA200_ma150_EOY_skimmedTree_mA_200p0.root",	"sig_2HDMa_mA200_ma150")

fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree_mA_200p0.root",	"VHToGG_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree_mA_200p0.root",	"VBFHToGG_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree_mA_200p0.root",	"ttHJetToGG_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree_mA_200p0.root",	"GluGluHToGG_mA200")

fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree_mA_200p0.root", "DiPhoton_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree_mA_200p0.root",		    "QCD_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree_mA_200p0.root",            "GJets_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree_mA_200p0.root",	    "DYJetsToLL_mA200")

fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree_mA_200p0.root",    "TTGJets_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree_mA_200p0.root", "TTGG_0Jets_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree_mA_200p0.root",     "TTJets_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree_mA_200p0.root",     "TGJets_mA200")

fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree_mA_200p0.root", "TTWJetsToLNu_mA200")

fitterFormatting("$dnnBins_mA200","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree_mA_200p0.root",	"Data_mA200")




fitterFormatting("$dnnBins_mA300","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA300_ma150_EOY_skimmedTree_mA_300p0.root",	"sig_2HDMa_mA300_ma150")

fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree_mA_300p0.root",	"VHToGG_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree_mA_300p0.root",	"VBFHToGG_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree_mA_300p0.root",	"ttHJetToGG_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree_mA_300p0.root",	"GluGluHToGG_mA300")

fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree_mA_300p0.root", "DiPhoton_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree_mA_300p0.root",		    "QCD_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree_mA_300p0.root",            "GJets_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree_mA_300p0.root",	    "DYJetsToLL_mA300")

fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree_mA_300p0.root",    "TTGJets_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree_mA_300p0.root", "TTGG_0Jets_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree_mA_300p0.root",     "TTJets_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree_mA_300p0.root",     "TGJets_mA300")

fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree_mA_300p0.root", "TTWJetsToLNu_mA300")

fitterFormatting("$dnnBins_mA300","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree_mA_300p0.root",	"Data_mA300")




fitterFormatting("$dnnBins_mA400","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA400_ma150_EOY_skimmedTree_mA_400p0.root",	"sig_2HDMa_mA400_ma150")

fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree_mA_400p0.root",	"VHToGG_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree_mA_400p0.root",	"VBFHToGG_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree_mA_400p0.root",	"ttHJetToGG_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree_mA_400p0.root",	"GluGluHToGG_mA400")

fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree_mA_400p0.root", "DiPhoton_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree_mA_400p0.root",		    "QCD_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree_mA_400p0.root",            "GJets_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree_mA_400p0.root",	    "DYJetsToLL_mA400")

fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree_mA_400p0.root",    "TTGJets_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree_mA_400p0.root", "TTGG_0Jets_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree_mA_400p0.root",     "TTJets_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree_mA_400p0.root",     "TGJets_mA400")

fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree_mA_400p0.root", "TTWJetsToLNu_mA400")

fitterFormatting("$dnnBins_mA400","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree_mA_400p0.root",	"Data_mA400")




fitterFormatting("$dnnBins_mA500","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA500_ma150_EOY_skimmedTree_mA_500p0.root",	"sig_2HDMa_mA500_ma150")

fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree_mA_500p0.root",	"VHToGG_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree_mA_500p0.root",	"VBFHToGG_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree_mA_500p0.root",	"ttHJetToGG_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree_mA_500p0.root",	"GluGluHToGG_mA500")

fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree_mA_500p0.root", "DiPhoton_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree_mA_500p0.root",		    "QCD_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree_mA_500p0.root",            "GJets_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree_mA_500p0.root",	    "DYJetsToLL_mA500")

fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree_mA_500p0.root",    "TTGJets_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree_mA_500p0.root", "TTGG_0Jets_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree_mA_500p0.root",     "TTJets_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree_mA_500p0.root",     "TGJets_mA500")

fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree_mA_500p0.root", "TTWJetsToLNu_mA500")

fitterFormatting("$dnnBins_mA500","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree_mA_500p0.root",	"Data_mA500")




fitterFormatting("$dnnBins_mA600","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA600_ma150_EOY_skimmedTree_mA_600p0.root",	"sig_2HDMa_mA600_ma150")

fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree_mA_600p0.root",	"VHToGG_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree_mA_600p0.root",	"VBFHToGG_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree_mA_600p0.root",	"ttHJetToGG_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree_mA_600p0.root",	"GluGluHToGG_mA600")

fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree_mA_600p0.root", "DiPhoton_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree_mA_600p0.root",		    "QCD_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree_mA_600p0.root",            "GJets_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree_mA_600p0.root",	    "DYJetsToLL_mA600")

fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree_mA_600p0.root",    "TTGJets_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree_mA_600p0.root", "TTGG_0Jets_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree_mA_600p0.root",     "TTJets_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree_mA_600p0.root",     "TGJets_mA600")

fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree_mA_600p0.root", "TTWJetsToLNu_mA600")

fitterFormatting("$dnnBins_mA600","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree_mA_600p0.root",	"Data_mA600")


.q

EOF
echo "Done"

echo "Adding MC Files Together"
hadd -f $outdir/Output_MC.root $outdir/sig* $outdir/GluGluHToGG* $outdir/DiPhoton* $outdir/V* $outdir/ttH* $outdir/QCD* $outdir/DY* $outdir/GJet* $outdir/T*
hadd -f $outdir/Output_Data.root $outdir/Data_*
rm $outdir/sig* $outdir/GluGluHToGG* $outdir/DiPhoton* $outdir/V* $outdir/ttH* $outdir/QCD* $outdir/DY* $outdir/GJet* $outdir/T*
rm $outdir/Data_*
