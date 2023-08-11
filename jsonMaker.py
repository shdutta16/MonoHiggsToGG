#jsonMaker.py

import sys
import os
from collections import OrderedDict

dict_binEdge = OrderedDict()

                                                                                      

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
