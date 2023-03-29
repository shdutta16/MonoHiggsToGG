# Setup Standard Analysis

From macros dir:
- `mergeTrees.sh` (merge trees from output of analyzer to make one root file per sample)
- `runAddWeights.sh LUMI Sample` (runs `../scripts/addWeights.C` to weight to LUMI (in 1/pb) to sample after analyzer)
- `runAddWeightsAll.sh LUMI` (same but for all samples in dir.)
- `mergeSpecies.sh` (merge the root files for various samples (i.e. diff pt bins of QCD into one QCD sample) 
- `runSkim.sh Sample` (runs `../scripts/skim.C` to apply sel. to prep samples)
- `runSkimAll.sh` (same but for all samples in dir.)

# Miscelaneous Scripts

From macros dir:
- `getSFvalues.C` (takes 2D SF root file and makes text file for input to analyzer)
- `optimization.C` (runs cut optimization study)
- `makeEfficiencyList.C` (makes raw text lists w/ efficiencies & yields for all signal samples)
- `convertToTable.py` (converts the raw eff from makeEfficiencyList.C to pretty latex table)
- `plot2Defficiency.C` (converts raw eff from makeEfficiencyList.C to nice 2D plot)
- `makeEfficiencyFit.C` (fit various efficiencies for each sample)

# Run the Analysis

Compile with `make`, clean with `make clean`.
Run the analysis by calling main. Can get config options with `./main --help`. 
Standard run configuration:  

``` ./main --outdir diPhoPlots/25ns_v80X_moriond17_v3/ --unblind --use-Data --do-standard --do-analysis --which-sel 2 --do-stack --mergebkgs --doQCDrewgt --scaletodata ```



