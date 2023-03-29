#! /bin/sh
#run extractFilesAndWeight.py for all samples in file

## FLASHgg 2017 verison 92X (2_7_5 = test)
python extractFilesAndWeight.py -i lists_2017_v0/MC/DYJetsToLL.json		-o DYJetsToLL 		-d lists_2017_v0
python extractFilesAndWeight.py -i lists_2017_v0/MC/GJet.json			-o GJet 		-d lists_2017_v0
python extractFilesAndWeight.py -i lists_2017_v0/Data/DoubleEG_Run2017C_v1.json	-o Run2017B -s v1	-d lists_2017_v0
python extractFilesAndWeight.py -i lists_2017_v0/Data/DoubleEG_Run2017A_v2.json -o Run2017A -s v2	-d lists_2017_v0
python extractFilesAndWeight.py -i lists_2017_v0/Data/DoubleEG_Run2017A_v3.json	-o Run2017A -s v3	-d lists_2017_v0
python extractFilesAndWeight.py -i lists_2017_v0/Data/DoubleEG_Run2017B_v2.json -o Run2017B -s v2	-d lists_2017_v0
python extractFilesAndWeight.py -i lists_2017_v0/Data/DoubleEG_Run2017E_v1.json	-o Run2017E -s v1	-d lists_2017_v0
python extractFilesAndWeight.py -i lists_2017_v0/Data/DoubleEG_Run2017F_v1.json -o Run2017F -s v1	-d lists_2017_v0
python extractFilesAndWeight.py -i lists_2017_v0/Data/DoubleEG_Run2017B_v1.json	-o Run2017B -s v1	-d lists_2017_v0
python extractFilesAndWeight.py -i lists_2017_v0/Data/DoubleEG_Run2017D_v1.json -o Run2017D -s v1	-d lists_2017_v0
python extractFilesAndWeight.py -i lists_2017_v0/Data/DoubleEG_Run2017C_v3.json	-o Run2017C -s v3	-d lists_2017_v0
python extractFilesAndWeight.py -i lists_2017_v0/Data/DoubleEG_Run2017C_v2.json -o Run2017C -s v2	-d lists_2017_v0
