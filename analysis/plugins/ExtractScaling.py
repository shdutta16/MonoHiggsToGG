infile = open('scaling.dat') 		 # open smearing file
target = open('cleaned_scaling.dat','w') # output file

for line in infile:
    string = line.split("\t")
    strerr = string[5].split(" ") 
  
    # split by low and high R9 
    if ("highR9" in string[0]):
       r9 = "hiR9"
    else:
       r9 = "loR9"

    # split by eta range 
    if ("absEta_0_1" in string[0]):
        eta_lo = 0.0
        eta_hi = 1.0
    elif ("absEta_1_1.4442" in string[0]):
        eta_lo = 1.0
        eta_hi = 1.4442
    elif ("absEta_1.566_2" in string[0]):
        eta_lo = 1.566
        eta_hi = 2.0
    elif ("absEta_2_2.5" in string[0]):
        eta_lo = 2.0
        eta_hi = 2.5

    # write in the correct format for the analyzer
    if ("absEta_0_1" in string[0]):
        target.write('if( fabs(sceta) <= '+str(eta_hi)+' && '+r9+' && runNumber >= '+string[2]+' && runNumber <= '+string[3]+') scalingValue = '+string[4]+'; \n')
        target.write('if( fabs(sceta) <= '+str(eta_hi)+' && '+r9+' && runNumber >= '+string[2]+' && runNumber <= '+string[3]+') scalingError = '+strerr[0]+'; \n')
    else:    
        target.write('if( fabs(sceta) > '+str(eta_lo)+' && fabs(sceta) <= '+str(eta_hi)+' && '+r9+' && runNumber >= '+string[2]+' && runNumber <= '+string[3]+') scalingValue = '+string[4]+'; \n')
        target.write('if( fabs(sceta) > '+str(eta_lo)+' && fabs(sceta) <= '+str(eta_hi)+' && '+r9+' && runNumber >= '+string[2]+' && runNumber <= '+string[3]+') scalingError = '+strerr[0]+'; \n')


target.close()
