#ifndef _comparertools_
#define _comparertools_

#include "Style.hh"

#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TROOT.h"
#include "TMath.h"
#include "TLorentzVector.h"

#include <iostream>
#include <fstream>
#include <cmath>

typedef std::vector<UInt_t>   IntVec;
typedef std::vector<Double_t> DblVec;
typedef std::vector<IntVec>   IntVecVec;
typedef std::vector<DblVec>   DblVecVec;


class CardMaker{
public:
  CardMaker(const Double_t scalefactor, const Double_t inLumi, const DblVec puweights, const TString inname, const TString outname, const Bool_t Blind, const Bool_t onesel, const TString type);
  void MakeCards();
  void SetupCutsToApply();
  void SetBranchAddresses( TTree * treeIn ); 
  void ApplyCommonSelection(const TString sampleName, const UInt_t sampleID, const UInt_t sampleNumber);
  void MakeYieldAndEfficiencyTables(const DblVecVec ND_Sig, const DblVecVec Err_ND_Sig, const DblVecVec Eff_Sig, const DblVecVec Err_Eff_Sig);
  void WriteDataCard(const TString fSigName, const Double_t ND_Sig, const UInt_t NA_Data, const UInt_t ND_Data, const DblVec ND_Res);
  Double_t GetXsec(const UInt_t mA0, const UInt_t mZp);
  ~CardMaker();

private:
  Double_t	flumi;
  TString	fType;
  Bool_t	doBlind;
  DblVec	fPUWeights;
  UInt_t	fNSamples;  
  TString	fOut;
  Double_t	alpha;
  TString	mainCut;
  Bool_t	useOneSel;

  DblVec	Cut_pt1mgg;
  DblVec	Cut_pt2mgg;
  DblVec	Cut_ptgg;
  DblVec	Cut_met;

  IntVecVec	Int_Results_NA; 
  IntVecVec	Int_Results_ND;
  DblVecVec	Dbl_Results_NA;
  DblVecVec	Dbl_Results_ND;
  IntVec	Int_Starting;
  DblVec	Dbl_Starting;
  DblVecVec	Dbl_Errors;
  DblVecVec	Dbl_Efficiency;
  DblVecVec	Dbl_Efficiency_Errors;
  DblVec	AnalyzerEff;
  DblVec	AnalyzerEffErr;
  DblVec	AnalyzerEff_unwgt;
  DblVec	AnalyzerEffErr_unwgt;

  std::ofstream fOutResultsGrid;
  std::ofstream	fOutTxtFile;

  SamplePairVec	Samples;
  TString	fInDir;
  TString	fOutDir;

  TStrVec	fDataNames;
  TStrVec	fBkgNames;
  TStrVec	fResNames;
  TStrVec	fSigNames;
  UInt_t	fNData;
  UInt_t	fNSig;
  UInt_t	fNBkg;
  UInt_t	fNRes;

  UInt_t	nentries;



  // Variables for the Trees:
  Int_t         run;
  UInt_t        event;
  Int_t         lumi;
  Float_t       rho;
  Int_t 	nvtx;
  Int_t         sampleID;
  Float_t	weight;
  Float_t	mgg;
  Float_t	ptgg;
  Float_t	t1pfmet; 
  Float_t       t1p2pfmet;
  Float_t       t1pfmetJetEnUp;
  Float_t       t1pfmetJetEnDown;
  Float_t       t1pfmetJetResUp;
  Float_t       t1pfmetJetResDown;
  Float_t       t1pfmetMuonEnUp;
  Float_t       t1pfmetMuonEnDown;
  Float_t       t1pfmetElectronEnUp;
  Float_t       t1pfmetElectronEnDown;
  Float_t       t1pfmetTauEnUp;
  Float_t       t1pfmetTauEnDown;
  Float_t       t1pfmetPhotonEnUp;
  Float_t       t1pfmetPhotonEnDown;
  Float_t       t1pfmetUnclusteredEnUp;
  Float_t       t1pfmetUnclusteredEnDown;
  Float_t	t1pfmetPhi; 
  Float_t	t1pfmetCorr; 
  Float_t	t1pfmetCorrPhi; 
  Float_t	t1pfmetSumEt; 
  Float_t	pfmet; 
  Float_t	pfmetphi; 
  Float_t	pfmetSumEt; 
  Float_t	calomet; 
  Float_t	calometphi; 
  Float_t	calometSumEt; 
  Int_t         genmatch1;
  Int_t         genmatch2;
  Float_t	pt1;
  Float_t	pt2;
  Float_t	phi1;
  Float_t	phi2;
  Float_t	eta1;
  Float_t	eta2;
  Float_t	r91;
  Float_t	r92;
  Float_t	phoiso1;
  Float_t	phoiso2;
  Float_t	chiso1;
  Float_t	chiso2;
  Float_t	neuiso1;
  Float_t	neuiso2;
  Float_t	sieie1;
  Float_t	sieie2;
  Float_t	hoe1;
  Float_t	hoe2;
  Int_t		passCHiso1;
  Int_t		passCHiso2;
  Int_t		passNHiso1;
  Int_t		passNHiso2;
  Int_t		passPHiso1;
  Int_t		passPHiso2;
  Int_t		passSieie1;
  Int_t		passSieie2;
  Int_t		passHoe1;
  Int_t		passHoe2;
  Int_t         passLooseCHiso1;
  Int_t         passLooseCHiso2;
  Int_t         passLooseNHiso1;
  Int_t         passLooseNHiso2;
  Int_t         passLoosePHiso1;
  Int_t         passLoosePHiso2;
  Int_t         passLooseSieie1;
  Int_t         passLooseSieie2;
  Int_t         passLooseHoe1;
  Int_t         passLooseHoe2;
  Int_t         passTightCHiso1;
  Int_t         passTightCHiso2;
  Int_t         passTightNHiso1;
  Int_t         passTightNHiso2;
  Int_t         passTightPHiso1;
  Int_t         passTightPHiso2;
  Int_t         passTightSieie1;
  Int_t         passTightSieie2;
  Int_t         passTightHoe1;
  Int_t         passTightHoe2;
  Int_t 	eleveto1;
  Int_t 	eleveto2;
  Int_t		sel1;
  Int_t		sel2;
  Int_t		presel1;
  Int_t		presel2;
  Int_t		hltPhoton26Photon16Mass60;
  Int_t		hltPhoton36Photon22Mass15;
  Int_t		hltPhoton42Photon25Mass15;
  Int_t		hltDiphoton30Mass95;
  Int_t		hltDiphoton30Mass70;
  Int_t		hltDiphoton30Mass55;
  Int_t		hltDiphoton30Mass55PV;
  Int_t		hltDiphoton30Mass55EB;
  Float_t       ptJetLead;
  Float_t       etaJetLead;
  Float_t       phiJetLead;
  Float_t       massJetLead;
  Int_t         indexJetLead;
  Float_t       NEMfracJet1;
  Float_t       CEMfracJet1;
  Float_t       ELfracJet1;
  Float_t       CHfracJet1;
  Float_t       NHfracJet1;
  Float_t       PHfracJet1;
  Float_t       MUfracJet1;
  Int_t         CHmultJet1;
  Int_t         NEmultJet1;
  Float_t       ptJetSubLead;
  Float_t       etaJetSubLead;
  Float_t       phiJetSubLead;
  Float_t       massJetSubLead;
  Int_t         indexJetSubLead;
  Float_t       NEMfracJet2;
  Float_t       CEMfracJet2;
  Float_t       ELfracJet2;
  Float_t       CHfracJet2;
  Float_t       NHfracJet2;
  Float_t       PHfracJet2;
  Float_t       MUfracJet2;
  Int_t         CHmultJet2;
  Int_t         NEmultJet2;
  Float_t       ptJet3;
  Float_t       etaJet3;
  Float_t       phiJet3;
  Float_t       massJet3;
  Int_t         indexJet3;
  Float_t       NEMfracJet3;
  Float_t       CEMfracJet3;
  Float_t       ELfracJet3;
  Float_t       CHfracJet3;
  Float_t       NHfracJet3;
  Float_t       PHfracJet3;
  Float_t       MUfracJet3;
  Int_t         CHmultJet3;
  Int_t         NEmultJet3;
  Float_t       ptJet4;
  Float_t       etaJet4;
  Float_t       phiJet4;
  Float_t       massJet4;
  Int_t         indexJet4;
  Float_t       NEMfracJet4;
  Float_t       CEMfracJet4;
  Float_t       ELfracJet4;
  Float_t       CHfracJet4;
  Float_t       NHfracJet4;
  Float_t       PHfracJet4;
  Float_t       MUfracJet4;
  Int_t         CHmultJet4;
  Int_t         NEmultJet4;
  Int_t         nEle;
  Int_t         nMuons;
  Int_t         nJets20;
  Int_t         nJets30;
  Int_t         nJets40;
  Int_t         nJets50;
  Int_t         nLooseBjets;
  Int_t         nMediumBjets;
  Int_t         vhtruth;

  Int_t         metF_GV;
  Int_t		metF_globalTightHalo;
  Int_t         metF_HBHENoise;
  Int_t         metF_HBHENoiseIso;
  Int_t         metF_CSC;
  Int_t         metF_eeBadSC;
  Int_t         metF_MuonBadTrack;
  Int_t         metF_HadronTrackRes;
  Int_t		metF_EcalDeadCell;
  Int_t		metF_badMuon;
  Int_t		metF_badChargedHadron;

  Int_t		vtxIndex;
  Float_t       vtxX;
  Float_t       vtxY;
  Float_t       vtxZ;
  Float_t       vtx0Z;
  Float_t       genVtxX;
  Float_t       genVtxY;
  Float_t       genVtxZ;
  Float_t       higgsVtxX;
  Float_t       higgsVtxY;
  Float_t       higgsVtxZ;
  Float_t       massCorrSmear;
  Float_t       massCorrSmearUp;
  Float_t       massCorrSmearDown;
  Float_t       massCorrScale;
  Float_t       massCorrScaleUp;
  Float_t       massCorrScaleDown;
  Float_t       massRaw;
  Float_t       mva1;
  Float_t       mva2;
  Int_t         genZ;
  Float_t       ptZ;
  Float_t       etaZ;
  Float_t       phiZ;
  Float_t	BDTptgg;
  Float_t	BDTmassRaw;
  Float_t	BDTr91;
  Float_t	BDTr92;
  Float_t	BDTvtxZ;
  Int_t		BDTindex;


  // branches
  TBranch       *b_run;   //!
  TBranch       *b_event;   //!
  TBranch       *b_lumi;   //!
  TBranch       *b_rho;   //!
  TBranch       *b_sampleID;   //!
  TBranch       *b_totXsec;   //!
  TBranch 	*b_nvtx;
  TBranch	*b_weight;
  TBranch	*b_mgg;
  TBranch	*b_ptgg;
  TBranch	*b_t1pfmet;
  TBranch       *b_t1p2pfmet;   //!
  TBranch       *b_t1pfmetJetEnUp;   //!
  TBranch       *b_t1pfmetJetEnDown;   //!
  TBranch       *b_t1pfmetJetResUp;   //!
  TBranch       *b_t1pfmetJetResDown;   //!
  TBranch       *b_t1pfmetMuonEnUp;   //!
  TBranch       *b_t1pfmetMuonEnDown;   //!
  TBranch       *b_t1pfmetElectronEnUp;   //!
  TBranch       *b_t1pfmetElectronEnDown;   //!
  TBranch       *b_t1pfmetTauEnUp;   //!
  TBranch       *b_t1pfmetTauEnDown;   //!
  TBranch       *b_t1pfmetPhotonEnUp;   //!
  TBranch       *b_t1pfmetPhotonEnDown;   //!
  TBranch       *b_t1pfmetUnclusteredEnUp;   //!
  TBranch       *b_t1pfmetUnclusteredEnDown;   //!
  TBranch	*b_t1pfmetPhi;
  TBranch	*b_t1pfmetCorr;
  TBranch	*b_t1pfmetCorrPhi;
  TBranch	*b_t1pfmetSumEt;
  TBranch	*b_pfmet;
  TBranch	*b_pfmetPhi;
  TBranch	*b_pfmetSumEt;
  TBranch	*b_calomet;
  TBranch	*b_calometPhi;
  TBranch	*b_calometSumEt;
  TBranch	*b_genmatch1;
  TBranch	*b_genmatch2;
  TBranch	*b_pt1;
  TBranch	*b_pt2;
  TBranch	*b_phi1;
  TBranch	*b_phi2;
  TBranch	*b_eta1;
  TBranch	*b_eta2;
  TBranch	*b_r91;
  TBranch	*b_r92;
  TBranch	*b_phoiso1;
  TBranch	*b_phoiso2;
  TBranch	*b_chiso1;
  TBranch	*b_chiso2;
  TBranch	*b_neuiso1;
  TBranch	*b_neuiso2;
  TBranch	*b_sieie1;
  TBranch	*b_sieie2;
  TBranch	*b_hoe1;
  TBranch	*b_hoe2;
  TBranch	*b_passCHiso1;
  TBranch	*b_passCHiso2;
  TBranch	*b_passNHiso1;
  TBranch	*b_passNHiso2;
  TBranch	*b_passPHiso1;
  TBranch	*b_passPHiso2;
  TBranch	*b_passSieie1;
  TBranch	*b_passSieie2;
  TBranch	*b_passHoe1;
  TBranch	*b_passHoe2;
  TBranch       *b_passLooseCHiso1;   //!
  TBranch       *b_passLooseCHiso2;   //!
  TBranch       *b_passLooseNHiso1;   //!
  TBranch       *b_passLooseNHiso2;   //!
  TBranch       *b_passLoosePHiso1;   //!
  TBranch       *b_passLoosePHiso2;   //!
  TBranch       *b_passLooseSieie1;   //!
  TBranch       *b_passLooseSieie2;   //!
  TBranch       *b_passLooseHoe1;   //!
  TBranch       *b_passLooseHoe2;   //!
  TBranch       *b_passTightCHiso1;   //!
  TBranch       *b_passTightCHiso2;   //!
  TBranch       *b_passTightNHiso1;   //!
  TBranch       *b_passTightNHiso2;   //!
  TBranch       *b_passTightPHiso1;   //!
  TBranch       *b_passTightPHiso2;   //!
  TBranch       *b_passTightSieie1;   //!
  TBranch       *b_passTightSieie2;   //!
  TBranch       *b_passTightHoe1;   //!
  TBranch       *b_passTightHoe2;   //!
  TBranch	*b_eleveto1;
  TBranch	*b_eleveto2;
  TBranch	*b_sel1;
  TBranch	*b_sel2;
  TBranch	*b_presel1;
  TBranch	*b_presel2;
  TBranch	*b_hltPhoton26Photon16Mass60;
  TBranch	*b_hltPhoton36Photon22Mass15;
  TBranch	*b_hltPhoton42Photon25Mass15;
  TBranch	*b_hltDiphoton30Mass95;
  TBranch	*b_hltDiphoton30Mass70;
  TBranch	*b_hltDiphoton30Mass55;
  TBranch	*b_hltDiphoton30Mass55PV;
  TBranch	*b_hltDiphoton30Mass55EB;
  TBranch       *b_nEle;   //!
  TBranch       *b_nMuons;   //!
  TBranch       *b_nJets20;   //!
  TBranch       *b_nJets30;   //!
  TBranch       *b_nJets40;   //!
  TBranch       *b_nJets50;   //!
  TBranch       *b_nLooseBjets;   //!
  TBranch       *b_nMediumBjets;   //!
  TBranch       *b_ptJetLead;   //!
  TBranch       *b_etaJetLead;   //!
  TBranch       *b_phiJetLead;   //!
  TBranch       *b_massJetLead;   //!
  TBranch       *b_indexJetLead;   //!
  TBranch       *b_NEMfracJet1;   //!
  TBranch       *b_CEMfracJet1;   //!
  TBranch       *b_ELfracJet1;   //!
  TBranch       *b_CHfracJet1;   //!
  TBranch       *b_NHfracJet1;   //!
  TBranch       *b_PHfracJet1;   //!
  TBranch       *b_MUfracJet1;   //!
  TBranch       *b_CHmultJet1;   //!
  TBranch       *b_NEmultJet1;   //!
  TBranch       *b_ptJetSubLead;   //!
  TBranch       *b_etaJetSubLead;   //!
  TBranch       *b_phiJetSubLead;   //!
  TBranch       *b_massJetSubLead;   //!
  TBranch       *b_indexJetSubLead;   //!
  TBranch       *b_NEMfracJet2;   //!
  TBranch       *b_CEMfracJet2;   //!
  TBranch       *b_ELfracJet2;   //!
  TBranch       *b_CHfracJet2;   //!
  TBranch       *b_NHfracJet2;   //!
  TBranch       *b_PHfracJet2;   //!
  TBranch       *b_MUfracJet2;   //!
  TBranch       *b_CHmultJet2;   //!
  TBranch       *b_NEmultJet2;   //!
  TBranch       *b_ptJet3;   //!
  TBranch       *b_etaJet3;   //!
  TBranch       *b_phiJet3;   //!
  TBranch       *b_massJet3;   //!
  TBranch       *b_indexJet3;   //!
  TBranch       *b_NEMfracJet3;   //!
  TBranch       *b_CEMfracJet3;   //!
  TBranch       *b_ELfracJet3;   //!
  TBranch       *b_CHfracJet3;   //!
  TBranch       *b_NHfracJet3;   //!
  TBranch       *b_PHfracJet3;   //!
  TBranch       *b_MUfracJet3;   //!
  TBranch       *b_CHmultJet3;   //!
  TBranch       *b_NEmultJet3;   //!
  TBranch       *b_ptJet4;   //!
  TBranch       *b_etaJet4;   //!
  TBranch       *b_phiJet4;   //!
  TBranch       *b_massJet4;   //!
  TBranch       *b_indexJet4;   //!
  TBranch       *b_NEMfracJet4;   //!
  TBranch       *b_CEMfracJet4;   //!
  TBranch       *b_ELfracJet4;   //!
  TBranch       *b_CHfracJet4;   //!
  TBranch       *b_NHfracJet4;   //!
  TBranch       *b_PHfracJet4;   //!
  TBranch       *b_MUfracJet4;   //!
  TBranch       *b_CHmultJet4;   //!
  TBranch       *b_NEmultJet4;   //!
  TBranch       *b_vhtruth;   //!

  TBranch       *b_metF_GV;   //!
  TBranch       *b_metF_HBHENoise;   //!
  TBranch       *b_metF_HBHENoiseIso;   //!
  TBranch       *b_metF_CSC;   //!
  TBranch       *b_metF_eeBadSC;   //!
  TBranch       *b_metF_MuonBadTrack;   //!
  TBranch       *b_metF_HadronTrackRes;   //!
  TBranch	*b_metF_globalTightHalo;
  TBranch	*b_metF_EcalDeadCell;
  TBranch	*b_metF_badMuon;
  TBranch	*b_metF_badChargedHadron;

  TBranch       *b_higgsVtxX;   //!
  TBranch       *b_higgsVtxY;   //!
  TBranch       *b_higgsVtxZ;   //!
  TBranch       *b_massCorrSmear;   //!
  TBranch       *b_massCorrSmearUp;   //!
  TBranch       *b_massCorrSmearDown;   //!
  TBranch       *b_massCorrScale;   //!
  TBranch       *b_massCorrScaleUp;   //!
  TBranch       *b_massCorrScaleDown;   //!
  TBranch       *b_massRaw;   //!
  TBranch       *b_mva1;   //!
  TBranch       *b_mva2;   //!
  TBranch       *b_genZ;   //!
  TBranch       *b_ptZ;   //!
  TBranch       *b_etaZ;   //!
  TBranch       *b_phiZ;   //!
  TBranch       *b_vtxIndex;
  TBranch       *b_vtxX;
  TBranch       *b_vtxY;
  TBranch       *b_vtxZ;
  TBranch       *b_vtx0Z;
  TBranch       *b_genVtxX;
  TBranch       *b_genVtxY;
  TBranch       *b_genVtxZ;
  TBranch	*b_BDTptgg;
  TBranch	*b_BDTmassRaw;
  TBranch	*b_BDTr91;
  TBranch	*b_BDTr92;
  TBranch	*b_BDTvtxZ;
  TBranch	*b_BDTindex;

};
#endif
