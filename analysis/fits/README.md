----------------------------------------------------------
# To make fits:
----------------------------------------------------------

## Step 1) 
Convert ntuples from FLASHgg format to format for fits by using fitterFormatting.cc 
This can be called by:  

```
  ./formatNtupleForFitting.sh
```
  
specifying fitterFormatting input arguements:
 fitterFormatting input arguements:
   1st: input directory
   2nd: output directory
   3rd: type (sig, bkg, data)
   4th: prompt (for duplicate removal)
   5th: input filename 
   6th: sample name
   7th: outfile name
   8th: category type (which selection to apply)

 final files used for the fit are:
   `outdir/Output_MC.root` and 
   `outdir/Output_Data.root`

MET categorization is done in a similar manner by fitterFormatting_METcat.cc
MET bins are specified in fitterFormatting_METcat.cc, so to change binning need to change there. 
This is called by: 

```
./formatNtupleForFitting_METcat.sh
```


## Step 2)
Check that the sample shapes don't change in the different PHO & MET bins (this is not working at the moment):

```root -l ProduceWorkspaces.C```

which calls runfits() from `FitTools.cc`. In FitTools, need to specify the number of MET cat (nMetCat) and the number of PHO cat (nPhoCat).
And in the runfits function specify files to run over with format:
`AddSigData(workspace,sample,type)`

where if (type != 0) name = sample.
Or if (type == 1) then script assumes using 2HDM sample, so just put in sample=mZP mass.

## Step 3) 
Run the fitter see directions: https://github.com/lsoffi/MonoHggFits
