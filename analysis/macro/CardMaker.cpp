#include "CardMaker.hh"
#include "../../../DataFormats/Math/interface/deltaPhi.h"

CardMaker::CardMaker(const Double_t scalefactor, const Double_t inLumi, const DblVec puweights, const TString indir, const TString outdir, const Bool_t Blind, const Bool_t onesel, const TString type){

  // Load RooFit
  gSystem->Load("libRooFit");

  // Store input variables
  alpha = scalefactor;
  fType = type;
  flumi = inLumi;
  fInDir = indir;
  fOutDir = outdir;
  doBlind = Blind;
  fPUWeights = puweights;
  useOneSel = onesel;
  fOut = "cards"; 
  if (useOneSel) fOut = "updated_cards_600sel"; 

  mainCut = "(mgg >= 100 && mgg <= 300)";// not used 

  // Make output datacard files
  MakeOutDirectory(Form("%s%s",fOutDir.Data(),fOut.Data()));


  // Set up sampleID matches
  // resonant bkgs
  Samples.push_back(SamplePair("GluGluHToGG",0)); 
  Samples.push_back(SamplePair("VBFHToGG",0)); 
  Samples.push_back(SamplePair("VHToGG",0));
  Samples.push_back(SamplePair("ttHJetToGG",0)); 
  // non-resonant bkgs
  Samples.push_back(SamplePair("DYJetsToLL",1));
  Samples.push_back(SamplePair("DiPhoton",1));
  //Samples.push_back(SamplePair("GJets",1)); 
  //Samples.push_back(SamplePair("QCD",1));
  //Samples.push_back(SamplePair("WGToLNuG",1));
  //Samples.push_back(SamplePair("ZZTo2L2Nu",1));
  //Samples.push_back(SamplePair("ZGTo2LG",1));
  //Samples.push_back(SamplePair("TTGG_0Jets",1));
  //Samples.push_back(SamplePair("TGJets",1));
  //Samples.push_back(SamplePair("TTGJets",1));
  // data 
  Samples.push_back(SamplePair("DoubleEG",2)); 

  // signals
  Samples.push_back(SamplePair("2HDM_mZP600_mA0300",100)); 
  Samples.push_back(SamplePair("2HDM_mZP800_mA0300",101)); 
  Samples.push_back(SamplePair("2HDM_mZP1000_mA0300",102));
  Samples.push_back(SamplePair("2HDM_mZP1200_mA0300",103));
  Samples.push_back(SamplePair("2HDM_mZP1400_mA0300",104));
  Samples.push_back(SamplePair("2HDM_mZP1700_mA0300",105));
  Samples.push_back(SamplePair("2HDM_mZP2000_mA0300",106)); 
  Samples.push_back(SamplePair("2HDM_mZP2500_mA0300",107));  

  Samples.push_back(SamplePair("2HDM_mZP600_mA0400",108)); 
  Samples.push_back(SamplePair("2HDM_mZP800_mA0400",109)); 
  Samples.push_back(SamplePair("2HDM_mZP1000_mA0400",110)); 
  Samples.push_back(SamplePair("2HDM_mZP1200_mA0400",111)); 
  Samples.push_back(SamplePair("2HDM_mZP1400_mA0400",112)); 
  Samples.push_back(SamplePair("2HDM_mZP1700_mA0400",113)); 
  Samples.push_back(SamplePair("2HDM_mZP2000_mA0400",114));  
  Samples.push_back(SamplePair("2HDM_mZP2500_mA0400",115));  

  //Samples.push_back(SamplePair("2HDM_mZP600_mA0500",116)); // NOT VALID  
  Samples.push_back(SamplePair("2HDM_mZP800_mA0500",117)); 
  //Samples.push_back(SamplePair("2HDM_mZP1000_mA0500",118)); 
  Samples.push_back(SamplePair("2HDM_mZP1200_mA0500",119)); 
  Samples.push_back(SamplePair("2HDM_mZP1400_mA0500",120)); 
  Samples.push_back(SamplePair("2HDM_mZP1700_mA0500",121)); 
  Samples.push_back(SamplePair("2HDM_mZP2000_mA0500",122));  
  Samples.push_back(SamplePair("2HDM_mZP2500_mA0500",123));  

  //Samples.push_back(SamplePair("2HDM_mZP600_mA0600",124)); // NOT VALID 
  Samples.push_back(SamplePair("2HDM_mZP800_mA0600",125)); 
  //Samples.push_back(SamplePair("2HDM_mZP1000_mA0600",126)); 
  Samples.push_back(SamplePair("2HDM_mZP1200_mA0600",127)); 
  Samples.push_back(SamplePair("2HDM_mZP1400_mA0600",128)); 
  Samples.push_back(SamplePair("2HDM_mZP1700_mA0600",129)); 
  Samples.push_back(SamplePair("2HDM_mZP2000_mA0600",130));  
  Samples.push_back(SamplePair("2HDM_mZP2500_mA0600",131));  

  //Samples.push_back(SamplePair("2HDM_mZP600_mA0700",132)); // NOT VALID 
  //Samples.push_back(SamplePair("2HDM_mZP800_mA0700",133)); // NOT VALID 
  //Samples.push_back(SamplePair("2HDM_mZP1000_mA0700",134)); 
  Samples.push_back(SamplePair("2HDM_mZP1200_mA0700",135)); 
  Samples.push_back(SamplePair("2HDM_mZP1400_mA0700",136)); 
  Samples.push_back(SamplePair("2HDM_mZP1700_mA0700",137)); 
  Samples.push_back(SamplePair("2HDM_mZP2000_mA0700",138));  
  Samples.push_back(SamplePair("2HDM_mZP2500_mA0700",139));  

  //Samples.push_back(SamplePair("2HDM_mZP600_mA0800",140)); // NOT VALID 
  //Samples.push_back(SamplePair("2HDM_mZP800_mA0800",141)); // NOT VALID
  Samples.push_back(SamplePair("2HDM_mZP1000_mA0800",141)); 
  Samples.push_back(SamplePair("2HDM_mZP1200_mA0800",142)); 
  Samples.push_back(SamplePair("2HDM_mZP1400_mA0800",143)); 
  Samples.push_back(SamplePair("2HDM_mZP1700_mA0800",144)); 
  Samples.push_back(SamplePair("2HDM_mZP2000_mA0800",145));  
  Samples.push_back(SamplePair("2HDM_mZP2500_mA0800",146));  


  Samples.push_back(SamplePair("BaryonicZp_mZP10000_mChi1000",150));
  Samples.push_back(SamplePair("BaryonicZp_mZP10000_mChi10"  ,151));
  Samples.push_back(SamplePair("BaryonicZp_mZP10000_mChi150" ,152));
  Samples.push_back(SamplePair("BaryonicZp_mZP10000_mChi1"   ,153));
  Samples.push_back(SamplePair("BaryonicZp_mZP10000_mChi500" ,154));
  Samples.push_back(SamplePair("BaryonicZp_mZP10000_mChi50"  ,155));
  Samples.push_back(SamplePair("BaryonicZp_mZP1000_mChi1000" ,156));
  Samples.push_back(SamplePair("BaryonicZp_mZP1000_mChi150"  ,157));
  Samples.push_back(SamplePair("BaryonicZp_mZP1000_mChi1"    ,158));
  Samples.push_back(SamplePair("BaryonicZp_mZP100_mChi10"    ,159));
  Samples.push_back(SamplePair("BaryonicZp_mZP100_mChi1"     ,160));
  Samples.push_back(SamplePair("BaryonicZp_mZP10_mChi1000"   ,161));
  Samples.push_back(SamplePair("BaryonicZp_mZP10_mChi10"     ,162));
  Samples.push_back(SamplePair("BaryonicZp_mZP10_mChi1"      ,163));
  Samples.push_back(SamplePair("BaryonicZp_mZP10_mChi50"     ,164));
  //Samples.push_back(SamplePair("BaryonicZp_mZP15_mChi10"     ,165));
  Samples.push_back(SamplePair("BaryonicZp_mZP1995_mChi1000" ,166));
  Samples.push_back(SamplePair("BaryonicZp_mZP2000_mChi1"    ,167));
  Samples.push_back(SamplePair("BaryonicZp_mZP2000_mChi500"  ,168));
  Samples.push_back(SamplePair("BaryonicZp_mZP200_mChi150"   ,169));
  Samples.push_back(SamplePair("BaryonicZp_mZP200_mChi1"     ,170));
  Samples.push_back(SamplePair("BaryonicZp_mZP200_mChi50"    ,171));
  Samples.push_back(SamplePair("BaryonicZp_mZP20_mChi1"      ,172));
  Samples.push_back(SamplePair("BaryonicZp_mZP295_mChi150"   ,173));
  Samples.push_back(SamplePair("BaryonicZp_mZP300_mChi50"    ,174));
  Samples.push_back(SamplePair("BaryonicZp_mZP500_mChi150"   ,175));
  Samples.push_back(SamplePair("BaryonicZp_mZP500_mChi1"     ,176));
  Samples.push_back(SamplePair("BaryonicZp_mZP500_mChi500"   ,177));
  Samples.push_back(SamplePair("BaryonicZp_mZP50_mChi10"     ,178));
  Samples.push_back(SamplePair("BaryonicZp_mZP50_mChi1"      ,179));
  Samples.push_back(SamplePair("BaryonicZp_mZP995_mChi500"   ,180));

  Samples.push_back(SamplePair("ScalarZp_mZP10000_mChi1"     ,181));
  Samples.push_back(SamplePair("ScalarZp_mZP10000_mChi10"    ,182));
  Samples.push_back(SamplePair("ScalarZp_mZP10000_mChi1000"  ,183));
  Samples.push_back(SamplePair("ScalarZp_mZP10000_mChi150"   ,184));
  Samples.push_back(SamplePair("ScalarZp_mZP10000_mChi50"    ,185));
  Samples.push_back(SamplePair("ScalarZp_mZP10000_mChi500"   ,186));
  Samples.push_back(SamplePair("ScalarZp_mZP1000_mChi1000"   ,210));
  Samples.push_back(SamplePair("ScalarZp_mZP1000_mChi1"      ,187));
  Samples.push_back(SamplePair("ScalarZp_mZP1000_mChi150"    ,188));
  Samples.push_back(SamplePair("ScalarZp_mZP100_mChi1"       ,189));
  Samples.push_back(SamplePair("ScalarZp_mZP100_mChi10"      ,190));
  Samples.push_back(SamplePair("ScalarZp_mZP10_mChi1"        ,191));
  Samples.push_back(SamplePair("ScalarZp_mZP10_mChi10"       ,192));
  Samples.push_back(SamplePair("ScalarZp_mZP10_mChi150"      ,211));
  Samples.push_back(SamplePair("ScalarZp_mZP10_mChi50"       ,212));
  Samples.push_back(SamplePair("ScalarZp_mZP10_mChi1000"     ,193));
  Samples.push_back(SamplePair("ScalarZp_mZP10_mChi500"      ,194));
  Samples.push_back(SamplePair("ScalarZp_mZP200_mChi1"       ,195));
  Samples.push_back(SamplePair("ScalarZp_mZP200_mChi150"     ,196));
  Samples.push_back(SamplePair("ScalarZp_mZP200_mChi50"      ,197));
  Samples.push_back(SamplePair("ScalarZp_mZP20_mChi1"        ,198));
  Samples.push_back(SamplePair("ScalarZp_mZP295_mChi150"     ,199));
  Samples.push_back(SamplePair("ScalarZp_mZP300_mChi1"       ,200));
  Samples.push_back(SamplePair("ScalarZp_mZP300_mChi50"      ,201));
  Samples.push_back(SamplePair("ScalarZp_mZP500_mChi1"       ,202));
  Samples.push_back(SamplePair("ScalarZp_mZP500_mChi150"     ,203));
  Samples.push_back(SamplePair("ScalarZp_mZP500_mChi500"     ,204));
  Samples.push_back(SamplePair("ScalarZp_mZP50_mChi1"        ,205));
  Samples.push_back(SamplePair("ScalarZp_mZP50_mChi10"       ,206));
  Samples.push_back(SamplePair("ScalarZp_mZP50_mChi50"       ,207));
  Samples.push_back(SamplePair("ScalarZp_mZP95_mChi50"       ,208));
  Samples.push_back(SamplePair("ScalarZp_mZP995_mChi500"     ,209));
  Samples.push_back(SamplePair("ScalarZp_mZP15_mChi10"       ,213));




  //Samples.push_back(SamplePair("DMHtoGG_M1",0)); 
  //Samples.push_back(SamplePair("DMHtoGG_M10",0)); 
  //Samples.push_back(SamplePair("DMHtoGG_M100",0)); 
  //Samples.push_back(SamplePair("DMHtoGG_M1000",0)); 


  // Read sample names
  for (SamplePairVecIter iter = Samples.begin(); iter != Samples.end(); ++iter){
    if ( (*iter).second == 1 )      {fBkgNames.push_back((*iter).first);}  // non-res background
    else if ( (*iter).second == 0 ) {fResNames.push_back((*iter).first);}  // resonant background
    else if ( (*iter).second == 2 ) {fDataNames.push_back((*iter).first);} // data
    else			    {fSigNames.push_back((*iter).first);}  // signal
  }

  fNData = fDataNames.size();
  fNBkg = fBkgNames.size();
  fNRes = fResNames.size();
  fNSig = fSigNames.size();
  std::cout << "fNSig = " << fNSig << std::endl;
  fNSamples = fNData+fNBkg+fNRes+fNSig;

}// end CardMaker::CardMaker

CardMaker::~CardMaker(){
  std::cout << "Finished & Deleting" << std::endl;
 
}// end CardMaker::~CardMaker

void CardMaker::MakeCards(){

  // set up vectors storing cuts to apply
  CardMaker::SetupCutsToApply();

  // set up vectors to store starting numbers (for efficiencies)
  Int_Starting.resize(fNSamples);
  Dbl_Starting.resize(fNSamples);
  for (UInt_t iter = 0; iter < fNSamples; ++iter){ 
    Int_Starting[iter] = 0;
    Dbl_Starting[iter] = 0;
  }

  // set up vectors to store results
  Int_Results_NA.resize(fNSamples);
  Int_Results_ND.resize(fNSamples); 
  Dbl_Results_NA.resize(fNSamples);
  Dbl_Results_ND.resize(fNSamples);
  Dbl_Errors.resize(fNSamples); //store errors
  Dbl_Efficiency.resize(fNSamples);//store efficiency
  Dbl_Efficiency_Errors.resize(fNSamples); //store eff. errors
  AnalyzerEff.resize(fNSamples);
  AnalyzerEffErr.resize(fNSamples);
  AnalyzerEff_unwgt.resize(fNSamples);
  AnalyzerEffErr_unwgt.resize(fNSamples);
  
  for (UInt_t iter = 0; iter < fNSamples; ++iter){ 
    Int_Results_NA[iter].resize(fNSig);
    Int_Results_ND[iter].resize(fNSig); 
    Dbl_Results_NA[iter].resize(fNSig);
    Dbl_Results_ND[iter].resize(fNSig);
    Dbl_Errors[iter].resize(fNSig);
    Dbl_Efficiency[iter].resize(fNSig);
    Dbl_Efficiency_Errors[iter].resize(fNSig);
    for (UInt_t n = 0; n < fNSig; n++){
      Int_Results_NA[iter][n] = 0;
      Int_Results_ND[iter][n] = 0; 
      Dbl_Results_NA[iter][n] = 0;
      Dbl_Results_ND[iter][n] = 0;
      Dbl_Errors[iter][n] = 0;
      Dbl_Efficiency[iter][n] = 0;
      Dbl_Efficiency_Errors[iter][n] = 0;
    } 
  }

  UInt_t SampleNumber=0;

  DblVecVec Dbl_Results_ND_Sig;
  DblVecVec Dbl_Results_ND_Res; 
  DblVecVec Dbl_Results_ND_Data;
  DblVecVec Dbl_Results_NA_Data;
  IntVec    Int_Results_ND_Data;
  IntVec    Int_Results_NA_Data;
  DblVec    Dbl_Results_NA_Bkg;
  IntVec    Int_Results_NA_Bkg;  

  DblVecVec Dbl_Errors_Sig;
  DblVecVec Dbl_Eff_Sig;
  DblVecVec Dbl_Error_Eff_Sig;

  Dbl_Results_NA_Bkg.resize(fNSig);// one entry for each cut (because one cut for each signal)
  for (UInt_t n=0; n<fNSig; n++) Dbl_Results_NA_Bkg[n]=0; 
  Int_Results_NA_Bkg.resize(fNSig);// one entry for each cut (because one cut for each signal)
  for (UInt_t n=0; n<fNSig; n++) Int_Results_NA_Bkg[n]=0; 
  Dbl_Results_ND_Res.resize(fNSig);// one entry for each cut (because one cut for each signal)

  // open the file for each sample, open the tree for that file and then 
  // apply the common selection to each sample
  for (SamplePairVecIter iter = Samples.begin(); iter != Samples.end(); ++iter){
    CardMaker::ApplyCommonSelection( (*iter).first, (*iter).second, SampleNumber);
    // regroup to be more convenient for datacard writing
    if ((*iter).second==0){
      for (UInt_t n=0; n<fNSig; n++){
	//std::cout << "ND res bkg = " << Dbl_Results_ND[SampleNumber][n] << std::endl;
        Dbl_Results_ND_Res[n].push_back(Dbl_Results_ND[SampleNumber][n]);
      }
      //Dbl_Results_ND_Res.push_back(Dbl_Results_ND[SampleNumber]);
    }
    else if ((*iter).second==2){
      Dbl_Results_ND_Data.push_back(Dbl_Results_ND[SampleNumber]);
      Dbl_Results_NA_Data.push_back(Dbl_Results_NA[SampleNumber]);
      for (UInt_t n=0; n<fNSig; n++){
        Int_Results_NA_Data.push_back(Int_Results_NA[SampleNumber][n]);
        Int_Results_ND_Data.push_back(Int_Results_ND[SampleNumber][n]);
      }
    }
    else if ((*iter).second==1){
      for (UInt_t n=0; n<fNSig; n++){ 
	//std::cout << " Dbl_NA = " << Dbl_Results_NA[SampleNumber][n] << " Int_NA = " << Int_Results_NA[SampleNumber][n] << std::endl;
        Dbl_Results_NA_Bkg[n]+=Dbl_Results_NA[SampleNumber][n];
        Int_Results_NA_Bkg[n]+=Int_Results_NA[SampleNumber][n];
	//std::cout << "FINAL Dbl_NA = " << Dbl_Results_NA_Bkg[n] << " Int_NA = " << Int_Results_NA_Bkg[n] << std::endl;
      }
    }
    else{
      Dbl_Results_ND_Sig.push_back(Dbl_Results_ND[SampleNumber]); 
      Dbl_Errors_Sig.push_back(Dbl_Errors[SampleNumber]); 
      Dbl_Eff_Sig.push_back(Dbl_Efficiency[SampleNumber]);
      Dbl_Error_Eff_Sig.push_back(Dbl_Efficiency_Errors[SampleNumber]);
    }
    SampleNumber++;
  }

  // make a histogram to store eff. values for easy plotting
  TFile* newFile = new TFile(Form("%s%s/output_eff.root",fOutDir.Data(),fOut.Data()),"RECREATE");
  std::cout << "Writing efficiency for plots in " << Form("%s%s/output_eff.root",fOutDir.Data(),fOut.Data()) << std::endl; 

  TH2D* effhisto = new TH2D("effhisto","effhisto",2300,400,2700,600,200,800); //x=Zp,y=A0,weight=eff
  Double_t mA0[6] = {300.,400.,500.,600.,700.,800.};
  Double_t mZp[8] = {600.,800.,1000.,1200.,1400.,1700.,2000.,2500.};
  Double_t binX = 0.0;
  Double_t binY = 0.0;
  Double_t wght = 0.0;
  Double_t erro = 0.0;

  TString OutputTable = Form("%s/%s/PlainEff.tex",fOutDir.Data(),fOut.Data());
  std::cout << "Writing Yield and Efficiency Table in: " << OutputTable.Data() << std::endl;
  std::ofstream fOutResults;
  fOutResults.open(OutputTable);
  if (fOutResults.is_open()){
   
     //setup Latex doc
     fOutResults << "\\documentclass[a4paper,landscape]{article}" << std::endl;
     fOutResults << "\\usepackage[paperheight=15.0in,paperwidth=6.0in,margin=1.0in,headheight=0.0in,footskip=0.5in,includehead,includefoot]{geometry}" << std::endl;
     fOutResults << "\\begin{document}" << std::endl;

     // setup table
     fOutResults << "\% Efficiencies listed for each signal" << std::endl;
     fOutResults << "\\begin{table}[bthp]" <<std::endl;
     fOutResults << "\\begin{tabular}{|l|r|}" <<std::endl;
     fOutResults << "\\hline \\hline" <<std::endl;
   
     for (UInt_t n=0; n<fNSig; n++){
       fOutResults << fSigNames[n] << " & " << Form("%1.3f",Dbl_Eff_Sig[n][0]) << " $ \\pm $ " << Form("%1.3f",Dbl_Error_Eff_Sig[n][0])  << " \\\\" << std::endl;
     } 
     fOutResults << "\\hline \\hline" <<std::endl;
     fOutResults << "\\end{tabular}" <<std::endl;
     fOutResults << "\\end{table}" <<std::endl;
     // end efficiency table

     // finish Latex doc
     fOutResults << "\\end{document}" <<std::endl;
  }
  

  for (UInt_t n=0; n<fNSig; n++){
    wght = Dbl_Eff_Sig[n][0];
    erro = Dbl_Error_Eff_Sig[n][0];

    UInt_t a0 = 0;
    if (n<8)  a0=0;	 //a0300
    else if (n<16) a0=1; //a0400
    else if (n<23) a0=2; //a0500 
    else if (n<30) a0=3; //a0600 
    else if (n<36) a0=4; //a0700 
    else a0=5; 		 //a0800 
    binY = mA0[a0];

    UInt_t zp = 0;
    if (n==0 || n==8) zp=0;					 //zp600
    if (n==1 || n==9 || n==16 || n==23) zp=1;			 //zp800
    if (n==2 || n==10 || n==17 || n==24 || n==30 || n==36) zp=2; //zp1000
    if (n==3 || n==11 || n==18 || n==25 || n==31 || n==37) zp=3; //zp1200
    if (n==4 || n==12 || n==19 || n==26 || n==32 || n==38) zp=4; //zp1400
    if (n==5 || n==13 || n==20 || n==27 || n==33 || n==39) zp=5; //zp1700
    if (n==6 || n==14 || n==21 || n==28 || n==34 || n==40) zp=6; //zp2000
    if (n==7 || n==15 || n==22 || n==29 || n==35 || n==41) zp=7; //zp2500
    binX = mZp[zp];

    UInt_t numbinx = effhisto->GetXaxis()->FindBin(binX);
    UInt_t numbiny = effhisto->GetYaxis()->FindBin(binY);
    effhisto->SetBinContent(numbinx,numbiny,wght);
    effhisto->SetBinError(numbinx,numbiny,erro);
  }

  newFile->cd();
  TCanvas* c = new TCanvas();
  c->cd();
  effhisto->Write();

  // Write out signal yield and efficiency tables
  //CardMaker::MakeYieldAndEfficiencyTables( Dbl_Results_ND_Sig , Dbl_Errors_Sig, Dbl_Eff_Sig, Dbl_Error_Eff_Sig);

  // Write out the datacard (one for each signal sample)
  for (UInt_t mc=0; mc < fNSig; mc++){
    // mass600 selection for each signal:
    if (useOneSel) CardMaker::WriteDataCard( fSigNames[mc] , Dbl_Results_ND_Sig[mc][0], Int_Results_NA_Data[0], Int_Results_ND_Data[0], Dbl_Results_ND_Res[0]);
    // optimal selection for each signal:
    else CardMaker::WriteDataCard( fSigNames[mc] , Dbl_Results_ND_Sig[mc][mc], Int_Results_NA_Data[mc], Int_Results_ND_Data[mc], Dbl_Results_ND_Res[mc]);


  } 

}// end CardMaker::MakeCards

void CardMaker::ApplyCommonSelection( const TString fSample, const UInt_t sampleID , const UInt_t sampleNumber){

  // open each file
  TFile *fileOrig = 0;
  TString fileName = Form("%s%s.root",fInDir.Data(),fSample.Data());
  fileOrig = TFile::Open(fileName.Data());
  CheckValidFile(fileOrig,fileName);

  // open the tree
  TTree *treeOrig = 0;
  fileOrig->cd();
  TString treeName = "DiPhotonTree";
  treeOrig = (TTree*)fileOrig->Get(treeName);
  CheckValidTree(treeOrig,treeName,fileName); 

  // open h_selection
  TH1D *fSel = (TH1D*)fileOrig->Get("h_selection");
  TH1D *fSel_unwgt = (TH1D*)fileOrig->Get("h_selection_unwgt");
  CheckValidTH1D(fSel,"h_selection",fileName);
  CheckValidTH1D(fSel_unwgt,"h_selection_unwgt",fileName);
 
  // store bin content for h_selection bins in vectors
  std::vector<Double_t> fSelection;
  std::vector<Double_t> fSelection_unwgt;
  for (UInt_t i=0; i<8; i++){
    fSelection.push_back(fSel->GetBinContent(i+1));
    fSelection_unwgt.push_back(fSel_unwgt->GetBinContent(i+1));
  }

  // get efficiency to pass photon ID applied in analyzer
  AnalyzerEff[sampleNumber]		= fSelection[7]/fSelection[0];
  AnalyzerEffErr[sampleNumber]		= TMath::Sqrt(AnalyzerEff[sampleNumber]*(1-AnalyzerEff[sampleNumber])/fSelection[0]); 
  AnalyzerEff_unwgt[sampleNumber]	= fSelection_unwgt[7]/fSelection_unwgt[0];
  AnalyzerEffErr_unwgt[sampleNumber]	= TMath::Sqrt(AnalyzerEff_unwgt[sampleNumber]*(1-AnalyzerEff_unwgt[sampleNumber])/fSelection_unwgt[0]); 

  std::cout << "Analyzer Eff         = " << AnalyzerEff[sampleNumber] << " \\pm " << AnalyzerEffErr[sampleNumber] << std::endl;
  std::cout << "Analyzer Eff (unwgt) = " << AnalyzerEff_unwgt[sampleNumber] << " \\pm " << AnalyzerEffErr_unwgt[sampleNumber] << std::endl;

  // Initialize variables for the tree
  CardMaker::SetBranchAddresses( treeOrig );

  std::cout << "Pickup MET Phi Correction" << std::endl; 
  std::vector< Double_t > fMETCorrMC;
  std::vector< Double_t > fMETCorrData;
  // pick up MC metCorr
  TString metStudyMC = Form("%s/metCorr_MC.root",fInDir.Data());
  TFile *fmetCorrMC = TFile::Open(metStudyMC.Data());
  TH1D *MCmet = (TH1D*)fmetCorrMC->Get("metCorr");  
  // pick up Data metCorr
  TString metStudyData = Form("%s/metCorr_Data.root",fInDir.Data());
  TFile *fmetCorrDATA = TFile::Open(metStudyData.Data());
  TH1D *DATAmet = (TH1D*)fmetCorrDATA->Get("metCorr");  
  for (UInt_t i=0; i<4; i++){
    fMETCorrMC.push_back(MCmet->GetBinContent(i+1));
    fMETCorrData.push_back(DATAmet->GetBinContent(i+1));
  }

  TLorentzVector fLorenzVecPho1;
  TLorentzVector fLorenzVecPho2;
  TLorentzVector fLorenzVecGG;
  TLorentzVector fLorenzVecJet1;
  TLorentzVector fLorenzVecJet2;
  TLorentzVector fLorenzVecJet3;
  TLorentzVector fLorenzVecJet4;
  TLorentzVector correctedMet;

  // loop over tree entries and apply selection
  UInt_t nentriesOrig = treeOrig->GetEntries();
  for (UInt_t i=0; i<nentriesOrig; i++){
    treeOrig->GetEntry(i);

    // fill the Lorentz Vectors
    fLorenzVecPho1.SetPtEtaPhiM(pt1,eta1,phi1,0.);
    fLorenzVecPho2.SetPtEtaPhiM(pt2,eta2,phi2,0.);
    fLorenzVecGG = fLorenzVecPho1 + fLorenzVecPho2;
    fLorenzVecJet1.SetPtEtaPhiM(ptJetLead,etaJetLead,phiJetLead,massJetLead);
    fLorenzVecJet2.SetPtEtaPhiM(ptJetSubLead,etaJetSubLead,phiJetSubLead,massJetSubLead);
    fLorenzVecJet3.SetPtEtaPhiM(ptJet3,etaJet3,phiJet3,massJet3);
    fLorenzVecJet4.SetPtEtaPhiM(ptJet4,etaJet4,phiJet4,massJet4);

    //// met-phi correction
    //Bool_t doMETphiCorr = true;
    //Double_t t1pfmetCorrX, t1pfmetCorrY, t1pfmetCorrE, t1pfmetCorr, t1pfmetCorrPhi;

    //if (sampleID==2){// data 
    //  t1pfmetCorrX = t1pfmet*cos(t1pfmetPhi) - (fMETCorrData[0] + fMETCorrData[1]*t1pfmetSumEt);
    //  t1pfmetCorrY = t1pfmet*sin(t1pfmetPhi) - (fMETCorrData[2] + fMETCorrData[3]*t1pfmetSumEt);
    //}
    //else{// mc
    //  t1pfmetCorrX = t1pfmet*cos(t1pfmetPhi) - (fMETCorrMC[0] + fMETCorrMC[1]*t1pfmetSumEt);
    //  t1pfmetCorrY = t1pfmet*sin(t1pfmetPhi) - (fMETCorrMC[2] + fMETCorrMC[3]*t1pfmetSumEt);
    //}
    //t1pfmetCorrE = sqrt(t1pfmetCorrX*t1pfmetCorrX + t1pfmetCorrY*t1pfmetCorrY);
    //correctedMet.SetPxPyPzE(t1pfmetCorrX,t1pfmetCorrY,0,t1pfmetCorrE);
    ////if (doMETphiCorr){
    ////  t1pfmetCorr = correctedMet.Pt();
    ////  t1pfmetCorrPhi = correctedMet.Phi();
    ////}
    //if (!doMETphiCorr){ //just take original met
    //  t1pfmetCorr    = t1pfmet; 
    //  t1pfmetCorrPhi = t1pfmetPhi;
    //}

    // calculate the weight
    Double_t Weight = 1.0;
    if (sampleID==2) Weight = 1; // no weight for data
    else if (sampleID==0 || sampleID==1) Weight = (weight)*fPUWeights[nvtx];// PURW[0] corresponds to bin1=0vtx
    else Weight = (weight)*fPUWeights[nvtx];//weighted to xsec*BR=1pb*BR

    // check that samples passes METfilters
    Bool_t passMetFil_All   = (metF_GV && metF_globalTightHalo && metF_HBHENoise && metF_HBHENoiseIso && metF_EcalDeadCell && metF_badMuon && metF_badChargedHadron);
    Bool_t passMetFil_Data  = (sampleID==2)?(metF_eeBadSC):true; // filter for data only
    Bool_t passMETfil       = (passMetFil_All && passMetFil_Data);
    if (!passMETfil) continue;
 
    // check that passes trigger
    if (sampleID==2 && hltDiphoton30Mass95==0) continue;

    // remove duplicate events
    if (fSample=="GJets" && (genmatch1==1 && genmatch2==1)) continue;   // only PF and FF for gjets  
    if (fSample=="QCD" && (genmatch1==1 && genmatch2==1)) continue;   // only PF and FF for gjets  

    Int_Starting[sampleNumber]++;
    Dbl_Starting[sampleNumber]+=Weight;

    // check if passing deltaPhi(gg,MET) cut
    Double_t dphi_ggMET = TMath::Abs(deltaPhi(fLorenzVecGG.Phi(),t1pfmetCorrPhi));

    // look at deltaPhi(jet,MET)
    Double_t min_dphi_JetMET = 10.;
    Double_t max_dphi_JetMET = 0.;

    if (nJets50 > 0){
      Double_t dphiJet1METmin = 10;
      Double_t dphiJet2METmin = 10;
      Double_t dphiJet3METmin = 10;
      Double_t dphiJet4METmin = 10;
      Double_t dphiJet1METmax = 0;
      Double_t dphiJet2METmax = 0;
      Double_t dphiJet3METmax = 0;
      Double_t dphiJet4METmax = 0;
      if ( ptJetLead > 50 ){
        dphiJet1METmin = TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetCorrPhi));
        dphiJet1METmax = TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetCorrPhi));
      }
      if ( ptJetSubLead > 50 ){
        dphiJet2METmin = TMath::Abs(deltaPhi(fLorenzVecJet2.Phi(),t1pfmetCorrPhi));
        dphiJet2METmax = TMath::Abs(deltaPhi(fLorenzVecJet2.Phi(),t1pfmetCorrPhi));
      }
      if ( ptJet3 > 50 ){
        dphiJet3METmin = TMath::Abs(deltaPhi(fLorenzVecJet3.Phi(),t1pfmetCorrPhi));
        dphiJet3METmax = TMath::Abs(deltaPhi(fLorenzVecJet3.Phi(),t1pfmetCorrPhi));
      }
      if ( ptJet4 > 50 ){
        dphiJet4METmin = TMath::Abs(deltaPhi(fLorenzVecJet4.Phi(),t1pfmetCorrPhi));
        dphiJet4METmax = TMath::Abs(deltaPhi(fLorenzVecJet4.Phi(),t1pfmetCorrPhi));
      }

      // find the min_dphi_JetMET 
      if (dphiJet1METmin < min_dphi_JetMET) min_dphi_JetMET = dphiJet1METmin;	   
      if (dphiJet2METmin < min_dphi_JetMET) min_dphi_JetMET = dphiJet2METmin;	   
      if (dphiJet3METmin < min_dphi_JetMET) min_dphi_JetMET = dphiJet3METmin;	   
      if (dphiJet4METmin < min_dphi_JetMET) min_dphi_JetMET = dphiJet4METmin;	   

      // find the max_dphi_JetMET 
      if (dphiJet1METmax > max_dphi_JetMET) max_dphi_JetMET = dphiJet1METmax;	   
      if (dphiJet2METmax > max_dphi_JetMET) max_dphi_JetMET = dphiJet2METmax;	   
      if (dphiJet3METmax > max_dphi_JetMET) max_dphi_JetMET = dphiJet3METmax;	   
      if (dphiJet4METmax > max_dphi_JetMET) max_dphi_JetMET = dphiJet4METmax;	   
    }

    if (dphi_ggMET < 2.1) continue;

    if (min_dphi_JetMET < 0.5) continue; 

    if (nMuons > 0 || nEle > 0) continue;

    if (nJets30 > 2) continue;

    if (mgg < 105 || mgg > 180) continue;
  
    // any events that make it to this point have passed all the common selection
    for (UInt_t cut=0; cut < fNSig; cut++){
      if (pt1/mgg > Cut_pt1mgg[cut] && pt2/mgg > Cut_pt2mgg[cut] && ptgg > Cut_ptgg[cut]){
        if (t1pfmetCorr > Cut_met[cut]){
	  if (mgg > 115 && mgg < 135){
	  //if (mgg > 120 && mgg < 130){
	    Int_Results_ND[sampleNumber][cut]++;
            Dbl_Results_ND[sampleNumber][cut]+=Weight;
	    Dbl_Errors[sampleNumber][cut]+=Weight*Weight;// part1 of err calc.
	  }
	  else{
	    Int_Results_NA[sampleNumber][cut]++;
            Dbl_Results_NA[sampleNumber][cut]+=Weight;
	  }
	}// met cut
      }// pt1/mgg, pt2/mgg, ptgg cuts
    }// loop over the cuts (one for each signal sample)

  }// loop over entries in treeOrig

  for (UInt_t cut=0; cut < fNSig; cut++){
    Dbl_Errors[sampleNumber][cut] = TMath::Sqrt(Dbl_Errors[sampleNumber][cut]);// part2 of err calc.
    // for efficiency after analyzer
    //Dbl_Efficiency[sampleNumber][cut] = (Double_t)Int_Results_ND[sampleNumber][cut]/(Double_t)Int_Starting[sampleNumber];
    //Dbl_Efficiency_Errors[sampleNumber][cut] = TMath::Sqrt(Dbl_Efficiency[sampleNumber][cut]*(1.0-Dbl_Efficiency[sampleNumber][cut])/(Double_t)Int_Starting[sampleNumber]);
    // for efficiency including analyzer eff 
    Dbl_Efficiency[sampleNumber][cut] = AnalyzerEff_unwgt[sampleNumber]*((Double_t)Int_Results_ND[sampleNumber][cut]/(Double_t)Int_Starting[sampleNumber]); 
    Dbl_Efficiency_Errors[sampleNumber][cut] = TMath::Sqrt(AnalyzerEffErr_unwgt[sampleNumber]*AnalyzerEffErr_unwgt[sampleNumber]+(Dbl_Efficiency[sampleNumber][cut]*(1.0-Dbl_Efficiency[sampleNumber][cut])/(Double_t)Int_Starting[sampleNumber])); 
  }
  
  for (UInt_t cut=0; cut < fNSig; cut++){
    //if (sampleID!=2 || (sampleID==2 && !doBlind)) std::cout << "Number of events in signal region = " << Dbl_Results_ND[sampleNumber][cut] << std::endl;
    //if (sampleID==1) std::cout << "(Int) Number of events in sideband region = " << Int_Results_NA[sampleNumber][cut] << std::endl;
    //if (sampleID==1) std::cout << "(Dbl) Number of events in sideband region = " << Dbl_Results_NA[sampleNumber][cut] << std::endl;
  }

  delete treeOrig;
  delete fileOrig;

}// end CardMaker::ApplyCommonSelection


void CardMaker::MakeYieldAndEfficiencyTables( const DblVecVec ND_Sig, const DblVecVec Err_ND_Sig, const DblVecVec Eff_Sig, const DblVecVec Err_Eff_Sig){

  TString OutputTableName = Form("%s/%s/SignalYieldAndEffGrid.tex",fOutDir.Data(),fOut.Data());
  std::cout << "Writing Yield and Efficiency Table in: " << OutputTableName.Data() << std::endl;
  fOutResultsGrid.open(OutputTableName);
  if (fOutResultsGrid.is_open()){
   
     //setup Latex doc
     fOutResultsGrid << "\\documentclass[a4paper,landscape]{article}" << std::endl;
     fOutResultsGrid << "\\usepackage[paperheight=15.0in,paperwidth=6.0in,margin=1.0in,headheight=0.0in,footskip=0.5in,includehead,includefoot]{geometry}" << std::endl;
     fOutResultsGrid << "\\begin{document}" << std::endl;

     // setup yields table
     fOutResultsGrid << "\% Summary of Events in Signal Region for each sample" << std::endl;
     fOutResultsGrid << "\\begin{table}[bthp]" <<std::endl;
     fOutResultsGrid << "\\begin{tabular}{|c|cccccccc|}" <<std::endl;
     fOutResultsGrid << "\\hline \\hline" <<std::endl;
     //fOutResultsGrid << Form("$\\sqrt{s}$ = 13 TeV; L = %1.1f $fb^{-1}$",flumi) <<" \\\\" <<std::endl;
     //fOutResultsGrid << "\\hline" <<std::endl;
     fOutResultsGrid << " $m_{Z'}$ & 600 	& 800 	& 1000 	& 1200 	& 1400 	& 1700	& 2000	& 2500  \\\\" << std::endl; 
     fOutResultsGrid << " $m_{A^0}$ & & & & & & & & \\\\" << std::endl;  
     fOutResultsGrid << "\\hline \\hline" << std::endl; 

     UInt_t sizeA0 = 6;
     UInt_t sizeZP = 8;
     UInt_t mass_A0[sizeA0] = {300,400,500,600,700,800};
     UInt_t mass_ZP[sizeZP] = {600,800,1000,1200,1400,1700,2000,2500};
     DblVecVec xsec;
     Double_t tempxsec = 1.0;
     xsec.resize(sizeA0);
     for (UInt_t mA0 = 0; mA0 < sizeA0; mA0++){
       xsec[mA0].resize(sizeZP);
       for (UInt_t mZp = 0; mZp < sizeZP; mZp++){
         xsec[mA0][mZp] = CardMaker::GetXsec(mass_A0[mA0],mass_ZP[mZp]);
       }
     } 

     if (useOneSel){
       fOutResultsGrid << " 300 & " << Form("%1.3f",xsec[0][0]*ND_Sig[0][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][0]*Err_ND_Sig[0][0]) 
       		     << " & " 	    << Form("%1.3f",xsec[0][1]*ND_Sig[1][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][1]*Err_ND_Sig[1][0]) 
          	     << " & "       << Form("%1.3f",xsec[0][2]*ND_Sig[2][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][2]*Err_ND_Sig[2][0])  
          	     << " & "       << Form("%1.3f",xsec[0][3]*ND_Sig[3][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][3]*Err_ND_Sig[3][0])  
          	     << " & "       << Form("%1.3f",xsec[0][4]*ND_Sig[4][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][4]*Err_ND_Sig[4][0])  
          	     << " & "       << Form("%1.3f",xsec[0][5]*ND_Sig[5][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][5]*Err_ND_Sig[5][0])  
          	     << " & "       << Form("%1.3f",xsec[0][6]*ND_Sig[6][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][6]*Err_ND_Sig[6][0])  
          	     << " & "       << Form("%1.3f",xsec[0][7]*ND_Sig[7][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][7]*Err_ND_Sig[7][0])  
          	     << " \\\\" << std::endl; 
       fOutResultsGrid << " 400 & " << Form("%1.3f",xsec[1][0]*ND_Sig[8][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][0]*Err_ND_Sig[8][0]) 
       		     << " & " 	    << Form("%1.3f",xsec[1][1]*ND_Sig[9][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][1]*Err_ND_Sig[9][0]) 
          	     << " & "       << Form("%1.3f",xsec[1][2]*ND_Sig[10][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][2]*Err_ND_Sig[10][0])  
          	     << " & "       << Form("%1.3f",xsec[1][3]*ND_Sig[11][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][3]*Err_ND_Sig[11][0])  
          	     << " & "       << Form("%1.3f",xsec[1][4]*ND_Sig[12][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][4]*Err_ND_Sig[12][0])  
          	     << " & "       << Form("%1.3f",xsec[1][5]*ND_Sig[13][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][5]*Err_ND_Sig[13][0])  
          	     << " & "       << Form("%1.3f",xsec[1][6]*ND_Sig[14][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][6]*Err_ND_Sig[14][0])  
          	     << " & "       << Form("%1.3f",xsec[1][7]*ND_Sig[15][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][7]*Err_ND_Sig[15][0])  
          	     << " \\\\" << std::endl; 
       fOutResultsGrid << " 500 & " << " - "  
       		     << " & " 	    << Form("%1.3f",xsec[2][1]*ND_Sig[16][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][1]*Err_ND_Sig[16][0]) 
          	     << " & "       << Form("%1.3f",xsec[2][2]*ND_Sig[17][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][2]*Err_ND_Sig[17][0])  
          	     << " & "       << Form("%1.3f",xsec[2][3]*ND_Sig[18][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][3]*Err_ND_Sig[18][0])  
          	     << " & "       << Form("%1.3f",xsec[2][4]*ND_Sig[19][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][4]*Err_ND_Sig[19][0])  
          	     << " & "       << Form("%1.3f",xsec[2][5]*ND_Sig[20][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][5]*Err_ND_Sig[20][0])  
          	     << " & "       << Form("%1.3f",xsec[2][6]*ND_Sig[21][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][6]*Err_ND_Sig[21][0])  
          	     << " & "       << Form("%1.3f",xsec[2][7]*ND_Sig[22][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][7]*Err_ND_Sig[22][0])  
          	     << " \\\\" << std::endl;                                                         
       fOutResultsGrid << " 600 & " << " - "  
       		     << " & " 	    << Form("%1.3f",xsec[3][1]*ND_Sig[23][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][1]*Err_ND_Sig[23][0]) 
          	     << " & "       << Form("%1.3f",xsec[3][2]*ND_Sig[24][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][2]*Err_ND_Sig[24][0])  
          	     << " & "       << Form("%1.3f",xsec[3][3]*ND_Sig[25][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][3]*Err_ND_Sig[25][0])  
          	     << " & "       << Form("%1.3f",xsec[3][4]*ND_Sig[26][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][4]*Err_ND_Sig[26][0])  
          	     << " & "       << Form("%1.3f",xsec[3][5]*ND_Sig[27][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][5]*Err_ND_Sig[27][0])  
          	     << " & "       << Form("%1.3f",xsec[3][6]*ND_Sig[28][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][6]*Err_ND_Sig[28][0])  
          	     << " & "       << Form("%1.3f",xsec[3][7]*ND_Sig[29][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][7]*Err_ND_Sig[29][0])  
          	     << " \\\\" << std::endl; 
       fOutResultsGrid << " 700 & " << " - "  
       		     << " & " 	    << " - " 
          	     << " & "       << Form("%1.3f",xsec[4][2]*ND_Sig[30][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[4][2]*Err_ND_Sig[30][0])  
          	     << " & "       << Form("%1.3f",xsec[4][3]*ND_Sig[31][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[4][3]*Err_ND_Sig[31][0])  
          	     << " & "       << Form("%1.3f",xsec[4][4]*ND_Sig[32][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[4][4]*Err_ND_Sig[32][0])  
          	     << " & "       << Form("%1.3f",xsec[4][5]*ND_Sig[33][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[4][5]*Err_ND_Sig[33][0])  
          	     << " & "       << Form("%1.3f",xsec[4][6]*ND_Sig[34][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[4][6]*Err_ND_Sig[34][0])  
          	     << " & "       << Form("%1.3f",xsec[4][7]*ND_Sig[35][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[4][7]*Err_ND_Sig[35][0])  
          	     << " \\\\" << std::endl; 
       fOutResultsGrid << " 800 & " << " - "  
       		     << " & " 	    << " - "
          	     << " & "       << Form("%1.3f",xsec[5][2]*ND_Sig[36][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[5][2]*Err_ND_Sig[36][0])  
          	     << " & "       << Form("%1.3f",xsec[5][3]*ND_Sig[37][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[5][3]*Err_ND_Sig[37][0])  
          	     << " & "       << Form("%1.3f",xsec[5][4]*ND_Sig[38][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[5][4]*Err_ND_Sig[38][0])  
          	     << " & "       << Form("%1.3f",xsec[5][5]*ND_Sig[39][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[5][5]*Err_ND_Sig[39][0])  
          	     << " & "       << Form("%1.3f",xsec[5][6]*ND_Sig[40][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[5][6]*Err_ND_Sig[40][0])  
          	     << " & "       << Form("%1.3f",xsec[5][7]*ND_Sig[41][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[5][7]*Err_ND_Sig[41][0])  
          	     << " \\\\" << std::endl; 
     }
     else{// not useOneSel
       fOutResultsGrid << " 300 & " << Form("%1.3f",xsec[0][0]*ND_Sig[0][0]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][0]*Err_ND_Sig[0][0]) 
       		     << " & " 	    << Form("%1.3f",xsec[0][1]*ND_Sig[1][1]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][1]*Err_ND_Sig[1][1]) 
          	     << " & "       << Form("%1.3f",xsec[0][2]*ND_Sig[2][2]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][2]*Err_ND_Sig[2][2])  
          	     << " & "       << Form("%1.3f",xsec[0][3]*ND_Sig[3][3]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][3]*Err_ND_Sig[3][3])  
          	     << " & "       << Form("%1.3f",xsec[0][4]*ND_Sig[4][4]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][4]*Err_ND_Sig[4][4])  
          	     << " & "       << Form("%1.3f",xsec[0][5]*ND_Sig[5][5]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][5]*Err_ND_Sig[5][5])  
          	     << " & "       << Form("%1.3f",xsec[0][6]*ND_Sig[6][6]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][6]*Err_ND_Sig[6][6])  
          	     << " & "       << Form("%1.3f",xsec[0][7]*ND_Sig[7][7]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[0][7]*Err_ND_Sig[7][7])  
          	     << " \\\\" << std::endl;                                                                             
       fOutResultsGrid << " 400 & " << Form("%1.3f",xsec[1][0]*ND_Sig[8][8]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][0]*Err_ND_Sig[8][8]) 
       		     << " & " 	    << Form("%1.3f",xsec[1][1]*ND_Sig[9][9]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][1]*Err_ND_Sig[9][9]) 
          	     << " & "       << Form("%1.3f",xsec[1][2]*ND_Sig[10][10]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][2]*Err_ND_Sig[10][10])  
          	     << " & "       << Form("%1.3f",xsec[1][3]*ND_Sig[11][11]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][3]*Err_ND_Sig[11][11])  
          	     << " & "       << Form("%1.3f",xsec[1][4]*ND_Sig[12][12]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][4]*Err_ND_Sig[12][12])  
          	     << " & "       << Form("%1.3f",xsec[1][5]*ND_Sig[13][13]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][5]*Err_ND_Sig[13][13])  
          	     << " & "       << Form("%1.3f",xsec[1][6]*ND_Sig[14][14]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][6]*Err_ND_Sig[14][14])  
          	     << " & "       << Form("%1.3f",xsec[1][7]*ND_Sig[15][15]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[1][7]*Err_ND_Sig[15][15])  
          	     << " \\\\" << std::endl;                                                                             
       fOutResultsGrid << " 500 & " << " - "                                                                              
       		     << " & " 	    << Form("%1.3f",xsec[2][1]*ND_Sig[16][16]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][1]*Err_ND_Sig[16][16]) 
          	     << " & "       << Form("%1.3f",xsec[2][2]*ND_Sig[17][17]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][2]*Err_ND_Sig[17][17])  
          	     << " & "       << Form("%1.3f",xsec[2][3]*ND_Sig[18][18]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][3]*Err_ND_Sig[18][18])  
          	     << " & "       << Form("%1.3f",xsec[2][4]*ND_Sig[19][19]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][4]*Err_ND_Sig[19][19])  
          	     << " & "       << Form("%1.3f",xsec[2][5]*ND_Sig[20][20]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][5]*Err_ND_Sig[20][20])  
          	     << " & "       << Form("%1.3f",xsec[2][6]*ND_Sig[21][21]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][6]*Err_ND_Sig[21][21])  
          	     << " & "       << Form("%1.3f",xsec[2][7]*ND_Sig[22][22]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[2][7]*Err_ND_Sig[22][22])  
          	     << " \\\\" << std::endl;                                                                             
       fOutResultsGrid << " 600 & " << " - "                                                                              
       		     << " & " 	    << Form("%1.3f",xsec[3][1]*ND_Sig[23][23]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][1]*Err_ND_Sig[23][23]) 
          	     << " & "       << Form("%1.3f",xsec[3][2]*ND_Sig[24][24]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][2]*Err_ND_Sig[24][24])  
          	     << " & "       << Form("%1.3f",xsec[3][3]*ND_Sig[25][25]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][3]*Err_ND_Sig[25][25])  
          	     << " & "       << Form("%1.3f",xsec[3][4]*ND_Sig[26][26]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][4]*Err_ND_Sig[26][26])  
          	     << " & "       << Form("%1.3f",xsec[3][5]*ND_Sig[27][27]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][5]*Err_ND_Sig[27][27])  
          	     << " & "       << Form("%1.3f",xsec[3][6]*ND_Sig[28][28]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][6]*Err_ND_Sig[28][28])  
          	     << " & "       << Form("%1.3f",xsec[3][7]*ND_Sig[29][29]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[3][7]*Err_ND_Sig[29][29])  
          	     << " \\\\" << std::endl;                                                                             
       fOutResultsGrid << " 700 & " << " - "                                                                              
       		     << " & " 	    << " - "                                                                              
          	     << " & "       << Form("%1.3f",xsec[4][2]*ND_Sig[30][30]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[4][2]*Err_ND_Sig[30][30])  
          	     << " & "       << Form("%1.3f",xsec[4][3]*ND_Sig[31][31]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[4][3]*Err_ND_Sig[31][31])  
          	     << " & "       << Form("%1.3f",xsec[4][4]*ND_Sig[32][32]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[4][4]*Err_ND_Sig[32][32])  
          	     << " & "       << Form("%1.3f",xsec[4][5]*ND_Sig[33][33]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[4][5]*Err_ND_Sig[33][33])  
          	     << " & "       << Form("%1.3f",xsec[4][6]*ND_Sig[34][34]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[4][6]*Err_ND_Sig[34][34])  
          	     << " & "       << Form("%1.3f",xsec[4][7]*ND_Sig[35][35]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[4][7]*Err_ND_Sig[35][35])  
          	     << " \\\\" << std::endl;                                                                             
       fOutResultsGrid << " 800 & " << " - "                                                                              
       		     << " & " 	    << " - "                                                                              
          	     << " & "       << Form("%1.3f",xsec[5][2]*ND_Sig[36][36]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[5][2]*Err_ND_Sig[36][36])  
          	     << " & "       << Form("%1.3f",xsec[5][3]*ND_Sig[37][37]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[5][3]*Err_ND_Sig[37][37])  
          	     << " & "       << Form("%1.3f",xsec[5][4]*ND_Sig[38][38]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[5][4]*Err_ND_Sig[38][38])  
          	     << " & "       << Form("%1.3f",xsec[5][5]*ND_Sig[39][39]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[5][5]*Err_ND_Sig[39][39])  
          	     << " & "       << Form("%1.3f",xsec[5][6]*ND_Sig[40][40]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[5][6]*Err_ND_Sig[40][40])  
          	     << " & "       << Form("%1.3f",xsec[5][7]*ND_Sig[41][41]) 	<< " $ \\pm $ " << Form("%1.4f",xsec[5][7]*Err_ND_Sig[41][41])  
          	     << " \\\\" << std::endl; 
     }//table with fully optimized sel 

     fOutResultsGrid << "\\hline \\hline" <<std::endl;
     fOutResultsGrid << "\\end{tabular}" <<std::endl;
     fOutResultsGrid << "\\end{table}" <<std::endl;
     // end yield table


     // setup efficiency table
     fOutResultsGrid << "\% Summary of Signal Efficiencies" << std::endl;
     fOutResultsGrid << "\\begin{table}[bthp]" <<std::endl;
     fOutResultsGrid << "\\begin{tabular}{|c|cccccccc|}" <<std::endl;
     fOutResultsGrid << "\\hline \\hline" <<std::endl;
     //fOutResultsGrid << Form("$\\sqrt{s}$ = 13 TeV; L = %1.1f $fb^{-1}$",flumi) <<" \\\\" <<std::endl;
     //fOutResultsGrid << "\\hline" <<std::endl;
     fOutResultsGrid << " $m_{Z'}$ & 600 	& 800 	& 1000 	& 1200 	& 1400 	& 1700	& 2000	& 2500  \\\\" << std::endl; 
     fOutResultsGrid << " $m_{A^0}$ & & & & & & & & \\\\" << std::endl;  
     fOutResultsGrid << "\\hline \\hline" <<std::endl;

     if (useOneSel){
       fOutResultsGrid << " 300 & " << Form("%1.3f",Eff_Sig[0][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[0][0]) 
  		     << " & "       << Form("%1.3f",Eff_Sig[1][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[1][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[2][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[2][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[3][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[3][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[4][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[4][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[5][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[5][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[6][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[6][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[7][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[7][0])  
  		     << " \\\\" << std::endl; 
       fOutResultsGrid << " 400 & " << Form("%1.3f",Eff_Sig[8][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[8][0]) 
  		     << " & "       << Form("%1.3f",Eff_Sig[9][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[9][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[10][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[10][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[11][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[11][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[12][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[12][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[13][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[13][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[14][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[14][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[15][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[15][0])  
  		     << " \\\\" << std::endl; 
       fOutResultsGrid << " 500 & " << " - "  
  		     << " & "       << Form("%1.3f",Eff_Sig[16][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[16][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[17][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[17][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[18][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[18][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[19][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[19][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[20][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[20][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[21][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[21][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[22][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[22][0])  
  		     << " \\\\" << std::endl; 
       fOutResultsGrid << " 600 & " << " - " 
  		     << " & "       << Form("%1.3f",Eff_Sig[23][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[23][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[24][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[24][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[25][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[25][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[26][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[26][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[27][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[27][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[28][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[28][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[29][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[29][0])  
  		     << " \\\\" << std::endl; 
       fOutResultsGrid << " 700 & " << " - "  
  		     << " & "       << " - " 
  		     << " & "       << Form("%1.3f",Eff_Sig[30][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[30][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[31][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[31][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[32][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[32][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[33][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[33][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[34][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[34][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[35][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[35][0])  
  		     << " \\\\" << std::endl; 
       fOutResultsGrid << " 800 & " << " - "  
  		     << " & "       << " - "
  		     << " & "       << Form("%1.3f",Eff_Sig[36][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[36][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[37][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[37][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[38][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[38][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[39][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[39][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[40][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[40][0])  
  		     << " & "       << Form("%1.3f",Eff_Sig[41][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[41][0])  
  		     << " \\\\" << std::endl; 
     }

     else{ // not useOneSel
       fOutResultsGrid << " 300 & " << Form("%1.3f",Eff_Sig[0][0]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[0][0]) 
  		     << " & "       << Form("%1.3f",Eff_Sig[1][1]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[1][1])  
  		     << " & "       << Form("%1.3f",Eff_Sig[2][2]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[2][2])  
  		     << " & "       << Form("%1.3f",Eff_Sig[3][3]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[3][3])  
  		     << " & "       << Form("%1.3f",Eff_Sig[4][4]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[4][4])  
  		     << " & "       << Form("%1.3f",Eff_Sig[5][5]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[5][5])  
  		     << " & "       << Form("%1.3f",Eff_Sig[6][6]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[6][6])  
  		     << " & "       << Form("%1.3f",Eff_Sig[7][7]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[7][7])  
  		     << " \\\\" << std::endl; 
       fOutResultsGrid << " 400 & " << Form("%1.3f",Eff_Sig[8][8]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[8][8]) 
  		     << " & "       << Form("%1.3f",Eff_Sig[9][9]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[9][9])  
  		     << " & "       << Form("%1.3f",Eff_Sig[10][10]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[10][10])  
  		     << " & "       << Form("%1.3f",Eff_Sig[11][11]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[11][11])  
  		     << " & "       << Form("%1.3f",Eff_Sig[12][12]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[12][12])  
  		     << " & "       << Form("%1.3f",Eff_Sig[13][13]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[13][13])  
  		     << " & "       << Form("%1.3f",Eff_Sig[14][14]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[14][14])  
  		     << " & "       << Form("%1.3f",Eff_Sig[15][15]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[15][15])  
  		     << " \\\\" << std::endl; 
       fOutResultsGrid << " 500 & " << " - "  
  		     << " & "       << Form("%1.3f",Eff_Sig[16][16]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[16][16])  
  		     << " & "       << Form("%1.3f",Eff_Sig[17][17]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[17][17])  
  		     << " & "       << Form("%1.3f",Eff_Sig[18][18]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[18][18])  
  		     << " & "       << Form("%1.3f",Eff_Sig[19][19]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[19][19])  
  		     << " & "       << Form("%1.3f",Eff_Sig[20][20]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[20][20])  
  		     << " & "       << Form("%1.3f",Eff_Sig[21][21]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[21][21])  
  		     << " & "       << Form("%1.3f",Eff_Sig[22][22]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[22][22])  
  		     << " \\\\" << std::endl;                                                                              
       fOutResultsGrid << " 600 & " << " - "                                                                               
  		     << " & "       << Form("%1.3f",Eff_Sig[23][23]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[23][23])  
  		     << " & "       << Form("%1.3f",Eff_Sig[24][24]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[24][24])  
  		     << " & "       << Form("%1.3f",Eff_Sig[25][25]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[25][25])  
  		     << " & "       << Form("%1.3f",Eff_Sig[26][26]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[26][26])  
  		     << " & "       << Form("%1.3f",Eff_Sig[27][27]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[27][27])  
  		     << " & "       << Form("%1.3f",Eff_Sig[28][28]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[28][28])  
  		     << " & "       << Form("%1.3f",Eff_Sig[29][29]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[29][29])  
  		     << " \\\\" << std::endl;                                                                              
       fOutResultsGrid << " 700 & " << " - "                                                                               
  		     << " & "       << " - "                                                                               
  		     << " & "       << Form("%1.3f",Eff_Sig[30][30]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[30][30])  
  		     << " & "       << Form("%1.3f",Eff_Sig[31][31]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[31][31])  
  		     << " & "       << Form("%1.3f",Eff_Sig[32][32]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[32][32])  
  		     << " & "       << Form("%1.3f",Eff_Sig[33][33]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[33][33])  
  		     << " & "       << Form("%1.3f",Eff_Sig[34][34]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[34][34])  
  		     << " & "       << Form("%1.3f",Eff_Sig[35][35]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[35][35])  
  		     << " \\\\" << std::endl;                                                                              
       fOutResultsGrid << " 800 & " << " - "                                                                               
  		     << " & "       << " - "                                                                               
  		     << " & "       << Form("%1.3f",Eff_Sig[36][36]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[36][36])  
  		     << " & "       << Form("%1.3f",Eff_Sig[37][37]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[37][37])  
  		     << " & "       << Form("%1.3f",Eff_Sig[38][38]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[38][38])  
  		     << " & "       << Form("%1.3f",Eff_Sig[39][39]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[39][39])  
  		     << " & "       << Form("%1.3f",Eff_Sig[40][40]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[40][40])  
  		     << " & "       << Form("%1.3f",Eff_Sig[41][41]) 	<< " $ \\pm $ " << Form("%1.3f",Err_Eff_Sig[41][41])  
  		     << " \\\\" << std::endl; 
     }// end table for fully optimized sel      

     fOutResultsGrid << "\\hline \\hline" <<std::endl;
     fOutResultsGrid << "\\end{tabular}" <<std::endl;
     fOutResultsGrid << "\\end{table}" <<std::endl;
     // end efficiency table

     // finish Latex doc
     fOutResultsGrid << "\\end{document}" <<std::endl;



  }// fOutResultsGrid is_open
  else std::cout << "Unable to open Yields/Efficiency Table Output File" << std::endl;
  fOutResultsGrid.close();
  std::cout << "Finished Writing Yields and Efficieny Table" << std::endl;

}// end CardMaker::MakeYieldAndEfficiencyTables

Double_t CardMaker::GetXsec(const UInt_t mA0, const UInt_t mZp){
  Double_t xsec = 1;
  TFile* theory_gz08 = new TFile("../hybridCL/ScanPlot_gz08.root"); 
  TH2F* xsecs = (TH2F*)theory_gz08->Get("xsec1"); 
  if (xsecs!=(TH2F*)NULL){
     Int_t binX = xsecs->GetXaxis()->FindBin(mZp);
     Int_t binY = xsecs->GetYaxis()->FindBin(mA0);
     xsec = xsecs->GetBinContent(binX,binY); 
  }
  else{
   xsec = 1;
   std::cout << "Couldn't find xsec histogram" << std::endl;
  }
  return xsec;
}// end CardMaker::GetXsec

void  CardMaker::WriteDataCard(const TString fSigName, const Double_t ND_Sig, const UInt_t NA_Data, const UInt_t ND_Data, const DblVec ND_Res){

  //final calculation: bkg rate = (Int)NA * Weight * alpha
  Double_t predBkg = (Double_t)NA_Data*alpha;

  TString mZp = "";
//  if (fSigName=="2HDM_mZP600_mA0300" || fSigName=="2HDM_mZP600_mA0400") mZp="600";  
//  if (fSigName=="2HDM_mZP800_mA0300" || fSigName=="2HDM_mZP800_mA0400" || fSigName=="2HDM_mZP800_mA0500" || fSigName=="2HDM_mZP800_mA0600") mZp="800";  
//  if (fSigName=="2HDM_mZP1000_mA0300" || fSigName=="2HDM_mZP1000_mA0400" || fSigName=="2HDM_mZP1000_mA0500" || fSigName=="2HDM_mZP1000_mA0600" || fSigName=="2HDM_mZP1000_mA0700" || fSigName=="2HDM_mZP1000_mA0800") mZp="1000";  
//  if (fSigName=="2HDM_mZP1200_mA0300" || fSigName=="2HDM_mZP1200_mA0400" || fSigName=="2HDM_mZP1200_mA0500" || fSigName=="2HDM_mZP1200_mA0600" || fSigName=="2HDM_mZP1200_mA0700" || fSigName=="2HDM_mZP1200_mA0800") mZp="1200";  
//  if (fSigName=="2HDM_mZP1400_mA0300" || fSigName=="2HDM_mZP1400_mA0400" || fSigName=="2HDM_mZP1400_mA0500" || fSigName=="2HDM_mZP1400_mA0600" || fSigName=="2HDM_mZP1400_mA0700" || fSigName=="2HDM_mZP1400_mA0800") mZp="1400";  
//  if (fSigName=="2HDM_mZP1700_mA0300" || fSigName=="2HDM_mZP1700_mA0400" || fSigName=="2HDM_mZP1700_mA0500" || fSigName=="2HDM_mZP1700_mA0600" || fSigName=="2HDM_mZP1700_mA0700" || fSigName=="2HDM_mZP1700_mA0800") mZp="1700";  
//  if (fSigName=="2HDM_mZP2000_mA0300" || fSigName=="2HDM_mZP2000_mA0400" || fSigName=="2HDM_mZP2000_mA0500" || fSigName=="2HDM_mZP2000_mA0600" || fSigName=="2HDM_mZP2000_mA0700" || fSigName=="2HDM_mZP2000_mA0800") mZp="2000";  
//  if (fSigName=="2HDM_mZP2500_mA0300" || fSigName=="2HDM_mZP2500_mA0400" || fSigName=="2HDM_mZP2500_mA0500" || fSigName=="2HDM_mZP2500_mA0600" || fSigName=="2HDM_mZP2500_mA0700" || fSigName=="2HDM_mZP2500_mA0800") mZp="2500";  


  Bool_t A0300 = false;
  if (fSigName=="2HDM_mZP600" || fSigName=="2HDM_mZP800" || fSigName=="2HDM_mZP1000" || fSigName=="2HDM_mZP1200" || fSigName=="2HDM_mZP1400" || fSigName=="2HDM_mZP1700" || fSigName=="2HDM_mZP2000" || fSigName=="2HDM_mZP2500") A0300=true;
  
  if (fSigName.Contains("_mZP600_",   TString::kExact)) mZp="600";
  if (fSigName.Contains("_mZP800_",   TString::kExact)) mZp="800";
  if (fSigName.Contains("_mZP1200_",  TString::kExact)) mZp="1200";
  if (fSigName.Contains("_mZP1400_",  TString::kExact)) mZp="1400";
  if (fSigName.Contains("_mZP1700_",  TString::kExact)) mZp="1700";
  if (fSigName.Contains("_mZP2000_",  TString::kExact)) mZp="2000";
  if (fSigName.Contains("_mZP2500_",  TString::kExact)) mZp="2500";
  if (fSigName.Contains("_mZP10000_", TString::kExact)) mZp="10000";
  if (fSigName.Contains("_mZP1000_",  TString::kExact)) mZp="1000";
  if (fSigName.Contains("_mZP100_",   TString::kExact)) mZp="100";
  if (fSigName.Contains("_mZP10_",    TString::kExact)) mZp="10";
  if (fSigName.Contains("_mZP15_",    TString::kExact)) mZp="15";
  if (fSigName.Contains("_mZP1995_",  TString::kExact)) mZp="1995";
  if (fSigName.Contains("_mZP200_",   TString::kExact)) mZp="200";
  if (fSigName.Contains("_mZP20_",    TString::kExact)) mZp="20";
  if (fSigName.Contains("_mZP295_",   TString::kExact)) mZp="295";
  if (fSigName.Contains("_mZP300_",   TString::kExact)) mZp="300";
  if (fSigName.Contains("_mZP500_",   TString::kExact)) mZp="500";
  if (fSigName.Contains("_mZP50_",    TString::kExact)) mZp="50";
  if (fSigName.Contains("_mZP995_",   TString::kExact)) mZp="995";
 
//  if (fSigName=="BaryonicZp_mZP1000_mChi1000" || fSigName=="BaryonicZp_mZP1000_mChi150" || fSigName=="BaryonicZp_mZP1000_mChi1") mZp="1000";
//  if (fSigName=="BaryonicZp_mZP500_mChi500"   || fSigName=="BaryonicZp_mZP500_mChi150"  || fSigName=="BaryonicZp_mZP500_mChi1")  mZp="500";


  TString DataCardName;
  if (A0300) DataCardName = Form("%s%s/DataCard_%s_mA0300.txt",fOutDir.Data(),fOut.Data(),fSigName.Data());
  else DataCardName = Form("%s%s/DataCard_%s.txt",fOutDir.Data(),fOut.Data(),fSigName.Data());
  std::cout << "Writing data card in: " << DataCardName.Data() << std::endl;
  fOutTxtFile.open(DataCardName); 
  if (fOutTxtFile.is_open()){
    fOutTxtFile << Form("#MonoHgg Datacard for C&C Limit Setting, %f fb-1 ",flumi) << std::endl;
    //fOutTxtFile << "#Run with:combine -M Asymptotic cardname.txt --run blind " << std::endl;
    fOutTxtFile << Form("# Lumi =  %f fb-1",flumi) << std::endl;
    fOutTxtFile << "imax 1 " << std::endl;
    fOutTxtFile << "jmax * " << std::endl;
    fOutTxtFile << "kmax * " << std::endl;
    fOutTxtFile << "------------------------------------" << std::endl;
    fOutTxtFile << "bin 1" << std::endl;
    if (doBlind) fOutTxtFile << "observation 0" << std::endl;
    else fOutTxtFile << Form("observation %i",ND_Data) << std::endl;
    fOutTxtFile << "------------------------------------" << std::endl;
    fOutTxtFile << "bin	    1	1	1	1	1	1" << std::endl;
    fOutTxtFile << Form("process	   MonoH_%s	b	GGH	VBF	VH	TTH",mZp.Data()) << std::endl;
    fOutTxtFile << "process	   0	1	2	3	4	5" << std::endl;
    fOutTxtFile << Form("rate	   %f	%f	%f	%f	%f	%f",ND_Sig,predBkg,ND_Res[0],ND_Res[1],ND_Res[2],ND_Res[3]) << std::endl;
    fOutTxtFile << " " << std::endl;
    fOutTxtFile << "------------------------------------" << std::endl;
    fOutTxtFile << "#MC related" << std::endl;
    fOutTxtFile << "lumi_13TeV	    lnN	1.027	-	1.027	1.027	1.027	1.027" << std::endl;
    fOutTxtFile << "CMS_MonoH_TrigEff	  lnN -		1.01      -   -   -   -" << std::endl;
    fOutTxtFile << "CMS_MonoH_eff	  lnN 1.020   -       1.020   1.020   1.020   1.020" << std::endl;
    fOutTxtFile << "CMS_MonoH_higg_BR     lnN -		-	0.953/1.050	0.953/1.050	0.953/1.050	0.953/1.050" << std::endl;
    fOutTxtFile << "CMS_MonoH_higg_alphas lnN -		-	0.940/0.965	0.940/0.965	0.940/0.965	0.940/0.965" << std::endl;
    fOutTxtFile << "CMS_MonoH_PDFs        lnN -		-	1.05		1.05		1.05		1.05" << std::endl;
    fOutTxtFile << "CMS_MonoH_JetEnUp     lnN 1.005           -       -		-		1.005		-" << std::endl;
    fOutTxtFile << "CMS_MonoH_JetEnDown   lnN 1.005           -       -		-		1.005		-" << std::endl;
    fOutTxtFile << "CMS_MonoH_PhoEnUp     lnN 1.005           -       -		-		1.005		-" << std::endl;
    fOutTxtFile << "CMS_MonoH_PhoEnDown   lnN 1.005           -       -		-		1.005		-" << std::endl;
    fOutTxtFile << "CMS_MonoH_UnclEnUp    lnN 1.005           -       -               -               1.005           -" << std::endl;
    fOutTxtFile << "CMS_MonoH_UnclEnDown  lnN 1.005           -       -               -               1.005           -" << std::endl;
    fOutTxtFile << "CMS_MonoH_FakeMet     lnN -               -		0.1/1.9         0.1/1.9		-		0.1/1.9 " << std::endl;
    fOutTxtFile << "------------------------------------" << std::endl;
    fOutTxtFile << "#background related " << std::endl;
    fOutTxtFile << Form("bg_lept	    gmN	%i	-	%f	-	-	-	-",NA_Data,alpha) << std::endl;
    fOutTxtFile << "alpha lnN - 1.2 - - - -" << std::endl;
  
  }// fOutTxtFile is_open
  else std::cout << "Unable to open DataCard Output File" << std::endl;
  fOutTxtFile.close();
  std::cout << "Finished Writing DataCard" << std::endl;



}// end CardMaker::WriteDataCard



void CardMaker::SetupCutsToApply(){

  /////////////////////////////////////////////////////////  
  // Setup cuts for pt1/mgg for each signal sample
  Cut_pt1mgg.push_back(0.50); // mZp600, mA0300
  Cut_pt1mgg.push_back(0.80); // mZp800, mA0300
  Cut_pt1mgg.push_back(0.85); // mZp1000, mA0300
  Cut_pt1mgg.push_back(1.20); // mZp1200, mA0300
  Cut_pt1mgg.push_back(1.40); // mZp1400, mA0300
  Cut_pt1mgg.push_back(1.40); // mZp1700, mA0300
  Cut_pt1mgg.push_back(1.40); // mZp2000, mA0300
  Cut_pt1mgg.push_back(1.40); // mZp2500, mA0300

  Cut_pt1mgg.push_back(Cut_pt1mgg[0]); // mZp600, mA0400
  Cut_pt1mgg.push_back(Cut_pt1mgg[1]); // mZp800, mA0400
  Cut_pt1mgg.push_back(Cut_pt1mgg[2]); // mZp1000, mA0400
  Cut_pt1mgg.push_back(Cut_pt1mgg[3]); // mZp1200, mA0400
  Cut_pt1mgg.push_back(Cut_pt1mgg[4]); // mZp1400, mA0400
  Cut_pt1mgg.push_back(Cut_pt1mgg[5]); // mZp1700, mA0400
  Cut_pt1mgg.push_back(Cut_pt1mgg[6]); // mZp2000, mA0400
  Cut_pt1mgg.push_back(Cut_pt1mgg[7]); // mZp2500, mA0400

  //Cut_pt1mgg.push_back(Cut_pt1mgg[0]); // mZp600, mA0500
  Cut_pt1mgg.push_back(Cut_pt1mgg[1]); // mZp800, mA0500
  Cut_pt1mgg.push_back(Cut_pt1mgg[2]); // mZp1000, mA0500
  Cut_pt1mgg.push_back(Cut_pt1mgg[3]); // mZp1200, mA0500
  Cut_pt1mgg.push_back(Cut_pt1mgg[4]); // mZp1400, mA0500
  Cut_pt1mgg.push_back(Cut_pt1mgg[5]); // mZp1700, mA0500
  Cut_pt1mgg.push_back(Cut_pt1mgg[6]); // mZp2000, mA0500
  Cut_pt1mgg.push_back(Cut_pt1mgg[7]); // mZp2500, mA0500
   
  //Cut_pt1mgg.push_back(Cut_pt1mgg[0]); // mZp600, mA0600
  Cut_pt1mgg.push_back(Cut_pt1mgg[1]); // mZp800, mA0600
  Cut_pt1mgg.push_back(Cut_pt1mgg[2]); // mZp1000, mA0600
  Cut_pt1mgg.push_back(Cut_pt1mgg[3]); // mZp1200, mA0600
  Cut_pt1mgg.push_back(Cut_pt1mgg[4]); // mZp1400, mA0600
  Cut_pt1mgg.push_back(Cut_pt1mgg[5]); // mZp1700, mA0600
  Cut_pt1mgg.push_back(Cut_pt1mgg[6]); // mZp2000, mA0600
  Cut_pt1mgg.push_back(Cut_pt1mgg[7]); // mZp2500, mA0600
   
  //Cut_pt1mgg.push_back(Cut_pt1mgg[0]); // mZp600, mA0700
  //Cut_pt1mgg.push_back(Cut_pt1mgg[1]); // mZp800, mA0700
  Cut_pt1mgg.push_back(Cut_pt1mgg[2]); // mZp1000, mA0700
  Cut_pt1mgg.push_back(Cut_pt1mgg[3]); // mZp1200, mA0700
  Cut_pt1mgg.push_back(Cut_pt1mgg[4]); // mZp1400, mA0700
  Cut_pt1mgg.push_back(Cut_pt1mgg[5]); // mZp1700, mA0700
  Cut_pt1mgg.push_back(Cut_pt1mgg[6]); // mZp2000, mA0700
  Cut_pt1mgg.push_back(Cut_pt1mgg[7]); // mZp2500, mA0700
   
  //Cut_pt1mgg.push_back(Cut_pt1mgg[0]); // mZp600, mA0800
  //Cut_pt1mgg.push_back(Cut_pt1mgg[1]); // mZp800, mA0800
  Cut_pt1mgg.push_back(Cut_pt1mgg[2]); // mZp1000, mA0800
  Cut_pt1mgg.push_back(Cut_pt1mgg[3]); // mZp1200, mA0800
  Cut_pt1mgg.push_back(Cut_pt1mgg[4]); // mZp1400, mA0800
  Cut_pt1mgg.push_back(Cut_pt1mgg[5]); // mZp1700, mA0800
  Cut_pt1mgg.push_back(Cut_pt1mgg[6]); // mZp2000, mA0800
  Cut_pt1mgg.push_back(Cut_pt1mgg[7]); // mZp2500, mA0800
  

 
  /////////////////////////////////////////////////////////  
  // Setup cuts for pt2/mgg for each signal sample
  Cut_pt2mgg.push_back(0.25); // mZp600, mA0300
  Cut_pt2mgg.push_back(0.25); // mZp800, mA0300
  Cut_pt2mgg.push_back(0.25); // mZp1000, mA0300
  Cut_pt2mgg.push_back(0.25); // mZp1200, mA0300
  Cut_pt2mgg.push_back(0.25); // mZp1400, mA0300
  Cut_pt2mgg.push_back(0.25); // mZp1700, mA0300
  Cut_pt2mgg.push_back(0.25); // mZp2000, mA0300
  Cut_pt2mgg.push_back(0.25); // mZp2500, mA0300

  Cut_pt2mgg.push_back(Cut_pt2mgg[0]); // mZp600, mA0400
  Cut_pt2mgg.push_back(Cut_pt2mgg[1]); // mZp800, mA0400
  Cut_pt2mgg.push_back(Cut_pt2mgg[2]); // mZp1000, mA0400
  Cut_pt2mgg.push_back(Cut_pt2mgg[3]); // mZp1200, mA0400
  Cut_pt2mgg.push_back(Cut_pt2mgg[4]); // mZp1400, mA0400
  Cut_pt2mgg.push_back(Cut_pt2mgg[5]); // mZp1700, mA0400
  Cut_pt2mgg.push_back(Cut_pt2mgg[6]); // mZp2000, mA0400
  Cut_pt2mgg.push_back(Cut_pt2mgg[7]); // mZp2500, mA0400

  //Cut_pt2mgg.push_back(Cut_pt2mgg[0]); // mZp600, mA0500
  Cut_pt2mgg.push_back(Cut_pt2mgg[1]); // mZp800, mA0500
  Cut_pt2mgg.push_back(Cut_pt2mgg[2]); // mZp1000, mA0500
  Cut_pt2mgg.push_back(Cut_pt2mgg[3]); // mZp1200, mA0500
  Cut_pt2mgg.push_back(Cut_pt2mgg[4]); // mZp1400, mA0500
  Cut_pt2mgg.push_back(Cut_pt2mgg[5]); // mZp1700, mA0500
  Cut_pt2mgg.push_back(Cut_pt2mgg[6]); // mZp2000, mA0500
  Cut_pt2mgg.push_back(Cut_pt2mgg[7]); // mZp2500, mA0500
   
  //Cut_pt2mgg.push_back(Cut_pt2mgg[0]); // mZp600, mA0600
  Cut_pt2mgg.push_back(Cut_pt2mgg[1]); // mZp800, mA0600
  Cut_pt2mgg.push_back(Cut_pt2mgg[2]); // mZp1000, mA0600
  Cut_pt2mgg.push_back(Cut_pt2mgg[3]); // mZp1200, mA0600
  Cut_pt2mgg.push_back(Cut_pt2mgg[4]); // mZp1400, mA0600
  Cut_pt2mgg.push_back(Cut_pt2mgg[5]); // mZp1700, mA0600
  Cut_pt2mgg.push_back(Cut_pt2mgg[6]); // mZp2000, mA0600
  Cut_pt2mgg.push_back(Cut_pt2mgg[7]); // mZp2500, mA0600
   
  //Cut_pt2mgg.push_back(Cut_pt2mgg[0]); // mZp600, mA0700
  //Cut_pt2mgg.push_back(Cut_pt2mgg[1]); // mZp800, mA0700
  Cut_pt2mgg.push_back(Cut_pt2mgg[2]); // mZp1000, mA0700
  Cut_pt2mgg.push_back(Cut_pt2mgg[3]); // mZp1200, mA0700
  Cut_pt2mgg.push_back(Cut_pt2mgg[4]); // mZp1400, mA0700
  Cut_pt2mgg.push_back(Cut_pt2mgg[5]); // mZp1700, mA0700
  Cut_pt2mgg.push_back(Cut_pt2mgg[6]); // mZp2000, mA0700
  Cut_pt2mgg.push_back(Cut_pt2mgg[7]); // mZp2500, mA0700
   
  //Cut_pt2mgg.push_back(Cut_pt2mgg[0]); // mZp600, mA0800
  //Cut_pt2mgg.push_back(Cut_pt2mgg[1]); // mZp800, mA0800
  Cut_pt2mgg.push_back(Cut_pt2mgg[2]); // mZp1000, mA0800
  Cut_pt2mgg.push_back(Cut_pt2mgg[3]); // mZp1200, mA0800
  Cut_pt2mgg.push_back(Cut_pt2mgg[4]); // mZp1400, mA0800
  Cut_pt2mgg.push_back(Cut_pt2mgg[5]); // mZp1700, mA0800
  Cut_pt2mgg.push_back(Cut_pt2mgg[6]); // mZp2000, mA0800
  Cut_pt2mgg.push_back(Cut_pt2mgg[7]); // mZp2500, mA0800
  

  
  /////////////////////////////////////////////////////////  
  // Setup cuts for ptgg for each signal sample
  Cut_ptgg.push_back(90);  // mZp600, mA0300
  Cut_ptgg.push_back(135); // mZp800, mA0300
  Cut_ptgg.push_back(170); // mZp1000, mA0300
  Cut_ptgg.push_back(250); // mZp1200, mA0300
  Cut_ptgg.push_back(260); // mZp1400, mA0300
  Cut_ptgg.push_back(315); // mZp1700, mA0300
  Cut_ptgg.push_back(315); // mZp2000, mA0300
  Cut_ptgg.push_back(315); // mZp2500, mA0300

  Cut_ptgg.push_back(Cut_ptgg[0]); // mZp600, mA0400
  Cut_ptgg.push_back(Cut_ptgg[1]); // mZp800, mA0400
  Cut_ptgg.push_back(Cut_ptgg[2]); // mZp1000, mA0400
  Cut_ptgg.push_back(Cut_ptgg[3]); // mZp1200, mA0400
  Cut_ptgg.push_back(Cut_ptgg[4]); // mZp1400, mA0400
  Cut_ptgg.push_back(Cut_ptgg[5]); // mZp1700, mA0400
  Cut_ptgg.push_back(Cut_ptgg[6]); // mZp2000, mA0400
  Cut_ptgg.push_back(Cut_ptgg[7]); // mZp2500, mA0400

  //Cut_ptgg.push_back(Cut_ptgg[0]); // mZp600, mA0500
  Cut_ptgg.push_back(Cut_ptgg[1]); // mZp800, mA0500
  Cut_ptgg.push_back(Cut_ptgg[2]); // mZp1000, mA0500
  Cut_ptgg.push_back(Cut_ptgg[3]); // mZp1200, mA0500
  Cut_ptgg.push_back(Cut_ptgg[4]); // mZp1400, mA0500
  Cut_ptgg.push_back(Cut_ptgg[5]); // mZp1700, mA0500
  Cut_ptgg.push_back(Cut_ptgg[6]); // mZp2000, mA0500
  Cut_ptgg.push_back(Cut_ptgg[7]); // mZp2500, mA0500
   
  //Cut_ptgg.push_back(Cut_ptgg[0]); // mZp600, mA0600
  Cut_ptgg.push_back(Cut_ptgg[1]); // mZp800, mA0600
  Cut_ptgg.push_back(Cut_ptgg[2]); // mZp1000, mA0600
  Cut_ptgg.push_back(Cut_ptgg[3]); // mZp1200, mA0600
  Cut_ptgg.push_back(Cut_ptgg[4]); // mZp1400, mA0600
  Cut_ptgg.push_back(Cut_ptgg[5]); // mZp1700, mA0600
  Cut_ptgg.push_back(Cut_ptgg[6]); // mZp2000, mA0600
  Cut_ptgg.push_back(Cut_ptgg[7]); // mZp2500, mA0600
   
  //Cut_ptgg.push_back(Cut_ptgg[0]); // mZp600, mA0700
  //Cut_ptgg.push_back(Cut_ptgg[1]); // mZp800, mA0700
  Cut_ptgg.push_back(Cut_ptgg[2]); // mZp1000, mA0700
  Cut_ptgg.push_back(Cut_ptgg[3]); // mZp1200, mA0700
  Cut_ptgg.push_back(Cut_ptgg[4]); // mZp1400, mA0700
  Cut_ptgg.push_back(Cut_ptgg[5]); // mZp1700, mA0700
  Cut_ptgg.push_back(Cut_ptgg[6]); // mZp2000, mA0700
  Cut_ptgg.push_back(Cut_ptgg[7]); // mZp2500, mA0700
   
  //Cut_ptgg.push_back(Cut_ptgg[0]); // mZp600, mA0800
  //Cut_ptgg.push_back(Cut_ptgg[1]); // mZp800, mA0800
  Cut_ptgg.push_back(Cut_ptgg[2]); // mZp1000, mA0800
  Cut_ptgg.push_back(Cut_ptgg[3]); // mZp1200, mA0800
  Cut_ptgg.push_back(Cut_ptgg[4]); // mZp1400, mA0800
  Cut_ptgg.push_back(Cut_ptgg[5]); // mZp1700, mA0800
  Cut_ptgg.push_back(Cut_ptgg[6]); // mZp2000, mA0800
  Cut_ptgg.push_back(Cut_ptgg[7]); // mZp2500, mA0800
   

  
  /////////////////////////////////////////////////////////  
  // Setup cuts for MET for each signal sample
  Cut_met.push_back(130); // mZp600, mA0300
  Cut_met.push_back(165); // mZp800, mA0300
  Cut_met.push_back(220); // mZp1000, mA0300
  Cut_met.push_back(245); // mZp1200, mA0300
  Cut_met.push_back(255); // mZp1400, mA0300
  Cut_met.push_back(285); // mZp1700, mA0300
  Cut_met.push_back(285); // mZp2000, mA0300
  Cut_met.push_back(285); // mZp2500, mA0300

  Cut_met.push_back(Cut_met[0]); // mZp600, mA0400
  Cut_met.push_back(Cut_met[1]); // mZp800, mA0400
  Cut_met.push_back(Cut_met[2]); // mZp1000, mA0400
  Cut_met.push_back(Cut_met[3]); // mZp1200, mA0400
  Cut_met.push_back(Cut_met[4]); // mZp1400, mA0400
  Cut_met.push_back(Cut_met[5]); // mZp1700, mA0400
  Cut_met.push_back(Cut_met[6]); // mZp2000, mA0400
  Cut_met.push_back(Cut_met[7]); // mZp2500, mA0400

  //Cut_met.push_back(Cut_met[0]); // mZp600, mA0500
  Cut_met.push_back(Cut_met[1]); // mZp800, mA0500
  Cut_met.push_back(Cut_met[2]); // mZp1000, mA0500
  Cut_met.push_back(Cut_met[3]); // mZp1200, mA0500
  Cut_met.push_back(Cut_met[4]); // mZp1400, mA0500
  Cut_met.push_back(Cut_met[5]); // mZp1700, mA0500
  Cut_met.push_back(Cut_met[6]); // mZp2000, mA0500
  Cut_met.push_back(Cut_met[7]); // mZp2500, mA0500
   
  //Cut_met.push_back(Cut_met[0]); // mZp600, mA0600
  Cut_met.push_back(Cut_met[1]); // mZp800, mA0600
  Cut_met.push_back(Cut_met[2]); // mZp1000, mA0600
  Cut_met.push_back(Cut_met[3]); // mZp1200, mA0600
  Cut_met.push_back(Cut_met[4]); // mZp1400, mA0600
  Cut_met.push_back(Cut_met[5]); // mZp1700, mA0600
  Cut_met.push_back(Cut_met[6]); // mZp2000, mA0600
  Cut_met.push_back(Cut_met[7]); // mZp2500, mA0600
   
  //Cut_met.push_back(Cut_met[0]); // mZp600, mA0700
  //Cut_met.push_back(Cut_met[1]); // mZp800, mA0700
  Cut_met.push_back(Cut_met[2]); // mZp1000, mA0700
  Cut_met.push_back(Cut_met[3]); // mZp1200, mA0700
  Cut_met.push_back(Cut_met[4]); // mZp1400, mA0700
  Cut_met.push_back(Cut_met[5]); // mZp1700, mA0700
  Cut_met.push_back(Cut_met[6]); // mZp2000, mA0700
  Cut_met.push_back(Cut_met[7]); // mZp2500, mA0700
   
  //Cut_met.push_back(Cut_met[0]); // mZp600, mA0800
  //Cut_met.push_back(Cut_met[1]); // mZp800, mA0800
  Cut_met.push_back(Cut_met[2]); // mZp1000, mA0800
  Cut_met.push_back(Cut_met[3]); // mZp1200, mA0800
  Cut_met.push_back(Cut_met[4]); // mZp1400, mA0800
  Cut_met.push_back(Cut_met[5]); // mZp1700, mA0800
  Cut_met.push_back(Cut_met[6]); // mZp2000, mA0800
  Cut_met.push_back(Cut_met[7]); // mZp2500, mA0800
   

}// end CardMaker::SetupCutsToApply



void CardMaker::SetBranchAddresses( TTree * treeIn){
  treeIn->SetBranchAddress("metF_GV",&metF_GV, &b_metF_GV);
  treeIn->SetBranchAddress("metF_globalTightHalo",&metF_globalTightHalo, &b_metF_globalTightHalo);
  treeIn->SetBranchAddress("metF_HBHENoise", &metF_HBHENoise, &b_metF_HBHENoise);
  treeIn->SetBranchAddress("metF_HBHENoiseIso", &metF_HBHENoiseIso, &b_metF_HBHENoiseIso);
  treeIn->SetBranchAddress("metF_EcalDeadCell",&metF_EcalDeadCell, &b_metF_EcalDeadCell);
  treeIn->SetBranchAddress("metF_badMuon",&metF_badMuon, &b_metF_badMuon);
  treeIn->SetBranchAddress("metF_badChargedHadron",&metF_badChargedHadron, &b_metF_badChargedHadron); 
  treeIn->SetBranchAddress("metF_CSC", &metF_CSC, &b_metF_CSC);
  treeIn->SetBranchAddress("metF_eeBadSC", &metF_eeBadSC, &b_metF_eeBadSC);
  treeIn->SetBranchAddress("metF_MuonBadTrack", &metF_MuonBadTrack, &b_metF_MuonBadTrack);
  treeIn->SetBranchAddress("metF_HadronTrackRes", &metF_HadronTrackRes, &b_metF_HadronTrackRes);

  treeIn->SetBranchAddress("run", &run, &b_run);
  treeIn->SetBranchAddress("event", &event, &b_event);
  treeIn->SetBranchAddress("lumi", &lumi, &b_lumi);
  treeIn->SetBranchAddress("rho", &rho, &b_rho);
  treeIn->SetBranchAddress("sampleID", &sampleID, &b_sampleID);
  treeIn->SetBranchAddress("weight", &weight,  &b_weight);
  treeIn->SetBranchAddress("nvtx",   &nvtx,    &b_nvtx);
  treeIn->SetBranchAddress("mgg",    &mgg,     &b_mgg);
  treeIn->SetBranchAddress("ptgg",   &ptgg,    &b_ptgg);
  treeIn->SetBranchAddress("t1pfmet", &t1pfmet, &b_t1pfmet);   
  treeIn->SetBranchAddress("t1pfmetCorr", &t1pfmetCorr, &b_t1pfmetCorr);   
  treeIn->SetBranchAddress("t1pfmetCorrPhi", &t1pfmetCorrPhi, &b_t1pfmetCorrPhi);   
  treeIn->SetBranchAddress("t1p2pfmet", &t1p2pfmet, &b_t1p2pfmet);
  treeIn->SetBranchAddress("t1pfmetJetEnUp", &t1pfmetJetEnUp, &b_t1pfmetJetEnUp);
  treeIn->SetBranchAddress("t1pfmetJetEnDown", &t1pfmetJetEnDown, &b_t1pfmetJetEnDown);
  treeIn->SetBranchAddress("t1pfmetJetResUp", &t1pfmetJetResUp, &b_t1pfmetJetResUp);
  treeIn->SetBranchAddress("t1pfmetJetResDown", &t1pfmetJetResDown, &b_t1pfmetJetResDown);
  treeIn->SetBranchAddress("t1pfmetMuonEnUp", &t1pfmetMuonEnUp, &b_t1pfmetMuonEnUp);
  treeIn->SetBranchAddress("t1pfmetMuonEnDown", &t1pfmetMuonEnDown, &b_t1pfmetMuonEnDown);
  treeIn->SetBranchAddress("t1pfmetElectronEnUp", &t1pfmetElectronEnUp, &b_t1pfmetElectronEnUp);
  treeIn->SetBranchAddress("t1pfmetElectronEnDown", &t1pfmetElectronEnDown, &b_t1pfmetElectronEnDown);
  treeIn->SetBranchAddress("t1pfmetTauEnUp", &t1pfmetTauEnUp, &b_t1pfmetTauEnUp);
  treeIn->SetBranchAddress("t1pfmetTauEnDown", &t1pfmetTauEnDown, &b_t1pfmetTauEnDown);
  treeIn->SetBranchAddress("t1pfmetPhotonEnUp", &t1pfmetPhotonEnUp, &b_t1pfmetPhotonEnUp);
  treeIn->SetBranchAddress("t1pfmetPhotonEnDown", &t1pfmetPhotonEnDown, &b_t1pfmetPhotonEnDown);
  treeIn->SetBranchAddress("t1pfmetUnclusteredEnUp", &t1pfmetUnclusteredEnUp, &b_t1pfmetUnclusteredEnUp);
  treeIn->SetBranchAddress("t1pfmetUnclusteredEnDown", &t1pfmetUnclusteredEnDown, &b_t1pfmetUnclusteredEnDown);
  treeIn->SetBranchAddress("t1pfmetPhi", &t1pfmetPhi, &b_t1pfmetPhi);
  treeIn->SetBranchAddress("t1pfmetSumEt", &t1pfmetSumEt, &b_t1pfmetSumEt);   
  treeIn->SetBranchAddress("pfmet", &pfmet, &b_pfmet);   
  treeIn->SetBranchAddress("pfmetPhi", &pfmetphi, &b_pfmetPhi);   
  treeIn->SetBranchAddress("pfmetSumEt", &pfmetSumEt, &b_pfmetSumEt);   
  treeIn->SetBranchAddress("calomet", &calomet, &b_calomet);   
  treeIn->SetBranchAddress("calometPhi", &calometphi, &b_calometPhi);   
  treeIn->SetBranchAddress("calometSumEt", &calometSumEt, &b_calometSumEt);   
  treeIn->SetBranchAddress("genmatch1", &genmatch1, &b_genmatch1);  
  treeIn->SetBranchAddress("genmatch2", &genmatch2, &b_genmatch2);   
  treeIn->SetBranchAddress("pt1", &pt1, &b_pt1);   
  treeIn->SetBranchAddress("pt2", &pt2, &b_pt2);   
  treeIn->SetBranchAddress("chiso1", &chiso1, &b_chiso1);   
  treeIn->SetBranchAddress("chiso2", &chiso2, &b_chiso2);   
  treeIn->SetBranchAddress("neuiso1", &neuiso1, &b_neuiso1);   
  treeIn->SetBranchAddress("neuiso2", &neuiso2, &b_neuiso2);   
  treeIn->SetBranchAddress("phoiso1", &phoiso1, &b_phoiso1);   
  treeIn->SetBranchAddress("phoiso2", &phoiso2, &b_phoiso2);   
  treeIn->SetBranchAddress("sieie1", &sieie1, &b_sieie1);   
  treeIn->SetBranchAddress("sieie2", &sieie2, &b_sieie2);   
  treeIn->SetBranchAddress("hoe1", &hoe1, &b_hoe1);   
  treeIn->SetBranchAddress("hoe2", &hoe2, &b_hoe2);   
  treeIn->SetBranchAddress("r91", &r91, &b_r91);   
  treeIn->SetBranchAddress("r92", &r92, &b_r92);   
  treeIn->SetBranchAddress("phi1", &phi1, &b_phi1);   
  treeIn->SetBranchAddress("phi2", &phi2, &b_phi2);   
  treeIn->SetBranchAddress("eta1", &eta1, &b_eta1);   
  treeIn->SetBranchAddress("eta2", &eta2, &b_eta2);   
  treeIn->SetBranchAddress("eleveto1", &eleveto1, &b_eleveto1);   
  treeIn->SetBranchAddress("eleveto2", &eleveto2, &b_eleveto2);  
  treeIn->SetBranchAddress("presel1", &presel1, &b_presel1); 
  treeIn->SetBranchAddress("presel2", &presel2, &b_presel2); 
  treeIn->SetBranchAddress("sel1", &sel1, &b_sel1); 
  treeIn->SetBranchAddress("sel2", &sel2, &b_sel2); 
  treeIn->SetBranchAddress("passCHiso1", &passCHiso1, &b_passCHiso1);   
  treeIn->SetBranchAddress("passCHiso2", &passCHiso2, &b_passCHiso2);   
  treeIn->SetBranchAddress("passNHiso1", &passNHiso1, &b_passNHiso1);   
  treeIn->SetBranchAddress("passNHiso2", &passNHiso2, &b_passNHiso2);   
  treeIn->SetBranchAddress("passPHiso1", &passPHiso1, &b_passNHiso1);   
  treeIn->SetBranchAddress("passPHiso2", &passPHiso2, &b_passNHiso2);   
  treeIn->SetBranchAddress("passSieie1", &passSieie1, &b_passSieie1);
  treeIn->SetBranchAddress("passSieie2", &passSieie2, &b_passSieie2);
  treeIn->SetBranchAddress("passHoe1", &passHoe1, &b_passHoe1);
  treeIn->SetBranchAddress("passHoe2", &passHoe2, &b_passHoe2);
  treeIn->SetBranchAddress("passLooseCHiso1", &passLooseCHiso1, &b_passLooseCHiso1);
  treeIn->SetBranchAddress("passLooseCHiso2", &passLooseCHiso2, &b_passLooseCHiso2);
  treeIn->SetBranchAddress("passLooseNHiso1", &passLooseNHiso1, &b_passLooseNHiso1);
  treeIn->SetBranchAddress("passLooseNHiso2", &passLooseNHiso2, &b_passLooseNHiso2);
  treeIn->SetBranchAddress("passLoosePHiso1", &passLoosePHiso1, &b_passLoosePHiso1);
  treeIn->SetBranchAddress("passLoosePHiso2", &passLoosePHiso2, &b_passLoosePHiso2);
  treeIn->SetBranchAddress("passLooseSieie1", &passLooseSieie1, &b_passLooseSieie1);
  treeIn->SetBranchAddress("passLooseSieie2", &passLooseSieie2, &b_passLooseSieie2);
  treeIn->SetBranchAddress("passLooseHoe1", &passLooseHoe1, &b_passLooseHoe1);
  treeIn->SetBranchAddress("passLooseHoe2", &passLooseHoe2, &b_passLooseHoe2);
  treeIn->SetBranchAddress("passTightCHiso1", &passTightCHiso1, &b_passTightCHiso1);
  treeIn->SetBranchAddress("passTightCHiso2", &passTightCHiso2, &b_passTightCHiso2);
  treeIn->SetBranchAddress("passTightNHiso1", &passTightNHiso1, &b_passTightNHiso1);
  treeIn->SetBranchAddress("passTightNHiso2", &passTightNHiso2, &b_passTightNHiso2);
  treeIn->SetBranchAddress("passTightPHiso1", &passTightPHiso1, &b_passTightPHiso1);
  treeIn->SetBranchAddress("passTightPHiso2", &passTightPHiso2, &b_passTightPHiso2);
  treeIn->SetBranchAddress("passTightSieie1", &passTightSieie1, &b_passTightSieie1);
  treeIn->SetBranchAddress("passTightSieie2", &passTightSieie2, &b_passTightSieie2);
  treeIn->SetBranchAddress("passTightHoe1", &passTightHoe1, &b_passTightHoe1);
  treeIn->SetBranchAddress("passTightHoe2", &passTightHoe2, &b_passTightHoe2);
  treeIn->SetBranchAddress("hltPhoton26Photon16Mass60", &hltPhoton26Photon16Mass60, &b_hltPhoton26Photon16Mass60);
  treeIn->SetBranchAddress("hltPhoton36Photon22Mass15", &hltPhoton36Photon22Mass15, &b_hltPhoton36Photon22Mass15);
  treeIn->SetBranchAddress("hltPhoton42Photon25Mass15", &hltPhoton42Photon25Mass15, &b_hltPhoton42Photon25Mass15);
  treeIn->SetBranchAddress("hltDiphoton30Mass95", &hltDiphoton30Mass95, &b_hltDiphoton30Mass95);
  treeIn->SetBranchAddress("hltDiphoton30Mass70", &hltDiphoton30Mass70, &b_hltDiphoton30Mass70);
  treeIn->SetBranchAddress("hltDiphoton30Mass55", &hltDiphoton30Mass55, &b_hltDiphoton30Mass55);
  treeIn->SetBranchAddress("hltDiphoton30Mass55PV", &hltDiphoton30Mass55PV, &b_hltDiphoton30Mass55PV);
  treeIn->SetBranchAddress("hltDiphoton30Mass55EB", &hltDiphoton30Mass55EB, &b_hltDiphoton30Mass55EB);
  treeIn->SetBranchAddress("nEle", &nEle, &b_nEle);
  treeIn->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
  treeIn->SetBranchAddress("nJets20", &nJets20, &b_nJets20);
  treeIn->SetBranchAddress("nJets30", &nJets30, &b_nJets30);
  treeIn->SetBranchAddress("nJets40", &nJets40, &b_nJets40);
  treeIn->SetBranchAddress("nJets50", &nJets50, &b_nJets50);
  treeIn->SetBranchAddress("nLooseBjets", &nLooseBjets, &b_nLooseBjets);
  treeIn->SetBranchAddress("nMediumBjets", &nMediumBjets, &b_nMediumBjets);
  treeIn->SetBranchAddress("ptJetLead", &ptJetLead, &b_ptJetLead);
  treeIn->SetBranchAddress("etaJetLead", &etaJetLead, &b_etaJetLead);
  treeIn->SetBranchAddress("phiJetLead", &phiJetLead, &b_phiJetLead);
  treeIn->SetBranchAddress("massJetLead", &massJetLead, &b_massJetLead);
  treeIn->SetBranchAddress("indexJetLead", &indexJetLead, &b_indexJetLead);
  treeIn->SetBranchAddress("NEMfracJet1", &NEMfracJet1, &b_NEMfracJet1);
  treeIn->SetBranchAddress("CEMfracJet1", &CEMfracJet1, &b_CEMfracJet1);
  treeIn->SetBranchAddress("ELfracJet1", &ELfracJet1, &b_ELfracJet1);
  treeIn->SetBranchAddress("CHfracJet1", &CHfracJet1, &b_CHfracJet1);
  treeIn->SetBranchAddress("NHfracJet1", &NHfracJet1, &b_NHfracJet1);
  treeIn->SetBranchAddress("PHfracJet1", &PHfracJet1, &b_PHfracJet1);
  treeIn->SetBranchAddress("MUfracJet1", &MUfracJet1, &b_MUfracJet1);
  treeIn->SetBranchAddress("CHmultJet1", &CHmultJet1, &b_CHmultJet1);
  treeIn->SetBranchAddress("NEmultJet1", &NEmultJet1, &b_NEmultJet1);
  treeIn->SetBranchAddress("ptJetSubLead", &ptJetSubLead, &b_ptJetSubLead);
  treeIn->SetBranchAddress("etaJetSubLead", &etaJetSubLead, &b_etaJetSubLead);
  treeIn->SetBranchAddress("phiJetSubLead", &phiJetSubLead, &b_phiJetSubLead);
  treeIn->SetBranchAddress("massJetSubLead", &massJetSubLead, &b_massJetSubLead);
  treeIn->SetBranchAddress("indexJetSubLead", &indexJetSubLead, &b_indexJetSubLead);
  treeIn->SetBranchAddress("NEMfracJet2", &NEMfracJet2, &b_NEMfracJet2);
  treeIn->SetBranchAddress("CEMfracJet2", &CEMfracJet2, &b_CEMfracJet2);
  treeIn->SetBranchAddress("ELfracJet2", &ELfracJet2, &b_ELfracJet2);
  treeIn->SetBranchAddress("CHfracJet2", &CHfracJet2, &b_CHfracJet2);
  treeIn->SetBranchAddress("NHfracJet2", &NHfracJet2, &b_NHfracJet2);
  treeIn->SetBranchAddress("PHfracJet2", &PHfracJet2, &b_PHfracJet2);
  treeIn->SetBranchAddress("MUfracJet2", &MUfracJet2, &b_MUfracJet2);
  treeIn->SetBranchAddress("CHmultJet2", &CHmultJet2, &b_CHmultJet2);
  treeIn->SetBranchAddress("NEmultJet2", &NEmultJet2, &b_NEmultJet2);
  treeIn->SetBranchAddress("ptJet3", &ptJet3, &b_ptJet3);
  treeIn->SetBranchAddress("etaJet3", &etaJet3, &b_etaJet3);
  treeIn->SetBranchAddress("phiJet3", &phiJet3, &b_phiJet3);
  treeIn->SetBranchAddress("massJet3", &massJet3, &b_massJet3);
  treeIn->SetBranchAddress("indexJet3", &indexJet3, &b_indexJet3);
  treeIn->SetBranchAddress("NEMfracJet3", &NEMfracJet3, &b_NEMfracJet3);
  treeIn->SetBranchAddress("CEMfracJet3", &CEMfracJet3, &b_CEMfracJet3);
  treeIn->SetBranchAddress("ELfracJet3", &ELfracJet3, &b_ELfracJet3);
  treeIn->SetBranchAddress("CHfracJet3", &CHfracJet3, &b_CHfracJet3);
  treeIn->SetBranchAddress("NHfracJet3", &NHfracJet3, &b_NHfracJet3);
  treeIn->SetBranchAddress("PHfracJet3", &PHfracJet3, &b_PHfracJet3);
  treeIn->SetBranchAddress("MUfracJet3", &MUfracJet3, &b_MUfracJet3);
  treeIn->SetBranchAddress("CHmultJet3", &CHmultJet3, &b_CHmultJet3);
  treeIn->SetBranchAddress("NEmultJet3", &NEmultJet3, &b_NEmultJet3);
  treeIn->SetBranchAddress("ptJet4", &ptJet4, &b_ptJet4);
  treeIn->SetBranchAddress("etaJet4", &etaJet4, &b_etaJet4);
  treeIn->SetBranchAddress("phiJet4", &phiJet4, &b_phiJet4);
  treeIn->SetBranchAddress("massJet4", &massJet4, &b_massJet4);
  treeIn->SetBranchAddress("indexJet4", &indexJet4, &b_indexJet4);
  treeIn->SetBranchAddress("NEMfracJet4", &NEMfracJet4, &b_NEMfracJet4);
  treeIn->SetBranchAddress("CEMfracJet4", &CEMfracJet4, &b_CEMfracJet4);
  treeIn->SetBranchAddress("ELfracJet4", &ELfracJet4, &b_ELfracJet4);
  treeIn->SetBranchAddress("CHfracJet4", &CHfracJet4, &b_CHfracJet4);
  treeIn->SetBranchAddress("NHfracJet4", &NHfracJet4, &b_NHfracJet4);
  treeIn->SetBranchAddress("PHfracJet4", &PHfracJet4, &b_PHfracJet4);
  treeIn->SetBranchAddress("MUfracJet4", &MUfracJet4, &b_MUfracJet4);
  treeIn->SetBranchAddress("CHmultJet4", &CHmultJet4, &b_CHmultJet4);
  treeIn->SetBranchAddress("NEmultJet4", &NEmultJet4, &b_NEmultJet4);
  treeIn->SetBranchAddress("vhtruth", &vhtruth, &b_vhtruth);

  treeIn->SetBranchAddress("higgsVtxX", &higgsVtxX, &b_higgsVtxX);
  treeIn->SetBranchAddress("higgsVtxY", &higgsVtxY, &b_higgsVtxY);
  treeIn->SetBranchAddress("higgsVtxZ", &higgsVtxZ, &b_higgsVtxZ);
  treeIn->SetBranchAddress("vtxIndex", &vtxIndex, &b_vtxIndex);
  treeIn->SetBranchAddress("vtxX", &vtxX, &b_vtxX);
  treeIn->SetBranchAddress("vtxY", &vtxY, &b_vtxY);
  treeIn->SetBranchAddress("vtxZ", &vtxZ, &b_vtxZ);
  treeIn->SetBranchAddress("vtx0Z", &vtx0Z, &b_vtx0Z);
  treeIn->SetBranchAddress("genVtxX", &genVtxX, &b_genVtxX);
  treeIn->SetBranchAddress("genVtxY", &genVtxY, &b_genVtxY);
  treeIn->SetBranchAddress("genVtxZ", &genVtxZ, &b_genVtxZ);
  treeIn->SetBranchAddress("massCorrSmear", &massCorrSmear, &b_massCorrSmear);
  treeIn->SetBranchAddress("massCorrSmearUp", &massCorrSmearUp, &b_massCorrSmearUp);
  treeIn->SetBranchAddress("massCorrSmearDown", &massCorrSmearDown, &b_massCorrSmearDown);
  treeIn->SetBranchAddress("massCorrScale", &massCorrScale, &b_massCorrScale);
  treeIn->SetBranchAddress("massCorrScaleUp", &massCorrScaleUp, &b_massCorrScaleUp);
  treeIn->SetBranchAddress("massCorrScaleDown", &massCorrScaleDown, &b_massCorrScaleDown);
  treeIn->SetBranchAddress("massRaw", &massRaw, &b_massRaw);
  treeIn->SetBranchAddress("mva1", &mva1, &b_mva1);
  treeIn->SetBranchAddress("mva2", &mva2, &b_mva2);
  treeIn->SetBranchAddress("genZ", &genZ, &b_genZ);
  treeIn->SetBranchAddress("ptZ",  &ptZ,  &b_ptZ);
  treeIn->SetBranchAddress("etaZ", &etaZ, &b_etaZ);
  treeIn->SetBranchAddress("phiZ", &phiZ, &b_phiZ);
  treeIn->SetBranchAddress("BDTptgg", &BDTptgg, &b_BDTptgg);
  treeIn->SetBranchAddress("BDTmassRaw", &BDTmassRaw, &b_BDTmassRaw);
  treeIn->SetBranchAddress("BDTr91", &BDTr91, &b_BDTr91);
  treeIn->SetBranchAddress("BDTr92", &BDTr92, &b_BDTr92);
  treeIn->SetBranchAddress("BDTvtxZ", &BDTvtxZ, &b_BDTvtxZ);
  treeIn->SetBranchAddress("BDTindex", &BDTindex, &b_BDTindex);


};// end CardMaker::SetBranchAddresses

