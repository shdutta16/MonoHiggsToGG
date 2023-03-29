#! /bin/sh

#-----------------------------------------------------------------
# Runs all scripts to do combination 
#----------------------------------------------------------------- 
model="2HDM"

ggindir="gg_${model}_param_raw/"
ggoutdir="gg_${model}_param_clean/"

#----------------------------------------------------------------- 
# Copy gg workspaces to ggoutdir if haven't done this yet 
#----------------------------------------------------------------- 
# cp ${ggindir}ttH* ${ggoutdir}
# cp ${ggindir}Glu* ${ggoutdir}
# cp ${ggindir}V*   ${ggoutdir}
# cp ${ggindir}ntu* ${ggoutdir}
#----------------------------------------------------------------- 

if [[ $model == "2HDM" ]]; then
  vals_mA=($(seq 300 25 700))
  vals_mZ=($(seq 600 50 2000))
fi
if [[ $model == "BARY" ]]; then
  vals_mA=($(seq 0 25 900))
  vals_mZ=($(seq 0 50 2500))
fi
#echo ${vals_mA[@]}
#echo ${vals_mZ[@]}

for mA in ${vals_mA[@]}; do
for mZ in ${vals_mZ[@]}; do 

  #echo "Running for ${model} : mDM = ${mA} , mZ = ${mZ}"

  if [[ $model == "BARY" ]]; then
    ggcard="datacard_ntuples4fit_pho_allMC_woLepVetos_met50_met130_cic_default_shapes_lumi_35.9_MonoHgg_sig_2HDM_mZP${mZ}_mA0${mA}_13TeV.txt"
  fi
  if [[ $model == "2HDM" ]]; then
    #ggcard="datacard_ntuples4fit_pho_met0_met130_cic_default_shapes_lumi_35.9_MonoHgg_sig_2HDM_mZP${mZ}_mA0${mA}_13TeV.txt"  # orig grid
    ggcard="dataCard_sig_2HDM_mZP${mZ}_mA0${mA}_13TeV.txt"                                                                    # parametrized cards
  fi

  if [ ! -f ${ggindir}${ggcard} ]; then
    echo "File ($mA,$mZ) NOT found"
  fi 

  #--- clean gg cards (workspaces were manually copied*)
  python cleanDataCards.py -i ${ggindir} -f ${ggcard} -o ${ggoutdir}

done # end mZ loop
done # end mA (or mDM) loop
