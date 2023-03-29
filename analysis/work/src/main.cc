//------------------------------------------------------------------------
// To clean:   make clean
// To compile: make
// To call:    ./main --help (gives config options)
//------------------------------------------------------------------------

#include "../interface/CommonTypes.hh"
#include "../interface/Common.hh"
#include "../interface/Config.hh"
#include "../interface/Analysis.hh"
#include "../interface/StackPlots.hh"
#include "../interface/OverlayPlots.hh"
#include "../interface/CardMaker.hh"
#include "../interface/METcorr.hh"

#include "TROOT.h"
#include <iostream>

void InitializeMain(std::ofstream &yields, TStyle *& tdrStyle)
{

  //------------------------------------------------------------------------
  // Set TDR style 
  //------------------------------------------------------------------------
  tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  SetTDRStyle(tdrStyle);
  gROOT->ForceStyle();

  //------------------------------------------------------------------------
  // Make output dir 
  //------------------------------------------------------------------------
  MakeOutDir(Config::outdir);

  //------------------------------------------------------------------------
  // Set sample map 
  //------------------------------------------------------------------------
  if (Config::useData) Config::SampleMap["DoubleEG"] = true;  // isData
  if (Config::doAnalysis || Config::doStack || Config::makeCards || Config::doComb)
  {
    Config::SampleMap["VHToGG"]			= false; // !isData
    Config::SampleMap["GluGluHToGG"]		= false; // !isData
    Config::SampleMap["ttHJetToGG"]		= false; // !isData
    Config::SampleMap["VBFHToGG"]		= false; // !isData
    Config::SampleMap["TGJets"]			= false; // !isData
    Config::SampleMap["TTGJets"]		= false; // !isData
    Config::SampleMap["TTGG_0Jets"]		= false; // !isData
    Config::SampleMap["TTJets"]			= false; // !isData
    Config::SampleMap["ZGTo2LG"]		= false; // !isData
    Config::SampleMap["ZGTo2NuG"]		= false; // !isData
    Config::SampleMap["ZZTo2L2Q"]		= false; // !isData
    Config::SampleMap["WGToLNuG"]		= false; // !isData
    Config::SampleMap["WJetsToLNu"]		= false; // !isData
    Config::SampleMap["WZTo2L2Q"]		= false; // !isData
    Config::SampleMap["DYJetsToLL"]		= false; // !isData
    Config::SampleMap["DiPhoton"]		= false; // !isData 
    Config::SampleMap["QCD"]			= false; // !isData
    Config::SampleMap["GJets"]			= false; // !isData
    Config::SampleMap["2HDM_mZP600_mA0300"]	= false; // !isData
    Config::SampleMap["BaryonicZp_mZP10_mChi1"]	= false; // !isData
    //Config::SampleMap["2HDM_mZP800_mA0300"]     = false; // !isData
    //Config::SampleMap["2HDM_mZP1000_mA0300"]    = false; // !isData 
  }

  //------------------------------------------------------------------------
  // Make a yields table to sort samples 
  //------------------------------------------------------------------------
  //yields.open(Form("%s/yields.txt",Config::outdir.Data()),std::ios_base::app); // open the file in append mode
  yields.open(Form("%s/yields.txt",Config::outdir.Data())); // overwrite the file

  //------------------------------------------------------------------------
  // Set color map 
  //------------------------------------------------------------------------
  Config::colorMap["DoubleEG"]		     = kBlack;
  Config::colorMap["2HDM_mZP600_mA0300"]     = kBlue;
  Config::colorMap["BaryonicZp_mZP10_mChi1"] = kMagenta;
  //Config::colorMap["2HDM_mZP800_mA0300"]     = kMagenta;
  //Config::colorMap["2HDM_mZP1000_mA0300"]    = kGreen;
  Config::colorMap["DiPhoton"]		     = kTeal-1;
  Config::colorMap["SMHiggs"]		     = kOrange-2;
  Config::colorMap["EWK1pho"]		     = kAzure+8;
  Config::colorMap["EWK2pho"]		     = kAzure+2;
  Config::colorMap["Jetspho"]		     = kGreen-9;
  Config::colorMap["DYJetsToLL"]	     = kTeal-7;
  Config::colorMap["VHToGG"]		     = kRed-10;
  // colors only used if writing out all samples separately
  Config::colorMap["QCD"] 		     = kYellow+8;
  Config::colorMap["GJets"] 		     = kGreen-9;
  Config::colorMap["GluGluHToGG"]	     = kOrange-2;
  Config::colorMap["ttHJetToGG"]	     = kOrange-4;
  Config::colorMap["VBFHToGG"]		     = kYellow-7;
  Config::colorMap["TGJets"]		     = kAzure+3;
  Config::colorMap["TTGJets"]		     = kAzure+2;
  Config::colorMap["TTGG_0Jets"]	     = kBlue-9;
  Config::colorMap["TTJets"]		     = kBlue-7;
  Config::colorMap["ZJets"]		     = kBlue;
  Config::colorMap["ZGTo2LG"]		     = kCyan;
  Config::colorMap["ZGTo2NuG"]		     = kCyan+2;
  Config::colorMap["WZTo2L2Q"]		     = kTeal;
  Config::colorMap["WJetsToLNu"]	     = kBlue+1;
  Config::colorMap["ZZTo2L2Nu"]		     = kTeal+1;
  Config::colorMap["ZZTo2L2Q"]		     = kTeal+5;
  Config::colorMap["WGToLNuG"]		     = kAzure+8;

  //------------------------------------------------------------------------
  // Set outputname map 
  //------------------------------------------------------------------------
  Config::nameMap["DoubleEG"]                = "Data";
  Config::nameMap["2HDM_mZP600_mA0300"]      = "2HDM: m_{A} = 300 GeV, m_{Z'} = 600 GeV";
  Config::nameMap["BaryonicZp_mZP10_mChi1"]  = "Baryonic: m_{Z'} = 10 GeV, m_{#chi} = 1 GeV";
  //Config::nameMap["2HDM_mZP800_mA0300"]      = "m_{A} = 300 GeV, m_{Z'} = 800 GeV";
  //Config::nameMap["2HDM_mZP1000_mA0300"]     = "m_{A} = 300 GeV, m_{Z'} = 1000 GeV";
  Config::nameMap["DiPhoton"]                = "#gamma#gamma";
  Config::nameMap["SMHiggs"]                 = "h #rightarrow #gamma#gamma (ggF,VBF,tth)";
  Config::nameMap["EWK1pho"]                 = "#gamma + t, tt, W, Z";
  Config::nameMap["EWK2pho"]                 = "ZZ, ZW, tt + #gamma#gamma";
  Config::nameMap["Jetspho"]                 = "QCD, #gamma + jets";
  Config::nameMap["DYJetsToLL"]              = "DY + jets";
  Config::nameMap["VHToGG"]                  = "Vh"; 
  // names only used if writing out all samples separately
  Config::nameMap["QCD"]                     = "QCD"; 
  Config::nameMap["GJets"]                   = "#gamma + jets"; 
  Config::nameMap["GluGluHToGG"]             = "SM h (GluGlu)"; 
  Config::nameMap["ttHJetToGG"]              = "SM h (tth)"; 
  Config::nameMap["VBFHToGG"]                = "SM h (VBF)"; 
  Config::nameMap["TGJets"]                  = "T + #gamma"; 
  Config::nameMap["TTGJets"]                 = "TT + #gamma"; 
  Config::nameMap["TTGG_0Jets"]	             = "TT + #gamma#gamma"; 
  Config::nameMap["TTJets"]                  = "TT"; 
  Config::nameMap["ZJets"]                   = "Z + jets"; 
  Config::nameMap["ZGTo2LG"]                 = "Z(ll) + #gamma"; 
  Config::nameMap["ZGTo2NuG"]                = "Z(#nu#nu) + #gamma"; 
  Config::nameMap["WZTo2L2Q"]                = "WZ #rightarrow llqq"; 
  Config::nameMap["WJetsToLNu"]              = "W(l#nu) + jets"; 
  Config::nameMap["ZZTo2L2Nu"]               = "ZZ #rightarrow ll#nu#nu"; 
  Config::nameMap["ZZTo2L2Q"]                = "ZZ #rightarrow llqq"; 
  Config::nameMap["WGToLNuG"]                = "W(l#nu) + #gamma"; 
  

}// end initializing

void DestroyMain(std::ofstream & yields, TStyle *& tdrStyle) 
{
  yields.close();
  delete tdrStyle;
}


int main(int argc, const char* argv[])
{

  //------------------------------------------------------------------------
  // Read in command line options 
  //------------------------------------------------------------------------
  lStr_t mArgs; 
  for (int i = 1; i < argc; ++i) { mArgs.push_back(argv[i]); }
  lStr_i i  = mArgs.begin();
  while (i != mArgs.end()) 
  {
    lStr_i start = i;
    if (*i == "-h" || *i == "-help" || *i == "--help") 
    {
      printf(
        "Usage: %s [options]\n"
        "Options:\n"
	"  --outdir        <string>      name of ouput directory (def: %s)\n"
        "  --unblind       <bool>        unblind the data (def: %s)\n"
        "  --do-analysis   <bool>        run analysis cuts (def: %s)\n"
        "  --do-stack      <bool>        run stacking for plots (def: %s)\n"
        "  --do-comb       <bool>        run overlay for plots (def: %s)\n"
        "  --make-cards    <bool>        make data cards with 2015 c&c approach (def: %s)\n"
        "  --plotnames     <string>      list of plots to stack (def: %s)\n"
        "  --mergebkgs     <bool>        merge bkgs into categories (def: %s)\n"
        "  --doQCDrewgt    <bool>        use GJets rewgt to QCD int as QCD (def: %s)\n"
        "  --scaletodata   <bool>        scale bkg int to int of data (def: %s)\n"
        "  --yieldsplot    <string>      name of the plot from which to extract yield (def: %s)\n"
        "  --which-sel     <int>         choose which selection to apply (def: %s)\n"
        "  --do-effplots   <bool>        make efficiency plots (def: %s)\n"
        "  --do-standard   <bool>        make standard plots (def: %s)\n"
        "  --do-nminus1    <bool>        make n minus 1 plots (def: %s)\n"
	"  --do-metcor     <bool>        calculate MET-phi corr (def: %s)\n"
        "  --use-Data      <bool>        use DoubleEG data (def: %s)\n"
        "  --out-image     <string>      extension of file to save plots (def: %s)\n"
        ,
        argv[0],
        Config::outdir.Data(),
        (Config::doBlind    ? "false" : "true"),
        (Config::doAnalysis ? "true" : "false"),
        (Config::doStack    ? "true" : "false"),
        (Config::doComb     ? "true" : "false"),
        (Config::makeCards  ? "true" : "false"),
        Config::plotnames.Data(),
        (Config::mergeBkgs  ? "true" : "false"),
        (Config::doQCDrewgt ? "true" : "false"),
        (Config::scaleToData? "true" : "false"),
        Config::yieldsPlot.Data(),
        Config::whichSel,
        (Config::doEffPlots ? "true" : "false"),
        (Config::doStandard ? "true" : "false"),
        (Config::doNminus1  ? "true" : "false"),
	(Config::useData    ? "true" : "false"),
	(Config::doMETcor   ? "true" : "false"),
        Config::outtype.Data()
      );
      exit(0);
    }

    else if (*i == "--outdir")      { next_arg_or_die(mArgs, i); Config::outdir = i->c_str(); }
    else if (*i == "--unblind")     { Config::doBlind      = false; }
    else if (*i == "--do-analysis") { Config::doAnalysis   = true; }
    else if (*i == "--do-stack")    { Config::doStack      = true; }
    else if (*i == "--do-comb")     { Config::doComb       = true; }
    else if (*i == "--make-cards")  { Config::makeCards    = true; }
    else if (*i == "--plotnames")   { next_arg_or_die(mArgs, i); Config::plotnames = std::atoi(i->c_str()); }
    else if (*i == "--mergebkgs")   { Config::mergeBkgs    = true; }
    else if (*i == "--doQCDrewgt")  { Config::doQCDrewgt   = true; }
    else if (*i == "--scaletodata") { Config::scaleToData  = true; }
    else if (*i == "--yieldsplot")  { next_arg_or_die(mArgs, i); Config::yieldsPlot = std::atoi(i->c_str()); }
    else if (*i == "--which-sel")   { next_arg_or_die(mArgs, i); Config::whichSel = std::atoi(i->c_str()); }
    else if (*i == "--do-effplots") { Config::doEffPlots   = true; }
    else if (*i == "--do-standard") { Config::doStandard   = true; }
    else if (*i == "--do-nminus1")  { Config::doNminus1    = true; }
    else if (*i == "--use-Data")    { Config::useData      = true; }
    else if (*i == "--do-metcor")   { Config::doMETcor     = true; }
    else if (*i == "--out-image")   { next_arg_or_die(mArgs, i); Config::outtype  = i->c_str(); }
    else    { std::cerr << "Error: Unknown option/argument: " << i->c_str() << " ...exiting..." << std::endl; exit(1); }
    mArgs.erase(start, ++i);
  }

  //------------------------------------------------------------------------
  // Initialization
  //------------------------------------------------------------------------
  TString inDir = "/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/";
  std::ofstream yields; TStyle * tdrStyle; 
  InitializeMain(yields, tdrStyle);

  //------------------------------------------------------------------------
  // Blinding
  //------------------------------------------------------------------------
  std::string input;
  if (Config::doBlind) std::cout << "Blinding data" << std::endl;
  else
  {
    std::cout << "Unblinding data!" << std::endl;
    std::cout << "Do you want to proceed? (yn) " << std::endl;
    std::cin >> input;
    if (input == "y") std::cout << "Proceeding" << std::endl;
    else{ std::cout << "Please do not use flag: --unblind ... exiting..." << std::endl; exit(1); }
  }

  //------------------------------------------------------------------------
  // Get the MET correction
  //------------------------------------------------------------------------
  DblVec metCorrMC;
  DblVec metCorrData;

  if (Config::doMETcor)
  {
    std::cout << "Get MET-phi Correction MC" << std::endl;
    METcorr * metcorrMC = new METcorr(0,inDir,"DiPhoton");
    metCorrMC = metcorrMC->Loop(inDir, "MC");    
    delete metcorrMC;
    
    std::cout << "Get MET-phi Correction Data" << std::endl;
    METcorr * metcorrData = new METcorr(0,inDir,"DoubleEG");
    metCorrData = metcorrData->Loop(inDir, "Data");    
    delete metcorrData;    
  }
  else
  {
    std::cout << "Skipping MET-phi calculation, just pickup correction" << std::endl; 
    // pick up MC metCorr
    TString metStudyMC = Form("%smetCorr_MC.root",inDir.Data());
    TFile *fmetCorrMC = TFile::Open(metStudyMC.Data());
    CheckValidFile(fmetCorrMC,metStudyMC);
    TH1D *MCmet = (TH1D*)fmetCorrMC->Get("metCorr");  
    CheckValidTH1D(MCmet,"",metStudyMC);

    // pick up Data metCorr
    TString metStudyData = Form("%smetCorr_Data.root",inDir.Data());
    TFile *fmetCorrDATA = TFile::Open(metStudyData.Data());
    CheckValidFile(fmetCorrDATA,metStudyData);
    TH1D *DATAmet = (TH1D*)fmetCorrDATA->Get("metCorr");  
    CheckValidTH1D(DATAmet,"",metStudyData);
     
    for (UInt_t i=0; i<4; i++){
      metCorrMC.push_back(MCmet->GetBinContent(i+1));
      metCorrData.push_back(DATAmet->GetBinContent(i+1));
    }
  }

  //------------------------------------------------------------------------
  // Do analysis
  //------------------------------------------------------------------------
  if (Config::doAnalysis)
  {
    Int_t prompt = 0; //Do prompt removal
    for (TStrBoolMapIter iter = Config::SampleMap.begin(); iter != Config::SampleMap.end(); ++iter){
      std::cout << "Working on " << (*iter).first << " sample with Sel = " << Config::whichSel << std::endl;
      if ( (*iter).first=="GJets")    prompt=1;
      else if ( (*iter).first=="QCD") prompt=2;
      else prompt=0;
      // input arguments: inputdir,outputdir,sample,lumi,isData,doBlind,png/pdf,metCorr,whichSel
      Analysis * plot = new Analysis(inDir,Config::outdir,(*iter).first,Config::lumi,(*iter).second,Config::doBlind,Config::outtype,metCorrMC,Config::whichSel);
      plot->DoPlots(prompt);
      delete plot;
      std::cout << "Finished " << (*iter).first << " sample" << std::endl;
    }
    std::cout << "Finished running analysis" << std::endl;
  }
  else std::cout << "Skipping running analysis" << std::endl;  

  //------------------------------------------------------------------------
  // Make stack plots 
  //------------------------------------------------------------------------
  if (Config::doStack)  
  {
    StackPlots * stack = new StackPlots();
    stack->DoStack(yields);
    delete stack;    
  }

  //------------------------------------------------------------------------
  // Make overlay plots 
  //------------------------------------------------------------------------
  if (Config::doComb)  
  {
    OverlayPlots * comb = new OverlayPlots();
    comb->DoComb();
    delete comb;    
  }

  //------------------------------------------------------------------------
  // Make data cards with c&c approach 
  //------------------------------------------------------------------------
  if (Config::makeCards)  
  {
    CardMaker * cards = new CardMaker(inDir);
    cards->MakeCards();
    delete cards;    
  }

  DestroyMain(yields,tdrStyle);
}// end main
