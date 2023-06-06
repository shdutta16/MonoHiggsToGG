#!/bin/bash

#echo
#echo "===================================================================================================="
#echo '$1 = ' $1
#echo '$2 = ' $2
#echo '$3 = ' $3
#echo '$4 = ' $4
#echo "===================================================================================================="
#echo

## version=full_analysis_anv1_v19
www=$PWD
version=$1 && shift
templatesMakerMonoHgg=$1 && shift
templatesMakerPrepareMonoHgg=$1 && shift
medMass=$1 && shift

echo
echo "===================================================================================================="
echo 'www = ' $www
echo 'version = ' $version
echo 'templatesMakerMonoHgg = ' $templatesMakerMonoHgg
echo 'templatesMakerPrepareMonoHgg = ' $templatesMakerPrepareMonoHgg
echo 'medMass = ' $medMass
echo "===================================================================================================="
echo

fitname=2D 
#www=/eos/user/d/dbhowmik/www/Analysis/Fit_DiPhotonTools/2017_2HDMa_EOY/LowMET
#www=/afs/cern.ch/work/s/shdutta/public/Analysis/MHgg/2018Analysis/Fit_DiPhotonTools/CMSSW_9_4_9/src/diphotons/Analysis/macros/2018_2HDMa_EOY/MET_DNN
#www=/afs/cern.ch/work/s/shdutta/public/Analysis/MHgg/2018Analysis/Fit_DiPhotonTools/CMSSW_9_4_9/src/diphotons/Analysis/macros/2018_2HDMa_EOY_reloaded/DNN
#www=/afs/cern.ch/work/s/shdutta/public/Analysis/MHgg/2018Analysis/Fit_DiPhotonTools/CMSSW_9_4_9/src/diphotons/Analysis/macros/2018_2HDMa_EOY_reloaded/MET_paraDNN
#www=/afs/cern.ch/work/s/shdutta/public/Analysis/MHgg/2018Analysis/Fit_DiPhotonTools/CMSSW_9_4_9/src/diphotons/Analysis/macros/2018_2HDMa_EOY_reloaded/DNN_DNNbins_TEST


shapes="default_shapes"
default_model=""

opts=""
input_opts=""
data_version=""
prepare=""
while [[ -n $1 ]]; do
    case $1 in
	--fit-name)
	    fitname=$2
	    shift
	    ;;
	--www)
	    www=$2
	    shift
	    ;;
	--verbose)
	    verbose="--verbose"
	    opts="$opts --verbose"
	    ;;
	--prepare-*)
	    prepare="$prepare $1"
	    ;;
	--redo-input)
	    rerun="1"
	    ;;
	--label)
	    addlabel=$2
	    shift
	    ;;
	--use-templates)
	    templates="semiparam"
	    opts="$opts $1"
	    ;;
        --mix-templates)
            mix="--mix-templates"
            ;;
	--bkg-shapes)
	    shapes=$(echo $(basename $2 | sed 's%.json%%'))
	    opts="$opts $1 $2"
	    shift
	    ;;
	--default-model)
	    default_model=$2
	    opts="$opts $1 $2"
	    shift
	    ;;
	--use-templates)
	    templates="use_templates"
	    opts="$opts $1"
	    ;;
	--mix-templates)
	    mix="--mix-templates"
	    ;;
	--nuisance-fractions-covariance)
	    covariance=$(echo $(basename $2 | sed 's%.json%%'))
	    opts="$opts $1"
	    ;;
	--generate-ws-bkgnbias)
	    spurious="bias";
	    opts="$opts $1"
	    ;;
	--fwhm-input-file)
	    fwhm="$2"
	    opts="$opts $1"
	    ;;
	--only-coup*)
	    log_label="${log_label}$(echo $2 | tr ',' '_')"
	    opts="$opts $1 $2"
	    shift
	    ;;
	--lumi*)
	    lumi=$2
	    shift
	    ;;
	--data-file)
	    input_opts="$input_opts $1 $2"
	    data_version="$(basename $(dirname $2))"
	    shift
	    ;;
	--*-file)
	    input_opts="$input_opts $1 $2"
	    shift
	    ;;
	--fit-background)
	    just_fit_bkg="1"
	    ;;
	--load)
	    load_also="$load_also $1 $2"
	    opts="$opts $1 $2"
	    echo "in options load_also = "$load_also
	    shift
	    ;;
	*)
	    opts="$opts $1"
	    ;;	    
    esac
    shift
done
shift

echo "reached here"
echo $version $lumi

if [[ -z $version ]] || [[ -z $lumi ]]; then
    echo "usage: $0 <analysis_version> --lumi <lumi> [run_options]"
    exit 0
fi

input_folder=$version

[[ -n $data_version ]] && version=$data_version

echo "data_version=" $data_version

label="$shapes"
[[ -n $default_model ]] && label="${label}_${default_model}"
[[ -n $covariance ]] && label="${label}_${covariance}"
[[ -n $templates ]] && label="${label}_${templates}"
[[ -n $bias ]] && label="${label}_${bias}"
[[ -n $templates ]] && label="${label}_${templates}"
[[ -n $addlabel ]] && label="${label}_${addlabel}"

input=${version}_${fitname}_${medMass}_final_ws_MonoHgg.root
input_log=${version}_${fitname}_${medMass}_final_ws_MonoHgg.log
treesdir=test
## ls $treesdir/$version
# ls $treesdir/$input_folder
if [[ ! -d $treesdir/$input_folder ]]; then
    echo "Found testdir"
fi
[[ ! -d $treesdir/$input_folder ]] && treesdir=$PWD
workdir=${version}_${fitname}_${label}_lumi_${lumi}

if [[ -n $bias ]]; then
    if [[ -z $fwhm ]]; then
	opts="--compute-fwhm"
    fi
fi
if [[ ! -d $workdir ]]; then
    mkdir $workdir
fi
if [[ ! -d $www/$version ]]; then
    mkdir $www/$version
fi

# set -x
if [[ -n $rerun  ]] || [[ ! -f $input ]]; then
    echo "**************************************************************************************************************************"
    echo "creating $input"
    echo "**************************************************************************************************************************"
    subset=$fitname
    if [[ "$fitname" == "2D" ]]; then
        subset="2D,singlePho"
        mix="--mix-templates"
    fi
    echo "I AM HERE"
    echo "subset = "$subset " mix = "$mix " load_also = "$load_also "input_opts = " $input_opts
    echo
    set -x
    ./templates_maker_MonoHgg.py --load $templatesMakerMonoHgg,$templatesMakerPrepareMonoHgg $load_also --only-subset $subset $mix --input-dir $treesdir/$input_folder $prepare -o $input $verbose $input_opts 2>&1 | tee $input_log
    set +x
    echo "Ran ./templates_maker_MonoHgg.py --load $templatesMakerMonoHgg,$templatesMakerPrepareMonoHgg $load_also --only-subset $subset $mix --input-dir $treesdir/$input_folder $prepare -o $input $verbose $input_opts 2>&1 | tee $input_log"
    echo "**************************************************************************************************************************"
elif [[ -n $mix ]]; then
    echo "**************************************************************************************************************************"
    echo "running event mixing"
    echo "**************************************************************************************************************************"    
    set -x
    ./templates_maker_MonoHgg.py --load $templatesMakerPrepareMonoHgg $load_also --read-ws $input $mix $verbose 2>&1 | tee mix_$input_log
    set +x
    echo "**************************************************************************************************************************"
fi
	    

echo "**************************************************************************************************************************"
echo "running model creation"
echo "**************************************************************************************************************************"

if [[ -z $just_fit_bkg ]]; then
    ##--binned-data-in-datacard \
    set -x
    ./combine_maker_MonoHgg.py $medMass \
	--fit-name $fitname  --luminosity $lumi  --lumi $lumi \
	--fit-background \
	--generate-signal \
        --generate-Higgs-dataset \
	--generate-datacard \
	--read-ws $input \
	--ws-dir $workdir \
        -O $www/$version/$workdir_$medMass \
        -o ${workdir}_${medMass}_MonoHgg.root  \
        --cardname datacard_${workdir}_${medMass}_MonoHgg.txt $opts 2>&1 | tee $workdir/combine_maker${log_label}_${medMass}_MonoHgg.log
    set +x
else
    set -x
    ./combine_maker_MonoHgg.py $medMass \
	--fit-name $fitname  --luminosity $lumi  --lumi $lumi \
	--fit-background \
        --generate-signal \
        --generate-Higgs-dataset \
        --generate-datacard \
	--read-ws $input \
	--ws-dir $workdir \
        -O $www/$version/$workdir_$medMass \
        -o ${workdir}_${medMass}_MonoHgg.root  \
        --cardname datacard_${workdir}_${medMass}_MonoHgg.txt \
        $opts 2>&1 | tee $workdir/combine_maker_bkg_only${log_label}_${medMass}_MonoHgg.log
    set +x
fi

echo "**************************************************************************************************************************"
