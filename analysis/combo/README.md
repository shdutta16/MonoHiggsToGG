# Directions for running combination

## Cards
Location of cards, current combine output, and jsons for plotting: 
dir = `/afs/cern.ch/work/m/mzientek/public/datacards_monoh_combo/`

* 2HDM cards: `[dir]/cards_2HDM/`
* 2HDM jsons: `[dir]/jsons_2HDM/`
* BARY cards: `[dir]/cards_BARY/`
* BARY jsons: `[dir]/jsons_BARY/`

## Setup combine area
```
# Setup higgs combine (in 74x)
export SCRAM_ARCH=slc6_amd64_gcc491
scram project CMSSW CMSSW_7_4_7
cd CMSSW_7_4_7/src
cmsenv
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
cd $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v6.3.1
scramv1 b clean; scramv1 b # always make a clean build, as scram doesn't always see updates to src/LinkDef.h

# Setup combine harvester
cd $CMSSW_BASE/src/
git clone https://github.com/cms-analysis/CombineHarvester.git CombineHarvester
scram b

# Setup area to run 
cd $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/
mkdir MonoHCombo
cd MonoHCombo/
cp ../CombineTools/scripts/combineTool.py .
```

## Pick up files needed for running my tools
```
# for running combine 
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/replaceZpNames.py
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/combineCards.sh
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/runCombineCards.sh
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/createjob.py
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/runAll.sh
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/launch_job.sh
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/submitjob.tmpl
# for plotting
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/collect2HDMLimits.py
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/plot1DlimitsAll.py
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/plot2DlimitsAll.py
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/runPlot1DLimits.sh
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/runPlot2DLimits.sh
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/plotting_interp.py
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/CMS_lumi.py
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/crosssectionZp2HDM.txt
wget https://raw.githubusercontent.com/mez34/MonoHiggsToGG/master/analysis/combo/crosssectionZpBaryonic.txt
```
## Running locally 
Do this only for a small num of points because each point takes ~10min to run.
Once you have checked things running locally, run in batch! (See next section.)

Run by calling `./combineCards.sh [MODEL] [MASS]` where MASS = mA or mDM. 
N.B. This script requires that combineTool.py and replaceZpNames.py be in the same dir. you are launching from. 
- This script runs over all mZp masses. 
- If you want to change Zp mass, modify vals_mZ. 
- You need to specify input directories for the datacards: ggindir and ttindir.
- This script calls combineCards.py (default script from combine to make the combined cards).
- Also calls replaceZpNames.py (a misnomer since it actually replaces A0 or DM) basically replacing TauTau card "MASS" with actual value of A0 or DM.
- And finally runs combine for the combined cards. By default it uses Combine Harvester, but you can switch to standard combine if wanted.
- Finally it moves all resulting higgsCombine files into results_MASS dir. and hadds them together.

## How to run in lxplus batch 
In principle `./runAll.sh [MODEL]` will launch lxplus batch jobs that run everything. However there are a few files that need modifying to put in your user area / correct inputs.

Details:
- `runAll.sh` will run `./launch_job.sh [MODEL] [MASS]` over A0 or DM masses for [MODEL] = 2HDM or BARY
  if you want to run over different A0 or DM mass points, change **vals_mA**.
- `launch_job.sh` copies the submitjob.tmpl and replaces MODEL and MASS in the template to the specified values
  making one `submitjob_MODEL_MASS.tmpl` for every specified A0/DM mass in runAll.sh.
  Further, the `launch_job.sh` then takes these new files and sends them to the grid. 
- The `submitjob.tmpl` **NEEDS to be updated to match your area. Change RUN_DIR**
  to match the PATH of the directory you are launching jobs from. N.B. do this before running launch_jobs.sh so that 
  the changes are made in all batch job submission templates.
- The `submitjob.tmpl` calls `./runCombineCards.sh [MODEL] [MASS]` which copies the needed files into the dir. combo_MODEL_mMASS
  and then runs `./combineCards.sh [MODEL] [MASS]`.
- `combineCards.sh` is the starting point for running in local (so see above for more info.) You will need to **specify here 
  the path to the input datacards (ggindir and ttindir)**. Keep in mind, if you use a relative path, that when launching batch jobs, 
  you will actually be launching a copy of this script from the combo_MODEL_mMASS dir. Also change in this script manually **vals_mZ** if you want to do something 
  other than the standard grid).

## Plotting
- Translate higgsCombine output root file to jsons that go into the 2D plots: `python collect2HDMLimits.py`
  specify: **model** and **indir** (takes root files from [indir]/combo_MODEL_mMASS/results_MASS), also setup needed **mZp and mA values**, and **outdir**. 
- Weight by cross section: `python scaleByXsec.py` specify: **indir** and **outdir** and **xsecfile**. This script basically collects all of the json files in json format in the indir, finds the corresponding xsec value in the xsec file and rewrites the jsons to outdir/ with values that are orig.value/xsec.
- Make 1D limit plots: `python plot1DlimitsAll.py [OUTDIR] [MODEL] [MASS]` (Can be called by `runPlot1DLimits.sh`) 
- Make 2D limit plots: `python plot2DlimitsAll.py -m [MODEL] -w [CHANNEL] -O [OUTDIR]` (Can be called by `runPlot2DLimits.sh`) 

## If needed, gg card production:
- Make cards (https://github.com/mez34/MonoHiggsToGG/blob/master/analysis/work/macros/makeGenericDataCard.py)
  which is called by bash script (https://github.com/mez34/MonoHiggsToGG/blob/master/analysis/work/macros/runMakeGenericDataCard.sh)
  starting points are for 2HDM: mA 300, mZp 600, for BARY: mX 1, mZp 10 -- so need this datacards/workspaces in input directory
- Clean cards to get them to match tautau names (https://github.com/mez34/MonoHiggsToGG/blob/master/analysis/combo/cleanDataCards.py) 
  callled by the bash script (https://github.com/mez34/MonoHiggsToGG/blob/master/analysis/combo/cleanCards.sh) 
  N.B. cards in the directory at the top of the Readme are already cleaned. 
  

