
  //////////////////////////////////////////////////////////////////////////////////////
  //
  // To Compile:
  // make clean (if you have changed something in the .hh files)
  // make
  //
  // Call this script:
  // ./main
  //
  //////////////////////////////////////////////////////////////////////////////////////

#include "Plotter.hh"
#include "Combiner.hh"
#include "CardMaker.hh"
#include "ReweightPU.hh"
#include "METCorr2016.hh"
#include "ABCDMethod.hh"
#include "Style.hh"

#include "TROOT.h"
#include "TTree.h"
#include "TStyle.h"
#include <iostream>

typedef std::pair<TString,Double_t>  SampleYieldPair;
typedef std::vector<SampleYieldPair> SampleYieldPairVec;

static bool sortByYield(const SampleYieldPair& mcpair1, const SampleYieldPair& mcpair2) {
  return mcpair1.second<=mcpair2.second;
}

int main(){
  // force TDR style instead of : setTDRStyle(); 
  TStyle * tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  SetTDRStyle(tdrStyle);
  gROOT->ForceStyle();
 
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  //										      //
  // 				SET MAIN PARAMETERS HERE 			      //
  //										      //
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////

  int whichSelection = 1; // Choose which selection to apply
  TString selName = "";   
  if (whichSelection == 0) selName = "OrigSel";// no additional selection & MET > 70  
  if (whichSelection == 1) selName = "OptSel1";// for Data/MC plot using m600 cuts: pt1/m > 0.5,  pt2/m > 0.25, ptgg > 90, MET > 105  
  if (whichSelection == 2) selName = "OptSel2";// for Data/MC plot using m600 cuts: pt2/m > 0.55, pt2/m > 0.25, ptgg/MET > 0.5, MET > 95 
  if (whichSelection == 3) selName = "OptSel3";// pt1/m > 0.55, pt2/m > 0.25, ptgg > 85, MET > 50 (using ptgg w/ requirement on #events) 
  if (whichSelection == 4) selName = "OptSel4";// pt1/m > 0.45, pt2/m > 0.25, ptgg/MET > 0.2, MET > 70 (using ptgg/MET w/ requirement on #events)

  Double_t alpha = 0.19; // Scale factor for C&C 

  //////////////////////////////////////////////////////////////////////////////////////

  //TString inDir = "data/25ns_v80X_moriond17_v2/"; 					// input directory of the samples
  TString inDir = "/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v2/"; 		// input directory of the samples
  TString outDir = Form("./diPhoPlots/25ns_v80X_moriond17_v2_%s_ScaleToData/",selName.Data());	// output directory to send results
  TString origDir = "./diPhoPlots/25ns_v80X_moriond17_v2_OrigSel/";			// output with original sel. for ABCD with OptSel 1 or 2

  //////////////////////////////////////////////////////////////////////////////////////

  TString type = "png";		// type of plots to be made
  bool doMETCorr = false;	// redo the MET correction for MC and data, else take the Corr from the root file
  bool doPlots = false;		// make plots for each sample individually
  bool doComb = true;		// make stack/overlay plots
  bool doABCD = false;		// run ABCD method, NB: it crashes first time making output file but will run fine next time - this should be fixed. 
  bool makeDataCards = false;	// make datacards for hybrid method
  bool useOneSel = true;	// for datacards making: use m600 sel for all points (false uses optimized sel)
  bool doMergeBkgs = true;      // merge the Higgs,EWK bkgs to make combined plots nicer
  bool doQCDrescale = true;	// use the GJets sample reweighted to the QCD integral for the QCD (avoids events with big weights)

  bool doFakeData = false;	// use FakeData to test combiner (mimicks data)
  bool sortMC = false;		// use if want to sort bkg smallest to biggest, else uses order given
  bool doBlind = false;		// use to blind the analysis for Data (don't use distributions for 115<mgg<135)
  bool makePURWfiles = false;	// recompute PURW and make files (need also doReweightPU=true for this to run)
  bool doReweightPU = false;	// use PURW from old files if !makePURWfiles

  Double_t lumi =  35.8;  // in fb^-1  
  UInt_t nBins_vtx = 100; // number of bins for PURW 
  
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
 
  // Protect against unblinding accidentally 
  std::string input;
  if (doBlind) std::cout << "Doing Analysis Blinding Data" << std::endl;
  else {
    std::cout << "UNBLINDING DATA" << std::endl;
    std::cout << "Do you want to proceed? (yn)" << std::endl;
    std::cin >> input;
    if (input == "y") std::cout << "Proceeding with Unblinding" << std::endl;  
    else{
      std::cout << "CANCELLING" << std::endl;
      std::cout << "Please set 'doBlind=true'." << std::endl;
      doPlots = false;
      doComb = false;
      doABCD = false;
      makeDataCards = false;
      doReweightPU = false;
      makePURWfiles = false;
    }
  }

  /////////////////////////////////////////////////////
  //
  // MET Correction
  //
  // Inputs to METCorr
  // 1st : input directory of samples
  // 2nd : output directory
  // 3rd : name of sample
  // 4th : metCorr1 outputs
  // 5th : metCorr2 outputs
  //
  /////////////////////////////////////////////////////
 
  DblVec metCorrMC;
  DblVec metCorrData; 

  if (doMETCorr){

    std::cout << "Get MET Phi Correction MC" << std::endl;
    METCorr2016 * metcorrMC = new METCorr2016(0,inDir,outDir,"DiPhoton");
    metCorrMC = metcorrMC->Loop(inDir, "MC");    
    delete metcorrMC;
    
    std::cout << "Get MET Phi Correction Data" << std::endl;
    METCorr2016 * metcorrData = new METCorr2016(0,inDir,outDir,"DoubleEG");
    metCorrData = metcorrData->Loop(inDir, "Data");    
    delete metcorrData;    

    //for (UInt_t i=0; i<4; i++){
    //  std::cout << "MC   " << metCorrMC[i] << std::endl;
    //  std::cout << "Data " << metCorrData[i] << std::endl;
    //}
  }

  else{
    if (doPlots){
      std::cout << "Pickup MET Phi Correction" << std::endl; 
      // pick up MC metCorr
      TString metStudyMC = Form("%s/metCorr_MC.root",inDir.Data());
      TFile *fmetCorrMC = TFile::Open(metStudyMC.Data());
      CheckValidFile(fmetCorrMC,metStudyMC);
      TH1D *MCmet = (TH1D*)fmetCorrMC->Get("metCorr");  
      CheckValidTH1D(MCmet,"",metStudyMC);

      // pick up Data metCorr
      TString metStudyData = Form("%s/metCorr_Data.root",inDir.Data());
      TFile *fmetCorrDATA = TFile::Open(metStudyData.Data());
      CheckValidFile(fmetCorrDATA,metStudyData);
      TH1D *DATAmet = (TH1D*)fmetCorrDATA->Get("metCorr");  
      CheckValidTH1D(DATAmet,"",metStudyData);
       
      for (UInt_t i=0; i<4; i++){
        metCorrMC.push_back(MCmet->GetBinContent(i+1));
        metCorrData.push_back(DATAmet->GetBinContent(i+1));
      }
    }
  }


  /////////////////////////////////////////////////////
  //
  // Pile up reweighting
  //
  // Inputs to ReweightPU
  // 1st : MC Sample to weight
  // 2nd : Data Sample
  // 3rd : lumi
  // 4th : number of bins for nvtx
  // 5th : input directory of samples
  // 6th : output directory
  // 7th : type of plots
  //
  /////////////////////////////////////////////////////

  DblVec	puweights_Data;
  DblVec 	puweights_MC;

  // no puweight for data 
  for (UInt_t i=0; i<=nBins_vtx; i++){ puweights_Data.push_back(1.0); }

  if (doReweightPU){
    if (makePURWfiles){ 
      std::cout << "Doing PU Reweighting" << std::endl;
      ReweightPU * reweightQCD = new ReweightPU("DiPhoton","DoubleEG",lumi, nBins_vtx, inDir, outDir+"purw/", type);
      puweights_MC = reweightQCD->GetPUWeights();
      delete reweightQCD;

      //std::cout << "Doing PU Reweighting Sig" << std::endl;
      //ReweightPU * reweightDMH = new ReweightPU("2HDM_mZP1200","DoubleEG",lumi, nBins_vtx, inDir, outDir+"purw/");
      //puweights_Sig = reweightDMH->GetPUWeights();
      //delete reweightDMH;

      // create text files with purw values
      std::ofstream fOutPURWFileBkg;
      fOutPURWFileBkg.open(Form("%spurw/purw_bkg.txt",outDir.Data()));  
      //std::ofstream fOutPURWFileSig;
      //fOutPURWFileSig.open(Form("%spurw/purw_sig.txt",outDir.Data()));  

      for (UInt_t i=0; i<=nBins_vtx; i++){ //i=0 corresponds to bin1 in nvtx distribution
        fOutPURWFileBkg << puweights_MC[i]     << std::endl;
        //fOutPURWFileSig << puweights_sig1000[i] << std::endl;
      }
      fOutPURWFileBkg.close();
      //fOutPURWFileSig.close();

    }// end if makePURWfiles

    else{ // load PURW from already made files
      TString fBkgName = Form("%spurw/PURW_MC.root",outDir.Data());
      //TString fBkgName = Form("%spurw/PURW_zmumu.root",outDir.Data());
      TFile *fBkg = TFile::Open(fBkgName.Data());
      CheckValidFile(fBkg,fBkgName);
      TH1D *fBkgRatio = (TH1D*)fBkg->Get("nvtx_dataOverMC");  
      CheckValidTH1D(fBkgRatio,"nvtx_dataOverMC",fBkgName);
      //TString fSigName = Form("%spurw/PURW_2HDM_mZP1200.root",outDir.Data());
      //TFile *fSig = TFile::Open(fSigName.Data());
      //CheckValidFile(fSig,fSigName);
      //TH1D *fSigRatio = (TH1D*)fSig->Get("nvtx_dataOverMC");  
      //CheckValidTH1D(fSigRatio,"nvtx_dataOverMC",fSigName);
       
      for (UInt_t i=0; i<=nBins_vtx; i++){
        puweights_MC.push_back(fBkgRatio->GetBinContent(i+1));
        //puweights_sig.push_back(fSigRatio->GetBinContent(i));
      }
    }
  }// end doReweightPU 

  else{ // if not doReweightPU, set puweights to 1
    std::cout << "No PU Reweighting applied" << std::endl;
    for (UInt_t i=0; i<=nBins_vtx; i++){
      puweights_MC.push_back(1.0);
      //puweights_sig.push_back(1.0);
    }
  }  

  //std::cout << "PU reweight values "<<std::endl;
  //for (UInt_t i=1; i<=nBins_vtx; i++){
  //  std::cout << "puweights_Data " << puweights_Data[i]    << std::endl;
  //  std::cout << "puweights_MC   " << puweights_MC[i]     << std::endl;   
  //  std::cout << "puweights_sig  " << puweights_sig[i]    << std::endl;
  //}

  if (doReweightPU) std::cout << "Finished PU Reweighting" << std::endl;


  /////////////////////////////////////////////////////
  //
  // Make plots for each sample
  //
  // Arguments to Plotter:
  // 1st : location of input data
  // 2nd : output data location
  // 3rd : name of sample
  // 4th : puweights vector 
  // 5th : lumi of data
  // 6th : bool isData
  // 7th : bool doBlinding
  // 8th : type of plots
  // 9th : metCorr vector
  // 10th: which selection
  //
  /////////////////////////////////////////////////////

  if (doFakeData){
    std::cout << "Working on FakeData sample" << std::endl;
    Plotter * FakeData = new Plotter(inDir,outDir,"FakeData",puweights_Data,lumi,true,doBlind,type,metCorrData,whichSelection);
    FakeData->DoPlots(0);
    delete FakeData;
    std::cout << "Finished FakeData sample" << std::endl;
  }
  if (doPlots){
    std::cout << "Working on DoubleEG sample" << std::endl;
    Plotter * dEG = new Plotter(inDir,outDir,"DoubleEG",puweights_Data,lumi,true,doBlind,type,metCorrData,whichSelection);
    dEG->DoPlots(0);
    delete dEG;
    std::cout << "Finished DoubleEG sample" << std::endl;

    std::cout << "Working on GJets sample" << std::endl;
    Plotter * GJets = new Plotter(inDir,outDir,"GJets",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    GJets->DoPlots(1);
    delete GJets;
    std::cout << "Finished GJets sample" << std::endl;

    std::cout << "Working on QCD sample" << std::endl;
    Plotter * QCD = new Plotter(inDir,outDir,"QCD",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    QCD->DoPlots(2);
    delete QCD;
    std::cout << "Finished QCD sample" << std::endl;

    std::cout << "Working on WZH sample" << std::endl;
    Plotter * WZH = new Plotter(inDir,outDir,"VHToGG",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    WZH->DoPlots(0);
    delete WZH;
    std::cout << "Finished WZH sample" << std::endl;

    std::cout << "Working on WGToLNuG sample" << std::endl;
    Plotter * WGToLNuG = new Plotter(inDir,outDir,"WGToLNuG",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    WGToLNuG->DoPlots(0);
    delete WGToLNuG;
    std::cout << "Finished WGToLNuG sample" << std::endl;

    std::cout << "Working on WJetsToLNu sample" << std::endl;
    Plotter * WJetsToLNu = new Plotter(inDir,outDir,"WJetsToLNu",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    WJetsToLNu->DoPlots(0);
    delete WJetsToLNu;
    std::cout << "Finished WJetsToLNu sample" << std::endl;

    std::cout << "Working on WZTo2L2Q sample" << std::endl;
    Plotter * WZTo2L2Q = new Plotter(inDir,outDir,"WZTo2L2Q",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    WZTo2L2Q->DoPlots(0);
    delete WZTo2L2Q;
    std::cout << "Finished WZTo2L2Q sample" << std::endl;

    std::cout << "Working on ZGTo2NuG sample" << std::endl;
    Plotter * ZGTo2NuG = new Plotter(inDir,outDir,"ZGTo2NuG",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    ZGTo2NuG->DoPlots(0);
    delete ZGTo2NuG;
    std::cout << "Finished ZGTo2NuG sample" << std::endl;

    std::cout << "Working on ZGTo2LG sample" << std::endl;
    Plotter * ZGTo2LG = new Plotter(inDir,outDir,"ZGTo2LG",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    ZGTo2LG->DoPlots(0);
    delete ZGTo2LG;
    std::cout << "Finished ZGTo2LG sample" << std::endl;

    std::cout << "Working on ZZTo2L2Q sample" << std::endl;
    Plotter * ZZTo2L2Q = new Plotter(inDir,outDir,"ZZTo2L2Q",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    ZZTo2L2Q->DoPlots(0);
    delete ZZTo2L2Q;
    std::cout << "Finished ZZTo2L2Q sample" << std::endl;

    //std::cout << "Working on ZZTo2L2Nu sample" << std::endl;
    //Plotter * ZZTo2L2Nu = new Plotter(inDir,outDir,"ZZTo2L2Nu",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    //ZZTo2L2Nu->DoPlots(0);
    //delete ZZTo2L2Nu;
    //std::cout << "Finished ZZTo2L2Nu sample" << std::endl;

    //std::cout << "Working on ZJets sample" << std::endl;
    //Plotter * ZJets = new Plotter(inDir,outDir,"ZJets",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    //ZJets->DoPlots(0);
    //delete ZJets;
    //std::cout << "Finished ZJets sample" << std::endl;

    std::cout << "Working on TGJets sample" << std::endl;
    Plotter * TGJets = new Plotter(inDir,outDir,"TGJets",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    TGJets->DoPlots(0);
    delete TGJets;
    std::cout << "Finished TGJets sample" << std::endl;

    std::cout << "Working on TTGJets sample" << std::endl;
    Plotter * TTGJets = new Plotter(inDir,outDir,"TTGJets",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    TTGJets->DoPlots(0);
    delete TTGJets;
    std::cout << "Finished TTGJets sample" << std::endl;

    std::cout << "Working on TTJets sample" << std::endl;
    Plotter * TTJets = new Plotter(inDir,outDir,"TTJets",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    TTJets->DoPlots(0);
    delete TTJets;
    std::cout << "Finished TTJets sample" << std::endl;

    std::cout << "Working on TTGG_0Jets sample" << std::endl;
    Plotter * TTGG0Jets = new Plotter(inDir,outDir,"TTGG_0Jets",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    TTGG0Jets->DoPlots(0);
    delete TTGG0Jets;
    std::cout << "Finished TTGG_0Jets sample" << std::endl;

    std::cout << "Working on GluGluH sample" << std::endl;
    Plotter * GGHGG = new Plotter(inDir,outDir,"GluGluHToGG",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    GGHGG->DoPlots(0);
    delete GGHGG;
    std::cout << "Finished GluGluH sample" << std::endl;

    std::cout << "Working on ttHJetToGG sample" << std::endl;
    Plotter * ttH = new Plotter(inDir,outDir,"ttHJetToGG",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    ttH->DoPlots(0);
    delete ttH;
    std::cout << "Finished ttHJetToGG sample" << std::endl;

    std::cout << "Working on VBFHToGG sample" << std::endl;
    Plotter * VBF = new Plotter(inDir,outDir,"VBFHToGG",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    VBF->DoPlots(0);
    delete VBF;
    std::cout << "Finished VBFHToGG sample" << std::endl;
  
    std::cout << "Working on DiPhoton sample" << std::endl;
    Plotter * GG = new Plotter(inDir,outDir,"DiPhoton",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    GG->DoPlots(0);
    delete GG;
    std::cout << "Finished DiPhoton sample" << std::endl;

    std::cout << "Working on DYJets sample" << std::endl;
    Plotter * DY = new Plotter(inDir,outDir,"DYJetsToLL",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    DY->DoPlots(0);
    delete DY;
    std::cout << "Finished DYJets sample" << std::endl;

    std::cout << "Working on DMHgg 2HDM MZP600 sample" << std::endl;
    Plotter * DMH_mZP600 = new Plotter(inDir,outDir,"2HDM_mZP600_mA0300",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    DMH_mZP600->DoPlots(0);
    delete DMH_mZP600;
    std::cout << "Finished DMHgg 2HDM MZP600 sample" << std::endl;
   
    std::cout << "Working on DMHgg 2HDM MZP800 sample" << std::endl;
    Plotter * DMH_mZP800 = new Plotter(inDir,outDir,"2HDM_mZP800_mA0300",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    DMH_mZP800->DoPlots(0);
    delete DMH_mZP800;
    std::cout << "Finished DMHgg 2HDM MZP800 sample" << std::endl;
   
    std::cout << "Working on DMHgg 2HDM MZP1000 sample" << std::endl;
    Plotter * DMH_mZP1000 = new Plotter(inDir,outDir,"2HDM_mZP1000_mA0300",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    DMH_mZP1000->DoPlots(0);
    delete DMH_mZP1000;
    std::cout << "Finished DMHgg 2HDM MZP1000 sample" << std::endl;
   
    //std::cout << "Working on DMHgg 2HDM MZP1200 sample" << std::endl;
    //Plotter * DMH_mZP1200 = new Plotter(inDir,outDir,"2HDM_mZP1200_mA0300",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    //DMH_mZP1200->DoPlots(0);
    //delete DMH_mZP1200;
    //std::cout << "Finished DMHgg 2HDM MZP1200 sample" << std::endl;

    //std::cout << "Working on DMHgg 2HDM MZP1400 sample" << std::endl;
    //Plotter * DMH_mZP1400 = new Plotter(inDir,outDir,"2HDM_mZP1400_mA0300",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    //DMH_mZP1400->DoPlots(0);
    //delete DMH_mZP1400;
    //std::cout << "Finished DMHgg 2HDM MZP1400 sample" << std::endl;

    //std::cout << "Working on DMHgg 2HDM MZP1700 sample" << std::endl;
    //Plotter * DMH_mZP1700 = new Plotter(inDir,outDir,"2HDM_mZP1700_mA0300",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    //DMH_mZP1700->DoPlots(0);
    //delete DMH_mZP1700;
    //std::cout << "Finished DMHgg 2HDM MZP1700 sample" << std::endl;

    //std::cout << "Working on DMHgg 2HDM MZP2000 sample" << std::endl;
    //Plotter * DMH_mZP2000 = new Plotter(inDir,outDir,"2HDM_mZP2000_mA0300",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    //DMH_mZP2000->DoPlots(0);
    //delete DMH_mZP2000;
    //std::cout << "Finished DMHgg 2HDM MZP2000 sample" << std::endl;

    //std::cout << "Working on DMHgg 2HDM MZP2500 sample" << std::endl;
    //Plotter * DMH_mZP2500 = new Plotter(inDir,outDir,"2HDM_mZP2500_mA0300",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    //DMH_mZP2500->DoPlots(0);
    //delete DMH_mZP2500;
    //std::cout << "Finished DMHgg 2HDM MZP2500 sample" << std::endl;
    //
    //std::cout << "Working on DMHgg 2HDM MZP600 MA0-400 sample" << std::endl;
    //Plotter * DMH_mZP600_mA0400 = new Plotter(inDir,outDir,"2HDM_mZP600_mA0400",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    //DMH_mZP600_mA0400->DoPlots(0);
    //delete DMH_mZP600_mA0400;
    //std::cout << "Finished DMHgg 2HDM MZP600 MA0-400 sample" << std::endl;
   
    //std::cout << "Working on DMHgg 2HDM MZP800 MA0-400 sample" << std::endl;
    //Plotter * DMH_mZP800_mA0400 = new Plotter(inDir,outDir,"2HDM_mZP800_mA0400",puweights_MC,lumi,false,doBlind,type,metCorrMC,whichSelection);
    //DMH_mZP800_mA0400->DoPlots(0);
    //delete DMH_mZP800_mA0400;
    //std::cout << "Finished DMHgg 2HDM MZP800 MA0-400 sample" << std::endl;
   
    //std::cout << "Working on DMHgg M1000 sample" << std::endl;
    //Plotter * DMH_M1000 = new Plotter(inDir,outDir,"DMHtoGG_M1000",puweights_sig,lumi,false,doBlind,type,metCorrMC,whichSelection);
    //DMH_M1000->DoPlots(0);
    //delete DMH_M1000;
    //std::cout << "Finished DMHgg M1000 sample" << std::endl;
  

  }// end doPlots


  // setup all samples for Combiner and ABCD
  ColorMap colorMap;
  colorMap["QCD"] 			= kYellow+8;
  colorMap["GJets"] 			= kGreen-9;
  colorMap["VHToGG"]			= kOrange-3;
  colorMap["ZJets"]			= kBlue;
  colorMap["GluGluHToGG"]		= kOrange-2;
  colorMap["ttHJetToGG"]		= kOrange-4;
  colorMap["VBFHToGG"]			= kYellow-7;
  colorMap["DiPhoton"]			= kTeal-1;
  colorMap["DYJetsToLL"]		= kTeal-7;
  colorMap["TGJets"]			= kAzure+3;
  colorMap["TTGJets"]			= kAzure+2;
  colorMap["TTGG_0Jets"]		= kBlue-9;
  colorMap["TTJets"]			= kBlue-7;
  colorMap["ZGTo2NuG"]			= kCyan+1;
  colorMap["ZGTo2LG"]			= kCyan;
  colorMap["WZTo2L2Nu"]			= kTeal;
  colorMap["WJetsToLNu"]		= kBlue+5;
  colorMap["ZZTo2L2Nu"]			= kTeal+1;
  colorMap["ZZTo2L2Q"]			= kTeal+5;
  colorMap["WGToLNuG"]			= kAzure+8;
  colorMap["DMHtoGG_M1"]		= kPink-2;
  colorMap["DMHtoGG_M10"]		= kPink-6;
  colorMap["DMHtoGG_M100"]		= kPink+6;
  colorMap["DMHtoGG_M1000"]		= kPink+8;
  colorMap["DoubleEG"]			= kBlack;
  colorMap["FakeData"]			= kBlack;
  colorMap["2HDM_mZP600_mA0300"]	= kPink-2;
  colorMap["2HDM_mZP800_mA0300"]	= kMagenta+2;
  colorMap["2HDM_mZP1000_mA0300"]	= kMagenta;
  colorMap["2HDM_mZP1200_mA0300"]	= kPink-6;
  colorMap["2HDM_mZP1400_mA0300"]	= kPink+4;
  colorMap["2HDM_mZP1700_mA0300"]	= kMagenta-2;
  colorMap["2HDM_mZP2000_mA0300"]	= kPink-1;
  colorMap["2HDM_mZP2500_mA0300"]	= kPink;
  colorMap["2HDM_mZP600_mA0400"]	= kRed;
  colorMap["2HDM_mZP800_mA0400"]	= kRed-4;

  colorMap["SMHiggs"]			= kOrange-2;
  colorMap["EWK1pho"]			= kAzure+8;
  colorMap["EWK2pho"]			= kAzure+2;
  colorMap["Jetspho"]			= kGreen-9;

  SamplePairVec Samples; // vector to also be used for stack plots
  //ordered to match Livia
  Samples.push_back(SamplePair("ttHJetToGG",1)); 
  Samples.push_back(SamplePair("VHToGG",1));
  Samples.push_back(SamplePair("VBFHToGG",1)); 
  Samples.push_back(SamplePair("GluGluHToGG",1)); 
  Samples.push_back(SamplePair("TTGG_0Jets",1));
  Samples.push_back(SamplePair("TGJets",1));
  Samples.push_back(SamplePair("TTGJets",1));
  Samples.push_back(SamplePair("TTJets",1));
  Samples.push_back(SamplePair("WGToLNuG",1));
  Samples.push_back(SamplePair("WJetsToLNu",1));
  Samples.push_back(SamplePair("WZTo2L2Q",1));
  //Samples.push_back(SamplePair("ZZTo2L2Nu",1));
  Samples.push_back(SamplePair("ZGTo2NuG",1));
  Samples.push_back(SamplePair("ZGTo2LG",1));
  Samples.push_back(SamplePair("ZZTo2L2Q",1));
  //Samples.push_back(SamplePair("ZJets",1)); 
  Samples.push_back(SamplePair("DYJetsToLL",1));
  Samples.push_back(SamplePair("QCD",1)); 
  Samples.push_back(SamplePair("GJets",1)); 
  Samples.push_back(SamplePair("DiPhoton",1));
  //Samples.push_back(SamplePair("DMHtoGG_M1",0)); 
  //Samples.push_back(SamplePair("DMHtoGG_M10",0)); 
  //Samples.push_back(SamplePair("DMHtoGG_M100",0)); 
  //Samples.push_back(SamplePair("DMHtoGG_M1000",0)); 
  Samples.push_back(SamplePair("DoubleEG",5));
  if (doFakeData) Samples.push_back(SamplePair("FakeData",5));
  Samples.push_back(SamplePair("2HDM_mZP600_mA0300",0)); 
  Samples.push_back(SamplePair("2HDM_mZP800_mA0300",0)); 
  Samples.push_back(SamplePair("2HDM_mZP1000_mA0300",0)); 
  //Samples.push_back(SamplePair("2HDM_mZP1200_mA0300",0)); 
  //Samples.push_back(SamplePair("2HDM_mZP1400_mA0300",0)); 
  //Samples.push_back(SamplePair("2HDM_mZP1700_mA0300",0)); 
  //Samples.push_back(SamplePair("2HDM_mZP2000_mA0300",0));  
  //Samples.push_back(SamplePair("2HDM_mZP2500_mA0300",0));  
  //Samples.push_back(SamplePair("2HDM_mZP600_mA0400",0));
  //Samples.push_back(SamplePair("2HDM_mZP800_mA0400",0));

  UInt_t nbkg = 0;
  UInt_t nsig = 0;
  UInt_t ndata = 0;
  
  for (SamplePairVecIter iter=Samples.begin(); iter != Samples.end(); ++iter){
    std::cout << "Analyzing Sample: "<< (*iter).first.Data() << std::endl;
    if ((*iter).second == 1) {nbkg++;}
    else if ((*iter).second == 0) {nsig++;}
    else {ndata++;} 
  }
  UInt_t nsamples = nbkg + nsig + ndata;
   
  SamplePairVec BkgSamples;
  SamplePairVec SigSamples;
  SamplePairVec DataSamples;
  for (UInt_t isample = 0; isample < nsamples; isample++){
    if (Samples[isample].second == 0) SigSamples.push_back(Samples[isample]);
    else if (Samples[isample].second == 1) BkgSamples.push_back(Samples[isample]);
    else  DataSamples.push_back(Samples[isample]);
  }

  if (sortMC){
  // to sort MC by smallest to largest for nice stacked plots
  SampleYieldPairVec tmp_mcyields;
  for (UInt_t mc = 0; mc < nbkg; mc++) {
      // open mc file first
      TString mcfilename = Form("%s%s/plots_%s.root",outDir.Data(),BkgSamples[mc].first.Data(),BkgSamples[mc].first.Data());
      TFile * tmp_mcfile = TFile::Open(mcfilename.Data());
      // open nvtx plot
      TH1D * tmpnvtx = (TH1D*)tmp_mcfile->Get("nvtx_n-1");
      // get yield and push back with corresponding sample name
      tmp_mcyields.push_back(SampleYieldPair(BkgSamples[mc].first,tmpnvtx->Integral()));
  
      delete tmpnvtx;
      delete tmp_mcfile;
   }
  
   std::sort(tmp_mcyields.begin(),tmp_mcyields.end(),sortByYield);
   std::cout << "Finished sorting MC, now put samples in right order to be processed" << std::endl;
   BkgSamples.clear();
    for (UInt_t mc = 0; mc < nbkg; mc++) { // init mc double hists
      BkgSamples.push_back(SamplePair(tmp_mcyields[mc].first,1));
    }

  Samples.clear();
  for (UInt_t data = 0; data < ndata; data++ ) {
    Samples.push_back(DataSamples[data]);
  }
  for (UInt_t mc = 0; mc < nbkg; mc++ ) {
    Samples.push_back(BkgSamples[mc]);
  }
  for (UInt_t mc = 0; mc < nsig; mc++) {
    Samples.push_back(SigSamples[mc]);
  }
  }// end if sortMC

  /////////////////////////////////////////////////////
  //
  // Make data cards using hybrid method
  //
  // Arguements to CardMaker:
  //
  // 1st : alpha (scale factor)
  // 2nd : lumi
  // 3rd : PU weight vector
  // 4th : input directory
  // 5th : output directory
  // 6th : do Blind (blind the observed # events)
  // 7th : use one sel for all points or use optimized one
  // 8th : type of plots out 
  //
  /////////////////////////////////////////////////////

  if (makeDataCards){
    std::cout << "Making datacards using the hybrid method" << std::endl;
    CardMaker *cards = new CardMaker(alpha,lumi,puweights_MC,inDir,outDir,doBlind,useOneSel,type);
    cards->MakeCards();
    delete cards;
    std::cout << "Finished making datacards" << std::endl; 
  }


  ////////////////////////////////////////////////////
  //
  // Make comb (stack & overlay) plots w/ all samples 
  //
  // Arguments of Combiner
  // 1st : SamplePairVec (Samples) that has Name,VALUE
  // 2rd : lumi
  // 3rd : ColorMap for samples
  // 4th : output directory
  // 5th : bool do N-1 plots
  // 6th : bool do Stack plots (false = do overlay)
  // 7th : type of plots out 
  // 8th : bool do rescaling of GJets to replace QCD sample
  // 9th : whichSelection (OrigSel/OptSel1..)
  // 10th: bool doMergeBkgs (plots have some bkgs merged together) 
  //
  ////////////////////////////////////////////////////

  if (doComb){// make overlayed and stack plots
    // Combiner( Samples, lumi, colorMap , outDir, doNmin1plots, doStack)
    
    // do overlay plots for normal plots
    Combiner *combAll = new Combiner(Samples,lumi,colorMap,outDir,false,false,type,doQCDrescale,whichSelection,doMergeBkgs);
    combAll->DoComb();
    delete combAll;   
    // do stack plots for normal plots
    Combiner *stackAll = new Combiner(Samples,lumi,colorMap,outDir,false,true,type,doQCDrescale,whichSelection,doMergeBkgs);
    stackAll->DoComb();
    delete stackAll;   
 
    ////// do overlay plots for n-1 plots
    //Combiner *combAlln1 = new Combiner(Samples,lumi,colorMap,outDir,true,false,type,doQCDrescale,whichSelection);
    //combAlln1->DoComb();
    //delete combAlln1;   
    ////// do stack plots for n-1 plots 
    //Combiner *stackAlln1 = new Combiner(Samples,lumi,colorMap,outDir,true,true,type,doQCDrescale,whichSelection);
    //stackAlln1->DoComb();
    //delete stackAlln1;   
  }// end doComb

  ////////////////////////////////////////////////////
  //
  // Make comb (stack & overlay) plots w/ all samples 
  //
  // Arguments of Combiner
  // 1st : SamplePairVec (Samples) that has Name,VALUE
  // 2rd : lumi
  // 3rd : input directory
  // 4th : output directory
  // 5th : bool do rescaling of GJets to replace QCD sample
  // 6th : which selection (this just affects the MET cut)
  //
  ////////////////////////////////////////////////////

  if (doABCD){
    if (whichSelection!=1 && whichSelection!=2){
      ABCDMethod *abcd = new ABCDMethod(Samples,lumi,outDir,outDir,doBlind,doQCDrescale,whichSelection,0);
      abcd->DoAnalysis(0);
      delete abcd; 
    }
    else{
      UInt_t NumMasses = 8; // 0 runs just the cut for the lowest mass, the selection for each mass  
      for (UInt_t mass = 1; mass < NumMasses; mass++){
        ABCDMethod *abcd = new ABCDMethod(Samples,lumi,origDir,outDir,doBlind,doQCDrescale,whichSelection,mass);
        abcd->DoAnalysis(mass);
        delete abcd;
      }
    }
  }// end doABCD

  //clear the vectors after they have been used
  puweights_Data.clear();
  puweights_MC.clear();
  //puweights_sig.clear();

  delete tdrStyle;

}// end main
