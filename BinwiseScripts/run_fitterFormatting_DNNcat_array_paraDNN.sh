#!/bin/sh

addname="optimized_allBins"

#indir="/eos/cms/store/group/phys_egamma/shdutta/2018Analysis_reloaded/WorkCopy/for_skim/DNN_skim/"
indir="/eos/user/s/shdutta/Store/2018Analysis_reloaded/nTuples/WorkCopy/DNN_skimmed_HEM/"


dnnBins_mA200="0, 0.112, 0.19,  0.385, 0.729, 0.908,   0.979,   0.99078, 0.99559, 0.99881, 1"
dnnBins_mA300="0, 0.04,  0.072, 0.106, 0.209, 0.385,   0.638,   0.863,   0.962,   0.99043, 1"
dnnBins_mA400="0, 0.255, 0.433, 0.644, 0.857, 0.955,   0.988,   0.99384, 0.99751, 0.99909, 1"
dnnBins_mA500="0, 0.329, 0.521, 0.746, 0.933, 0.98,    0.99291, 0.99654, 0.99851, 0.99964, 1"
dnnBins_mA600="0, 0.654, 0.838, 0.941, 0.975, 0.99127, 0.99603, 0.99873, 0.99964, 0.99996, 1"
dnnBins_mA700="0, 0.619, 0.849, 0.95,  0.986, 0.99334, 0.99696, 0.99861, 0.99964, 0.99999, 1"
dnnBins_mA800="0, 0.478, 0.719, 0.877, 0.954, 0.988,   0.99581, 0.99856, 0.99974, 0.99999, 1"
dnnBins_mA900="0, 0.497, 0.705, 0.919, 0.968, 0.99127, 0.996,   0.99864, 0.99964, 0.99999, 1"


#outdir="paraDNN_ntuples4fit_pho_newSig_${addname}_dnnBins_"${str_dnnBins}
outdir="paraDNN_ntuples4fit_${addname}_dnnBins"

mkdir -p $outdir

root -l -b << EOF
.L fitterFormatting_DNNcat_array.cc++


// For parametric DNN skimmed trees

fitterFormatting("$dnnBins_mA200","$indir","$outdir","sig",0, "nTuple_2HDMa_mA200_ma150_skimmedTree_mA_200p0.root",	"sig_2HDMa_mA200_ma150")

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




fitterFormatting("$dnnBins_mA300","$indir","$outdir","sig",0, "nTuple_2HDMa_mA300_ma150_skimmedTree_mA_300p0.root",	"sig_2HDMa_mA300_ma150")

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




fitterFormatting("$dnnBins_mA400","$indir","$outdir","sig",0, "nTuple_2HDMa_mA400_ma150_skimmedTree_mA_400p0.root",	"sig_2HDMa_mA400_ma150")

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




fitterFormatting("$dnnBins_mA500","$indir","$outdir","sig",0, "nTuple_2HDMa_mA500_ma150_skimmedTree_mA_500p0.root",	"sig_2HDMa_mA500_ma150")

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




fitterFormatting("$dnnBins_mA600","$indir","$outdir","sig",0, "nTuple_2HDMa_mA600_ma150_skimmedTree_mA_600p0.root",	"sig_2HDMa_mA600_ma150")

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





fitterFormatting("$dnnBins_mA700","$indir","$outdir","sig",0, "nTuple_2HDMa_mA700_ma150_skimmedTree_mA_700p0.root",	"sig_2HDMa_mA700_ma150")

fitterFormatting("$dnnBins_mA700","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree_mA_700p0.root",	"VHToGG_mA700")
fitterFormatting("$dnnBins_mA700","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree_mA_700p0.root",	"VBFHToGG_mA700")
fitterFormatting("$dnnBins_mA700","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree_mA_700p0.root",	"ttHJetToGG_mA700")
fitterFormatting("$dnnBins_mA700","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree_mA_700p0.root",	"GluGluHToGG_mA700")

fitterFormatting("$dnnBins_mA700","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree_mA_700p0.root", "DiPhoton_mA700")
fitterFormatting("$dnnBins_mA700","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree_mA_700p0.root",		    "QCD_mA700")
fitterFormatting("$dnnBins_mA700","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree_mA_700p0.root",            "GJets_mA700")
fitterFormatting("$dnnBins_mA700","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree_mA_700p0.root",	    "DYJetsToLL_mA700")

fitterFormatting("$dnnBins_mA700","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree_mA_700p0.root",    "TTGJets_mA700")
fitterFormatting("$dnnBins_mA700","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree_mA_700p0.root", "TTGG_0Jets_mA700")
fitterFormatting("$dnnBins_mA700","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree_mA_700p0.root",     "TTJets_mA700")
fitterFormatting("$dnnBins_mA700","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree_mA_700p0.root",     "TGJets_mA700")

fitterFormatting("$dnnBins_mA700","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree_mA_700p0.root", "TTWJetsToLNu_mA700")

fitterFormatting("$dnnBins_mA700","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree_mA_700p0.root",	"Data_mA700")





fitterFormatting("$dnnBins_mA800","$indir","$outdir","sig",0, "nTuple_2HDMa_mA800_ma150_skimmedTree_mA_800p0.root",	"sig_2HDMa_mA800_ma150")

fitterFormatting("$dnnBins_mA800","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree_mA_800p0.root",	"VHToGG_mA800")
fitterFormatting("$dnnBins_mA800","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree_mA_800p0.root",	"VBFHToGG_mA800")
fitterFormatting("$dnnBins_mA800","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree_mA_800p0.root",	"ttHJetToGG_mA800")
fitterFormatting("$dnnBins_mA800","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree_mA_800p0.root",	"GluGluHToGG_mA800")

fitterFormatting("$dnnBins_mA800","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree_mA_800p0.root", "DiPhoton_mA800")
fitterFormatting("$dnnBins_mA800","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree_mA_800p0.root",		    "QCD_mA800")
fitterFormatting("$dnnBins_mA800","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree_mA_800p0.root",            "GJets_mA800")
fitterFormatting("$dnnBins_mA800","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree_mA_800p0.root",	    "DYJetsToLL_mA800")

fitterFormatting("$dnnBins_mA800","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree_mA_800p0.root",    "TTGJets_mA800")
fitterFormatting("$dnnBins_mA800","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree_mA_800p0.root", "TTGG_0Jets_mA800")
fitterFormatting("$dnnBins_mA800","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree_mA_800p0.root",     "TTJets_mA800")
fitterFormatting("$dnnBins_mA800","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree_mA_800p0.root",     "TGJets_mA800")

fitterFormatting("$dnnBins_mA800","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree_mA_800p0.root", "TTWJetsToLNu_mA800")

fitterFormatting("$dnnBins_mA800","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree_mA_800p0.root",	"Data_mA800")





fitterFormatting("$dnnBins_mA900","$indir","$outdir","sig",0, "nTuple_2HDMa_mA900_ma150_skimmedTree_mA_900p0.root",	"sig_2HDMa_mA900_ma150")

fitterFormatting("$dnnBins_mA900","$indir","$outdir","bkg",0, "nTuple_VHToGG_skimmedTree_mA_900p0.root",	"VHToGG_mA900")
fitterFormatting("$dnnBins_mA900","$indir","$outdir","bkg",0, "nTuple_VBFHToGG_skimmedTree_mA_900p0.root",	"VBFHToGG_mA900")
fitterFormatting("$dnnBins_mA900","$indir","$outdir","bkg",0, "nTuple_ttHJetToGG_skimmedTree_mA_900p0.root",	"ttHJetToGG_mA900")
fitterFormatting("$dnnBins_mA900","$indir","$outdir","bkg",0, "nTuple_GluGluHToGG_skimmedTree_mA_900p0.root",	"GluGluHToGG_mA900")

fitterFormatting("$dnnBins_mA900","$indir","$outdir","bkg",0, "nTuple_DiPhotonJetsBox_skimmedTree_mA_900p0.root", "DiPhoton_mA900")
fitterFormatting("$dnnBins_mA900","$indir","$outdir","bkg",0, "nTuple_QCD_skimmedTree_mA_900p0.root",		    "QCD_mA900")
fitterFormatting("$dnnBins_mA900","$indir","$outdir","bkg",0, "nTuple_GJet_skimmedTree_mA_900p0.root",            "GJets_mA900")
fitterFormatting("$dnnBins_mA900","$indir","$outdir","bkg",0, "nTuple_DYJetsToLL_skimmedTree_mA_900p0.root",	    "DYJetsToLL_mA900")

fitterFormatting("$dnnBins_mA900","$indir","$outdir","bkg",0, "nTuple_TTGJets_skimmedTree_mA_900p0.root",    "TTGJets_mA900")
fitterFormatting("$dnnBins_mA900","$indir","$outdir","bkg",0, "nTuple_TTGG_0Jets_skimmedTree_mA_900p0.root", "TTGG_0Jets_mA900")
fitterFormatting("$dnnBins_mA900","$indir","$outdir","bkg",0, "nTuple_TTJets_skimmedTree_mA_900p0.root",     "TTJets_mA900")
fitterFormatting("$dnnBins_mA900","$indir","$outdir","bkg",0, "nTuple_TGJets_skimmedTree_mA_900p0.root",     "TGJets_mA900")

fitterFormatting("$dnnBins_mA900","$indir","$outdir","bkg",0, "nTuple_TTWJetsToLNu_skimmedTree_mA_900p0.root", "TTWJetsToLNu_mA900")

fitterFormatting("$dnnBins_mA900","$indir","$outdir","data",0,"nTuple_DoubleEG_skimmedTree_mA_900p0.root",	"Data_mA900")


.q

EOF
echo "Done"

echo "Adding MC Files Together"
hadd -f $outdir/Output_MC.root $outdir/sig* $outdir/GluGluHToGG* $outdir/DiPhoton* $outdir/V* $outdir/ttH* $outdir/QCD* $outdir/DY* $outdir/GJet* $outdir/T*
hadd -f $outdir/Output_Data.root $outdir/Data_*
rm $outdir/sig* $outdir/GluGluHToGG* $outdir/DiPhoton* $outdir/V* $outdir/ttH* $outdir/QCD* $outdir/DY* $outdir/GJet* $outdir/T*
rm $outdir/Data_*
