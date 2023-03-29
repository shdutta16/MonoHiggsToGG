// root -l addWeightsToTree.cc
// addWeights(file,lumiForWeight)

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <iostream>

using namespace std;

void addWeights(const char* filename, float lumiForW, float massTrue=1) {

  cout << "Adding weight branch to file " << filename << endl;  

  TFile *fileOrig = 0;
  TTree *treeOrig = 0;
  TH1F  *h_entries = 0;
  TH1F  *h_sumW = 0;
  TH1F  *h_selection = 0;
  TH1F  *h_selection_unwgt = 0;

  fileOrig = TFile::Open(filename);
  if( fileOrig ) {
    fileOrig->cd();
    treeOrig = (TTree*)fileOrig->Get("diPhoAna/DiPhotonTree");
    h_entries = (TH1F*)fileOrig->Get("diPhoAna/h_entries");
    h_sumW    = (TH1F*)fileOrig->Get("diPhoAna/h_sumW");
    h_selection = (TH1F*)fileOrig->Get("diPhoAna/h_selection");
  } else {
    cout << "File " << filename << " not existing !" << endl;
    return;
  }


  fileOrig->cd();
  if (!treeOrig) {
    cout << "Tree diPhoAna/DiPhotonTree not existing !" << endl; 
    return;    
  }
  
  float sampleSumWeight = (float)h_sumW->Integral();    // sum of weights in the dataset we ran on originally

  int nentriesOrig = treeOrig->GetEntries();   // number fo entries saved in the first tree
  
  TFile *fileNew = TFile::Open(filename,"recreate");
  TTree *treeNew = new TTree("DiPhotonTree","tree with 2 photon selection");
  
  std::vector<TTree*> trees; 
  trees.push_back(treeNew);
  
  // original tree leaves
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
  Float_t	  pfmet;
  Float_t	  pfmetPhi;
  Float_t	  pfmetSumEt;
  Float_t	  t1pfmet;
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
  Float_t	  t1pfmetPhi;
  Float_t	  t1pfmetSumEt;
  Float_t	  calomet;
  Float_t	  calometPhi;
  Float_t	  calometSumEt;
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
  Int_t		  eleveto1;
  Int_t		  eleveto2;
  Int_t           presel1;
  Int_t           presel2;
  Int_t           sel1;
  Int_t           sel2;
  Int_t           tightsel1;
  Int_t           tightsel2;
  Float_t         genmgg;
  Int_t           genmatch1;
  Int_t           genmatch2;
  Float_t         geniso1;
  Float_t         geniso2;
  Float_t         ptJetLead;
  Float_t         etaJetLead;
  Float_t         phiJetLead;
  Float_t         massJetLead;
  Int_t           indexJetLead;
  Float_t         NEMfracJet1;
  Float_t         CEMfracJet1;
  Float_t         ELfracJet1;
  Float_t         CHfracJet1;
  Float_t         NHfracJet1;
  Float_t         PHfracJet1;
  Float_t         MUfracJet1;
  Int_t           CHmultJet1;
  Int_t           NEmultJet1;
  Float_t         ptJetSubLead;
  Float_t         etaJetSubLead;
  Float_t         phiJetSubLead;
  Float_t         massJetSubLead;
  Int_t           indexJetSubLead;
  Float_t         NEMfracJet2;
  Float_t         CEMfracJet2;
  Float_t         ELfracJet2;
  Float_t         CHfracJet2;
  Float_t         NHfracJet2;
  Float_t         PHfracJet2;
  Float_t         MUfracJet2;
  Int_t           CHmultJet2;
  Int_t           NEmultJet2;
  Float_t         ptJet3;
  Float_t         etaJet3;
  Float_t         phiJet3;
  Float_t         massJet3;
  Int_t           indexJet3;
  Float_t         NEMfracJet3;
  Float_t         CEMfracJet3;
  Float_t         ELfracJet3;
  Float_t         CHfracJet3;
  Float_t         NHfracJet3;
  Float_t         PHfracJet3;
  Float_t         MUfracJet3;
  Int_t           CHmultJet3;
  Int_t           NEmultJet3;
  Float_t         ptJet4;
  Float_t         etaJet4;
  Float_t         phiJet4;
  Float_t         massJet4;
  Int_t           indexJet4;
  Float_t         NEMfracJet4;
  Float_t         CEMfracJet4;
  Float_t         ELfracJet4;
  Float_t         CHfracJet4;
  Float_t         NHfracJet4;
  Float_t         PHfracJet4;
  Float_t         MUfracJet4;
  Int_t           CHmultJet4;
  Int_t           NEmultJet4;
  Int_t           vtxIndex;
  Float_t         higgsVtxX;
  Float_t         higgsVtxY;
  Float_t         higgsVtxZ;
  Float_t         vtxX;
  Float_t         vtxY;
  Float_t         vtxZ;
  Float_t         vtx0Z;
  Float_t         genVtxX;
  Float_t         genVtxY;
  Float_t         genVtxZ;
  Int_t		  passCHiso1;
  Int_t		  passCHiso2;
  Int_t		  passNHiso1;
  Int_t		  passNHiso2;
  Int_t		  passPHiso1;
  Int_t		  passPHiso2;
  Int_t		  passSieie1;
  Int_t		  passSieie2;
  Int_t		  passHoe1;
  Int_t		  passHoe2;
  Int_t		  passLooseCHiso1;
  Int_t		  passLooseCHiso2;
  Int_t		  passLooseNHiso1;
  Int_t		  passLooseNHiso2;
  Int_t		  passLoosePHiso1;
  Int_t		  passLoosePHiso2;
  Int_t		  passLooseSieie1;
  Int_t		  passLooseSieie2;
  Int_t		  passLooseHoe1;
  Int_t		  passLooseHoe2;
  Int_t		  passTightCHiso1;
  Int_t		  passTightCHiso2;
  Int_t		  passTightNHiso1;
  Int_t		  passTightNHiso2;
  Int_t		  passTightPHiso1;
  Int_t		  passTightPHiso2;
  Int_t		  passTightSieie1;
  Int_t		  passTightSieie2;
  Int_t		  passTightHoe1;
  Int_t		  passTightHoe2;
  Int_t		  hltPhoton26Photon16Mass60;
  Int_t		  hltPhoton36Photon22Mass15;
  Int_t		  hltPhoton42Photon25Mass15;
  Int_t		  hltDiphoton30Mass95; 
  Int_t		  hltDiphoton30Mass70; 
  Int_t		  hltDiphoton30Mass55; 
  Int_t		  hltDiphoton30Mass55PV;
  Int_t		  hltDiphoton30Mass55EB;
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
  Int_t		  metF_MuonBadTrack;
  Int_t		  metF_HadronTrackRes;
  Float_t         massCorrSmear;
  Float_t         massCorrSmearUp;
  Float_t         massCorrSmearDown;
  Float_t         massCorrScale;
  Float_t         massCorrScaleUp;
  Float_t         massCorrScaleDown;
  Float_t         massRaw;
  Float_t         mva1;
  Float_t         mva2;
  Int_t		  genZ;
  Float_t	  ptZ;
  Float_t	  etaZ;
  Float_t	  phiZ; 
  Float_t	  BDTptgg;
  Float_t	  BDTmassRaw;
  Float_t	  BDTr91;
  Float_t	  BDTr92;
  Float_t	  BDTvtxZ;
  Int_t		  BDTindex;

 
  // List of branches - original tree
  TBranch        *b_run; 
  TBranch        *b_event;
  TBranch        *b_lumi;
  TBranch        *b_nvtx;
  TBranch        *b_rho; 
  TBranch        *b_sampleID; 
  TBranch        *b_totXsec;
  TBranch        *b_pu_weight;
  TBranch        *b_pu_n;
  TBranch        *b_sumDataset;
  TBranch        *b_perEveW;
  TBranch	 *b_pfmet;
  TBranch	 *b_pfmetPhi;
  TBranch	 *b_pfmetSumEt;
  TBranch	 *b_t1pfmet;
  TBranch        *b_t1p2pfmet;   //!
  TBranch        *b_t1pfmetJetEnUp;   //!
  TBranch        *b_t1pfmetJetEnDown;   //!
  TBranch        *b_t1pfmetJetResUp;   //!
  TBranch        *b_t1pfmetJetResDown;   //!
  TBranch        *b_t1pfmetMuonEnUp;   //!
  TBranch        *b_t1pfmetMuonEnDown;   //!
  TBranch        *b_t1pfmetElectronEnUp;   //!
  TBranch        *b_t1pfmetElectronEnDown;   //!
  TBranch        *b_t1pfmetTauEnUp;   //!
  TBranch        *b_t1pfmetTauEnDown;   //!
  TBranch        *b_t1pfmetPhotonEnUp;   //!
  TBranch        *b_t1pfmetPhotonEnDown;   //!
  TBranch        *b_t1pfmetUnclusteredEnUp;   //!
  TBranch        *b_t1pfmetUnclusteredEnDown;   //!
  TBranch	 *b_t1pfmetPhi;
  TBranch	 *b_t1pfmetSumEt;
  TBranch	 *b_calomet;
  TBranch	 *b_calometPhi;
  TBranch	 *b_calometSumEt;
  TBranch        *b_ptgg;
  TBranch        *b_mgg; 
  TBranch        *b_eventClass; 
  TBranch        *b_pt1; 
  TBranch        *b_ptOverM1;
  TBranch        *b_eta1; 
  TBranch        *b_phi1; 
  TBranch        *b_sceta1; 
  TBranch        *b_r91; 
  TBranch        *b_sieie1;
  TBranch        *b_hoe1; 
  TBranch        *b_scRawEne1; 
  TBranch        *b_chiso1;  
  TBranch        *b_phoiso1; 
  TBranch        *b_neuiso1;   
  TBranch        *b_pt2; 
  TBranch        *b_ptOverM2;
  TBranch        *b_eta2; 
  TBranch        *b_phi2;
  TBranch        *b_sceta2;  
  TBranch        *b_r92; 
  TBranch        *b_sieie2;
  TBranch        *b_hoe2; 
  TBranch        *b_scRawEne2; 
  TBranch        *b_chiso2;  
  TBranch        *b_phoiso2; 
  TBranch        *b_neuiso2;   
  TBranch	 *b_eleveto1;
  TBranch	 *b_eleveto2;
  TBranch        *b_presel1;
  TBranch        *b_presel2;
  TBranch        *b_sel1;
  TBranch        *b_sel2;
  TBranch        *b_tightsel1;
  TBranch        *b_tightsel2;
  TBranch        *b_genmgg; 
  TBranch        *b_ptJetLead;   //!
  TBranch        *b_etaJetLead;   //!
  TBranch        *b_phiJetLead;   //!
  TBranch        *b_massJetLead;   //!
  TBranch        *b_indexJetLead;   //!
  TBranch        *b_NEMfracJet1;   //!
  TBranch        *b_CEMfracJet1;   //!
  TBranch        *b_ELfracJet1;   //!
  TBranch        *b_CHfracJet1;   //!
  TBranch        *b_NHfracJet1;   //!
  TBranch        *b_PHfracJet1;   //!
  TBranch        *b_MUfracJet1;   //!
  TBranch        *b_CHmultJet1;   //!
  TBranch        *b_NEmultJet1;   //!
  TBranch        *b_ptJetSubLead;   //!
  TBranch        *b_etaJetSubLead;   //!
  TBranch        *b_phiJetSubLead;   //!
  TBranch        *b_massJetSubLead;   //!
  TBranch        *b_indexJetSubLead;   //!
  TBranch        *b_NEMfracJet2;   //!
  TBranch        *b_CEMfracJet2;   //!
  TBranch        *b_ELfracJet2;   //!
  TBranch        *b_CHfracJet2;   //!
  TBranch        *b_NHfracJet2;   //!
  TBranch        *b_PHfracJet2;   //!
  TBranch        *b_MUfracJet2;   //!
  TBranch        *b_CHmultJet2;   //!
  TBranch        *b_NEmultJet2;   //!
  TBranch        *b_ptJet3;   //!
  TBranch        *b_etaJet3;   //!
  TBranch        *b_phiJet3;   //!
  TBranch        *b_massJet3;   //!
  TBranch        *b_indexJet3;   //!
  TBranch        *b_NEMfracJet3;   //!
  TBranch        *b_CEMfracJet3;   //!
  TBranch        *b_ELfracJet3;   //!
  TBranch        *b_CHfracJet3;   //!
  TBranch        *b_NHfracJet3;   //!
  TBranch        *b_PHfracJet3;   //!
  TBranch        *b_MUfracJet3;   //!
  TBranch        *b_CHmultJet3;   //!
  TBranch        *b_NEmultJet3;   //!
  TBranch        *b_ptJet4;   //!
  TBranch        *b_etaJet4;   //!
  TBranch        *b_phiJet4;   //!
  TBranch        *b_massJet4;   //!
  TBranch        *b_indexJet4;   //!
  TBranch        *b_NEMfracJet4;   //!
  TBranch        *b_CEMfracJet4;   //!
  TBranch        *b_ELfracJet4;   //!
  TBranch        *b_CHfracJet4;   //!
  TBranch        *b_NHfracJet4;   //!
  TBranch        *b_PHfracJet4;   //!
  TBranch        *b_MUfracJet4;   //!
  TBranch        *b_CHmultJet4;   //!
  TBranch        *b_NEmultJet4;   //!
  TBranch        *b_genmatch1; 
  TBranch        *b_genmatch2; 
  TBranch        *b_geniso1; 
  TBranch        *b_geniso2; 
  TBranch        *b_vtxIndex; 
  TBranch        *b_vtxX;
  TBranch        *b_vtxY;
  TBranch        *b_vtxZ;
  TBranch        *b_vtx0Z;
  TBranch        *b_genVtxX; 
  TBranch        *b_genVtxY; 
  TBranch        *b_genVtxZ; 
  TBranch	 *b_passCHiso1;
  TBranch	 *b_passCHiso2;
  TBranch	 *b_passNHiso1;
  TBranch	 *b_passNHiso2;
  TBranch	 *b_passPHiso1;
  TBranch	 *b_passPHiso2;
  TBranch	 *b_passSieie1;
  TBranch	 *b_passSieie2;
  TBranch	 *b_passHoe1;
  TBranch	 *b_passHoe2;
  TBranch	 *b_passLooseCHiso1;
  TBranch	 *b_passLooseCHiso2;
  TBranch	 *b_passLooseNHiso1;
  TBranch	 *b_passLooseNHiso2;
  TBranch	 *b_passLoosePHiso1;
  TBranch	 *b_passLoosePHiso2;
  TBranch	 *b_passLooseSieie1;
  TBranch	 *b_passLooseSieie2;
  TBranch	 *b_passLooseHoe1;
  TBranch	 *b_passLooseHoe2;
  TBranch	 *b_passTightCHiso1;
  TBranch	 *b_passTightCHiso2;
  TBranch	 *b_passTightNHiso1;
  TBranch	 *b_passTightNHiso2;
  TBranch	 *b_passTightPHiso1;
  TBranch	 *b_passTightPHiso2;
  TBranch	 *b_passTightSieie1;
  TBranch	 *b_passTightSieie2;
  TBranch	 *b_passTightHoe1;
  TBranch	 *b_passTightHoe2;
  TBranch	 *b_hltPhoton26Photon16Mass60;
  TBranch	 *b_hltPhoton36Photon22Mass15;
  TBranch	 *b_hltPhoton42Photon25Mass15;
  TBranch	 *b_hltDiphoton30Mass95; 
  TBranch	 *b_hltDiphoton30Mass70; 
  TBranch	 *b_hltDiphoton30Mass55; 
  TBranch	 *b_hltDiphoton30Mass55PV;
  TBranch	 *b_hltDiphoton30Mass55EB;
  TBranch        *b_nEle;   //!  
  TBranch        *b_nMuons;   //!  
  TBranch        *b_nJets;   //!      
  TBranch        *b_nLooseBjets;   //!  
  TBranch        *b_nMediumBjets;   //!      
  TBranch        *b_vhtruth;   //!     
  TBranch        *b_metF_GV;    
  TBranch        *b_metF_HBHENoise;    
  TBranch        *b_metF_HBHENoiseIso;    
  TBranch        *b_metF_CSC;    
  TBranch        *b_metF_eeBadSC;    
  TBranch        *b_metF_MuonBadTrack;    
  TBranch        *b_metF_HadronTrackRes;    
  TBranch        *b_higgsVtxX;   //!
  TBranch        *b_higgsVtxY;   //!
  TBranch        *b_higgsVtxZ;   //!
  TBranch        *b_massCorrSmear;   //!
  TBranch        *b_massCorrSmearUp;   //!
  TBranch        *b_massCorrSmearDown;   //!
  TBranch        *b_massCorrScale;   //!
  TBranch        *b_massCorrScaleUp;   //!
  TBranch        *b_massCorrScaleDown;   //!
  TBranch        *b_massRaw;   //!
  TBranch        *b_mva1;   //!
  TBranch        *b_mva2;   //!
  TBranch	 *b_genZ;
  TBranch	 *b_ptZ;
  TBranch	 *b_etaZ;
  TBranch	 *b_phiZ;
  TBranch	 *b_BDTptgg;
  TBranch	 *b_BDTmassRaw;
  TBranch	 *b_BDTr91;
  TBranch	 *b_BDTr92;
  TBranch	 *b_BDTvtxZ;
  TBranch	 *b_BDTindex;


  // Set branch addresses and branch pointers 
  treeOrig->SetBranchAddress("run", &run, &b_run);
  treeOrig->SetBranchAddress("event", &event, &b_event);
  treeOrig->SetBranchAddress("lumi", &lumi, &b_lumi);
  treeOrig->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
  treeOrig->SetBranchAddress("rho", &rho, &b_rho);
  treeOrig->SetBranchAddress("sampleID", &sampleID, &b_sampleID);
  treeOrig->SetBranchAddress("totXsec", &totXsec, &b_totXsec);
  treeOrig->SetBranchAddress("pu_weight", &pu_weight, &b_pu_weight);
  treeOrig->SetBranchAddress("pu_n", &pu_n, &b_pu_n);
  treeOrig->SetBranchAddress("sumDataset", &sumDataset, &b_sumDataset);
  treeOrig->SetBranchAddress("perEveW", &perEveW, &b_perEveW);
  treeOrig->SetBranchAddress("pfmet", &pfmet, &b_pfmet);
  treeOrig->SetBranchAddress("pfmetPhi", &pfmetPhi, &b_pfmetPhi);
  treeOrig->SetBranchAddress("pfmetSumEt", &pfmetSumEt, &b_pfmetSumEt);
  treeOrig->SetBranchAddress("t1pfmet", &t1pfmet, &b_t1pfmet);
  treeOrig->SetBranchAddress("t1p2pfmet", &t1p2pfmet, &b_t1p2pfmet);
  treeOrig->SetBranchAddress("t1pfmetJetEnUp", &t1pfmetJetEnUp, &b_t1pfmetJetEnUp);
  treeOrig->SetBranchAddress("t1pfmetJetEnDown", &t1pfmetJetEnDown, &b_t1pfmetJetEnDown);
  treeOrig->SetBranchAddress("t1pfmetJetResUp", &t1pfmetJetResUp, &b_t1pfmetJetResUp);
  treeOrig->SetBranchAddress("t1pfmetJetResDown", &t1pfmetJetResDown, &b_t1pfmetJetResDown);
  treeOrig->SetBranchAddress("t1pfmetMuonEnUp", &t1pfmetMuonEnUp, &b_t1pfmetMuonEnUp);
  treeOrig->SetBranchAddress("t1pfmetMuonEnDown", &t1pfmetMuonEnDown, &b_t1pfmetMuonEnDown);
  treeOrig->SetBranchAddress("t1pfmetElectronEnUp", &t1pfmetElectronEnUp, &b_t1pfmetElectronEnUp);
  treeOrig->SetBranchAddress("t1pfmetElectronEnDown", &t1pfmetElectronEnDown, &b_t1pfmetElectronEnDown);
  treeOrig->SetBranchAddress("t1pfmetTauEnUp", &t1pfmetTauEnUp, &b_t1pfmetTauEnUp);
  treeOrig->SetBranchAddress("t1pfmetTauEnDown", &t1pfmetTauEnDown, &b_t1pfmetTauEnDown);
  treeOrig->SetBranchAddress("t1pfmetPhotonEnUp", &t1pfmetPhotonEnUp, &b_t1pfmetPhotonEnUp);
  treeOrig->SetBranchAddress("t1pfmetPhotonEnDown", &t1pfmetPhotonEnDown, &b_t1pfmetPhotonEnDown);
  treeOrig->SetBranchAddress("t1pfmetUnclusteredEnUp", &t1pfmetUnclusteredEnUp, &b_t1pfmetUnclusteredEnUp);
  treeOrig->SetBranchAddress("t1pfmetUnclusteredEnDown", &t1pfmetUnclusteredEnDown, &b_t1pfmetUnclusteredEnDown);
  treeOrig->SetBranchAddress("t1pfmetPhi", &t1pfmetPhi, &b_t1pfmetPhi);
  treeOrig->SetBranchAddress("t1pfmetSumEt", &t1pfmetSumEt, &b_t1pfmetSumEt);
  treeOrig->SetBranchAddress("calomet", &calomet, &b_calomet);
  treeOrig->SetBranchAddress("calometPhi", &calometPhi, &b_calometPhi);
  treeOrig->SetBranchAddress("calometSumEt", &calometSumEt, &b_calometSumEt);
  treeOrig->SetBranchAddress("ptgg", &ptgg, &b_ptgg);
  treeOrig->SetBranchAddress("mgg", &mgg, &b_mgg);
  treeOrig->SetBranchAddress("eventClass", &eventClass, &b_eventClass);
  treeOrig->SetBranchAddress("pt1", &pt1, &b_pt1);
  treeOrig->SetBranchAddress("ptOverM1", &ptOverM1, &b_ptOverM1);
  treeOrig->SetBranchAddress("eta1", &eta1, &b_eta1);
  treeOrig->SetBranchAddress("phi1", &phi1, &b_phi1);
  treeOrig->SetBranchAddress("sceta1", &sceta1, &b_sceta1);
  treeOrig->SetBranchAddress("r91", &r91, &b_r91);
  treeOrig->SetBranchAddress("sieie1", &sieie1, &b_sieie1);
  treeOrig->SetBranchAddress("hoe1", &hoe1, &b_hoe1);
  treeOrig->SetBranchAddress("scRawEne1", &scRawEne1, &b_scRawEne1);
  treeOrig->SetBranchAddress("chiso1", &chiso1, &b_chiso1);
  treeOrig->SetBranchAddress("phoiso1", &phoiso1, &b_phoiso1);
  treeOrig->SetBranchAddress("neuiso1", &neuiso1, &b_neuiso1);
  treeOrig->SetBranchAddress("pt2", &pt2, &b_pt2);
  treeOrig->SetBranchAddress("ptOverM2", &ptOverM2, &b_ptOverM2);
  treeOrig->SetBranchAddress("eta2", &eta2, &b_eta2);
  treeOrig->SetBranchAddress("phi2", &phi2, &b_phi2);
  treeOrig->SetBranchAddress("sceta2", &sceta2, &b_sceta2);
  treeOrig->SetBranchAddress("r92", &r92, &b_r92);
  treeOrig->SetBranchAddress("sieie2", &sieie2, &b_sieie2);
  treeOrig->SetBranchAddress("hoe2", &hoe2, &b_hoe2);
  treeOrig->SetBranchAddress("scRawEne2", &scRawEne2, &b_scRawEne2);
  treeOrig->SetBranchAddress("chiso2", &chiso2, &b_chiso2);
  treeOrig->SetBranchAddress("phoiso2", &phoiso2, &b_phoiso2);
  treeOrig->SetBranchAddress("neuiso2", &neuiso2, &b_neuiso2);
  treeOrig->SetBranchAddress("eleveto1",&eleveto1,&b_eleveto1);
  treeOrig->SetBranchAddress("eleveto2",&eleveto2,&b_eleveto2);
  treeOrig->SetBranchAddress("presel1",&presel1,&b_presel1);
  treeOrig->SetBranchAddress("presel2",&presel2,&b_presel2);
  treeOrig->SetBranchAddress("sel1",&sel1,&b_sel1);
  treeOrig->SetBranchAddress("sel2",&sel2,&b_sel2);
  treeOrig->SetBranchAddress("tightsel1",&tightsel1,&b_tightsel1);
  treeOrig->SetBranchAddress("tightsel2",&tightsel2,&b_tightsel2);
  treeOrig->SetBranchAddress("genmgg", &genmgg, &b_genmgg);
  treeOrig->SetBranchAddress("genmatch1", &genmatch1, &b_genmatch1);
  treeOrig->SetBranchAddress("genmatch2", &genmatch2, &b_genmatch2);
  treeOrig->SetBranchAddress("geniso1", &geniso1, &b_geniso1);
  treeOrig->SetBranchAddress("geniso2", &geniso2, &b_geniso2);
  treeOrig->SetBranchAddress("ptJetLead", &ptJetLead, &b_ptJetLead);
  treeOrig->SetBranchAddress("etaJetLead", &etaJetLead, &b_etaJetLead);
  treeOrig->SetBranchAddress("phiJetLead", &phiJetLead, &b_phiJetLead);
  treeOrig->SetBranchAddress("massJetLead", &massJetLead, &b_massJetLead);
  treeOrig->SetBranchAddress("indexJetLead", &indexJetLead, &b_indexJetLead);
  treeOrig->SetBranchAddress("NEMfracJet1", &NEMfracJet1, &b_NEMfracJet1);
  treeOrig->SetBranchAddress("CEMfracJet1", &CEMfracJet1, &b_CEMfracJet1);
  treeOrig->SetBranchAddress("ELfracJet1", &ELfracJet1, &b_ELfracJet1);
  treeOrig->SetBranchAddress("CHfracJet1", &CHfracJet1, &b_CHfracJet1);
  treeOrig->SetBranchAddress("NHfracJet1", &NHfracJet1, &b_NHfracJet1);
  treeOrig->SetBranchAddress("PHfracJet1", &PHfracJet1, &b_PHfracJet1);
  treeOrig->SetBranchAddress("MUfracJet1", &MUfracJet1, &b_MUfracJet1);
  treeOrig->SetBranchAddress("CHmultJet1", &CHmultJet1, &b_CHmultJet1);
  treeOrig->SetBranchAddress("NEmultJet1", &NEmultJet1, &b_NEmultJet1);
  treeOrig->SetBranchAddress("ptJetSubLead", &ptJetSubLead, &b_ptJetSubLead);
  treeOrig->SetBranchAddress("etaJetSubLead", &etaJetSubLead, &b_etaJetSubLead);
  treeOrig->SetBranchAddress("phiJetSubLead", &phiJetSubLead, &b_phiJetSubLead);
  treeOrig->SetBranchAddress("massJetSubLead", &massJetSubLead, &b_massJetSubLead);
  treeOrig->SetBranchAddress("indexJetSubLead", &indexJetSubLead, &b_indexJetSubLead);
  treeOrig->SetBranchAddress("NEMfracJet2", &NEMfracJet2, &b_NEMfracJet2);
  treeOrig->SetBranchAddress("CEMfracJet2", &CEMfracJet2, &b_CEMfracJet2);
  treeOrig->SetBranchAddress("ELfracJet2", &ELfracJet2, &b_ELfracJet2);
  treeOrig->SetBranchAddress("CHfracJet2", &CHfracJet2, &b_CHfracJet2);
  treeOrig->SetBranchAddress("NHfracJet2", &NHfracJet2, &b_NHfracJet2);
  treeOrig->SetBranchAddress("PHfracJet2", &PHfracJet2, &b_PHfracJet2);
  treeOrig->SetBranchAddress("MUfracJet2", &MUfracJet2, &b_MUfracJet2);
  treeOrig->SetBranchAddress("CHmultJet2", &CHmultJet2, &b_CHmultJet2);
  treeOrig->SetBranchAddress("NEmultJet2", &NEmultJet2, &b_NEmultJet2);
  treeOrig->SetBranchAddress("ptJet3", &ptJet3, &b_ptJet3);
  treeOrig->SetBranchAddress("etaJet3", &etaJet3, &b_etaJet3);
  treeOrig->SetBranchAddress("phiJet3", &phiJet3, &b_phiJet3);
  treeOrig->SetBranchAddress("massJet3", &massJet3, &b_massJet3);
  treeOrig->SetBranchAddress("indexJet3", &indexJet3, &b_indexJet3);
  treeOrig->SetBranchAddress("NEMfracJet3", &NEMfracJet3, &b_NEMfracJet3);
  treeOrig->SetBranchAddress("CEMfracJet3", &CEMfracJet3, &b_CEMfracJet3);
  treeOrig->SetBranchAddress("ELfracJet3", &ELfracJet3, &b_ELfracJet3);
  treeOrig->SetBranchAddress("CHfracJet3", &CHfracJet3, &b_CHfracJet3);
  treeOrig->SetBranchAddress("NHfracJet3", &NHfracJet3, &b_NHfracJet3);
  treeOrig->SetBranchAddress("PHfracJet3", &PHfracJet3, &b_PHfracJet3);
  treeOrig->SetBranchAddress("MUfracJet3", &MUfracJet3, &b_MUfracJet3);
  treeOrig->SetBranchAddress("CHmultJet3", &CHmultJet3, &b_CHmultJet3);
  treeOrig->SetBranchAddress("NEmultJet3", &NEmultJet3, &b_NEmultJet3);
  treeOrig->SetBranchAddress("ptJet4", &ptJet4, &b_ptJet4);
  treeOrig->SetBranchAddress("etaJet4", &etaJet4, &b_etaJet4);
  treeOrig->SetBranchAddress("phiJet4", &phiJet4, &b_phiJet4);
  treeOrig->SetBranchAddress("massJet4", &massJet4, &b_massJet4);
  treeOrig->SetBranchAddress("indexJet4", &indexJet4, &b_indexJet4);
  treeOrig->SetBranchAddress("NEMfracJet4", &NEMfracJet4, &b_NEMfracJet4);
  treeOrig->SetBranchAddress("CEMfracJet4", &CEMfracJet4, &b_CEMfracJet4);
  treeOrig->SetBranchAddress("ELfracJet4", &ELfracJet4, &b_ELfracJet4);
  treeOrig->SetBranchAddress("CHfracJet4", &CHfracJet4, &b_CHfracJet4);
  treeOrig->SetBranchAddress("NHfracJet4", &NHfracJet4, &b_NHfracJet4);
  treeOrig->SetBranchAddress("PHfracJet4", &PHfracJet4, &b_PHfracJet4);
  treeOrig->SetBranchAddress("MUfracJet4", &MUfracJet4, &b_MUfracJet4);
  treeOrig->SetBranchAddress("CHmultJet4", &CHmultJet4, &b_CHmultJet4);
  treeOrig->SetBranchAddress("NEmultJet4", &NEmultJet4, &b_NEmultJet4);
  treeOrig->SetBranchAddress("vtxIndex", &vtxIndex, &b_vtxIndex);
  treeOrig->SetBranchAddress("vtxX", &vtxX, &b_vtxX);
  treeOrig->SetBranchAddress("vtxY", &vtxY, &b_vtxY);
  treeOrig->SetBranchAddress("vtxZ", &vtxZ, &b_vtxZ);
  treeOrig->SetBranchAddress("vtx0Z", &vtx0Z, &b_vtx0Z);
  treeOrig->SetBranchAddress("genVtxX", &genVtxX, &b_genVtxX);
  treeOrig->SetBranchAddress("genVtxY", &genVtxY, &b_genVtxY);
  treeOrig->SetBranchAddress("genVtxZ", &genVtxZ, &b_genVtxZ);
  treeOrig->SetBranchAddress("passCHiso1", &passCHiso1, &b_passCHiso1);
  treeOrig->SetBranchAddress("passCHiso2", &passCHiso2, &b_passCHiso2);
  treeOrig->SetBranchAddress("passNHiso1", &passNHiso1, &b_passNHiso1);
  treeOrig->SetBranchAddress("passNHiso2", &passNHiso2, &b_passNHiso2);
  treeOrig->SetBranchAddress("passPHiso1", &passPHiso1, &b_passPHiso1);
  treeOrig->SetBranchAddress("passPHiso2", &passPHiso2, &b_passPHiso2);
  treeOrig->SetBranchAddress("passSieie1", &passSieie1, &b_passSieie1);
  treeOrig->SetBranchAddress("passSieie2", &passSieie2, &b_passSieie2);
  treeOrig->SetBranchAddress("passHoe1", &passHoe1, &b_passHoe1);
  treeOrig->SetBranchAddress("passHoe2", &passHoe2, &b_passHoe2);
  treeOrig->SetBranchAddress("passLooseCHiso1", &passLooseCHiso1, &b_passLooseCHiso1);
  treeOrig->SetBranchAddress("passLooseCHiso2", &passLooseCHiso2, &b_passLooseCHiso2);
  treeOrig->SetBranchAddress("passLooseNHiso1", &passLooseNHiso1, &b_passLooseNHiso1);
  treeOrig->SetBranchAddress("passLooseNHiso2", &passLooseNHiso2, &b_passLooseNHiso2);
  treeOrig->SetBranchAddress("passLoosePHiso1", &passLoosePHiso1, &b_passLoosePHiso1);
  treeOrig->SetBranchAddress("passLoosePHiso2", &passLoosePHiso2, &b_passLoosePHiso2);
  treeOrig->SetBranchAddress("passLooseSieie1", &passLooseSieie1, &b_passLooseSieie1);
  treeOrig->SetBranchAddress("passLooseSieie2", &passLooseSieie2, &b_passLooseSieie2);
  treeOrig->SetBranchAddress("passLooseHoe1", &passLooseHoe1, &b_passLooseHoe1);
  treeOrig->SetBranchAddress("passLooseHoe2", &passLooseHoe2, &b_passLooseHoe2);
  treeOrig->SetBranchAddress("passTightCHiso1", &passTightCHiso1, &b_passTightCHiso1);
  treeOrig->SetBranchAddress("passTightCHiso2", &passTightCHiso2, &b_passTightCHiso2);
  treeOrig->SetBranchAddress("passTightNHiso1", &passTightNHiso1, &b_passTightNHiso1);
  treeOrig->SetBranchAddress("passTightNHiso2", &passTightNHiso2, &b_passTightNHiso2);
  treeOrig->SetBranchAddress("passTightPHiso1", &passTightPHiso1, &b_passTightPHiso1);
  treeOrig->SetBranchAddress("passTightPHiso2", &passTightPHiso2, &b_passTightPHiso2);
  treeOrig->SetBranchAddress("passTightSieie1", &passTightSieie1, &b_passTightSieie1);
  treeOrig->SetBranchAddress("passTightSieie2", &passTightSieie2, &b_passTightSieie2);
  treeOrig->SetBranchAddress("passTightHoe1", &passTightHoe1, &b_passTightHoe1);
  treeOrig->SetBranchAddress("passTightHoe2", &passTightHoe2, &b_passTightHoe2);
  treeOrig->SetBranchAddress("hltPhoton26Photon16Mass60", &hltPhoton26Photon16Mass60, &b_hltPhoton26Photon16Mass60);
  treeOrig->SetBranchAddress("hltPhoton36Photon22Mass15", &hltPhoton36Photon22Mass15, &b_hltPhoton36Photon22Mass15);
  treeOrig->SetBranchAddress("hltPhoton42Photon25Mass15", &hltPhoton42Photon25Mass15, &b_hltPhoton42Photon25Mass15);
  treeOrig->SetBranchAddress("hltDiphoton30Mass95", &hltDiphoton30Mass95, &b_hltDiphoton30Mass95); 
  treeOrig->SetBranchAddress("hltDiphoton30Mass70", &hltDiphoton30Mass70, &b_hltDiphoton30Mass70); 
  treeOrig->SetBranchAddress("hltDiphoton30Mass55", &hltDiphoton30Mass55, &b_hltDiphoton30Mass55); 
  treeOrig->SetBranchAddress("hltDiphoton30Mass55PV", &hltDiphoton30Mass55PV, &b_hltDiphoton30Mass55PV);
  treeOrig->SetBranchAddress("hltDiphoton30Mass55EB", &hltDiphoton30Mass55EB, &b_hltDiphoton30Mass55EB);
  treeOrig->SetBranchAddress("nEle", &nEle, &b_nEle);      
  treeOrig->SetBranchAddress("nMuons", &nMuons, &b_nMuons);  
  treeOrig->SetBranchAddress("nJets", &nJets, &b_nJets);   
  treeOrig->SetBranchAddress("nLooseBjets", &nLooseBjets, &b_nLooseBjets);  
  treeOrig->SetBranchAddress("nMediumBjets", &nMediumBjets, &b_nMediumBjets);   
  treeOrig->SetBranchAddress("vhtruth", &vhtruth, &b_vhtruth);      
  treeOrig->SetBranchAddress("metF_GV",&metF_GV,&b_metF_GV);
  treeOrig->SetBranchAddress("metF_HBHENoise",&metF_HBHENoise,&b_metF_HBHENoise);
  treeOrig->SetBranchAddress("metF_HBHENoiseIso",&metF_HBHENoiseIso,&b_metF_HBHENoiseIso);
  treeOrig->SetBranchAddress("metF_CSC",&metF_CSC,&b_metF_CSC);
  treeOrig->SetBranchAddress("metF_eeBadSC",&metF_eeBadSC,&b_metF_eeBadSC);
  treeOrig->SetBranchAddress("metF_MuonBadTrack",&metF_MuonBadTrack,&b_metF_MuonBadTrack);
  treeOrig->SetBranchAddress("metF_HadronTrackRes",&metF_HadronTrackRes,&b_metF_HadronTrackRes);
  treeOrig->SetBranchAddress("higgsVtxX", &higgsVtxX, &b_higgsVtxX);
  treeOrig->SetBranchAddress("higgsVtxY", &higgsVtxY, &b_higgsVtxY);
  treeOrig->SetBranchAddress("higgsVtxZ", &higgsVtxZ, &b_higgsVtxZ);
  treeOrig->SetBranchAddress("massCorrSmear", &massCorrSmear, &b_massCorrSmear);
  treeOrig->SetBranchAddress("massCorrSmearUp", &massCorrSmearUp, &b_massCorrSmearUp);
  treeOrig->SetBranchAddress("massCorrSmearDown", &massCorrSmearDown, &b_massCorrSmearDown);
  treeOrig->SetBranchAddress("massCorrScale", &massCorrScale, &b_massCorrScale);
  treeOrig->SetBranchAddress("massCorrScaleUp", &massCorrScaleUp, &b_massCorrScaleUp);
  treeOrig->SetBranchAddress("massCorrScaleDown", &massCorrScaleDown, &b_massCorrScaleDown);
  treeOrig->SetBranchAddress("massRaw", &massRaw, &b_massRaw);
  treeOrig->SetBranchAddress("mva1", &mva1, &b_mva1);
  treeOrig->SetBranchAddress("mva2", &mva2, &b_mva2);
  treeOrig->SetBranchAddress("genZ", &genZ, &b_genZ);
  treeOrig->SetBranchAddress("ptZ", &ptZ, &b_ptZ);
  treeOrig->SetBranchAddress("etaZ", &etaZ, &b_etaZ);
  treeOrig->SetBranchAddress("phiZ", &phiZ, &b_phiZ);
  treeOrig->SetBranchAddress("BDTptgg", &BDTptgg, &b_BDTptgg);
  treeOrig->SetBranchAddress("BDTmassRaw", &BDTmassRaw, &b_BDTmassRaw);
  treeOrig->SetBranchAddress("BDTr91", &BDTr91, &b_BDTr91);
  treeOrig->SetBranchAddress("BDTr92", &BDTr92, &b_BDTr92);
  treeOrig->SetBranchAddress("BDTvtxZ", &BDTvtxZ, &b_BDTvtxZ);
  treeOrig->SetBranchAddress("BDTindex", &BDTindex, &b_BDTindex);



  // new variables to be added
  Float_t xsecWeight;
  Float_t weight;
  Float_t mggNominal;
  Float_t mggGen;

  // xsec to weight histos
  Float_t xsecToWeight = 0.;

  for(int i=0; i<(int)trees.size();i++) {
    TTree *theTreeNew = trees[i];

    // New branches
    theTreeNew->Branch("xsecWeight", &xsecWeight, "xsecWeight/F");
    theTreeNew->Branch("weight", &weight, "weight/F");
    theTreeNew->Branch("mggNominal", &mggNominal, "mggNominal/F");
    theTreeNew->Branch("mggGen", &mggGen, "mggGen/F");
    
    // Copy branches
    theTreeNew->Branch("run", &run, "run/I");
    theTreeNew->Branch("event", &event, "event/I");
    theTreeNew->Branch("lumi", &lumi, "lumi/I");
    theTreeNew->Branch("nvtx", &nvtx, "nvtx/I");
    theTreeNew->Branch("rho", &rho, "rho/F");
    theTreeNew->Branch("sampleID", &sampleID, "sampleID/I");
    theTreeNew->Branch("totXsec", &totXsec, "totXsec/F");
    theTreeNew->Branch("pu_weight", &pu_weight, "pu_weight/F");
    theTreeNew->Branch("pu_n", &pu_n, "pu_n/F");
    theTreeNew->Branch("sumDataset", &sumDataset, "sumDataset/F");
    theTreeNew->Branch("perEveW", &perEveW, "perEveW/F");
    theTreeNew->Branch("pfmet", &pfmet, "pfmet/F");
    theTreeNew->Branch("pfmetPhi", &pfmetPhi, "pfmetPhi/F");
    theTreeNew->Branch("pfmetSumEt", &pfmetSumEt, "pfmetSumEt/F");
    theTreeNew->Branch("t1pfmet", &t1pfmet, "t1pfmet/F");
    theTreeNew->Branch("t1p2pfmet", &t1p2pfmet, "b_t1p2pfmet/F");
    theTreeNew->Branch("t1pfmetJetEnUp", &t1pfmetJetEnUp, "b_t1pfmetJetEnUp/F");
    theTreeNew->Branch("t1pfmetJetEnDown", &t1pfmetJetEnDown, "b_t1pfmetJetEnDown/F");
    theTreeNew->Branch("t1pfmetJetResUp", &t1pfmetJetResUp, "b_t1pfmetJetResUp/F");
    theTreeNew->Branch("t1pfmetJetResDown", &t1pfmetJetResDown, "b_t1pfmetJetResDown/F");
    theTreeNew->Branch("t1pfmetMuonEnUp", &t1pfmetMuonEnUp, "b_t1pfmetMuonEnUp/F");
    theTreeNew->Branch("t1pfmetMuonEnDown", &t1pfmetMuonEnDown, "b_t1pfmetMuonEnDown/F");
    theTreeNew->Branch("t1pfmetElectronEnUp", &t1pfmetElectronEnUp, "b_t1pfmetElectronEnUp/F");
    theTreeNew->Branch("t1pfmetElectronEnDown", &t1pfmetElectronEnDown, "b_t1pfmetElectronEnDown/F");
    theTreeNew->Branch("t1pfmetTauEnUp", &t1pfmetTauEnUp, "b_t1pfmetTauEnUp/F");
    theTreeNew->Branch("t1pfmetTauEnDown", &t1pfmetTauEnDown, "b_t1pfmetTauEnDown/F");
    theTreeNew->Branch("t1pfmetPhotonEnUp", &t1pfmetPhotonEnUp, "b_t1pfmetPhotonEnUp/F");
    theTreeNew->Branch("t1pfmetPhotonEnDown", &t1pfmetPhotonEnDown, "b_t1pfmetPhotonEnDown/F");
    theTreeNew->Branch("t1pfmetUnclusteredEnUp", &t1pfmetUnclusteredEnUp, "b_t1pfmetUnclusteredEnUp/F");
    theTreeNew->Branch("t1pfmetUnclusteredEnDown", &t1pfmetUnclusteredEnDown, "b_t1pfmetUnclusteredEnDown/F");
    theTreeNew->Branch("t1pfmetPhi", &t1pfmetPhi, "t1pfmetPhi/F");
    theTreeNew->Branch("t1pfmetSumEt", &t1pfmetSumEt, "t1pfmetSumEt/F");
    theTreeNew->Branch("calomet", &calomet, "calomet/F");
    theTreeNew->Branch("calometPhi", &calometPhi, "calometPhi/F");
    theTreeNew->Branch("calometSumEt", &calometSumEt, "calometSumEt/F");
    theTreeNew->Branch("ptgg", &ptgg, "ptgg/F");
    theTreeNew->Branch("mgg", &mgg, "mgg/F");
    theTreeNew->Branch("eventClass", &eventClass, "eventClass/I");
    theTreeNew->Branch("pt1", &pt1, "pt1/F");
    theTreeNew->Branch("ptOverM1", &ptOverM1, "ptOverM1/F");
    theTreeNew->Branch("eta1", &eta1, "eta1/F");
    theTreeNew->Branch("phi1", &phi1, "phi1/F");
    theTreeNew->Branch("sceta1", &sceta1, "sceta1/F");
    theTreeNew->Branch("r91", &r91, "r91/F");
    theTreeNew->Branch("sieie1", &sieie1, "sieie1/F");
    theTreeNew->Branch("hoe1", &hoe1, "hoe1/F");
    theTreeNew->Branch("scRawEne1", &scRawEne1, "scRawEne1/F");
    theTreeNew->Branch("chiso1", &chiso1, "chiso1/F");
    theTreeNew->Branch("phoiso1", &phoiso1, "phoiso1/F");
    theTreeNew->Branch("neuiso1", &neuiso1, "neuiso1/F");
    theTreeNew->Branch("pt2", &pt2, "pt2/F");
    theTreeNew->Branch("ptOverM2", &ptOverM2, "ptOverM2/F");
    theTreeNew->Branch("eta2", &eta2, "eta2/F");
    theTreeNew->Branch("phi2", &phi2, "phi2/F");
    theTreeNew->Branch("sceta2", &sceta2, "sceta2/F");
    theTreeNew->Branch("r92", &r92, "r92/F");
    theTreeNew->Branch("sieie2", &sieie2, "sieie2/F");
    theTreeNew->Branch("hoe2", &hoe2, "hoe2/F");
    theTreeNew->Branch("scRawEne2", &scRawEne2, "scRawEne2/F");
    theTreeNew->Branch("chiso2", &chiso2, "chiso2/F");
    theTreeNew->Branch("phoiso2", &phoiso2, "phoiso2/F");
    theTreeNew->Branch("neuiso2", &neuiso2, "neuiso2/F");
    theTreeNew->Branch("eleveto1",&eleveto1,"eleveto1/I");
    theTreeNew->Branch("eleveto2",&eleveto2,"eleveto2/I");
    theTreeNew->Branch("presel1",&presel1,"presel1/I");
    theTreeNew->Branch("presel2",&presel2,"presel2/I");
    theTreeNew->Branch("sel1",&sel1,"sel1/I");
    theTreeNew->Branch("sel2",&sel2,"sel2/I");
    theTreeNew->Branch("tightsel1",&tightsel1,"tightsel1/I");
    theTreeNew->Branch("tightsel2",&tightsel2,"tightsel2/I");
    theTreeNew->Branch("genmatch1", &genmatch1, "genmatch1/I");
    theTreeNew->Branch("genmatch2", &genmatch2, "genmatch2/I");
    theTreeNew->Branch("geniso1", &geniso1, "geniso1/F");
    theTreeNew->Branch("geniso2", &geniso2, "geniso2/F");
    theTreeNew->Branch("ptJetLead", &ptJetLead, "ptJetLead/F");
    theTreeNew->Branch("etaJetLead", &etaJetLead, "etaJetLead/F");
    theTreeNew->Branch("phiJetLead", &phiJetLead, "phiJetLead/F");
    theTreeNew->Branch("massJetLead", &massJetLead, "massJetLead/F");
    theTreeNew->Branch("indexJetLead", &indexJetLead, "indexJetLead/I");
    theTreeNew->Branch("NEMfracJet1", &NEMfracJet1, "NEMfracJet1/F");
    theTreeNew->Branch("CEMfracJet1", &CEMfracJet1, "CEMfracJet1/F");
    theTreeNew->Branch("ELfracJet1", &ELfracJet1, "ELfracJet1/F");
    theTreeNew->Branch("CHfracJet1", &CHfracJet1, "CHfracJet1/F");
    theTreeNew->Branch("NHfracJet1", &NHfracJet1, "NHfracJet1/F");
    theTreeNew->Branch("PHfracJet1", &PHfracJet1, "PHfracJet1/F");
    theTreeNew->Branch("MUfracJet1", &MUfracJet1, "MUfracJet1/F");
    theTreeNew->Branch("CHmultJet1", &CHmultJet1, "CHmultJet1/I");
    theTreeNew->Branch("NEmultJet1", &NEmultJet1, "NEmultJet1/I");
    theTreeNew->Branch("ptJetSubLead", &ptJetSubLead, "ptJetSubLead/F");
    theTreeNew->Branch("etaJetSubLead", &etaJetSubLead, "etaJetSubLead/F");
    theTreeNew->Branch("phiJetSubLead", &phiJetSubLead, "phiJetSubLead/F");
    theTreeNew->Branch("massJetSubLead", &massJetSubLead, "massJetSubLead/F");
    theTreeNew->Branch("indexJetSubLead", &indexJetSubLead, "indexJetSubLead/I");
    theTreeNew->Branch("NEMfracJet2", &NEMfracJet2, "NEMfracJet2/F");
    theTreeNew->Branch("CEMfracJet2", &CEMfracJet2, "CEMfracJet2/F");
    theTreeNew->Branch("ELfracJet2", &ELfracJet2, "ELfracJet2/F");
    theTreeNew->Branch("CHfracJet2", &CHfracJet2, "CHfracJet2/F");
    theTreeNew->Branch("NHfracJet2", &NHfracJet2, "NHfracJet2/F");
    theTreeNew->Branch("PHfracJet2", &PHfracJet2, "PHfracJet2/F");
    theTreeNew->Branch("MUfracJet2", &MUfracJet2, "MUfracJet2/F");
    theTreeNew->Branch("CHmultJet2", &CHmultJet2, "CHmultJet2/I");
    theTreeNew->Branch("NEmultJet2", &NEmultJet2, "NEmultJet2/I");
    theTreeNew->Branch("ptJet3", &ptJet3, "ptJet3/F");
    theTreeNew->Branch("etaJet3", &etaJet3, "etaJet3/F");
    theTreeNew->Branch("phiJet3", &phiJet3, "phiJet3/F");
    theTreeNew->Branch("massJet3", &massJet3, "massJet3/F");
    theTreeNew->Branch("indexJet3", &indexJet3, "indexJet3/I");
    theTreeNew->Branch("NEMfracJet3", &NEMfracJet3, "NEMfracJet3/F");
    theTreeNew->Branch("CEMfracJet3", &CEMfracJet3, "CEMfracJet3/F");
    theTreeNew->Branch("ELfracJet3", &ELfracJet3, "ELfracJet3/F");
    theTreeNew->Branch("CHfracJet3", &CHfracJet3, "CHfracJet3/F");
    theTreeNew->Branch("NHfracJet3", &NHfracJet3, "NHfracJet3/F");
    theTreeNew->Branch("PHfracJet3", &PHfracJet3, "PHfracJet3/F");
    theTreeNew->Branch("MUfracJet3", &MUfracJet3, "MUfracJet3/F");
    theTreeNew->Branch("CHmultJet3", &CHmultJet3, "CHmultJet3/I");
    theTreeNew->Branch("NEmultJet3", &NEmultJet3, "NEmultJet3/I");
    theTreeNew->Branch("ptJet4", &ptJet4, "ptJet4/F");
    theTreeNew->Branch("etaJet4", &etaJet4, "etaJet4/F");
    theTreeNew->Branch("phiJet4", &phiJet4, "phiJet4/F");
    theTreeNew->Branch("massJet4", &massJet4, "massJet4/F");
    theTreeNew->Branch("indexJet4", &indexJet4, "indexJet4/I");
    theTreeNew->Branch("NEMfracJet4", &NEMfracJet4, "NEMfracJet4/F");
    theTreeNew->Branch("CEMfracJet4", &CEMfracJet4, "CEMfracJet4/F");
    theTreeNew->Branch("ELfracJet4", &ELfracJet4, "ELfracJet4/F");
    theTreeNew->Branch("CHfracJet4", &CHfracJet4, "CHfracJet4/F");
    theTreeNew->Branch("NHfracJet4", &NHfracJet4, "NHfracJet4/F");
    theTreeNew->Branch("PHfracJet4", &PHfracJet4, "PHfracJet4/F");
    theTreeNew->Branch("MUfracJet4", &MUfracJet4, "MUfracJet4/F");
    theTreeNew->Branch("CHmultJet4", &CHmultJet4, "CHmultJet4/I");
    theTreeNew->Branch("NEmultJet4", &NEmultJet4, "NEmultJet4/I");
    theTreeNew->Branch("vtxIndex", &vtxIndex, "vtxIndex/I");
    theTreeNew->Branch("vtxX", &vtxX, "vtxX/F");
    theTreeNew->Branch("vtxY", &vtxY, "vtxY/F");
    theTreeNew->Branch("vtxZ", &vtxZ, "vtxZ/F");
    theTreeNew->Branch("vtx0Z", &vtx0Z, "vtx0Z/F");
    theTreeNew->Branch("genVtxX", &genVtxX, "genVtxX/F");
    theTreeNew->Branch("genVtxY", &genVtxY, "genVtxY/F");
    theTreeNew->Branch("genVtxZ", &genVtxZ, "genVtxZ/F");
    theTreeNew->Branch("passCHiso1", &passCHiso1, "passCHiso1/I");
    theTreeNew->Branch("passCHiso2", &passCHiso2, "passCHiso2/I");
    theTreeNew->Branch("passNHiso1", &passNHiso1, "passNHiso1/I");
    theTreeNew->Branch("passNHiso2", &passNHiso2, "passNHiso2/I");
    theTreeNew->Branch("passPHiso1", &passPHiso1, "passPHiso1/I");
    theTreeNew->Branch("passPHiso2", &passPHiso2, "passPHiso2/I");
    theTreeNew->Branch("passSieie1", &passSieie1, "passSieie1/I");
    theTreeNew->Branch("passSieie2", &passSieie2, "passSieie2/I");
    theTreeNew->Branch("passHoe1", &passHoe1, "passHoe1/I");
    theTreeNew->Branch("passHoe2", &passHoe2, "passHoe2/I");
    theTreeNew->Branch("passLooseCHiso1", &passLooseCHiso1, "passLooseCHiso1/I");
    theTreeNew->Branch("passLooseCHiso2", &passLooseCHiso2, "passLooseCHiso2/I");
    theTreeNew->Branch("passLooseNHiso1", &passLooseNHiso1, "passLooseNHiso1/I");
    theTreeNew->Branch("passLooseNHiso2", &passLooseNHiso2, "passLooseNHiso2/I");
    theTreeNew->Branch("passLoosePHiso1", &passLoosePHiso1, "passLoosePHiso1/I");
    theTreeNew->Branch("passLoosePHiso2", &passLoosePHiso2, "passLoosePHiso2/I");
    theTreeNew->Branch("passLooseSieie1", &passLooseSieie1, "passLooseSieie1/I");
    theTreeNew->Branch("passLooseSieie2", &passLooseSieie2, "passLooseSieie2/I");
    theTreeNew->Branch("passLooseHoe1", &passLooseHoe1, "passLooseHoe1/I");
    theTreeNew->Branch("passLooseHoe2", &passLooseHoe2, "passLooseHoe2/I");
    theTreeNew->Branch("passTightCHiso1", &passTightCHiso1, "passTightCHiso1/I");
    theTreeNew->Branch("passTightCHiso2", &passTightCHiso2, "passTightCHiso2/I");
    theTreeNew->Branch("passTightNHiso1", &passTightNHiso1, "passTightNHiso1/I");
    theTreeNew->Branch("passTightNHiso2", &passTightNHiso2, "passTightNHiso2/I");
    theTreeNew->Branch("passTightPHiso1", &passTightPHiso1, "passTightPHiso1/I");
    theTreeNew->Branch("passTightPHiso2", &passTightPHiso2, "passTightPHiso2/I");
    theTreeNew->Branch("passTightSieie1", &passTightSieie1, "passTightSieie1/I");
    theTreeNew->Branch("passTightSieie2", &passTightSieie2, "passTightSieie2/I");
    theTreeNew->Branch("passTightHoe1", &passTightHoe1, "passTightHoe1/I");
    theTreeNew->Branch("passTightHoe2", &passTightHoe2, "passTightHoe2/I");
    theTreeNew->Branch("hltPhoton26Photon16Mass60", &hltPhoton26Photon16Mass60, "hltPhoton26Photon16Mass60/I");
    theTreeNew->Branch("hltPhoton36Photon22Mass15", &hltPhoton36Photon22Mass15, "hltPhoton36Photon22Mass15/I");
    theTreeNew->Branch("hltPhoton42Photon25Mass15", &hltPhoton42Photon25Mass15, "hltPhoton42Photon25Mass15/I");
    theTreeNew->Branch("hltDiphoton30Mass95", &hltDiphoton30Mass95, "hltDiphoton30Mass95/I");
    theTreeNew->Branch("hltDiphoton30Mass70", &hltDiphoton30Mass70, "hltDiphoton30Mass70/I");
    theTreeNew->Branch("hltDiphoton30Mass55", &hltDiphoton30Mass55, "hltDiphoton30Mass55/I");
    theTreeNew->Branch("hltDiphoton30Mass55PV", &hltDiphoton30Mass55PV, "hltDiphoton30Mass55PV/I");
    theTreeNew->Branch("hltDiphoton30Mass55EB", &hltDiphoton30Mass55EB, "hltDiphoton30Mass55EB/I");
    theTreeNew->Branch("nEle", &nEle, "nEle/I");      
    theTreeNew->Branch("nMuons", &nMuons, "nMuons/I");  
    theTreeNew->Branch("nJets", &nJets, "nJets/I");   
    theTreeNew->Branch("nLooseBjets", &nLooseBjets, "nLooseBjets/I");  
    theTreeNew->Branch("nMediumBjets", &nMediumBjets, "nMediumBjets/I");   
    theTreeNew->Branch("vhtruth", &vhtruth, "vhtruth/I");       
    theTreeNew->Branch("metF_GV",&metF_GV,"metF_GV/I");
    theTreeNew->Branch("metF_HBHENoise",&metF_HBHENoise,"metF_HBHENoise/I");
    theTreeNew->Branch("metF_HBHENoiseIso",&metF_HBHENoiseIso,"metF_HBHENoiseIso/I");
    theTreeNew->Branch("metF_CSC",&metF_CSC,"metF_CSC/I");
    theTreeNew->Branch("metF_eeBadSC",&metF_eeBadSC,"metF_eeBadSC/I");
    theTreeNew->Branch("metF_MuonBadTrack",&metF_MuonBadTrack,"metF_MuonBadTrack/I");
    theTreeNew->Branch("metF_HadronTrackRes",&metF_HadronTrackRes,"metF_HadronTrackRes/I");
    theTreeNew->Branch("higgsVtxX", &higgsVtxX, "higgsVtxX/F");
    theTreeNew->Branch("higgsVtxY", &higgsVtxY, "higgsVtxY/F");
    theTreeNew->Branch("higgsVtxZ", &higgsVtxZ, "higgsVtxZ/F");
    theTreeNew->Branch("massCorrSmear", &massCorrSmear, "massCorrSmear/F");
    theTreeNew->Branch("massCorrSmearUp", &massCorrSmearUp, "massCorrSmearUp/F");
    theTreeNew->Branch("massCorrSmearDown", &massCorrSmearDown, "massCorrSmearDown/F");
    theTreeNew->Branch("massCorrScale", &massCorrScale, "massCorrScale/F");
    theTreeNew->Branch("massCorrScaleUp", &massCorrScaleUp, "massCorrScaleUp/F");
    theTreeNew->Branch("massCorrScaleDown", &massCorrScaleDown, "massCorrScaleDown/F");
    theTreeNew->Branch("massRaw", &massRaw, "massRaw/F");
    theTreeNew->Branch("mva1", &mva1, "mva1/F");
    theTreeNew->Branch("mva2", &mva2, "mva2/F");
    theTreeNew->Branch("genZ", &genZ, "genZ/I");
    theTreeNew->Branch("ptZ", &ptZ,   "ptZ/F");
    theTreeNew->Branch("etaZ", &etaZ, "etaZ/F");
    theTreeNew->Branch("phiZ", &phiZ, "phiZ/F");
    theTreeNew->Branch("BDTptgg", &BDTptgg, "BDTptgg/F");
    theTreeNew->Branch("BDTmassRaw", &BDTmassRaw, "BDTmassRaw/F");
    theTreeNew->Branch("BDTr91", &BDTr91, "BDTr91/F");
    theTreeNew->Branch("BDTr92", &BDTr92, "BDTr92/F");
    theTreeNew->Branch("BDTvtxZ", &BDTvtxZ, "BDTvtxZ/F");
    theTreeNew->Branch("BDTindex", &BDTindex, "BDTindex/I");
  }
  
  for(int i=0; i<nentriesOrig; i++) {

    if (i%10000 == 0){
       std::cout << ">>> Weighting event # " << i << " / " << nentriesOrig << " entries" << std::endl; 
       //if (sampleID>=10000) std::cout << "No weighting done for Data!" << std::endl;
       //if (sampleID<=0)     std::cout << "Not valid sampleID" << std::endl;
    }
    treeOrig->GetEntry(i);

    if (i==0) xsecToWeight = totXsec;
   
    //if (sampleID>=100 && sampleID<200){//sig
    //  xsecWeight = perEveW * lumiForW * totXsec / sampleSumWeight;             
    //  weight     = xsecWeight * pu_weight * 0.001; // Margaret this was to scale the sig to 1fb since I had produced at 1pb
    //  mggNominal = massTrue;
    //  mggGen     = genmgg;
    //}

    if (sampleID>0 && sampleID<10000) { //MC
      xsecWeight = perEveW * lumiForW * totXsec / sampleSumWeight;             
      weight     = xsecWeight * pu_weight;
      mggNominal = massTrue;
      mggGen     = genmgg;
    } else { //Data   
      if (i%10000 == 0 && sampleID>=10000) std::cout << "No weighting done for Data!" << std::endl;
      xsecWeight = 1.;
      weight     = 1.;
      mggNominal = 1.;
      mggGen     = 1.;
    }

    treeNew->Fill();
  }

  // take the h_selection unscaled for h_selection_unwgt 
  h_selection_unwgt = (TH1F*)h_selection->Clone();
  h_selection_unwgt->SetName("h_selection_unwgt");
  // histo scaling to get the correct normalization
  h_selection->Scale( xsecToWeight / sampleSumWeight );  

  // new info
  fileNew->cd();
  h_entries->Write();
  h_sumW->Write();
  h_selection->Write();
  h_selection_unwgt->Write();
  treeNew->Write();
  fileNew->Close();

  fileOrig->cd();
  fileOrig->Close();
  
}


