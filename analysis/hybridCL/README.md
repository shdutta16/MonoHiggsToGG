
--------------------------------------------------------
# Running the Hybrid-New CLs in Combine
-------------------------------------------------------- 

Need to have the high mass diphoton tools installed 
( https://github.com/musella/diphotons ). 
You need to compile with `scram b -j 16` the RooUtils and Utils directories.

## Step 1) Run toys
This is a time consuming step, need to run as jobs. 

Test locally with one job: 
```combine datacardM800.txt -m 800 -M HybridNew --freq -s 123 --saveToys --saveHybridResult```

To run on the batch queue calls combine.py in the datacard directory.
Can see an example of how to submit in `submitcombine.sh`. This submission has format:

```python combine.py -i inputdatacard.txt -d outputdir -q 1nh -M HybridNew --onews --begin mZp --end mZp -t ntoys``` 

Then `hadd` the output root files. (This can easily be achieved with the bash script `runhadd.sh`.)

## Step 2) Get the quantiles

Get the 2.5%, 16%, 50%, 84%, 97.5% quantiles for the grid preduced from the toys.

Can do this locally, for example with command:
```combine datacardM800.txt -m 800 -M HybridNew --freq --grid=higgsCombineTest.HybridNew.mH800.123.root --expectedFromGrid=0.5```

Batch script that does this for each mass point and each quantile is in `getquantiles.sh`. 

To find the observed number of events use remove the `--expectedFromGrid` option.
Do this in a different directory as the input grid file, otherwise the output will overwrite the file.  

Then `hadd` the output root files. (This can easily be done with the bash script `haddquantiles.sh` .)

## Step 3) Make limit plots

Run `limitplotter.py`, called by `limitplotter.sh`. 

This also uses as input the root files for the theoretical cross-sections (`gz08_fbU.root` and `Signal_crossSections.root`). 

## Step 4) Make 2D limit plots

Run macro `make2Dlimitplots.cc` called by:
```root -l -b -q make2Dlimitplots.cc```
