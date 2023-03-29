#
# usage: %prog [opts] --cfg cmssw.py dataset doPUreweighting(0/1) sampleIndex PUweightsFile x-section(in pb) kFactor
#
# Backgrounds: sampleID>0 && sampleID<100
# Signals:     sampleID>100
# Data:        sampleID=0


## 25ns samples from FLASHgg
#./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py DoubleEG         		0 10001 pippo 1 1

./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py DiPhoton			1 15  /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 84.0     1 
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py GJet_Pt-20to40     		1  1  /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 218.6108 1 
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py GJet_Pt-40toInf    		1  2  /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 863.1088 1 
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py QCD_Pt-30to40      		1  3  /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 24300    1 
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py QCD_Pt-30toInf     		1  4  /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 259296   1 
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py QCD_Pt-40toInf     		1  5  /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 108240   1 
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py GluGluHToGG	     		1 10  /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 0.087842 1 
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py VH				1 11  /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 0.004499 1 
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py DYJetsToLL			1 12  /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 6025.2   1 
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py ttHJetToGG			1 21  /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 0.00107  1  
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py VBFHToGG			1 22  /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 0.007496 1 
                                                                                                                                                          
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py privMC_2HDM_MZP600		1 100 /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 0.001 1 #0.00009338  1 #46.69fb xsec * 0.002br
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py privMC_2HDM_MZP800		1 101 /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 0.001 1 #0.00010348  1 #51.74fb xsec * 0.002br
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py privMC_2HDM_MZP1000		1 102 /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 0.001 1 #0.00008394  1 #41.97fb xsec * 0.002br
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py privMC_2HDM_MZP1200		1 103 /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 0.001 1 #0.00006352  1 #31.76fb xsec * 0.002br
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py privMC_2HDM_MZP1400		1 104 /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 0.001 1 #0.00004712  1 #23.56fb xsec * 0.002br
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py privMC_2HDM_MZP1700		1 105 /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 0.001 1 #0.00003020  1 #15.10fb xsec * 0.002br
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py privMC_2HDM_MZP2000		1 106 /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 0.001 1 #0.000019468 1 #9.734fb xsec * 0.002br
./submitBatchDiPho.py --cfg diPhoAnaBATCH_MVA.py privMC_2HDM_MZP2500		1 107 /afs/cern.ch/user/m/mzientek/public/pileupWeights_fullReReco74X.root 0.001 1 #0.00000972  1 #4.860fb xsec * 0.002br




#
##./submitBatchDiPho.py --cfg diPhoAnaBATCH.py testSig				0  110 pippo 0.00009338  1 
##
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py 2HDM_MZP600			1 100 PURW_MC.root 0.00009338  1 #46.69fb xsec * 0.002br
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py 2HDM_MZP800			1 101 PURW_MC.root 0.00010348  1 #51.74fb xsec * 0.002br
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py 2HDM_MZP1000			1 102 PURW_MC.root 0.00008394  1 #41.97fb xsec * 0.002br
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py 2HDM_MZP1200			1 103 PURW_MC.root 0.00006352  1 #31.76fb xsec * 0.002br
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py 2HDM_MZP1400			1 104 PURW_MC.root 0.00004712  1 #23.56fb xsec * 0.002br
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py 2HDM_MZP1700			0  105 pippo 1 1 #0.00003020  1 #15.10fb xsec * 0.002br
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py 2HDM_MZP2000			0  106 pippo 1 1 #0.000019468 1 #9.734fb xsec * 0.002br
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py 2HDM_MZP2500			0  107 pippo 1 1 #0.00000972  1 #4.860fb xsec * 0.002br




# 50ns Data
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py DoubleEG           		0 10000 pippo  1 1
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py EGamma             		0 10001 pippo  1 1
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py SinglePhoton       		0 10002 pippo  1 1

#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py ZH					0 11  pippo 0.8696 1 
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py WminusH				0 12  pippo 1.38 1
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py WplusH				0 13  pippo 1.38 1 
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Higgs_scalar_nohdecay_gg_1000GeV	0 100 pippo 0.01 1 #10fb xsec
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Higgs_scalar_nohdecay_gg_100GeV	0 101 pippo 0.01 1
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Higgs_scalar_nohdecay_gg_10GeV	0 102 pippo 0.01 1
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Higgs_scalar_nohdecay_gg_1GeV	0 103 pippo 0.01 1
 



# GG+jets
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GGJets_M-1000To2000  0 1 pippo 0.0104901    1  
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GGJets_M-2000To4000  0 2 pippo 0.000439813  1  
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GGJets_M-200To500    0 3 pippo 2.433823     1 
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GGJets_M-4000To8000  0 4 pippo 2.19697e-06  1 
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GGJets_M-500To1000   0 5 pippo 0.172872     1 
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GGJets_M-8000To13000 0 6 pippo 7.05314e-11  1 

# GJets
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GJets_HT-100to200  0  7 pippo 1534.  1  
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GJets_HT-200to400  0  8 pippo 490.0  1  
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GJets_HT-400to600  0  9 pippo 62.05  1  
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py GJets_HT-600toInf  0 10 pippo 20.87  1  


# QCD
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py QCD_HT-100To250  0 11 pippo 28730000.0 1
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py QCD_HT-250To500  0 12 pippo 670500.0   1
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py QCD_HT-500To1000 0 13 pippo 26740.0    1
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py QCD_HT-1000ToInf 0 14 pippo 769.7      1  

# Graviton signal
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py RSGravToGG_kMpl-001_M-1500  0 101 pippo 0.001095  1  
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py RSGravToGG_kMpl-001_M-750   0 102 pippo 0.04471   1  
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py RSGravToGG_kMpl-01_M-1500   0 103 pippo 0.1086    1  
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py RSGravToGG_kMpl-01_M-3000   0 104 pippo 0.001229  1  
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py RSGravToGG_kMpl-02_M-1500   0 105 pippo 0.4202    1  
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py RSGravToGG_kMpl-02_M-3000   0 106 pippo 0.0045    1  
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py RSGravToGG_kMpl-02_M-5000   0 107 pippo 5.01 e-05 1  
#./submitBatchDiPho.py --cfg diPhoAnaBATCH.py RSGravToGG_kMpl-001_M-5000  0 108 pippo 1.202e-07 1  .
