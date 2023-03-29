//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Feb 15 10:42:53 2016 by ROOT version 6.02/05
// from TTree DiPhotonTree/tree with 2 photon selection
// found on file: diPhotons.root
//////////////////////////////////////////////////////////

#ifndef METCorr2016_h
#define METCorr2016_h

#include "Style.hh"
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TLorentzVector.h"

// Header file for the classes stored in the TTree if any.

class METCorr2016 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   //Float_t         xsecWgt;
   Float_t         weight;
   //Float_t         mggNominal;
   //Float_t         mggGen;
   Int_t           run;
   Int_t           event;
   Int_t           lumi;
   Int_t           nvtx;
   Float_t         rho;
   Int_t           sampleID;
   Float_t         totXsec;
   Float_t         pu_weight;
   Float_t         pu_n;
   Float_t         sumDataset;
   Float_t         perEveW;
   Float_t         pfmet;
   Float_t         pfmetPhi;
   Float_t         pfmetSumEt;
   Float_t         t1pfmet;
   Float_t         t1p2pfmet;
   Float_t         t1pfmetJetEnUp;
   Float_t         t1pfmetJetEnDown;
   Float_t         t1pfmetJetResUp;
   Float_t         t1pfmetJetResDown;
   Float_t         t1pfmetMuonEnUp;
   Float_t         t1pfmetMuonEnDown;
   Float_t         t1pfmetElectronEnUp;
   Float_t         t1pfmetElectronEnDown;
   Float_t         t1pfmetTauEnUp;
   Float_t         t1pfmetTauEnDown;
   Float_t         t1pfmetPhotonEnUp;
   Float_t         t1pfmetPhotonEnDown;
   Float_t         t1pfmetUnclusteredEnUp;
   Float_t         t1pfmetUnclusteredEnDown;
   Float_t         t1pfmetPhi;
   Float_t         t1pfmetSumEt;
   Float_t         calomet;
   Float_t         calometPhi;
   Float_t         calometSumEt;
   Float_t         ptgg;
   Float_t         mgg;
   Int_t           eventClass;
   Float_t         pt1;
   Float_t         ptOverM1;
   Float_t         eta1;
   Float_t         phi1;
   Float_t         sceta1;
   Float_t         r91;
   Float_t         sieie1;
   Float_t         hoe1;
   Float_t         scRawEne1;
   Float_t         chiso1;
   Float_t         phoiso1;
   Float_t         neuiso1;
   Float_t         pt2;
   Float_t         ptOverM2;
   Float_t         eta2;
   Float_t         phi2;
   Float_t         sceta2;
   Float_t         r92;
   Float_t         sieie2;
   Float_t         hoe2;
   Float_t         scRawEne2;
   Float_t         chiso2;
   Float_t         phoiso2;
   Float_t         neuiso2;
   Int_t           eleveto1;
   Int_t           eleveto2;
   Int_t           presel1;
   Int_t           presel2;
   Int_t           sel1;
   Int_t           sel2;
   Int_t           tightsel1;
   Int_t           tightsel2;
   Int_t           genmatch1;
   Int_t           genmatch2;
   Float_t         geniso1;
   Float_t         geniso2;
   Int_t           vtxIndex;
   Float_t         vtxX;
   Float_t         vtxY;
   Float_t         vtxZ;
   Float_t         genVtxX;
   Float_t         genVtxY;
   Float_t         genVtxZ;
   Int_t           passCHiso1;
   Int_t           passCHiso2;
   Int_t           passNHiso1;
   Int_t           passNHiso2;
   Int_t           passPHiso1;
   Int_t           passPHiso2;
   Int_t           passSieie1;
   Int_t           passSieie2;
   Int_t           passHoe1;
   Int_t           passHoe2;
   Int_t           passLooseCHiso1;
   Int_t           passLooseCHiso2;
   Int_t           passLooseNHiso1;
   Int_t           passLooseNHiso2;
   Int_t           passLoosePHiso1;
   Int_t           passLoosePHiso2;
   Int_t           passLooseSieie1;
   Int_t           passLooseSieie2;
   Int_t           passLooseHoe1;
   Int_t           passLooseHoe2;
   Int_t           passTightCHiso1;
   Int_t           passTightCHiso2;
   Int_t           passTightNHiso1;
   Int_t           passTightNHiso2;
   Int_t           passTightPHiso1;
   Int_t           passTightPHiso2;
   Int_t           passTightSieie1;
   Int_t           passTightSieie2;
   Int_t           passTightHoe1;
   Int_t           passTightHoe2;
   Int_t           hltPhoton26Photon16Mass60;
   Int_t           hltPhoton36Photon22Mass15;
   Int_t           hltPhoton42Photon25Mass15;
   Int_t           hltDiphoton30Mass95;
   Int_t           hltDiphoton30Mass70;
   Int_t           hltDiphoton30Mass55;
   Int_t           hltDiphoton30Mass55PV;
   Int_t           hltDiphoton30Mass55EB;
   Int_t           nEle;
   Int_t           nMuons;
   Int_t           nJets;
   Int_t           nLooseBjets;
   Int_t           nMediumBjets;
   Int_t           vhtruth;
   Int_t           metF_GV;
   Int_t           metF_HBHENoise;
   Int_t           metF_HBHENoiseIso;
   Int_t           metF_CSC;
   Int_t           metF_eeBadSC;
   Float_t         higgsVtxX;
   Float_t         higgsVtxY;
   Float_t         higgsVtxZ;
   Float_t         massCorrSmear;
   Float_t         massCorrSmearUp;
   Float_t         massCorrSmearDown;
   Float_t         massCorrScale;
   Float_t         massCorrScaleUp;
   Float_t         massCorrScaleDown;
   Float_t         massRaw;
   Float_t         mva1;
   Float_t         mva2;
   Int_t           genZ;
   Float_t         ptZ;
   Float_t         etaZ;
   Float_t         phiZ;

   // List of branches
   //TBranch        *b_xsecWgt;   //!
   TBranch        *b_weight;   //!
   //TBranch        *b_mggNominal;   //!
   //TBranch        *b_mggGen;   //!
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_nvtx;   //!
   TBranch        *b_rho;   //!
   TBranch        *b_sampleID;   //!
   TBranch        *b_totXsec;   //!
   TBranch        *b_pu_weight;   //!
   TBranch        *b_pu_n;   //!
   TBranch        *b_sumDataset;   //!
   TBranch        *b_perEveW;   //!
   TBranch        *b_pfmet;   //!
   TBranch        *b_pfmetPhi;   //!
   TBranch        *b_pfmetSumEt;   //!
   TBranch        *b_t1pfmet;   //!
   TBranch        *b_b_t1p2pfmet;   //!
   TBranch        *b_b_t1pfmetJetEnUp;   //!
   TBranch        *b_b_t1pfmetJetEnDown;   //!
   TBranch        *b_b_t1pfmetJetResUp;   //!
   TBranch        *b_b_t1pfmetJetResDown;   //!
   TBranch        *b_b_t1pfmetMuonEnUp;   //!
   TBranch        *b_b_t1pfmetMuonEnDown;   //!
   TBranch        *b_b_t1pfmetElectronEnUp;   //!
   TBranch        *b_b_t1pfmetElectronEnDown;   //!
   TBranch        *b_b_t1pfmetTauEnUp;   //!
   TBranch        *b_b_t1pfmetTauEnDown;   //!
   TBranch        *b_b_t1pfmetPhotonEnUp;   //!
   TBranch        *b_b_t1pfmetPhotonEnDown;   //!
   TBranch        *b_b_t1pfmetUnclusteredEnUp;   //!
   TBranch        *b_b_t1pfmetUnclusteredEnDown;   //!
   TBranch        *b_t1pfmetPhi;   //!
   TBranch        *b_t1pfmetSumEt;   //!
   TBranch        *b_calomet;   //!
   TBranch        *b_calometPhi;   //!
   TBranch        *b_calometSumEt;   //!
   TBranch        *b_ptgg;   //!
   TBranch        *b_mgg;   //!
   TBranch        *b_eventClass;   //!
   TBranch        *b_pt1;   //!
   TBranch        *b_ptOverM1;   //!
   TBranch        *b_eta1;   //!
   TBranch        *b_phi1;   //!
   TBranch        *b_sceta1;   //!
   TBranch        *b_r91;   //!
   TBranch        *b_sieie1;   //!
   TBranch        *b_hoe1;   //!
   TBranch        *b_scRawEne1;   //!
   TBranch        *b_chiso1;   //!
   TBranch        *b_phoiso1;   //!
   TBranch        *b_neuiso1;   //!
   TBranch        *b_pt2;   //!
   TBranch        *b_ptOverM2;   //!
   TBranch        *b_eta2;   //!
   TBranch        *b_phi2;   //!
   TBranch        *b_sceta2;   //!
   TBranch        *b_r92;   //!
   TBranch        *b_sieie2;   //!
   TBranch        *b_hoe2;   //!
   TBranch        *b_scRawEne2;   //!
   TBranch        *b_chiso2;   //!
   TBranch        *b_phoiso2;   //!
   TBranch        *b_neuiso2;   //!
   TBranch        *b_eleveto1;   //!
   TBranch        *b_eleveto2;   //!
   TBranch        *b_presel1;   //!
   TBranch        *b_presel2;   //!
   TBranch        *b_sel1;   //!
   TBranch        *b_sel2;   //!
   TBranch        *b_tightsel1;   //!
   TBranch        *b_tightsel2;   //!
   TBranch        *b_genmatch1;   //!
   TBranch        *b_genmatch2;   //!
   TBranch        *b_geniso1;   //!
   TBranch        *b_geniso2;   //!
   TBranch        *b_vtxIndex;   //!
   TBranch        *b_vtxX;   //!
   TBranch        *b_vtxY;   //!
   TBranch        *b_vtxZ;   //!
   TBranch        *b_genVtxX;   //!
   TBranch        *b_genVtxY;   //!
   TBranch        *b_genVtxZ;   //!
   TBranch        *b_passCHiso1;   //!
   TBranch        *b_passCHiso2;   //!
   TBranch        *b_passNHiso1;   //!
   TBranch        *b_passNHiso2;   //!
   TBranch        *b_passPHiso1;   //!
   TBranch        *b_passPHiso2;   //!
   TBranch        *b_passSieie1;   //!
   TBranch        *b_passSieie2;   //!
   TBranch        *b_passHoe1;   //!
   TBranch        *b_passHoe2;   //!
   TBranch        *b_passLooseCHiso1;   //!
   TBranch        *b_passLooseCHiso2;   //!
   TBranch        *b_passLooseNHiso1;   //!
   TBranch        *b_passLooseNHiso2;   //!
   TBranch        *b_passLoosePHiso1;   //!
   TBranch        *b_passLoosePHiso2;   //!
   TBranch        *b_passLooseSieie1;   //!
   TBranch        *b_passLooseSieie2;   //!
   TBranch        *b_passLooseHoe1;   //!
   TBranch        *b_passLooseHoe2;   //!
   TBranch        *b_passTightCHiso1;   //!
   TBranch        *b_passTightCHiso2;   //!
   TBranch        *b_passTightNHiso1;   //!
   TBranch        *b_passTightNHiso2;   //!
   TBranch        *b_passTightPHiso1;   //!
   TBranch        *b_passTightPHiso2;   //!
   TBranch        *b_passTightSieie1;   //!
   TBranch        *b_passTightSieie2;   //!
   TBranch        *b_passTightHoe1;   //!
   TBranch        *b_passTightHoe2;   //!
   TBranch        *b_hltPhoton26Photon16Mass60;   //!
   TBranch        *b_hltPhoton36Photon22Mass15;   //!
   TBranch        *b_hltPhoton42Photon25Mass15;   //!
   TBranch        *b_hltDiphoton30Mass95;   //!
   TBranch        *b_hltDiphoton30Mass70;   //!
   TBranch        *b_hltDiphoton30Mass55;   //!
   TBranch        *b_hltDiphoton30Mass55PV;   //!
   TBranch        *b_hltDiphoton30Mass55EB;   //!
   TBranch        *b_nEle;   //!
   TBranch        *b_nMuons;   //!
   TBranch        *b_nJets;   //!
   TBranch        *b_nLooseBjets;   //!
   TBranch        *b_nMediumBjets;   //!
   TBranch        *b_vhtruth;   //!
   TBranch        *b_metF_GV;   //!
   TBranch        *b_metF_HBHENoise;   //!
   TBranch        *b_metF_HBHENoiseIso;   //!
   TBranch        *b_metF_CSC;   //!
   TBranch        *b_metF_eeBadSC;   //!
   TBranch        *b_b_higgsVtxX;   //!
   TBranch        *b_b_higgsVtxY;   //!
   TBranch        *b_b_higgsVtxZ;   //!
   TBranch        *b_b_massCorrSmear;   //!
   TBranch        *b_b_massCorrSmearUp;   //!
   TBranch        *b_b_massCorrSmearDown;   //!
   TBranch        *b_b_massCorrScale;   //!
   TBranch        *b_b_massCorrScaleUp;   //!
   TBranch        *b_b_massCorrScaleDown;   //!
   TBranch        *b_b_massRaw;   //!
   TBranch        *b_b_mva1;   //!
   TBranch        *b_b_mva2;   //!
   TBranch        *b_b_genZ;   //!
   TBranch        *b_b_ptZ;   //!
   TBranch        *b_b_etaZ;   //!
   TBranch        *b_b_phiZ;   //!

   METCorr2016(TTree *tree=0, TString inDir="", TString outDir="", TString inSpecies="");
   virtual ~METCorr2016();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   DblVec  Loop(TString inDir, TString outFile);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   private:
   TString	name;
   TString	species;
   TString	inFile;

};

#endif

#ifdef METCorr2016_cxx
 METCorr2016::METCorr2016(TTree *tree, TString inDir, TString outDir, TString inSpecies) : fChain(0) 
{

// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  name = inDir; 
  species = inSpecies; 
  inFile = Form("%s%s.root",name.Data(),species.Data());

  if (tree == 0) {
    //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("2HDM_mZP600.root");
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(Form("%s.root",species.Data()));
    if (!f || !f->IsOpen()) {
      //f = new TFile("2HDM_mZP600.root");
      f = new TFile(inFile);
    }
    f->GetObject("DiPhotonTree",tree);
  }
  Init(tree);
}

METCorr2016::~METCorr2016()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   std::cout << "Finished calculating MET correction" << std::endl;
}

Int_t METCorr2016::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t METCorr2016::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void METCorr2016::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the  addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set  addresses and branch pointers

   std::cout << "tree is " << tree << std::endl;

   if (tree->InheritsFrom("TChain")) ((TChain*)tree)->LoadTree(0);
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   //fChain->SetBranchAddress("xsecWgt", &xsecWgt, &b_xsecWgt);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   //fChain->SetBranchAddress("mggNominal", &mggNominal, &b_mggNominal);
   //fChain->SetBranchAddress("mggGen", &mggGen, &b_mggGen);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("rho", &rho, &b_rho);
   fChain->SetBranchAddress("sampleID", &sampleID, &b_sampleID);
   fChain->SetBranchAddress("totXsec", &totXsec, &b_totXsec);
   fChain->SetBranchAddress("pu_weight", &pu_weight, &b_pu_weight);
   fChain->SetBranchAddress("pu_n", &pu_n, &b_pu_n);
   fChain->SetBranchAddress("sumDataset", &sumDataset, &b_sumDataset);
   fChain->SetBranchAddress("perEveW", &perEveW, &b_perEveW);
   fChain->SetBranchAddress("pfmet", &pfmet, &b_pfmet);
   fChain->SetBranchAddress("pfmetPhi", &pfmetPhi, &b_pfmetPhi);
   fChain->SetBranchAddress("pfmetSumEt", &pfmetSumEt, &b_pfmetSumEt);
   fChain->SetBranchAddress("t1pfmet", &t1pfmet, &b_t1pfmet);
   fChain->SetBranchAddress("t1p2pfmet", &t1p2pfmet, &b_b_t1p2pfmet);
   fChain->SetBranchAddress("t1pfmetJetEnUp", &t1pfmetJetEnUp, &b_b_t1pfmetJetEnUp);
   fChain->SetBranchAddress("t1pfmetJetEnDown", &t1pfmetJetEnDown, &b_b_t1pfmetJetEnDown);
   fChain->SetBranchAddress("t1pfmetJetResUp", &t1pfmetJetResUp, &b_b_t1pfmetJetResUp);
   fChain->SetBranchAddress("t1pfmetJetResDown", &t1pfmetJetResDown, &b_b_t1pfmetJetResDown);
   fChain->SetBranchAddress("t1pfmetMuonEnUp", &t1pfmetMuonEnUp, &b_b_t1pfmetMuonEnUp);
   fChain->SetBranchAddress("t1pfmetMuonEnDown", &t1pfmetMuonEnDown, &b_b_t1pfmetMuonEnDown);
   fChain->SetBranchAddress("t1pfmetElectronEnUp", &t1pfmetElectronEnUp, &b_b_t1pfmetElectronEnUp);
   fChain->SetBranchAddress("t1pfmetElectronEnDown", &t1pfmetElectronEnDown, &b_b_t1pfmetElectronEnDown);
   fChain->SetBranchAddress("t1pfmetTauEnUp", &t1pfmetTauEnUp, &b_b_t1pfmetTauEnUp);
   fChain->SetBranchAddress("t1pfmetTauEnDown", &t1pfmetTauEnDown, &b_b_t1pfmetTauEnDown);
   fChain->SetBranchAddress("t1pfmetPhotonEnUp", &t1pfmetPhotonEnUp, &b_b_t1pfmetPhotonEnUp);
   fChain->SetBranchAddress("t1pfmetPhotonEnDown", &t1pfmetPhotonEnDown, &b_b_t1pfmetPhotonEnDown);
   fChain->SetBranchAddress("t1pfmetUnclusteredEnUp", &t1pfmetUnclusteredEnUp, &b_b_t1pfmetUnclusteredEnUp);
   fChain->SetBranchAddress("t1pfmetUnclusteredEnDown", &t1pfmetUnclusteredEnDown, &b_b_t1pfmetUnclusteredEnDown);
   fChain->SetBranchAddress("t1pfmetPhi", &t1pfmetPhi, &b_t1pfmetPhi);
   fChain->SetBranchAddress("t1pfmetSumEt", &t1pfmetSumEt, &b_t1pfmetSumEt);
   fChain->SetBranchAddress("calomet", &calomet, &b_calomet);
   fChain->SetBranchAddress("calometPhi", &calometPhi, &b_calometPhi);
   fChain->SetBranchAddress("calometSumEt", &calometSumEt, &b_calometSumEt);
   fChain->SetBranchAddress("ptgg", &ptgg, &b_ptgg);
   fChain->SetBranchAddress("mgg", &mgg, &b_mgg);
   fChain->SetBranchAddress("eventClass", &eventClass, &b_eventClass);
   fChain->SetBranchAddress("pt1", &pt1, &b_pt1);
   fChain->SetBranchAddress("ptOverM1", &ptOverM1, &b_ptOverM1);
   fChain->SetBranchAddress("eta1", &eta1, &b_eta1);
   fChain->SetBranchAddress("phi1", &phi1, &b_phi1);
   fChain->SetBranchAddress("sceta1", &sceta1, &b_sceta1);
   fChain->SetBranchAddress("r91", &r91, &b_r91);
   fChain->SetBranchAddress("sieie1", &sieie1, &b_sieie1);
   fChain->SetBranchAddress("hoe1", &hoe1, &b_hoe1);
   fChain->SetBranchAddress("scRawEne1", &scRawEne1, &b_scRawEne1);
   fChain->SetBranchAddress("chiso1", &chiso1, &b_chiso1);
   fChain->SetBranchAddress("phoiso1", &phoiso1, &b_phoiso1);
   fChain->SetBranchAddress("neuiso1", &neuiso1, &b_neuiso1);
   fChain->SetBranchAddress("pt2", &pt2, &b_pt2);
   fChain->SetBranchAddress("ptOverM2", &ptOverM2, &b_ptOverM2);
   fChain->SetBranchAddress("eta2", &eta2, &b_eta2);
   fChain->SetBranchAddress("phi2", &phi2, &b_phi2);
   fChain->SetBranchAddress("sceta2", &sceta2, &b_sceta2);
   fChain->SetBranchAddress("r92", &r92, &b_r92);
   fChain->SetBranchAddress("sieie2", &sieie2, &b_sieie2);
   fChain->SetBranchAddress("hoe2", &hoe2, &b_hoe2);
   fChain->SetBranchAddress("scRawEne2", &scRawEne2, &b_scRawEne2);
   fChain->SetBranchAddress("chiso2", &chiso2, &b_chiso2);
   fChain->SetBranchAddress("phoiso2", &phoiso2, &b_phoiso2);
   fChain->SetBranchAddress("neuiso2", &neuiso2, &b_neuiso2);
   fChain->SetBranchAddress("eleveto1", &eleveto1, &b_eleveto1);
   fChain->SetBranchAddress("eleveto2", &eleveto2, &b_eleveto2);
   fChain->SetBranchAddress("presel1", &presel1, &b_presel1);
   fChain->SetBranchAddress("presel2", &presel2, &b_presel2);
   fChain->SetBranchAddress("sel1", &sel1, &b_sel1);
   fChain->SetBranchAddress("sel2", &sel2, &b_sel2);
   fChain->SetBranchAddress("tightsel1", &tightsel1, &b_tightsel1);
   fChain->SetBranchAddress("tightsel2", &tightsel2, &b_tightsel2);
   fChain->SetBranchAddress("genmatch1", &genmatch1, &b_genmatch1);
   fChain->SetBranchAddress("genmatch2", &genmatch2, &b_genmatch2);
   fChain->SetBranchAddress("geniso1", &geniso1, &b_geniso1);
   fChain->SetBranchAddress("geniso2", &geniso2, &b_geniso2);
   fChain->SetBranchAddress("vtxIndex", &vtxIndex, &b_vtxIndex);
   fChain->SetBranchAddress("vtxX", &vtxX, &b_vtxX);
   fChain->SetBranchAddress("vtxY", &vtxY, &b_vtxY);
   fChain->SetBranchAddress("vtxZ", &vtxZ, &b_vtxZ);
   fChain->SetBranchAddress("genVtxX", &genVtxX, &b_genVtxX);
   fChain->SetBranchAddress("genVtxY", &genVtxY, &b_genVtxY);
   fChain->SetBranchAddress("genVtxZ", &genVtxZ, &b_genVtxZ);
   fChain->SetBranchAddress("passCHiso1", &passCHiso1, &b_passCHiso1);
   fChain->SetBranchAddress("passCHiso2", &passCHiso2, &b_passCHiso2);
   fChain->SetBranchAddress("passNHiso1", &passNHiso1, &b_passNHiso1);
   fChain->SetBranchAddress("passNHiso2", &passNHiso2, &b_passNHiso2);
   fChain->SetBranchAddress("passPHiso1", &passPHiso1, &b_passPHiso1);
   fChain->SetBranchAddress("passPHiso2", &passPHiso2, &b_passPHiso2);
   fChain->SetBranchAddress("passSieie1", &passSieie1, &b_passSieie1);
   fChain->SetBranchAddress("passSieie2", &passSieie2, &b_passSieie2);
   fChain->SetBranchAddress("passHoe1", &passHoe1, &b_passHoe1);
   fChain->SetBranchAddress("passHoe2", &passHoe2, &b_passHoe2);
   fChain->SetBranchAddress("passLooseCHiso1", &passLooseCHiso1, &b_passLooseCHiso1);
   fChain->SetBranchAddress("passLooseCHiso2", &passLooseCHiso2, &b_passLooseCHiso2);
   fChain->SetBranchAddress("passLooseNHiso1", &passLooseNHiso1, &b_passLooseNHiso1);
   fChain->SetBranchAddress("passLooseNHiso2", &passLooseNHiso2, &b_passLooseNHiso2);
   fChain->SetBranchAddress("passLoosePHiso1", &passLoosePHiso1, &b_passLoosePHiso1);
   fChain->SetBranchAddress("passLoosePHiso2", &passLoosePHiso2, &b_passLoosePHiso2);
   fChain->SetBranchAddress("passLooseSieie1", &passLooseSieie1, &b_passLooseSieie1);
   fChain->SetBranchAddress("passLooseSieie2", &passLooseSieie2, &b_passLooseSieie2);
   fChain->SetBranchAddress("passLooseHoe1", &passLooseHoe1, &b_passLooseHoe1);
   fChain->SetBranchAddress("passLooseHoe2", &passLooseHoe2, &b_passLooseHoe2);
   fChain->SetBranchAddress("passTightCHiso1", &passTightCHiso1, &b_passTightCHiso1);
   fChain->SetBranchAddress("passTightCHiso2", &passTightCHiso2, &b_passTightCHiso2);
   fChain->SetBranchAddress("passTightNHiso1", &passTightNHiso1, &b_passTightNHiso1);
   fChain->SetBranchAddress("passTightNHiso2", &passTightNHiso2, &b_passTightNHiso2);
   fChain->SetBranchAddress("passTightPHiso1", &passTightPHiso1, &b_passTightPHiso1);
   fChain->SetBranchAddress("passTightPHiso2", &passTightPHiso2, &b_passTightPHiso2);
   fChain->SetBranchAddress("passTightSieie1", &passTightSieie1, &b_passTightSieie1);
   fChain->SetBranchAddress("passTightSieie2", &passTightSieie2, &b_passTightSieie2);
   fChain->SetBranchAddress("passTightHoe1", &passTightHoe1, &b_passTightHoe1);
   fChain->SetBranchAddress("passTightHoe2", &passTightHoe2, &b_passTightHoe2);
   fChain->SetBranchAddress("hltPhoton26Photon16Mass60", &hltPhoton26Photon16Mass60, &b_hltPhoton26Photon16Mass60);
   fChain->SetBranchAddress("hltPhoton36Photon22Mass15", &hltPhoton36Photon22Mass15, &b_hltPhoton36Photon22Mass15);
   fChain->SetBranchAddress("hltPhoton42Photon25Mass15", &hltPhoton42Photon25Mass15, &b_hltPhoton42Photon25Mass15);
   fChain->SetBranchAddress("hltDiphoton30Mass95", &hltDiphoton30Mass95, &b_hltDiphoton30Mass95);
   fChain->SetBranchAddress("hltDiphoton30Mass70", &hltDiphoton30Mass70, &b_hltDiphoton30Mass70);
   fChain->SetBranchAddress("hltDiphoton30Mass55", &hltDiphoton30Mass55, &b_hltDiphoton30Mass55);
   fChain->SetBranchAddress("hltDiphoton30Mass55PV", &hltDiphoton30Mass55PV, &b_hltDiphoton30Mass55PV);
   fChain->SetBranchAddress("hltDiphoton30Mass55EB", &hltDiphoton30Mass55EB, &b_hltDiphoton30Mass55EB);
   fChain->SetBranchAddress("nEle", &nEle, &b_nEle);
   fChain->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
   fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
   fChain->SetBranchAddress("nLooseBjets", &nLooseBjets, &b_nLooseBjets);
   fChain->SetBranchAddress("nMediumBjets", &nMediumBjets, &b_nMediumBjets);
   fChain->SetBranchAddress("vhtruth", &vhtruth, &b_vhtruth);
   fChain->SetBranchAddress("metF_GV", &metF_GV, &b_metF_GV);
   fChain->SetBranchAddress("metF_HBHENoise", &metF_HBHENoise, &b_metF_HBHENoise);
   fChain->SetBranchAddress("metF_HBHENoiseIso", &metF_HBHENoiseIso, &b_metF_HBHENoiseIso);
   fChain->SetBranchAddress("metF_CSC", &metF_CSC, &b_metF_CSC);
   fChain->SetBranchAddress("metF_eeBadSC", &metF_eeBadSC, &b_metF_eeBadSC);
   fChain->SetBranchAddress("higgsVtxX", &higgsVtxX, &b_b_higgsVtxX);
   fChain->SetBranchAddress("higgsVtxY", &higgsVtxY, &b_b_higgsVtxY);
   fChain->SetBranchAddress("higgsVtxZ", &higgsVtxZ, &b_b_higgsVtxZ);
   fChain->SetBranchAddress("massCorrSmear", &massCorrSmear, &b_b_massCorrSmear);
   fChain->SetBranchAddress("massCorrSmearUp", &massCorrSmearUp, &b_b_massCorrSmearUp);
   fChain->SetBranchAddress("massCorrSmearDown", &massCorrSmearDown, &b_b_massCorrSmearDown);
   fChain->SetBranchAddress("massCorrScale", &massCorrScale, &b_b_massCorrScale);
   fChain->SetBranchAddress("massCorrScaleUp", &massCorrScaleUp, &b_b_massCorrScaleUp);
   fChain->SetBranchAddress("massCorrScaleDown", &massCorrScaleDown, &b_b_massCorrScaleDown);
   fChain->SetBranchAddress("massRaw", &massRaw, &b_b_massRaw);
   fChain->SetBranchAddress("mva1", &mva1, &b_b_mva1);
   fChain->SetBranchAddress("mva2", &mva2, &b_b_mva2);
   fChain->SetBranchAddress("genZ", &genZ, &b_b_genZ);
   fChain->SetBranchAddress("ptZ", &ptZ, &b_b_ptZ);
   fChain->SetBranchAddress("etaZ", &etaZ, &b_b_etaZ);
   fChain->SetBranchAddress("phiZ", &phiZ, &b_b_phiZ);
   Notify();
}

Bool_t METCorr2016::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void METCorr2016::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t METCorr2016::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef METCorr2016_cxx
