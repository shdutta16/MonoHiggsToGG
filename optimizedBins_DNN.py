
#effVsDNNplotter.py

import sys
import os
import numpy as np


# Function to calculate significance for given list of cuts and return
# significance vs cuts
def signifiCalc( sigData, bkgData, cut, debug=False):     # cut is the value of the preceding binEdge upto which scan needs to be done
    cut_lastIndex = np.where( sigData[:,0] == cut )[0][0] # get the index corresponding to cut
    if(debug):
        print "cut = ", cut, "\tcut_lastIndex = ", cut_lastIndex
        print ""

    sigTotal = sigData[0,1] # total no. of sig events
    bkgTotal = bkgData[0,1] # total no. of bkg events

    cuts = sigData[:cut_lastIndex,0]  # get all the DNN score values upto cut

    arr_signifi = np.zeros(len(cuts)-1, dtype=[('cut','f8'),('signifi','f8')]) # array to store cut and signigicance
    index=0
    for icut in cuts[1:]:    # loop over all DNN score upto cut starting from the second entry
        cut_index = np.where( sigData[:,0] == icut )[0][0]      # get the index of ith DNN score cut

        s_1 = sigTotal - sigData[cut_index,1]     # sig events between 0 and ith cut
        b_1 = bkgTotal - bkgData[cut_index,1]     # bkg events between 0 and ith cut
        if( s_1 == 0.0 or b_1 == 0.0 ): continue  # if either sig or bkg is 0 continue
        #signifi_1 = s_1/np.sqrt(s_1+b_1)
        signifi_1 = np.sqrt(2.*( (s_1+b_1)*np.log(1.+ (s_1/b_1)) - s_1) )

        s_2 = sigTotal-sigData[cut_lastIndex,1]-s_1  # sig events between ith cut and cut (preceding binEdge)
        b_2 = bkgTotal-bkgData[cut_lastIndex,1]-b_1  # bkg events between ith cut and cut (preceding binEdge)
        if( s_2 == 0.0 or b_2 == 0.0 ): continue     # if either sig or bkg is 0 continue
        #signifi_2 = s_2/np.sqrt(s_2+b_2)
        signifi_2 = np.sqrt(2.*( (s_2+b_2)*np.log(1.+ (s_2/b_2)) - s_2) )

        signifi = np.sqrt( signifi_1*signifi_1 + signifi_2*signifi_2 )    # total significance

        if(debug):
            print "cut = ",icut, "\tcut_index = ",cut_index
            print "s_1 = ",s_1
            print "b_1 = ",b_1
            print "signifi_1 = ",signifi_1
            print "s_2 = ",s_2
            print "b_2 = ",b_2
            print "signifi_2 = ",signifi_2
            print "cut = ",icut,"\tsignificance = ",signifi
            print ""

        arr_signifi[index] = (icut,signifi)   # store ith cut and corresponding significance value in array
        index += 1

    return arr_signifi   # return significance vs. DNN score cut array where the cut is upto previous binEdge




inDir  = "./DNN_efficiency_2/"    # directory with efficiency vs. DNN score files
outDir = "./DNN_efficiency_2/"

if( not os.path.isdir(outDir) ):
    os.mkdir( outDir )

l_mA = [200, 300, 400, 500, 600]
#l_mA = [200]


for imA in l_mA:
    sigFile = inDir+"Efficiency_vs_DNNscore_sig_nTuple_2017_2HDMa_mA"+str(imA)+"_ma150_EOY.txt"
    bkgFile = inDir+"Efficiency_vs_DNNscore_bkg_FailingPhotonID_2017C_88percent.txt"

    sigData = np.loadtxt( sigFile )   # load sig data
    bkgData = np.loadtxt( bkgFile )   # load bkg data

    arr_binEdge = np.zeros(0, dtype='f8')  # initialize empty array to store binEdges
    arr_signifi = np.zeros(0, dtype='f8')  # initialize empty array to store significance

    binEdge = sigData[-1,0]    # initialize binEdge with last value of DNN score
    significance = 0.0

    print "mA = ",imA

    for i in range(10):
        cuts_signifi = signifiCalc( sigData, bkgData, binEdge )      # call function to calculate significance vs. cuts
        binEdge = cuts_signifi['cut'][ cuts_signifi['signifi']==max(cuts_signifi['signifi']) ][0]           # get the cut where significance is maximum
        maxSignifi = cuts_signifi['signifi'][ cuts_signifi['signifi']==max(cuts_signifi['signifi']) ][0]     # get the max significance value
        significance += maxSignifi * maxSignifi
        print "i = ",i,"\tbinEdge = ",binEdge,"\tsignificance = ", np.sqrt(significance)
        if( binEdge == sigData[1,0] ):    # if the binEdge equals the first DNN score cut then break
            break

        arr_binEdge = np.append(arr_binEdge, binEdge)      # add the calculated binEdge to array
        arr_signifi = np.append(arr_signifi, np.sqrt(significance) ) # add the significance to the array

        if( i==0 ):     # i=0 is the first scan so plot significance vs. DNN score for entire range
            nm_file = outDir+"significanceVsDNNscore_mA"+str(imA)+".txt"
            np.savetxt( nm_file, cuts_signifi[:-1] )

    # to make significance vs. # bin plot
    arr_signifi = np.hstack( (np.arange(len(arr_signifi)).reshape(len(arr_signifi),1),arr_signifi.reshape(len(arr_signifi),1)))

    nm_file = outDir+"significanceVsNBin_mA"+str(imA)+".txt"
    np.savetxt(nm_file, arr_signifi)

    nm_file = outDir+"binEdge_mA"+str(imA)+".txt"
    np.savetxt(nm_file, arr_binEdge)

    print ""



