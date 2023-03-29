Step 1) Run skim first

Step 2) Train an MVA (can be run in batch with `bsub -q 2nw jobscript.sh`) 
- `python runMVAtest.py --mva {MVA}`

Step 3) Apply MVA to all samples
- `applyMVA.py`

Step 4) Plot MVA (various MVA quanties, input & output variables):
- `python makeMVAPlots.py --mva DNN -i TMVA_DNN_BARY.root -O ~/www/Plots/MonoHgg2017/MVAplots/ --suffix "_BARY"`

Step 5) Make ntuples4fit

TMVA Examples:
- SVN: https://dbaumgartel.wordpress.com/2014/03/14/machine-learning-examples-scikit-learn-versus-tmva-cern-root/
- DNN: http://oproject.org/DeepLearning
