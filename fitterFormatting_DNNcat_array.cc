#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TLorentzVector.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// fitterFormatting_METcat input arguements:
//   1st: DNNbins
//   2nd: input directory
//   3rd: output directory
//   4th: type (sig, bkg, data)
//   5th: prompt (for duplicate removal)
//   6th: input filename
//   7th: sample name


string dotTop( TString f_str )
{
    std::string str = std::string(f_str);

    while( str.find(".")!=std::string::npos )
    {
        str.replace( str.find("."), 1, "p" );
    }

    return str;
}


string toString( double num, int precision=2)
{

  ostringstream ss_num;
  ss_num << num;

  if( precision >= 0 )
  {
      ss_num.str(std::string());
      ss_num << std::fixed << std::setprecision(precision) << num;
  }

  return ss_num.str();

}



std::vector<double> strToArr(std::string);

void fitterFormatting( std::string dnnBins,
                       TString inDir,
                       TString outDir,
                       TString type,
                       Int_t prompt,
                       const char* filename,
                       TString theSample)
{

    cout << "\n\nFormatting " << inDir << filename << endl;


    TFile* fileOrig = nullptr;
    TTree* treeOrig = nullptr;

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

    if( numPhoCat==0 ) std::cout << "ERROR!!! numPhoCat is size 0." << std::endl;

    // Setup for the DNN bins out

    // vector to store values of selection
    std::vector<double> v_dnnBins = strToArr( dnnBins );

//    std::vector<double> v_dnnBins{0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.90,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.990,0.991,0.992,0.993,0.994,0.995,0.996,0.997,0.998,0.999,1.0};
//    std::vector<double> v_dnnBins{0.8,0.90,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.990,0.991,0.992,0.993,0.994,0.995,0.996,0.997,0.998,0.999,1.0};
//    std::vector<double> v_dnnBins{0.90,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.990,0.991,0.992,0.993,0.994,0.995,0.996,0.997,0.998,0.999,1.0};

    // vector to store names for dnn bin out
    std::vector<TString> v_str_dnnBins;

//    v_str_dnnBins.push_back( dotTop(TString::Format("dnnBins_minus99_%s", (toString(v_dnnBins[0],-1)).c_str())) ); // Uncomment to start binning from -99-0.1 16/04/2023
    for( UInt_t i=1; i<v_dnnBins.size(); i++ )
    {
        v_str_dnnBins.push_back( dotTop(TString::Format("dnnBins_%s_%s", (toString(v_dnnBins[i-1],-1)).c_str(), (toString(v_dnnBins[i],-1)).c_str()) ) );
    }    
//    v_str_dnnBins.push_back( dotTop(TString::Format("dnnBins_%s_99", (toString(v_dnnBins[v_dnnBins.size()-1],-1)).c_str()) ) );

    UInt_t n_dnnBins = v_str_dnnBins.size();

    if( v_str_dnnBins.size()==0 ) std::cout << "ERROR!!! v_str_dnnBins is size 0." << std::endl;



    // make output file and new trees
    TString outputname = "Output_Data.root";
    TFile* fileNew = new TFile();
    if (type=="data")
    {
        cout << "OutputFile: " << outDir << "/" << theSample << "_new.root" << endl;
        fileNew = TFile::Open(TString::Format("%s/%s_new.root",outDir.Data(),theSample.Data()),"RECREATE");
    }

    else
    {
        cout << "OutputFile: " << outDir << "/" << theSample << "_new.root" << endl;
        fileNew = TFile::Open(TString::Format("%s/%s_new.root",outDir.Data(),theSample.Data()),"RECREATE");
    }
    vector< vector<TTree*> > trees;
    trees.resize(n_dnnBins);

    for(UInt_t i=0; i < n_dnnBins; i++){ trees[i].resize(numPhoCat); }

    // make output structure of directories
    TString dir1 = "";
    TString dir2 = "trees";
    if (type=="data") dir1 = "cic"; // data
    else dir1 = "genmc";            // mc
    TDirectory* TDir1 = fileNew->mkdir(dir1);
    TDir1->cd();
    TDirectory* TDir2 = TDir1->mkdir(dir2);
    TDir2->cd();


    // make tree names for data & mc
    TString treeName = "";
    // if (type=="data") treeName = "Data_13TeV_";    Commented by Shubham 26/03/2023
    if (type=="data") treeName = TString::Format("%s_13TeV_",theSample.Data());
    else treeName = TString::Format("%s_13TeV_",theSample.Data());


    // make a separate tree for each photon category
    TString fullTreeName = "";
    for(UInt_t iDNN = 0; iDNN < n_dnnBins; iDNN++)
    {
        for (UInt_t iPho = 0; iPho < numPhoCat; iPho++)
        {
            if (iPho==0) fullTreeName = TString::Format("%s%s",   treeName.Data(), v_str_dnnBins[iDNN].Data() );
            else         fullTreeName = TString::Format("%s%s%s", treeName.Data(), v_str_dnnBins[iDNN].Data(), thePhoCat[iPho].Data() );
            TTree *treeNew = new TTree(fullTreeName,fullTreeName);
            trees[iDNN][iPho] = treeNew;
        }
    }


    // original tree leaves
    Int_t   lumi        = 0.;
    Int_t   run		= 0.;
    Long64_t  event     = 0.;
    Int_t	  nvtx	= 0.;
    Float_t weight	= 0.;
    Float_t rho		= 0.;
    Float_t mgg		= 0.;
    Float_t massCorrScale = 0.;
    Float_t eta1        = 0.;
    Float_t eta2	= 0.;
    Float_t r91		= 0.;
    Float_t r92		= 0.;
    Float_t t1pfmet	= 0.;
    Float_t t1pfmetCorr	= 0.;
    Float_t t1pfmetPhi	= 0.;
    Float_t pt1		= 0.;
    Float_t pt2		= 0.;
    Float_t ptgg	= 0.;
    Float_t chiso1 	= 0.;
    Float_t chiso2 	= 0.;
    Float_t phoiso1	= 0.;
    Float_t phoiso2	= 0.;
    Float_t neuiso1 	= 0.;
    Float_t neuiso2 	= 0.;
    Float_t phi1	= 0.;
    Float_t phi2	= 0.;
    Float_t hoe1	= 0.;
    Float_t hoe2	= 0.;
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
    Float_t dnnScore      = 0.;

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
    TBranch *b_dnnScore;

    // set branch addresses and branch pointers
    treeOrig->SetBranchAddress("lumi",		&lumi,		&b_lumi);
    treeOrig->SetBranchAddress("run",		&run,		&b_run);
    treeOrig->SetBranchAddress("event",		&event,		&b_event);
    treeOrig->SetBranchAddress("weight",        &weight,	&b_weight);
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
    treeOrig->SetBranchAddress("t1pfmet",	&t1pfmet,	&b_t1pfmet);
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
    treeOrig->SetBranchAddress("sceta1",	&sceta1,	&b_sceta1);
    treeOrig->SetBranchAddress("sceta2",	&sceta2,	&b_sceta2);
    treeOrig->SetBranchAddress("eleveto1",	&eleveto1,	&b_eleveto1);
    treeOrig->SetBranchAddress("eleveto2",	&eleveto2,	&b_eleveto2);
    treeOrig->SetBranchAddress("hltDiphoton30Mass95",	&hltDiphoton30Mass95,	&b_hltDiphoton30Mass95);
    treeOrig->SetBranchAddress("nEle", 		&nEle, 		&b_nEle);
    treeOrig->SetBranchAddress("nMuons", 	&nMuons, 	&b_nMuons);
    treeOrig->SetBranchAddress("nJets", 	&nJets, 	&b_nJets);
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
    treeOrig->SetBranchAddress("DNNscore",      &dnnScore,      &b_dnnScore);


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
    theTreeNew.resize(n_dnnBins);

    for( UInt_t i=0; i<n_dnnBins; i++ )
    {
        theTreeNew[i].resize(numPhoCat);

        for (UInt_t j=0; j<numPhoCat;j++)
        {
            theTreeNew[i][j] = trees[i][j];
            theTreeNew[i][j]->Branch("lumi",		&lumi,			"lumi/I");
            theTreeNew[i][j]->Branch("run",		&run,			"run/I");
            theTreeNew[i][j]->Branch("event",		&event,			"event/i");
            theTreeNew[i][j]->Branch("weight", 	        &weight, 		"weight/F");
            theTreeNew[i][j]->Branch("rho",		&rho,			"rho/F");
            theTreeNew[i][j]->Branch("mass", 		&mass, 			"mass/F");
            theTreeNew[i][j]->Branch("nvtx",		&nvtx,			"nvtx/I");
            theTreeNew[i][j]->Branch("leadEta", 	&leadEta, 		"leadEta/F");
            theTreeNew[i][j]->Branch("subleadEta", 	&subleadEta, 		"subleadEta/F");
            theTreeNew[i][j]->Branch("leadPt",	        &leadPt,		"leadPt/F");
            theTreeNew[i][j]->Branch("subleadPt",	&subleadPt,		"subleadPt/F");
            theTreeNew[i][j]->Branch("ptgg",		&ptgg,			"ptgg/F");
            theTreeNew[i][j]->Branch("t1pfmet",	        &t1pfmet,		"t1pfmet/F");
            theTreeNew[i][j]->Branch("t1pfmetCorr",	&t1pfmetCorr,		"t1pfmetCorr/F");
            theTreeNew[i][j]->Branch("t1pfmetSumEt",	&t1pfmetSumEt,		"t1pfmetSumEt/F");
            theTreeNew[i][j]->Branch("t1pfmetPhi",	&t1pfmetPhi,		"t1pfmetPhi/F");
            theTreeNew[i][j]->Branch("leadChIso",  	&leadChIso,  		"leadChIso/F");
            theTreeNew[i][j]->Branch("subleadChIso",    &subleadChIso, 		"subleadChIso/F");
            theTreeNew[i][j]->Branch("leadPhoIso", 	&leadPhoIso, 		"leadPhoIso/F");
            theTreeNew[i][j]->Branch("subleadPhoIso",   &subleadPhoIso, 	"subleadPhoIso/F");
            theTreeNew[i][j]->Branch("leadNeutIso", 	&leadNeutIso,	 	"leadNeutIso/F");
            theTreeNew[i][j]->Branch("subleadNeutIso",  &subleadNeutIso, 	"subleadNeutIso/F");
            theTreeNew[i][j]->Branch("leadPhi",    	&leadPhi,    		"leadPhi/F");
            theTreeNew[i][j]->Branch("subleadPhi",   	&subleadPhi,    	"subleadPhi/F");
            theTreeNew[i][j]->Branch("leadHoE",    	&leadHoE,    		"leadHoE/F");
            theTreeNew[i][j]->Branch("subleadHoE",   	&subleadHoE,    	"subleadHoE/F");
            theTreeNew[i][j]->Branch("leadR9",    	&leadR9,    		"leadR9/F");
            theTreeNew[i][j]->Branch("subleadR9",   	&subleadR9, 	   	"subleadR9/F");
            theTreeNew[i][j]->Branch("leadSigmaIeIe",	&leadSigmaIeIe,  	"leadSigmaIeIe/F");
            theTreeNew[i][j]->Branch("subleadSigmaIeIe",   &subleadSigmaIeIe,  	"subleadSigmaIeIe/F");
            theTreeNew[i][j]->Branch("leadScEta",	&leadScEta,		"leadScEta/F");
            theTreeNew[i][j]->Branch("subleadScEta",	&subleadScEta,		"subleadScEta/F");
            theTreeNew[i][j]->Branch("leadPassEleVeto",	&leadPassEleVeto,	"leadPassEleVeto/I");
            theTreeNew[i][j]->Branch("subleadPassEleVeto", &subleadPassEleVeto,	"subleadPassEleVeto/I");
            theTreeNew[i][j]->Branch("passHlt",		&passHlt,		"passHlt/I");
            theTreeNew[i][j]->Branch("DNNscore",        &dnnScore,              "dnnScore/F");
        }// end loop over new trees in pho cat
    }// end loop over new trees in dnn bins


    // make vectors to store if passing different DNN score cuts
    vector<bool> passDNN;
    passDNN.resize(n_dnnBins);


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


    //-------------------------------------------------------------
    // Loop over the entries in the ntuple
    //-------------------------------------------------------------

    int count=0;
    long int dnnCount[n_dnnBins];

    for( int iDNN=0; iDNN<n_dnnBins; iDNN++ ){
        dnnCount[iDNN] = 0;
    }


    for (UInt_t i=0; i<nentriesOrig; i++)
    {
        treeOrig->GetEntry(i);

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


        for( UInt_t iDNN=0; iDNN < n_dnnBins; iDNN++ )
        {
            if(count==0) std::cout << "i = " << iDNN << " cut = " << v_str_dnnBins[iDNN] << std::endl;

            passDNN[iDNN]  = false;

//************************************************************************************************************
//            // For DNN score binning starting from -99
//            // ***************************************

//            if( iDNN==0 )
//            {
//                if( dnnScore <= v_dnnBins[iDNN] )
//                {
//                    passDNN[iDNN] = true;
////                    std::cout << "iDNN = " << iDNN << "  "
////                              << "DNN bin = " << v_dnnBins[iDNN] << std::endl;
//                }

//            }

//            else if( iDNN == n_dnnBins-1 )
//            {
//                if( dnnScore >= v_dnnBins[iDNN-1] )
//                {
//                    passDNN[iDNN] = true;
////                    std::cout << "iDNN = " << iDNN << "  "
////                              << "DNN bin = " << v_dnnBins[iDNN] << std::endl;
//                }

//            }

//            else
//            {
//                if( dnnScore >= v_dnnBins[iDNN-1] && dnnScore < v_dnnBins[iDNN] )
//                {
//                    passDNN[iDNN] = true;

//                }
//            }
//***********************************************************************************************************



//***********************************************************************************************************
             // For DNN score binning NOT starting from -99
             // ********************************************
            if( iDNN == n_dnnBins-1 )
            {
                if( dnnScore >= v_dnnBins[iDNN-1] )
                {
                    passDNN[iDNN] = true;
//                    std::cout << "iDNN = " << iDNN << "  "
//                              << "DNN bin = " << v_dnnBins[iDNN] << std::endl;
                }

            }

            else
            {
                if( dnnScore >= v_dnnBins[iDNN] && dnnScore < v_dnnBins[iDNN+1] )
                {
                    passDNN[iDNN] = true;

                }
            }
//************************************************************************************************************


        }// end dnn loop
        count++;

        // set the new variables (i.e. renamed from old tree)
        mass = mgg;
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


        // Fill the trees for events in the different DNN score categories
        for( UInt_t iDNN=0; iDNN < n_dnnBins; iDNN++ )
        {
            if( passDNN[iDNN] )
            {
                TDir1->cd();
                TDir2->cd();
                theTreeNew[iDNN][0]->Fill();   // noPhoSel
                dnnCount[iDNN]++;
                if (inEB) theTreeNew[iDNN][1]->Fill();
                else      theTreeNew[iDNN][2]->Fill();

            }
        }//end filling loop

    }// end loop over entries in orig tree



    // write new file
    fileNew->ls();
    fileNew->cd();

    for( UInt_t iDNN=0; iDNN < n_dnnBins; iDNN++ )
    {
        for( UInt_t iPho=0; iPho < numPhoCat; iPho++)
        {
            TDir1->cd();
            TDir2->cd();
            theTreeNew[iDNN][iPho]->Write();
        }
    }

    // close files
    fileNew->Close();
    fileOrig->cd();
    fileOrig->Close();

    for( int iDNN=0; iDNN<n_dnnBins; iDNN++ ){
        std::cout << "DNNbin " << iDNN << ": "
                  << (double)dnnCount[iDNN]/(double)nentriesOrig*100. << "%" << std::endl;
    }


}// end fitterFormatting




std::vector<double> strToArr(std::string inputStr)
{

  double num;
  
  std::stringstream ss_inputStr(inputStr);
  std::vector<double> v_num;



  while( ss_inputStr >> num )
  {
      v_num.push_back(num);

      if( ss_inputStr.peek()==',' )
          ss_inputStr.ignore();
  }


  std::cout << "Bin edges: ";
  for(auto& inum : v_num)
  {
      std::cout << inum << "  ";
  }
  std::cout << std::endl;


  return v_num;

}
