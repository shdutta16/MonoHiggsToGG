#! /bin/sh

#-----------------------------------------------------------------
# Runs all scripts to do combination 
#----------------------------------------------------------------- 
model=$1
mass=$2

#--- Setup directories
ggindir="../../gg_${model}/"
ttindir="../../tt_${model}/"

#--- Setup the range to run over
if [[ $model == "2HDM" ]]; then
  vals_mA=($(seq 400 25 700))
  vals_mZ=($(seq 600 50 2000))
fi
if [[ $model == "BARY" ]]; then
  vals_mA=($(seq 0 25 900))
  vals_mZ=($(seq 0 50 2500))
fi
#echo ${vals_mA[@]}
#echo ${vals_mZ[@]}

#--- Loop over each card
for mA in $mass; do 
#for mA in ${vals_mA[@]}; do
for mZ in ${vals_mZ[@]}; do 
  if [[ $model == "BARY" ]]; then
    ggcard="datacard_ntuples4fit_pho_allMC_woLepVetos_met50_met130_cic_default_shapes_lumi_35.9_MonoHgg_sig_ZpBaryonic_mZP${mZ}_mChi${mA}_13TeV.txt"
  fi
  if [[ $model == "2HDM" ]]; then
    #ggcard="datacard_ntuples4fit_pho_met0_met130_cic_default_shapes_lumi_35.9_MonoHgg_sig_2HDM_mZP${mZ}_mA0${mA}_13TeV.txt"  # orig grid
    ggcard="dataCard_sig_2HDM_mZP${mZ}_mA0${mA}_13TeV.txt"                                                                    # parametrized cards
  fi

  ttcard="Zprime${mZ}A${mA}/cmb/${mA}/combined.txt.cmb"
  cbcard="dataCard_comb_2HDM_mZP${mZ}_mA${mA}" 

  if [ ! -f ${ggindir}${ggcard} ]; then 
    echo "gg card ($mZ,$mA) does NOT exist!"
    echo "gg card ($mZ,$mA) does NOT exist!"  >> "MissingCards.txt"
  elif [ ! -f ${ttindir}${ttcard} ]; then 
    echo "tt card ($mZ,$mA) does NOT exist!"
    echo "tt card ($mZ,$mA) does NOT exist!"  >> "MissingCards.txt"
  else       
    #--- combine cards
    combineCards.py ${ggindir}${ggcard} ${ttindir}${ttcard} > "${cbcard}.txt"

    #--- replace mass var in datacard
    python replaceZpNames.py -f "${cbcard}.txt" -m ${mA}

    #--- call combine
    python combineTool.py -M Asymptotic -m ${mZ} -d "${cbcard}_new.txt" --there -n .limit --parallel 18     # use combineHarvester
    ##combine -M Asymptotic -m ${mZ} "${cbcard}_new.txt"                                                    # use standard combine

    echo "Finishd $mZ , $mA " >> "Finished.txt"

  fi

done # end mZ loop

mkdir "results_${mA}"
mv higgsCombine.limit.Asymptotic.mH*.root "results_${mA}/"

#--- hadd files
hadd higgsCombine.Asymptotic.${model}_mA${mA}.root results_${mA}/higgsCombine.limit.Asymptotic.mH*.root 

done # end mA (or mDM) loop

