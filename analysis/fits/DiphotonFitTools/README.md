# Directions for Setting up & Running Fit

## SETUP
Setup CMSSW, get FLASHgg, Fit dir., HiggsCombine:

 ```
 # setup CMSSW
 cmsrel CMSSW_8_0_28
 cd CMSSW_8_0_28/src
 cmsenv
 git cms-init
 cd $CMSSW_BASE/src 
 
 # get FLASHgg first
 git clone https://github.com/cms-analysis/flashgg flashgg
 source flashgg/setup.sh
 
 # get diphotons analysis dir
 cd ${CMSSW_BASE}/src
 git clone https://github.com/musella/diphotons.git
 
 # get HiggsCombine dir
 cd ${CMSSW_BASE}/src
 git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
 cd HiggsAnalysis/CombinedLimit
 git checkout v7.0.1
 ```

N.B. Exact commits I used: 
- FLASHgg: (ef07e16)
- diphotons: (a4b40a7)
- HiggsCombine: v7.0.1 (00e57d7)

Hand modify a few files: 
- $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/plotting/bandUtils.cxx
  - Line #20: change the bool use_precomputed_quantiles `false` -> `true`
- $CMSSW_BASE/src/diphotons/RooUtils/src/classes.h 
  - Line #6: change `dictionary` -> `roodictionary`

Copy all of the files in this directory to: $CMSSW/src/diphotons/Analysis/macros/

Compile everything: 
```
cd $CMSSW_BASE/src
SCRAM_IGNORE_PACKAGES="flashgg/Validation" scram b -j 10
```

## RUN

```
cd $CMSSW/src/diphotons/Analysis/macros/

# Run the fit
./combine_maker_MonoHgg.sh {INDIR} --lumi 35.9 --fit-name cic --mc-file Output_MC.root --fit-background --redo-input 1

# Run combine 
./combineall_MonoHgg.sh {INDIR} --hadd --model ZpBaryonic -C 0.9 -M Asymptotic --run both 

# Make limit plots
./limit_plots_MonoHgg.sh {INDIR}
```

Extra details on the running: 
- `combine_maker_MonoHgg.sh` is the master script that has two parts: 
  - Part 1) setup templates: `template_maker_MonoHgg.py` -- reads the input jsons: `templates_maker_MonoHgg.json` (ntuple tree structure and sample names) & `template_maker_prepare_MonoHgg.json`(categories i.e. high/lowMET and additional selection). This step makes log & root file that go into Part 2 (this part is called by option `--redo-input 1`, but if you have the outputs already, can just run part 2 alone). 
  - Part 2) run the fit: `combine_maker_MonoHgg.py` -- to make datacards/ws & fit plots that are written to -O dir
- `combineall_MonoHgg.sh` runs combine. Need to specify --model (currently ZpBaryonic or 2HDM supported) -C (CL percentage) -M (method)
 
 







