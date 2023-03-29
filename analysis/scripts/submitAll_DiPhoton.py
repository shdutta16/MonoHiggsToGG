pileupfile="/afs/cern.ch/user/m/mzientek/public/pileupWeights_moriond17_v1.root"

# -------------------------------------------------------------------------------

# Scripts to send analyzer to batch jobs

# -------------------------------------------------------------------------------
# -------------------------------------------------------------------------------

# usage: %prog [opts] --cfg cmssw.py dataset doPUreweighting(0/1) sampleIndex PUweightsFile x-section(in pb) kFactor

# Backgrounds: sampleID>0 && sampleID<100
# Signals:     sampleID>100
# Data:        sampleID>10000

##--------------------------------------------------------------------------------
## 2017 Samples
##--------------------------------------------------------------------------------

  ##--------------------------------------------------------------------------------
  ## DATA
  ##--------------------------------------------------------------------------------

./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Run2017A_v2         		0 10001 pippo 1 1
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Run2017A_v3         		0 10002 pippo 1 1
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Run2017B_v1         		0 10003 pippo 1 1
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Run2017B_v2         		0 10004 pippo 1 1
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Run2017C_v2         		0 10005 pippo 1 1
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Run2017C_v3         		0 10006 pippo 1 1
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Run2017D_v1        		0 10007 pippo 1 1
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Run2017E_v1         		0 10008 pippo 1 1
./submitBatchDiPho.py --cfg diPhoAnaBATCH.py Run2017F_v1         		0 10009 pippo 1 1

##--------------------------------------------------------------------------------
## 2016 Samples
##--------------------------------------------------------------------------------

#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Run2016B         		0 10002 pippo 1 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Run2016C         		0 10003 pippo 1 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Run2016D      	  		0 10004 pippo 1 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Run2016E         		0 10005 pippo 1 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Run2016F         		0 10006 pippo 1 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Run2016G         		0 10007 pippo 1 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Run2016H         		0 10008 pippo 1 1

  ##--------------------------------------------------------------------------------
  ## SM HIGGS BKGS (use HLT2 instead of HLT) 
  ##--------------------------------------------------------------------------------

##./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py GluGluHToGG	 		1 10 ${pileupFile}  0.1102766	1 # AMCATNLO
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py GluGluHToGG	 		1 10 ${pileupFile}  0.1001978	1 # POWHEG 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py VBFHToGG				1 11 ${pileupFile}  0.00858514	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py VHToGG				1 12 ${pileupFile}  0.00512339 	1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ttHToGG				1 13 ${pileupFile}  0.001151117	1

  ##--------------------------------------------------------------------------------
  ## NON-RESONANT BKGS 
  ##--------------------------------------------------------------------------------

#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py DYJetsToLL 			1 14 ${pileupFile}  6025.2	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py GJet_Pt-20toInf 	      		1 19 ${pileupFile}  3216.0	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py GJet_Pt-20to40 	      		1 18 ${pileupFile}  220.0	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py GJet_Pt-40toInf			1 20 ${pileupFile}  850.8	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py DiPhotonJetsBox_M40_80		1 16 ${pileupFile}  303.2	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py DiPhotonJetsBox_MGG-80toInf 	1 17 ${pileupFile}  84.0	1 # SHERPA
##./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py DiPhotonJets_MGG-80toInf 	1 17 ${pileupFile}  135.1	1 # AMCATNLO
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py QCD_Pt-30to40 			1 15 ${pileupFile}  22110.0	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py QCD_Pt-40toInf 			1 22 ${pileupFile}  113400.0	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py QCD_Pt-30toInf 			1 21 ${pileupFile}  260500.0	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py TTJets 				1 26 ${pileupFile}  665.0  	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py TTGJets 				1 25 ${pileupFile}  3.819	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py TTGG_0Jets 			1 24 ${pileupFile}  0.01731	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py TGJets 				1 23 ${pileupFile}  2.967	1

#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py WGToLNuG				1 28 ${pileupFile}  489.0	1 # AMCATNLO
##./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py WGToLNuG			1 28 ${pileupFile}  405.271	1 # MADGRAPH
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py WJetsToLNu 			1 29 ${pileupFile}  61526.7 	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py WZTo2L2Q				1 31 ${pileupFile}  5.595 	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZGTo2LG				1 32 ${pileupFile}  117.864	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZGTo2NuG				1 32 ${pileupFile}  28.05 	1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZZTo2L2Q				1 33 ${pileupFile}  3.22	1
##./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py TT_				1 27 ${pileupFile}  xxxxxx 	1
##./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py WW_ 				1 30 ${pileupFile}  xxxxxx 	1

  ##--------------------------------------------------------------------------------
  ## SIGNAL (xsec*BR = 1pb*0.002)
  ##--------------------------------------------------------------------------------

#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-600_MA0-300 			1 140 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-800_MA0-300 			1 141 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1000_MA0-300 			1 142 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1200_MA0-300 			1 143 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1400_MA0-300 			1 144 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1700_MA0-300 			1 145 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-2000_MA0-300 			1 146 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-2500_MA0-300 			1 147 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-600_MA0-400 			1 148 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-800_MA0-400 			1 149 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1000_MA0-400 			1 150 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1200_MA0-400 			1 151 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1400_MA0-400 			1 152 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1700_MA0-400 			1 153 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-2000_MA0-400 			1 154 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-2500_MA0-400 			1 155 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-800_MA0-500 			1 156 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1000_MA0-500 			1 157 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1200_MA0-500 			1 158 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1400_MA0-500 			1 159 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1700_MA0-500 			1 160 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-2000_MA0-500 			1 161 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-2500_MA0-500 			1 162 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-800_MA0-600 			1 139 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1000_MA0-600 			1 163 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1200_MA0-600 			1 164 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1400_MA0-600 			1 165 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1700_MA0-600 			1 166 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-2000_MA0-600 			1 167 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-2500_MA0-600 			1 168 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1000_MA0-700 			1 169 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1200_MA0-700 			1 170 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1400_MA0-700 			1 171 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1700_MA0-700 			1 172 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-2000_MA0-700 			1 173 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-2500_MA0-700 			1 174 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1000_MA0-800 			1 175 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1200_MA0-800 			1 176 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1400_MA0-800 			1 177 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-1700_MA0-800 			1 178 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-2000_MA0-800 			1 179 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py 2HDM_MZp-2500_MA0-800 			1 180 ${pileupFile} 0.002 1 

#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-1000_MChi-1000_13TeV	1 181 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-1000_MChi-150_13TeV	1 182 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-1000_MChi-1_13TeV		1 183 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-500_MChi-500_13TeV		1 184 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-500_MChi-150_13TeV		1 185 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-500_MChi-1_13TeV		1 186 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-100_MChi-10_13TeV		1 187 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-10_MChi-10_13TeV		1 188 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-10000_MChi-1000_13TeV	1 189 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-10000_MChi-500_13TeV	1 190 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-10000_MChi-50_13TeV	1 191 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-10000_MChi-150_13TeV	1 192 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-10000_MChi-10_13TeV	1 193 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-10000_MChi-1_13TeV		1 194 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-100_MChi-1_13TeV		1 195 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-10_MChi-1000_13TeV		1 196 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-10_MChi-1_13TeV		1 197 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-10_MChi-50_13TeV		1 198 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-1995_MChi-1000_13TeV	1 200 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-2000_MChi-1_13TeV		1 201 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-2000_MChi-500_13TeV	1 202 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-200_MChi-150_13TeV		1 203 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-200_MChi-50_13TeV		1 204 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-200_MChi-1_13TeV		1 205 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-20_MChi-1_13TeV		1 206 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-295_MChi-150_13TeV    	1 207 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-300_MChi-50_13TeV		1 208 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-50_MChi-10_13TeV		1 209 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-50_MChi-1_13TeV		1 210 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-995_MChi-500_13TeV		1 211 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-15_MChi-10_13TeV		1 199 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-10_MChi-150_13TeV		1 212 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-10_MChi-500_13TeV		1 213 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-50_MChi-50_13TeV		1 214 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-95_MChi-50_13TeV		1 211 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py ZpBaryonic_MZp-300_MChi-1_13TeV		1 215 ${pileupFile} 0.002 1 

#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-10000_MChi-1000_13TeV		1 252 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-10000_MChi-10_13TeV  		1 253 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-10000_MChi-150_13TeV 		1 254 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-10000_MChi-1_13TeV   		1 255 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-10000_MChi-500_13TeV 		1 256 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-10000_MChi-50_13TeV  		1 257 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-1000_MChi-1000_13TeV 		1 258 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-1000_MChi-150_13TeV  		1 259 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-1000_MChi-1_13TeV    		1 260 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-100_MChi-10_13TeV    		1 261 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-100_MChi-1_13TeV     		1 262 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-10_MChi-1000_13TeV   		1 263 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-10_MChi-10_13TeV     		1 264 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-10_MChi-150_13TeV    		1 265 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-10_MChi-1_13TeV	     	1 266 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-10_MChi-500_13TeV    		1 267 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-10_MChi-50_13TeV     		1 268 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-15_MChi-10_13TeV     		1 269 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-200_MChi-150_13TeV   		1 270 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-200_MChi-1_13TeV     		1 271 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-200_MChi-50_13TeV    		1 272 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-20_MChi-1_13TeV	     	1 273 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-295_MChi-150_13TeV   		1 274 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-300_MChi-1_13TeV     		1 275 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-300_MChi-50_13TeV    		1 276 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-500_MChi-150_13TeV   		1 277 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-500_MChi-1_13TeV     		1 278 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-500_MChi-500_13TeV   		1 279 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-50_MChi-10_13TeV     		1 280 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-50_MChi-1_13TeV	     	1 281 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-50_MChi-50_13TeV     		1 282 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-95_MChi-50_13TeV     		1 283 ${pileupFile} 0.002 1
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py Scalar_MZp-995_MChi-500_13TeV   		1 284 ${pileupFile} 0.002 1 

#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py SMS-T2bH_mSbottom-500_mLSP-1   		1 285 ${pileupFile} 0.002 1 
#./submitBatchDiPho.py --cfg moriond17diPhoAnaBATCH.py SMS-T2bH_mSbottom-500_mLSP-50   		1 286 ${pileupFile} 0.002 1 
