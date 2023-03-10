#run_skim_DNN.sh

inDir=for_skim/
tree=DiPhotonTree
outDir=DNN_skim
dnnModel=~/ANAWORK/MonoHgg/2018Analysis_reloaded/limitPlots/DNN/AnalysisSelection_allvar_halfStat_womgg_ANN_parametric_model.json
dnnWeights=~/ANAWORK/MonoHgg/2018Analysis_reloaded/limitPlots/DNN/AnalysisSelection_allvar_halfStat_womgg_ANN_parametric_model.h5


cutVal=0.0 # all events will be selected
outTrees=y
effOut=na
cutBasedEff=na
mggOut=na
effVsDNN=nw
metCatDNNeff=n
medMass=600.0


# Signal                                                                              outTrees effOut cutBasedEff mggOut effVsDNN metCatDNNeff
python skim_DNN.py "$inDir" "nTuple_2017_2HDMa_mA200_ma150_EOY.root" "$outDir" "$tree"  $cutVal "y" "nw" "nw" "nw" "nw" "n" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_2017_2HDMa_mA300_ma150_EOY.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_2017_2HDMa_mA400_ma150_EOY.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_2017_2HDMa_mA500_ma150_EOY.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_2017_2HDMa_mA600_ma150_EOY.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass


# Background
python skim_DNN.py "$inDir" "nTuple_DYJetsToLL.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_DiPhotonJetsBox.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_GJet.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_GluGluHToGG.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_QCD.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_TGJets.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_TTGG_0Jets.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_TTGJets.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_TTJets.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_TTWJetsToLNu.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_VBFHToGG.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_VHToGG.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_WGToLNuG.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass
python skim_DNN.py "$inDir" "nTuple_ttHJetToGG.root" "$outDir" "$tree"  $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass


# Data
python skim_DNN.py "$inDir" "nTuple_DoubleEG.root" "$outDir" "$tree" $cutVal "$outTrees" "$effOut" "$cutBasedEff" "$mggOut" "$effVsDNN" "$metCatDNNeff" "$dnnModel" "$dnnWeights" $medMass

## Background
#python skim_DNN.py "" "FailingPhotonID_2017C_88percent.root" "$outDir" "$tree" $cutVal "$effWriteMode" "$cutBasedEff" "$mggOut" $medMass

#hadd -f DNN_new/skimmedTree_mgg/ntuple_mgg.root DNN_new/skimmedTree_mgg/nTuple_*
