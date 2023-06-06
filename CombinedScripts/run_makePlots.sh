#run_makePlots.sh

inDir=$1 && shift
mainDir=$1 && shift

cd $mainDir

./combineCards.sh ../$inDir

cd ../

set -x
./mycombineall_MonoHgg2HDMa.sh "$mainDir" --hadd --model 2HDMa -C 0.95 -M AsymptoticLimits --run both
./mylimit_plots_MonoHgg.sh "$mainDir"
rm -f *~
set +x










