#! /bin/sh

addname=""

indir="/eos/cms/store/group/phys_egamma/shdutta/2018Analysis_reloaded/WorkCopy/for_skim/DNN_skim/"

dnnBins=0.0_0.1_0.2_0.3_0.4_0.5_0.6_0.7_0.8_0.9_1.0

outdir="ntuples4fit_pho_newSig_test${addname}_dnnBins_"${dnnBins}

mkdir -p $outdir

root -l -b << EOF
.L fitterFormatting_DNNcat_array.cc++

fitterFormatting("$dnnBins","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA200_ma150_EOY_skimmedTree_mA_600p0.root",	"sig_2HDMa_mA200_ma150")
fitterFormatting("$dnnBins","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA300_ma150_EOY_skimmedTree_mA_600p0.root",	"sig_2HDMa_mA300_ma150")
fitterFormatting("$dnnBins","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA400_ma150_EOY_skimmedTree_mA_600p0.root",	"sig_2HDMa_mA400_ma150")
fitterFormatting("$dnnBins","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA500_ma150_EOY_skimmedTree_mA_600p0.root",	"sig_2HDMa_mA500_ma150")
fitterFormatting("$dnnBins","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA600_ma150_EOY_skimmedTree_mA_600p0.root",	"sig_2HDMa_mA600_ma150")


fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree_mA_600p0.root",	"VHToGG")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree_mA_600p0.root",	"VBFHToGG")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree_mA_600p0.root",	"ttHJetToGG")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree_mA_600p0.root",	"GluGluHToGG")

fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree_mA_600p0.root", "DiPhoton")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree_mA_600p0.root",		    "QCD")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree_mA_600p0.root",            "GJets")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree_mA_600p0.root",	    "DYJetsToLL")

fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree_mA_600p0.root",    "TTGJets")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree_mA_600p0.root", "TTGG_0Jets")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree_mA_600p0.root",     "TTJets")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree_mA_600p0.root",     "TGJets")

fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree_mA_600p0.root", "TTWJetsToLNu")


fitterFormatting("$dnnBins","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree_mA_600p0.root",	"Output_Data")

.q

EOF
echo "Done"

echo "Adding MC Files Together"
hadd $outdir/Output_MC.root $outdir/sig* $outdir/GluGluHToGG* $outdir/DiPhoton* $outdir/V* $outdir/ttH* $outdir/QCD* $outdir/DY* $outdir/GJet* $outdir/T*
