#! /bin/sh
#run extractJSONS.py for all samples in file

# FLASHgg 2017 version 92X (2_7_5 = test)
python extractJSONS.py -i datasets_2017/datasets_test0.json	-o DYJetsToLL							-d lists_2017_v0/MC
python extractJSONS.py -i datasets_2017/datasets_test6.json	-o GJet								-d lists_2017_v0/MC
python extractJSONS.py -i datasets_2017/datasets_test0.json	-o DoubleEG -e Run2017C-PromptReco-v1	-s DoubleEG_Run2017C_v1	-d lists_2017_v0/Data
python extractJSONS.py -i datasets_2017/datasets_test2.json	-o DoubleEG -e Run2017A-PromptReco-v2	-s DoubleEG_Run2017A_v2	-d lists_2017_v0/Data
python extractJSONS.py -i datasets_2017/datasets_test2.json	-o DoubleEG -e Run2017A-PromptReco-v3	-s DoubleEG_Run2017A_v3	-d lists_2017_v0/Data
python extractJSONS.py -i datasets_2017/datasets_test2.json	-o DoubleEG -e Run2017B-PromptReco-v2	-s DoubleEG_Run2017B_v2	-d lists_2017_v0/Data
python extractJSONS.py -i datasets_2017/datasets_test7.json	-o DoubleEG -e Run2017E-PromptReco-v1	-s DoubleEG_Run2017E_v1	-d lists_2017_v0/Data 
python extractJSONS.py -i datasets_2017/datasets_test8.json	-o DoubleEG -e Run2017F-PromptReco-v1	-s DoubleEG_Run2017F_v1	-d lists_2017_v0/Data 
python extractJSONS.py -i datasets_2017/datasets_test9.json	-o DoubleEG -e Run2017B-PromptReco-v1	-s DoubleEG_Run2017B_v1	-d lists_2017_v0/Data 
python extractJSONS.py -i datasets_2017/datasets_test10.json	-o DoubleEG -e Run2017D-PromptReco-v1	-s DoubleEG_Run2017D_v1	-d lists_2017_v0/Data 
python extractJSONS.py -i datasets_2017/datasets_test11.json	-o DoubleEG -e Run2017C-PromptReco-v3	-s DoubleEG_Run2017C_v3	-d lists_2017_v0/Data 
python extractJSONS.py -i datasets_2017/datasets_test12.json	-o DoubleEG -e Run2017C-PromptReco-v2	-s DoubleEG_Run2017C_v2	-d lists_2017_v0/Data 
