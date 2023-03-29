<<<<<<< HEAD
-----------------------------------------------------------
# MonoHiggsToGG analysis
-----------------------------------------------------------
This repo is https://github.com/mez34/MonoHiggsToGG

But originally comes from: https://github.com/mez34/MonoHgg

Check there for History before 17 Nov 2015

-----------------------------------------------------------


-----------------------------------------------------------
# Run the Analysis
-----------------------------------------------------------

# MicroAOD to diPhotonTrees 
This package depends on [flashgg](https://github.com/cms-analysis/flashgg).

(Follow what is done here from P. Musella: https://github.com/cms-analysis/flashgg/tree/master/MetaData 
 and here from C. Rovelli: https://github.com/musella/diphotons/tree/master/fullAnalysisRoma )

## Step 1) Produce catalogue of MicroAODs and Extract JSON & PU Weights File
Once MicroAOD files are produced run these scripts to create the json file (catalogue) and  compute the weights:

- `fggManageSamples.py -C CAMPAIGN -V VERSION import`
- `fggManageSamples.py -C CAMPAIGN review`
- `fggManageSamples.py -C CAMPAIGN check`

NB: This searches for datasets matching the form: *CAMPAIGN-VERSION*

### a) Extract JSON
Extract the processed json (used as an input in the analyzer) and used to compute the PU weight file: 
- `fggManageSamples.py -C CAMPAIGN getlumi <full dataset name>` 

This json corresponds to the processed (through FLASHgg) dataset. We are interested in the the AND of this json and the golden json. 
The convolution json is applyed in the analyzer to make sure we are restricted to the right portion of the data.
This is specified in the diPhoAna.py (and moriond17diPhoAnaBATCH.py) in `JSONfile`.
To get this convolution of these jsons use brilcalc:
- `compareJSON.py --and processed.json golden.json >> processedANDgolden.json`
 
### b) Get PU Weights File
To get pileup in data, only need to specify MyAnalysisJSON.txt:

```pileupCalc.py -i MyAnalysisJSON.txt --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/pileup_latest.txt --calcMode true --minBiasXsec 69000 --maxPileupBin 70 --numPileupBins 70  MyDataPileupHistogram.root```

To get the PU weights file, next run the macro `pileupWeights.C` (found in macro dir.)
which uses MyDataPileupHistogram.root and compares it with a MC pileup histogram found in: 
/afs/cern.ch/user/c/crovelli/public/json2016/prompt/mcPUhisto___25ns_Spring16MC_PUScenarioV1_PoissonOOTPU.root 


This produces the `pileupWeights.root` file which is used in the analyzer.

## Step 2) Copy json to the scripts/list* directory
- Copy directly from FLASHgg catalogue or list as produced in Step 1.
- If json file is not separated by name can extract smaller json files with:
`./runExtractJSONS.sh` which calls: `python extractJSONS.py -i input.json -o samplename -d outputdir` 

## Step 3) Extract files and weights
This creates .list and .weight files in list* directory

Write the proper name of the catalogue in the extract*.py scripts and write the name of the samples in:
- `python extractWeights.py`
- `python extractFiles.py`

OR run `./runExtractFilesAndWeights.sh` does the same thing (takes input list of .json files and outputs weight and files in same list dir.)

## Step 4) Run in local the diphoton analyzer (from python directory):
- Write by hand one microAOD file that can be taken from the json file
- Fix by hand xsec and sumDataSet that can be found from the corresponding json file
- For data, specify the `JSONfile` as mentioned earlier
- Specify the bool isMC = true or false
- Optional: put in PU reweighting file (`puWFileName`) 
- called by `cmsRun diPhoAna.py`
   
## Step 5) Run in batch the diphoton analyzer (from script directory):
To make it works one needs:
- a list of files in script/list directory with the name of the samples 
- a list of weights in script/list directoy with the name of the samples
- the name of the list directory needs to be written in submitBatchDiPho.py
- specify the bool isMC in diPhoAnaBATCH.py
- the value of the xsec
- add optional PU reweighting & input PU reweighting file
- the output directory either in eos or in lxpus (this has to be fixed in the submitBatchDiPho.py script by hand)

Example on how to run: 
``` 
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GJets_HT-100to200 0 7 pippo 1534. 1 
```
NB. The name GJets_HT-100to200 has to match the one of the .list and the .weight files.

Can also run: `./submitAll_DiPhioton.py` which has the names of current samples & their xsecs
      
## Step 6) Manage the output trees before making plots 
From the macro directory:

- Merge the output files with `./mergeTrees.sh` specifying in here the output directory
- Add the weights to the trees with addWeightsToTree.cc run by `./weighTrees.sh LUMI` which weights for the provided `LUMI` (in pb^-1)
- Merge the species with `./mergeSpecies.sh`

NB. The structure of how to use these scripts can be seen in `doAll.sh`

If you add additional variables to the ntuples in the analyzer, you need to modify addWeightsToTree.cc to include these variables.

## Step 7) Produce plots 
The analysis is done in CMSSW_8_0_26_patch1
- `make` (to compile) 
- `./main` (to run)
Can use `make clean` to clean.

Choose which selection to apply (whichSelection).
The ones that are currently in place:
-  if (whichSelection == 0) selName = "OrigSel";
-  if (whichSelection == 1) selName = "OptSel1";  
-  if (whichSelection == 2) selName = "OptSel2"; 
-  if (whichSelection == 3) selName = "OptSel3"; 
-  if (whichSelection == 4) selName = "OptSel4"; 

The values of the cuts on the pTs are defined in Plotter.cpp. The MET cuts are also defined in the Plotter & in ABCD.cpp.
If you want to add additional selection options, you will need to add the respective cuts to both of these.

Specify the input directory for the samples (inDir) & your output directory (outDir).
Also specify the total luminosity (lumi) in fb^-1 and the type of plots (type) wanted - i.e. png/pdf.  

In main.cpp set the following bools. 
These are the ones you will have to work with most frequently: 
- (doPlots) 	  : calls Plotter.cpp 	  --- makes the histos for each sample individually
- (doComb)  	  : calls Combiner.cpp 	  --- overlays and stacks samples in plots
- (doABCD)	  : calls ABCDMethod.cpp  --- does the ABCD C&C analysis
- (doMETCorr)	  : calls METCorr2016.cpp --- MET phi correction is calculated to t1pfmet 
- (doQCDrescale)  : rescale GJet for QCD  --- rescale GJet by QCD integral to estimate the QCD contribution
- (doBlind)	  : blinds data in Plots  --- blinds the data mass & met distributions

Bools that will likely not be needed anymore and are defaulted to false:
- (doFakeData)    : calls Plotter.cpp for fake data -- requires having a sample called FakeData.root in the samples inDir.
- (sortMC)        : before doing Combiner.cpp, this sorts the MC for plotting by smallest to biggest.
- (makePURWfiles) : calls ReweightPU.cpp  --- makes PURW files for samples)
- (doReweightPU)  : opens PURW files      --- does PURW instead of weighting=1

When you first run the plotting tools for a specific data and MC dataset, you need to run with doMETCorr = true. 
This produces the root files (in the same input dir. as the data) that has the MET Phi corrections for both Data and MC.
Then when you set this bool to false, it will gather the needed corrections from the saved root file. 

The basic flow of these tools: 
- Each sample is run individually through the Plotter.cpp which applies the appropriate selection & corrections and outputs a root file: outDir/SampleName/plots_SampleName.root and some TH2D plots in the same directory. The default is to not draw the png/pdf versions of these plots (but these lines can be uncommented in the Plotter if wanted). 
- The Combiner.cpp takes the output root file from the Plotter and does for each plot that is specified in fTH1DNames an overlay of the shapes for each MC sample and a Data/MC stack plot. Both linear & log plots are produced. Some additional special plots are produced (efficiencies, certain special shape plots etc.). Also the MET efficiencies for various corrections (taken using the cut MET > 80) are calculated and collected in outDir/ResultsSystematicsForLatex.tex . To be fixed -- change the MET > 80 to a parameter specified like in the Plotter/ABCD. 
- Optionally: ABCD.cpp does the ABCD cut-and-count analysis and produces a datacard for each signal sample, cards found in outDir/ABCD/DataCard_SIGNAME_metCUT.txt. In the same directory, 3 tables in Latex form are found (ResultsTableForLatex.tex) which has inclusive numbers, numbers in signal region, and the results of the ABCD analysis. Unfortunately, calling ABCD crashes the first time it is run -- rerunning without any changes will work.

The style for the plots is set with Style.cpp -- shouldn't need to touch this unless the formatting is off.

## Step 8) Change ntuple format for the fit
Go to analysis/fits. More specific instructions in the README there.

In short: 
Convert ntuples from FLASHgg format to format for fits by using fitterFormatting.cc 
Called by: `./formatNtupleForFitting.sh`

Choose which selection to apply by specifying the values of category: 0 to 4 are currently allowed.
Additional categories can be applied by adding the selection in fitterFormatting.cc.

-----------------------------------------------------------
# Copy the Framework from Github
-----------------------------------------------------------
```
cmsrel CMSSW_8_0_26_patch1
cmsenv 

cd ${CMSSW_BASE}/src
git cms-init

# clone flashgg 
cd ${CMSSW_BASE}/src
git clone https://github.com/cms-analysis/flashgg.git
cd flashgg

cd ${CMSSW_BASE}/src
bash flashgg/setup.sh | tee setup.log

```

Then customize flashgg:

1) flashgg/MicroAOD/test/microAODstd.py 
=> just add these three lines:
from flashgg.MicroAOD.flashggDiPhotons_cfi import flashggDiPhotons
process.flashggDiPhotons0vtx = flashggDiPhotons.clone()
process.flashggDiPhotons0vtx.VertexSelectorName = "FlashggZerothVertexSelector"

2) flashgg/MicroAOD/plugins/ZerothVertexSelector.cc
=> to set the vertex you want (i.e.:0)

3) flashgg/interface/LeptonSelection.hh
=> replace with analysis/addfiles/LeptonSelection.hh

4) flashgg/src/LeptonSelection.cc
=> replace with analysis/addfiles/LeptonSelection.cc

Get egamma updates for smear/scale corr:
From https://twiki.cern.ch/twiki/bin/viewauth/CMS/EGMRegression
```
git cms-merge-topic cms-egamma:EGM_gain_v1 
```

There will be merge conflicts:
- EgammaAnalysis/ElectronTools/src/PhotonEnergyCalibratorRun2.cc
- EgammaAnalysis/ElectronTools/src/EnergyScaleCorrection_class.cc
- EgammaAnalysis/ElectronTools/src/ElectronEnergyCalibratorRun2.cc
- EgammaAnalysis/ElectronTools/plugins/CalibratedPhotonProducersRun2.cc
- EgammaAnalysis/ElectronTools/plugins/CalibratedElectronProducersRun2.cc
- EgammaAnalysis/ElectronTools/interface/PhotonEnergyCalibratorRun2.h
- EgammaAnalysis/ElectronTools/interface/EnergyScaleCorrection_class.h
- EgammaAnalysis/ElectronTools/interface/ElectronEnergyCalibratorRun2.h

The conflicts have been resolved, so replace the default verions with the corresponding files stored in analysis/addFiles.

```
# clone this repository
cd ${CMSSW_BASE}/src
git clone git@github.com:mez34/MonoHiggsToGG.git

# add Math package
git cms-addpkg DataFormats/Math

# now compile everything
cd ${CMSSW_BASE}/src
scram b -j 16
```

###MicroAOD file to test the dumper:
root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshgg/ferriff/flashgg/RunIIFall15DR76-1_3_0-25ns_ext1/1_3_1/GluGluHToGG_M-125_13TeV_powheg_pythia8/RunIIFall15DR76-1_3_0-25ns_ext1-1_3_1-v0-RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160130_032602/0000/myMicroAODOutputFile_1.root
=======
# MonoHiggsToGG
>>>>>>> 40f2401495fdf21dea6bdb22735a01b6bb516bd7
