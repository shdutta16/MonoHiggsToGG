# MonoHiggsToGG

## Setting up flashgg dev_legacy_runII with MonoHiggsToGG

1. Get flashgg (for more details [link](https://github.com/cms-analysis/flashgg/tree/dev_legacy_runII) )
   ```
   export SCRAM_ARCH=slc7_amd64_gcc700
   cmsrel CMSSW_10_6_29
   cd CMSSW_10_6_29/src
   cmsenv
   git cms-init
   cd $CMSSW_BASE/src 
   git clone -b dev_legacy_runII https://github.com/cms-analysis/flashgg 
   source flashgg/setup_flashgg.sh
   ```
   
2. Get MonoHiggsToGG (after following above steps) by cloning this repo: `git clone -b main https://github.com/shdutta16/MonoHiggsToGG`

3. Copy the `LeptonSelection` from MonoHiggsToGG setup to flashgg setup. This contains additional methods that are used by MonoHiggsToGG.
   ```
   cp MonoHiggsToGG/analysis/addfiles/LeptonSelection.h flashgg/Taggers/interface/LeptonSelection_MHgg.h
   cp MonoHiggsToGG/analysis/addfiles/LeptonSelection.cc flashgg/Taggers/src/LeptonSelection_MHgg.cc
   ```
   
4. Build: `scram b -j 10`
