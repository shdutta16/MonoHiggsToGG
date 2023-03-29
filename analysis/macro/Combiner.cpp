#include "Combiner.hh"
#include <iostream>
#include <fstream>

Combiner::Combiner( SamplePairVec Samples, const Double_t inLumi, const ColorMap colorMap, const TString outdir, const Bool_t doNmin1, const Bool_t do_stack, const TString type, const Bool_t doQCDrescale, const Int_t whichSelection, const Bool_t mergeBkgs){

  if (doNmin1) addText = "_n-1";
  else addText="";

  doStack = false;
  if (do_stack) doStack = true;

  doQCDscale = doQCDrescale;

  doMergeBkgs = mergeBkgs;

  fType = type;
  lumi	= inLumi;
  fOutDir = outdir;
  TString fOut = "comb";
  
  METcut = 70;// METcut to apply for OrigSel
  //if (fWhichSel == 1) METcut = 105;// OptSel1
  if (fWhichSel == 1) METcut = 130;// OptSel1
  if (fWhichSel == 2) METcut = 95;//  OptSel2
  if (fWhichSel == 3) METcut = 50;//  OptSel3
  if (fWhichSel == 4) METcut = 70;//  OptSel4

  MakeOutDirectory(Form("%s%s",fOutDir.Data(),fOut.Data()));
  fOutFile = new TFile(Form("%s%s/combplots%s.root",fOutDir.Data(),fOut.Data(),addText.Data()),"RECREATE");
  CheckValidFile(fOutFile, Form("%s%s/combplots%s.root",fOutDir.Data(),fOut.Data(),addText.Data())); 

  for (SamplePairVecIter iter = Samples.begin(); iter != Samples.end(); ++iter){
    if ( (*iter).second == 1 ) {fBkgNames.push_back((*iter).first);}      // background
    else if ( (*iter).second == 0 ) {fSigNames.push_back((*iter).first);} // signal
    else {fDataNames.push_back((*iter).first);}			          // data
  }
  
  fNData = fDataNames.size();
  fNBkg  = fBkgNames.size();
  fNSig  = fSigNames.size();
 
  std::cout << "DataSize = " << fNData << " BkgSize = " << fNBkg << " SigSize = " << fNSig << std::endl;

  Combiner::InitTH1DNames();
  fNTH1D = fTH1DNames.size(); 

  // define colorMap and title
  fColorMap = colorMap;
  
  fSampleTitleMap["SMHiggs"]		= "SM h #rightarrow #gamma#gamma";
  fSampleTitleMap["EWK1pho"]		= "EW + #gamma";
  fSampleTitleMap["EWK2pho"]		= "EW + #gamma#gamma";
  fSampleTitleMap["Jetspho"]		= "QCD, #gamma + jets";

  fSampleTitleMap["DoubleEG"]		= "Data";
  fSampleTitleMap["QCD"] 		= "QCD";
  fSampleTitleMap["GJets"]		= "#gamma + jets";
  fSampleTitleMap["DYJetsToLL"]		= "DY + jets";
  fSampleTitleMap["DiPhoton"]		= "#gamma#gamma";
  fSampleTitleMap["GluGluHToGG"]	= "h #rightarrow #gamma#gamma (ggH)";
  fSampleTitleMap["VHToGG"]		= "Vh";
  fSampleTitleMap["ttHJetToGG"]		= "tt + h #rightarrow #gamma#gamma";
  fSampleTitleMap["VBFHToGG"]		= "VBF h #rightarrow #gamma#gamma";
  fSampleTitleMap["WGToLNuG"]		= "#gamma + W #rightarrow l #nu";
  fSampleTitleMap["WJetsToLNu"]		= "W + Z #rightarrow l#nu";
  fSampleTitleMap["WZTo2L2Q"]		= "W + Z #rightarrow llqq";
  fSampleTitleMap["ZGTo2NuG"]		= "#gamma + Z #rightarrow #nu#nu";
  fSampleTitleMap["ZGTo2LG"]		= "#gamma + Z #rightarrow ll";
  fSampleTitleMap["ZJets"]		= "Z #rightarrow #nu#nu + Jets";
  fSampleTitleMap["ZZTo2L2Q"]		= "Z + Z #rightarrow llqq";
  fSampleTitleMap["ZZTo2L2Nu"]		= "Z + Z #rightarrow ll#nu#nu";
  fSampleTitleMap["TTGJets"]		= "tt + #gamma + Jets";
  fSampleTitleMap["TTGG_0Jets"]		= "tt + #gamma#gamma";
  fSampleTitleMap["TGJets"]		= "t + #gamma + Jets";
  fSampleTitleMap["TTJets"]		= "tt + Jets";
  //fSampleTitleMap["DMHtoGG_M1"]		= "m_{#chi} = 1 GeV";//#bar{#chi}#chi HH ,m_{#chi} = 1 GeV";
  //fSampleTitleMap["DMHtoGG_M10"]	= "m_{#chi} = 10 GeV";//#bar{#chi}#chi HH ,m_{#chi} = 10 GeV";
  //fSampleTitleMap["DMHtoGG_M100"]	= "m_{#chi} = 100 GeV";//#bar{#chi}#chi HH ,m_{#chi} = 100 GeV";
  //fSampleTitleMap["DMHtoGG_M1000"]	= "m_{#chi} = 1000 GeV";//#bar{#chi}#chi HH ,m_{#chi} = 1000 GeV";
  fSampleTitleMap["FakeData"]		= "FakeData";  
  fSampleTitleMap["FakeDataII"]		= "Test";
  fSampleTitleMap["2HDM_mZP600_mA0300"]	= "m_{Z'} = 600 GeV";//#bar{#chi}#chi HH ,m_{#chi} = 1 GeV";
  fSampleTitleMap["2HDM_mZP800_mA0300"]	= "m_{Z'} = 800 GeV";//#bar{#chi}#chi HH ,m_{#chi} = 1 GeV";
  fSampleTitleMap["2HDM_mZP1000_mA0300"]= "m_{Z'} = 1000 GeV";//#bar{#chi}#chi HH ,m_{#chi} = 1 GeV";
  fSampleTitleMap["2HDM_mZP1200_mA0300"]= "m_{Z'}=1200 GeV";//#bar{#chi}#chi HH ,m_{#chi} = 10 GeV";
  fSampleTitleMap["2HDM_mZP1400_mA0300"]= "m_{Z'}=1400 GeV";//#bar{#chi}#chi HH ,m_{#chi} = 10 GeV";
  fSampleTitleMap["2HDM_mZP1700_mA0300"]= "m_{Z'}=1700 GeV";//#bar{#chi}#chi HH ,m_{#chi} = 100 GeV";
  fSampleTitleMap["2HDM_mZP2000_mA0300"]= "m_{Z'}=2000 GeV";//#bar{#chi}#chi HH ,m_{#chi} = 1000 GeV";
  fSampleTitleMap["2HDM_mZP2500_mA0300"]= "m_{Z'}=2500 GeV";//#bar{#chi}#chi HH ,m_{#chi} = 1000 GeV";
  fSampleTitleMap["2HDM_mZP600_mA0400"]	= "m_{Z'}=600 (m_{A0}=400) GeV";//#bar{#chi}#chi HH ,m_{#chi} = 1 GeV";
  fSampleTitleMap["2HDM_mZP800_mA0400"]	= "m_{Z'}=800 (m_{A0}=400) GeV";//#bar{#chi}#chi HH ,m_{#chi} = 1 GeV";

  //for (std::map<TString,TString>::iterator iter = fSampleTitleMap.begin(); iter != fSampleTitleMap.end(); ++iter) {
  //  std::cout << (*iter).first << "  " << (*iter).second << std::endl;
  //}

  Combiner::InitCanvAndHists();

}// end Combiner::Combiner

Combiner::~Combiner(){
  std::cout << "Finished & Deleting" << std::endl;

 // delete all pointers
  for (UInt_t th1d = 0; th1d < fNTH1D; th1d++){
    //if (fNData > 0 ){
      delete fOutDataTH1DHists[th1d];
      delete fOutTH1DRatioPads[th1d];
      delete fOutTH1DRatioLines[th1d];
    //}
    delete GJetsClone[th1d];
    delete fOutBkgTH1DHists[th1d];
    delete fOutHiggsBkgTH1DHists[th1d];
    delete fOutEWK1phoBkgTH1DHists[th1d];
    delete fOutEWK2phoBkgTH1DHists[th1d];
    delete fOutJetsphoBkgTH1DHists[th1d];
    delete fOutDYJetsBkgTH1DHists[th1d];
    delete fOutBkgTH1DStacks[th1d];
    delete fOutBkgTH1DStacksForUncer[th1d];
    delete fTH1DLegends[th1d];
    delete fTH1DLegendsSig[th1d];
    delete fOutTH1DStackPads[th1d];
    delete fOutTH1DCanvases[th1d];
    
    for (UInt_t data = 0; data < fNData; data++) { delete fInDataTH1DHists[th1d][data]; }
    for (UInt_t mc = 0; mc < fNBkg; mc++) { delete fInBkgTH1DHists[th1d][mc]; }
    for (UInt_t mc = 0; mc < fNSig; mc++) { delete fInSigTH1DHists[th1d][mc]; }
  }

  for (UInt_t data = 0; data < fNData; data++) { delete fDataFiles[data]; }
  for (UInt_t mc = 0; mc < fNBkg; mc++) { delete fBkgFiles[mc]; }
  for (UInt_t mc = 0; mc < fNSig; mc++) { delete fSigFiles[mc]; }

  delete fOutFile;

}// end Combiner::~Combiner

void Combiner::DoComb(){
  // copy th1d plots into output hists/stacks
  for (UInt_t th1d = 0; th1d < fNTH1D; th1d++){

    // data : copy first histogram & add all others too it 
    //if (fNData > 0){
      for (UInt_t data = 0; data < fNData; data++){
        if (data == 0) fOutDataTH1DHists[th1d] = (TH1D*)fInDataTH1DHists[th1d][data]->Clone(); 
        else fOutDataTH1DHists[th1d]->Add(fInDataTH1DHists[th1d][data]);
      }
    //}// end if ndata>0

    ////// ---------------------------------
    ////// scale to the integral of the data
    ////// ---------------------------------
    Double_t data_integral = fOutDataTH1DHists[th1d]->Integral();
    TH1D *allBkgHisto;
    for (UInt_t mc = 0; mc < fNBkg; mc++){
      //if (fTH1DNames[th1d]=="mgg_IsolateALLlowCUT") std::cout << "------ Sample : " << fBkgNames[mc] << " ------- integral = " << fInBkgTH1DHists[th1d][mc]->Integral() << std::endl;
      if (mc==0) allBkgHisto = (TH1D*)fInBkgTH1DHists[th1d][mc]->Clone();
      else allBkgHisto->Add(fInBkgTH1DHists[th1d][mc]);
    }
    Double_t bkg_integral = allBkgHisto->Integral();
    Double_t binMggLo = allBkgHisto->FindBin(115);
    Double_t binMggHi = allBkgHisto->FindBin(135); 
    Double_t blinded_part = allBkgHisto->Integral(binMggLo,binMggHi);
    //  if (fTH1DNames[th1d]=="mgg_IsolateALL" || fTH1DNames[th1d]=="mgg" || fTH1DNames[th1d]=="mgg_IsolateALLmetCUT" || fTH1DNames[th1d]=="mgg_IsolateALLlowCUT") data_integral += blinded_part;
    Double_t new_integral = data_integral/bkg_integral;
    std::cout << fTH1DNames[th1d] << ": Data = " << data_integral << "Bkg = " << bkg_integral << " ----- Ratio = " << data_integral/bkg_integral << std::endl;
    for (UInt_t mc = 0; mc < fNBkg; mc++){
     //if ( fBkgNames[mc]=="GluGluHToGG")     fInBkgTH1DHists[th1d][mc]->Scale(0.1001978*1.33/0.0952946);
     //else if ( fBkgNames[mc]=="VBFHToGG")   fInBkgTH1DHists[th1d][mc]->Scale(0.00858514*1.33/0.00890975); 
     //else if ( fBkgNames[mc]=="ttHJetToGG") fInBkgTH1DHists[th1d][mc]->Scale(0.001151117*1.33/0.001132049); 
     //else if ( fBkgNames[mc]=="VHToGG")     fInBkgTH1DHists[th1d][mc]->Scale(0.00512339*1.33/0.002050718);
     if (new_integral > 0 && fBkgNames[mc]!="GluGluHToGG" && fBkgNames[mc]!="VBFHToGG" && fBkgNames[mc]!="ttHJetToGG" && fBkgNames[mc]!="VHToGG") fInBkgTH1DHists[th1d][mc]->Scale(new_integral);
    // if (new_integral > 0) fInBkgTH1DHists[th1d][mc]->Scale(new_integral);
    }
    delete allBkgHisto;
 
    // ---------------------------------
    // Because QCD has some events with very large weights
    // Copy the GJets histo, weight it by the QCD integral 
    // and use it for QCD distribution instead.
    // ---------------------------------
    Double_t qcd_integral = 0;
    Double_t gjets_integral = 0;
    for (UInt_t mc = 0; mc < fNBkg; mc++){
      if (fBkgNames[mc] == "QCD") qcd_integral = fInBkgTH1DHists[th1d][mc]->Integral();  
      if (fBkgNames[mc] == "GJets"){
         GJetsClone[th1d] = (TH1D*)fInBkgTH1DHists[th1d][mc]->Clone();
         GJetsClone[th1d]->SetFillColor(fColorMap["QCD"]);
         gjets_integral = GJetsClone[th1d]->Integral();
         if (gjets_integral > 0) GJetsClone[th1d]->Scale(qcd_integral/gjets_integral);
      }
    }

    //Double_t METint = 0.; 
    //// find the integral of MET above 450
    //if (doStack && fTH1DNames[th1d]=="metCorr_IsolateALL"){
    //  for (UInt_t mc=0; mc < fNBkg; mc++){
    //    int BinToUse1 = fInBkgTH1DHists[th1d][mc]->GetBin(450);
    //    int BinToUse2 = fInBkgTH1DHists[th1d][mc]->GetBin(1500);
    //    METint += fInBkgTH1DHists[th1d][mc]->Integral(BinToUse1,BinToUse2);
    //  }
    //  std::cout << "-------- MET integral above 450 = " << METint << std::endl;
    //}


    // Merge some of the bkgs to make plots cleaner
    if (doMergeBkgs){
      for (UInt_t mc = 0; mc < fNBkg; mc++){
	// add SM Higgs bkgs together
	if (fBkgNames[mc]=="ttHJetToGG"){
	  fOutHiggsBkgTH1DHists[th1d] = (TH1D*)fInBkgTH1DHists[th1d][mc]->Clone();
	}
        else if (fBkgNames[mc]=="GluGluHToGG" || fBkgNames[mc]=="VBFHToGG"){
	  fOutHiggsBkgTH1DHists[th1d]->Add(fInBkgTH1DHists[th1d][mc]);
	}
	// add EWK + 1pho bkgs together
	if (fBkgNames[mc]=="TGJets"){
	  fOutEWK1phoBkgTH1DHists[th1d] = (TH1D*)fInBkgTH1DHists[th1d][mc]->Clone();
	}
        else if (fBkgNames[mc]=="TTGJets" || fBkgNames[mc]=="TTJets" || fBkgNames[mc]=="WGToLNuG" || fBkgNames[mc]=="ZGTo2LG" || fBkgNames[mc]=="WJetsToLNu" || fBkgNames[mc]=="ZGTo2NuG"){
	  fOutEWK1phoBkgTH1DHists[th1d]->Add(fInBkgTH1DHists[th1d][mc]);
	}
	// add EWK + 2pho bkgs together
	if (fBkgNames[mc]=="TTGG_0Jets"){
		fOutEWK2phoBkgTH1DHists[th1d] = (TH1D*)fInBkgTH1DHists[th1d][mc]->Clone();
	}
        else if (fBkgNames[mc]=="ZZTo2L2Nu" || fBkgNames[mc]=="ZZTo2L2Q" || fBkgNames[mc]=="WZTo2L2Q"){
	  fOutEWK2phoBkgTH1DHists[th1d]->Add(fInBkgTH1DHists[th1d][mc]);
	}
	// add DY + ZJets bkgs together
	//if (fBkgNames[mc]=="ZJets"){
	//  fOutDYJetsBkgTH1DHists[th1d] = (TH1D*)fInBkgTH1DHists[th1d][mc]->Clone();
        //}
	else if (fBkgNames[mc]=="DYJetsToLL"){
	  fOutDYJetsBkgTH1DHists[th1d] = (TH1D*)fInBkgTH1DHists[th1d][mc]->Clone();
	//  fOutDYJetsBkgTH1DHists[th1d]->Add(fInBkgTH1DHists[th1d][mc]);
 	}
	// add QCD + GJet bkgs togetehr
	if (fBkgNames[mc]=="QCD"){
	  if (doQCDscale){
	    fOutJetsphoBkgTH1DHists[th1d] = (TH1D*)GJetsClone[th1d]->Clone();
	  }
	  else{
	    fOutJetsphoBkgTH1DHists[th1d] = (TH1D*)fInBkgTH1DHists[th1d][mc]->Clone();
	  } 
	}
	else if (fBkgNames[mc]=="GJets"){
	  fOutJetsphoBkgTH1DHists[th1d]->Add(fInBkgTH1DHists[th1d][mc]);
	}
      }// end loop over bkg MC

    fOutHiggsBkgTH1DHists[th1d]->SetFillColor(fColorMap["SMHiggs"]);
    fOutEWK1phoBkgTH1DHists[th1d]->SetFillColor(fColorMap["EWK1pho"]);
    fOutEWK2phoBkgTH1DHists[th1d]->SetFillColor(fColorMap["EWK2pho"]);
    fOutJetsphoBkgTH1DHists[th1d]->SetFillColor(fColorMap["Jetspho"]);
    fOutDYJetsBkgTH1DHists[th1d]->SetFillColor(fColorMap["DYJetsToLL"]);

    }// end doMergeBkgs 



    int i_hgg, i_vbf, i_vh, i_tth;
    for (UInt_t mc = 0; mc < fNBkg; mc++){
      if (fBkgNames[mc] == "GluGluHToGG") i_hgg = mc;
      if (fBkgNames[mc] == "VBFHToGG")    i_vbf = mc;
      if (fBkgNames[mc] == "ttHJetToGG")  i_tth = mc;
      if (fBkgNames[mc] == "VHToGG")	  i_vh  = mc;
    }

    // bkg : copy histos and add to stacks (here *)
    if (doMergeBkgs && fTH1DNames[th1d]!="mgg_forShape" && fTH1DNames[th1d]!="mgg_metCUT_forShape"){
      fOutBkgTH1DStacks[th1d]->Add(fInBkgTH1DHists[th1d][i_vh]);
      fOutBkgTH1DStacks[th1d]->Add(fOutHiggsBkgTH1DHists[th1d]);
      fOutBkgTH1DStacks[th1d]->Add(fOutEWK2phoBkgTH1DHists[th1d]);
      fOutBkgTH1DStacks[th1d]->Add(fOutDYJetsBkgTH1DHists[th1d]);
      fOutBkgTH1DStacks[th1d]->Add(fOutEWK1phoBkgTH1DHists[th1d]);
      fOutBkgTH1DStacks[th1d]->Add(fOutJetsphoBkgTH1DHists[th1d]);
    }

    for (UInt_t mc = 0; mc < fNBkg; mc++){
      //fInBkgTH1DHists[th1d][mc]->Scale(lumi);
      if (fTH1DNames[th1d]=="mgg_forShape" || fTH1DNames[th1d]=="mgg_metCUT_forShape"){
        if (mc==i_vh || mc==i_tth || mc==i_hgg || mc==i_vbf){
          fOutBkgTH1DStacks[th1d]->Add(fInBkgTH1DHists[th1d][mc]);
          fOutBkgTH1DStacksForUncer[th1d]->Add(fInBkgTH1DHists[th1d][mc]);
        }
      }

      // setup uncertainty stack (doesn't matter if the bkgs are merged or not since they are all merged here)
      if (doQCDscale && fBkgNames[mc]=="QCD"){
        fOutBkgTH1DStacksForUncer[th1d]->Add(GJetsClone[th1d]);
      }
      else if (fInBkgTH1DHists[th1d][mc]->Integral() >= 0) fOutBkgTH1DStacksForUncer[th1d]->Add(fInBkgTH1DHists[th1d][mc]);

      // add other bkgs to the stack
      if (doMergeBkgs){//merged bkgs have already been added to the stack (here *)
        if (fBkgNames[mc]=="DiPhoton"){
          fOutBkgTH1DStacks[th1d]->Add(fInBkgTH1DHists[th1d][mc]); 
        }
      }
      else{
	if (doQCDscale && fBkgNames[mc] == "QCD"){ 
          fOutBkgTH1DStacks[th1d]->Add(GJetsClone[th1d]);
	}
        else if (fInBkgTH1DHists[th1d][mc]->Integral() >= 0) fOutBkgTH1DStacks[th1d]->Add(fInBkgTH1DHists[th1d][mc]);
      }

      // draw bkg in legend as box for stack plots, and line for overlay plot
      //if (doStack) fTH1DLegends[th1d]->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"f");
      //else fTH1DLegends[th1d]->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"l");
   }

   if (doMergeBkgs){
     fOutBkgTH1DHists[th1d] = (TH1D*)fInBkgTH1DHists[th1d][i_vh]->Clone();
     fOutBkgTH1DHists[th1d]->Add(fOutHiggsBkgTH1DHists[th1d]);
     fOutBkgTH1DHists[th1d]->Add(fOutEWK2phoBkgTH1DHists[th1d]);
     fOutBkgTH1DHists[th1d]->Add(fOutDYJetsBkgTH1DHists[th1d]);
     fOutBkgTH1DHists[th1d]->Add(fOutEWK1phoBkgTH1DHists[th1d]);
     fOutBkgTH1DHists[th1d]->Add(fOutJetsphoBkgTH1DHists[th1d]);
     for (UInt_t mc = 0; mc < fNBkg; mc++){
       //if (fBkgNames[mc]=="QCD"){
       //  if (doQCDscale) fOutBkgTH1DHists[th1d]->Add(GJetsClone[th1d]);
       //  else fOutBkgTH1DHists[th1d]->Add(fInBkgTH1DHists[th1d][mc]);
       //}
       if (fBkgNames[mc]=="DiPhoton") fOutBkgTH1DHists[th1d]->Add(fInBkgTH1DHists[th1d][mc]);
     }
   }
   else{
      for (UInt_t mc = 0; mc < fNBkg; mc++){ 
        if (mc == 0){
          fOutBkgTH1DHists[th1d] = (TH1D*)fInBkgTH1DHists[th1d][mc]->Clone();
        }
        else if (doQCDscale && fBkgNames[mc]=="QCD"){
          fOutBkgTH1DHists[th1d]->Add(GJetsClone[th1d]);
        }
        else{
          fOutBkgTH1DHists[th1d]->Add(fInBkgTH1DHists[th1d][mc]);
        }
      }
   }


    

    //if (fTH1DNames[th1d]=="mgg_IsolateALLmetCUT"){
    if (fTH1DNames[th1d]=="mgg_IsolateALLlowCUT"){
    //if (fTH1DNames[th1d]=="mgg_lowmet"){
    //if (fTH1DNames[th1d]=="mgg_IsolateALL"){
      std::ofstream	fOutTableTxtFile2;
      fOutTableTxtFile2.open(Form("%s/comb/IntegralsAfterAllCuts.tex",fOutDir.Data()));

      TStrMap fLatexSampleTitleMap;
      fLatexSampleTitleMap["QCD"] 			= "QCD";
      fLatexSampleTitleMap["GJets"]			= "$\\gamma$ + Jets";
      fLatexSampleTitleMap["VHToGG"]			= "V + h";
      fLatexSampleTitleMap["DYJetsToLL"]		= "Drell-Yan + Jets";
      fLatexSampleTitleMap["GluGluHToGG"]		= "$h \\rightarrow \\gamma \\gamma$ (ggh)";
      fLatexSampleTitleMap["DiPhoton"]			= "$\\gamma\\gamma$";
      fLatexSampleTitleMap["ttHJetToGG"]		= "tt $+ h \\rightarrow \\gamma\\gamma$";
      fLatexSampleTitleMap["VBFHToGG"]			= "VBF $h \\rightarrow \\gamma\\gamma$";
      fLatexSampleTitleMap["WGToLNuG"]			= "$\\gamma$ + W $\\rightarrow l \\nu$";
      fLatexSampleTitleMap["WJetsToLNu"]		= "W + Jets $\\rightarrow l\\nu$";
      fLatexSampleTitleMap["ZGTo2NuG"]			= "$\\gamma$ + Z $\\rightarrow \\nu\\nu$";
      fLatexSampleTitleMap["ZGTo2LG"]			= "$\\gamma$ + Z $\\rightarrow ll$";
      fLatexSampleTitleMap["ZJets"]			= "Z $\\rightarrow \\nu \\nu$ + Jets";
      fLatexSampleTitleMap["ZZTo2L2Nu"]			= "ZZ $\\rightarrow ll \\nu \\nu$";
      fLatexSampleTitleMap["WZTo2L2Q"]			= "WZ $\\rightarrow llqq$";
      fLatexSampleTitleMap["ZZTo2L2Q"]			= "ZZ $\\rightarrow llqq$";
      fLatexSampleTitleMap["TTGG_0Jets"]		= "tt + $\\gamma \\gamma$";
      fLatexSampleTitleMap["TTGJets"]			= "tt + $\\gamma$ + Jets";
      fLatexSampleTitleMap["TTJets"]			= "tt + Jets";
      fLatexSampleTitleMap["TGJets"]			= "t + $\\gamma$ + Jets";
      fLatexSampleTitleMap["2HDM_mZP600_mA0300"]	= "2HDM, $m_{Z'} = 600 GeV, m_{A0} = 300 GeV$";
      fLatexSampleTitleMap["2HDM_mZP800_mA0300"]	= "2HDM, $m_{Z'} = 800 GeV, m_{A0} = 300 GeV$";
      fLatexSampleTitleMap["2HDM_mZP1000_mA0300"]	= "2HDM, $m_{Z'} = 1000 GeV, m_{A0} = 300 GeV$";
      fLatexSampleTitleMap["2HDM_mZP1200_mA0300"]	= "2HDM, $m_{Z'} = 1200 GeV, m_{A0} = 300 GeV$";
      fLatexSampleTitleMap["2HDM_mZP1400_mA0300"]	= "2HDM, $m_{Z'} = 1400 GeV, m_{A0} = 300 GeV$";
      fLatexSampleTitleMap["2HDM_mZP1700_mA0300"]	= "2HDM, $m_{Z'} = 1700 GeV, m_{A0} = 300 GeV$";
      fLatexSampleTitleMap["2HDM_mZP2000_mA0300"]	= "2HDM, $m_{Z'} = 2000 GeV, m_{A0} = 300 GeV$";
      fLatexSampleTitleMap["2HDM_mZP2500_mA0300"]	= "2HDM, $m_{Z'} = 2500 GeV, m_{A0} = 300 GeV$";
      fLatexSampleTitleMap["2HDM_mZP600_mA0400"]	= "2HDM, $m_{Z'} = 600 GeV, m_{A0} = 400 GeV$";
      fLatexSampleTitleMap["2HDM_mZP800_mA0400"]	= "2HDM, $m_{Z'} = 800 GeV, m_{A0} = 400 GeV$";

      UInt_t sbbin0 = fOutDataTH1DHists[th1d]->GetXaxis()->FindBin(105.);
      UInt_t sbbin1 = fOutDataTH1DHists[th1d]->GetXaxis()->FindBin(115.);
      UInt_t sbbin2 = fOutDataTH1DHists[th1d]->GetXaxis()->FindBin(135.);
      UInt_t sbbin3 = fOutDataTH1DHists[th1d]->GetXaxis()->FindBin(180.);

      std::vector<Double_t> fSigInt;
      std::vector<Double_t> fSigIntErr;
      fSigInt.resize(fNSig);
      fSigIntErr.resize(fNSig);

      std::vector<Double_t> fBkgInt2;
      std::vector<Double_t> fBkgIntErr2;
      fBkgInt2.resize(fNBkg);
      fBkgIntErr2.resize(fNBkg);
      Float_t fSignalRegInt2 = 0.;
      Float_t fSignalRegIntErr2 = 0.;

      if (fOutTableTxtFile2.is_open()){
        //setup Latex doc
        fOutTableTxtFile2 << "\\documentclass[a4paper,landscape]{article}" << std::endl;
        fOutTableTxtFile2 << "\\usepackage[paperheight=15.0in,paperwidth=6.0in,margin=1.0in,headheight=0.0in,footskip=0.5in,includehead,includefoot]{geometry}" << std::endl;
        fOutTableTxtFile2 << "\\begin{document}" << std::endl;

        // ==========================================================
        // start summary of results table
        fOutTableTxtFile2 << "\% Integrals " << std::endl; 
        fOutTableTxtFile2 << "\\begin{table}[bthp]" <<std::endl;
        fOutTableTxtFile2 << "\\begin{tabular}{|l|r|}" <<std::endl;
        fOutTableTxtFile2 << "\\hline \\hline" <<std::endl;
        fOutTableTxtFile2 << "\\multicolumn{2}{|c|}{" << Form("$\\sqrt{s}$ = 13 TeV; L = %1.1f $fb^{-1}$",lumi) <<"} \\\\" <<std::endl;
        fOutTableTxtFile2 << "\\hline \\hline" << std::endl;
        fOutTableTxtFile2 << "Sample & Integral \\\\" << std::endl;
        fOutTableTxtFile2 << "\\hline" <<std::endl;

        for (UInt_t mc = 0; mc < fNSig; mc++){
          fSigInt[mc] = fInSigTH1DHists[th1d][mc]->IntegralAndError(sbbin1,sbbin2,fSigIntErr[mc]);
          //fOutTableTxtFile2 << fSampleTitleMap[fSigNames[mc]] << " & " << fSigInt[mc] << " \\pm " << fSigIntErr[mc] << "\\\\" << std::endl; 
        }

        for (UInt_t mc = 0; mc < fNBkg; mc++){
          fBkgInt2[mc] = fInBkgTH1DHists[th1d][mc]->IntegralAndError(sbbin1,sbbin2,fBkgIntErr2[mc]);
          //fOutTableTxtFile2 << fSampleTitleMap[fBkgNames[mc]] << " & " << fBkgInt2[mc] << " \\pm " << fBkgIntErr2[mc] << "\\\\" << std::endl; 
          //std::cout << " -- B -- " << fBkgNames[mc] << " = " << fBkgInt2[mc] << " +/- " << fBkgIntErr2[mc] << std::endl; 
          if (fBkgInt2[mc] > 0){
            fSignalRegInt2 += fBkgInt2[mc];
            fSignalRegIntErr2 += fBkgIntErr2[mc]*fBkgIntErr2[mc];
          }
        }

        //fOutTableTxtFile2 << "Total Bkg &" << fSignalRegInt2 << " \\pm " << TMath::Sqrt(fSignalRegIntErr2) << "\\\\" << std::endl; 
        //fOutTableTxtFile2 << "Sqrt Bkg &" << TMath::Sqrt(fSignalRegInt2) << std::endl; 
        //for (UInt_t mc = 0; mc < fNSig; mc++) std::cout << " -- Sig -- " << fSigInt[mc]/TMath::Sqrt(fSigInt[mc]+fSignalRegInt2) << std::endl; 

        UInt_t bin0 = fOutDataTH1DHists[th1d]->GetXaxis()->FindBin(105.);
        UInt_t bin1 = fOutDataTH1DHists[th1d]->GetXaxis()->FindBin(115.);
        UInt_t bin2 = fOutDataTH1DHists[th1d]->GetXaxis()->FindBin(135.);
        UInt_t bin3 = fOutDataTH1DHists[th1d]->GetXaxis()->FindBin(180.);

        Float_t fSignalRegInt = 0.;
        Float_t fSignalRegIntErr = 0.;
        Float_t fSigRegionInt = 0.;
        Float_t fSigRegionIntErr = 0.;
        Float_t fSideBandRegInt = 0.;
        Float_t fSideBandRegIntErr = 0.;
        std::vector<Double_t> fBkgSigInt;
        std::vector<Double_t> fBkgSigIntErr;
        std::vector<Double_t> fBkgInt;
        std::vector<Double_t> fBkgIntErr;

        fBkgInt.resize(fNBkg);
        fBkgIntErr.resize(fNBkg);
        fBkgSigInt.resize(fNBkg);
        fBkgSigIntErr.resize(fNBkg);

        // calculate integrals for background
        for (UInt_t mc = 0; mc < fNBkg; mc++){
          fBkgInt[mc]    = fInBkgTH1DHists[th1d][mc]->IntegralAndError(bin0,bin3,fBkgIntErr[mc]);
          fBkgSigInt[mc] = fInBkgTH1DHists[th1d][mc]->IntegralAndError(bin1,bin2,fBkgSigIntErr[mc]);
          fOutTableTxtFile2 << fLatexSampleTitleMap[fBkgNames[mc]] << " & " << Form("%3.2f",fBkgInt[mc]) << " $\\pm$ " << Form("%3.2f",fBkgIntErr[mc]) << "\\\\" << std::endl; 
          if (fBkgSigInt[mc] > 0){
	    fSigRegionInt += fBkgSigInt[mc];
            fSigRegionIntErr += fBkgSigIntErr[mc];
          }
          if (fBkgInt[mc] > 0){
            fSignalRegInt += fBkgInt[mc];
            fSignalRegIntErr += fBkgIntErr[mc];
          }
        }
        fSideBandRegInt    = fSignalRegInt - fSigRegionInt;
        fSideBandRegIntErr = fSignalRegIntErr - fSigRegionIntErr; 
        fOutTableTxtFile2 << "\\hline" << std::endl;
        fOutTableTxtFile2 << "Total Bkg & " << Form("%3.2f",fSignalRegInt) << " $\\pm$ " << Form("%3.2f",fSignalRegIntErr) << " \\\\" << std::endl; 
        fOutTableTxtFile2 << "Bkg (sideband) & " << Form("%3.2f",fSideBandRegInt) << "$\\pm$ " << Form("%3.2f",fSideBandRegIntErr) << " \\\\" << std::endl;

        // calculate integrals for data
        Double_t DataInt1, DataErr1, DataInt2, DataErr2;
        Double_t DataInt, DataIntErr;
        DataInt1 = fOutDataTH1DHists[th1d]->IntegralAndError(bin0,bin1,DataErr1);
        DataInt2 = fOutDataTH1DHists[th1d]->IntegralAndError(bin2,bin3,DataErr2);
        DataInt = DataInt1 + DataInt2;
        DataIntErr = TMath::Sqrt(DataInt);

        Double_t DataErrTot = 0; 
        Double_t DataIntTot = fOutDataTH1DHists[th1d]->IntegralAndError(bin0,bin3,DataErrTot);
        fOutTableTxtFile2 << "\\hline" << std::endl;
        fOutTableTxtFile2 << "Total Data & "   << DataIntTot << " $\\pm$ " << DataErrTot << " \\\\" << std::endl; 
        fOutTableTxtFile2 << "Data (sideband) & " << DataInt << " $\\pm$ " << DataIntErr << " \\\\" << std::endl; 
        fOutTableTxtFile2 << "\\hline" << std::endl;

        // calculate integrals for signal
        Double_t SigIntegral, SigIntegralErr;
        for (UInt_t mc = 0; mc < fNSig; mc++){
          SigIntegral = fInSigTH1DHists[th1d][mc]->IntegralAndError(bin0,bin3,SigIntegralErr);
          fOutTableTxtFile2 << fLatexSampleTitleMap[fSigNames[mc]] << " & " << SigIntegral << " $\\pm$ " << SigIntegralErr << "\\\\" << std::endl; 
        }
      fOutTableTxtFile2 << "\\hline" << std::endl;
      fOutTableTxtFile2 << "\\end{tabular}" <<std::endl;
      fOutTableTxtFile2 << "\\end{table}" <<std::endl;

      // ==========================================================
      // end Latex doc
      fOutTableTxtFile2 << "\\end{document}" <<std::endl;
      std::cout << "Writing ResultsTable in " << Form("%s/comb/IntegralsAfterAllCuts.tex",fOutDir.Data()) << std::endl;
      }
      else std::cout << "File didn't Open" << std::endl;
      // close output text files
      fOutTableTxtFile2.close();

    }


    // print out efficiencies for fake MET systematic 
    if (fTH1DNames[th1d]=="metCorr_IsolateALL"){
      UInt_t binMETze = fOutDataTH1DHists[th1d]->GetXaxis()->FindBin(0.);
      UInt_t binMETlo = fOutDataTH1DHists[th1d]->GetXaxis()->FindBin(METcut);
      UInt_t binMEThi = fOutDataTH1DHists[th1d]->GetXaxis()->FindBin(299.);

      Float_t fTotalBkgInt_metCUT = 0.;
      Float_t fTotalBkgInt_metall = 0.;
      Float_t fTotalBkgEfficiency = 0.;
      for (UInt_t mc = 0; mc < fNBkg; mc++){
        if (addText!="_n-1"){
	  //std::cout << " *** " << fBkgNames[mc] << " *** " << std::endl;
          ////std::cout << "Events in MET tail of CorrMET + ALL Iso = " << fInBkgTH1DHists[th1d][mc]->Integral(binMETlo,binMEThi) << std::endl;
          ////std::cout << "Events in all MET  of CorrMET + ALL Iso = " << fInBkgTH1DHists[th1d][mc]->Integral(binMETze,binMEThi) << std::endl;
          ////std::cout << "Efficiency                              = " << fInBkgTH1DHists[th1d][mc]->Integral(binMETlo,binMEThi)/fInBkgTH1DHists[th1d][mc]->Integral(binMETze,binMEThi
	  //if (fInBkgTH1DHists[th1d][mc]->Integral(binMETlo,binMEThi) >= 0 && fInBkgTH1DHists[th1d][mc]->Integral(binMETze,binMEThi) >= 0){
	  //  std::cout << "Events in MET tail of CorrMET + ALL Iso	= " << fInBkgTH1DHists[th1d][mc]->Integral(binMETlo,binMEThi) << std::endl;
	  //  std::cout << "Events in all MET  of CorrMET + ALL Iso	= " << fInBkgTH1DHists[th1d][mc]->Integral(binMETze,binMEThi) << std::endl;
	  //  std::cout << "Efficiency 				= " << fInBkgTH1DHists[th1d][mc]->Integral(binMETlo,binMEThi)/fInBkgTH1DHists[th1d][mc]->Integral(binMETze,binMEThi) << std::endl;
	  //}
	  //else std::cout << "Events in distributions is less than 0." << std::endl;
	  //// sum all backgrounds to get overall efficiency 
	  if (fInBkgTH1DHists[th1d][mc]->Integral(binMETlo,binMEThi) > 0) fTotalBkgInt_metCUT  += fInBkgTH1DHists[th1d][mc]->Integral(binMETlo,binMEThi); 
	  if (fInBkgTH1DHists[th1d][mc]->Integral(binMETze,binMEThi) > 0) fTotalBkgInt_metall += fInBkgTH1DHists[th1d][mc]->Integral(binMETze,binMEThi); 
	}
      }

      //// efficiency for data
      //Float_t fDataEfficiency = fOutDataTH1DHists[th1d]->Integral(binMETlo,binMEThi)/fOutDataTH1DHists[th1d]->Integral(binMETze,binMEThi); 
      //std::cout << " *** Data *** " << std::endl; 
      //std::cout << "Events in MET tail of CorrMET + ALL Iso	= " << fOutDataTH1DHists[th1d]->Integral(binMETlo,binMEThi) << std::endl;
      //std::cout << "Events in all MET  of CorrMET + ALL Iso	= " << fOutDataTH1DHists[th1d]->Integral(binMETze,binMEThi) << std::endl;
      //std::cout << "Efficiency 					= " << fDataEfficiency << std::endl; 
      //// efficiency for total bkg MC
      //fTotalBkgEfficiency = fTotalBkgInt_metCUT/fTotalBkgInt_metall;
      //std::cout << " *** Total Bkg *** " << std::endl;
      //std::cout << "Events in MET tail of CorrMET + ALL Iso	= " << fTotalBkgInt_metCUT  << std::endl;
      //std::cout << "Events in all MET  of CorrMET + ALL Iso	= " << fTotalBkgInt_metall << std::endl;
      //std::cout << "Efficiency 				= " << fTotalBkgEfficiency << std::endl; 
      //// ratio of eff data/ eff bkg MC 
      //std::cout << " *** Scale Factor = " << fDataEfficiency/fTotalBkgEfficiency << " *** " << std::endl; 

    }

    if (th1d==fIndexMgg){
      for (UInt_t mc = 0; mc < fNBkg; mc++){// print out values of integrals
        if (addText!="_n-1"){
          std::cout << fBkgNames[mc] << " in " << fTH1DNames[th1d] << " = " << fInBkgTH1DHists[th1d][mc]->Integral() 
          << " %: " << fInBkgTH1DHists[th1d][mc]->Integral()/fOutBkgTH1DHists[th1d]->Integral() << std::endl;
        }
      }
    }
 
    //if (th1d==fIndexNvtx && addText!="_n-1"){
    //  std::cout << "DoubleEG" << " in " << fTH1DNames[th1d] << " = " << fOutDataTH1DHists[th1d]->Integral()  << std::endl;
    //  for (UInt_t mc = 0; mc < fNBkg; mc++){// print out values of integrals
    //    std::cout << fBkgNames[mc] << " in " << fTH1DNames[th1d] << " = " << fInBkgTH1DHists[th1d][mc]->Integral()  << std::endl;
    //  }
    //  for (UInt_t mc = 0; mc < fNSig; mc++){// print out values of integrals
    //    std::cout << fSigNames[mc] << " in " << fTH1DNames[th1d] << " = " << fInSigTH1DHists[th1d][mc]->Integral()  << std::endl;
    //  }
    //} 

    //fOutBkgTH1DHists[th1d]->Sumw2();
    //std::cout << "histo# " << th1d << std::endl;
    //std::cout << "maxbin " << fOutBkgTH1DHists[th1d]->GetSize() << std::endl;
    //for (UInt_t bin = 1; bin < fOutBkgTH1DHists[th1d]->GetSize(); bin++){
    //  std::cout << "bin " << bin << " Val = " << fOutBkgTH1DHists[th1d]->GetBinContent(bin) << " Err = " << fOutBkgTH1DHists[th1d]->GetBinError(bin) << std::endl;
    //}
    fOutBkgTH1DHists[th1d]->SetFillColor(kGray+3);
    fOutBkgTH1DHists[th1d]->SetLineColor(kGray+3);
    fOutBkgTH1DHists[th1d]->SetFillStyle(3013);
    //fOutBkgTH1DHists[th1d]->SetFillStyle(3003);
    fOutBkgTH1DHists[th1d]->SetMarkerSize(0);

    // LEGEND 
    if (doMergeBkgs){
      //-------style legend---------
      // data		DY
      // vh		ewk+1pho
      // smhgg		pho+jets
      // ewk+2pho	dipho
      // stat.unc.
      //----------------------------

      // add data to legend if int > 0
      Double_t dataInt = fOutDataTH1DHists[th1d]->Integral();
      if (fNData > 0 && doStack && dataInt > 0) fTH1DLegends[th1d]->AddEntry(fOutDataTH1DHists[th1d],"Data","p");
      if (doStack) fTH1DLegends[th1d]->AddEntry(fOutDYJetsBkgTH1DHists[th1d],fSampleTitleMap["DYJetsToLL"],"f");
      else fTH1DLegends[th1d]->AddEntry(fOutDYJetsBkgTH1DHists[th1d],fSampleTitleMap["DYJetsToLL"],"l");

      for (UInt_t mc = 0; mc < fNBkg; mc++){
	if (fBkgNames[mc]=="VHToGG"){
          if (doStack) fTH1DLegends[th1d]->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"f");
          else fTH1DLegends[th1d]->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"l");
	}
      }

      if (doStack){
        fTH1DLegends[th1d]->AddEntry(fOutEWK1phoBkgTH1DHists[th1d],fSampleTitleMap["EWK1pho"],"f");
        fTH1DLegends[th1d]->AddEntry(fOutHiggsBkgTH1DHists[th1d],fSampleTitleMap["SMHiggs"],"f");
        fTH1DLegends[th1d]->AddEntry(fOutJetsphoBkgTH1DHists[th1d],fSampleTitleMap["Jetspho"],"f");
        fTH1DLegends[th1d]->AddEntry(fOutEWK2phoBkgTH1DHists[th1d],fSampleTitleMap["EWK2pho"],"f");
      }
      else{
        fTH1DLegends[th1d]->AddEntry(fOutEWK1phoBkgTH1DHists[th1d],fSampleTitleMap["EWK1pho"],"l");
        fTH1DLegends[th1d]->AddEntry(fOutHiggsBkgTH1DHists[th1d],fSampleTitleMap["SMHiggs"],"l");
        fTH1DLegends[th1d]->AddEntry(fOutJetsphoBkgTH1DHists[th1d],fSampleTitleMap["Jetspho"],"l");
        fTH1DLegends[th1d]->AddEntry(fOutEWK2phoBkgTH1DHists[th1d],fSampleTitleMap["EWK2pho"],"l");
      }
      for (UInt_t mc = 0; mc < fNBkg; mc++){
	if (fBkgNames[mc]=="DiPhoton"){
          if (doStack) fTH1DLegends[th1d]->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"f");
          else fTH1DLegends[th1d]->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"l");
	}
      }

      // add uncertainty in stack plot
      fOutBkgTH1DStacksForUncer[th1d]->Add(fOutBkgTH1DHists[th1d],"E2");
      if (doStack) fTH1DLegends[th1d]->AddEntry(fOutBkgTH1DHists[th1d],"Stat. Unc.","F");
      for (UInt_t mc = 0; mc < fNSig; mc++){
	fTH1DLegendsSig[th1d]->AddEntry(fInSigTH1DHists[th1d][mc],fSampleTitleMap[fSigNames[mc]],"l");
      }
    }

    else{
      UInt_t fNMaxMC = 0;
      if (fNSig <= fNBkg) fNMaxMC = fNBkg;
      else fNMaxMC = fNSig;
      for (UInt_t mc = 0; mc < fNMaxMC; mc++){
        if (mc < fNBkg){
          if (doStack) fTH1DLegends[th1d]->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"f");
          else fTH1DLegends[th1d]->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"l");
        }
        if (mc < fNSig) fTH1DLegendsSig[th1d]->AddEntry(fInSigTH1DHists[th1d][mc],fSampleTitleMap[fSigNames[mc]],"l");
      } 
      // add uncertainty in stack plot
      fOutBkgTH1DStacksForUncer[th1d]->Add(fOutBkgTH1DHists[th1d],"E2");
      if (doStack) fTH1DLegends[th1d]->AddEntry(fOutBkgTH1DHists[th1d],"Stat. Unc.","F");
      // add data to legend if int > 0
      Double_t dataInt = fOutDataTH1DHists[th1d]->Integral();
      if (fNData > 0 && doStack && dataInt > 0) fTH1DLegends[th1d]->AddEntry(fOutDataTH1DHists[th1d],"Data","p");


      //// sig: just add to legend
      //for (UInt_t mc = 0; mc < fNSig; mc++){
      //  //fInSigTH1DHists[th1d][mc]->Scale(lumi);
      //  fTH1DLegends[th1d]->AddEntry(fInSigTH1DHists[th1d][mc],fSampleTitleMap[fSigNames[mc]],"l");
      //}
      //// bkg : copy histos and add to stacks
      //for (UInt_t mc = 0; mc < fNBkg; mc++){
      //  // draw bkg in legend as box for stack plots, and line for overlay plot
      //  if (doStack) fTH1DLegends[th1d]->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"f");
      //  else fTH1DLegends[th1d]->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"l");
      //}
    }// not merged bkgs


  }// end loop over th1d histos

  if (addText!="_n-1") Combiner::MakeEffPlots();

  // look at photon ID efficienes (only relevant for MVA samples)   
  //for (UInt_t mc = 0; mc < fNSig; mc++){
  //  if (addText!="_n-1") Combiner::PhotonIDEfficiencies(mc,0);
  //}
  //for (UInt_t mc = 0; mc < fNBkg; mc++){
  //  if (addText!="_n-1") Combiner::PhotonIDEfficiencies(mc,1);
  //}

  //if (addText!="_n-1") Combiner::VtxEfficiencies();
 
  // look at the MET efficiencies after different corrections
  if (addText!="_n-1") Combiner::FindMETEfficiencies();
  
  Combiner::MakeOutputCanvas();

}// end Combiner::DoComb

void Combiner::GetEfficiency(TH1D *h_num, TH1D *h_den, TString name, TString sample){
  if (TEfficiency::CheckConsistency(*h_num,*h_den)){
    TEfficiency *tmpeff = new TEfficiency(*h_num,*h_den);
    TCanvas * c1 = new TCanvas();
    c1->cd();
    tmpeff->Draw("AP");
    CMSLumi(c1,11,lumi);
    c1->SaveAs(Form("%scomb/%s_%s.%s",fOutDir.Data(),name.Data(),sample.Data(),fType.Data()));
    delete tmpeff;
    delete c1;
  }
  else std::cout << "Check Consistency for Efficiency Plot Fails" << std::endl;
}// end Combiner::MakeEfficiencyPlot




void Combiner::VtxEfficiencies(){
  // pickup the vtx efficiency histograms
  UInt_t th1d_nvtx_n, th1d_nvtx_d;
  UInt_t th1d_met_n, th1d_met_d;
  UInt_t th1d_ptzp_n, th1d_ptzp_d;
  UInt_t th1d_njet_n, th1d_njet_d;
  for (UInt_t th1d = 0; th1d < fNTH1D; th1d++){
    if (fTH1DNames[th1d]=="vtx_eff_ptzp_n") th1d_ptzp_n = th1d;
    if (fTH1DNames[th1d]=="vtx_eff_ptzp_d") th1d_ptzp_d = th1d;
    if (fTH1DNames[th1d]=="vtx_eff_nvtx_n") th1d_nvtx_n = th1d;
    if (fTH1DNames[th1d]=="vtx_eff_nvtx_d") th1d_nvtx_d = th1d;
    if (fTH1DNames[th1d]=="vtx_eff_met_n")  th1d_met_n	= th1d; 
    if (fTH1DNames[th1d]=="vtx_eff_met_d")  th1d_met_d	= th1d;
    if (fTH1DNames[th1d]=="vtx_eff_njet_n") th1d_njet_n = th1d;
    if (fTH1DNames[th1d]=="vtx_eff_njet_d") th1d_njet_d = th1d;
  }

  // vtx efficiency plots for signal
  for (UInt_t mc = 0; mc < fNSig; mc++){
    Combiner::GetEfficiency(fInSigTH1DHists[th1d_ptzp_n][mc],fInSigTH1DHists[th1d_ptzp_d][mc],"VtxEff_ptzp",fSigNames[mc].Data());
    Combiner::GetEfficiency(fInSigTH1DHists[th1d_nvtx_n][mc],fInSigTH1DHists[th1d_nvtx_d][mc],"VtxEff_nvtx",fSigNames[mc].Data());
    Combiner::GetEfficiency(fInSigTH1DHists[th1d_met_n][mc],fInSigTH1DHists[th1d_met_d][mc],"VtxEff_met",fSigNames[mc].Data());
    Combiner::GetEfficiency(fInSigTH1DHists[th1d_njet_n][mc],fInSigTH1DHists[th1d_njet_d][mc],"VtxEff_njet",fSigNames[mc].Data());
  }

  // vtx efficiency plots for background
  for (UInt_t mc = 0; mc < fNBkg; mc++){
    Combiner::GetEfficiency(fInBkgTH1DHists[th1d_ptzp_n][mc],fInBkgTH1DHists[th1d_ptzp_d][mc],"VtxEff_ptzp",fBkgNames[mc].Data());
    Combiner::GetEfficiency(fInBkgTH1DHists[th1d_nvtx_n][mc],fInBkgTH1DHists[th1d_nvtx_d][mc],"VtxEff_nvtx",fBkgNames[mc].Data());
    Combiner::GetEfficiency(fInBkgTH1DHists[th1d_met_n][mc],fInBkgTH1DHists[th1d_met_d][mc],"VtxEff_met",fBkgNames[mc].Data());
    Combiner::GetEfficiency(fInBkgTH1DHists[th1d_njet_n][mc],fInBkgTH1DHists[th1d_njet_d][mc],"VtxEff_njet",fBkgNames[mc].Data());
  }

}// end Combiner::VtxEfficiencies


void Combiner::PhotonIDEfficiencies(const UInt_t mc, const UInt_t isType){
  // Find the efficiency for photons passing loose selection that have passed the MVA selection
  UInt_t th1d_pt1_n, th1d_pt1_d;
  UInt_t th1d_eta1_n, th1d_eta1_d;
  for (UInt_t th1d = 0; th1d < fNTH1D; th1d++){
    if (fTH1DNames[th1d]=="pt1_afterIDloose")	th1d_pt1_n = th1d;
    if (fTH1DNames[th1d]=="pt1_beforeIDloose")	th1d_pt1_d = th1d;
    if (fTH1DNames[th1d]=="eta1_afterIDloose")	th1d_eta1_n = th1d;
    if (fTH1DNames[th1d]=="eta1_beforeIDloose")	th1d_eta1_d = th1d;
  }

  if (isType==0){
    Combiner::GetEfficiency(fInSigTH1DHists[th1d_pt1_n][mc],fInSigTH1DHists[th1d_pt1_d][mc],"phoIDeff_pt",fSigNames[mc].Data());
    Combiner::GetEfficiency(fInSigTH1DHists[th1d_eta1_n][mc],fInSigTH1DHists[th1d_eta1_d][mc],"phoIDeff_eta",fSigNames[mc].Data());
  }
  else if (isType==1){
    Combiner::GetEfficiency(fInBkgTH1DHists[th1d_pt1_n][mc],fInBkgTH1DHists[th1d_pt1_d][mc],"phoIDeff_pt",fBkgNames[mc].Data());
    Combiner::GetEfficiency(fInBkgTH1DHists[th1d_eta1_n][mc],fInBkgTH1DHists[th1d_eta1_d][mc],"phoIDeff_eta",fBkgNames[mc].Data());
  }
  else std::cout << "Type is not Sig or Bkg MC" << std::endl;
 
}// end Combiner::PhotonIDEfficiencies


void Combiner::FindMETEfficiencies(){
  // Finds the efficiency for each sample (integral of events passing 80 GeV cut over all selected events)
  // Integrals are computed also including the overflow bin
  // Also plots for each sample the MET shape for the different corrections to visualize the effect

  Float_t metcut = 130;

  fNMETPlots = 15;
  UInt_t fNMETCat = fNMETPlots*2;

  DblVecVec fSigMETEff;
  DblVecVec fBkgMETEff;
  DblVec    fDataMETEff;
  fSigMETEff.resize(fNSig);
  fBkgMETEff.resize(fNBkg);

  DblVecVec fSigMET;
  DblVecVec fBkgMET;
  DblVec    fDataMET;
  fSigMET.resize(fNSig);
  fBkgMET.resize(fNBkg);
   

  for (UInt_t mc = 0; mc < fNSig; mc++){
    fSigMETEff[mc].resize(fNMETCat); 
    fSigMET[mc].resize(fNMETCat); 
    for (UInt_t th1d = fIndexMET; th1d < (fIndexMET+fNMETPlots); th1d++){
      UInt_t maxbin = fInSigTH1DHists[th1d][mc]->GetSize(); 
      UInt_t minbin = fInSigTH1DHists[th1d][mc]->GetXaxis()->FindBin(metcut);
      UInt_t zerbin = fInSigTH1DHists[th1d][mc]->GetXaxis()->FindBin(0.0);
      UInt_t effbin1 = th1d-fIndexMET;
      UInt_t effbin2 = th1d-fIndexMET+fNMETPlots;
      fSigMETEff[mc][th1d-fIndexMET+fNMETPlots] = fInSigTH1DHists[th1d][mc]->Integral(minbin,maxbin);// events above 80 GeV
      fSigMETEff[mc][th1d-fIndexMET] = fInSigTH1DHists[th1d][mc]->Integral(zerbin,maxbin);// all events 
      if (fSigMETEff[mc][th1d-fIndexMET] > 0) fSigMET[mc][th1d-fIndexMET] = (fSigMETEff[mc][th1d-fIndexMET+fNMETPlots]/fSigMETEff[mc][th1d-fIndexMET]);
      else fSigMET[mc][th1d-fIndexMET]=0; 
    }
  }  
  for (UInt_t mc = 0; mc < fNBkg; mc++){
    fBkgMETEff[mc].resize(fNMETCat); 
    fBkgMET[mc].resize(fNMETCat); 
    for (UInt_t th1d = fIndexMET; th1d < (fIndexMET+fNMETPlots); th1d++){
      UInt_t maxbin = fInBkgTH1DHists[th1d][mc]->GetSize(); 
      UInt_t minbin = fInBkgTH1DHists[th1d][mc]->GetXaxis()->FindBin(metcut);
      UInt_t effbin1 = th1d-fIndexMET;
      UInt_t effbin2 = th1d-fIndexMET+fNMETPlots;
      fBkgMETEff[mc][th1d-fIndexMET+fNMETPlots] = fInBkgTH1DHists[th1d][mc]->Integral(minbin,maxbin);// events above 80 GeV
      fBkgMETEff[mc][th1d-fIndexMET] = fInBkgTH1DHists[th1d][mc]->Integral();// all events
      if (fBkgMETEff[mc][th1d-fIndexMET] > 0) fBkgMET[mc][th1d-fIndexMET] = fBkgMETEff[mc][th1d-fIndexMET+fNMETPlots]/fBkgMETEff[mc][th1d-fIndexMET];
      else fBkgMET[mc][th1d-fIndexMET]=0; 
    }
  }  
  fDataMETEff.resize(fNMETCat);
  fDataMET.resize(fNMETCat);
  for (UInt_t th1d = fIndexMET; th1d < (fIndexMET+fNMETPlots); th1d++){
    UInt_t maxbin = fOutDataTH1DHists[th1d]->GetSize(); 
    UInt_t minbin = fOutDataTH1DHists[th1d]->GetXaxis()->FindBin(metcut);
    UInt_t effbin1 = th1d-fIndexMET;
    UInt_t effbin2 = th1d-fIndexMET+fNMETPlots;
    fDataMETEff[th1d-fIndexMET+fNMETPlots] = fOutDataTH1DHists[th1d]->Integral(minbin,maxbin);// events above 80 GeV
    fDataMETEff[th1d-fIndexMET] = fOutDataTH1DHists[th1d]->Integral();// all events
    if (fDataMETEff[th1d-fIndexMET] > 0) fDataMET[th1d-fIndexMET] = fDataMETEff[th1d-fIndexMET+fNMETPlots]/fDataMETEff[th1d-fIndexMET];
    else fDataMET[th1d-fIndexMET]=0; 
  }

  SystMET.push_back("Original");
  SystMET.push_back("JetEnUp");
  SystMET.push_back("JetEnDown");
  SystMET.push_back("JetResUp");
  SystMET.push_back("JetResDown");
  SystMET.push_back("MuonEnUp");
  SystMET.push_back("MuonEnDown");
  SystMET.push_back("EleEnUp");
  SystMET.push_back("EleEnDown");
  SystMET.push_back("TauEnUp");
  SystMET.push_back("TauEnDown");
  SystMET.push_back("PhoEnUp");
  SystMET.push_back("PhoEnDown");
  SystMET.push_back("JetUnclEnUp");
  SystMET.push_back("JetUnclEnDown");
 
  fSystMETTitleMap["Original"]		= "Original";  
  fSystMETTitleMap["JetEnUp"] 		= "Jet Energy Up"; 
  fSystMETTitleMap["JetEnDown"]		= "Jet Energy Down"; 
  fSystMETTitleMap["JetResUp"] 		= "Jet Res. Up"; 
  fSystMETTitleMap["JetResDown"]	= "Jet Res. Down"; 
  fSystMETTitleMap["MuonEnUp"]		= "Muon Energy Up"; 
  fSystMETTitleMap["MuonEnDown"]	= "Muon Energy Down"; 
  fSystMETTitleMap["EleEnUp"]		= "Electron Energy Up"; 
  fSystMETTitleMap["EleEnDown"]		= "Electron Energy Down"; 
  fSystMETTitleMap["TauEnUp"]		= "Tau Energy Up"; 
  fSystMETTitleMap["TauEnDown"]		= "Tau Energy Down"; 
  fSystMETTitleMap["PhoEnUp"]		= "Photon Energy Up"; 
  fSystMETTitleMap["PhoEnDown"] 	= "Photon Energy Down"; 
  fSystMETTitleMap["JetUnclEnUp"]	= "Unclust. Jet Energy Up"; 
  fSystMETTitleMap["JetUnclEnDown"] 	= "Unclust. Jet Energy Down"; 


  //std::cout << " ---------------------------------------------------------------- " << std::endl;
  //std::cout << " ---------------------------------------------------------------- " << std::endl;
  //std::cout << "MET Efficiencies for Systematics " << std::endl; 
  //std::cout << " ---------------------------------------------------------------- " << std::endl;
  //std::cout << " ---------------------------------------------------------------- " << std::endl;
  ////for (UInt_t i=0; i < fNMETPlots; i++){
  ////  std::cout << " Data		: " << SystMET[i] << "		= " << fDataMETEff[i+fNMETPlots] << " / " << fDataMETEff[i] << "	= " << fDataMET[i] << std::endl;
  ////}
  ////std::cout << " ---------------------------------------------------------------- " << std::endl;
  //for (UInt_t mc=0; mc < fNSig; mc++){
  //  for (UInt_t i=0; i < fNMETPlots; i++){
  //    std::cout << fSigNames[mc] << " 	: " << SystMET[i] << "		= " << fSigMETEff[mc][i+fNMETPlots] << " / " << fSigMETEff[mc][i] << "	= " << fSigMET[mc][i] << std::endl;
  //  }
  //  std::cout << " ---------------------------------------------------------------- " << std::endl;
  //}
  //for (UInt_t mc=0; mc < fNBkg; mc++){
  //  for (UInt_t i=0; i < fNMETPlots; i++){
  //    std::cout << fBkgNames[mc] << " 	: " << SystMET[i] << "		= " << fBkgMETEff[mc][i+fNMETPlots] << " / " << fBkgMETEff[mc][i] << "	= " << fBkgMET[mc][i] << std::endl;
  //  }
  //  std::cout << " ---------------------------------------------------------------- " << std::endl;
  //}
  //std::cout << " ---------------------------------------------------------------- " << std::endl;

  // write as a table for Latex
  std::ofstream	fOutTableTxtFile;
  fOutTableTxtFile.open(Form("%s/comb/ResultsSystematicsForLatex.tex",fOutDir.Data()));

  TStrMap fSampleTitleMap;
  fSampleTitleMap["QCD"] 		= "QCD";
  fSampleTitleMap["GJets"]		= "$\\gamma$ + Jets";
  fSampleTitleMap["VHToGG"]		= "V + H";
  fSampleTitleMap["DYJetsToLL"]		= "Drell-Yan";
  fSampleTitleMap["GluGluHToGG"]	= "$H \\rightarrow \\gamma \\gamma$ (ggH)";
  fSampleTitleMap["DiPhoton"]		= "$\\gamma\\gamma$";
  fSampleTitleMap["ttHJetToGG"]		= "tt $+ H \\rightarrow \\gamma\\gamma$";
  fSampleTitleMap["VBFHToGG"]		= "VBF $H \\rightarrow \\gamma\\gamma$";
  fSampleTitleMap["WGToLNuG"]		= "$\\gamma$ + W $\\rightarrow l \\nu$";
  fSampleTitleMap["ZGTo2NuG"]		= "$\\gamma$ + Z $\\rightarrow \\nu\\nu$";
  fSampleTitleMap["ZGTo2LG"]		= "$\\gamma$ + Z $\\rightarrow ll$";
  fSampleTitleMap["ZJets"]		= "Z $\\rightarrow \\nu \\nu$ + Jets";
  fSampleTitleMap["ZZTo2L2Nu"]		= "Z + Z $\\rightarrow ll \\nu\\nu$";
  fSampleTitleMap["TTGJets"]		= "tt + $\\gamma$ + Jets";
  fSampleTitleMap["TTGG_0Jets"]		= "tt + $\\gamma\\gamma$";
  fSampleTitleMap["TGJets"]		= "t + $\\gamma$ + Jets";
  fSampleTitleMap["2HDM_mZP600_mA0300"]	= "2HDM, $m_{Z'} = 600 GeV, m_{A0} = 300 GeV$";
  fSampleTitleMap["2HDM_mZP800_mA0300"]	= "2HDM, $m_{Z'} = 800 GeV, m_{A0} = 300 GeV$";
  fSampleTitleMap["2HDM_mZP1000_mA0300"]= "2HDM, $m_{Z'} = 1000 GeV, m_{A0} = 300 GeV$";
  fSampleTitleMap["2HDM_mZP1200_mA0300"]= "2HDM, $m_{Z'} = 1200 GeV, m_{A0} = 300 GeV$";
  fSampleTitleMap["2HDM_mZP1400_mA0300"]= "2HDM, $m_{Z'} = 1400 GeV, m_{A0} = 300 GeV$";
  fSampleTitleMap["2HDM_mZP1700_mA0300"]= "2HDM, $m_{Z'} = 1700 GeV, m_{A0} = 300 GeV$";
  fSampleTitleMap["2HDM_mZP2000_mA0300"]= "2HDM, $m_{Z'} = 2000 GeV, m_{A0} = 300 GeV$";
  fSampleTitleMap["2HDM_mZP2500_mA0300"]= "2HDM, $m_{Z'} = 2500 GeV, m_{A0} = 300 GeV$";
  fSampleTitleMap["2HDM_mZP600_mA0400"]	= "2HDM, $m_{Z'} = 600 GeV, m_{A0} = 400 GeV$";
  fSampleTitleMap["2HDM_mZP800_mA0400"]	= "2HDM, $m_{Z'} = 800 GeV, m_{A0} = 400 GeV$";

  // find the biggest & smallest efficiencies
  // data
  Double_t maxData = 0;
  Double_t minData = 100; 
  for (UInt_t i=0; i < fNMETPlots; i++){
    if (maxData < fDataMET[i]) maxData = fDataMET[i];
    if (minData > fDataMET[i]) minData = fDataMET[i];
  }
  // sig
  DblVec   maxSig, minSig;
  maxSig.resize(fNSig);
  minSig.resize(fNSig);
  for (UInt_t mc=0; mc < fNSig; mc++){
    maxSig[mc]=0;
    minSig[mc]=100;
    for (UInt_t i=0; i < fNMETPlots; i++){
      if (maxSig[mc] <= fSigMET[mc][i]) maxSig[mc] = fSigMET[mc][i];
      if (minSig[mc] >= fSigMET[mc][i]) minSig[mc] = fSigMET[mc][i]; 
    } 
  } 
  // bkg
  DblVec   maxBkg, minBkg;
  maxBkg.resize(fNBkg);
  minBkg.resize(fNBkg); 
  for (UInt_t mc=0; mc < fNBkg; mc++){
    maxBkg[mc]=0;
    minBkg[mc]=100;
    for (UInt_t i=0; i < fNMETPlots; i++){
      if (maxBkg[mc] <= fBkgMET[mc][i]) maxBkg[mc] = fBkgMET[mc][i];
      if (minBkg[mc] >= fBkgMET[mc][i]) minBkg[mc] = fBkgMET[mc][i]; 
    } 
  } 

  //for (UInt_t mc = 0; mc < fNSig; mc++){
  //   std::cout << fSampleTitleMap[fSigNames[mc]] << " === " << fSigMET[mc][0] << " - " << minSig[mc] << " + " << maxSig[mc] << std::endl; 
  //}
  //for (UInt_t mc = 0; mc < fNBkg; mc++){
  //   std::cout << fSampleTitleMap[fBkgNames[mc]] << " === " << fBkgMET[mc][0] << " - " << minBkg[mc] << " + " << maxBkg[mc] << std::endl; 
  //}

  if (fOutTableTxtFile.is_open()){
    //setup Latex doc
    fOutTableTxtFile << "\\documentclass[a4paper,landscape]{article}" << std::endl;
    fOutTableTxtFile << "\\usepackage[paperheight=15.0in,paperwidth=6.0in,margin=1.0in,headheight=0.0in,footskip=0.5in,includehead,includefoot]{geometry}" << std::endl;
    fOutTableTxtFile << "\\begin{document}" << std::endl;

    // ==========================================================
    // start summary of results table
    fOutTableTxtFile << "\% Summary of MET Systematics" << std::endl; 
    fOutTableTxtFile << "\\begin{table}[bthp]" <<std::endl;
    fOutTableTxtFile << "\\begin{tabular}{|l|c|c|}" <<std::endl;
    fOutTableTxtFile << "\\hline \\hline" <<std::endl;
    fOutTableTxtFile << "\\multicolumn{3}{|c|}{" << Form("$\\sqrt{s}$ = 13 TeV; L = %1.1f $fb^{-1}$",lumi) <<"} \\\\" <<std::endl;
    fOutTableTxtFile << "\\hline \\hline" << std::endl;
    fOutTableTxtFile << "Sample & Efficiency & Efficiency Range \\\\" << std::endl;
    fOutTableTxtFile << "\\hline" <<std::endl;
    for (UInt_t mc = 0; mc < fNSig; mc++){
      fOutTableTxtFile << fSampleTitleMap[fSigNames[mc]] << " & " << Form("%1.4f",fSigMET[mc][0]) << "& -" << Form("%1.4f",fSigMET[mc][0]-minSig[mc]) << ", +" << Form("%1.4f",maxSig[mc]-fSigMET[mc][0]) << " \\\\" << std::endl; 
    }
    fOutTableTxtFile << "\\hline" <<std::endl;
    for (UInt_t mc = 0; mc < fNBkg; mc++){
      fOutTableTxtFile << fSampleTitleMap[fBkgNames[mc]] << " & " << Form("%1.4f",fBkgMET[mc][0]) << "& -" << Form("%1.4f",fBkgMET[mc][0]-minBkg[mc]) << ", +" << Form("%1.4f",maxBkg[mc]-fBkgMET[mc][0]) << " \\\\" << std::endl; 
    }
    fOutTableTxtFile << "Data & " << Form("%1.4f",fDataMET[0]) << "& -" << Form("%1.4f",fDataMET[0]-minData) << ", +" << Form("%1.4f",maxData-fDataMET[0]) << " \\\\" << std::endl; 
    // end table
    fOutTableTxtFile << "\\hline \\hline" <<std::endl;
    fOutTableTxtFile << "\\end{tabular}" <<std::endl;
    fOutTableTxtFile << "\\end{table}" <<std::endl;
    // ==========================================================

    // ==========================================================
    // start list of full systematics table
    fOutTableTxtFile << "\% Full MET Systematics" << std::endl; 

    // signal
    for (UInt_t mc=0; mc < fNSig; mc++){
      fOutTableTxtFile << "\\begin{table}[bthp]" <<std::endl;
      fOutTableTxtFile << "\\begin{tabular}{|l|c|}" <<std::endl;
      fOutTableTxtFile << "\\hline \\hline" <<std::endl;
      fOutTableTxtFile << "\\multicolumn{2}{|c|}{" << fSampleTitleMap[fSigNames[mc]] << "} \\\\" << std::endl;
      fOutTableTxtFile << "\\multicolumn{2}{|c|}{" << Form("$\\sqrt{s}$ = 13 TeV; L = %1.1f $fb^{-1}$",lumi) <<"} \\\\" <<std::endl;
      fOutTableTxtFile << "\\hline" <<std::endl;
      fOutTableTxtFile << "Type1 PF MET Version & Efficiency \\\\" << std::endl;
      for (UInt_t i=0; i < fNMETPlots; i++){ 
        fOutTableTxtFile << fSystMETTitleMap[SystMET[i]] << " & " << Form("%1.4f",fSigMET[mc][i])  <<  " \\\\" << std::endl;
        if (i==0) fOutTableTxtFile << "\\hline" <<std::endl;
      }
      fOutTableTxtFile << "\\hline" << std::endl;
      fOutTableTxtFile << "\\end{tabular}" <<std::endl;
      fOutTableTxtFile << "\\end{table}" <<std::endl;
    }

    // background
    for (UInt_t mc=0; mc < fNBkg; mc++){
      fOutTableTxtFile << "\\begin{table}[bthp]" <<std::endl;
      fOutTableTxtFile << "\\begin{tabular}{|l|c|}" <<std::endl;
      fOutTableTxtFile << "\\hline \\hline" <<std::endl;
      fOutTableTxtFile << "\\multicolumn{2}{|c|}{" << fSampleTitleMap[fBkgNames[mc]] << "} \\\\" << std::endl;
      fOutTableTxtFile << "\\multicolumn{2}{|c|}{" << Form("$\\sqrt{s}$ = 13 TeV; L = %1.1f $fb^{-1}$",lumi) <<"} \\\\" <<std::endl;
      fOutTableTxtFile << "\\hline" <<std::endl;
      fOutTableTxtFile << "Type1 PF MET Version & Efficiency \\\\" << std::endl;
      for (UInt_t i=0; i < fNMETPlots; i++){ 
        fOutTableTxtFile << fSystMETTitleMap[SystMET[i]] << " & " << Form("%1.4f",fBkgMET[mc][i])  <<  " \\\\" << std::endl;
        if (i==0) fOutTableTxtFile << "\\hline" <<std::endl;
      }
      fOutTableTxtFile << "\\hline" << std::endl;
      fOutTableTxtFile << "\\end{tabular}" <<std::endl;
      fOutTableTxtFile << "\\end{table}" <<std::endl;
    }

    // data
    fOutTableTxtFile << "\\begin{table}[bthp]" <<std::endl;
    fOutTableTxtFile << "\\begin{tabular}{|l|c|}" <<std::endl;
    fOutTableTxtFile << "\\hline \\hline" <<std::endl;
    fOutTableTxtFile << "\\multicolumn{2}{|c|}{Data} \\\\" << std::endl;
    fOutTableTxtFile << "\\multicolumn{2}{|c|}{" << Form("$\\sqrt{s}$ = 13 TeV; L = %1.1f $fb^{-1}$",lumi) <<"} \\\\" <<std::endl;
    fOutTableTxtFile << "Type1 PF MET Version & Efficiency \\\\" << std::endl;
    for (UInt_t i=0; i < fNMETPlots; i++){ 
      fOutTableTxtFile << fSystMETTitleMap[SystMET[i]] << " & " << Form("%1.4f",fDataMET[i])  <<  " \\\\" << std::endl;
      if (i==0) fOutTableTxtFile << "\\hline" <<std::endl;
    }
    fOutTableTxtFile << "\\hline" << std::endl;
    fOutTableTxtFile << "\\end{tabular}" <<std::endl;
    fOutTableTxtFile << "\\end{table}" <<std::endl;


    // ==========================================================
    // end Latex doc
    fOutTableTxtFile << "\\end{document}" <<std::endl;
    std::cout << "Writing ResultsTable in " << Form("%s/ResultsTableForLatex.tex",fOutDir.Data()) << std::endl;
  }
  else std::cout << "File didn't Open" << std::endl;

  // close output text files
  fOutTableTxtFile.close();

  Combiner::MakeMETEffPlots();
}// end Combiner::FindMETEfficiencies


void Combiner::MakeMETEffPlots(){

  // Make comparison plots of MET shapes after the various corrections
  // go into the output file
  fOutFile->cd();

  // setup color map for different MET shapes
  fColorMapMETEff["Original"]		= kBlack;
  fColorMapMETEff["JetEnUp"]		= kRed;
  fColorMapMETEff["JetEnDown"]		= kMagenta;
  fColorMapMETEff["JetResUp"]		= kOrange-3;
  fColorMapMETEff["JetResDown"]		= kOrange-3;
  fColorMapMETEff["MuonEnUp"]		= kTeal-1;
  fColorMapMETEff["MuonEnDown"]		= kTeal-1;
  fColorMapMETEff["EleEnUp"]		= kYellow;
  fColorMapMETEff["EleEnDown"]		= kYellow;
  fColorMapMETEff["TauEnUp"]		= kRed;
  fColorMapMETEff["TauEnDown"]		= kRed;
  fColorMapMETEff["PhoEnUp"]		= kGreen;
  fColorMapMETEff["PhoEnDown"]		= kTeal;
  fColorMapMETEff["JetUnclEnUp"]	= kBlue;
  fColorMapMETEff["JetUnclEnDown"]	= kBlue;

  // setup plot legend for signal
  std::vector<TLegend* > fSigMETEffLegend;
  fSigMETEffLegend.resize(fNSig*2);
  for (UInt_t mc = 0; mc < fNSig*2; mc++){
    fSigMETEffLegend[mc] = new TLegend(0.32,0.7,0.9,0.934); // (x1,y1,x2,y2)
    fSigMETEffLegend[mc]->SetNColumns(2);
    fSigMETEffLegend[mc]->SetBorderSize(4);
    fSigMETEffLegend[mc]->SetLineColor(kBlack);
    fSigMETEffLegend[mc]->SetTextSize(0.03);//0.035
    fSigMETEffLegend[mc]->SetLineWidth(2);
  }
  // setup plot legend for background 
  std::vector<TLegend* > fBkgMETEffLegend;
  fBkgMETEffLegend.resize(fNBkg*2);
  for (UInt_t mc = 0; mc < fNBkg*2; mc++){
    fBkgMETEffLegend[mc] = new TLegend(0.32,0.7,0.9,0.934); // (x1,y1,x2,y2)
    fBkgMETEffLegend[mc]->SetNColumns(2);
    fBkgMETEffLegend[mc]->SetBorderSize(4);
    fBkgMETEffLegend[mc]->SetLineColor(kBlack);
    fBkgMETEffLegend[mc]->SetTextSize(0.03);//0.035
    fBkgMETEffLegend[mc]->SetLineWidth(2);
  }

  // copy the plots and normalize them
  fOutSigMETEffTH1DHists.resize(fNMETPlots);
  fOutBkgMETEffTH1DHists.resize(fNMETPlots);
  for (UInt_t th1d = 0; th1d < fNMETPlots; th1d++){
    // sig
    fOutSigMETEffTH1DHists[th1d].resize(fNSig);
    for (UInt_t mc = 0; mc < fNSig; mc++){
      fOutSigMETEffTH1DHists[th1d][mc] = (TH1D*) fInSigTH1DHists[th1d+fIndexMET][mc]->Clone();
      //if (mc==0 || mc==1) fOutSigMETEffTH1DHists[th1d][mc]->GetXaxis()->SetMaximum(500);
      //else fOutSigMETEffTH1DHists[th1d][mc]->GetXaxis()->SetMaximum(900);
      if (fOutSigMETEffTH1DHists[th1d][mc]->Integral() > 0){
        fOutSigMETEffTH1DHists[th1d][mc]->Scale(1.0/fOutSigMETEffTH1DHists[th1d][mc]->Integral());
      } 
      fOutSigMETEffTH1DHists[th1d][mc]->SetFillColor(0);
      fOutSigMETEffTH1DHists[th1d][mc]->SetLineColor(fColorMapMETEff[SystMET[th1d]]);
      fOutSigMETEffTH1DHists[th1d][mc]->SetTitle(fSigNames[mc]);
      fOutSigMETEffTH1DHists[th1d][mc]->GetYaxis()->SetTitle("");
      if (th1d!=0) fSigMETEffLegend[mc]->AddEntry(fOutSigMETEffTH1DHists[th1d][mc],SystMET[th1d],"l");
      if (th1d==1 || th1d==2 || th1d==11 || th1d==12) fSigMETEffLegend[mc+fNSig]->AddEntry(fOutSigMETEffTH1DHists[th1d][mc],SystMET[th1d],"l");
    }
    // bkg
    fOutBkgMETEffTH1DHists[th1d].resize(fNBkg);
    for (UInt_t mc = 0; mc < fNBkg; mc++){
      fOutBkgMETEffTH1DHists[th1d][mc] = (TH1D*) fInBkgTH1DHists[th1d+fIndexMET][mc]->Clone();
      //fOutBkgMETEffTH1DHists[th1d][mc]->GetXaxis()->SetMaximum(350);
      fOutBkgMETEffTH1DHists[th1d][mc]->GetXaxis()->SetRangeUser(10,100);
      if (fOutBkgMETEffTH1DHists[th1d][mc]->Integral() > 0){
        fOutBkgMETEffTH1DHists[th1d][mc]->Scale(1.0/fOutBkgMETEffTH1DHists[th1d][mc]->Integral());
      } 
      fOutBkgMETEffTH1DHists[th1d][mc]->SetFillColor(0);
      fOutBkgMETEffTH1DHists[th1d][mc]->SetLineColor(fColorMapMETEff[SystMET[th1d]]);
      fOutBkgMETEffTH1DHists[th1d][mc]->SetTitle(fBkgNames[mc]);
      fOutBkgMETEffTH1DHists[th1d][mc]->GetYaxis()->SetTitle("");
      if (th1d!=0) fBkgMETEffLegend[mc]->AddEntry(fOutBkgMETEffTH1DHists[th1d][mc],SystMET[th1d],"l");
      if (th1d==1 || th1d==2 || th1d==11 || th1d==12) fBkgMETEffLegend[mc+fNBkg]->AddEntry(fOutBkgMETEffTH1DHists[th1d][mc],SystMET[th1d],"l");
    }
  }
  // add original MET distribution last in legend
  for (UInt_t mc = 0; mc < fNSig*2; mc++){
    if (mc < fNSig) fSigMETEffLegend[mc]->AddEntry(fOutSigMETEffTH1DHists[0][mc],SystMET[0],"l"); 
    else fSigMETEffLegend[mc]->AddEntry(fOutSigMETEffTH1DHists[0][mc-fNSig],SystMET[0],"l"); 
  }  
  for (UInt_t mc = 0; mc < fNBkg*2; mc++){
    if (mc < fNBkg) fBkgMETEffLegend[mc]->AddEntry(fOutBkgMETEffTH1DHists[0][mc],SystMET[0],"l"); 
    else fBkgMETEffLegend[mc]->AddEntry(fOutBkgMETEffTH1DHists[0][mc-fNBkg],SystMET[0],"l"); 
  }  

  // Make signal plots
  TCanvVec fOutSigMETEffCanvas;
  fOutSigMETEffCanvas.resize(fNSig*2); 
  std::vector<TPad* > fOutSigMETEffPad;
  fOutSigMETEffPad.resize(fNSig*2); 

  for (UInt_t mc = 0; mc < fNSig*2; mc++){
    gStyle->SetOptStat(0);
    // setup canvases
    if (mc < fNSig) fOutSigMETEffCanvas[mc] = new TCanvas(fSigNames[mc].Data(),"");
    else fOutSigMETEffCanvas[mc] = new TCanvas(Form("%s_v2",fSigNames[mc-fNSig].Data()),"");
    if (fOutSigMETEffCanvas[mc] == (TCanvas*)"NULL") std::cout << "CANVAS IS NULL" << std::endl;
    fOutSigMETEffCanvas[mc]->cd();
    fOutSigMETEffCanvas[mc]->Draw();
    // setup pads
    fOutSigMETEffPad[mc] = new TPad("","",0.01,0.2,0.99,1.);
    fOutSigMETEffPad[mc]->SetBottomMargin(0);
    fOutSigMETEffPad[mc]->SetRightMargin(0.06); 
    fOutSigMETEffPad[mc]->SetLeftMargin(0.12); 
    fOutSigMETEffPad[mc]->Draw(); 
    fOutSigMETEffPad[mc]->cd(); 

    Double_t maxval = 1; 
    if (mc < fNSig) maxval = fOutSigMETEffTH1DHists[0][mc]->GetMaximum();
    else maxval = fOutSigMETEffTH1DHists[0][mc-fNSig]->GetMaximum();
    // plots with all met shapes
    if (mc < fNSig){
      for (UInt_t th1d = 0; th1d < fNMETPlots; th1d++){
        if (th1d == 0){
          fOutSigMETEffTH1DHists[th1d][mc]->SetMaximum(maxval*10);
          fOutSigMETEffTH1DHists[th1d][mc]->SetMinimum(1E-3);
          fOutSigMETEffTH1DHists[th1d][mc]->Draw("HIST"); 
          if (mc == 0) fOutSigMETEffTH1DHists[th1d][mc]->GetXaxis()->SetRangeUser(0,400); 
          if (mc == 1) fOutSigMETEffTH1DHists[th1d][mc]->GetXaxis()->SetRangeUser(0,500); 
        }
        else fOutSigMETEffTH1DHists[th1d][mc]->Draw("HIST SAME");
      }
      fOutSigMETEffTH1DHists[0][mc]->SetLineWidth(2); 
      fOutSigMETEffTH1DHists[0][mc]->Draw("HIST SAME"); 
    }
    // these have the reduced plots
    else {
      for (UInt_t th1d = 0; th1d < fNMETPlots; th1d++){
        if (th1d == 0){
          fOutSigMETEffTH1DHists[th1d][mc-fNSig]->SetMaximum(maxval);
          fOutSigMETEffTH1DHists[th1d][mc-fNSig]->SetMinimum(1E-3);
          fOutSigMETEffTH1DHists[th1d][mc-fNSig]->Draw("HIST"); 
        }
        else if (th1d == 1 || th1d == 2 || th1d == 11 || th1d == 12 ) fOutSigMETEffTH1DHists[th1d][mc-fNSig]->Draw("HIST SAME");
      }
      fOutSigMETEffTH1DHists[0][mc-fNSig]->SetLineWidth(2); 
      fOutSigMETEffTH1DHists[0][mc-fNSig]->Draw("HIST SAME"); 
    }

    fSigMETEffLegend[mc]->Draw("SAME");
    // make right format for output plots & save them
    fOutSigMETEffPad[mc]->SetLogy(1); 
    CMSLumi(fOutSigMETEffCanvas[mc],11,lumi);
    if (mc < fNSig) fOutSigMETEffCanvas[mc]->SaveAs(Form("%scomb/METEff_%s.%s",fOutDir.Data(),fSigNames[mc].Data(),fType.Data()));
    else fOutSigMETEffCanvas[mc]->SaveAs(Form("%scomb/METEff_%s_Reduced.%s",fOutDir.Data(),fSigNames[mc-fNSig].Data(),fType.Data()));
    fOutFile->cd();
    if (mc < fNSig) fOutSigMETEffCanvas[mc]->Write(Form("METEff_%s",fSigNames[mc].Data()));
    else fOutSigMETEffCanvas[mc]->Write(Form("METEff_%s_Reduced",fSigNames[mc-fNSig].Data()));

    delete fSigMETEffLegend[mc];
    delete fOutSigMETEffPad[mc];
    delete fOutSigMETEffCanvas[mc];
  }
  for (UInt_t mc = 0; mc < fNSig; mc++){
    for (UInt_t th1d = 0; th1d < fNMETPlots; th1d++){ 
      delete fOutSigMETEffTH1DHists[th1d][mc]; 
    }
  }
  
  TCanvVec fOutBkgMETEffCanvas;
  fOutBkgMETEffCanvas.resize(fNBkg*2); 
  std::vector<TPad* > fOutBkgMETEffPad;
  fOutBkgMETEffPad.resize(fNBkg*2); 

  for (UInt_t mc = 0; mc < fNBkg*2; mc++){
    gStyle->SetOptStat(0);
    // setup canvases
    if (mc < fNBkg) fOutBkgMETEffCanvas[mc] = new TCanvas(fBkgNames[mc].Data(),"");
    else fOutBkgMETEffCanvas[mc] = new TCanvas(Form("%s_v2",fBkgNames[mc-fNBkg].Data()),"");
    if (fOutBkgMETEffCanvas[mc] == (TCanvas*)"NULL") std::cout << "CANVAS IS NULL" << std::endl;
    fOutBkgMETEffCanvas[mc]->cd();
    fOutBkgMETEffCanvas[mc]->Draw();
    // setup pads
    fOutBkgMETEffPad[mc] = new TPad("","",0.01,0.2,0.99,1.);
    fOutBkgMETEffPad[mc]->SetBottomMargin(0);
    fOutBkgMETEffPad[mc]->SetRightMargin(0.06); 
    fOutBkgMETEffPad[mc]->SetLeftMargin(0.12); 
    fOutBkgMETEffPad[mc]->Draw(); 
    fOutBkgMETEffPad[mc]->cd(); 

    Double_t maxval = 1;
    if (mc < fNBkg) maxval = fOutBkgMETEffTH1DHists[0][mc]->GetMaximum();
    else maxval = fOutBkgMETEffTH1DHists[0][mc-fNBkg]->GetMaximum();

    // plots with all met shapes
    if (mc < fNBkg){
      for (UInt_t th1d = 0; th1d < fNMETPlots; th1d++){
        if (th1d == 0){
          fOutBkgMETEffTH1DHists[th1d][mc]->SetMaximum(maxval*10);
          fOutBkgMETEffTH1DHists[th1d][mc]->SetMinimum(1E-3);
          fOutBkgMETEffTH1DHists[th1d][mc]->Draw("HIST"); 
        }
        else fOutBkgMETEffTH1DHists[th1d][mc]->Draw("HIST SAME");
        fOutBkgMETEffTH1DHists[th1d][mc]->GetXaxis()->SetRangeUser(0,350); 
      }
      fOutBkgMETEffTH1DHists[0][mc]->SetLineWidth(2); 
      fOutBkgMETEffTH1DHists[0][mc]->Draw("HIST SAME"); 
    }
    // these have the reduced plots
    else {
      for (UInt_t th1d = 0; th1d < fNMETPlots; th1d++){
        if (th1d == 0){ 
          fOutBkgMETEffTH1DHists[th1d][mc-fNBkg]->SetMaximum(maxval);
          fOutBkgMETEffTH1DHists[th1d][mc-fNBkg]->SetMinimum(1E-3);
          fOutBkgMETEffTH1DHists[th1d][mc-fNBkg]->Draw("HIST"); 
        }
        else if (th1d == 1 || th1d == 2 || th1d == 11 || th1d == 12 ) fOutBkgMETEffTH1DHists[th1d][mc-fNBkg]->Draw("HIST SAME");
      }
      fOutBkgMETEffTH1DHists[0][mc-fNBkg]->SetLineWidth(2); 
      fOutBkgMETEffTH1DHists[0][mc-fNBkg]->Draw("HIST SAME"); 
    }

    fBkgMETEffLegend[mc]->Draw("SAME");
    // make right format for output plots & save them
    fOutBkgMETEffPad[mc]->SetLogy(1); 
    CMSLumi(fOutBkgMETEffCanvas[mc],11,lumi);
    if (mc < fNBkg) fOutBkgMETEffCanvas[mc]->SaveAs(Form("%scomb/METEff_%s.%s",fOutDir.Data(),fBkgNames[mc].Data(),fType.Data()));
    else fOutBkgMETEffCanvas[mc]->SaveAs(Form("%scomb/METEff_%s_Reduced.%s",fOutDir.Data(),fBkgNames[mc-fNBkg].Data(),fType.Data()));
    fOutFile->cd();
    if (mc < fNBkg) fOutBkgMETEffCanvas[mc]->Write(Form("METEff_%s",fBkgNames[mc].Data()));
    else fOutBkgMETEffCanvas[mc]->Write(Form("METEff_%s_Reduced",fBkgNames[mc-fNBkg].Data()));

    delete fBkgMETEffLegend[mc];
    delete fOutBkgMETEffPad[mc];
    delete fOutBkgMETEffCanvas[mc];
  }

  for (UInt_t mc = 0; mc < fNBkg; mc++){
    for (UInt_t th1d = 0; th1d < fNMETPlots; th1d++){ 
      if (mc < fNBkg) delete fOutBkgMETEffTH1DHists[th1d][mc]; 
    }
  }

}// end Combiner::MakeMETEffPlots



void Combiner::MakeEffPlots(){

  fOutFile->cd();
  TCanvas *c = new TCanvas();
  c->cd();

  TH1D *eff_mDM = new TH1D("eff_mDM","",2500,500,3000);

  Double_t eff_val = 0.;
  Double_t eff_num = 0.;
  Double_t eff_den = 0.;
  Double_t num_err = 0.;
  Double_t den_err = 0.;
  Double_t eff_err = 0.;
  std::vector<Double_t> mass =  {600,800,1000,1200,1400,1700,2500};//{1,10,100,1000}; 
  Int_t binForMass = 0;


  for (UInt_t mc = 0; mc < fNSig; mc++){
    for (UInt_t th1d = 0; th1d < fNTH1D; th1d++){
      if (fTH1DNames[th1d] == "nvtx_IsolateALLmetCUT") eff_num = fInSigTH1DHists[th1d][mc]->GetEntries();
      if (fTH1DNames[th1d] == "selection_unwgt"){
          //eff_num = fInSigTH1DHists[th1d][mc]->GetBinContent(8); // events passing sel,mgg,met
          eff_den = fInSigTH1DHists[th1d][mc]->GetBinContent(2); // events have no requirements 
      }
    }
    if (eff_den > 0) eff_val = eff_num/eff_den;
    std::cout << "eff_num = " << eff_num << " eff_den = " << eff_den << " eff = " << eff_val << std::endl;
    num_err = TMath::Sqrt(eff_num);
    den_err = TMath::Sqrt(eff_den);
    eff_err = TMath::Sqrt(eff_val*(1.0-eff_val)/eff_den); 
    //std::cout << fSigNames[mc]<< " mass = " << mass[mc] <<" -> eff_val = " << eff_val << "\\pm " << eff_err << std::endl;
    binForMass = eff_mDM->FindBin(mass[mc]);
    eff_mDM->SetBinContent(binForMass,eff_val);
    eff_mDM->SetBinError(binForMass,eff_err);
  }

  eff_mDM->SetMaximum(1.0);
  eff_mDM->SetMinimum(0.0);
  eff_mDM->GetXaxis()->SetTitle("m_{Z'} [GeV]");
  eff_mDM->GetYaxis()->SetTitle("Efficiency");
  eff_mDM->Draw("PE1");
  eff_mDM->Write();
  
  CMSLumi(c,11,lumi);
  c->SetLogx();
  c->SaveAs(Form("%scomb/eff_mDM.%s",fOutDir.Data(),fType.Data()));
  delete c; 

  //// make significance plots for ptgg, absdphi, deta
  //UInt_t num_sel = 20;
  //TH1D *signif_ptgg = new TH1D("signif_ptgg","",num_sel,-0.5,19.5);
  //TH1D *signif_dphi = new TH1D("signif_dphi","",num_sel,-0.5,19.5);
  //TH1D *signif_deta = new TH1D("signif_deta","",num_sel,-0.5,19.5);

  //DblVecVec signal_ptgg, signal_dphi, signal_deta;
  //DblVec    totbkg_ptgg, totbkg_dphi, totbkg_deta;
  //signal_ptgg.resize(fNSig);
  //signal_dphi.resize(fNSig);
  //signal_deta.resize(fNSig);
  //
  //for (UInt_t mc = 0; mc < fNSig; mc++){
  //  signal_ptgg[mc].resize(num_sel); 
  //  signal_dphi[mc].resize(num_sel); 
  //  signal_deta[mc].resize(num_sel); 
  //  for (UInt_t cut = 0; cut < num_sel; cut++){
  //     //fInSigTH1DHists[fIndexPtgg][mc]->Integral();  
  //     //fInSigTH1DHists[fIndexDphi][mc]->Integral();  
  //     //fInSigTH1DHists[fIndexDeta][mc]->Integral();  
  //  }    
  //} 

  





}// end Combiner::MakeEffPlots


void Combiner::MakeOutputCanvas(){
  for (UInt_t th1d = 0; th1d < fNTH1D; th1d++){

/*
     for (UInt_t mc = 0; mc < fNBkg; mc++){
      std::cout << th1d << " before " << fInBkgTH1DHists[th1d][mc]->GetMaximum() << std::endl;
      fInBkgTH1DHists[th1d][mc]->Scale(lumi);
    }
    if (fNData > 0) fOutDataTH1DHists[th1d]->Scale(lumi);
*/
    Bool_t isLogY = false;
    if (doStack){// do Stack plots
      Combiner::DrawCanvasStack(th1d,isLogY);
      isLogY = true;
      Combiner::DrawCanvasStack(th1d,isLogY);
      if (fNData > 0) fOutTH1DRatioPads[th1d]->Clear(); //delete the ratio plot for overlay plots
    }
    else{// do overlay next 
      Combiner::DrawCanvasOverlay(th1d,isLogY);
      isLogY = true;
      Combiner::DrawCanvasOverlay(th1d,isLogY);
    }
  }
}// end Combiner::MakeOutputCanvas


void Combiner::DrawCanvasOverlay(const UInt_t th1d, const Bool_t isLogY){
  gStyle->SetOptStat(0);
  fOutTH1DCanvases[th1d]->cd();
  fOutTH1DStackPads[th1d]->Draw();
  fOutTH1DStackPads[th1d]->cd();
   
  //if (fOutDataTH1DHists[th1d]->Integral() > 0) fOutDataTH1DHists[th1d]->Scale(1.0/fOutDataTH1DHists[th1d]->Integral());

  for (UInt_t mc = 0; mc < fNSig; mc++){
    if (fInSigTH1DHists[th1d][mc]->Integral() > 0){
      fInSigTH1DHists[th1d][mc]->Scale(1.0/fInSigTH1DHists[th1d][mc]->Integral());
    }
  }

  if (doMergeBkgs){
    fOutHiggsBkgTH1DHists[th1d]->Scale(1.0/fOutHiggsBkgTH1DHists[th1d]->Integral());
    fOutHiggsBkgTH1DHists[th1d]->SetFillColor(0);
    fOutHiggsBkgTH1DHists[th1d]->SetLineColor(fColorMap["SMHiggs"]);

    fOutEWK1phoBkgTH1DHists[th1d]->Scale(1.0/fOutEWK1phoBkgTH1DHists[th1d]->Integral());
    fOutEWK1phoBkgTH1DHists[th1d]->SetFillColor(0);
    fOutEWK1phoBkgTH1DHists[th1d]->SetLineColor(fColorMap["EWK1pho"]);

    fOutEWK2phoBkgTH1DHists[th1d]->Scale(1.0/fOutEWK2phoBkgTH1DHists[th1d]->Integral());
    fOutEWK2phoBkgTH1DHists[th1d]->SetFillColor(0);
    fOutEWK2phoBkgTH1DHists[th1d]->SetLineColor(fColorMap["EWK2pho"]);

    fOutJetsphoBkgTH1DHists[th1d]->Scale(1.0/fOutJetsphoBkgTH1DHists[th1d]->Integral());
    fOutJetsphoBkgTH1DHists[th1d]->SetFillColor(0);
    fOutJetsphoBkgTH1DHists[th1d]->SetLineColor(fColorMap["Jetspho"]);

    fOutDYJetsBkgTH1DHists[th1d]->Scale(1.0/fOutDYJetsBkgTH1DHists[th1d]->Integral());
    fOutDYJetsBkgTH1DHists[th1d]->SetFillColor(0);
    fOutDYJetsBkgTH1DHists[th1d]->SetLineColor(fColorMap["DYJetsToLL"]);
  }

  for (UInt_t mc = 0; mc < fNBkg; mc++){
    if (fInBkgTH1DHists[th1d][mc]->Integral() > 0 ){
      fInBkgTH1DHists[th1d][mc]->Scale(1.0/fInBkgTH1DHists[th1d][mc]->Integral());
    }
    fInBkgTH1DHists[th1d][mc]->SetFillColor(0);
    fInBkgTH1DHists[th1d][mc]->SetLineColor(fColorMap[fBkgNames[mc]]);
  }

  Double_t maxOverlay = -100;
  maxOverlay = Combiner::GetMaximum(th1d, false);
  //Double_t minOverlay = 1e9;
  //minOverlay = Combiner::GetMinimum(th1d, false);  

  // start by drawing the sig first
  if (isLogY) fInSigTH1DHists[th1d][0]->SetMaximum(maxOverlay*1E3);
  else fInSigTH1DHists[th1d][0]->SetMaximum(maxOverlay*1.4);

  //fInSigTH1DHists[th1d][0]->SetMinimum(0.0);
  //if (fNData > 0) fInSigTH1DHists[th1d][0]->SetMinimum(minOverlay*0.9);
  //if (th1d==fIndexMgg){ 
  //  //fInSigTH1DHists[th1d][0]->SetMinimum(0.001); 
  //  fInSigTH1DHists[th1d][0]->SetMaximum(10);
  //}
  int i_gg, i_gj, i_hgg, i_vbf, i_vh;
  for (UInt_t mc = 0; mc < fNBkg; mc++){
    if (fBkgNames[mc] == "DiPhoton")	i_gg  = mc;
    if (fBkgNames[mc] == "GJets")	i_gj  = mc;
    if (fBkgNames[mc] == "GluGluHToGG") i_hgg = mc;
    if (fBkgNames[mc] == "VBFHToGG")    i_vbf = mc;
    if (fBkgNames[mc] == "VHToGG")	i_vh  = mc;
  }

  if (fTH1DNames[th1d]=="metCorr_forShape"){
    fInSigTH1DHists[th1d][0]->SetTitle("");
    fInSigTH1DHists[th1d][0]->GetYaxis()->SetTitle("");
    fInSigTH1DHists[th1d][0]->SetLineColor(kWhite);
    fInSigTH1DHists[th1d][0]->Draw("hist");

    TLegend* ftempLegends = new TLegend(0.55,0.7,0.9,0.934); // (x1,y1,x2,y2)

    for (UInt_t mc = 0; mc < fNBkg; mc++){
      if (mc == i_hgg){
        fInBkgTH1DHists[th1d][mc]->SetLineColor(kRed);
        fInBkgTH1DHists[th1d][mc]->SetFillColor(kRed);
        fInBkgTH1DHists[th1d][mc]->SetLineWidth(2);
        fInBkgTH1DHists[th1d][mc]->SetFillStyle(3004);
        fInBkgTH1DHists[th1d][mc]->SetMarkerSize(0.);
        fInBkgTH1DHists[th1d][mc]->Draw("HISTE SAME");
        ftempLegends->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"LF");
      }	
      if (mc == i_gj){
        fInBkgTH1DHists[th1d][mc]->SetLineColor(kGreen+2);
        fInBkgTH1DHists[th1d][mc]->SetLineWidth(2);
        fInBkgTH1DHists[th1d][mc]->SetMarkerSize(0.);
        fInBkgTH1DHists[th1d][mc]->Draw("HISTE SAME");
        ftempLegends->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"LF");
      }	
      if (mc == i_gg){
        fInBkgTH1DHists[th1d][mc]->SetLineColor(kGreen+4);
        fInBkgTH1DHists[th1d][mc]->SetLineWidth(2);
        fInBkgTH1DHists[th1d][mc]->SetMarkerSize(0.);
        fInBkgTH1DHists[th1d][mc]->Draw("HISTE SAME");
        ftempLegends->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"LF");
      }	
      //if (mc == i_gg || mc == i_gj || mc == i_hgg ){
      //  fInBkgTH1DHists[th1d][mc]->SetLineWidth(2);
      //  fInBkgTH1DHists[th1d][mc]->Draw("HIST SAME");
      //  ftempLegends->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"l");
      //}
    }

    ftempLegends->SetBorderSize(4);
    ftempLegends->SetLineColor(kBlack);
    ftempLegends->SetTextSize(0.03);//0.035
    ftempLegends->SetLineWidth(2);
    ftempLegends->Draw("SAME");
  }
  else if (fTH1DNames[th1d]=="mgg_forShape"){
    fInSigTH1DHists[th1d][0]->SetTitle("");
    fInSigTH1DHists[th1d][0]->Draw("hist");
    fInSigTH1DHists[th1d][0]->GetYaxis()->SetTitle("");

    TLegend* ftempLegends = new TLegend(0.55,0.7,0.85,0.934); // (x1,y1,x2,y2)

    for (UInt_t mc = 0; mc < fNSig; mc++){
      if (mc==0 || mc==2 || mc==fNSig-1){
        fInSigTH1DHists[th1d][mc]->SetLineWidth(2);
        fInSigTH1DHists[th1d][mc]->Draw("HIST SAME");
        ftempLegends->AddEntry(fInSigTH1DHists[th1d][mc],fSampleTitleMap[fSigNames[mc]],"l");
      }
    }

    ftempLegends->SetBorderSize(4);
    ftempLegends->SetLineColor(kBlack);
    ftempLegends->SetTextSize(0.03);//0.035
    ftempLegends->SetLineWidth(2);
    ftempLegends->Draw("SAME");
  }
  else{   
    fInSigTH1DHists[th1d][0]->SetTitle("");
    fInSigTH1DHists[th1d][0]->GetYaxis()->SetTitle("");
    fInSigTH1DHists[th1d][0]->Draw("hist");

    if (doMergeBkgs){
      fInBkgTH1DHists[th1d][i_vh]->Draw("HIST SAME");
      fOutHiggsBkgTH1DHists[th1d]->Draw("HIST SAME");
      fOutEWK2phoBkgTH1DHists[th1d]->Draw("HIST SAME");
      fOutDYJetsBkgTH1DHists[th1d]->Draw("HIST SAME");
      fOutEWK1phoBkgTH1DHists[th1d]->Draw("HIST SAME");
      fOutJetsphoBkgTH1DHists[th1d]->Draw("HIST SAME");
      for (UInt_t mc = 0; mc < fNBkg; mc++){
        if (fBkgNames[mc]=="DiPhoton"){
          fInBkgTH1DHists[th1d][mc]->Draw("HIST SAME");
        }
      } 
    } 
    else{//not merging bkgs
      for (UInt_t mc = 0; mc < fNBkg; mc++){
        fInBkgTH1DHists[th1d][mc]->Draw("HIST SAME");
      } 
    }
    for (UInt_t mc = 0; mc < fNSig; mc++){
      fInSigTH1DHists[th1d][mc]->Draw("HIST SAME");
    }
    //if (fNData > 0) fOutDataTH1DHists[th1d]->Draw("PE SAME");
    fTH1DLegends[th1d]->Draw("SAME"); 
    fTH1DLegendsSig[th1d]->Draw("SAME"); 
  }


  TString suffix = "";
  if (isLogY) suffix="_log";

  fOutTH1DStackPads[th1d]->SetLogy(isLogY);
  fOutTH1DCanvases[th1d]->cd();

  if (fTH1DNames[th1d]!="mgg_forShape") CMSLumi(fOutTH1DCanvases[th1d],11,lumi);

  // don't save plots that are used for efficiencies 
  if (fTH1DNames[th1d]!="vtx_eff_ptzp_n" && fTH1DNames[th1d]!="vtx_eff_ptzp_d" && fTH1DNames[th1d]!="vtx_eff_nvtx_n" && fTH1DNames[th1d]!="vtx_eff_nvtx_d" && fTH1DNames[th1d]!="vtx_eff_met_n" && fTH1DNames[th1d]!="vtx_eff_met_d" && fTH1DNames[th1d]!="vtx_eff_njet_n" && fTH1DNames[th1d]!="vtx_eff_njet_d" && fTH1DNames[th1d]!="JetEnUp" && fTH1DNames[th1d]!="JetEnDown" && fTH1DNames[th1d]!="JetResUp" && fTH1DNames[th1d]!="JetResDown" && fTH1DNames[th1d]!="MuonEnUp" && fTH1DNames[th1d]!="MuonEnDown" && fTH1DNames[th1d]!="EleEnUp" && fTH1DNames[th1d]!="EleEnDown" && fTH1DNames[th1d]!="TauEnUp" && fTH1DNames[th1d]!="TauEnDown" && fTH1DNames[th1d]!="PhoEnUp" && fTH1DNames[th1d]!="PhoEnDown" && fTH1DNames[th1d]!="UnclEnUp" && fTH1DNames[th1d]!="UnclEnDown" && fTH1DNames[th1d]!="pt1_afterIDloose"&& fTH1DNames[th1d]!="pt1_beforeIDloose" && fTH1DNames[th1d]!="eta1_afterIDloose" && fTH1DNames[th1d]!="eta1_beforeIDloose"){
    // for all other plots
    fOutTH1DCanvases[th1d]->SaveAs(Form("%scomb/%s_comb%s%s.%s",fOutDir.Data(),fTH1DNames[th1d].Data(),addText.Data(),suffix.Data(),fType.Data()));  
    fOutFile->cd();
    fOutTH1DCanvases[th1d]->Write(Form("%s%s_comb%s",fTH1DNames[th1d].Data(),suffix.Data(),addText.Data()));
  }

}// end Combiner::DrawCanvasOverlay

void Combiner::DrawCanvasStack(const UInt_t th1d, const Bool_t isLogY){
  gStyle->SetOptStat(0);
  gStyle->SetFrameLineWidth(3);
  fOutTH1DCanvases[th1d]->cd();
  fOutTH1DStackPads[th1d]->Draw();
  fOutTH1DStackPads[th1d]->cd();


 /* for (UInt_t mc = 0; mc < fNSig; mc++){
    fInSigTH1DHists[th1d][mc]->Scale(lumi);
  }
  for (UInt_t data = 0; data < fNData; data++){
    //std::cout << fDataNames[data].Data() << std::endl;
//    if (Form("%s",fDataNames[data].Data()) == "FakeData"){
      if ( fDataNames[data].Data() == "FakeData" ) std::cout << "worked!" << std::endl;
      else std::cout << "hahahaha"<< std::endl;
      fOutDataTH1DHists[th1d]->Scale(lumi);
   // }
  }*/

  Double_t maxval = -100;
  maxval = Combiner::GetMaximum(th1d, true);
  //Double_t minval = 1E20;
  //minval = Combiner::GetMinimum(th1d, true);

  int i_hgg, i_vbf, i_vh, i_tth;
  for (UInt_t mc = 0; mc < fNBkg; mc++){
    if (fBkgNames[mc] == "GluGluHToGG") i_hgg = mc;
    if (fBkgNames[mc] == "VBFHToGG")    i_vbf = mc;
    if (fBkgNames[mc] == "ttHJetToGG")	i_tth = mc;
    if (fBkgNames[mc] == "VHToGG")	i_vh  = mc;
  }
  Double_t dataInt = 0;

  if (fTH1DNames[th1d]=="mgg_forShape" || fTH1DNames[th1d]=="mgg_metCUT_forShape"){

    THStack* mgg_Shape = new THStack();
    TLegend* ftempLegend = new TLegend(0.6,0.8,0.9,0.934); // (x1,y1,x2,y2)

    fInSigTH1DHists[th1d][0]->SetTitle("");
    fInSigTH1DHists[th1d][0]->SetLineColor(kWhite);
    if (isLogY){
      fInSigTH1DHists[th1d][0]->SetMaximum(maxval);
      fInSigTH1DHists[th1d][0]->SetMinimum(1E-3);
    }
    else {
      fInSigTH1DHists[th1d][0]->SetMaximum(2);
      fInSigTH1DHists[th1d][0]->SetMinimum(0);
    }
    fInSigTH1DHists[th1d][0]->Draw("HIST");
    for (UInt_t mc = 0; mc < fNBkg; mc++){
      if (mc == i_vbf || mc == i_hgg || mc == i_tth || mc == i_vh){
	 mgg_Shape->Add(fInBkgTH1DHists[th1d][mc]);
         ftempLegend->AddEntry(fInBkgTH1DHists[th1d][mc],fSampleTitleMap[fBkgNames[mc]],"f");
      }
    } 
    mgg_Shape->Draw("HIST SAME");

    ftempLegend->SetBorderSize(4);
    ftempLegend->SetLineColor(kBlack);
    ftempLegend->SetTextSize(0.03);//0.035
    ftempLegend->SetLineWidth(2);
    ftempLegend->Draw("SAME");

    dataInt = fOutDataTH1DHists[th1d]->Integral();
  }
  else {
    // start by drawing the sig first
    fInSigTH1DHists[th1d][0]->SetTitle("");
    fInSigTH1DHists[th1d][0]->GetXaxis()->SetTitleOffset(999);
    fInSigTH1DHists[th1d][0]->GetXaxis()->SetLabelSize(0);
    if (isLogY){
      if (fTH1DNames[th1d]=="mgg_IsolateALL") fInSigTH1DHists[th1d][0]->SetMaximum(maxval*9E2);
      else if (fTH1DNames[th1d]=="t1pfmetCorr_selmgg") fInSigTH1DHists[th1d][0]->SetMaximum(maxval*1E1);
      else if (fTH1DNames[th1d]=="nvtx") fInSigTH1DHists[th1d][0]->SetMaximum(maxval*1E4);
      else if (fTH1DNames[th1d]=="jetInfo_eta1") fInSigTH1DHists[th1d][0]->SetMaximum(maxval*1E4);
      else fInSigTH1DHists[th1d][0]->SetMaximum(maxval*1E2);
      if (fTH1DNames[th1d]=="t1pfmetCorr_selmgg") fInSigTH1DHists[th1d][0]->SetMinimum(0.5E-2);
      else fInSigTH1DHists[th1d][0]->SetMinimum(0.5E-1);
    }
    else {
      if (fTH1DNames[th1d]=="newstudies_mgg_dphijMET") fInSigTH1DHists[th1d][0]->SetMaximum(400);
      else if (fTH1DNames[th1d]=="newstudies_mgg_ptgg") fInSigTH1DHists[th1d][0]->SetMaximum(8000);
      else fInSigTH1DHists[th1d][0]->SetMaximum(maxval*1.5);
      fInSigTH1DHists[th1d][0]->SetMinimum(0);
    }
    fInSigTH1DHists[th1d][0]->GetYaxis()->SetTitleSize(0.07);
    fInSigTH1DHists[th1d][0]->GetYaxis()->SetTitleOffset(0.8);
    if (fTH1DNames[th1d]=="mgg_IsolateALL")     fInSigTH1DHists[th1d][0]->GetYaxis()->SetTitle("Events / 2 GeV");
    if (fTH1DNames[th1d]=="t1pfmetCorr_selmgg") fInSigTH1DHists[th1d][0]->GetYaxis()->SetTitle("Events / 5 GeV");
    fInSigTH1DHists[th1d][0]->GetYaxis()->SetTitleFont(42);
    fInSigTH1DHists[th1d][0]->GetYaxis()->SetLabelFont(42);
    fInSigTH1DHists[th1d][0]->GetYaxis()->SetLabelSize(0.07);

    fInSigTH1DHists[th1d][0]->Draw("HIST");

    fOutBkgTH1DStacks[th1d]->Draw("HIST SAME");
    //fOutBkgTH1DHists[th1d]->Draw("E2 SAME");//E2 draws error as rectangle
    //fOutBkgTH1DStacksForUncer[th1d]->Draw("nostack E2 SAME");

    // check that the blinding does not completely cover the plot
    // this could be problematic in the future
    dataInt = fOutDataTH1DHists[th1d]->Integral();
    if (fNData > 0 && dataInt > 0){
      fOutDataTH1DHists[th1d]->Draw("PE SAME");
    }

    for (UInt_t mc = 0; mc < fNSig; mc++){
      fInSigTH1DHists[th1d][mc]->SetLineStyle(8); 
      fInSigTH1DHists[th1d][mc]->Draw("HIST SAME");
      if ( mc == fNSig-1 ){ // on last draw redraw axis because they are overwritten by stack
        fInSigTH1DHists[th1d][mc]->Draw("AXIS SAME");
      }
    }

    //fInSigTH1DHists[th1d][0]->Draw("AXIS SAME");

    fOutBkgTH1DHists[th1d]->Draw("E2 SAME");//E2 draws error as rectangle
    fTH1DLegends[th1d]->Draw("SAME"); 
    fTH1DLegendsSig[th1d]->Draw("SAME"); 
  }

  TString suffix = "";
  if (isLogY) suffix="_log";

  fOutTH1DStackPads[th1d]->SetLogy(isLogY);

  fOutTH1DCanvases[th1d]->cd();

  // change style to match Hbb
  TString latexCMS = "CMS";
  TString latexHgg = "Z' #rightarrow DM + h(#gamma#gamma)";
  TString latexlumi = Form("%1.1f fb^{-1}",lumi);
  TString latexener = " (13 TeV)";
  TString latexname = latexlumi+latexener; 

  TLatex *l1 = new TLatex(0.17,0.92,latexCMS);
  l1->SetTextSize(0.036);
  l1->SetTextAlign(12);
  l1->SetNDC(kTRUE);
  l1->SetTextFont(62);
 
  TLatex *l2 = new TLatex(0.73,0.98,latexname); // to have right aligned
  //TLatex *l2 = new TLatex(0.65,0.98,latexname);   // to match Hbb
  l2->SetTextSize(0.034);
  l2->SetTextAlign(12);
  l2->SetNDC(kTRUE);
  l2->SetTextFont(42);

  TLatex *l3 = new TLatex(0.17,0.88,latexHgg);
  l3->SetTextSize(0.036);
  l3->SetTextAlign(12);
  l3->SetNDC(kTRUE);
  l3->SetTextFont(42);

  l1->Draw("same");
  l2->Draw("same");
  l3->Draw("same");


  if (fNData > 0 && dataInt > 0){ // make & draw ratio plots
    Combiner::MakeRatioPlots();
    //Combiner::MakeRatioLine(th1d);

    gStyle->SetFrameLineWidth(3);
    fOutTH1DRatioPads[th1d]->Draw();
    fOutTH1DRatioPads[th1d]->cd();
    fOutTH1DRatioPads[th1d]->SetFillColor(0);
    fOutTH1DRatioPads[th1d]->SetTicky(1);
    fOutTH1DRatioPads[th1d]->SetFrameFillStyle(0);
    fOutTH1DRatioPads[th1d]->SetFrameBorderMode(0);

    fOutRatioTH1DHists[th1d]->Draw("EP"); // draw first so line can appear
    fOutTH1DRatioLines[th1d]->Draw("SAME");

    fOutRatioTH1DHists[th1d]->GetYaxis()->SetNdivisions(505);

    fOutRatioTH1DHists[th1d]->GetXaxis()->SetLabelSize(0.157);
    fOutRatioTH1DHists[th1d]->GetXaxis()->SetTitleSize(0.16);
    fOutRatioTH1DHists[th1d]->GetXaxis()->SetTitleOffset(1.02);;
    fOutRatioTH1DHists[th1d]->GetXaxis()->SetTitleFont(42);
    fOutRatioTH1DHists[th1d]->GetXaxis()->SetTickLength(0.07);
    fOutRatioTH1DHists[th1d]->GetXaxis()->SetTickSize(0.11);
    fOutRatioTH1DHists[th1d]->GetXaxis()->SetLabelFont(42);
    fOutRatioTH1DHists[th1d]->GetXaxis()->SetMoreLogLabels(); 
    fOutRatioTH1DHists[th1d]->GetXaxis()->SetNoExponent();
    fOutRatioTH1DHists[th1d]->GetXaxis()->SetNdivisions(508);

    fOutRatioTH1DHists[th1d]->GetYaxis()->SetLabelSize(0.15);
    fOutRatioTH1DHists[th1d]->GetYaxis()->SetTitleSize(0.14);
    fOutRatioTH1DHists[th1d]->GetYaxis()->SetTitleOffset(0.38);
    fOutRatioTH1DHists[th1d]->GetYaxis()->SetTitleFont(42);
    fOutRatioTH1DHists[th1d]->GetYaxis()->CenterTitle();
    fOutRatioTH1DHists[th1d]->GetYaxis()->SetLabelFont(42);

    fOutRatioTH1DHistsCopy[th1d]->Draw("E2 SAME");
    fOutRatioTH1DHists[th1d]->Draw("EP SAME");
  } 


  TLegend *ratioleg = new TLegend(0.65,0.85,0.94,0.94);//x1,y1,x2,y2
  ratioleg->SetFillColor(0);
  ratioleg->SetLineColor(0);
  ratioleg->SetShadowColor(0);
  ratioleg->SetTextFont(42);
  ratioleg->SetTextSize(0.09);
  ratioleg->SetBorderSize(1);
  //ratioleg->SetNColumns(2);
  ratioleg->AddEntry(fOutRatioTH1DHistsCopy[th1d], "MC uncert. (stat)","f");
  if (fTH1DNames[th1d]!="mgg_forShape") ratioleg->Draw("same");


  if (fTH1DNames[th1d]=="mgg_IsolateALL" || fTH1DNames[th1d]=="mgg_IsolateALL_pt1" || fTH1DNames[th1d]=="mgg_IsolateALL_pt2" || fTH1DNames[th1d]=="mgg_IsolateALL_ptgg" || fTH1DNames[th1d]=="mgg_IsolateALL_woPtgg" || fTH1DNames[th1d]=="mgg_IsolateALL_wPtgg"){
    //std::cout << " ----------------------------------------------------- " << std::endl;
    //std::cout << "Histogram Name = " << fTH1DNames[th1d] << std::endl;
    //std::cout << "Data Total = " << fInDataTH1DHists[th1d][0]->Integral() << std::endl; 
    Double_t BkgIntegral = 0.;
    for (UInt_t mc = 0; mc < fNBkg; mc++){
      //std::cout << fBkgNames[mc] << " = " << fInBkgTH1DHists[th1d][mc]->Integral() << std::endl;
      BkgIntegral += fInBkgTH1DHists[th1d][mc]->Integral();
    }
    ////std::cout << "QCD Reweight = " << GJetsClone[th1d]->Integral() << std::endl;
    //std::cout << "Bkg Total = " << BkgIntegral << std::endl;
    //std::cout << "Ratio Data/MC = " << fInDataTH1DHists[th1d][0]->Integral()/BkgIntegral << std::endl;
    //std::cout << " ----------------------------------------------------- " << std::endl;
  }

  fOutTH1DCanvases[th1d]->Update();

  //CMSLumi(fOutTH1DCanvases[th1d],11,lumi);

  if (fTH1DNames[th1d]!="vtx_eff_ptzp_n" && fTH1DNames[th1d]!="vtx_eff_ptzp_d" && fTH1DNames[th1d]!="vtx_eff_nvtx_n" && fTH1DNames[th1d]!="vtx_eff_nvtx_d" && fTH1DNames[th1d]!="vtx_eff_met_n" && fTH1DNames[th1d]!="vtx_eff_met_d" && fTH1DNames[th1d]!="vtx_eff_njet_n" && fTH1DNames[th1d]!="vtx_eff_njet_d" && fTH1DNames[th1d]!="JetEnUp" && fTH1DNames[th1d]!="JetEnDown" && fTH1DNames[th1d]!="JetResUp" && fTH1DNames[th1d]!="JetResDown" && fTH1DNames[th1d]!="MuonEnUp" && fTH1DNames[th1d]!="MuonEnDown" && fTH1DNames[th1d]!="EleEnUp" && fTH1DNames[th1d]!="EleEnDown" && fTH1DNames[th1d]!="TauEnUp" && fTH1DNames[th1d]!="TauEnDown" && fTH1DNames[th1d]!="PhoEnUp" && fTH1DNames[th1d]!="PhoEnDown" && fTH1DNames[th1d]!="UnclEnUp" && fTH1DNames[th1d]!="UnclEnDown" && fTH1DNames[th1d]!="pt1_afterIDloose"&& fTH1DNames[th1d]!="pt1_beforeIDloose" && fTH1DNames[th1d]!="eta1_afterIDloose" && fTH1DNames[th1d]!="eta1_beforeIDloose"){
    fOutTH1DCanvases[th1d]->SaveAs(Form("%scomb/%s_stack%s%s.%s",fOutDir.Data(),fTH1DNames[th1d].Data(),addText.Data(),suffix.Data(),fType.Data()));  
    fOutFile->cd();
    fOutTH1DCanvases[th1d]->Write(Form("%s%s_stack%s",fTH1DNames[th1d].Data(),suffix.Data(),addText.Data()));
  }

  /*
  for (UInt_t mc = 0; mc < fNSig; mc++){
    fInSigTH1DHists[th1d][mc]->Scale(1/lumi);
  }
  for (UInt_t data = 0; data < fNData; data++){
    //std::cout << fDataNames[data].Data() << std::endl;
//    if (Form("%s",fDataNames[data].Data()) == "FakeData"){
      if ( fDataNames[data].Data() == "FakeData" ) std::cout << "worked!" << std::endl;
      else std::cout << "hahahaha"<< std::endl;
      fOutDataTH1DHists[th1d]->Scale(1/lumi);
   // }
  }
  */

}// end Combiner::DrawCanvasStack

void Combiner::MakeRatioLine(const UInt_t th1d){
  // have line held at ratio of 1.0 over whole x range
  fOutTH1DRatioLines[th1d]->SetX1(fOutRatioTH1DHists[th1d]->GetXaxis()->GetXmin());
  fOutTH1DRatioLines[th1d]->SetY1(1.0);
  fOutTH1DRatioLines[th1d]->SetX2(fOutRatioTH1DHists[th1d]->GetXaxis()->GetXmax());
  fOutTH1DRatioLines[th1d]->SetY2(1.0);

  // customize appearance
  fOutTH1DRatioLines[th1d]->SetLineColor(kRed);
  fOutTH1DRatioLines[th1d]->SetLineWidth(2);

}// end Combiner::MakeRatioLine

void Combiner::MakeRatioPlots(){

Double_t bkg = 0.;
Double_t dat = 0.;
Double_t bkgErr = 0.;
Double_t datErr = 0.;
Double_t ratErr = 0.;

  for (UInt_t th1d = 0; th1d < fNTH1D; th1d++){ // double hists
    if (fNData > 0) fOutRatioTH1DHists[th1d] = (TH1D*)fOutDataTH1DHists[th1d]->Clone();
    else fOutRatioTH1DHists[th1d] = (TH1D*)fOutBkgTH1DHists[th1d]->Clone();

    fOutRatioTH1DHists[th1d]->SetTitle("");
    fOutRatioTH1DHists[th1d]->Divide(fOutBkgTH1DHists[th1d]);  

    fOutRatioTH1DHistsCopy[th1d] = (TH1D*)fOutRatioTH1DHists[th1d]->Clone();

    fOutRatioTH1DHists[th1d]->SetLineColor(kBlack);
    fOutRatioTH1DHists[th1d]->SetMinimum(-0.1);  // Define Y ..
    fOutRatioTH1DHists[th1d]->SetMaximum(2.2);   // .. range
    fOutRatioTH1DHists[th1d]->SetStats(0);       // No statistics on lower plot
    fOutRatioTH1DHists[th1d]->GetYaxis()->SetTitle("Data/MC");
    for (UInt_t bin=0; bin<=fOutRatioTH1DHists[th1d]->GetNbinsX();bin++){
      if (fOutBkgTH1DHists[th1d]->GetBinContent(bin)){
        bkg = fOutBkgTH1DHists[th1d]->GetBinContent(bin);
        dat = fOutDataTH1DHists[th1d]->GetBinContent(bin);
        bkgErr = fOutBkgTH1DHists[th1d]->GetBinError(bin);
        datErr = fOutDataTH1DHists[th1d]->GetBinError(bin);
        ratErr = TMath::Sqrt(TMath::Power(datErr/bkg,2)+TMath::Power(dat*bkgErr/(bkg*bkg),2)); 
      }
      else ratErr = 0.;
      //std::cout << "RatioErr1 = " << ratErr << std::endl;
      fOutRatioTH1DHists[th1d]->SetBinError(bin,ratErr);
    }
    for (UInt_t bin=0; bin<=fOutRatioTH1DHistsCopy[th1d]->GetNbinsX();bin++){
      if (fOutBkgTH1DHists[th1d]->GetBinContent(bin)){
        bkg = fOutBkgTH1DHists[th1d]->GetBinContent(bin);
        dat = fOutDataTH1DHists[th1d]->GetBinContent(bin);
        bkgErr = fOutBkgTH1DHists[th1d]->GetBinError(bin);
        datErr = fOutDataTH1DHists[th1d]->GetBinError(bin);
        ratErr = bkgErr/bkg; 
      }
      else ratErr = 0.;
      //std::cout << "RatioErr2 = " << ratErr << std::endl;
      fOutRatioTH1DHistsCopy[th1d]->SetFillColor(kGray+3);
      fOutRatioTH1DHistsCopy[th1d]->SetLineColor(kGray+3);
      //fOutRatioTH1DHistsCopy[th1d]->SetFillStyle(3003);
      fOutRatioTH1DHistsCopy[th1d]->SetFillStyle(3013);
      fOutRatioTH1DHistsCopy[th1d]->SetMarkerSize(0);
      fOutRatioTH1DHistsCopy[th1d]->SetBinContent(bin,1.0);
      fOutRatioTH1DHistsCopy[th1d]->SetBinError(bin,ratErr);
    }
  }// end loop over hists

}// end Combiner::MakeRatioPlots

Double_t Combiner::GetMaximum(const UInt_t th1d, const Bool_t stack) {
  Double_t max = -100;

  std::vector<Double_t> tmpmax;
  for (UInt_t mc = 0; mc < fNSig; mc++){
    tmpmax.push_back( fInSigTH1DHists[th1d][mc]->GetBinContent(fInSigTH1DHists[th1d][mc]->GetMaximumBin()));
  }
  if (fNData > 0 && stack) tmpmax.push_back(fOutDataTH1DHists[th1d]->GetBinContent(fOutDataTH1DHists[th1d]->GetMaximumBin()));
  if (stack) tmpmax.push_back(fOutBkgTH1DStacks[th1d]->GetMaximum());
  else{
    for (UInt_t mc = 0; mc < fNBkg; mc++){
      tmpmax.push_back(fInBkgTH1DHists[th1d][mc]->GetBinContent(fInBkgTH1DHists[th1d][mc]->GetMaximumBin()));
    }
  }

  for (UInt_t i = 0; i < tmpmax.size(); i++){
    if ( tmpmax[i] > max ) max = tmpmax[i];
  }

  return max;
}// end Combiner::GetMaximum

Double_t Combiner::GetMinimum(const UInt_t th1d, const Bool_t stack) {
  // need to loop through to check bin != 0
  Double_t datamin  = 1e20;

  if (fNData > 0){ 
    for (Int_t bin = 1; bin <= fOutDataTH1DHists[th1d]->GetNbinsX(); bin++){
      Float_t tmpmin = fOutDataTH1DHists[th1d]->GetBinContent(bin);
      if ((tmpmin < datamin) && (tmpmin > 0)) {
        datamin    = tmpmin;
      }
    } 
  }
  return datamin;
}// end Combiner::GetMinimum

void Combiner::InitCanvAndHists(){
  // open input files into TFileVec for data
  fDataFiles.resize(fNData);
  for (UInt_t data = 0; data < fNData; data++) {
    TString datafile = Form("%s%s/plots_%s.root",fOutDir.Data(),fDataNames[data].Data(),fDataNames[data].Data());
    fDataFiles[data] = TFile::Open(datafile.Data());
    CheckValidFile(fDataFiles[data],datafile);
  }

  // open input files into TFileVec for bkg
  fBkgFiles.resize(fNBkg);
  for (UInt_t mc = 0; mc < fNBkg; mc++) {
    TString bkgfile = Form("%s%s/plots_%s.root",fOutDir.Data(),fBkgNames[mc].Data(),fBkgNames[mc].Data());
    fBkgFiles[mc] = TFile::Open(bkgfile.Data());
    CheckValidFile(fBkgFiles[mc],bkgfile);
  }

  // open input files into TFileVec for bkg
  fSigFiles.resize(fNSig);
  for (UInt_t mc = 0; mc < fNSig; mc++) {
    TString sigfile = Form("%s%s/plots_%s.root",fOutDir.Data(),fSigNames[mc].Data(),fSigNames[mc].Data());
    fSigFiles[mc] = TFile::Open(sigfile.Data());
    CheckValidFile(fSigFiles[mc],sigfile);
  }

  fInDataTH1DHists.resize(fNTH1D);
  fInBkgTH1DHists.resize(fNTH1D);
  fInSigTH1DHists.resize(fNTH1D);

  for (UInt_t th1d = 0; th1d < fNTH1D; th1d++){ // loop over double hists
    fInDataTH1DHists[th1d].resize(fNData); 
    for (UInt_t data = 0; data < fNData; data++) { // init data double hists
      fInDataTH1DHists[th1d][data] = (TH1D*)fDataFiles[data]->Get(Form("%s%s",fTH1DNames[th1d].Data(),addText.Data()));
      CheckValidTH1D(fInDataTH1DHists[th1d][data],fTH1DNames[th1d],fDataFiles[data]->GetName());
    }
    fInBkgTH1DHists[th1d].resize(fNBkg); 
    for (UInt_t mc = 0; mc < fNBkg; mc++) { // init bkg double hists
      fInBkgTH1DHists[th1d][mc] = (TH1D*)fBkgFiles[mc]->Get(Form("%s%s",fTH1DNames[th1d].Data(),addText.Data()));
      CheckValidTH1D(fInBkgTH1DHists[th1d][mc],fTH1DNames[th1d],fBkgFiles[mc]->GetName());
      fInBkgTH1DHists[th1d][mc]->SetFillColor(fColorMap[fBkgNames[mc]]);
      fInBkgTH1DHists[th1d][mc]->SetLineColor(kBlack);
    }
    fInSigTH1DHists[th1d].resize(fNSig); 
    for (UInt_t mc = 0; mc < fNSig; mc++) { // init sig double hists
      fInSigTH1DHists[th1d][mc] = (TH1D*)fSigFiles[mc]->Get(Form("%s%s",fTH1DNames[th1d].Data(),addText.Data()));
      CheckValidTH1D(fInSigTH1DHists[th1d][mc],fTH1DNames[th1d],fSigFiles[mc]->GetName());
      fInSigTH1DHists[th1d][mc]->SetLineColor(fColorMap[fSigNames[mc]]);
      fInSigTH1DHists[th1d][mc]->SetLineWidth(3);
    }
  }

  // output histos
  fOutDataTH1DHists.resize(fNTH1D);
  fOutHiggsBkgTH1DHists.resize(fNTH1D);
  fOutEWK1phoBkgTH1DHists.resize(fNTH1D);
  fOutEWK2phoBkgTH1DHists.resize(fNTH1D);
  fOutJetsphoBkgTH1DHists.resize(fNTH1D);
  fOutDYJetsBkgTH1DHists.resize(fNTH1D);
  fOutBkgTH1DStacks.resize(fNTH1D);
  fOutBkgTH1DStacksForUncer.resize(fNTH1D);
  for (UInt_t th1d = 0; th1d < fNTH1D; th1d++){
    fOutBkgTH1DStacks[th1d] = new THStack("","");
    fOutBkgTH1DStacksForUncer[th1d] = new THStack("","");
  }

  fTH1DLegends.resize(fNTH1D);
  fTH1DLegendsSig.resize(fNTH1D);
  for (UInt_t th1d = 0; th1d < fNTH1D; th1d++){
    fTH1DLegends[th1d] = new TLegend(0.56,0.69,0.92,0.92,NULL,"brNDC"); // (x1,y1,x2,y2)
    //fTH1DLegends[th1d] = new TLegend(0.48,0.76,0.9,0.934); // (x1,y1,x2,y2)
    fTH1DLegends[th1d]->SetTextSize(0.036);
    fTH1DLegends[th1d]->SetNColumns(2);
    fTH1DLegends[th1d]->SetBorderSize(0);
    fTH1DLegends[th1d]->SetLineColor(1);
    fTH1DLegends[th1d]->SetLineStyle(1);
    fTH1DLegends[th1d]->SetLineWidth(1);
    fTH1DLegends[th1d]->SetFillColor(0);
    fTH1DLegends[th1d]->SetFillStyle(0);
    fTH1DLegends[th1d]->SetTextFont(42);


    fTH1DLegendsSig[th1d] = new TLegend(0.56,0.55,0.74,0.68); // (x1,y1,x2,y2)
    fTH1DLegendsSig[th1d]->SetHeader("m_{A} = 300 GeV with #sigma = 1pb");
    fTH1DLegendsSig[th1d]->SetTextSize(0.036);
    fTH1DLegendsSig[th1d]->SetNColumns(1);
    fTH1DLegendsSig[th1d]->SetBorderSize(0);
    fTH1DLegendsSig[th1d]->SetLineColor(1);
    fTH1DLegendsSig[th1d]->SetLineStyle(1);
    fTH1DLegendsSig[th1d]->SetLineWidth(1);
    fTH1DLegendsSig[th1d]->SetFillColor(0);
    fTH1DLegendsSig[th1d]->SetFillStyle(0);
    fTH1DLegendsSig[th1d]->SetTextFont(42);
  }


  GJetsClone.resize(fNTH1D);
  fOutTH1DCanvases.resize(fNTH1D);
  fOutTH1DStackPads.resize(fNTH1D);
  fOutBkgTH1DHists.resize(fNTH1D);
  //if (fNData > 0){
    fOutRatioTH1DHists.resize(fNTH1D); 
    fOutRatioTH1DHistsCopy.resize(fNTH1D); 
    fOutTH1DRatioLines.resize(fNTH1D);
    fOutTH1DRatioPads.resize(fNTH1D);
  //} 

  for (UInt_t th1d = 0; th1d < fNTH1D; th1d++){
    //fOutTH1DCanvases[th1d] = new TCanvas(fTH1DNames[th1d].Data(),"");
    fOutTH1DCanvases[th1d] = new TCanvas(fTH1DNames[th1d].Data(),"",550,550);
    fOutTH1DCanvases[th1d]->cd();

    //fOutTH1DStackPads[th1d] = new TPad("","",0.01,0.2,0.99,1.);//x1,y1,x2,y2
    //fOutTH1DStackPads[th1d]->SetBottomMargin(0); // upper and lower pad are joined
    //fOutTH1DStackPads[th1d]->SetRightMargin(0.06); 
    //fOutTH1DStackPads[th1d]->SetLeftMargin(0.12); 
    fOutTH1DStackPads[th1d] = new TPad("","",0,0.3,1,1);//x1,y1,x2,y2
    fOutTH1DStackPads[th1d]->SetBottomMargin(0.03); // upper and lower pad are joined
    fOutTH1DStackPads[th1d]->SetTopMargin(0.06);
    fOutTH1DStackPads[th1d]->SetRightMargin(0.05040323); 
    fOutTH1DStackPads[th1d]->SetLeftMargin(0.1290323); 
    

    //if (fNData > 0){// for lower pad with ratio plot
      //fOutTH1DRatioPads[th1d] = new TPad("","",0.01,0.001,0.99,0.19);//x1,y1,x2,y2
      //fOutTH1DRatioPads[th1d]->SetTopMargin(0.01);
      //fOutTH1DRatioPads[th1d]->SetRightMargin(0.06);
      //fOutTH1DRatioPads[th1d]->SetLeftMargin(0.12);
      //fOutTH1DRatioPads[th1d]->SetBottomMargin(0.4);
      fOutTH1DRatioPads[th1d] = new TPad("","",0,0,1,0.3);//x1,y1,x2,y2
      fOutTH1DRatioPads[th1d]->SetTopMargin(0.0);
      fOutTH1DRatioPads[th1d]->SetRightMargin(0.05040323);
      fOutTH1DRatioPads[th1d]->SetLeftMargin(0.1290323);
      fOutTH1DRatioPads[th1d]->SetBottomMargin(0.366666678814);
      fOutTH1DRatioPads[th1d]->SetFrameFillStyle(0);
      fOutTH1DRatioPads[th1d]->SetFrameBorderMode(0);
      fOutTH1DRatioLines[th1d] = new TLine();
    //}
  }
}// end Combiner::InitCanvAndHists



void Combiner::InitTH1DNames(){
  
  // higgs & met variables
  fTH1DNames.push_back("mgg");
  fIndexMgg = fTH1DNames.size()-1;
  fTH1DNames.push_back("ptgg");
  fIndexPtgg = fTH1DNames.size()-1;
  fTH1DNames.push_back("nvtx"); 
  fIndexNvtx = fTH1DNames.size()-1;
  fTH1DNames.push_back("t1pfmetphi");
  //fTH1DNames.push_back("pfmetphi");
  //fTH1DNames.push_back("pfmet");
  //fTH1DNames.push_back("calometphi");
  //fTH1DNames.push_back("calomet");

  //// photon variables
  fTH1DNames.push_back("pt1_afterIDloose");
  fTH1DNames.push_back("eta1_afterIDloose");
  fTH1DNames.push_back("pt1_beforeIDloose");
  fTH1DNames.push_back("eta1_beforeIDloose");
  fTH1DNames.push_back("pt1");
  fTH1DNames.push_back("pt2");     
  fTH1DNames.push_back("eta1");
  fTH1DNames.push_back("eta2");
  fTH1DNames.push_back("phi1");
  fTH1DNames.push_back("phi2");
  fTH1DNames.push_back("r91");
  fTH1DNames.push_back("r92");

  // photon ID variables
  fTH1DNames.push_back("hoe1");
  fTH1DNames.push_back("hoe2");
  fTH1DNames.push_back("sieie1");
  fTH1DNames.push_back("sieie2");
  fTH1DNames.push_back("sieie1EB");
  fTH1DNames.push_back("sieie2EB");
  fTH1DNames.push_back("sieie1EE");
  fTH1DNames.push_back("sieie2EE");
  fTH1DNames.push_back("phoiso1");
  fTH1DNames.push_back("phoiso2");
  fTH1DNames.push_back("chiso1");
  fTH1DNames.push_back("chiso2");
  fTH1DNames.push_back("neuiso1");
  fTH1DNames.push_back("neuiso2");
  fTH1DNames.push_back("deltaRphotons");

  if (addText!="_n-1"){ // plots that don't have n-1 versions 
    //fTH1DNames.push_back("eleveto1");
    //fTH1DNames.push_back("eleveto2");
    //fTH1DNames.push_back("phi1_pho2pass");
    //fTH1DNames.push_back("phi2_pho1pass");
    fTH1DNames.push_back("nJets");
    fTH1DNames.push_back("nElec");
    fTH1DNames.push_back("nMuon");
    fTH1DNames.push_back("nJets_metCUT");
    fTH1DNames.push_back("nElec_metCUT");
    fTH1DNames.push_back("nMuon_metCUT");
    fTH1DNames.push_back("BDTindex");

    fTH1DNames.push_back("t1pfmetCorr");
    fTH1DNames.push_back("t1pfmetphiCorr");
    fTH1DNames.push_back("t1pfmet_zoom");
    fTH1DNames.push_back("t1pfmetSumEt");
    fTH1DNames.push_back("t1pfmetSumEtCorr");
    fTH1DNames.push_back("t1pfmet_partblind");
    fTH1DNames.push_back("t1pfmetCorr_partblind");
    fTH1DNames.push_back("ptggOverMET");
    
    fTH1DNames.push_back("t1pfmet");
    fIndexMET = fTH1DNames.size()-1;
    fTH1DNames.push_back("JetEnUp");
    fTH1DNames.push_back("JetEnDown");
    fTH1DNames.push_back("JetResUp");
    fTH1DNames.push_back("JetResDown");
    fTH1DNames.push_back("MuonEnUp");
    fTH1DNames.push_back("MuonEnDown");
    fTH1DNames.push_back("EleEnUp");
    fTH1DNames.push_back("EleEnDown");
    fTH1DNames.push_back("TauEnUp");
    fTH1DNames.push_back("TauEnDown");
    fTH1DNames.push_back("PhoEnUp");
    fTH1DNames.push_back("PhoEnDown");
    fTH1DNames.push_back("UnclEnUp");
    fTH1DNames.push_back("UnclEnDown");

    //fTH1DNames.push_back("ptJet1");
    //fTH1DNames.push_back("ptJet2");
    //fTH1DNames.push_back("phiJet1");
    //fTH1DNames.push_back("phiJet2");
    //fTH1DNames.push_back("etaJet1");
    //fTH1DNames.push_back("etaJet2");
    fTH1DNames.push_back("dphiJet1MET");
    fTH1DNames.push_back("dphiJet2MET");
    fTH1DNames.push_back("absdphiJet1MET");
    fTH1DNames.push_back("absdphiJet2MET");
    fTH1DNames.push_back("absdphi_maxJetMET");
    fTH1DNames.push_back("absdphi_minJetMET");
    fTH1DNames.push_back("absdphiUncorr_maxJetMET");
    fTH1DNames.push_back("absdphiUncorr_minJetMET");
    fTH1DNames.push_back("absdphi_maxgMET");
    fTH1DNames.push_back("absdphi_maxgMET_metCUT");
    fTH1DNames.push_back("absdphi_g1MET");
    fTH1DNames.push_back("absdphi_ggmet_metCUT");
    fTH1DNames.push_back("absdphi_g1MET_metCUT");
    fTH1DNames.push_back("absdphi_maxJetMET_metCUT");
    fTH1DNames.push_back("absdphi_minJetMET_metCUT");
    fTH1DNames.push_back("nvtx_afterJetCut");
    fTH1DNames.push_back("ptgg_afterJetCut");
    fTH1DNames.push_back("mgg_afterJetCut");
    fTH1DNames.push_back("nvtx_afterggMETCut");
    fTH1DNames.push_back("ptgg_afterggMETCut");
    fTH1DNames.push_back("pt1_afterggMETCut");
    fTH1DNames.push_back("pt2_afterggMETCut");

    //fTH1DNames.push_back("met_afterJetCut");
    //fTH1DNames.push_back("metCorr_afterJetCut");
    //fTH1DNames.push_back("met_afterggMETCut");
    //fTH1DNames.push_back("metCorr_afterggMETCut");
    //fTH1DNames.push_back("met_aftergMETCut");
    //fTH1DNames.push_back("metCorr_aftergMETCut");
    //fTH1DNames.push_back("met_afterJetMETPhiCut");
    //fTH1DNames.push_back("metCorr_afterJetMETPhiCut");
    //fTH1DNames.push_back("met_womaxJetMET");
    //fTH1DNames.push_back("metCorr_womaxJetMET");
    //fTH1DNames.push_back("met_maxJetMET");
    //fTH1DNames.push_back("metCorr_maxJetMET");
    //fTH1DNames.push_back("met_minJetMET");
    //fTH1DNames.push_back("metCorr_minJetMET");

    fTH1DNames.push_back("jetInfo_CHfrac1");	
    fTH1DNames.push_back("jetInfo_NHfrac1");	
    fTH1DNames.push_back("jetInfo_NEMfrac1");
    fTH1DNames.push_back("jetInfo_CEMfrac1");
    fTH1DNames.push_back("jetInfo_PHfrac1");	
    fTH1DNames.push_back("jetInfo_ELfrac1");
    fTH1DNames.push_back("jetInfo_MUfrac1");	
    fTH1DNames.push_back("jetInfo_CHmult1");
    fTH1DNames.push_back("jetInfo_NEmult1");
    fTH1DNames.push_back("jetInfo_pt1");
    fTH1DNames.push_back("jetInfo_eta1");
    fTH1DNames.push_back("jetInfo_phi1");
    fTH1DNames.push_back("jetInfo_mass1");

    fTH1DNames.push_back("met_Isolategg");
    fTH1DNames.push_back("metCorr_Isolategg");
    fTH1DNames.push_back("met_IsolateALL");
    fTH1DNames.push_back("metCorr_IsolateALL");
    //fTH1DNames.push_back("metCorr_IsolateALL_selmgg");
    fTH1DNames.push_back("metCorr_forShape");
    fTH1DNames.push_back("mgg_forShape");
    fTH1DNames.push_back("mgg_metCUT_forShape");
    fTH1DNames.push_back("mgg_IsolateALL");
    fTH1DNames.push_back("mgg_IsolateALL_pt1");
    fTH1DNames.push_back("mgg_IsolateALL_pt2");
    fTH1DNames.push_back("mgg_IsolateALL_ptgg");
    fTH1DNames.push_back("mgg_IsolateALL_woPtgg");
    fTH1DNames.push_back("mgg_IsolateALL_wPtgg");
    fTH1DNames.push_back("mgg_IsolateALLmetCUT");
    fTH1DNames.push_back("mgg_IsolateALLlowCUT");
    fTH1DNames.push_back("mgg_IsolateALLUncorrmetCUT");
    fTH1DNames.push_back("ptgg_IsolateALL");
    fTH1DNames.push_back("ptgg_IsolateALLmetCUT");
    fTH1DNames.push_back("nvtx_IsolateALL");
    fTH1DNames.push_back("nvtx_IsolateALLmetCUT");

    //fTH1DNames.push_back("pt1_afterMETcut");
    //fTH1DNames.push_back("pt2_afterMETcut");
    //fTH1DNames.push_back("ptgg_afterMETcut");
 
    //fTH1DNames.push_back("mgg_lowmet"); 
    //fTH1DNames.push_back("mgg_nocuts");
    //fTH1DNames.push_back("mgg_dphiJmet");
    //fTH1DNames.push_back("mgg_dphiGGmet");
    fTH1DNames.push_back("clean_dphi_min_gMET");
    fTH1DNames.push_back("clean_dphi_min_gMET_metCUT");
    fTH1DNames.push_back("clean_dphi_min_jMET_metCUT");
    fTH1DNames.push_back("clean_dphi_hMET_metCUT");

    fTH1DNames.push_back("studies_dphi_minjMET_lowMET");
    fTH1DNames.push_back("studies_dphi_hMET_lowMET");
    fTH1DNames.push_back("studies_dphi_minjMET_lowMET10");
    fTH1DNames.push_back("studies_dphi_hMET_lowMET10");
    fTH1DNames.push_back("studies_dphi_minjMET_lowMET20");
    fTH1DNames.push_back("studies_dphi_hMET_lowMET20");
    fTH1DNames.push_back("studies_dphi_minjMET_lowMET30");
    fTH1DNames.push_back("studies_dphi_hMET_lowMET30");
    fTH1DNames.push_back("studies_dphi_minjMET_lowMET40");
    fTH1DNames.push_back("studies_dphi_hMET_lowMET40");

    fTH1DNames.push_back("newstudies_MET_outmgg");	 
    fTH1DNames.push_back("newstudies_MET_selmgg");	 
    fTH1DNames.push_back("newstudies_MET_lowmgg");	 
    fTH1DNames.push_back("newstudies_MET_highmgg");	 
    fTH1DNames.push_back("newstudies_ptgg_lowMET");       
    fTH1DNames.push_back("newstudies_ptgg_highMET");      
    fTH1DNames.push_back("newstudies_njet_lowMET");     
    fTH1DNames.push_back("newstudies_njet_highMET");      
    fTH1DNames.push_back("newstudies_mgg_barrel");     
    fTH1DNames.push_back("newstudies_mgg_else");     
    fTH1DNames.push_back("newstudies_mgg_njet");     
    fTH1DNames.push_back("newstudies_mgg_ptgg");         
    fTH1DNames.push_back("newstudies_mgg_dphihMET");      
    fTH1DNames.push_back("newstudies_mgg_dphijMET");     
    fTH1DNames.push_back("newstudies_mgg_addnjet");     
    fTH1DNames.push_back("newstudies_mgg_addptgg");         
    fTH1DNames.push_back("newstudies_mgg_adddphihMET");      
    fTH1DNames.push_back("newstudies_mgg_adddphijMET");     
    fTH1DNames.push_back("newstudies_dphihMET_lowMET");   
    fTH1DNames.push_back("newstudies_dphihMET_highMET"); 
    fTH1DNames.push_back("newstudies_dphijMET_lowMET"); 
    fTH1DNames.push_back("newstudies_dphijMET_highMET");  
    fTH1DNames.push_back("newstudies_dphihMET_lowMET_outmgg");
    fTH1DNames.push_back("newstudies_dphihMET_lowMET_selmgg");
    fTH1DNames.push_back("newstudies_dphijMET_lowMET_outmgg");
    fTH1DNames.push_back("newstudies_dphijMET_lowMET_selmgg");
    fTH1DNames.push_back("newstudies_dphihMET_highMET_outmgg");
    fTH1DNames.push_back("newstudies_dphihMET_highMET_selmgg");
    fTH1DNames.push_back("newstudies_dphijMET_highMET_outmgg");
    fTH1DNames.push_back("newstudies_dphijMET_highMET_selmgg");

    //fTH1DNames.push_back("t1pfmet_zoom_wofil");
    fTH1DNames.push_back("mgg_selt1pfmet");
    fTH1DNames.push_back("t1pfmet_selmgg");
    fTH1DNames.push_back("t1pfmetCorr_selmgg_Varbin");
    fTH1DNames.push_back("t1pfmetCorr_selmgg");
    fTH1DNames.push_back("phigg");
    //fTH1DNames.push_back("ggphi_metCUT");
    //fTH1DNames.push_back("metphi_metCUT");
    fTH1DNames.push_back("dphi_ggmet");
    fTH1DNames.push_back("absdphi_ggJet1");
    fTH1DNames.push_back("absdphiUncorr_ggmet");
    fTH1DNames.push_back("absdphi_ggmet");
    fIndexDphi = fTH1DNames.size()-1;
    fTH1DNames.push_back("deta_gg");
    fTH1DNames.push_back("absdeta_gg");
    fIndexDeta = fTH1DNames.size()-1;
    fTH1DNames.push_back("selection");
    fTH1DNames.push_back("selection_unwgt");
    fIndexEff = fTH1DNames.size()-1;
    fTH1DNames.push_back("eff_sel");
 
    fTH1DNames.push_back("vtx_eff_ptzp_n");
    fTH1DNames.push_back("vtx_eff_ptzp_d");
    fTH1DNames.push_back("vtx_eff_nvtx_n");
    fTH1DNames.push_back("vtx_eff_nvtx_d");
    fTH1DNames.push_back("vtx_eff_met_n");
    fTH1DNames.push_back("vtx_eff_met_d");
    fTH1DNames.push_back("vtx_eff_njet_n");
    fTH1DNames.push_back("vtx_eff_njet_d");

    //fTH1DNames.push_back("EBHighR9_mgg");
    //fTH1DNames.push_back("EBHighR9_ptgg");
    //fTH1DNames.push_back("EBHighR9_t1pfmet");
    //fTH1DNames.push_back("EBLowR9_mgg");
    //fTH1DNames.push_back("EBLowR9_ptgg");
    //fTH1DNames.push_back("EBLowR9_t1pfmet");
    //fTH1DNames.push_back("EEHighR9_mgg");
    //fTH1DNames.push_back("EEHighR9_ptgg");
    //fTH1DNames.push_back("EEHighR9_t1pfmet");
    //fTH1DNames.push_back("EELowR9_mgg");
    //fTH1DNames.push_back("EELowR9_ptgg");
    //fTH1DNames.push_back("EELowR9_t1pfmet");

  }
}// end Combiner::InitTH1DNames
