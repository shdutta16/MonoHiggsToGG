#!/bin/bash

# set -x

folder=$1 && shift
coupl=$1 && shift
# echo $coupl

cd $folder

args=""
masses=""
sub=""
while [[ -n $1 ]]; do
    case $1 in
	-M)
	    method=$2
	    args="$args $1 $2"
	    shift
	    ;;
	-m)
	    # masses="^$(echo $2 | sed  's%,%$|^%g')\$"
	    masses="$2"
	    shift
	    ;;
	--parametric)
	    isparametric="1"
	    ;;
	-n)
	    label=$2
	    shift
	    ;;
	-C)
	    percent=$2
	    shift
	    ;;
	--model)
	    model=$2
	    shift
	    ;;
	--mass-in-label)
	    mass_in_label="1"
	    ;;
	-s)
	    seed=$2
	    args="$args $1 $2"
	    shift
	    ;;
	--output)
	    outfolder="$2"
	    shift
	    ;;
	--hadd)
	    hadd="hadd"
	    ;;
	--dry-run)
	    dry="1"
	    ;;
	--cont)
	    cont="1"
	    ;;
	--sub)
	    sub="$2"
	    shift
	    ;;
	*)	    
	    args="$args $1"
	    ;;	    
    esac
    shift
done
shift

if [[ -n $masses ]]; then
    if [[ -n $isparametric ]]; then
	masses="$(echo $masses | sed  's%,% %g')"
    else
    	masses="^$(echo $masses | sed  's%,%$|^%g')\$"
    fi
fi

if [[ -n $isparametric ]] && [[ -z $masses ]]; then
    echo "workspace is parametric but no list of masses specified... baliing out"
    exit -1
fi


echo $masses
echo $outfolder

if [[ -n $outfolder ]] && [[ ! -d $outfolder ]]; then
    mkdir -p $outfolder
fi

if [[ -n $sub ]]; then
    cat > env.sh <<EOF
export PATH=$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
EOF
fi

libs="-L libdiphotonsUtils"
rootversion=$(root-config --version| tr '.' ' ')
##[[ $rootversion -gt 5 ]] && libs="-L libdiphotonsRooUtils"

label="MonoHgg_sig_"${model}
m1name="mChi"
if [[ $model == *"2HDMa"* ]]; then 
    #m1name="mA0"
    m1name="ma"
fi 

outputs=""

# m1 = mDM or mA
# m2 = mZp
for m1 in 150; do
  for m2 in {200,300,400,500,600}; do
    card=dataCard_sig_${model}_mA${m2}_${m1name}${m1}_13TeV.txt
    binary=$(echo $card | sed 's%.txt$%.root%')
    signame=$(echo $card | sed 's%.*MonoHgg_%%; s%.txt%%')
    echo $signame

    namestr=${label}_mA${m2}_${m1name}${m1}

    if [[ -n $seed ]]; then
        filename=higgsCombine${namestr}.${method}.mH${m2}.${seed}.root
    else
	filename=higgsCombine${namestr}.${method}.mH${m2}.root
    fi
    
    log=combine_log_${method}_${label}_mA${mass}.log
    set -x
    if [[ -z $dry ]] && ( [[ -z $cont ]] || [[ ! -f $filename ]] ); then 
	if [[ -f $binary ]] && [[ $binary -nt $card ]]; then
	    card=$binary
	fi
	if [[ -z $sub ]]; then 
	    echo combine $libs $args -n $label -m $m2 -C $percent $card > $log
	    combine $libs $args -n ${namestr} -m $m2 -C $percent $card 2>&1 | tee -a $log
	else
	    rm $log
	    bsub -o $log -q $sub run.sh -env $PWD/env.sh -copy $filename -outdir $PWD  -- combine $libs $args -n $label -m $m2 -C $percent $PWD/$card
	fi    
	
    elif [[ -n $outfolder ]]; then
	filename=$outfolder/$filename
    fi
    #[[ -f $filename ]] && 
    outputs="$outputs $filename"
    set +x
    tail -5 $log 
  
  done

  if [[ -n $seed ]]; then 
      hout=higgsCombine${label}Combined.${method}.${model}_${m1name}${m1}.${seed}.root
  else
      hout=higgsCombine${label}Combined.${method}.${model}_${m1name}${m1}.root
  fi
   
  hadd -f $hout $outputs 
 
  if [[ -n $hadd ]]; then
     hadd -f $hout $outputs
     outputs="$hout $outputs"
  fi
  if [[ -n $outfolder ]]; then
      cp -p $outputs $outfolder
  fi
 
  outputs=""
 
done

