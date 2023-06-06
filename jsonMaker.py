#jsonMaker.py

import sys
import os
from collections import OrderedDict

dict_binEdge = OrderedDict()

################################################################################
## FOR SIMPLE DNN
################################################################################
#dict_binEdge[200]=[0.39,0.75,0.93,0.98,0.99378,0.99835,1]
#dict_binEdge[300]=[0.27,0.45,0.71,0.91,0.98,0.99183,1]
#dict_binEdge[400]=[0.82,0.89,0.98,0.99285,0.99751,0.99885,1]
#dict_binEdge[500]=[0.95,0.98,0.99228,0.99581,0.99845,0.99964,1]
#dict_binEdge[600]=[0.98,0.99285,0.99733,0.99846,0.99964,0.99996,1]
################################################################################


################################################################################
## FOR PARAMETRIC DNN
################################################################################
#dict_binEdge[200]=[0.3,0.56,0.8,0.98,0.99176,0.9964,1]
#dict_binEdge[300]=[0.2,0.4,0.69,0.89,0.97,0.99073,1]
#dict_binEdge[400]=[0.75,0.91,0.98,0.99004,0.99513,0.99786,1]
#dict_binEdge[500]=[0.86,0.94,0.99086,0.99595,0.99794,0.9991,1]
#dict_binEdge[600]=[0.93,0.98,0.99442,0.99799,0.99928,0.9999,1]
################################################################################



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
# FOR PARAMETRIC DNN (all bins)
########################################################################################
dict_binEdge[200]=[0,0.06,0.1,0.18,0.3,0.56,0.8,0.93,0.98,0.99176,0.9964,1]
dict_binEdge[300]=[0,0.02,0.03,0.06,0.13,0.2,0.4,0.69,0.89,0.97,0.99073,1]
dict_binEdge[400]=[0,0.06,0.2,0.31,0.56,0.75,0.91,0.98,0.99004,0.99513,0.99786,1]
dict_binEdge[500]=[0,0.08,0.18,0.24,0.57,0.86,0.94,0.99086,0.99595,0.99794,0.9991,1]
dict_binEdge[600]=[0,0.07,0.18,0.41,0.78,0.93,0.98,0.99442,0.99799,0.99928,0.9999,1]
########################################################################################



#dict_binEdge[200]=[0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.90,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.990,0.991,0.992,0.993,0.994,0.995,0.996,0.997,0.998,0.999,1]
#dict_binEdge[300]=[0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.90,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.990,0.991,0.992,0.993,0.994,0.995,0.996,0.997,0.998,0.999,1]
#dict_binEdge[400]=[0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.90,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.990,0.991,0.992,0.993,0.994,0.995,0.996,0.997,0.998,0.999,1]
#dict_binEdge[500]=[0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.90,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.990,0.991,0.992,0.993,0.994,0.995,0.996,0.997,0.998,0.999,1]
#dict_binEdge[600]=[0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.90,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.990,0.991,0.992,0.993,0.994,0.995,0.996,0.997,0.998,0.999,1]


outDir_nm = "./jsonFiles_paraDNN"

if( not os.path.isdir(outDir_nm) ):
    os.mkdir( outDir_nm )




def makeTemplatesMaker(mass, binEdge):
    for lowEdge,highEdge in zip(binEdge[0:-1],binEdge[1:]):
        #print "low = ", lowEdge, "\thigh = ", highEdge
        low  = str(lowEdge).replace(".","p")
        high = str(highEdge).replace(".","p")
        lowIdx  = str( binEdge.index(lowEdge) )
        highIdx = str( binEdge.index(highEdge) )
        #print "low = ", low, "\thigh = ", high

        nm_outFile = "templates_maker_MonoHgg_mA"+str(mass)+"_dnnBins_binEdge"+lowIdx+"_binEdge"+highIdx+".json"
        print nm_outFile

        templateFile = open("templates_maker_MonoHgg_DNN_original.json","r")
        outFile = open(outDir_nm+"/"+nm_outFile, "w")

        for line in templateFile.readlines():
            if (line.find("mA") != -1):
                print line.replace("mA","mA"+str(mass))
                outFile.write( line.replace("mA","mA"+str(mass)) )

            elif (line.find("low") != -1):
                line = line.replace("low",low)
                line = line.replace("high",high)
                print line
                outFile.write( line )

            else:
                outFile.write( line )


def makeTemplatesMakerPrepare(mass, binEdge):
    for lowEdge,highEdge in zip(binEdge[0:-1],binEdge[1:]):
        #print "low = ", lowEdge, "\thigh = ", highEdge
        low  = str(lowEdge).replace(".","p")
        high = str(highEdge).replace(".","p")
        lowIdx  = str( binEdge.index(lowEdge) )
        highIdx = str( binEdge.index(highEdge) )
        #print "low = ", low, "\thigh = ", high

        nm_outFile = "templates_maker_prepare_MonoHgg_mA"+str(mass)+"_dnnBins_binEdge"+lowIdx+"_binEdge"+highIdx+".json"
        print nm_outFile

        templateFile = open("templates_maker_prepare_MonoHgg_dnnBins_original.json","r")
        outFile = open(outDir_nm+"/"+nm_outFile, "w")

        for line in templateFile.readlines():
            if (line.find("low") != -1):
                line = line.replace("low",low)
                line = line.replace("high",high)
                print line
                outFile.write( line )

            else:
                outFile.write( line )




for key in dict_binEdge:
    print key,dict_binEdge[key]
    print "mass = ", key
    makeTemplatesMaker(key, dict_binEdge[key])
    makeTemplatesMakerPrepare(key, dict_binEdge[key])
    print ""
