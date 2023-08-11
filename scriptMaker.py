#jsonMaker.py

import sys
import os
from collections import OrderedDict

dict_binEdge = OrderedDict()

#########################################################################################                                                                                              
## FOR SIMPLE DNN (all bins)                                                                                                                                                           
#########################################################################################                                                                                              
#dict_binEdge[200]=[0,0.03,0.05,0.1,0.2,0.39,0.75,0.93,0.98,0.99378,0.99835,1]                                                                                                         
#dict_binEdge[300]=[0,0.03,0.04,0.09,0.13,0.27,0.45,0.71,0.91,0.98,0.99183,1]                                                                                                          
#dict_binEdge[400]=[0,0.07,0.19,0.42,0.64,0.82,0.89,0.98,0.99285,0.99751,0.99885,1]                                                                                                    
#dict_binEdge[500]=[0,0.19,0.3,0.57,0.83,0.95,0.98,0.99228,0.99581,0.99845,0.99964,1]                                                                                                  
#dict_binEdge[600]=[0,0.41,0.57,0.8,0.93,0.98,0.99285,0.99733,0.99846,0.99964,0.99996,1]                                                                                               
#########################################################################################                                                                                              


########################################################################################                                                                                               
# FOR SIMPLE DNN (all bins) Central Samples                                                                                                                                            
########################################################################################                                                                                               
dict_binEdge[200]=[0, 0.112, 0.19,  0.385, 0.729, 0.908,   0.979,   0.99078, 0.99559, 0.99881, 1]
dict_binEdge[300]=[0, 0.04,  0.072, 0.106, 0.209, 0.385,   0.638,   0.863,   0.962,   0.99043, 1]
dict_binEdge[400]=[0, 0.255, 0.433, 0.644, 0.857, 0.955,   0.988,   0.99384, 0.99751, 0.99909, 1]
dict_binEdge[500]=[0, 0.329, 0.521, 0.746, 0.933, 0.98,    0.99291, 0.99654, 0.99851, 0.99964, 1]
dict_binEdge[600]=[0, 0.654, 0.838, 0.941, 0.975, 0.99127, 0.99603, 0.99873, 0.99964, 0.99996, 1]
dict_binEdge[700]=[0, 0.619, 0.849, 0.95,  0.986, 0.99334, 0.99696, 0.99861, 0.99964, 0.99999, 1]
dict_binEdge[800]=[0, 0.478, 0.719, 0.877, 0.954, 0.988,   0.99581, 0.99856, 0.99974, 0.99999, 1]
dict_binEdge[900]=[0, 0.497, 0.705, 0.919, 0.968, 0.99127, 0.996,   0.99864, 0.99964, 0.99999, 1]
########################################################################################


outDir_nm = "./jsonFiles_paraDNN"

if( not os.path.isdir(outDir_nm) ):
    os.mkdir( outDir_nm )


pDir="",""

def runCombineMakerMonoHggMaker(mass, arr_nBins, isFirst=False, isLast=False):
    global pDir
    global outDir_nm

    outFile = ""
    for low,high in zip(arr_nBins[0:-1],arr_nBins[1:]):

        str_low,str_high = str(low),str(high)

        if( isFirst ):
            outFile_nm = "run_combineMakerMonoHgg.sh"
            outFile = open(outDir_nm+"/"+outFile_nm, "w")
            outFile.write( "#!/bin/bash \n\n" )
            outFile.write( "#run_combinMakerMonoHgg.sh \n\n" )
            outFile.write( "set -x \n\n" )
            outFile.write( "mainDir=$1 && shift\n\n" )
            outFile.write( "cp -r $mainDir ntuples4fit_dnnBins_binEdge"+str_low+"_binEdge"+str_high+"\n" )
            outFile.write( './combine_maker_MonoHgg.sh "ntuples4fit_dnnBins_binEdge'+str_low+'_binEdge'+str_high+'" "templates_maker_MonoHgg_mA'+str(mass)+'_dnnBins_binEdge'+str_low+'_binEdge'+str_high+'.json" "templates_maker_prepare_MonoHgg_mA'+str(mass)+'_dnnBins_binEdge'+str_low+'_binEdge'+str_high+'.json" "'+str(mass)+'" --lumi 59.69 --fit-name cic --mc-file Output_MC.root --fit-background --redo-input \n')
            isFirst = False
            pDir = "ntuples4fit_dnnBins_binEdge"+str_low+"_binEdge"+str_high

        else:
            outFile_nm = "run_combineMakerMonoHgg.sh"
            outFile = open(outDir_nm+"/"+outFile_nm, "a")
            outFile.write( "mkdir -p ntuples4fit_dnnBins_binEdge"+str_low+"_binEdge"+str_high+"\n" )
            outFile.write( "mv "+pDir+"/Output_*.root ntuples4fit_dnnBins_binEdge"+str_low+"_binEdge"+str_high+"/ \n" )
            outFile.write( './combine_maker_MonoHgg.sh "ntuples4fit_dnnBins_binEdge'+str_low+'_binEdge'+str_high+'" "templates_maker_MonoHgg_mA'+str(mass)+'_dnnBins_binEdge'+str_low+'_binEdge'+str_high+'.json" "templates_maker_prepare_MonoHgg_mA'+str(mass)+'_dnnBins_binEdge'+str_low+'_binEdge'+str_high+'.json" "'+str(mass)+'" --lumi 59.69 --fit-name cic --mc-file Output_MC.root --fit-background --redo-input \n')
            pDir = "ntuples4fit_dnnBins_binEdge"+str_low+"_binEdge"+str_high

    outFile.write("\n\n")
    if( isLast ):
        outFile.write( "rm -r ntuples4fit_dnnBins_binEdge"+str_low+"_binEdge"+str_high+"/Output_*.root \n\n" )
        outFile.write( "set +x" )

    outFile.close()


def runMyCombineAllMonoHiggs2HDMaMaker(arr_nBins):
    global outDir_nm

    outFile = ""
    for low,high in zip(arr_nBins[0:-1],arr_nBins[1:]):

        str_low,str_high = str(low),str(high)

        if( arr_nBins.index(low)==0 ):
            outFile_nm = "run_myCombineAllMonoHiggs2HDMa.sh"
            outFile = open(outDir_nm+"/"+outFile_nm, "w")
            outFile.write( "#!/bin/bash \n\n" )
            outFile.write( "#run_myCombineAllMonoHiggs2HDMa.sh \n\n")
            outFile.write( "set -x \n\n" )
            outFile.write( './mycombineall_MonoHgg2HDMa.sh "ntuples4fit_dnnBins_binEdge'+str_low+'_binEdge'+str_high+'_cic_default_shapes_lumi_59.69" --hadd --model 2HDMa -C 0.95 -M AsymptoticLimits --run both \n')
            isFirst = False

        else:
            outFile_nm = "run_myCombineAllMonoHiggs2HDMa.sh"
            outFile = open(outDir_nm+"/"+outFile_nm, "a")
            outFile.write( './mycombineall_MonoHgg2HDMa.sh "ntuples4fit_dnnBins_binEdge'+str_low+'_binEdge'+str_high+'_cic_default_shapes_lumi_59.69" --hadd --model 2HDMa -C 0.95 -M AsymptoticLimits --run both \n')

    outFile.write("\n\n")
    outFile.write( "set +x" )
    outFile.close()



def runLimitPlotsMonoHggMaker(arr_nBins):
    global outDir_nm

    outFile = ""
    for low,high in zip(arr_nBins[0:-1],arr_nBins[1:]):

        str_low,str_high = str(low),str(high)

        if( arr_nBins.index(low)==0 ):
            outFile_nm = "run_limitPlotsMonoHgg.sh"
            outFile = open(outDir_nm+"/"+outFile_nm, "w")
            outFile.write( "#!/bin/bash \n\n" )
            outFile.write( "#run_limitPlotsMonoHgg.sh \n\n" )
            outFile.write( "set -x \n\n" )
            outFile.write( './mylimit_plots_MonoHgg.sh "ntuples4fit_dnnBins_binEdge'+str_low+'_binEdge'+str_high+'_cic_default_shapes_lumi_59.69" "_dnnBins_binEdge'+str_low+'_binEdge'+str_high+'" \n')
            isFirst = False

        else:
            outFile_nm = "run_limitPlotsMonoHgg.sh"
            outFile = open(outDir_nm+"/"+outFile_nm, "a")
            outFile.write( './mylimit_plots_MonoHgg.sh "ntuples4fit_dnnBins_binEdge'+str_low+'_binEdge'+str_high+'_cic_default_shapes_lumi_59.69" "_dnnBins_binEdge'+str_low+'_binEdge'+str_high+'" \n')

    outFile.write("\n\n")
    outFile.write( "set +x" )
    outFile.close()



def runCombineCardsMaker(mass, arr_nBins, isFirst=False, isLast=False):
    global outDir_nm

    outFile = ""
    for low,high in zip(arr_nBins[0:-1],arr_nBins[1:]):

        str_low,str_high = str(low),str(high)

        if( isFirst ):
            outFile_nm = "combineCards.sh"
            outFile = open(outDir_nm+"/"+outFile_nm, "w")
            outFile.write( "#!/bin/bash \n\n" )
            outFile.write( "#combineCards.sh \n\n" )
            outFile.write( "set -x \n\n" )
            outFile.write( "inDir=$1 && shift \n\n" )
            outFile.write( "#mA="+str(mass)+"\n" )
            outFile.write( "combineCards.py of"+str_low+"j=$inDir/ntuples4fit_dnnBins_binEdge"+str_low+"_binEdge"+str_high+"_cic_default_shapes_lumi_59.69/dataCard_sig_2HDMa_mA"+str(mass)+"_ma150_13TeV.txt \\")
            isFirst = False

        else:
            outFile_nm = "combineCards.sh"
            outFile = open(outDir_nm+"/"+outFile_nm, "a")
            if(low == 0):
                outFile.write( "#mA="+str(mass)+"\n" )
                outFile.write( "combineCards.py of"+str_low+"j=$inDir/ntuples4fit_dnnBins_binEdge"+str_low+"_binEdge"+str_high+"_cic_default_shapes_lumi_59.69/dataCard_sig_2HDMa_mA"+str(mass)+"_ma150_13TeV.txt \\")
            else:
                outFile.write( "\n                of"+str_low+"j=$inDir/ntuples4fit_dnnBins_binEdge"+str_low+"_binEdge"+str_high+"_cic_default_shapes_lumi_59.69/dataCard_sig_2HDMa_mA"+str(mass)+"_ma150_13TeV.txt \\")

        if( high==arr_nBins[-1] ):
            outFile.write("\n                > dataCard_sig_2HDMa_mA"+str(mass)+"_ma150_13TeV.txt \n")
            outFile.write("echo \n")
            outFile.write("echo \n\n")

        if( isLast and high==arr_nBins[-1] ):
            outFile.write("set +x")

    outFile.write("\n\n")


    outFile.close()


for key in dict_binEdge:
    print key,dict_binEdge[key]
    print "mass = ", key

    if( key == 200 ):
        runCombineMakerMonoHggMaker(key, range(len(dict_binEdge[key])), True, False)
        runCombineCardsMaker(key, range(len(dict_binEdge[key])), True, False)
    elif( key == 900 ):
        runCombineMakerMonoHggMaker(key, range(len(dict_binEdge[key])), False, True)
        runCombineCardsMaker(key, range(len(dict_binEdge[key])), False, True )
    else:
        runCombineMakerMonoHggMaker(key, range(len(dict_binEdge[key])) )
        runCombineCardsMaker(key, range(len(dict_binEdge[key])) )

    runMyCombineAllMonoHiggs2HDMaMaker( range(len(dict_binEdge[200])) )
    runLimitPlotsMonoHggMaker( range(len(dict_binEdge[200])) )

    print ""
