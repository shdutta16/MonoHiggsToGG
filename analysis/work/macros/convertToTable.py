import sys

def AllGlobal(fname):
  fin = open(fname,"r")
  fout = open(fname[:-4]+"_clean.tex","w")

  eff2hdm600 = ["-"]*6
  eff2hdm800 = ["-"]*6
  eff2hdm1000 = ["-"]*6
  eff2hdm1200 = ["-"]*6
  eff2hdm1400 = ["-"]*6
  eff2hdm1700 = ["-"]*6
  eff2hdm2000 = ["-"]*6
  eff2hdm2500 = ["-"]*6

  effbary10000 = ["-"]*6
  effbary2000 = ["-"]*6
  effbary1995 = ["-"]*6
  effbary1000 = ["-"]*6
  effbary995 = ["-"]*6
  effbary500 = ["-"]*6
  effbary300 = ["-"]*6
  effbary295 = ["-"]*6
  effbary200 = ["-"]*6
  effbary100 = ["-"]*6
  effbary50 = ["-"]*6
  effbary20 = ["-"]*6
  effbary15 = ["-"]*6
  effbary10 = ["-"]*6

  effscal10000 = ["-"]*6
  effscal1000 = ["-"]*6
  effscal995 = ["-"]*6
  effscal500 = ["-"]*6
  effscal300 = ["-"]*6
  effscal295 = ["-"]*6
  effscal200 = ["-"]*6
  effscal100 = ["-"]*6
  effscal95 = ["-"]*6  
  effscal50 = ["-"]*6
  effscal20 = ["-"]*6
  effscal15 = ["-"]*6
  effscal10 = ["-"]*6

  for line in fin:
    line = line.strip()
    line = line.replace('\\\\','')
    string = line.split(" & ")

    if "2HDM" in string[0]: #separate 2hdm
       if "mZP600" in string[0]: #mZP600
           if "mA0300" in string[0]:
             eff2hdm600[0]=string[1]
           if "mA0400" in string[0]:
             eff2hdm600[1]=string[1]
       if "mZP800" in string[0]: #mZP800
           if "mA0300" in string[0]:
             eff2hdm800[0]=string[1]
           if "mA0400" in string[0]:
             eff2hdm800[1]=string[1]
           if "mA0500" in string[0]:
             eff2hdm800[2]=string[1]
           if "mA0600" in string[0]:
             eff2hdm800[3]=string[1]
       if "mZP1000" in string[0]: #mZP1000
           if "mA0300" in string[0]:
             eff2hdm1000[0]=string[1]
           if "mA0400" in string[0]:
             eff2hdm1000[1]=string[1]
           if "mA0500" in string[0]:
             eff2hdm1000[2]=string[1]
           if "mA0600" in string[0]:
             eff2hdm1000[3]=string[1]
           if "mA0700" in string[0]:
             eff2hdm1000[4]=string[1]
           if "mA0800" in string[0]:
             eff2hdm1000[5]=string[1]
       if "mZP1200" in string[0]: #mZP1200
           if "mA0300" in string[0]:
             eff2hdm1200[0]=string[1]
           if "mA0400" in string[0]:
             eff2hdm1200[1]=string[1]
           if "mA0500" in string[0]:
             eff2hdm1200[2]=string[1]
           if "mA0600" in string[0]:
             eff2hdm1200[3]=string[1]
           if "mA0700" in string[0]:
             eff2hdm1200[4]=string[1]
           if "mA0800" in string[0]:
             eff2hdm1200[5]=string[1]
       if "mZP1400" in string[0]: #mZP1400
           if "mA0300" in string[0]:
             eff2hdm1400[0]=string[1]
           if "mA0400" in string[0]:
             eff2hdm1400[1]=string[1]
           if "mA0500" in string[0]:
             eff2hdm1400[2]=string[1]
           if "mA0600" in string[0]:
             eff2hdm1400[3]=string[1]
           if "mA0700" in string[0]:
             eff2hdm1400[4]=string[1]
           if "mA0800" in string[0]:
             eff2hdm1400[5]=string[1]
       if "mZP1700" in string[0]: #mZP1700
           if "mA0300" in string[0]:
             eff2hdm1700[0]=string[1]
           if "mA0400" in string[0]:
             eff2hdm1700[1]=string[1]
           if "mA0500" in string[0]:
             eff2hdm1700[2]=string[1]
           if "mA0600" in string[0]:
             eff2hdm1700[3]=string[1]
           if "mA0700" in string[0]:
             eff2hdm1700[4]=string[1]
           if "mA0800" in string[0]:
             eff2hdm1700[5]=string[1]
       if "mZP2000" in string[0]: #mZP2000
           if "mA0300" in string[0]:
             eff2hdm2000[0]=string[1]
           if "mA0400" in string[0]:
             eff2hdm2000[1]=string[1]
           if "mA0500" in string[0]:
             eff2hdm2000[2]=string[1]
           if "mA0600" in string[0]:
             eff2hdm2000[3]=string[1]
           if "mA0700" in string[0]:
             eff2hdm2000[4]=string[1]
           if "mA0800" in string[0]:
             eff2hdm2000[5]=string[1]
       if "mZP2500" in string[0]: #mZP2500
           if "mA0300" in string[0]:
             eff2hdm2500[0]=string[1]
           if "mA0400" in string[0]:
             eff2hdm2500[1]=string[1]
           if "mA0500" in string[0]:
             eff2hdm2500[2]=string[1]
           if "mA0600" in string[0]:
             eff2hdm2500[3]=string[1]
           if "mA0700" in string[0]:
             eff2hdm2500[4]=string[1]
           if "mA0800" in string[0]:
             eff2hdm2500[5]=string[1]

    elif "Baryonic" in string[0]:
       if "mZP10000" in string[0]: #mZP10000
           if "mChi1000" in string[0]:
             effbary10000[5]=string[1]
           elif "mChi500" in string[0]:
             effbary10000[4]=string[1]
           elif "mChi150" in string[0]:
             effbary10000[3]=string[1]
           elif "mChi50" in string[0]:
             effbary10000[2]=string[1]
           elif "mChi10" in string[0]:
             effbary10000[1]=string[1]
           elif "mChi1" in string[0]:
             effbary10000[0]=string[1]
       elif "mZP2000" in string[0]: #mZP2000
           if "mChi1000" in string[0]:
             effbary2000[5]=string[1]
           elif "mChi500" in string[0]:
             effbary2000[4]=string[1]
           elif "mChi150" in string[0]:
             effbary2000[3]=string[1]
           elif "mChi50" in string[0]:
             effbary2000[2]=string[1]
           elif "mChi10" in string[0]:
             effbary2000[1]=string[1]
           elif "mChi1" in string[0]:
             effbary2000[0]=string[1]
       elif "mZP1995" in string[0]: #mZP1995
           if "mChi1000" in string[0]:
             effbary1995[5]=string[1]
           elif "mChi500" in string[0]:
             effbary1995[4]=string[1]
           elif "mChi150" in string[0]:
             effbary1995[3]=string[1]
           elif "mChi50" in string[0]:
             effbary1995[2]=string[1]
           elif "mChi10" in string[0]:
             effbary1995[1]=string[1]
           elif "mChi1" in string[0]:
             effbary1995[0]=string[1]
       elif "mZP1000" in string[0]: #mZP1000
           if "mChi1000" in string[0]:
             effbary1000[5]=string[1]
           elif "mChi500" in string[0]:
             effbary1000[4]=string[1]
           elif "mChi150" in string[0]:
             effbary1000[3]=string[1]
           elif "mChi50" in string[0]:
             effbary1000[2]=string[1]
           elif "mChi10" in string[0]:
             effbary1000[1]=string[1]
           elif "mChi1" in string[0]:
             effbary1000[0]=string[1]
       elif "mZP995" in string[0]: #mZP995
           if "mChi1000" in string[0]:
             effbary995[5]=string[1]
           elif "mChi500" in string[0]:
             effbary995[4]=string[1]
           elif "mChi150" in string[0]:
             effbary995[3]=string[1]
           elif "mChi50" in string[0]:
             effbary995[2]=string[1]
           elif "mChi10" in string[0]:
             effbary995[1]=string[1]
           elif "mChi1" in string[0]:
             effbary995[0]=string[1]
       elif "mZP500" in string[0]: #mZP500
           if "mChi1000" in string[0]:
             effbary500[5]=string[1]
           elif "mChi500" in string[0]:
             effbary500[4]=string[1]
           elif "mChi150" in string[0]:
             effbary500[3]=string[1]
           elif "mChi50" in string[0]:
             effbary500[2]=string[1]
           elif "mChi10" in string[0]:
             effbary500[1]=string[1]
           elif "mChi1" in string[0]:
             effbary500[0]=string[1]
       elif "mZP300" in string[0]: #mZP300
           if "mChi1000" in string[0]:
             effbary300[5]=string[1]
           elif "mChi500" in string[0]:
             effbary300[4]=string[1]
           elif "mChi150" in string[0]:
             effbary300[3]=string[1]
           elif "mChi50" in string[0]:
             effbary300[2]=string[1]
           elif "mChi10" in string[0]:
             effbary300[1]=string[1]
           elif "mChi1" in string[0]:
             effbary300[0]=string[1]
       elif "mZP295" in string[0]: #mZP295
           if "mChi1000" in string[0]:
             effbary295[5]=string[1]
           elif "mChi500" in string[0]:
             effbary295[4]=string[1]
           elif "mChi150" in string[0]:
             effbary295[3]=string[1]
           elif "mChi50" in string[0]:
             effbary295[2]=string[1]
           elif "mChi10" in string[0]:
             effbary295[1]=string[1]
           elif "mChi1" in string[0]:
             effbary295[0]=string[1]
       elif "mZP200" in string[0]: #mZP200
           if "mChi1000" in string[0]:
             effbary200[5]=string[1]
           elif "mChi500" in string[0]:
             effbary200[4]=string[1]
           elif "mChi150" in string[0]:
             effbary200[3]=string[1]
           elif "mChi50" in string[0]:
             effbary200[2]=string[1]
           elif "mChi10" in string[0]:
             effbary200[1]=string[1]
           elif "mChi1" in string[0]:
             effbary200[0]=string[1]
       elif "mZP100" in string[0]: #mZP100
           if "mChi1000" in string[0]:
             effbary100[5]=string[1]
           elif "mChi500" in string[0]:
             effbary100[4]=string[1]
           elif "mChi150" in string[0]:
             effbary100[3]=string[1]
           elif "mChi50" in string[0]:
             effbary100[2]=string[1]
           elif "mChi10" in string[0]:
             effbary100[1]=string[1]
           elif "mChi1" in string[0]:
             effbary100[0]=string[1]
       elif "mZP50" in string[0]: #mZP50
           if "mChi1000" in string[0]:
             effbary50[5]=string[1]
           elif "mChi500" in string[0]:
             effbary50[4]=string[1]
           elif "mChi150" in string[0]:
             effbary50[3]=string[1]
           elif "mChi50" in string[0]:
             effbary50[2]=string[1]
           elif "mChi10" in string[0]:
             effbary50[1]=string[1]
           elif "mChi1" in string[0]:
             effbary50[0]=string[1]
       elif "mZP20" in string[0]: #mZP20
           if "mChi1000" in string[0]:
             effbary20[5]=string[1]
           elif "mChi500" in string[0]:
             effbary20[4]=string[1]
           elif "mChi150" in string[0]:
             effbary20[3]=string[1]
           elif "mChi50" in string[0]:
             effbary20[2]=string[1]
           elif "mChi10" in string[0]:
             effbary20[1]=string[1]
           elif "mChi1" in string[0]:
             effbary20[0]=string[1]
       elif "mZP15" in string[0]: #mZP15
           if "mChi1000" in string[0]:
             effbary15[5]=string[1]
           elif "mChi500" in string[0]:
             effbary15[4]=string[1]
           elif "mChi150" in string[0]:
             effbary15[3]=string[1]
           elif "mChi50" in string[0]:
             effbary15[2]=string[1]
           elif "mChi10" in string[0]:
             effbary15[1]=string[1]
           elif "mChi1" in string[0]:
             effbary100[0]=string[1]
       elif "mZP10" in string[0]: #mZP10
           if "mChi1000" in string[0]:
             effbary10[5]=string[1]
           elif "mChi500" in string[0]:
             effbary10[4]=string[1]
           elif "mChi150" in string[0]:
             effbary10[3]=string[1]
           elif "mChi50" in string[0]:
             effbary10[2]=string[1]
           elif "mChi10" in string[0]:
             effbary10[1]=string[1]
           elif "mChi1" in string[0]:
             effbary10[0]=string[1]
            
    elif "Scalar" in string[0]:
       if "mZP10000" in string[0]: #mZP10000
           if "mChi1000" in string[0]:
             effscal10000[5]=string[1]
           elif "mChi500" in string[0]:
             effscal10000[4]=string[1]
           elif "mChi150" in string[0]:
             effscal10000[3]=string[1]
           elif "mChi50" in string[0]:
             effscal10000[2]=string[1]
           elif "mChi10" in string[0]:
             effscal10000[1]=string[1]
           elif "mChi1" in string[0]:
             effscal10000[0]=string[1]
       elif "mZP1000" in string[0]: #mZP1000
           if "mChi1000" in string[0]:
             effscal1000[5]=string[1]
           elif "mChi500" in string[0]:
             effscal1000[4]=string[1]
           elif "mChi150" in string[0]:
             effscal1000[3]=string[1]
           elif "mChi50" in string[0]:
             effscal1000[2]=string[1]
           elif "mChi10" in string[0]:
             effscal1000[1]=string[1]
           elif "mChi1" in string[0]:
             effscal1000[0]=string[1]
       elif "mZP995" in string[0]: #mZP995
           if "mChi1000" in string[0]:
             effscal995[5]=string[1]
           elif "mChi500" in string[0]:
             effscal995[4]=string[1]
           elif "mChi150" in string[0]:
             effscal995[3]=string[1]
           elif "mChi50" in string[0]:
             effscal995[2]=string[1]
           elif "mChi10" in string[0]:
             effscal995[1]=string[1]
           elif "mChi1" in string[0]:
             effscal995[0]=string[1]
       elif "mZP500" in string[0]: #mZP500
           if "mChi1000" in string[0]:
             effscal500[5]=string[1]
           elif "mChi500" in string[0]:
             effscal500[4]=string[1]
           elif "mChi150" in string[0]:
             effscal500[3]=string[1]
           elif "mChi50" in string[0]:
             effscal500[2]=string[1]
           elif "mChi10" in string[0]:
             effscal500[1]=string[1]
           elif "mChi1" in string[0]:
             effscal500[0]=string[1]
       elif "mZP300" in string[0]: #mZP300
           if "mChi1000" in string[0]:
             effscal300[5]=string[1]
           elif "mChi500" in string[0]:
             effscal300[4]=string[1]
           elif "mChi150" in string[0]:
             effscal300[3]=string[1]
           elif "mChi50" in string[0]:
             effscal300[2]=string[1]
           elif "mChi10" in string[0]:
             effscal300[1]=string[1]
           elif "mChi1" in string[0]:
             effscal300[0]=string[1]
       elif "mZP295" in string[0]: #mZP295
           if "mChi1000" in string[0]:
             effscal295[5]=string[1]
           elif "mChi500" in string[0]:
             effscal295[4]=string[1]
           elif "mChi150" in string[0]:
             effscal295[3]=string[1]
           elif "mChi50" in string[0]:
             effscal295[2]=string[1]
           elif "mChi10" in string[0]:
             effscal295[1]=string[1]
           elif "mChi1" in string[0]:
             effscal295[0]=string[1]
       elif "mZP200" in string[0]: #mZP200
           if "mChi1000" in string[0]:
             effscal200[5]=string[1]
           elif "mChi500" in string[0]:
             effscal200[4]=string[1]
           elif "mChi150" in string[0]:
             effscal200[3]=string[1]
           elif "mChi50" in string[0]:
             effscal200[2]=string[1]
           elif "mChi10" in string[0]:
             effscal200[1]=string[1]
           elif "mChi1" in string[0]:
             effscal200[0]=string[1]
       elif "mZP100" in string[0]: #mZP100
           if "mChi1000" in string[0]:
             effscal100[5]=string[1]
           elif "mChi500" in string[0]:
             effscal100[4]=string[1]
           elif "mChi150" in string[0]:
             effscal100[3]=string[1]
           elif "mChi50" in string[0]:
             effscal100[2]=string[1]
           elif "mChi10" in string[0]:
             effscal100[1]=string[1]
           elif "mChi1" in string[0]:
             effscal100[0]=string[1]
       elif "mZP95" in string[0]: #mZP95
           if "mChi1000" in string[0]:
             effscal95[5]=string[1]
           elif "mChi500" in string[0]:
             effscal95[4]=string[1]
           elif "mChi150" in string[0]:
             effscal95[3]=string[1]
           elif "mChi50" in string[0]:
             effscal95[2]=string[1]
           elif "mChi10" in string[0]:
             effscal95[1]=string[1]
           elif "mChi1" in string[0]:
             effscal95[0]=string[1]
       elif "mZP50" in string[0]: #mZP50
           if "mChi1000" in string[0]:
             effscal50[5]=string[1]
           elif "mChi500" in string[0]:
             effscal50[4]=string[1]
           elif "mChi150" in string[0]:
             effscal50[3]=string[1]
           elif "mChi50" in string[0]:
             effscal50[2]=string[1]
           elif "mChi10" in string[0]:
             effscal50[1]=string[1]
           elif "mChi1" in string[0]:
             effscal50[0]=string[1]
       elif "mZP20" in string[0]: #mZP20
           if "mChi1000" in string[0]:
             effscal20[5]=string[1]
           elif "mChi500" in string[0]:
             effscal20[4]=string[1]
           elif "mChi150" in string[0]:
             effscal20[3]=string[1]
           elif "mChi50" in string[0]:
             effscal20[2]=string[1]
           elif "mChi10" in string[0]:
             effscal20[1]=string[1]
           elif "mChi1" in string[0]:
             effscal20[0]=string[1]
       elif "mZP15" in string[0]: #mZP15
           if "mChi1000" in string[0]:
             effscal15[5]=string[1]
           elif "mChi500" in string[0]:
             effscal15[4]=string[1]
           elif "mChi150" in string[0]:
             effscal15[3]=string[1]
           elif "mChi50" in string[0]:
             effscal15[2]=string[1]
           elif "mChi10" in string[0]:
             effscal15[1]=string[1]
           elif "mChi1" in string[0]:
             effscal100[0]=string[1]
       elif "mZP10" in string[0]: #mZP10
           if "mChi1000" in string[0]:
             effscal10[5]=string[1]
           elif "mChi500" in string[0]:
             effscal10[4]=string[1]
           elif "mChi150" in string[0]:
             effscal10[3]=string[1]
           elif "mChi50" in string[0]:
             effscal10[2]=string[1]
           elif "mChi10" in string[0]:
             effscal10[1]=string[1]
           elif "mChi1" in string[0]:
             effscal10[0]=string[1]


  # WRITE OUT TO A NEW TABLE
  fout.write("\\documentclass[a4paper,landscape]{article} \n")
  fout.write("\\usepackage[paperheight=8.0in,paperwidth=11.0in,margin=1.0in,headheight=0.0in,footskip=0.5in,includehead,includefoot]{geometry} \n")
  fout.write("\\begin{document} \n")
  fout.write("\n")
  fout.write("\\begin{table}[bthp] \n")
  fout.write("\\centering \n")
  fout.write("\\begin{tabular}{|c|c|c|c|c|c|c|} \n")
  fout.write("\\hline \n")

  fout.write("% 2HDM Efficiencies \n")
  fout.write("\\multicolumn{7}{|c|}{ 2HDM } \\\\ \n")
  fout.write("\\hline \n")
  fout.write("$m_{A}$ [GeV] & 300 & 400 & 500 & 600 & 700 & 800 \\\\   \n")
  fout.write("$m_{Z'}$ [GeV] &    &     &     &     &     &     \\\\   \n")
  fout.write("\\hline \n")
  fout.write("600 & "+eff2hdm600[0]+" & "+eff2hdm600[1]+" & "+eff2hdm600[2]+" & "+eff2hdm600[3]+" & "+eff2hdm600[4]+" & "+eff2hdm600[5]+" \\\\ \n")
  fout.write("800 & "+eff2hdm800[0]+" & "+eff2hdm800[1]+" & "+eff2hdm800[2]+" & "+eff2hdm800[3]+" & "+eff2hdm800[4]+" & "+eff2hdm800[5]+" \\\\ \n")
  fout.write("1000 & "+eff2hdm1000[0]+" & "+eff2hdm1000[1]+" & "+eff2hdm1000[2]+" & "+eff2hdm1000[3]+" & "+eff2hdm1000[4]+" & "+eff2hdm1000[5]+" \\\\ \n")
  fout.write("1200 & "+eff2hdm1200[0]+" & "+eff2hdm1200[1]+" & "+eff2hdm1200[2]+" & "+eff2hdm1200[3]+" & "+eff2hdm1200[4]+" & "+eff2hdm1200[5]+" \\\\ \n")
  fout.write("1400 & "+eff2hdm1400[0]+" & "+eff2hdm1400[1]+" & "+eff2hdm1400[2]+" & "+eff2hdm1400[3]+" & "+eff2hdm1400[4]+" & "+eff2hdm1400[5]+" \\\\ \n")
  fout.write("1700 & "+eff2hdm1700[0]+" & "+eff2hdm1700[1]+" & "+eff2hdm1700[2]+" & "+eff2hdm1700[3]+" & "+eff2hdm1700[4]+" & "+eff2hdm1700[5]+" \\\\ \n")
  fout.write("2000 & "+eff2hdm2000[0]+" & "+eff2hdm2000[1]+" & "+eff2hdm2000[2]+" & "+eff2hdm2000[3]+" & "+eff2hdm2000[4]+" & "+eff2hdm2000[5]+" \\\\ \n")
  fout.write("2500 & "+eff2hdm2500[0]+" & "+eff2hdm2500[1]+" & "+eff2hdm2500[2]+" & "+eff2hdm2500[3]+" & "+eff2hdm2500[4]+" & "+eff2hdm2500[5]+" \\\\ \n")
  fout.write("\\hline \n")
  fout.write("\\hline \n")

  fout.write("% Baryonic Model Efficiencies \n")
  fout.write("\\multicolumn{7}{|c|}{ Baryonic } \\\\ \n")
  fout.write("\\hline \n")
  fout.write("$m_{\\chi}$ [GeV] & 1 & 10 & 50 & 150 & 500 & 1000 \\\\   \n")
  fout.write("$m_{Z'}$ [GeV]    &   &    &    &     &     &      \\\\   \n")
  fout.write("\\hline \n")
  fout.write("10 & "+effbary10[0]+" & "+effbary10[1]+" & "+effbary10[2]+" & "+effbary10[3]+" & "+effbary10[4]+" & "+effbary10[5]+" \\\\ \n")
  fout.write("15 & "+effbary15[0]+" & "+effbary15[1]+" & "+effbary15[2]+" & "+effbary15[3]+" & "+effbary15[4]+" & "+effbary15[5]+" \\\\ \n")
  fout.write("20 & "+effbary20[0]+" & "+effbary20[1]+" & "+effbary20[2]+" & "+effbary20[3]+" & "+effbary20[4]+" & "+effbary20[5]+" \\\\ \n")
  fout.write("50 & "+effbary50[0]+" & "+effbary50[1]+" & "+effbary50[2]+" & "+effbary50[3]+" & "+effbary50[4]+" & "+effbary50[5]+" \\\\ \n")
  fout.write("100 & "+effbary100[0]+" & "+effbary100[1]+" & "+effbary100[2]+" & "+effbary100[3]+" & "+effbary100[4]+" & "+effbary100[5]+" \\\\ \n")
  fout.write("200 & "+effbary200[0]+" & "+effbary200[1]+" & "+effbary200[2]+" & "+effbary200[3]+" & "+effbary200[4]+" & "+effbary200[5]+" \\\\ \n")
  fout.write("295 & "+effbary295[0]+" & "+effbary295[1]+" & "+effbary295[2]+" & "+effbary295[3]+" & "+effbary295[4]+" & "+effbary295[5]+" \\\\ \n")
  fout.write("300 & "+effbary300[0]+" & "+effbary300[1]+" & "+effbary300[2]+" & "+effbary300[3]+" & "+effbary300[4]+" & "+effbary300[5]+" \\\\ \n")
  fout.write("500 & "+effbary500[0]+" & "+effbary500[1]+" & "+effbary500[2]+" & "+effbary500[3]+" & "+effbary500[4]+" & "+effbary500[5]+" \\\\ \n")
  fout.write("995 & "+effbary995[0]+" & "+effbary995[1]+" & "+effbary995[2]+" & "+effbary995[3]+" & "+effbary995[4]+" & "+effbary995[5]+" \\\\ \n")
  fout.write("1000 & "+effbary1000[0]+" & "+effbary1000[1]+" & "+effbary1000[2]+" & "+effbary1000[3]+" & "+effbary1000[4]+" & "+effbary1000[5]+" \\\\ \n")
  fout.write("1995 & "+effbary1995[0]+" & "+effbary1995[1]+" & "+effbary1995[2]+" & "+effbary1995[3]+" & "+effbary1995[4]+" & "+effbary1995[5]+" \\\\ \n")
  fout.write("2000 & "+effbary2000[0]+" & "+effbary2000[1]+" & "+effbary2000[2]+" & "+effbary2000[3]+" & "+effbary2000[4]+" & "+effbary2000[5]+" \\\\ \n")
  fout.write("10000 & "+effbary10000[0]+" & "+effbary10000[1]+" & "+effbary10000[2]+" & "+effbary10000[3]+" & "+effbary10000[4]+" & "+effbary10000[5]+" \\\\ \n")
  fout.write("\\hline \n")
  fout.write("\\hline \n")

  fout.write("% Scalar Model Efficiencies \n")
  fout.write("\\multicolumn{7}{|c|}{ Scalar } \\\\ \n")
  fout.write("\\hline \n")
  fout.write("$m_{\\chi}$ [GeV] & 1 & 10 & 50 & 150 & 500 & 1000 \\\\   \n")
  fout.write("$m_{S}$ [GeV]    &   &    &    &     &     &       \\\\   \n")
  fout.write("\\hline \n")
  fout.write("10 & "+effscal10[0]+" & "+effscal10[1]+" & "+effscal10[2]+" & "+effscal10[3]+" & "+effscal10[4]+" & "+effscal10[5]+" \\\\ \n")
  fout.write("15 & "+effscal15[0]+" & "+effscal15[1]+" & "+effscal15[2]+" & "+effscal15[3]+" & "+effscal15[4]+" & "+effscal15[5]+" \\\\ \n")
  fout.write("20 & "+effscal20[0]+" & "+effscal20[1]+" & "+effscal20[2]+" & "+effscal20[3]+" & "+effscal20[4]+" & "+effscal20[5]+" \\\\ \n")
  fout.write("50 & "+effscal50[0]+" & "+effscal50[1]+" & "+effscal50[2]+" & "+effscal50[3]+" & "+effscal50[4]+" & "+effscal50[5]+" \\\\ \n")
  fout.write("95 & "+effscal95[0]+" & "+effscal95[1]+" & "+effscal95[2]+" & "+effscal95[3]+" & "+effscal95[4]+" & "+effscal95[5]+" \\\\ \n")
  fout.write("100 & "+effscal100[0]+" & "+effscal100[1]+" & "+effscal100[2]+" & "+effscal100[3]+" & "+effscal100[4]+" & "+effscal100[5]+" \\\\ \n")
  fout.write("200 & "+effscal200[0]+" & "+effscal200[1]+" & "+effscal200[2]+" & "+effscal200[3]+" & "+effscal200[4]+" & "+effscal200[5]+" \\\\ \n")
  fout.write("295 & "+effscal295[0]+" & "+effscal295[1]+" & "+effscal295[2]+" & "+effscal295[3]+" & "+effscal295[4]+" & "+effscal295[5]+" \\\\ \n")
  fout.write("300 & "+effscal300[0]+" & "+effscal300[1]+" & "+effscal300[2]+" & "+effscal300[3]+" & "+effscal300[4]+" & "+effscal300[5]+" \\\\ \n")
  fout.write("500 & "+effscal500[0]+" & "+effscal500[1]+" & "+effscal500[2]+" & "+effscal500[3]+" & "+effscal500[4]+" & "+effscal500[5]+" \\\\ \n")
  fout.write("995 & "+effscal995[0]+" & "+effscal995[1]+" & "+effscal995[2]+" & "+effscal995[3]+" & "+effscal995[4]+" & "+effscal995[5]+" \\\\ \n")
  fout.write("1000 & "+effscal1000[0]+" & "+effscal1000[1]+" & "+effscal1000[2]+" & "+effscal1000[3]+" & "+effscal1000[4]+" & "+effscal1000[5]+" \\\\ \n")
  fout.write("10000 & "+effscal10000[0]+" & "+effscal10000[1]+" & "+effscal10000[2]+" & "+effscal10000[3]+" & "+effscal10000[4]+" & "+effscal10000[5]+" \\\\ \n")
  fout.write("\\hline \n")
  fout.write("\\end{tabular} \n")
  fout.write("\\end{table} \n")
  fout.write("\n")
  fout.write("\\end{document} \n")

if __name__ == "__main__":
  AllGlobal("EffandYieldsTables/PlainEff_optB_highMET.tex")
  AllGlobal("EffandYieldsTables/PlainEff_optB_lowMET.tex")
  AllGlobal("EffandYieldsTables/Yields_optB_highMET.tex")
  AllGlobal("EffandYieldsTables/Yields_optB_lowMET.tex")

