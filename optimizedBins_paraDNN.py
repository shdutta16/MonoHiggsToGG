#effVsParaDNNplotter.py

import numpy as np


# Function to calculate significance for given list of cuts and return
# significance vs cuts
def signifiCalc( sigData, bkgData, cut, debug=False):
    cut_lastIndex = np.where( sigData[:,0] == cut )[0][0]
    if(debug):
        print "cut = ", cut, "\tcut_lastIndex = ", cut_lastIndex
        print ""

    sigTotal = sigData[0,1]
    bkgTotal = bkgData[0,1]

    cuts = sigData[:cut_lastIndex,0]

    arr_signifi = np.zeros(len(cuts)-1, dtype=[('cut','f8'),('signifi','f8')])
    index=0
    for icut in cuts[1:]:
        cut_index = np.where( sigData[:,0] == icut )[0][0]

        s_1 = sigTotal - sigData[cut_index,1]
        b_1 = bkgTotal - bkgData[cut_index,1]
        if( s_1 == 0.0 or b_1 == 0.0 ): continue
        #signifi_1 = s_1/np.sqrt(s_1+b_1)
        signifi_1 = np.sqrt(2.*( (s_1+b_1)*np.log(1.+ (s_1/b_1)) - s_1) )


        s_2 = sigTotal-sigData[cut_lastIndex,1]-s_1
        b_2 = bkgTotal-bkgData[cut_lastIndex,1]-b_1
        if( s_2 == 0.0 or b_2 == 0.0 ): continue
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

        arr_signifi[index] = (icut,signifi)
        index += 1

    return arr_signifi




inDir  = "./paraDNN_efficiency_2/"
outDir = "./paraDNN_efficiency_2/"

l_mA = [200, 300, 400, 500, 600]


for imA in l_mA:
    sigFile = inDir+"Efficiency_vs_DNNscore_sig_mA_"+str(imA)+"p0_nTuple_2017_2HDMa_mA"+str(imA)+"_ma150_EOY.txt"
    bkgFile = inDir+"Efficiency_vs_DNNscore_bkg_mA_"+str(imA)+"p0_FailingPhotonID_2017C_88percent.txt"

    sigData = np.loadtxt( sigFile )
    bkgData = np.loadtxt( bkgFile )

    arr_binEdge = np.zeros(0, dtype='f8')
    arr_signifi = np.zeros(0, dtype='f8')

    binEdge = sigData[-1,0]
    significance = 0.0

    print "mA = ",imA

    for i in range(10):
        cuts_signifi = signifiCalc( sigData, bkgData, binEdge )
        binEdge = cuts_signifi['cut'][ cuts_signifi['signifi']==max(cuts_signifi['signifi']) ][0]
        maxSignifi = cuts_signifi['signifi'][ cuts_signifi['signifi']==max(cuts_signifi['signifi']) ]     # get the max significance value
        significance += maxSignifi * maxSignifi
        print "i = ",i,"\tbinEdge = ",binEdge,"\tsignificance = ",np.sqrt(significance)
        if( binEdge == sigData[1,0] ):
            break

        arr_binEdge = np.append(arr_binEdge, binEdge)
        arr_signifi = np.append(arr_signifi, np.sqrt(significance) )

        if( i==0 ):
            nm_file = outDir+"significanceVsDNNscore_mA"+str(imA)+".txt"
            np.savetxt( nm_file, cuts_signifi[:-1] )


    arr_signifi = np.hstack( (np.arange(len(arr_signifi)).reshape(len(arr_signifi),1),arr_signifi.reshape(len(arr_signifi),1)))

    nm_file = outDir+"significanceVsNBin_mA"+str(imA)+".txt"
    np.savetxt(nm_file, arr_signifi)

    nm_file = outDir+"binEdge_mA"+str(imA)+".txt"
    np.savetxt(nm_file, arr_binEdge)

    print ""



