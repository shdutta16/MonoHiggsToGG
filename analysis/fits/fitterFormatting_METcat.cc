#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TLorentzVector.h>
#include <iostream>
//#include "../../../DataFormats/Math/interface/deltaPhi.h"

using namespace std;

// fitterFormatting_METcat input arguements:
//   1st: whichDphi cuts to apply
//   2nd: highMET cut to apply
//   3rd: input directory
//   4th: output directory
//   5th: type (sig, bkg, data)
//   6th: prompt (for duplicate removal)
//   7th: input filename 
//   8th: sample name

void fitterFormatting(float whichDphi, float masscut, TString inDir, TString outDir, TString type, Int_t prompt, const char* filename, TString theSample) {
  cout << "Formatting " << inDir << filename << endl;
  cout << "Move to Pasquale's format for fit." << endl;
  if (whichDphi==0) cout << " Without dphi cuts" << endl;
  if (whichDphi==1) cout << " With dphi ggMET cut" << endl;
  if (whichDphi==2) cout << " With dphi jMET cut" << endl;
  if (whichDphi==3) cout << " With both dphi cuts" << endl;

  TFile *fileOrig = 0;
  TTree *treeOrig = 0;

  fileOrig = TFile::Open(inDir+filename);
  if ( fileOrig ) {
    fileOrig->cd();
    TString theTree = TString::Format("DiPhotonTree");
    cout << "Reading " << theTree << endl;
    treeOrig = (TTree*)fileOrig->Get(theTree);
  }
  else {
    cout << "File " << inDir << filename << " does not exist!" << endl;
    return;
  }

  fileOrig->cd();
  if (!treeOrig) {
    cout << "Tree does not exist!" << endl;
    return;
  }

  treeOrig->SetMakeClass(0);
  // get number of entries in original tree to loop over
  UInt_t nentriesOrig = treeOrig->GetEntries();

  // vector for new photon categories out
  vector<TString> thePhoCat;
  thePhoCat.push_back("all");
  thePhoCat.push_back("EB");
  thePhoCat.push_back("EE");
  UInt_t numPhoCat = thePhoCat.size();

  // setup for the met categories out
  // vector to store values of selection
  std::vector<Int_t> MetCut;
  MetCut.push_back(0);
  MetCut.push_back(masscut);

  //// vector to store how selection is applied to trees
  //std::vector<TString> MetCat;
  //MetCat.push_back(TString::Format("t1pfmet>=%d",MetCut[0]));			 	// met > MetCut0
  //MetCat.push_back(TString::Format("t1pfmet>=%d && t1pfmet<%d",MetCut[0],MetCut[1]));	// met [MetCut0,MetCut1] 
  //MetCat.push_back(TString::Format("t1pfmet>=%d && t1pfmet<%d",MetCut[1],MetCut[2])); 	// met [MetCut1,MetCut2]
  ////MetCat.push_back(TString::Format("t1pfmet>=%d && t1pfmet<%d",MetCut[2],MetCut[3]));   // met [MetCut2,MetCut3]
  //MetCat.push_back(TString::Format("t1pfmet>=%d",MetCut[2]));				// met > MetCut3

  //// To do in pTgg categoires:
  //MetCat.push_back(TString::Format("ptgg>=%d",MetCut[0]));
  //MetCat.push_back(TString::Format("ptgg>=%d && ptgg<%d",MetCut[0],MetCut[1]));
  //MetCat.push_back(TString::Format("ptgg>=%d && ptgg<%d",MetCut[1],MetCut[2]));
  //MetCat.push_back(TString::Format("ptgg>=%d",MetCut[1])); 
 

  // vector to store names for met cat out
  std::vector<TString> theMetCat;
  //theMetCat.push_back(TString::Format("met%d",MetCut[0]));
  //theMetCat.push_back(TString::Format("met%d-%d",MetCut[0],MetCut[1]));
  //theMetCat.push_back(TString::Format("met%d",MetCut[1]));
  theMetCat.push_back("met0");
  theMetCat.push_back(TString::Format("met0-%d",MetCut[1]));
  theMetCat.push_back(TString::Format("met%d",MetCut[1]));
  //theMetCat.push_back(TString::Format("met%d-%d",MetCut[1],MetCut[2]));
  //theMetCat.push_back(TString::Format("met%d",MetCut[2]));

  // To do in pTgg categories:
  //theMetCat.push_back(TString::Format("ptgg%d",MetCut[0]));
  //theMetCat.push_back(TString::Format("ptgg%d-%d",MetCut[0],MetCut[1]));
  //theMetCat.push_back(TString::Format("ptgg%d-%d",MetCut[1],MetCut[2]));
  //theMetCat.push_back(TString::Format("ptgg%d",MetCut[2]));
  UInt_t numMetCat = theMetCat.size();

  // make output file and new trees
  TString outputname = "Output_Data.root";
  TFile *fileNew = new TFile();
  if (type=="data"){ 
    cout << "OutputFile: " << outDir << "/" << outputname << endl;
    fileNew = TFile::Open(TString::Format("%s/%s",outDir.Data(),outputname.Data()),"RECREATE");
  }
  else{
    cout << "OutputFile: " << outDir << "/" << theSample << "_new.root" << endl;
    fileNew = TFile::Open(TString::Format("%s/%s_new.root",outDir.Data(),theSample.Data()),"RECREATE");
  }
  vector< vector<TTree*> > trees;
  trees.resize(numMetCat);
  for (UInt_t met=0; met < numMetCat; met++){ trees[met].resize(numPhoCat); }
  //vector<TDirectory*> newDir;
  //newDir.resize(numMetCat);  

  // make output structure of directories
  TString dir1 = "";
  TString dir2 = "trees";
  if (type=="data") dir1 = "cic"; // data
  else dir1 = "genmc";  // mc 
  TDirectory *TDir1 = fileNew->mkdir(dir1);
  TDir1->cd();
  TDirectory *TDir2 = TDir1->mkdir(dir2);
  TDir2->cd();
  
  // make tree names for data & mc
  TString treeName = "";
  if (type=="data") treeName = "Data_13TeV_";
  else treeName = TString::Format("%s_13TeV_",theSample.Data());

  // make a separate tree for each photon category
  TString fullTreeName = "";
  for (UInt_t met = 0; met<numMetCat; met++){
    for (UInt_t pho = 0; pho<numPhoCat; pho++){
      if (pho==0) fullTreeName = TString::Format("%s%s",treeName.Data(),theMetCat[met].Data());
      else        fullTreeName = TString::Format("%s%s%s",treeName.Data(),theMetCat[met].Data(),thePhoCat[pho].Data());
      TTree *treeNew = new TTree(fullTreeName,fullTreeName);
      trees[met][pho]=treeNew;
    }
  }

  //// make a tree for each photon category and each met category
  //trees.resize(numMetCat);
  //TString theNewDir = "";
  //for (UInt_t met = 0; met<numMetCat; met++){
  //  trees[met].resize(numPhoCat);
  //  theNewDir = TString::Format("metCat%d",met);// label cat in met by number
  //  //if (met==0) theNewDir = "met0";
  //  //else if (met==numMetCat-1) theNewDir = TString::Format("met%s",MetCut[met-1].Data());
  //  //else theNewDir=TString::Format("met%s_%s",MetCut[met-1].Data(),MetCut[met].Data());
  //  std::cout << theNewDir << std::endl;
  //  TDirectory *Dir = fileNew->mkdir(theNewDir);
  //  newDir[met] = Dir;
  //  newDir[met]->cd();
  //  for (UInt_t pho = 0; pho<numPhoCat; pho++){
  //    TString theNewTree = TString::Format("%s_%s_%s",type.Data(),theSample.Data(),thePhoCat[pho].Data());
  //    TTree *treeNew = new TTree(theNewTree,theNewTree); 
  //    trees[met][pho]=treeNew;
  //  }
  //}

  // original tree leaves
  Int_t   lumi		= 0.;
  Int_t   run		= 0.;
  UInt_t  event		= 0.;   
  Int_t	  nvtx		= 0.;
  Float_t weight	= 0.;
  Float_t rho		= 0.;
  Float_t mgg		= 0.;
  Float_t massCorrScale = 0.;
  Float_t eta1		= 0.;
  Float_t eta2		= 0.;
  Float_t r91		= 0.;
  Float_t r92		= 0.;
  Float_t t1pfmet	= 0.;
  Float_t t1pfmetCorr	= 0.;
  Float_t t1pfmetPhi	= 0.;
  Float_t pt1		= 0.;
  Float_t pt2		= 0.;
  Float_t ptgg		= 0.;
  Float_t chiso1 	= 0.;
  Float_t chiso2 	= 0.;
  Float_t phoiso1	= 0.;
  Float_t phoiso2	= 0.;
  Float_t neuiso1 	= 0.;
  Float_t neuiso2 	= 0.;
  Float_t phi1		= 0.;  
  Float_t phi2		= 0.;  
  Float_t hoe1		= 0.;
  Float_t hoe2		= 0.;
  Float_t sieie1 	= 0.;
  Float_t sieie2 	= 0.;
  Float_t sceta1	= 0.;
  Float_t sceta2	= 0.;
  Int_t   eleveto1	= 0.;
  Int_t   eleveto2	= 0.;
  Int_t   hltDiphoton30Mass95 = 0.;
  Int_t	  nEle		= 0.;
  Int_t	  nMuons	= 0.;
  Int_t	  nJets		= 0.;
  Int_t	  nLooseBjets	= 0.;
  Int_t	  nMediumBjets	= 0.;
  Int_t	  vhtruth	= 0.;
  Int_t	  metF_GV	= 0.;
  Int_t	  metF_HBHENoise = 0.;
  Int_t	  metF_HBHENoiseIso = 0.;
  Int_t	  metF_CSC	= 0.;
  Int_t	  metF_eeBadSC	= 0.;
  Int_t   genmatch1	= 0.; 
  Int_t   genmatch2	= 0.; 
  Float_t ptJetLead	= 0.; 
  Float_t ptJetSubLead	= 0.; 
  Float_t etaJetLead	= 0.; 
  Float_t etaJetSubLead	= 0.; 
  Float_t phiJetLead	= 0.; 
  Float_t phiJetSubLead	= 0.; 
  Float_t massJetLead	= 0.; 
  Float_t massJetSubLead= 0.; 
  Float_t ptJet3	= 0.;
  Float_t etaJet3	= 0.;
  Float_t phiJet3	= 0.;
  Float_t massJet3	= 0.;
  Float_t ptJet4	= 0.;
  Float_t etaJet4	= 0.;
  Float_t phiJet4	= 0.;
  Float_t massJet4	= 0.;
  Float_t t1pfmetSumEt	= 0.;
  Float_t phigg         = 0.;
  Float_t dphiggmet     = 0.;
  Float_t mindphijmet   = 0.;

  // branches from original tree
  TBranch *b_lumi;
  TBranch *b_run;
  TBranch *b_event;
  TBranch *b_nvtx;
  TBranch *b_rho;
  TBranch *b_weight;
  TBranch *b_mgg;
  TBranch *b_massCorrScale;
  TBranch *b_ptgg;
  TBranch *b_eta1;
  TBranch *b_eta2;
  TBranch *b_r91;
  TBranch *b_r92;
  TBranch *b_pt1;
  TBranch *b_pt2;
  TBranch *b_t1pfmet;
  TBranch *b_t1pfmetCorr;
  TBranch *b_t1pfmetPhi;
  TBranch *b_chiso1; 
  TBranch *b_chiso2;  
  TBranch *b_phoiso1; 
  TBranch *b_phoiso2;
  TBranch *b_neuiso1; 
  TBranch *b_neuiso2; 
  TBranch *b_phi1;	  
  TBranch *b_phi2;	  
  TBranch *b_hoe1;	  
  TBranch *b_hoe2;	  
  TBranch *b_sieie1;  
  TBranch *b_sieie2;
  TBranch *b_sceta1;
  TBranch *b_sceta2; 
  TBranch *b_eleveto1;
  TBranch *b_eleveto2;
  TBranch *b_hltDiphoton30Mass95;
  TBranch *b_nEle;   //!
  TBranch *b_nMuons;   //!
  TBranch *b_nJets;   //!
  TBranch *b_nLooseBjets;   //!
  TBranch *b_nMediumBjets;   //!
  TBranch *b_vhtruth;   //!
  TBranch *b_metF_GV;   //!
  TBranch *b_metF_HBHENoise;   //!
  TBranch *b_metF_HBHENoiseIso;   //!
  TBranch *b_metF_CSC;   //!
  TBranch *b_metF_eeBadSC;   //!
  TBranch *b_genmatch1; 
  TBranch *b_genmatch2;
  TBranch *b_ptJetLead; 
  TBranch *b_ptJetSubLead; 
  TBranch *b_etaJetLead; 
  TBranch *b_etaJetSubLead; 
  TBranch *b_phiJetLead; 
  TBranch *b_phiJetSubLead; 
  TBranch *b_massJetLead; 
  TBranch *b_massJetSubLead; 
  TBranch *b_ptJet3;
  TBranch *b_etaJet3;
  TBranch *b_phiJet3;
  TBranch *b_massJet3; 
  TBranch *b_ptJet4;
  TBranch *b_etaJet4;
  TBranch *b_phiJet4;
  TBranch *b_massJet4; 
  TBranch *b_t1pfmetSumEt;
  TBranch *b_phigg;
  TBranch *b_dphiggmet;
  TBranch *b_mindphijmet;

  // set branch addresses and branch pointers
  treeOrig->SetBranchAddress("lumi",		&lumi,		&b_lumi);
  treeOrig->SetBranchAddress("run",		&run,		&b_run);
  treeOrig->SetBranchAddress("event",		&event,		&b_event);
  treeOrig->SetBranchAddress("weight",		&weight,	&b_weight);
  treeOrig->SetBranchAddress("nvtx",		&nvtx,		&b_nvtx);
  treeOrig->SetBranchAddress("rho",		&rho,		&b_rho);
  treeOrig->SetBranchAddress("mgg", 		&mgg,		&b_mgg);
  treeOrig->SetBranchAddress("massCorrScale",	&massCorrScale,	&b_massCorrScale);
  treeOrig->SetBranchAddress("eta1",		&eta1,		&b_eta1); 
  treeOrig->SetBranchAddress("eta2",		&eta2,		&b_eta2); 
  treeOrig->SetBranchAddress("r91",		&r91,		&b_r91);
  treeOrig->SetBranchAddress("r92",		&r92,		&b_r92);
  treeOrig->SetBranchAddress("pt1",		&pt1,		&b_pt1);
  treeOrig->SetBranchAddress("pt2",		&pt2,		&b_pt2);
  treeOrig->SetBranchAddress("ptgg",		&ptgg,		&b_ptgg);
  treeOrig->SetBranchAddress("t1pfmet",		&t1pfmet,	&b_t1pfmet);
  treeOrig->SetBranchAddress("t1pfmetCorr",	&t1pfmetCorr,	&b_t1pfmetCorr);
  treeOrig->SetBranchAddress("t1pfmetPhi",	&t1pfmetPhi,	&b_t1pfmetPhi);
  treeOrig->SetBranchAddress("chiso1",  	&chiso1,  	&b_chiso1); 
  treeOrig->SetBranchAddress("chiso2",  	&chiso2,  	&b_chiso2);  
  treeOrig->SetBranchAddress("phoiso1", 	&phoiso1, 	&b_phoiso1); 
  treeOrig->SetBranchAddress("phoiso2", 	&phoiso2, 	&b_phoiso2);
  treeOrig->SetBranchAddress("neuiso1", 	&neuiso1, 	&b_neuiso1); 
  treeOrig->SetBranchAddress("neuiso2", 	&neuiso2, 	&b_neuiso2); 
  treeOrig->SetBranchAddress("phi1",    	&phi1,    	&b_phi1);    
  treeOrig->SetBranchAddress("phi2",    	&phi2,    	&b_phi2);    
  treeOrig->SetBranchAddress("hoe1",    	&hoe1,    	&b_hoe1);    
  treeOrig->SetBranchAddress("hoe2",    	&hoe2,    	&b_hoe2);    
  treeOrig->SetBranchAddress("sieie1",  	&sieie1,  	&b_sieie1);  
  treeOrig->SetBranchAddress("sieie2",  	&sieie2,  	&b_sieie2); 
  treeOrig->SetBranchAddress("sceta1",		&sceta1,	&b_sceta1);
  treeOrig->SetBranchAddress("sceta2",		&sceta2,	&b_sceta2);
  treeOrig->SetBranchAddress("eleveto1",	&eleveto1,	&b_eleveto1);
  treeOrig->SetBranchAddress("eleveto2",	&eleveto2,	&b_eleveto2);
  treeOrig->SetBranchAddress("hltDiphoton30Mass95",	&hltDiphoton30Mass95,	&b_hltDiphoton30Mass95);  
  treeOrig->SetBranchAddress("nEle", 		&nEle, 		&b_nEle);
  treeOrig->SetBranchAddress("nMuons", 		&nMuons, 	&b_nMuons);
  treeOrig->SetBranchAddress("nJets", 		&nJets, 	&b_nJets);
  treeOrig->SetBranchAddress("nLooseBjets", 	&nLooseBjets, 	&b_nLooseBjets);
  treeOrig->SetBranchAddress("nMediumBjets", 	&nMediumBjets, 	&b_nMediumBjets);
  treeOrig->SetBranchAddress("vhtruth", 	&vhtruth, 	&b_vhtruth);
  treeOrig->SetBranchAddress("metF_GV", 	&metF_GV, 	&b_metF_GV);
  treeOrig->SetBranchAddress("metF_HBHENoise", 	&metF_HBHENoise,&b_metF_HBHENoise);
  treeOrig->SetBranchAddress("metF_HBHENoiseIso",	&metF_HBHENoiseIso, 	&b_metF_HBHENoiseIso);
  treeOrig->SetBranchAddress("metF_CSC", 	&metF_CSC, 	&b_metF_CSC);
  treeOrig->SetBranchAddress("metF_eeBadSC", 	&metF_eeBadSC, 	&b_metF_eeBadSC);
  treeOrig->SetBranchAddress("genmatch1", 	&genmatch1, 	&b_genmatch1);  
  treeOrig->SetBranchAddress("genmatch2", 	&genmatch2, 	&b_genmatch2); 
  treeOrig->SetBranchAddress("ptJetLead", 	&ptJetLead, 	&b_ptJetLead);
  treeOrig->SetBranchAddress("etaJetLead", 	&etaJetLead, 	&b_etaJetLead);
  treeOrig->SetBranchAddress("phiJetLead", 	&phiJetLead, 	&b_phiJetLead);
  treeOrig->SetBranchAddress("massJetLead", 	&massJetLead, 	&b_massJetLead);
  treeOrig->SetBranchAddress("ptJetSubLead", 	&ptJetSubLead, 	&b_ptJetSubLead);
  treeOrig->SetBranchAddress("etaJetSubLead", 	&etaJetSubLead, &b_etaJetSubLead);
  treeOrig->SetBranchAddress("phiJetSubLead", 	&phiJetSubLead,	&b_phiJetSubLead);
  treeOrig->SetBranchAddress("massJetSubLead", 	&massJetSubLead,&b_massJetSubLead);
  treeOrig->SetBranchAddress("ptJet3", 		&ptJet3,	&b_ptJet3);
  treeOrig->SetBranchAddress("etaJet3", 	&etaJet3,	&b_etaJet3);
  treeOrig->SetBranchAddress("phiJet3", 	&phiJet3, 	&b_phiJet3);
  treeOrig->SetBranchAddress("massJet3",	&massJet3,	&b_massJet3);
  treeOrig->SetBranchAddress("ptJet4", 		&ptJet4,	&b_ptJet4);
  treeOrig->SetBranchAddress("etaJet4", 	&etaJet4,	&b_etaJet4);
  treeOrig->SetBranchAddress("phiJet4", 	&phiJet4, 	&b_phiJet4);
  treeOrig->SetBranchAddress("massJet4",	&massJet4,	&b_massJet4);
  treeOrig->SetBranchAddress("t1pfmetSumEt",	&t1pfmetSumEt,	&b_t1pfmetSumEt);
  treeOrig->SetBranchAddress("phigg",		&phigg,		&b_phigg);
  treeOrig->SetBranchAddress("dphiggmet",	&dphiggmet,	&b_dphiggmet);
  treeOrig->SetBranchAddress("mindphijmet",	&mindphijmet,	&b_mindphijmet);


  // new variables (needed if variable has diff name in new tree) 
  float mass;
  float leadPt, subleadPt;
  float leadEta, subleadEta;
  float leadScEta, subleadScEta;
  float leadPhoIso, subleadPhoIso;
  float leadChIso, subleadChIso;
  float leadNeutIso, subleadNeutIso; 
  float leadPhi, subleadPhi;
  float leadHoE, subleadHoE;
  float leadR9, subleadR9;
  float leadSigmaIeIe, subleadSigmaIeIe;
  int   leadPassEleVeto, subleadPassEleVeto;
  int   passHlt;
 
  // setup new trees
  vector< vector<TTree*> > theTreeNew;
  theTreeNew.resize(numMetCat);
  //theTreeNew.resize(numMetCat);
  for (UInt_t i=0; i<numMetCat;i++) {
    theTreeNew[i].resize(numPhoCat);
    for (UInt_t j=0; j<numPhoCat;j++) {
      theTreeNew[i][j] = trees[i][j];
      theTreeNew[i][j]->Branch("lumi",		&lumi,			"lumi/I");
      theTreeNew[i][j]->Branch("run",		&run,			"run/I");
      theTreeNew[i][j]->Branch("event",		&event,			"event/i");
      theTreeNew[i][j]->Branch("weight", 	&weight, 		"weight/F");
      theTreeNew[i][j]->Branch("rho",		&rho,			"rho/F");
      theTreeNew[i][j]->Branch("mass", 		&mass, 			"mass/F");
      theTreeNew[i][j]->Branch("nvtx",		&nvtx,			"nvtx/I");
      theTreeNew[i][j]->Branch("leadEta", 	&leadEta, 		"leadEta/F");
      theTreeNew[i][j]->Branch("subleadEta", 	&subleadEta, 		"subleadEta/F");
      theTreeNew[i][j]->Branch("leadPt",	&leadPt,		"leadPt/F");
      theTreeNew[i][j]->Branch("subleadPt",	&subleadPt,		"subleadPt/F");
      theTreeNew[i][j]->Branch("ptgg",		&ptgg,			"ptgg/F");
      theTreeNew[i][j]->Branch("t1pfmet",	&t1pfmet,		"t1pfmet/F");     
      theTreeNew[i][j]->Branch("t1pfmetCorr",	&t1pfmetCorr,		"t1pfmetCorr/F");     
      theTreeNew[i][j]->Branch("t1pfmetSumEt",	&t1pfmetSumEt,		"t1pfmetSumEt/F");     
      theTreeNew[i][j]->Branch("t1pfmetPhi",	&t1pfmetPhi,		"t1pfmetPhi/F");
      theTreeNew[i][j]->Branch("leadChIso",  	&leadChIso,  		"leadChIso/F"); 
      theTreeNew[i][j]->Branch("subleadChIso",  &subleadChIso, 		"subleadChIso/F");  
      theTreeNew[i][j]->Branch("leadPhoIso", 	&leadPhoIso, 		"leadPhoIso/F");  
      theTreeNew[i][j]->Branch("subleadPhoIso", &subleadPhoIso, 	"subleadPhoIso/F");
      theTreeNew[i][j]->Branch("leadNeutIso", 	&leadNeutIso,	 	"leadNeutIso/F"); 
      theTreeNew[i][j]->Branch("subleadNeutIso",&subleadNeutIso, 	"subleadNeutIso/F"); 
      theTreeNew[i][j]->Branch("leadPhi",    	&leadPhi,    		"leadPhi/F");    
      theTreeNew[i][j]->Branch("subleadPhi",   	&subleadPhi,    	"subleadPhi/F");    
      theTreeNew[i][j]->Branch("leadHoE",    	&leadHoE,    		"leadHoE/F");    
      theTreeNew[i][j]->Branch("subleadHoE",   	&subleadHoE,    	"subleadHoE/F");    
      theTreeNew[i][j]->Branch("leadR9",    	&leadR9,    		"leadR9/F");    
      theTreeNew[i][j]->Branch("subleadR9",   	&subleadR9, 	   	"subleadR9/F");    
      theTreeNew[i][j]->Branch("leadSigmaIeIe",	&leadSigmaIeIe,  	"leadSigmaIeIe/F");  
      theTreeNew[i][j]->Branch("subleadSigmaIeIe",	&subleadSigmaIeIe,  	"subleadSigmaIeIe/F");
      theTreeNew[i][j]->Branch("leadScEta",	&leadScEta,		"leadScEta/F"); 
      theTreeNew[i][j]->Branch("subleadScEta",	&subleadScEta,		"subleadScEta/F"); 
      theTreeNew[i][j]->Branch("leadPassEleVeto",	&leadPassEleVeto,	"leadPassEleVeto/I");
      theTreeNew[i][j]->Branch("subleadPassEleVeto",	&subleadPassEleVeto,	"subleadPassEleVeto/I");
      theTreeNew[i][j]->Branch("passHlt",		&passHlt,		"passHlt/I");
    }// end loop oever new trees in pho cat 
  }// end loop over new trees in met cat

  // make vectors to store if passing different met cuts
  vector<bool> passMet;
  passMet.resize(numMetCat);

  bool EB1, EB2;
  bool EE1, EE2;

  bool inEB, inEE;
  bool hiR9, loR9;

  TLorentzVector fLorenzVecPho1;
  TLorentzVector fLorenzVecPho2;
  TLorentzVector fLorenzVecGG;
  TLorentzVector fLorenzVecJet1;
  TLorentzVector fLorenzVecJet2;
  TLorentzVector fLorenzVecJet3;
  TLorentzVector fLorenzVecJet4;
  TLorentzVector correctedMet;

  // Commented by Shubham

  // std::cout << "Pickup MET Phi Correction" << std::endl; 
  // std::vector< Double_t > fMETCorrMC;
  // std::vector< Double_t > fMETCorrData;
  // pick up MC metCorr
  // TString metStudyMC = Form("%smetCorr_MC.root",inDir.Data());
  // TFile *fmetCorrMC = TFile::Open(metStudyMC.Data());
  // TH1D *MCmet = (TH1D*)fmetCorrMC->Get("metCorr");  
  // pick up Data metCorr
  // TString metStudyData = Form("%smetCorr_Data.root",inDir.Data());
  // TFile *fmetCorrDATA = TFile::Open(metStudyData.Data());
  // TH1D *DATAmet = (TH1D*)fmetCorrDATA->Get("metCorr");  
  // for (UInt_t i=0; i<4; i++){
  //  fMETCorrMC.push_back(MCmet->GetBinContent(i+1));
  //  fMETCorrData.push_back(DATAmet->GetBinContent(i+1));
  // }

  //********************************************************************

  //-------------------------------------------------------------
  // Loop over the entries in the ntuple
  //-------------------------------------------------------------

  for (UInt_t i=0; i<nentriesOrig; i++){
    treeOrig->GetEntry(i);

    //if (type!="data") weight *= 1.33;

    //// check that data passes METfilters
    //if (type=="data" && (metF_GV!=1 || metF_HBHENoise!=1 || metF_HBHENoiseIso!=1 /*|| metF_CSC!=1*/ || metF_eeBadSC!=1)) continue; 

    //// check that passes trigger
    //if (hltDiphoton30Mass95==0) continue;

    //// apply electron veto
    //if (eleveto1==0 || eleveto2==0) continue;

    //// remove duplicate events
    //if (prompt==1 && (genmatch1==1 && genmatch2==1)) continue;   // only PF and FF for gjets  
    //if (prompt==2 && (genmatch1==1 && genmatch2==1)) continue;   // only PF and FF for gjets  

    //if (mgg >= 100 && mgg <= 200 && pt1>0.5*mgg && pt2>0.25*mgg && ptgg >= 90){

      // split events by eta
      EB1 = false;
      EB2 = false;
      EE1 = false;
      EE2 = false;
      if (fabs(eta1)>1.566)  EE1=true;
      if (fabs(eta2)>1.566)  EE2=true;
      if (fabs(eta1)<1.4442) EB1=true;
      if (fabs(eta2)<1.4442) EB2=true; 
      inEE=false;
      inEB=false;
      if (EB1 && EB2) inEB=true;
      else if (EE1 || EE2) inEE=true;
      
      // split events by r9
      hiR9 = false;
      loR9 = false;
      if (r91 > 0.94 && r92 > 0.94) hiR9 = true;
      else if (r91 > 0.94 || r92 > 0.94) loR9 = true;

      //fLorenzVecPho1.SetPtEtaPhiM(pt1,eta1,phi1,0.);
      //fLorenzVecPho2.SetPtEtaPhiM(pt2,eta2,phi2,0.);
      //fLorenzVecGG = fLorenzVecPho1 + fLorenzVecPho2;
      //fLorenzVecJet1.SetPtEtaPhiM(ptJetLead,etaJetLead,phiJetLead,massJetLead);
      //fLorenzVecJet2.SetPtEtaPhiM(ptJetSubLead,etaJetSubLead,phiJetSubLead,massJetSubLead);
      //fLorenzVecJet3.SetPtEtaPhiM(ptJet3,etaJet3,phiJet3,massJet3);
      //fLorenzVecJet4.SetPtEtaPhiM(ptJet4,etaJet4,phiJet4,massJet4);

      //// met-phi correction
      //Double_t t1pfmetCorrX, t1pfmetCorrY, t1pfmetCorrE;

      //if (type=="data"){ 
      //  t1pfmetCorrX = t1pfmet*cos(t1pfmetPhi) - (fMETCorrData[0] + fMETCorrData[1]*t1pfmetSumEt);
      //  t1pfmetCorrY = t1pfmet*sin(t1pfmetPhi) - (fMETCorrData[2] + fMETCorrData[3]*t1pfmetSumEt);
      //}
      //else{
      //  t1pfmetCorrX = t1pfmet*cos(t1pfmetPhi) - (fMETCorrMC[0] + fMETCorrMC[1]*t1pfmetSumEt);
      //  t1pfmetCorrY = t1pfmet*sin(t1pfmetPhi) - (fMETCorrMC[2] + fMETCorrMC[3]*t1pfmetSumEt);
      //}
      //t1pfmetCorrE = sqrt(t1pfmetCorrX*t1pfmetCorrX + t1pfmetCorrY*t1pfmetCorrY);
      //correctedMet.SetPxPyPzE(t1pfmetCorrX,t1pfmetCorrY,0,t1pfmetCorrE);
      //Double_t t1pfmetPhiCorr = correctedMet.Phi(); 
      //Double_t t1pfmetCorr = correctedMet.Pt();

      //// check if passing deltaPhi(gg,MET) cut
      //Double_t dphi_ggMET = TMath::Abs(deltaPhi(fLorenzVecGG.Phi(),t1pfmetPhiCorr));
      //if (dphi_ggMET < 2.1) continue;

      //// look at deltaPhi(jet,MET)
      //Double_t min_dphi_JetMET = 10.;
      //Double_t max_dphi_JetMET = 0.;

      //if (nJets > 0){
      //  Double_t dphiJet1METmin = 10;
      //  Double_t dphiJet2METmin = 10;
      //  Double_t dphiJet3METmin = 10;
      //  Double_t dphiJet4METmin = 10;
      //  Double_t dphiJet1METmax = 0;
      //  Double_t dphiJet2METmax = 0;
      //  Double_t dphiJet3METmax = 0;
      //  Double_t dphiJet4METmax = 0;
      //  if ( ptJetLead > 50 ){
      //    dphiJet1METmin = TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetPhiCorr));
      //    dphiJet1METmax = TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetPhiCorr));
      //  }
      //  if ( ptJetSubLead > 50 ){
      //    dphiJet2METmin = TMath::Abs(deltaPhi(fLorenzVecJet2.Phi(),t1pfmetPhiCorr));
      //    dphiJet2METmax = TMath::Abs(deltaPhi(fLorenzVecJet2.Phi(),t1pfmetPhiCorr));
      //  }
      //  if ( ptJet3 > 50 ){
      //    dphiJet3METmin = TMath::Abs(deltaPhi(fLorenzVecJet3.Phi(),t1pfmetPhiCorr));
      //    dphiJet3METmax = TMath::Abs(deltaPhi(fLorenzVecJet3.Phi(),t1pfmetPhiCorr));
      //  }
      //  if ( ptJet4 > 50 ){
      //    dphiJet4METmin = TMath::Abs(deltaPhi(fLorenzVecJet4.Phi(),t1pfmetPhiCorr));
      //    dphiJet4METmax = TMath::Abs(deltaPhi(fLorenzVecJet4.Phi(),t1pfmetPhiCorr));
      //  }

      //  // find the min_dphi_JetMET 
      //  if (dphiJet1METmin < min_dphi_JetMET) min_dphi_JetMET = dphiJet1METmin;	   
      //  if (dphiJet2METmin < min_dphi_JetMET) min_dphi_JetMET = dphiJet2METmin;	   
      //  if (dphiJet3METmin < min_dphi_JetMET) min_dphi_JetMET = dphiJet3METmin;	   
      //  if (dphiJet4METmin < min_dphi_JetMET) min_dphi_JetMET = dphiJet4METmin;	   

      //  // find the max_dphi_JetMET 
      //  if (dphiJet1METmax > max_dphi_JetMET) max_dphi_JetMET = dphiJet1METmax;	   
      //  if (dphiJet2METmax > max_dphi_JetMET) max_dphi_JetMET = dphiJet2METmax;	   
      //  if (dphiJet3METmax > max_dphi_JetMET) max_dphi_JetMET = dphiJet3METmax;	   
      //  if (dphiJet4METmax > max_dphi_JetMET) max_dphi_JetMET = dphiJet4METmax;	   
      //}
      ////if (min_dphi_JetMET < 0.5 || max_dphi_JetMET > 2.7) continue; 
      //if (min_dphi_JetMET < 0.5) continue; 

      //if (nMuons > 0 || nEle > 1) continue;
      if (whichDphi==1 && dphiggmet < 2.1)                        continue;
      if (whichDphi==2 && mindphijmet < 0.5)                      continue;
      if (whichDphi==3 && (dphiggmet < 2.1 || mindphijmet < 0.5)) continue;

      for (UInt_t met=0; met<numMetCat; met++){
        //std::cout << "i = " << met << " cut = " << MetCut[met] << std::endl;
        //if (met==0) std::cout << "cut = " << MetCut[met] << std::endl;
        //else if (met==numMetCat-1) std::cout << " cut = " << MetCut[met-1]  << std::endl;
        //else std::cout << "cut > " << MetCut[met-1] << " & cut < " << MetCut[met] << std::endl;
	passMet[met]=false;
        if (met==0){               if (t1pfmetCorr>=MetCut[met]) passMet[met]=true; }
        else if (met==numMetCat-1){if (t1pfmetCorr>=MetCut[met-1]) passMet[met]=true; }
        else{if (t1pfmetCorr>=MetCut[met-1] && t1pfmetCorr<MetCut[met]) passMet[met]=true; }
        //if (met==0 && ptgg>=MetCut[met]) passMet[met]=true;
        //else if (met==numMetCat-1 && ptgg>=MetCut[met-1]) passMet[met]=true;
        //else{
        //  if (ptgg >= MetCut[met-1] && ptgg<MetCut[met]) passMet[met]=true;
        //}

      }// end met loop

      // set the new variables (i.e. renamed from old tree)
      mass = mgg;
      //mass = massCorrScale;
      leadPt = pt1;			subleadPt = pt2;
      leadEta = eta1;			subleadEta = eta2;  
      leadPhi = phi1;			subleadPhi = phi2;
      leadPhoIso = phoiso1;		subleadPhoIso = phoiso2;
      leadNeutIso = neuiso1;		subleadNeutIso = neuiso2;
      leadChIso = chiso1;		subleadChIso = chiso2;
      leadSigmaIeIe = sieie1;		subleadSigmaIeIe = sieie2;
      leadHoE = hoe1;			subleadHoE = hoe2;
      leadPassEleVeto = eleveto1;	subleadPassEleVeto = eleveto2;
      leadScEta = sceta1;		subleadScEta = sceta2;
      leadR9 = r91;			subleadR9 = r92;
      passHlt = hltDiphoton30Mass95;
      
      // fill the trees for events in the different categories
      for (UInt_t met=0; met<numMetCat; met++){
        //newDir[met]->cd();
        if (passMet[met]){
            TDir1->cd();
            TDir2->cd();    
            theTreeNew[met][0]->Fill();//noPhoSel
            if (inEB) theTreeNew[met][1]->Fill();
            else      theTreeNew[met][2]->Fill();
            //if (met==0)       theTreeNew[met][0]->Fill();// pho=0 noPhoSel 
	    //if (inEB && hiR9) theTreeNew[met][1]->Fill();// pho=1 EBHighR9
	    //if (inEB && loR9) theTreeNew[met][2]->Fill();// pho=2 EBLowR9
	    //if (inEE && hiR9) theTreeNew[met][3]->Fill();// pho=3 EEHighR9
	    //if (inEE && loR9) theTreeNew[met][4]->Fill();// pho=4 EELowR9
         }
      }//end filling loop

    //}// end mgg presel 
  }// end loop over entries in orig tree

  // write new file
  fileNew->ls();
  fileNew->cd();
  for (UInt_t met=0; met<numMetCat; met++){
    for (UInt_t pho=0; pho<numPhoCat; pho++){
      TDir1->cd();
      TDir2->cd();
      theTreeNew[met][pho]->Write();
    }
  }
  
  // close files
  fileNew->Close();
  fileOrig->cd();
  fileOrig->Close();

}// end fitterFormatting
