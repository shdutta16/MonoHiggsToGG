#!/bin/sh

addname="optimized_allBins"

#indir="/eos/cms/store/group/phys_egamma/shdutta/2018Analysis_reloaded/WorkCopy/for_skim/DNN_skim/"
indir="/eos/cms/store/group/phys_egamma/shdutta/2018Analysis_reloaded/WorkCopy/for_skim/DNN_skim_DNNbins/"


dnnBins_mA200="0,0.03,0.05,0.1,0.2,0.39,0.75,0.93,0.98,0.99378,0.99835,1"
dnnBins_mA300="0,0.03,0.04,0.09,0.13,0.27,0.45,0.71,0.91,0.98,0.99183,1"
dnnBins_mA400="0,0.07,0.19,0.42,0.64,0.82,0.89,0.98,0.99285,0.99751,0.99885,1"
dnnBins_mA500="0,0.19,0.3,0.57,0.83,0.95,0.98,0.99228,0.99581,0.99845,0.99964,1"
dnnBins_mA600="0,0.41,0.57,0.8,0.93,0.98,0.99285,0.99733,0.99846,0.99964,0.99996,1"


#outdir="DNN_ntuples4fit_pho_newSig_${addname}_dnnBins_"${str_dnnBins}
outdir="DNN_ntuples4fit_${addname}_dnnBins"

mkdir -p $outdir

root -l -b << EOF
.L fitterFormatting_DNNcat_array.cc++

/*
// For simple DNN skimmed trees with uniform bins

fitterFormatting("$dnnBins","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA200_ma150_EOY_skimmedTree.root",	"sig_2HDMa_mA200_ma150")
fitterFormatting("$dnnBins","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA300_ma150_EOY_skimmedTree.root",	"sig_2HDMa_mA300_ma150")
fitterFormatting("$dnnBins","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA400_ma150_EOY_skimmedTree.root",	"sig_2HDMa_mA400_ma150")
fitterFormatting("$dnnBins","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA500_ma150_EOY_skimmedTree.root",	"sig_2HDMa_mA500_ma150")
fitterFormatting("$dnnBins","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA600_ma150_EOY_skimmedTree.root",	"sig_2HDMa_mA600_ma150")



fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree.root",	"VHToGG")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree.root",	"VBFHToGG")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree.root",	"ttHJetToGG")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree.root",	"GluGluHToGG")

fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree.root",  "DiPhoton")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree.root",		    "QCD")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree.root",             "GJets")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree.root",	    "DYJetsToLL")

fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree.root",    "TTGJets")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree.root", "TTGG_0Jets")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree.root",     "TTJets")
fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree.root",     "TGJets")

fitterFormatting("$dnnBins","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree.root", "TTWJetsToLNu")


fitterFormatting("$dnnBins","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree.root",	"Data")
*/


// For simple DNN skimmed trees with non-uniform bins


fitterFormatting("$dnnBins_mA200","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA200_ma150_EOY_skimmedTree.root",	"sig_2HDMa_mA200_ma150")

fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree.root",	"VHToGG_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree.root",	"VBFHToGG_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree.root",	"ttHJetToGG_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree.root",	"GluGluHToGG_mA200")

fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree.root",  "DiPhoton_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree.root",		    "QCD_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree.root",             "GJets_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree.root",	    "DYJetsToLL_mA200")

fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree.root",    "TTGJets_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree.root", "TTGG_0Jets_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree.root",     "TTJets_mA200")
fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree.root",     "TGJets_mA200")

fitterFormatting("$dnnBins_mA200","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree.root", "TTWJetsToLNu_mA200")

fitterFormatting("$dnnBins_mA200","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree.root",	"Data_mA200")




fitterFormatting("$dnnBins_mA300","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA300_ma150_EOY_skimmedTree.root",	"sig_2HDMa_mA300_ma150")

fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree.root",	"VHToGG_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree.root",	"VBFHToGG_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree.root",	"ttHJetToGG_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree.root",	"GluGluHToGG_mA300")

fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree.root",  "DiPhoton_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree.root",		    "QCD_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree.root",             "GJets_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree.root",	    "DYJetsToLL_mA300")

fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree.root",    "TTGJets_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree.root", "TTGG_0Jets_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree.root",     "TTJets_mA300")
fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree.root",     "TGJets_mA300")

fitterFormatting("$dnnBins_mA300","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree.root", "TTWJetsToLNu_mA300")

fitterFormatting("$dnnBins_mA300","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree.root",	"Data_mA300")




fitterFormatting("$dnnBins_mA400","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA400_ma150_EOY_skimmedTree.root",	"sig_2HDMa_mA400_ma150")

fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree.root",	"VHToGG_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree.root",	"VBFHToGG_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree.root",	"ttHJetToGG_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree.root",	"GluGluHToGG_mA400")

fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree.root",  "DiPhoton_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree.root",		    "QCD_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree.root",             "GJets_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree.root",	    "DYJetsToLL_mA400")

fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree.root",    "TTGJets_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree.root", "TTGG_0Jets_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree.root",     "TTJets_mA400")
fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree.root",     "TGJets_mA400")

fitterFormatting("$dnnBins_mA400","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree.root", "TTWJetsToLNu_mA400")

fitterFormatting("$dnnBins_mA400","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree.root",	"Data_mA400")




fitterFormatting("$dnnBins_mA500","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA500_ma150_EOY_skimmedTree.root",	"sig_2HDMa_mA500_ma150")

fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree.root",	"VHToGG_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree.root",	"VBFHToGG_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree.root",	"ttHJetToGG_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree.root",	"GluGluHToGG_mA500")

fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree.root",  "DiPhoton_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree.root",		    "QCD_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree.root",             "GJets_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree.root",	    "DYJetsToLL_mA500")

fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree.root",    "TTGJets_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree.root", "TTGG_0Jets_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree.root",     "TTJets_mA500")
fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree.root",     "TGJets_mA500")

fitterFormatting("$dnnBins_mA500","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree.root", "TTWJetsToLNu_mA500")

fitterFormatting("$dnnBins_mA500","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree.root",	"Data_mA500")




fitterFormatting("$dnnBins_mA600","$indir","$outdir","sig",0, "nTuple_2017_2HDMa_mA600_ma150_EOY_skimmedTree.root",	"sig_2HDMa_mA600_ma150")

fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree.root",	"VHToGG_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree.root",	"VBFHToGG_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree.root",	"ttHJetToGG_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree.root",	"GluGluHToGG_mA600")

fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree.root",  "DiPhoton_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree.root",		    "QCD_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree.root",             "GJets_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree.root",	    "DYJetsToLL_mA600")

fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree.root",    "TTGJets_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree.root", "TTGG_0Jets_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree.root",     "TTJets_mA600")
fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree.root",     "TGJets_mA600")

fitterFormatting("$dnnBins_mA600","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree.root", "TTWJetsToLNu_mA600")

fitterFormatting("$dnnBins_mA600","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree.root",	"Data_mA600")



.q

EOF
echo "Done"

echo "Adding MC Files Together"
hadd -f $outdir/Output_MC.root $outdir/sig* $outdir/GluGluHToGG* $outdir/DiPhoton* $outdir/V* $outdir/ttH* $outdir/QCD* $outdir/DY* $outdir/GJet* $outdir/T*
hadd -f $outdir/Output_Data.root $outdir/Data_*
rm $outdir/sig* $outdir/GluGluHToGG* $outdir/DiPhoton* $outdir/V* $outdir/ttH* $outdir/QCD* $outdir/DY* $outdir/GJet* $outdir/T*
rm $outdir/Data_*
