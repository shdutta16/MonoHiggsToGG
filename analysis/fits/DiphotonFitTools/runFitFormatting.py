import sys
import os
import numpy
import math
import datetime
from ROOT import *
from optparse import OptionParser, make_option

def run(): 

    # option list
    parser = OptionParser("usage: %prog [options]")
    parser.add_option("-f","--infile",action="store",dest="names_file",type="string",
                      default="names.txt",help="Names of samples"),
    parser.add_option("-i",action="store",type="string",dest="indir",
                      default="/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/",
                      help="Input sample dir"),
    parser.add_option("-o",action="store",type="string",dest="outdir",
                      default="test_dir/",help="Output dir"),
    parser.add_option("--ext",action="store",dest="ext",type="string",
                      default="_skim_woVetos",help="Type of skim"),
    parser.add_option("-l","--label",action="store",dest="label",type="string",
                      default="",help="Additional label"),
    # end option list
    (options,args) = parser.parse_args()

    # read input file 
    sigs = []
    bkgs = []
    data = []
    pickupfiles(options.names_file,sigs,bkgs,data)
   
    # make output dir
    if not os.path.exists(options.outdir): os.mkdir(options.outdir) 
    # make output file
    settingsfilename = "SettingDescription.txt"
    print("Making settings description: %s%s"%(options.outdir,settingsfilename))
    settingsfile = open(options.outdir+settingsfilename,"w")

    #writeoutfile(outputfile,options,sigs,bkgs,data)

    # setup branch addresses
    gROOT.ProcessLine(
    "struct tree_t {\
        Float_t	mgg;\
        Float_t	met;\
        Float_t	mva;\
        Float_t	dphiggmet;\
        Float_t	dphijmet;\
        Int_t	njets;\
        Int_t	nels;\
        Int_t	nmus;\
        Float_t eta1;\
        Float_t eta2;\
    }" )

    # setup cats
    cats = ["met_0-all","met_0-130","met_130-all"]
    #cats = ["mva_0-1"]

    # setup cuts
    # vetos events that don't fit the cut 
    # format: [varname] [<,<=,==,!=,=>,>] [cut]
    # varnames allowed: mgg, mva, met, dphiggmet, dphijmet, njets, nels, nmus 
    cuts = []
    cuts.append("dphiggmet > 2.1")
    cuts.append("dphijmet > 0.5")
    cuts.append("njets < 2")
    cuts.append("#nels == 0")
    cuts.append("#nmus == 0")  

    # write out a file that stores what settings were applied
    settingsfile.write("---------------------------------------------------\n")
    settingsfile.write(" Settings for these ntuples (dir:%s) \n" %options.outdir)
    settingsfile.write(" Produced on: ")
    settingsfile.write(datetime.datetime.now().strftime('%Y-%m-%d at %H:%M:%S \n'))
    settingsfile.write("---------------------------------------------------\n")
    settingsfile.write("\nCategories: \n")
    for cat in cats: settingsfile.write("\t %s \n" %cat)
    settingsfile.write("\nPhoton categories (default): all, EB, EE \n")
    settingsfile.write("\nCuts applied: \n")
    for cut in cuts: 
        if cut[0]=="#": continue  
        settingsfile.write("\t %s \n" %cut)
    settingsfile.write("\nSamples: \n")
    settingsfile.write("\t dir: %s \n"%options.indir)
    settingsfile.write("\t ext: %s \n"%options.ext)
    for sig in sigs: settingsfile.write("\n"+sig)
    for bkg in bkgs: settingsfile.write("\n"+bkg)
    for dat in data: settingsfile.write("\n"+dat) 

    # format ntuples
    for sig in sigs: formatting(options,tree_t,0,sig,cuts,cats)
    #for bkg in bkgs: formatting(options,tree_t,1,bkg,cuts,cats)
    #for dat in data: formatting(options,tree_t,2,dat,cuts,cats)

    # hadd MC ntuples
    haddcmd = "hadd -f "+options.outdir+"Output_MC.root "+options.outdir+"sig*"
    for bkg in bkgs: 
        haddcmd += " "
        haddcmd += options.outdir
        haddcmd += bkg
        haddcmd += options.ext
        haddcmd += "_new.root"
    print haddcmd
    os.system(haddcmd)

def formatting(opts,tree_t,sampletype,infile,cuts,cats):

    print("Formating sample: %s"%infile)
    # get input file
    fOrigName = opts.indir+infile+opts.ext+".root"
    fOrig = TFile(fOrigName)
    if (not fOrig): 
        print("File NOT found: %s" %fOrigName)
        sys.exit()
    tOrig = fOrig.Get("DiPhotonTree")
    if (not tOrig):
        print("TTree NOT found")
        sys.exit()

    DiPhotonTree = tree_t()
    tOrig.SetBranchAddress("mgg",AddressOf(DiPhotonTree,'mgg'))
    tOrig.SetBranchAddress("t1pfmetCorr",AddressOf(DiPhotonTree,'met'))
    tOrig.SetBranchAddress("MVA_Response",AddressOf(DiPhotonTree,'mva'))
    tOrig.SetBranchAddress("dphiggmet",AddressOf(DiPhotonTree,'dphiggmet'))
    tOrig.SetBranchAddress("mindphijmet",AddressOf(DiPhotonTree,'dphijmet'))
    tOrig.SetBranchAddress("nJets30",AddressOf(DiPhotonTree,'njets'))
    tOrig.SetBranchAddress("nEle",AddressOf(DiPhotonTree,'nels'))
    tOrig.SetBranchAddress("nMuons",AddressOf(DiPhotonTree,'nmus'))
    tOrig.SetBranchAddress("eta1",AddressOf(DiPhotonTree,'eta1'))
    tOrig.SetBranchAddress("eta2",AddressOf(DiPhotonTree,'eta2'))

    # setup cuts
    varname = []
    varsign = {} 
    varvalu = {}
    for cut in cuts:
        if cut[0]=="#": continue # ignore "commented out" cuts
        tmpname = cut.split(" ")[0]
        varname.append(tmpname)
        varsign[tmpname]=cut.split(" ")[1]
        varvalu[tmpname]=cut.split(" ")[2]

    # setup categories
    nCat = len(cats)
    catlo = []
    cathi = []
    var = "" 
    for cat in cats:
        cat = cat.split("_") # split cat into: name cut
        var = cat[0]
        catlo.append(cat[1].split("-")[0])
        cathi.append(1000000) if cat[1].split("-")[1]=="all" else cathi.append(cat[1].split("-")[1])

    # setup photon categories
    phoCat = []
    phoCat.append("all")
    phoCat.append("EB")
    phoCat.append("EE")    
    nPho = len(phoCat)

    # setup output file & trees
    fOutName = opts.outdir
    if (sampletype==0): fOutName = fOutName+"sig_"+infile+opts.ext+"_new.root" # signals
    if (sampletype==1): fOutName = fOutName+infile+opts.ext+"_new.root"        # backgrounds
    if (sampletype==2): fOutName = fOutName+"Output_Data.root"                 # data
    fOut = TFile(fOutName,'RECREATE')
    dir1 = "cic" if sampletype==2 else "genmc"
    dir2 = "trees"
    fOut.mkdir(dir1)
    fOut.cd(dir1)
    fOut.mkdir(dir1+"/"+dir2)
    fOut.cd(dir1+"/"+dir2)
    tOut = [[tOrig.CloneTree(0)]*nPho]*nCat 

    # run over events of tree 
    for i in range(0,tOrig.GetEntries()):
        tOrig.GetEntry(i)  

        # apply cuts
        variable = 0.0
        for var in varname:
            # pick up the variable
            if var=="dphiggmet":   variable = tOrig.dphiggmet
            if var=="dphijmet":    variable = tOrig.mindphijmet
            if var=="met":         variable = tOrig.t1pfmetCorr
            if var=="mva":         variable = tOrig.MVA_Response
            if var=="mgg":         variable = tOrig.mgg
            if var=="njets":       variable = tOrig.nJets30
            if var=="nels":        variable = tOrig.nEle
            if var=="nmus":        variable = tOrig.nMuons
            # compare the cut
            if varsign[var]=="<"  and variable >= float(varvalu[var]): continue 
            if varsign[var]==">"  and variable <= float(varvalu[var]): continue
            if varsign[var]=="==" and variable != float(varvalu[var]): continue
            if varsign[var]=="!=" and variable == float(varvalu[var]): continue
            if varsign[var]==">=" and variable <  float(varvalu[var]): continue
            if varsign[var]=="<=" and variable >  float(varvalu[var]): continue

        # setup categories
        passCatSel = [False]*nCat
        for c in range(0,nCat):
            if var=="met" and (tOrig.t1pfmetCorr >= float(catlo[c]) and tOrig.t1pfmetCorr < float(cathi[c])) :
                passCatSel[c] = True
            if var=="mva" and (tOrig.MVA_response >= float(catlo[c]) and tOrig.MVA_response < float(cathi[c])) :
                passCatSel[c] = True
       
        # split into photon categories
        EB1 = True if (math.fabs(tOrig.eta1)>1.566)  else False
        EB2 = True if (math.fabs(tOrig.eta2)>1.566)  else False
        EE1 = True if (math.fabs(tOrig.eta1)<1.4442) else False
        EE2 = True if (math.fabs(tOrig.eta2)<1.4442) else False
        EB  = True if (EB1 and EB2) else False
        EE  = True if (EE1 or EE2) else False 

        # fill trees 
        fOut.cd(dir1)
        fOut.cd(dir1+"/"+dir2)
        for c in range(0,nCat):
            if passCatSel[c]:
                tOut[c][0].Fill()         # all
                if EB : tOut[c][1].Fill() # EB
                if EE : tOut[c][2].Fill() # EE
    
    # write out file
    print "Writing out file"
    fOut.cd()
    fOut.cd(dir1)
    fOut.cd(dir1+"/"+dir2)
    # rename output trees
    tOutName = [[""]*nPho]*nCat
    for c in range(0,nCat):
        for p in range(0,nPho):
            tOutName[c][p] = "Data_13TeV_" if sampletype==2 else infile+"_13TeV_"
            tmpCatStr = cats[c].replace("_","")
            tOutName[c][p] += tmpCatStr.replace("-all","")
            if p!=0: tOutName[c][p] += phoCat[p]
            tOut[c][p].SetName(tOutName[c][p])
            tOut[c][p].Write()
    # close file
    fOut.ls()
    # write out tdir structure
    fOutSettings = open(opts.outdir+"NtupleStructure.txt","w")

def pickupfiles(names_file,sigs,bkgs,data):
    namesfile  = open(names_file,"r")

    for line in namesfile:
      if "#" in line: continue # remove commented out lines
      line = line.rstrip('\n')
      line = line.split(' ')
      if "sig" in line[0]:
          sigs.append(line[1]) 
      if "bkg" in line[0]:
          bkgs.append(line[1])
      if "dat" in line[0]:
          data.append(line[1])


if __name__ == "__main__":
    run()
