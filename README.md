# MonoHiggsToGG

Scripts for optimized bin calculation and categorization of events for limit calculation. 

Instructions:

1. Pull the repo: `git pull MonoHiggsToGG optimizedBinScripts`
2. Copy the contents of the directory `BinwiseScripts` to the directory where bin-wise limit calculation will be done.
3. Copy the contents of the directory `CombinedScripts` to the directory where the combine calculation (combining the limits of all bins or categories) will be done. 
4. If the calculation is being done for simple DNN (parametric DNN) then keep the file `run_fitterFormatting_DNNcat_array_DNN.sh` (`run_fitterFormatting_DNNcat_array_paraDNN.sh`) and delete the other.
5. In `run_fitterFormatting_DNNcat_array_DNN.sh`, enter the bin edges corresponding to each mass point as present in the script from an earlier run and also set the in-directory where the skimmed flat ntuples are present.
6. Do `./run_fitterFormatting_DNNcat_array_DNN.sh` to create the ntuples with DNN-score categorization.
7. In `jsonMaker.py` and `scriptMaker.py` too, enter the bin edges as already present there from an earlier run and also set the out-directory. The python scripts use the json templates `templates_maker_MonoHgg_DNN_original.json` and `templates_maker_prepare_MonoHgg_dnnBins_original.json` to make the json files. Note that when entering the bin edge values as 0 and 1, DO NOT put decimal as in 0.0 or 1.0
8. Move the contents of the out-directory (created in the previous step) to the current directory. Make all the bash scripts executable by doing `chmod`. 
9. Execute:
   ```
   cmsenv
   ./run_combineMakerMonoHgg.sh <directory created in steps 5-6>
   ./run_myCombineAllMonoHiggs2HDMa.sh
   ./run_limitPlotsMonoHgg.sh
   ```
