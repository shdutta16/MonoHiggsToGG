#! /bin/sh

model=$1
mass=$2
dir="combo_${model}_m${mass}"

mkdir $dir
cp combineCards.sh   "${dir}/"
cp combineTool.py    "${dir}/"
cp replaceZpNames.py "${dir}/"

cd $dir
./combineCards.sh $model $mass
