# LimitPlots
Scripts to produce limit plots after from skimmed trees.


## General description of the files
<This section will be updated later>


## Steps to run:
The instruction are taken from the [link](https://github.com/DebabrataBhowmik/Analysis_Instructions_forShubham/blob/main/README.md).
Please, do `cmsenv` to set the environment before running the commands. I have not included this as a step in the following. 

1. Copy the directory with the trees that have the events categorized in DNN score bins (created by running `formatNtupleForFitting_DNNcat_array.sh` script) to the location where the `LimitPlots` scripts are kept. For me the location is `/afs/cern.ch/work/s/shdutta/public/Analysis/MHgg/2018Analysis/Fit_DiPhotonTools/CMSSW_9_4_9/src/diphotons/Analysis/macros/2018_2HDMa_EOY_reloaded/DNN/`. Since, this is a public location you can too check things from here. 

2. Make a copy of the directory (that contains the skimmed tree) with an appropriate name corresponding to the DNN score bin for which limit plot is being calculated. For example, I have given the name `ntuples4fit_pho_newSig_test_dnnBins_0.9_1.0` for bin 0.9-1.0 

3. Open the `templates_maker_MonoHgg.json` file and change and search for `dnnBin`. There update the DNN score bin you want the script to run on. Please, note that the branch names have not been formatted properly, so you need to give all the 0s (up to six decimal places) as shown below:
    ```
    "groups": {
        "dnnBin_0p900000_1p000000"    : ["dnnBin_0p900000_1p000000"],
        "dnnBin_1p000000_99"    : ["dnnBin_1p000000_99"]
        }, 
    ```

4. Repeat step 2 for `templates_maker_prepare_MonoHgg.json` file as well. 

5. Open the `combine_maker_MonoHgg.sh` file and set `www` to the current working directory.

6. Run 
   ```
   ./combine_maker_MonoHgg.sh "ntuples4fit_pho_newSig_test_dnnBins_0.9_1.0" --lumi 59.69 --fit-name cic --mc-file Output_MC.root --fit-background --redo-input
   ```
   This will create a directory with the tag `_cic_default_shapes_lumi_59.69` where all the trees created by the script are stored. 
   
7. Run 
   ```
   ./mycombineall_MonoHgg2HDMa.sh "ntuples4fit_pho_newSig_test_dnnBins_0.9_1.0_cic_default_shapes_lumi_59.69" --hadd --model 2HDMa -C 0.95 -M AsymptoticLimits --run both
   ```

8. Open `mylimit_plots_MonoHgg.sh` and set the `-O` argument to the current directory.

9. Run 
   ```
   ./mylimit_plots_MonoHgg.sh "ntuples4fit_pho_newSig_test_dnnBins_0.9_1.0_cic_default_shapes_lumi_59.69" "_dnnBin_0p9_1p0"
   ```


Note: I am not entirely sure if these are the sole changes that need to be done in the template maker scripts to produce the datacards. This needs some discussion. When the limit plots were made for >2 MET categorization, an additional step was done where the datacard was splitted into two, corresponding to the two MET bins for which the script was being run. I am yet to figure out, why and how to implement that for the DNN score bins. 
