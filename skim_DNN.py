#!/usr/bin/env python

# skim_DNN.py

import sys
import os

import numpy as np
import math

from collections import OrderedDict
import re

import uproot3 as uproot
from ROOT import TFile
from ROOT import TTree
from ROOT import TDirectory
from ROOT import Math as TMath
from ROOT import TLorentzVector
from ROOT import VecOps

import pandas as pd

import keras
from keras.models import Sequential
from tensorflow.keras.models import model_from_json
from keras.layers import Dense
from keras.layers import Dropout
from keras.layers import Flatten
from keras.layers.convolutional import Conv2D
from keras.layers.convolutional import MaxPooling2D
from keras.utils.np_utils import to_categorical
from keras.utils import plot_model

from sklearn.preprocessing import normalize


debug = False

################################################################################################
# Input file and tree
inDir_nm   = sys.argv[1]
inFile_nm  = sys.argv[2]
outDir_nm  = sys.argv[3]
inTree_nm  = sys.argv[4]

cut = float(sys.argv[5])

makeOutTrees = bool(sys.argv[6]=='y')

effOut = bool(sys.argv[7][0]=='y')
effWriteMode = sys.argv[7][1]

calculateCutBasedEff = bool(sys.argv[8][0]=='y')
cutBasedEffWriteMode = sys.argv[8][1]

mggOut = bool(sys.argv[9][0]=='y')
mggWriteMode = sys.argv[9][1]

effVsDNNscoreOut = bool(sys.argv[10][0]=='y')
effVsDNNscoreWriteMode = sys.argv[10][1]

calculateMetCatDNNeff = bool(sys.argv[11]=='y')

dnnModel   = sys.argv[12]
dnnWeights = sys.argv[13]

medMass = float(sys.argv[14])

################################################################################################

#inFile_nm    = "FailingPhotonID_2017C_88percent.root"
#inTreeDir_nm = ""
#inTree_nm    = "DiPhotonTree"

#cut = 0.5

print ""
print ""
print "InDir: ", inDir_nm
print "FileName: ", inFile_nm
print "OutDir: ", outDir_nm
print "Tree: ", inTree_nm
print ""
print "Cut: ", cut
print "Make outTrees: ", makeOutTrees
print "Write Efficiency: ", effOut
print "EfficiencyFile WriteMode: ", effWriteMode
print "Calculate CutBased Efficiency: ", calculateCutBasedEff
print "CutBased EfficiencyFile WriteMode: ", cutBasedEffWriteMode
print "Write mgg: ", mggOut
print "mggFile WriteMode: ", mggWriteMode
print "Efficiency vs. DNNScore: ", effVsDNNscoreOut
print "EffVsDNNscore WriteMode: ", effVsDNNscoreWriteMode
print ""
print "Mediator Mass: ", medMass
print ""
print ""


####################################################################################################
# Check and create outDir
if( not os.path.isdir(outDir_nm) ):
    os.mkdir( outDir_nm )


# Open inFile and Tree
inFile = uproot.open( inDir_nm+inFile_nm )
inTree = inFile[ inTree_nm ]



# Check if inFile is data
isData = False
if( inFile_nm.find("DoubleEG")+1 ):
    print "Data file"
    print ""
    isData = True

# Check if inFile is QCD
isQCD = False
if( inFile_nm.find("QCD")+1 ):
    print "QCD file"
    print ""
    isQCD = True

# Check if inFile is GJets
isGJets = False
if( inFile_nm.find("GJets")+1 ):
    print "GJets file"
    print ""
    isGJets = True

# Check if inFile is MC signal
isSig = False
if( inFile_nm.find("2HDMa_mA")+1 ):
    print "Signal file"
    print ""
    isSig = True

# Check if inFile is MC background
isBkg = False
if( not isData and not isSig ):
    print "Background file"
    print ""
    isBkg = True

########################################################################################################



print "Loading relevant branches to array"


####################################
# Store all branch names in a list #
####################################
l_branch_nm = inTree.keys()
if( debug ): print l_branch_nm



########################################################
# Load relevant branches for DNN from inTree to array  #
########################################################

arr_mgg = inTree.array("mgg")
arr_ptgg = inTree.array("ptgg")

arr_pt1  = inTree.array("pt1")
arr_eta1 = inTree.array("eta1")
arr_phi1 = inTree.array("phi1")

arr_pt2  = inTree.array("pt2")
arr_eta2 = inTree.array("eta2")
arr_phi2 = inTree.array("phi2")

arr_ptJetLead   = inTree.array("ptJetLead")
arr_etaJetLead  = inTree.array("etaJetLead")
arr_phiJetLead  = inTree.array("phiJetLead")
arr_massJetLead = inTree.array("massJetLead")

arr_ptJetSubLead   = inTree.array("ptJetSubLead")
arr_etaJetSubLead  = inTree.array("etaJetSubLead")
arr_phiJetSubLead  = inTree.array("phiJetSubLead")
arr_massJetSubLead = inTree.array("massJetSubLead")

arr_ptJet3   = inTree.array("ptJet3")
arr_etaJet3  = inTree.array("etaJet3")
arr_phiJet3  = inTree.array("phiJet3")
arr_massJet3 = inTree.array("massJet3")

arr_ptJet4   = inTree.array("ptJet4")
arr_etaJet4  = inTree.array("etaJet4")
arr_phiJet4  = inTree.array("phiJet4")
arr_massJet4 = inTree.array("massJet4")

arr_t1pfmetCorr    = inTree.array("t1pfmetCorr")
arr_t1pfmetCorrPhi = inTree.array("t1pfmetCorrPhi")
arr_t1pfmetCorrSig = inTree.array("t1pfmetCorrSig")

arr_deltaR12 = inTree.array("deltaR12")

arr_nJets30 = inTree.array("nJets30")
arr_nJets50 = inTree.array("nJets50")
arr_nEle    = inTree.array("nEle")
arr_nMuons  = inTree.array("nMuons")

####################################################################################################



###########################################################################
# Load these branches for calculating cut-based efficiencies (old method) #
###########################################################################
arr_metF_GV = []
arr_metF_globalTightHalo = []
arr_metF_HBHENoise = []
arr_metF_HBHENoiseIso = []
arr_metF_EcalDeadCell = []
arr_metF_badMuon = []
arr_genmatch1 = []
arr_genmatch2 = []
arr_hltDiphoton30Mass95 = []
arr_metF_eeBadSC = []

if( calculateCutBasedEff ):

    arr_metF_GV              = inTree.array("metF_GV")
    arr_metF_globalTightHalo = inTree.array("metF_globalTightHalo")
    arr_metF_HBHENoise       = inTree.array("metF_HBHENoise")
    arr_metF_HBHENoiseIso    = inTree.array("metF_HBHENoiseIso")
    arr_metF_EcalDeadCell    = inTree.array("metF_EcalDeadCell")
    arr_metF_badMuon         = inTree.array("metF_badMuon")

    arr_genmatch1 = inTree.array("genmatch1")
    arr_genmatch2 = inTree.array("genmatch2")

    # Load these branches only for data
    if( isData ):
        arr_hltDiphoton30Mass95 = inTree.array("hltDiphoton30Mass95")
        arr_metF_eeBadSC        = inTree.array("metF_eeBadSC")

##################################################################################################################



##################################################
# Compute and store necessary quantities for DNN #
##################################################

print "calculating LorentzVecPho1 \r",
sys.stdout.flush()
arr_fLorentzVecPho1 = np.array( [TMath.LorentzVector('ROOT::Math::PtEtaPhiM4D<double>')(pt1,eta1,phi1,0.)
                                for pt1,eta1,phi1
                                in zip(arr_pt1,arr_eta1,arr_phi1)] )
print "LorentzVecPho1 calculated"


print "calculating LorentzVecPho2"
arr_fLorentzVecPho2 = np.array( [TMath.LorentzVector('ROOT::Math::PtEtaPhiM4D<double>')(pt2,eta2,phi2,0.)
                                for pt2,eta2,phi2
                                in zip(arr_pt2,arr_eta2,arr_phi2)] )
sys.stdout.write("\033[F"+"")
print "\rLorentzVecPho2 calculated"


print "calculating LorentzVecGG"
arr_fLorentzVecGG   = np.array( [fLorentzVecPho1+fLorentzVecPho2
                                for fLorentzVecPho1,fLorentzVecPho2
                                in zip(arr_fLorentzVecPho1,arr_fLorentzVecPho2)] )
sys.stdout.write("\033[F"+"")
print "\rLorentzVecGG calculated"




print "calculating LorentzVecJet1"
arr_fLorentzVecJet1 = np.array( [TMath.LorentzVector('ROOT::Math::PtEtaPhiM4D<double>')(ptJetLead,etaJetLead,phiJetLead,massJetLead)
                                for ptJetLead,etaJetLead,phiJetLead,massJetLead
                                in zip(arr_ptJetLead,arr_etaJetLead,arr_phiJetLead,arr_massJetLead)] )
sys.stdout.write("\033[F"+"")
print "\rLorentzVecJet1 calculated"


print "calculating LorentzVecJet2"
arr_fLorentzVecJet2 = np.array( [TMath.LorentzVector('ROOT::Math::PtEtaPhiM4D<double>')(ptJetSubLead,etaJetSubLead,phiJetSubLead,massJetSubLead)
                                for ptJetSubLead,etaJetSubLead,phiJetSubLead,massJetSubLead
                                in zip(arr_ptJetSubLead,arr_etaJetSubLead,arr_phiJetSubLead,arr_massJetSubLead)] )
sys.stdout.write("\033[F"+"")
print "\rLorentzVecJet2 calculated"


print "calculating LorentzVecJet3"
arr_fLorentzVecJet3 = np.array( [TMath.LorentzVector('ROOT::Math::PtEtaPhiM4D<double>')(ptJet3,etaJet3,phiJet3,massJet3)
                                for ptJet3,etaJet3,phiJet3,massJet3
                                in zip(arr_ptJet3,arr_etaJet3,arr_phiJet3,arr_massJet3)] )
sys.stdout.write("\033[F"+"")
print "\rLorentzVecJet3 calculated"


print "calculating LorentzVecJet4"
arr_fLorentzVecJet4 = np.array( [TMath.LorentzVector('ROOT::Math::PtEtaPhiM4D<double>')(ptJet4,etaJet4,phiJet4,massJet4)
                                for ptJet4,etaJet4,phiJet4,massJet4
                                in zip(arr_ptJet4,arr_etaJet4,arr_phiJet4,arr_massJet4)] )
sys.stdout.write("\033[F"+"")
print "\rLorentzVecJet4 calculated"



print "calculating phigg"
arr_phigg = np.array( [fLorentzVecGG.Phi() for fLorentzVecGG in arr_fLorentzVecGG] )
sys.stdout.write("\033[F"+"")  # cursor up one line in terminal
print "\rphigg calculated"



print "calculating dphiggmet"
arr_dphiggmet = np.abs( [VecOps.DeltaPhi[float](fLorentzVecGG.Phi(),t1pfmetCorrPhi)
                        for fLorentzVecGG,t1pfmetCorrPhi
                        in zip(arr_fLorentzVecGG,arr_t1pfmetCorrPhi)] )
sys.stdout.write("\033[F"+"")
print "\rdphiggmet calculated"


print "calculating dphiPho1met"
arr_dphiPho1met = np.abs( [VecOps.DeltaPhi[float](fLorentzVecPho1.Phi(),t1pfmetCorrPhi)
                          for fLorentzVecPho1,t1pfmetCorrPhi
                          in zip(arr_fLorentzVecPho1,arr_t1pfmetCorrPhi)] )
sys.stdout.write("\033[F"+"")
print "\rdphiPho1met calulated"


print "calculating dphiPho2met"
arr_dphiPho2met = np.abs( [VecOps.DeltaPhi[float](fLorentzVecPho2.Phi(),t1pfmetCorrPhi)
                          for fLorentzVecPho2,t1pfmetCorrPhi
                          in zip(arr_fLorentzVecPho2,arr_t1pfmetCorrPhi)] )
sys.stdout.write("\033[F"+"")
print "\rdphiPho2met calulated"


print "calculating dphiPho1Pho2"
arr_dphiPho1Pho2 = np.abs( [VecOps.DeltaPhi[float](fLorentzVecPho1.Phi(),fLorentzVecPho2.Phi())
                           for fLorentzVecPho1,fLorentzVecPho2
                           in zip(arr_fLorentzVecPho1,arr_fLorentzVecPho2)] )
sys.stdout.write("\033[F"+"")
print "\rdphiPho1Pho2met calulated"



print "calculating dphij1met"
arr_dphij1met = np.abs( [VecOps.DeltaPhi[float](fLorentzVecJet1.Phi(),t1pfmetCorrPhi)
                        for fLorentzVecJet1,t1pfmetCorrPhi
                        in zip(arr_fLorentzVecJet1,arr_t1pfmetCorrPhi)] )
sys.stdout.write("\033[F"+"")
print "\rdphij1met calculated"


print "calculating dphij2met"
arr_dphij2met = np.abs( [VecOps.DeltaPhi[float](fLorentzVecJet2.Phi(),t1pfmetCorrPhi)
                        for fLorentzVecJet2,t1pfmetCorrPhi
                        in zip(arr_fLorentzVecJet2,arr_t1pfmetCorrPhi)] )
sys.stdout.write("\033[F"+"")
print "\rdphij2met calculated"


print "calculating dphij3met"
arr_dphij3met = np.abs( [VecOps.DeltaPhi[float](fLorentzVecJet3.Phi(),t1pfmetCorrPhi)
                        for fLorentzVecJet3,t1pfmetCorrPhi
                        in zip(arr_fLorentzVecJet3,arr_t1pfmetCorrPhi)] )
sys.stdout.write("\033[F"+"")
print "\rdphij3met calculated"


print "calculating dphij4met"
arr_dphij4met = np.abs( [VecOps.DeltaPhi[float](fLorentzVecJet4.Phi(),t1pfmetCorrPhi)
                        for fLorentzVecJet4,t1pfmetCorrPhi
                        in zip(arr_fLorentzVecJet4,arr_t1pfmetCorrPhi)] )
sys.stdout.write("\033[F"+"")
print "\rdphij4met calculated"


print "calculating dphiPho1j1",
sys.stdout.flush()
arr_dphiPho1j1 = np.abs( [VecOps.DeltaPhi[float](fLorentzVecPho1.Phi(),fLorentzVecJet1.Phi())
                         for fLorentzVecPho1,fLorentzVecJet1
                         in zip(arr_fLorentzVecPho1,arr_fLorentzVecJet1)] )
print "\rdphiPho1j1met calculated"


print "calculating dphiPho1j2"
arr_dphiPho1j2 = np.abs( [VecOps.DeltaPhi[float](fLorentzVecPho1.Phi(),fLorentzVecJet2.Phi())
                         for fLorentzVecPho1,fLorentzVecJet2
                         in zip(arr_fLorentzVecPho1,arr_fLorentzVecJet2)] )
sys.stdout.write("\033[F"+"")
print "\rdphiPho1j2met calculated"


print "calculating dphiPho2j1"
arr_dphiPho2j1 = np.abs( [VecOps.DeltaPhi[float](fLorentzVecPho2.Phi(),fLorentzVecJet1.Phi())
                         for fLorentzVecPho2,fLorentzVecJet1
                         in zip(arr_fLorentzVecPho2,arr_fLorentzVecJet1)] )
sys.stdout.write("\033[F"+"")
print "\rdphiPho2j1met calculated"


print "calculating dphiPho2j2"
arr_dphiPho2j2 = np.abs( [VecOps.DeltaPhi[float](fLorentzVecPho2.Phi(),fLorentzVecJet2.Phi())
                         for fLorentzVecPho2,fLorentzVecJet2
                         in zip(arr_fLorentzVecPho2,arr_fLorentzVecJet2)] )
sys.stdout.write("\033[F"+"")
print "\rdphiPho2j2met calculated"



print "M_R calculating"
arr_M_R = np.sqrt( [(fLorentzVecPho1.E()+fLorentzVecPho2.E())**2 + (fLorentzVecPho1.Pz()+fLorentzVecPho2.Pz())**2
                   for fLorentzVecPho1,fLorentzVecPho2
                   in zip(arr_fLorentzVecPho1,arr_fLorentzVecPho2)] )
sys.stdout.write("\033[F"+"")
print "\rM_R calculated"


print "MT_R calculating"
arr_MT_R = np.array( [t1pfmetCorr*( pt1+pt2 - pt1*math.cos(dphiPho1met) - pt2*math.cos(dphiPho2met) )
                     for t1pfmetCorr,pt1,pt2,dphiPho1met,dphiPho2met
                     in zip(arr_t1pfmetCorr,arr_pt1,arr_pt2,arr_dphiPho1met,arr_dphiPho2met)] )
sys.stdout.write("\033[F"+"")
print "\rMT_R calculated"


print "R calculating"
arr_R = np.array( [MT_R/M_R for MT_R,M_R in zip(arr_MT_R,arr_M_R)] )
sys.stdout.write("\033[F"+"")
print "\rR calculated"



print ""
totalEvents = len( arr_mgg )


####################################################################
# Following quantities are calculated only for cutBased efficiency #
####################################################################
arr_dphij1metmin = 10.*np.ones( totalEvents )
arr_dphij2metmin = 10.*np.ones( totalEvents )
arr_dphij3metmin = 10.*np.ones( totalEvents )
arr_dphij4metmin = 10.*np.ones( totalEvents )
arr_mindphijmet  = 10.*np.ones( totalEvents )
if( calculateCutBasedEff ):
    print "dphij1metmin calulated"
    arr_dphij1metmin = np.abs( [VecOps.DeltaPhi[float](fLorentzVecJet1.Phi(),t1pfmetCorrPhi) if ptJetLead>50 else dphij1metmin
                            for fLorentzVecJet1,t1pfmetCorrPhi,ptJetLead,dphij1metmin
                            in zip(arr_fLorentzVecJet1,arr_t1pfmetCorrPhi,arr_ptJetLead,arr_dphij1metmin)] )

    print "dphij2metmin calulated"
    arr_dphij2metmin = np.abs( [VecOps.DeltaPhi[float](fLorentzVecJet2.Phi(),t1pfmetCorrPhi) if ptJetSubLead>50 else dphij2metmin
                            for fLorentzVecJet2,t1pfmetCorrPhi,ptJetSubLead,dphij2metmin
                            in zip(arr_fLorentzVecJet2,arr_t1pfmetCorrPhi,arr_ptJetSubLead,arr_dphij2metmin)] )

    print "dphij3metmin calulated"
    arr_dphij3metmin = np.abs( [VecOps.DeltaPhi[float](fLorentzVecJet3.Phi(),t1pfmetCorrPhi) if ptJet3>50 else dphij3metmin
                            for fLorentzVecJet3,t1pfmetCorrPhi,ptJet3,dphij3metmin
                            in zip(arr_fLorentzVecJet3,arr_t1pfmetCorrPhi,arr_ptJet3,arr_dphij3metmin)] )

    print "dphij4metmin calulated"
    arr_dphij4metmin = np.abs( [VecOps.DeltaPhi[float](fLorentzVecJet4.Phi(),t1pfmetCorrPhi) if ptJet4>50 else dphij4metmin
                            for fLorentzVecJet4,t1pfmetCorrPhi,ptJet4,dphij4metmin
                            in zip(arr_fLorentzVecJet4,arr_t1pfmetCorrPhi,arr_ptJet4,arr_dphij4metmin)] )

    print "mindphijmet calulated"
    arr_mindphijmet = np.array( [ min([dphij1metmin,dphij2metmin,dphij3metmin,dphij4metmin]) if nJets50>0 else mindphijmet
                            for dphij1metmin,dphij2metmin,dphij3metmin,dphij4metmin,nJets50,mindphijmet
                            in zip(arr_dphij1metmin,arr_dphij2metmin,arr_dphij3metmin,arr_dphij4metmin,arr_nJets50,arr_mindphijmet) ])


#####################################################################################################################################################################




#################################################################
# Calculate pass arrays for cut-based efficiencies (old method) #
#################################################################

passedEvents_cutBased            = 0
passedEvents_cutBased_met50      = 0
passedEvents_cutBased_met50_70   = 0
passedEvents_cutBased_met70_100  = 0
passedEvents_cutBased_met100_130 = 0
passedEvents_cutBased_met130_150 = 0
passedEvents_cutBased_met150     = 0

cutBasedEff            = 0
cutBasedEff10K         = 0
cutBasedEff_met50      = 0
cutBasedEff_met50_70   = 0
cutBasedEff_met70_100  = 0
cutBasedEff_met100_130 = 0
cutBasedEff_met130_150 = 0
cutBasedEff_met150     = 0


if( calculateCutBasedEff ):
    triggered = np.ones( totalEvents, dtype=bool )
    if( isData ):
        triggered = arr_hltDiphoton30Mass95.astype(bool)
        

    passMetFil_All = (arr_metF_GV * arr_metF_globalTightHalo * arr_metF_HBHENoise * arr_metF_HBHENoiseIso
                      * arr_metF_EcalDeadCell * arr_metF_badMuon).astype(bool)

    passMetFil_Data = np.ones( totalEvents, dtype=bool )
    if( isData ):
        passMetFil_Data = arr_metF_eeBadSC.astype(bool)

    passMETfilters = passMetFil_All * passMetFil_Data


    passKinematics = ( (arr_pt1>0.5*arr_mgg) * (arr_pt2>0.25*arr_mgg)
                       * (((arr_t1pfmetCorr<=150) * (arr_ptgg>40)) + ((arr_t1pfmetCorr>150) * (arr_ptgg>90)))
                       * (arr_mgg>=100) * (arr_mgg<=200) )


    pass_GJet_DupRem = ( ((not isGJets)*np.ones(totalEvents)).astype(bool)
                         + (((isGJets)*np.ones(totalEvents)).astype(bool) * ( (arr_genmatch1 !=1) + (arr_genmatch2 !=1) )) )

    pass_QCD_DupRem = ( ((not isQCD)*np.ones(totalEvents)).astype(bool)
                        + (((isQCD)*np.ones(totalEvents)).astype(bool) * ( (arr_genmatch1 !=1) + (arr_genmatch2 !=1) )) )

    passDupRemoval = pass_GJet_DupRem * pass_QCD_DupRem


    passLepVetoes = (arr_nEle==0) * (arr_nMuons==0)
    passJetVetoes = arr_nJets30<3

    passDphiCuts  = (arr_dphiggmet>=2.1) * (arr_mindphijmet>=0.5)

    passAllCuts = (triggered * passMETfilters * passKinematics * passDupRemoval
                   * passLepVetoes * passDphiCuts * passJetVetoes).astype(bool)


    # Calculate passed events and efficiencies for cut-based method

    # All Events
    passedEvents_cutBased = sum(passAllCuts)
    cutBasedEff   = float(passedEvents_cutBased)/float(totalEvents)*100.
    if( isSig ):
        cutBasedEff10K = float(passedEvents_cutBased)/float(10000)*100.

    # MET category met50 (>=50)
    passedEvents_cutBased_met50 = sum( passAllCuts * (arr_t1pfmetCorr>=50) )
    cutBasedEff_met50   = float(passedEvents_cutBased_met50)/float(totalEvents)*100.

    # MET category met50_70 (>=50 and <70)
    passedEvents_cutBased_met50_70 = sum( passAllCuts * (arr_t1pfmetCorr>=50) * (arr_t1pfmetCorr<70) )
    cutBasedEff_met50_70   = float(passedEvents_cutBased_met50_70)/float(totalEvents)*100.

    # MET category met70_100 (>=70 and <100)
    passedEvents_cutBased_met70_100 = sum( passAllCuts * (arr_t1pfmetCorr>=70) * (arr_t1pfmetCorr<100) )
    cutBasedEff_met70_100   = float(passedEvents_cutBased_met70_100)/float(totalEvents)*100.

    # MET category met100_130 (>=100 and <130)
    passedEvents_cutBased_met100_130 = sum( passAllCuts * (arr_t1pfmetCorr>=100) * (arr_t1pfmetCorr<130) )
    cutBasedEff_met100_130   = float(passedEvents_cutBased_met100_130)/float(totalEvents)*100.

    # MET category met130_150 (>=130 and <150)
    passedEvents_cutBased_met130_150 = sum( passAllCuts * (arr_t1pfmetCorr>=130) * (arr_t1pfmetCorr<150) )
    cutBasedEff_met130_150   = float(passedEvents_cutBased_met130_150)/float(totalEvents)*100.

    # MET category met150 (>=150)
    passedEvents_cutBased_met150 = sum( passAllCuts * (arr_t1pfmetCorr>=150) )
    cutBasedEff_met150   = float(passedEvents_cutBased_met150)/float(totalEvents)*100.


################################################################################################################################




########################
# Parametric DNN Model #
########################

# Load DNN model
#dnn_json = open('AnalysisSelection_allvar_womgg_ANN_model.json','r')
#dnn_json = open('./DNN_new/AnalysisSelection_allvar_halfStat_womgg_ANN_parametric_model.json')
dnn_json = open(dnnModel)
dnn_model_json = dnn_json.read()
dnn_json.close()

dnn_model = model_from_json( dnn_model_json )


# Load weights
#dnn_model.load_weights('AnalysisSelection_allvar_womgg_ANN_model.h5','r')
#dnn_model.load_weights('./DNN_new/AnalysisSelection_allvar_halfStat_womgg_ANN_parametric_model.h5')
dnn_model.load_weights(dnnWeights)
print ("\nModel & weights ---> LOCK & LOAD\n")

dnn_model.compile(loss="categorical_crossentropy",optimizer="adam",metrics=['binary_accuracy'])

# array for mediator mass which is passed as a parameter in the DNN
arr_medMass = medMass * np.ones( len(arr_pt1) )

# DNN input array
x_test_dict = OrderedDict([ ("mgg",arr_mgg), ("t1pfmetCorr",arr_t1pfmetCorr),
                            ("pt1",arr_pt1), ("pt2",arr_pt2), ("ptgg",arr_ptgg),
                            ("t1pfmetCorrSig",arr_t1pfmetCorrSig), ("deltaR12",arr_deltaR12),
                            ("nJets30",arr_nJets30), ("nEle",arr_nEle), ("nMuons",arr_nMuons),
                            ("dphiggmet",arr_dphiggmet),
                            ("dphiPho1met",arr_dphiPho1met), ("dphiPho2met",arr_dphiPho2met),
                            ("dphij1met",arr_dphij1met), ("dphij2met",arr_dphij2met),
                            ("dphiPho1j1",arr_dphiPho1j1), ("dphiPho1j2",arr_dphiPho1j2),
                            ("dphiPho2j1",arr_dphiPho2j1), ("dphiPho2j2",arr_dphiPho2j2),
                            ("eta1",arr_eta1),("eta2",arr_eta2),("dphiPho1Pho2",arr_dphiPho1Pho2),
                            ("ptJetLead",arr_ptJetLead), ("ptJetSubLead",arr_ptJetSubLead),
                            ("M_R",arr_M_R),("MT_R",arr_MT_R),("R",arr_R),
                            ("medMass",arr_medMass)
                         ])

x_test = pd.DataFrame.from_dict( x_test_dict ).to_numpy()

#if( debug ):
#signalFile = open("signalFile.csv","w")
#for irow in xrange(len(x_test[:,0])):
#    #print x_test[irow,:]
#    for icol in x_test[irow,:]:
#        #print icol
#        signalFile.write( str(icol)+" " )

#    signalFile.write("\n")

#signalFile.close()

#normWith = np.array([ 7.08940e+02, 5.25537e+02, 3.15149e+02, 8.11041e+02, 5.52040e+02, 3.84614e+00,
#                      7.00000e+00, 2.00000e+00, 1.00000e+00, 3.14151e+00, 3.14148e+00, 3.14130e+00,
#                      3.14125e+00, 3.14123e+00, 3.14142e+00, 3.14136e+00, 3.14146e+00, 3.14146e+00,
#                      2.51262e+00, 2.50267e+00, 3.14159e+00, 1.16107e+03, 4.10661e+02, 2.20970e+03,
#                      2.99690e+05, 4.38648e+02 ])

normWith = np.array([  1.94852e+03, 8.80390e+02, 3.15579e+02, 9.36157e+02, 1.81368e+03, 3.83984e+00,
                       7.00000e+00, 2.00000e+00, 1.00000e+00, 3.14155e+00, 3.14142e+00, 3.14117e+00,
                       3.14157e+00, 3.14156e+00, 3.14142e+00, 3.14070e+00, 3.14152e+00, 3.14157e+00,
                       2.51599e+00, 2.51793e+00, 3.14158e+00, 1.43417e+03, 6.16126e+02, 2.59100e+03,
                       4.96325e+05, 1.19985e+03, 6.00000e+02 ])


x_test = x_test[:,1:]
if( debug ): print x_test
x_test = x_test / normWith[None,:]
if( debug ): print x_test[0,:]

y_prob = dnn_model.predict( x_test, verbose=1 )[:,1]
print ""
#print y_score,">=",cut
y_score = y_prob >= cut
#print y_score
#print ""

###############################################################################################################################



####################################################
# Calculate passed events and efficiencies for DNN #
####################################################

# All Events
passedEvents_dnn = sum( y_score )
dnnEff = float(passedEvents_dnn)/float(totalEvents)*100.
if( calculateMetCatDNNeff ):
    dnnEff10K = 0
    if( isSig ):
        dnnEff10K = float(passedEvents_dnn)/float(10000)*100.

    # MET category met50 (>=50)
    passedEvents_dnn_met50 = sum( y_score * (arr_t1pfmetCorr>=50) )
    dnnEff_met50   = float(passedEvents_dnn_met50)/float(totalEvents)*100.

    # MET category met50_70 (>=50 and <70)
    passedEvents_dnn_met50_70 = sum( y_score * (arr_t1pfmetCorr>=50) * (arr_t1pfmetCorr<70) )
    dnnEff_met50_70   = float(passedEvents_dnn_met50_70)/float(totalEvents)*100.

    # MET category met70_100 (>=70 and <100)
    passedEvents_dnn_met70_100 = sum( y_score * (arr_t1pfmetCorr>=70) * (arr_t1pfmetCorr<100) )
    dnnEff_met70_100   = float(passedEvents_dnn_met70_100)/float(totalEvents)*100.

    # MET category met100_130 (>=100 and <130)
    passedEvents_dnn_met100_130 = sum( y_score * (arr_t1pfmetCorr>=100) * (arr_t1pfmetCorr<130) )
    dnnEff_met100_130   = float(passedEvents_dnn_met100_130)/float(totalEvents)*100.

    # MET category met130_150 (>=130 and <150)
    passedEvents_dnn_met130_150 = sum( y_score * (arr_t1pfmetCorr>=130) * (arr_t1pfmetCorr<150) )
    dnnEff_met130_150   = float(passedEvents_dnn_met130_150)/float(totalEvents)*100.

    # MET category met150 (>=150)
    passedEvents_dnn_met150 = sum( y_score * (arr_t1pfmetCorr>=150) )
    dnnEff_met150   = float(passedEvents_dnn_met150)/float(totalEvents)*100.

##################################################################################################################



#################
# Make outTrees #
#################

if( makeOutTrees ):
    # Create the branches for outTree
    outBranch_dict = OrderedDict()

    # Iterate through all branches of inTree and store in outBranch_dict
    # And also add new branches
    for branch in l_branch_nm:
        dType = str( type(inTree.array( str(branch),entrystart=0,entrystop=1 )[0]) )
        dType = re.findall(r"[\w]+|[^\s\w]", dType)[5]
        if( debug ): print branch," ",dType
        if (dType == "uint32"): dType = "int64"

        outBranch_dict[str(branch)] = uproot.newbranch(dType, title=str(branch))

        # Add new branches
        outBranch_dict["phigg"] = uproot.newbranch("float32", title="phigg")
        outBranch_dict["mindphijmet"] = uproot.newbranch("float32", title="mindphijmet")

        outBranch_dict["dphiggmet"] = uproot.newbranch("float32", title="dphiggmet")

        outBranch_dict["dphiPho1met"]  = uproot.newbranch("float32", title="dphiPho1met")
        outBranch_dict["dphiPho2met"]  = uproot.newbranch("float32", title="dphiPho2met")
        outBranch_dict["dphiPho1Pho2"] = uproot.newbranch("float32", title="dphiPho1Pho2")

        outBranch_dict["dphij1met"] = uproot.newbranch("float32", title="dphij1met")
        outBranch_dict["dphij2met"] = uproot.newbranch("float32", title="dphij2met")
        outBranch_dict["dphij3met"] = uproot.newbranch("float32", title="dphij3met")
        outBranch_dict["dphij4met"] = uproot.newbranch("float32", title="dphij4met")
        outBranch_dict["mindphijmet"] = uproot.newbranch("float32", title="mindphijmet")

        outBranch_dict["dphiPho1j1"] = uproot.newbranch("float32", title="dphiPho1j1")
        outBranch_dict["dphiPho1j2"] = uproot.newbranch("float32", title="dphiPho1j2")
        outBranch_dict["dphiPho2j1"] = uproot.newbranch("float32", title="dphiPho2j1")
        outBranch_dict["dphiPho2j2"] = uproot.newbranch("float32", title="dphiPho2j2")

        outBranch_dict["M_R"]  = uproot.newbranch("float32", title="M_R")
        outBranch_dict["MT_R"] = uproot.newbranch("float32", title="MT_R")
        outBranch_dict["R"]    = uproot.newbranch("float32", title="R")

        outBranch_dict["DNNscore"] = uproot.newbranch("float32", title="DNNscore")



        # Create the tree and out root file
        outFile_nm = inFile_nm.split(".")[0] + "_skimmedTree_mA_"+str(medMass).replace(".","p")+".root"

    print "Filling branches of outTree"
    with uproot.recreate( outDir_nm+"/"+outFile_nm ) as outFile:
        outFile[ inTree_nm ] = uproot.newtree( outBranch_dict )

        # Iterate through existing branches from inTree and fill events selected through [y_score]
        for branch in l_branch_nm:
            arr_branchData = inTree.array( str(branch) )[y_score]
            outFile[inTree_nm][str(branch)].newbasket( arr_branchData )
            if( debug ): print len( arr_branchData )
            #print "Branch ",str(branch)," filled"

        # Fill new branches with events selected through [y_score]
        outFile[inTree_nm]["phigg"].newbasket( arr_phigg[y_score] )
        #print "Branch ","phigg"," filled"

        outFile[inTree_nm]["dphiggmet"].newbasket( arr_dphiggmet[y_score] )
        #print "Branch ","dphiggmet"," filled"

        outFile[inTree_nm]["dphiPho1met"].newbasket( arr_dphiPho1met[y_score] )
        #print "Branch ","dphiPho1met"," filled"
        outFile[inTree_nm]["dphiPho2met"].newbasket( arr_dphiPho2met[y_score] )
        #print "Branch ","dphiPho2met"," filled"
        outFile[inTree_nm]["dphiPho1Pho2"].newbasket( arr_dphiPho1Pho2[y_score] )
        #print "Branch ","dphiPho1Pho2"," filled"

        outFile[inTree_nm]["dphij1met"].newbasket( arr_dphij1met[y_score] )
        #print "Branch ","dphij1met"," filled"
        outFile[inTree_nm]["dphij2met"].newbasket( arr_dphij2met[y_score] )
        #print "Branch ","dphij2met"," filled"
        outFile[inTree_nm]["dphij3met"].newbasket( arr_dphij3met[y_score] )
        #print "Branch ","dphij3met"," filled"
        outFile[inTree_nm]["dphij4met"].newbasket( arr_dphij4met[y_score] )
        #print "Branch ","dphij4met"," filled"
        outFile[inTree_nm]["mindphijmet"].newbasket( arr_mindphijmet[y_score] )
        #print "Branch ","mindphijmet"," filled"

        outFile[inTree_nm]["dphiPho1j1"].newbasket( arr_dphiPho1j1[y_score] )
        #print "Branch ","dphiPho1j1"," filled"
        outFile[inTree_nm]["dphiPho1j2"].newbasket( arr_dphiPho1j2[y_score] )
        #print "Branch ","dphiPho1j2"," filled"
        outFile[inTree_nm]["dphiPho2j1"].newbasket( arr_dphiPho2j1[y_score] )
        #print "Branch ","dphiPho2j1"," filled"
        outFile[inTree_nm]["dphiPho2j2"].newbasket( arr_dphiPho2j2[y_score] )
        #print "Branch ","dphiPho2j2"," filled"

        outFile[inTree_nm]["M_R"].newbasket( arr_M_R[y_score] )
        #print "Branch ","M_R"," filled"
        outFile[inTree_nm]["MT_R"].newbasket( arr_MT_R[y_score] )
        #print "Branch ","MT_R"," filled"
        outFile[inTree_nm]["R"].newbasket( arr_R[y_score] )
        #print "Branch ","R"," filled"

        outFile[inTree_nm]["DNNscore"].newbasket( y_prob[y_score] )




    print ""
    print "**********************************************************"
    print passedEvents_dnn,"passed out of a total of",totalEvents,"for a cut of",cut
    print "Efficiency =",round(dnnEff,2),"%"
    print "****************************************************************************"
    print outFile_nm, "created successfully!"
    print "****************************************************************************"
    print ""


if( calculateCutBasedEff ):
    print "*************************************************************"
    print passedEvents_cutBased,"passed out of a total of",totalEvents,"with cut-based method"
    print "Efficiency =",round(cutBasedEff,2),"%"
    print "*************************************************************"
    print ""


# Write efficiencies to file
if( effOut ):
    effFile = open( "./"+outDir_nm+"/"+"Efficiency.txt", effWriteMode )

    if( mode == 'w' ):
        effFile.write("{:<40}".format("Data/Background/Signal") +
                      "{:^6}".format("Cut") +
                      "{:^12}".format("Efficiency") + "\n")

    effFile.write( "{:<40}".format(inFile_nm.split('.')[0]) +
                   "{:^6.2f}".format( round(cut,2) ) +
                   "{:^12.2f}".format( round(effDNN,2) ) + "\n" )

    effFile.close()


# Write cut-based and DNN efficiencies to file comparison
if( calculateCutBasedEff ):
    effFile = open( "./"+outDir_nm+"/"+"Efficiency_cutBased_DNN.txt", cutBasedEffWriteMode )

    if( cutBasedEffWriteMode == 'w' ):
        effFile.write("CutBased Efficiency = CBE(%)" + "   " +
                      "DNN Efficiency (cut=" + str(round(cut,4))+") = DNE(%)" + "\n\n")

        effFile.write("{:<40}".format("Data/Background/Signal") +
                      "{:^15}".format("Total") +
                      "{:^15}".format("met50") +
                      "{:^15}".format("met50_70") +
                      "{:^15}".format("met70_100") +
                      "{:^15}".format("met100_130") +
                      "{:^15}".format("met130_150") +
                      "{:^15}".format("met150") +
                      "{:^15}".format("SigEff (10K)") + "\n")

        effFile.write("{:<40}".format(" ") +
                      "{:>15}".format("CBE    DNE  |") +
                      "{:>15}".format("CBE    DNE  |") +
                      "{:>15}".format("CBE    DNE  |") +
                      "{:>15}".format("CBE    DNE  |") +
                      "{:>15}".format("CBE    DNE  |") +
                      "{:>15}".format("CBE    DNE  |") +
                      "{:>15}".format("CBE    DNE  |") +
                      "{:>15}".format("CBE    DNE  |") + "\n")

    effFile.write( "{:<40}".format( inFile_nm.split('.')[0] ) +
                   "{:>6.2f}".format(round(cutBasedEff,2))           +"{:>7.2f}".format(round(dnnEff,2))           +"{:>2}".format("|") +
                   "{:>6.2f}".format(round(cutBasedEff_met50,2))     +"{:>7.2f}".format(round(dnnEff_met50,2))     +"{:>2}".format("|") +
                   "{:>6.2f}".format(round(cutBasedEff_met50_70,2))  +"{:>7.2f}".format(round(dnnEff_met50_70,2))  +"{:>2}".format("|") +
                   "{:>6.2f}".format(round(cutBasedEff_met70_100,2)) +"{:>7.2f}".format(round(dnnEff_met70_100,2)) +"{:>2}".format("|") +
                   "{:>6.2f}".format(round(cutBasedEff_met100_130,2))+"{:>7.2f}".format(round(dnnEff_met100_130,2))+"{:>2}".format("|") +
                   "{:>6.2f}".format(round(cutBasedEff_met130_150,2))+"{:>7.2f}".format(round(dnnEff_met130_150,2))+"{:>2}".format("|") +
                   "{:>6.2f}".format(round(cutBasedEff_met150,2))    +"{:>7.2f}".format(round(dnnEff_met150,2))    +"{:>2}".format("|") )
    if( isSig ):
        effFile.write("{:>6.2f}".format(round(cutBasedEff10K,2)) + "{:>7.2f}".format(round(dnnEff10K,2)) + "{:>2}".format("|") + "\n" )
    else:
        effFile.write("\n")


    effFile.close()



#outDir_mgg_nm = outDir_nm+"/"+outDir_nm+"_mgg"
outDir_mgg_nm = outDir_nm+"/skimmedTree_mgg"
if( mggOut ):
    if( not os.path.isdir(outDir_mgg_nm) ):
        os.mkdir( outDir_mgg_nm )


if( mggOut ):
    with uproot.recreate( outDir_mgg_nm+"/"+inFile_nm.split(".")[0] + "_mgg.root" ) as mggFile:
        # Signal
        if( isSig ):

            # mA200
            if( inFile_nm.find("200")+1 ):
                # DNN cut
                mggFile["sig_mA200_DNN"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                            "weight": uproot.newbranch(np.float64, title="weight")
                                                           })
                mggFile["sig_mA200_DNN"]["mgg"].newbasket( arr_mgg[y_score] )
                mggFile["sig_mA200_DNN"]["weight"].newbasket( inTree.array("weight")[y_score] )

                # CB cut
                mggFile["sig_mA200_CB"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                           "weight": uproot.newbranch(np.float64, title="weight")
                                                          })
                mggFile["sig_mA200_CB"]["mgg"].newbasket( arr_mgg[passAllCuts] )
                mggFile["sig_mA200_CB"]["weight"].newbasket( inTree.array("weight")[passAllCuts] )

                # DNN output
                mggFile["sig_mA200_DNN_score"] = uproot.newtree({ "DNNscore": uproot.newbranch(np.float64, title="DNNscore")})
                mggFile["sig_mA200_DNN_score"]["DNNscore"].newbasket( y_prob )



            # mA300
            elif( inFile_nm.find("300")+1 ):
                # DNN cut
                mggFile["sig_mA300_DNN"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                            "weight": uproot.newbranch(np.float64, title="weight")
                                                           })
                mggFile["sig_mA300_DNN"]["mgg"].newbasket( arr_mgg[y_score] )
                mggFile["sig_mA300_DNN"]["weight"].newbasket( inTree.array("weight")[y_score] )

                # CB cut
                mggFile["sig_mA300_CB"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                           "weight": uproot.newbranch(np.float64, title="weight")
                                                          })
                mggFile["sig_mA300_CB"]["mgg"].newbasket( arr_mgg[passAllCuts] )
                mggFile["sig_mA300_CB"]["weight"].newbasket( inTree.array("weight")[passAllCuts] )

                # DNN output
                mggFile["sig_mA300_DNN_score"] = uproot.newtree({ "DNNscore": uproot.newbranch(np.float64, title="DNNscore")})
                mggFile["sig_mA300_DNN_score"]["DNNscore"].newbasket( y_prob )


            # mA400
            elif( inFile_nm.find("400")+1 ):
                # DNN cut
                mggFile["sig_mA400_DNN"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                            "weight": uproot.newbranch(np.float64, title="weight")
                                                           })
                mggFile["sig_mA400_DNN"]["mgg"].newbasket( arr_mgg[y_score] )
                mggFile["sig_mA400_DNN"]["weight"].newbasket( inTree.array("weight")[y_score] )

                # CB cut
                mggFile["sig_mA400_CB"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                           "weight": uproot.newbranch(np.float64, title="weight")
                                                          })
                mggFile["sig_mA400_CB"]["mgg"].newbasket( arr_mgg[passAllCuts] )
                mggFile["sig_mA400_CB"]["weight"].newbasket( inTree.array("weight")[passAllCuts] )

                # DNN output
                mggFile["sig_mA400_DNN_score"] = uproot.newtree({ "DNNscore": uproot.newbranch(np.float64, title="DNNscore")})
                mggFile["sig_mA400_DNN_score"]["DNNscore"].newbasket( y_prob )


            # mA500
            elif( inFile_nm.find("500")+1 ):
                # DNN cut
                mggFile["sig_mA500_DNN"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                            "weight": uproot.newbranch(np.float64, title="weight")
                                                           })
                mggFile["sig_mA500_DNN"]["mgg"].newbasket( arr_mgg[y_score] )
                mggFile["sig_mA500_DNN"]["weight"].newbasket( inTree.array("weight")[y_score] )

                # CB cut
                mggFile["sig_mA500_CB"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                           "weight": uproot.newbranch(np.float64, title="weight")
                                                          })
                mggFile["sig_mA500_CB"]["mgg"].newbasket( arr_mgg[passAllCuts] )
                mggFile["sig_mA500_CB"]["weight"].newbasket( inTree.array("weight")[passAllCuts] )

                # DNN output
                mggFile["sig_mA500_DNN_score"] = uproot.newtree({ "DNNscore": uproot.newbranch(np.float64, title="DNNscore")})
                mggFile["sig_mA500_DNN_score"]["DNNscore"].newbasket( y_prob )


            # mA600
            elif( inFile_nm.find("600")+1 ):
                # DNN cut
                mggFile["sig_mA600_DNN"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                            "weight": uproot.newbranch(np.float64, title="weight")
                                                           })
                mggFile["sig_mA600_DNN"]["mgg"].newbasket( arr_mgg[y_score] )
                mggFile["sig_mA600_DNN"]["weight"].newbasket( inTree.array("weight")[y_score] )

                # CB cut
                mggFile["sig_mA600_CB"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                           "weight": uproot.newbranch(np.float64, title="weight")
                                                          })
                mggFile["sig_mA600_CB"]["mgg"].newbasket( arr_mgg[passAllCuts] )
                mggFile["sig_mA600_CB"]["weight"].newbasket( inTree.array("weight")[passAllCuts] )

                # DNN output
                mggFile["sig_mA600_DNN_score"] = uproot.newtree({ "DNNscore": uproot.newbranch(np.float64, title="DNNscore")})
                mggFile["sig_mA600_DNN_score"]["DNNscore"].newbasket( y_prob )


        # Background
        if( isBkg ):
            mggFile["bkg_DNN"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                  "weight": uproot.newbranch(np.float64, title="weight")
                                                 })

            mggFile["bkg_CB"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                 "weight": uproot.newbranch(np.float64, title="weight")
                                                })

            mggFile["bkg_DNN_score"] = uproot.newtree({ "DNNscore": uproot.newbranch(np.float64, title="DNNscore")})


            mggFile["bkg_DNN"]["mgg"].newbasket( arr_mgg[y_score] )
            mggFile["bkg_DNN"]["weight"].newbasket( inTree.array("weight")[y_score] )
            mggFile["bkg_CB"]["mgg"].newbasket( arr_mgg[passAllCuts] )
            mggFile["bkg_CB"]["weight"].newbasket( inTree.array("weight")[passAllCuts] )

            # DNN output
            mggFile["bkg_DNN_score"]["DNNscore"].newbasket( y_prob )



        # Data
        if( isData ):
            mggFile["data_DNN"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                   "weight": uproot.newbranch(np.float64, title="weight")
                                                  })

            mggFile["data_CB"] = uproot.newtree({ "mgg": uproot.newbranch(np.float64, title="mgg"),
                                                  "weight": uproot.newbranch(np.float64, title="weight")
                                                 })

            mggFile["data_DNN"]["mgg"].newbasket( arr_mgg[y_score] )
            mggFile["data_DNN"]["weight"].newbasket( inTree.array("weight")[y_score] )
            mggFile["data_CB"]["mgg"].newbasket( arr_mgg[passAllCuts] )
            mggFile["data_CB"]["weight"].newbasket( inTree.array("weight")[passAllCuts] )



if( effVsDNNscoreOut ):
    n = 20
    cut = 1.0/n
    outFile_nm = ""
    if( isSig ):
        outFile_nm = "./"+outDir_nm+"/"+"Efficiency_vs_DNNscore_sig_mA_"+str(medMass).replace(".","p")+".txt"
    if( isBkg ):
        outFile_nm = "./"+outDir_nm+"/"+"Efficiency_vs_DNNscore_bkg_mA_"+str(medMass).replace(".","p")+".txt"
    effVsDNNscoreOutFile = open(outFile_nm, effVsDNNscoreWriteMode)
    for i in range(n):
        y_score = y_prob >= cut
        effVsDNNscoreOutFile.write( str(cut) + "\t" + str( sum(y_score) ) + "\n" )
        cut += 1.0/n

    print outFile_nm+" created.\n\n\n"



