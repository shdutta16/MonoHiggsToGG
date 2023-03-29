#include "Plotter.hh"
#include "Style.hh"
#include "../../../DataFormats/Math/interface/deltaPhi.h"
#include "deltaR.h"
#include "mkPlotsLivia/CMS_lumi.C"

Plotter::Plotter( TString inName, TString outName, TString inSpecies, const DblVec puweights, const Double_t lumi, Bool_t Data, Bool_t Blind, TString type, const DblVec metCorr, const int whichSelection){

  fType = type;  
  isData = Data;
  doBlind = Blind;
  fMETCorr = metCorr;
  fWhichSel = whichSelection;

  // Get ROOT file
  name = inName;
  species = inSpecies;
  inFile = TFile::Open(Form("%s%s.root",name.Data(),species.Data()));
  CheckValidFile(inFile,Form("%s%s.root",name.Data(),species.Data()));  
  // Open Tree from inFile
  tpho = (TTree*)inFile->Get("DiPhotonTree"); 
  CheckValidTree(tpho,"DiPhotonTree",Form("%s%s.root",name.Data(),species.Data()));

  fLumi = lumi;
  fPUWeights = puweights;

  fSelection.resize(8);
  fSelection_unwgt.resize(8);
  TH1D *fSel = (TH1D*)inFile->Get("h_selection");
  TH1D *fSel_unwgt = (TH1D*)inFile->Get("h_selection_unwgt");
  CheckValidTH1D(fSel,"h_selection",Form("%s%s.root",name.Data(),species.Data()));
  CheckValidTH1D(fSel_unwgt,"h_selection_unwgt",Form("%s%s.root",name.Data(),species.Data()));

  if (!(isData && doBlind)){
    for (UInt_t i=0; i<8; i++){ 
      // values of bin i correspond to passing (all cuts previous + one listed below):  
      // 1=trigger, 2=presel, 3=selection, 4=pt1>30,pt2>20, 5=pt1>mgg/3,pt2>mgg/4, 6=goodVtx, 7=mgg, 8=met
      fSelection[i]=fSel->GetBinContent(i+1);
      fSelection_unwgt[i]=fSel_unwgt->GetBinContent(i+1);
    }
    //fTH1DMap["selection_unwgt"] = (TH1D*)fSel_unwgt->Clone();
  }
  std::cout << "Finished getting the h_selection & h_selection_unwgt" << std::endl;  

  // Make TLorentzVector for the photons
  TLorentzVector *fLorenzVec1		= new TLorentzVector();
  TLorentzVector *fLorenzVec2		= new TLorentzVector();
  TLorentzVector *fLorenzVecgg		= new TLorentzVector();
  TLorentzVector *fLorenzVecJet1	= new TLorentzVector();
  TLorentzVector *fLorenzVecJet2	= new TLorentzVector();
  TLorentzVector *fLorenzVecJet3	= new TLorentzVector();
  TLorentzVector *fLorenzVecJet4	= new TLorentzVector();

  // Make output directory
  fName = outName;
  TString FullPath = fName.Data();
  FullPath+=species.Data();
  FullPath+="/";
  MakeOutDirectory(FullPath.Data());

  // Make output ROOT file
  outFile = new TFile(Form("%s/%s/plots_%s.root",fName.Data(),species.Data(),species.Data()),"RECREATE");
  CheckValidFile(outFile,Form("%s/%s/plots_%s.root",fName.Data(),species.Data(),species.Data()));

  // set all the branch addresses appropriately
  Plotter::SetBranchAddresses();

}// end Plotter::Plotter


Plotter::~Plotter(){
  std::cout << "Finished & Deleting" <<std::endl;
  std::cout << "Deleting inTree" <<std::endl;
  delete tpho;
  std::cout << "Deleting inFile" <<std::endl;
  delete inFile;
  // Write and Close output ROOT file
  //Plotter::DeleteBranches();
  std::cout << "Delete histos" <<std::endl;
  Plotter::DeleteHists();
  std::cout << "Deleting outFile" <<std::endl;
  delete outFile;
  std::cout << "Finished Deleting" <<std::endl;
}// end Plotter::~Plotter


void Plotter::DoPlots(int prompt){
  Plotter::SetUpPlots();
 
  nentries = tpho->GetEntries(); 
  std::cout << "nentries = " << nentries << std::endl;
  
  Double_t effPUn[100]={0};
  Double_t effPUd[100]={0};
  Double_t effptn[100]={0};
  Double_t effptd[100]={0};

  fTH1DMap["hlt"]->Fill(0.5,nentries);
  // fSelection[i]-> i: 0=trigger, 1=presel, 2=selection, 3=pt1>30,pt2>20, 4=pt1>mgg/3,pt2>mgg/4, 5=goodVtx, 6=mgg, 7=met
  for (UInt_t i=0; i<8; i++){
    fTH1DMap["selection"]->Fill(i+0.5,fSelection[i]);
    fTH1DMap["selection_unwgt"]->Fill(i+0.5,fSelection_unwgt[i]);
  }

  Double_t numMargaretMETfil   = 0; 
  Double_t numMargaretSel      = 0;
  Double_t numMargaretTrig     = 0;
  Double_t numMargaretDupRem   = 0;
  Double_t numMargaretLepVeto  = 0;
  Double_t numMargaretmgg      = 0;
  Double_t numMargaretMETcut   = 0;
  Double_t numMargaretmaxggMET = 0;
  Double_t numMargaretmaxjMET  = 0;
  Double_t numMargaretminjMET  = 0;
  Double_t numMargaret_METCorr = 0;
  Double_t numMargaret_METCorrMETcut = 0;
  Double_t numMargaret_METUncorr = 0;
  Double_t numMargaret_METUncorrMETcut = 0;

  
  //Int_t numMargaretMETfil   = 0; 
  //Int_t numMargaretSel      = 0;
  //Int_t numMargaretTrig     = 0;
  //Int_t numMargaretDupRem   = 0;
  //Int_t numMargaretLepVeto  = 0;
  //Int_t numMargaretmgg      = 0;
  //Int_t numMargaretMETcut   = 0;
  //Int_t numMargaretmaxggMET = 0;
  //Int_t numMargaretmaxjMET  = 0;
  //Int_t numMargaretminjMET  = 0;
  //Int_t numMargaret_METCorr = 0;
  //Int_t numMargaret_METCorrMETcut = 0;
  //Int_t numMargaret_METUncorr = 0;
  //Int_t numMargaret_METUncorrMETcut = 0;

  Int_t numFailingMETfil = 0;

  Int_t nData = 0;
  Int_t nDataMETfil1 = 0;
  Int_t nDataMETfil2 = 0;
  Int_t nDataMETfil3 = 0;
  Int_t nDataMETfil4 = 0;
  Int_t nDataMETfil5 = 0;
  Int_t nDataMETfil6 = 0;
  Int_t nDataMETfil7 = 0;
  
  Int_t nDataOrig = 0;
  Int_t nDatadphi1 = 0;
  Int_t nDatadphi2 = 0;
  Int_t nDatadphi3 = 0;

  Int_t nEvents_allVtx = 0;
  Int_t nEvents_goodVtx = 0;
  Int_t nEvents_goodVtx0 = 0; 
 
  Double_t METcut = 70;// METcut to apply for OrigSel
  //if (fWhichSel == 1) METcut = 105;// OptSel1
  if (fWhichSel == 1) METcut = 130;// OptSel1
  if (fWhichSel == 2) METcut = 95;//  OptSel2
  if (fWhichSel == 3) METcut = 50;//  OptSel3
  if (fWhichSel == 4) METcut = 70;//  OptSel4

  for (UInt_t entry = 0; entry < nentries; entry++){
    tpho->GetEntry(entry);

    // Fill TLorentzVector
    fLorenzVec1.SetPtEtaPhiM(pt1,eta1,phi1,0.);
    fLorenzVec2.SetPtEtaPhiM(pt2,eta2,phi2,0.);
    fLorenzVecgg = fLorenzVec1 + fLorenzVec2;
    fLorenzVecJet1.SetPtEtaPhiM(ptJetLead,etaJetLead,phiJetLead,massJetLead);
    fLorenzVecJet2.SetPtEtaPhiM(ptJetSubLead,etaJetSubLead,phiJetSubLead,massJetSubLead);
    fLorenzVecJet3.SetPtEtaPhiM(ptJet3,etaJet3,phiJet3,massJet3);
    fLorenzVecJet4.SetPtEtaPhiM(ptJet4,etaJet4,phiJet4,massJet4);

    // calculate the weight
    Double_t Weight = (weight);//*fPUWeights[nvtx];// PURW[0] corresponds to bin1=0vtx

    if (hltPhoton26Photon16Mass60 == 1) fTH1DMap["hlt"]->Fill(1.5,1);
    if (hltPhoton36Photon22Mass15 == 1) fTH1DMap["hlt"]->Fill(2.5,1);
    if (hltPhoton42Photon25Mass15 == 1) fTH1DMap["hlt"]->Fill(3.5,1);
    if (hltDiphoton30Mass95 == 1)   	fTH1DMap["hlt"]->Fill(4.5,1);
    if (hltDiphoton30Mass70 == 1)   	fTH1DMap["hlt"]->Fill(5.5,1);
    if (hltDiphoton30Mass55 == 1)   	fTH1DMap["hlt"]->Fill(6.5,1);
    if (hltDiphoton30Mass55PV == 1) 	fTH1DMap["hlt"]->Fill(7.5,1);
    if (hltDiphoton30Mass55EB == 1) 	fTH1DMap["hlt"]->Fill(8.5,1);

    fTH1DMap["eff_sel"]->Fill(0.5,Weight);

    Bool_t passCH1  = false;
    Bool_t passCH2  = false;
    Bool_t passNH1  = false;
    Bool_t passNH2  = false;
    Bool_t passPH1  = false;
    Bool_t passPH2  = false;
    Bool_t passS1   = false;
    Bool_t passS2   = false;
    Bool_t passHE1  = false;
    Bool_t passHE2  = false;
    Bool_t passAll1 = false;
    Bool_t passAll2 = false;
    Bool_t passBoth = false;
    Bool_t passEV1  = false; 
    Bool_t passEV2  = false; 

    // For LOOSE Photon ID Working Point
    // Can replace "Loose" with "Tight" 
    // OR remove "Loose" for Medium WP
    if (passLooseCHiso1==1) passCH1 = true; 
    if (passLooseCHiso2==1) passCH2 = true; 
    if (passLooseNHiso1==1) passNH1 = true;
    if (passLooseNHiso2==1) passNH2 = true;
    if (passLoosePHiso1==1) passPH1 = true;
    if (passLoosePHiso2==1) passPH2 = true;
    if (passLooseSieie1==1) passS1  = true;
    if (passLooseSieie2==1) passS2  = true;
    if (passLooseHoe1==1)   passHE1 = true; 
    if (passLooseHoe2==1)   passHE2 = true; 
    if (eleveto1==1)	    passEV1 = true;
    if (eleveto2==1)	    passEV2 = true; 

    //if (!passCH1) std::cout << "Fails CHIso1" << std::endl;
    //if (!passCH2) std::cout << "Fails CHIso2" << std::endl;
    //if (!passNH1) std::cout << "Fails NHIso1" << std::endl;
    //if (!passNH2) std::cout << "Fails NHIso2" << std::endl;
    //if (!passPH1) std::cout << "Fails PHIso1" << std::endl;
    //if (!passPH2) std::cout << "Fails PHIso2" << std::endl;
    //if (!passS1)  std::cout << "Fails SIEIE1" << std::endl;
    //if (!passS2)  std::cout << "Fails SIEIE2" << std::endl;
    //if (!passHE1) std::cout << "Fails HoE1"   << std::endl;
    //if (!passHE2) std::cout << "Fails HoE2"   << std::endl;

    if (passCH1 && passNH1 && passPH1 && passS1 && passHE1 && passEV1) passAll1 = true;
    if (passCH2 && passNH2 && passPH2 && passS2 && passHE2 && passEV2) passAll2 = true;
    if (passAll1 && passAll2) passBoth = true;


    Bool_t EB1, EB2, EE1, EE2, inEE, inEB;
    Bool_t hiR9, loR9;

    // Check if the Data passes MET filters
    Bool_t passMetFil_All   = (metF_GV && metF_globalTightHalo && metF_HBHENoise && metF_HBHENoiseIso && metF_EcalDeadCell && metF_badMuon && metF_badChargedHadron);
    Bool_t passMetFil_Data  = (isData)?(metF_eeBadSC):true; // filter for data only
    Bool_t passMETfil       = (passMetFil_All && passMetFil_Data);
    if (!passMETfil) numFailingMETfil++;

    // Count number of events failing MET filters
    if (isData && hltDiphoton30Mass95==1){
      nData++;
      if (metF_GV==1){
	nDataMETfil1++;
	if (metF_eeBadSC==1){
	  nDataMETfil2++;
	  if (metF_HBHENoiseIso==1){
	    nDataMETfil3++;
	    if (metF_HBHENoise==1){
	      nDataMETfil4++;
	      if (metF_MuonBadTrack==1){
		nDataMETfil5++;
		if (metF_HadronTrackRes==1){
		  nDataMETfil6++;
		  if (metF_CSC==1){
		    nDataMETfil7++;
		  }
		}
	      }
	    }
	  }	
	}
      }
    } 

    fTH1DMap["pt1_beforeIDloose"]->Fill(pt1);
    fTH1DMap["eta1_beforeIDloose"]->Fill(eta1);
    if (passBoth){// check that diphoton passes the loose ID
      fTH1DMap["pt1_afterIDloose"]->Fill(pt1);
      fTH1DMap["eta1_afterIDloose"]->Fill(eta1);
    }

    // t1pfmet phi Correction
    Double_t t1pfmetCorrX;
    Double_t t1pfmetCorrY;
    Double_t t1pfmetCorrE;
    t1pfmetCorrX = t1pfmet*cos(t1pfmetPhi) - (fMETCorr[0] + fMETCorr[1]*t1pfmetSumEt);
    t1pfmetCorrY = t1pfmet*sin(t1pfmetPhi) - (fMETCorr[2] + fMETCorr[3]*t1pfmetSumEt);
    //std::cout << "px = t1pfmet*cos(t1pfmetPhi) - (" << fMETCorr[0] << " + " << fMETCorr[1] << "*t1pfmetSumEt)" << std::endl;
    //std::cout << "py = t1pfmet*sin(t1pfmetPhi) - (" << fMETCorr[2] << " + " << fMETCorr[3] << "*t1pfmetSumEt)" << std::endl;
    t1pfmetCorrE = sqrt(t1pfmetCorrX*t1pfmetCorrX + t1pfmetCorrY*t1pfmetCorrY);
    TLorentzVector correctedMet;
    correctedMet.SetPxPyPzE(t1pfmetCorrX,t1pfmetCorrY,0,t1pfmetCorrE);
    TLorentzVector fLorenzVecZp;
    fLorenzVecZp = correctedMet + fLorenzVecgg;
    //Double_t t1pfmetPhiCorr   = correctedMet.Phi(); 
    //Double_t t1pfmetCorr      = correctedMet.Pt();
    //Double_t t1pfmetSumEtCorr = correctedMet.Et();

    Double_t t1pfmetPhiCorr   = t1pfmetCorrPhi; 
    //Double_t t1pfmetCorr      = t1pfmetCorr;
    Double_t t1pfmetSumEtCorr = t1pfmetSumEt;

    // START full selection for plots
    if (passMETfil){ //Data passes MET filter
      //numMargaretMETfil++;
      numMargaretMETfil += Weight;

      /////////////////////////////////////////////////////////////////
      // OptSel1 = using ptgg 	  & optimized w/o requirement on number of events 
      // OptSel2 = using ptgg/MET & optimized w/o requirement on number of events
      // OptSel3 = using ptgg	  & optimized w/ requirement on number of events
      // OptSel4 = using ptgg/MET & optimized w/ requirement on number of events
      /////////////////////////////////////////////////////////////////
      bool passTheSel = false;
      if (fWhichSel==0) passTheSel = true; // Orignal Selection
      if (fWhichSel==1 && (pt1 > 0.50*mgg && pt2 > 0.25*mgg /*&& ptgg > 90*/)) 		passTheSel = true;// OptSel1 (for M600 point)
      if (fWhichSel==2 && (pt1 > 0.55*mgg && pt2 > 0.25*mgg && ptgg/t1pfmetCorr > 0.5)) passTheSel = true;// OptSel2 (for M600 point)
      if (fWhichSel==3 && (pt1 > 0.55*mgg && pt2 > 0.25*mgg && ptgg > 90)) 		passTheSel = true;// OptSel3
      if (fWhichSel==4 && (pt1 > 0.45*mgg && pt2 > 0.25*mgg && ptgg/t1pfmetCorr > 0.2)) passTheSel = true;// OptSel4

      if (passTheSel){
        //numMargaretSel++;
        numMargaretSel += Weight;
        fTH1DMap["eff_sel"]->Fill(1.5,Weight);
        if (!isData || (isData && hltDiphoton30Mass95==1)){ // data has to pass trigger
          //numMargaretTrig++;
          numMargaretTrig += Weight;
          // to remove duplicate events
          // when genmatch is 1 indicates the photon is prompt  
          if (prompt==1 /*GJet*/ && (genmatch1==1 && genmatch2==1)) continue;   // only PF and FF for GJets  
          if (prompt==2 /*QCD */ && (genmatch1==1 && genmatch2==1)) continue;   // only FF for QCD       
          //numMargaretDupRem++;
          numMargaretDupRem+=Weight;

	  fTH1DMap["nJets"]->Fill(nJets30,Weight);
	  fTH1DMap["nElec"]->Fill(nEle,Weight);
	  fTH1DMap["nMuon"]->Fill(nMuons,Weight);

	  Bool_t max_dphiJETMETpass = true;	// max dphi Jet-MET < 2.7 
	  Bool_t min_dphiJETMETpass = true;	// min dphi Jet-MET > 0.5 
	  Double_t min_dphi_JetMET = 10.;
          Double_t max_dphi_JetMET = 0.;
	  // UNCORRECTED MET VERSIONS
	  Bool_t max_dphiJETMETpassUncorr = true;	// max dphi Jet-MET < 2.7 
	  Bool_t min_dphiJETMETpassUncorr = true;	// min dphi Jet-MET > 0.5 
	  Double_t min_dphi_JetMETUncorr = 10.;
          Double_t max_dphi_JetMETUncorr = 0.;

	  if ( nJets50 > 0 ){
	    Double_t dphiJet1METmin = 10;
	    Double_t dphiJet2METmin = 10;
	    Double_t dphiJet3METmin = 10;
	    Double_t dphiJet4METmin = 10;
	    Double_t dphiJet1METmax = 0;
	    Double_t dphiJet2METmax = 0;
	    Double_t dphiJet3METmax = 0;
	    Double_t dphiJet4METmax = 0;
	    Double_t dphiJet1METminUncorr = 10;
	    Double_t dphiJet2METminUncorr = 10;
	    Double_t dphiJet3METminUncorr = 10;
	    Double_t dphiJet4METminUncorr = 10;
	    Double_t dphiJet1METmaxUncorr = 0;
	    Double_t dphiJet2METmaxUncorr = 0;
	    Double_t dphiJet3METmaxUncorr = 0;
	    Double_t dphiJet4METmaxUncorr = 0;
	    if ( ptJetLead > 50 ){
	      dphiJet1METmin = TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetPhiCorr));
	      dphiJet1METmax = TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetPhiCorr));
	      dphiJet1METminUncorr = TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetPhi));
	      dphiJet1METmaxUncorr = TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetPhi));
	    }
	    if ( ptJetSubLead > 50 ){
	      dphiJet2METmin = TMath::Abs(deltaPhi(fLorenzVecJet2.Phi(),t1pfmetPhiCorr));
	      dphiJet2METmax = TMath::Abs(deltaPhi(fLorenzVecJet2.Phi(),t1pfmetPhiCorr));
	      dphiJet2METminUncorr = TMath::Abs(deltaPhi(fLorenzVecJet2.Phi(),t1pfmetPhi));
	      dphiJet2METmaxUncorr = TMath::Abs(deltaPhi(fLorenzVecJet2.Phi(),t1pfmetPhi));
	    }
	    if ( ptJet3 > 50 ){
	      dphiJet3METmin = TMath::Abs(deltaPhi(fLorenzVecJet3.Phi(),t1pfmetPhiCorr));
	      dphiJet3METmax = TMath::Abs(deltaPhi(fLorenzVecJet3.Phi(),t1pfmetPhiCorr));
	      dphiJet3METminUncorr = TMath::Abs(deltaPhi(fLorenzVecJet3.Phi(),t1pfmetPhi));
	      dphiJet3METmaxUncorr = TMath::Abs(deltaPhi(fLorenzVecJet3.Phi(),t1pfmetPhi));
	    }
	    if ( ptJet4 > 50 ){
 	      dphiJet4METmin = TMath::Abs(deltaPhi(fLorenzVecJet4.Phi(),t1pfmetPhiCorr));
 	      dphiJet4METmax = TMath::Abs(deltaPhi(fLorenzVecJet4.Phi(),t1pfmetPhiCorr));
 	      dphiJet4METminUncorr = TMath::Abs(deltaPhi(fLorenzVecJet4.Phi(),t1pfmetPhi));
 	      dphiJet4METmaxUncorr = TMath::Abs(deltaPhi(fLorenzVecJet4.Phi(),t1pfmetPhi));
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

	    // UNCORRECTED MET VERSIONS
	    // find the min_dphi_JetMET 
	    if (dphiJet1METminUncorr < min_dphi_JetMETUncorr) min_dphi_JetMETUncorr = dphiJet1METminUncorr;	   
	    if (dphiJet2METminUncorr < min_dphi_JetMETUncorr) min_dphi_JetMETUncorr = dphiJet2METminUncorr;	   
	    if (dphiJet3METminUncorr < min_dphi_JetMETUncorr) min_dphi_JetMETUncorr = dphiJet3METminUncorr;	   
	    if (dphiJet4METminUncorr < min_dphi_JetMETUncorr) min_dphi_JetMETUncorr = dphiJet4METminUncorr;	   

	    // find the max_dphi_JetMET 
	    if (dphiJet1METmaxUncorr > max_dphi_JetMETUncorr) max_dphi_JetMETUncorr = dphiJet1METmaxUncorr;	   
	    if (dphiJet2METmaxUncorr > max_dphi_JetMETUncorr) max_dphi_JetMETUncorr = dphiJet2METmaxUncorr;	   
	    if (dphiJet3METmaxUncorr > max_dphi_JetMETUncorr) max_dphi_JetMETUncorr = dphiJet3METmaxUncorr;	   
	    if (dphiJet4METmaxUncorr > max_dphi_JetMETUncorr) max_dphi_JetMETUncorr = dphiJet4METmaxUncorr;	   
	  }// end if njets > 0

	  //if (isData) std::cout << entry << " max_dphi_JetMET = " << max_dphi_JetMET << std::endl;
	  //if (isData) std::cout << entry << " min_dphi_JetMET = " << min_dphi_JetMET << std::endl;

	  //if (max_dphi_JetMET > 2.7) max_dphiJETMETpass = false;// max dphi Jet-MET < 2.7 
	  max_dphiJETMETpass = true;// max dphi Jet-MET < 2.7 
	  if (min_dphi_JetMET < 0.5) min_dphiJETMETpass = false;// min dphi Jet-MET > 0.5 
	  // UNCORRECTED MET VERSIONS
	  //if (max_dphi_JetMETUncorr > 2.7) max_dphiJETMETpassUncorr = false;// max dphi Jet-MET < 2.7 
	  max_dphiJETMETpassUncorr = true;// max dphi Jet-MET < 2.7 
	  if (min_dphi_JetMETUncorr < 0.5) min_dphiJETMETpassUncorr = false;// min dphi Jet-MET > 0.5 

	  // DeltaPhi between Jet1 and gg 
	  Double_t dphiJet1gg = 0;
	  dphiJet1gg = TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),fLorenzVecgg.Phi()));
	  Bool_t dphiJet1ggpass = false; // dphi gg-Jet1 < 2.7
	  if ( dphiJet1gg < 2.7 ) dphiJet1ggpass = true;

	  // Max DeltaPhi between each photon and MET
	  Double_t dphig1MET = 0;
	  Double_t dphig2MET = 0;
 	  dphig1MET = TMath::Abs(deltaPhi(fLorenzVec1.Phi(),t1pfmetPhiCorr));
 	  dphig2MET = TMath::Abs(deltaPhi(fLorenzVec2.Phi(),t1pfmetPhiCorr));
	  Double_t maxdphigMET = TMath::Max(dphig1MET,dphig2MET);
          Double_t mindphigMET = TMath::Min(dphig1MET,dphig2MET);

	  // DeltaPhi between gg and MET
	  Double_t dphiggMET = TMath::Abs(deltaPhi(fLorenzVecgg.Phi(),t1pfmetPhiCorr));
	  Bool_t dphiggMETpass = false; // dphi gg-MET > 2.1
	  if ( dphiggMET >= 2.1 ) dphiggMETpass = true;
	  // UNCORRECTED MET VERSIONS
	  Double_t dphiggMETUncorr = TMath::Abs(deltaPhi(fLorenzVecgg.Phi(),t1pfmetPhi));
	  Bool_t dphiggMETpassUncorr = false; // dphi gg-MET > 2.1
	  if ( dphiggMETUncorr >= 2.1 ) dphiggMETpassUncorr = true;

	  // make a set of bools for easier comparison
	  Bool_t outsideMgg = false; // event lies outside mgg range
	  if ( mgg < 115 || mgg > 135 ) outsideMgg = true;

          if (dphiggMETpass && min_dphiJETMETpass && t1pfmetCorr >= METcut && ptgg > 90){
	    fTH1DMap["nElec_metCUT"]->Fill(nEle,Weight);
	    fTH1DMap["nMuon_metCUT"]->Fill(nMuons,Weight);
            fTH1DMap["nJets_metCUT"]->Fill(nJets30,Weight);
          }
          
	  if (nEle >= 1 || nMuons >= 1) continue; //reject events with leptons
	  //numMargaretLepVeto++;
	  numMargaretLepVeto+=Weight;

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
          else if (r91 <= 0.94 || r92 <= 0.94) loR9 = true;

          //-------------------------------------------------
          // plots for studies: n-1 
          //-------------------------------------------------
          if ( ptgg > 90 ){
            //if (Weight > 10) std::cout << "Weight = " << Weight << " weight = " << weight << std::endl; 
            fTH1DMap["newstudies_mgg_addptgg"]->Fill(mgg,Weight);
            if ( nJets30 <= 2 ){
              fTH1DMap["newstudies_mgg_addnjet"]->Fill(mgg,Weight);
              if ( min_dphiJETMETpass ){
                fTH1DMap["newstudies_mgg_adddphijMET"]->Fill(mgg,Weight);
                if ( dphiggMETpass ){
                  fTH1DMap["newstudies_mgg_adddphihMET"]->Fill(mgg,Weight);
                }
              }
            }
          }
          if ( dphiggMETpass && min_dphiJETMETpass && ptgg > 90 && nJets30 <= 2 ){
            if ( EB1 && EB2 ) fTH1DMap["newstudies_mgg_barrel"]->Fill(mgg,Weight);
            else              fTH1DMap["newstudies_mgg_else"]->Fill(mgg,Weight);
            if ( outsideMgg ) fTH1DMap["newstudies_MET_outmgg"]->Fill(t1pfmetCorr,Weight);
            else              fTH1DMap["newstudies_MET_selmgg"]->Fill(t1pfmetCorr,Weight);
            if ( mgg < 115 )  fTH1DMap["newstudies_MET_lowmgg"]->Fill(t1pfmetCorr,Weight);
            if ( mgg > 135 )  fTH1DMap["newstudies_MET_highmgg"]->Fill(t1pfmetCorr,Weight);
          } 
          if ( dphiggMETpass && min_dphiJETMETpass && nJets30 <= 2 ){
            fTH1DMap["newstudies_mgg_ptgg"]->Fill(mgg,Weight);
            if ( t1pfmetCorr < 130) fTH1DMap["newstudies_ptgg_lowMET"]->Fill(ptgg,Weight);
            else                    fTH1DMap["newstudies_ptgg_highMET"]->Fill(ptgg,Weight);
          }         
          if ( dphiggMETpass && min_dphiJETMETpass && ptgg > 90 ){
            fTH1DMap["newstudies_mgg_njet"]->Fill(mgg,Weight);
            if ( t1pfmetCorr < 130) fTH1DMap["newstudies_njet_lowMET"]->Fill(nJets30,Weight);
            else                    fTH1DMap["newstudies_njet_highMET"]->Fill(nJets30,Weight);
          }
          if ( dphiggMETpass && ptgg > 90 && nJets30 <= 2 ){
            fTH1DMap["newstudies_mgg_dphijMET"]->Fill(mgg,Weight);
            if ( t1pfmetCorr < 130) fTH1DMap["newstudies_dphijMET_lowMET"]->Fill(min_dphi_JetMET,Weight);
            else                    fTH1DMap["newstudies_dphijMET_highMET"]->Fill(min_dphi_JetMET,Weight);
          }
          if ( min_dphiJETMETpass && ptgg > 90 && nJets30 <= 2 ){
            fTH1DMap["newstudies_mgg_dphihMET"]->Fill(mgg,Weight);
            if ( t1pfmetCorr < 130) fTH1DMap["newstudies_dphihMET_lowMET"]->Fill(dphiggMET,Weight);
            else                    fTH1DMap["newstudies_dphihMET_highMET"]->Fill(dphiggMET,Weight);
          }
          if ( ptgg > 90 && nJets30 <= 2 ){
            if ( t1pfmetCorr < 130){
              if ( outsideMgg ) fTH1DMap["newstudies_dphihMET_lowMET_outmgg"]->Fill(dphiggMET,Weight);
              else              fTH1DMap["newstudies_dphihMET_lowMET_selmgg"]->Fill(dphiggMET,Weight);
              if ( outsideMgg ) fTH1DMap["newstudies_dphijMET_lowMET_outmgg"]->Fill(min_dphi_JetMET,Weight);
              else              fTH1DMap["newstudies_dphijMET_lowMET_selmgg"]->Fill(min_dphi_JetMET,Weight);
            }
            else{
              if ( outsideMgg ) fTH1DMap["newstudies_dphihMET_highMET_outmgg"]->Fill(dphiggMET,Weight);
              else              fTH1DMap["newstudies_dphihMET_highMET_selmgg"]->Fill(dphiggMET,Weight);
              if ( outsideMgg ) fTH1DMap["newstudies_dphijMET_highMET_outmgg"]->Fill(min_dphi_JetMET,Weight);
              else              fTH1DMap["newstudies_dphijMET_highMET_selmgg"]->Fill(min_dphi_JetMET,Weight);
            }
          }
   
          //-------------------------------------------------

          if (nJets30 > 2) continue; // jet veto

          if (ptgg <= 90) continue;


          //if (passEV1 && passEV2){
	  //  if (inEB && hiR9){  
	  //    if (isData && doBlind){
	  //      if (mgg<115 || mgg>135) fTH1DMap["EBHighR9_mgg"]->Fill(mgg,Weight);
	  //      fTH1DMap["EBHighR9_ptgg"]->Fill(ptgg,Weight);
	  //      if (t1pfmet<100) fTH1DMap["EBHighR9_t1pfmet"]->Fill(t1pfmet,Weight);
	  //    }
	  //    else{
	  //      fTH1DMap["EBHighR9_mgg"]->Fill(mgg,Weight);
	  //      fTH1DMap["EBHighR9_ptgg"]->Fill(ptgg,Weight);
	  //      fTH1DMap["EBHighR9_t1pfmet"]->Fill(t1pfmet,Weight);
	  //    }
          //  }
          //  if (inEB && loR9){
	  //    if (isData && doBlind){
	  //      if (mgg<115 || mgg>135) fTH1DMap["EBLowR9_mgg"]->Fill(mgg,Weight);
	  //      fTH1DMap["EBLowR9_ptgg"]->Fill(ptgg,Weight);
	  //      if (t1pfmet<100) fTH1DMap["EBLowR9_t1pfmet"]->Fill(t1pfmet,Weight);
	  //    }
	  //    else{
	  //      fTH1DMap["EBLowR9_mgg"]->Fill(mgg,Weight);
	  //      fTH1DMap["EBLowR9_ptgg"]->Fill(ptgg,Weight);
	  //      fTH1DMap["EBLowR9_t1pfmet"]->Fill(t1pfmet,Weight);
	  //    }
          //  }
          //  if (inEE && hiR9){
	  //    if (isData && doBlind){
	  //      if (mgg<115 || mgg>135) fTH1DMap["EEHighR9_mgg"]->Fill(mgg,Weight);
	  //      fTH1DMap["EEHighR9_ptgg"]->Fill(ptgg,Weight);
	  //      if (t1pfmet<100) fTH1DMap["EEHighR9_t1pfmet"]->Fill(t1pfmet,Weight);
	  //    }
	  //    else{
	  //      fTH1DMap["EEHighR9_mgg"]->Fill(mgg,Weight);
	  //      fTH1DMap["EEHighR9_ptgg"]->Fill(ptgg,Weight);
	  //      fTH1DMap["EEHighR9_t1pfmet"]->Fill(t1pfmet,Weight);
	  //    }
          //  }
          //  if (inEE && loR9){
	  //    if (isData && doBlind){
	  //      if (mgg<115 || mgg>135) fTH1DMap["EELowR9_mgg"]->Fill(mgg,Weight);
	  //      fTH1DMap["EELowR9_ptgg"]->Fill(ptgg,Weight);
	  //      if (t1pfmet<100) fTH1DMap["EELowR9_t1pfmet"]->Fill(t1pfmet,Weight);
	  //    }
	  //    else{
	  //      fTH1DMap["EELowR9_mgg"]->Fill(mgg,Weight);
	  //      fTH1DMap["EELowR9_ptgg"]->Fill(ptgg,Weight);
	  //      fTH1DMap["EELowR9_t1pfmet"]->Fill(t1pfmet,Weight);
	  //    }
          //  }
          //}

	  if ( ptJetSubLead > ptJetLead ) std::cout << "Jets are NOT ordered ! " << std::endl;

          fTH1DMap["eff_sel"]->Fill(2.5,Weight);
          //Fill histograms
          if (doBlind){
           if (mgg < 115 || mgg > 135){
             fTH1DMap["t1pfmet_partblind"]->Fill(t1pfmet,Weight);
             fTH1DMap["t1pfmetCorr_partblind"]->Fill(t1pfmetCorr,Weight);
           }
          }
          else{
            fTH1DMap["t1pfmet_partblind"]->Fill(t1pfmet,Weight);
            fTH1DMap["t1pfmetCorr_partblind"]->Fill(t1pfmetCorr,Weight);
          }

          if (isData && doBlind){ // BLIND THE DATA mgg and met distributions
            if (mgg < 115 || mgg > 135){
              fTH1DMap["t1pfmet_scaledipho"]->Fill(t1pfmet,Weight);

              fTH1DMap["mgg"]->Fill(mgg,Weight);
              fTH2DMap["mgg_PU"]->Fill(nvtx,mgg,Weight);
              fTH2DMap["mgg_ptgg"]->Fill(ptgg,mgg,Weight);
              fTH2DMap["t1pfmet_dphi"]->Fill(deltaPhi(fLorenzVecgg.Phi(),t1pfmetPhi),t1pfmet,Weight);
            }
            //fTH1DMap["t1pfmet"]->Fill(t1pfmet,Weight);
            //fTH1DMap["t1pfmet_zoom"]->Fill(t1pfmet,Weight);
            if (t1pfmet < 100){
              fTH1DMap["t1pfmet"]->Fill(t1pfmet,Weight);
              fTH1DMap["t1pfmet_zoom"]->Fill(t1pfmet,Weight);
              fTH1DMap["t1pfmetCorr_zoom"]->Fill(t1pfmetCorr,Weight);
              fTH2DMap["t1pfmet_PU"]->Fill(nvtx,t1pfmet,Weight);
              fTH1DMap["t1pfmetCorr"]->Fill(t1pfmetCorr,Weight);
            }

            if (pfmet < 100) fTH1DMap["pfmet"]->Fill(pfmet,Weight);
            if (calomet < 100) fTH1DMap["calomet"]->Fill(calomet,Weight);
            /*if (ptgg<0) */ fTH1DMap["ptgg"]->Fill(ptgg,Weight);
          }
          else{// not blinded data

            fTH1DMap["mgg"]->Fill(mgg,Weight);
            fTH1DMap["ptgg"]->Fill(ptgg,Weight);
            fTH1DMap["t1pfmet"]->Fill(t1pfmet,Weight);
            fTH1DMap["t1pfmet_scaledipho"]->Fill(t1pfmet,Weight*1.5);
            fTH1DMap["t1pfmetCorr"]->Fill(t1pfmetCorr,Weight);
            fTH1DMap["t1pfmetCorr_zoom"]->Fill(t1pfmetCorr,Weight);
            fTH1DMap["t1pfmetJetEnUp"]->Fill(t1pfmetJetEnUp,Weight);
            fTH1DMap["t1pfmetJetEnDown"]->Fill(t1pfmetJetEnDown,Weight);
  	    fTH1DMap["t1pfmetJetResUp"]->Fill(t1pfmetJetResUp,Weight);
  	    fTH1DMap["t1pfmetJetResDown"]->Fill(t1pfmetJetResDown,Weight);
  	    fTH1DMap["t1pfmetMuonEnUp"]->Fill(t1pfmetMuonEnUp,Weight);
  	    fTH1DMap["t1pfmetMuonEnDown"]->Fill(t1pfmetMuonEnDown,Weight);
  	    fTH1DMap["t1pfmetEleEnUp"]->Fill(t1pfmetElectronEnUp,Weight);
  	    fTH1DMap["t1pfmetEleEnDown"]->Fill(t1pfmetElectronEnDown,Weight);
  	    fTH1DMap["t1pfmetTauEnUp"]->Fill(t1pfmetTauEnUp,Weight);
  	    fTH1DMap["t1pfmetTauEnDown"]->Fill(t1pfmetTauEnDown,Weight);
  	    fTH1DMap["t1pfmetPhoEnUp"]->Fill(t1pfmetPhotonEnUp,Weight);
  	    fTH1DMap["t1pfmetPhoEnDown"]->Fill(t1pfmetPhotonEnDown,Weight);
  	    fTH1DMap["t1pfmetUnclEnUp"]->Fill(t1pfmetUnclusteredEnUp,Weight);
  	    fTH1DMap["t1pfmetUnclEnDown"]->Fill(t1pfmetUnclusteredEnDown,Weight);
            //if (mgg >= 110 && mgg <= 130) fTH1DMap["t1pfmet_selmgg"]->Fill(t1pfmet,Weight); 
            if (ptgg > 70) fTH1DMap["t1pfmet_selptgg"]->Fill(t1pfmet,Weight);
            if (t1pfmet >= METcut) fTH1DMap["ptgg_selt1pfmet"]->Fill(ptgg,Weight);
            fTH1DMap["pfmet"]->Fill(pfmet,Weight);
            fTH1DMap["calomet"]->Fill(calomet,Weight);
            fTH1DMap["t1pfmet_zoom"]->Fill(t1pfmet,Weight);
            fTH2DMap["mgg_PU"]->Fill(nvtx,mgg,Weight);
            fTH2DMap["mgg_ptgg"]->Fill(ptgg,mgg,Weight);
            fTH2DMap["t1pfmet_PU"]->Fill(nvtx,t1pfmet,Weight);

          }

	  double delta_eta = eta1-eta2;
	  double delta_phi = deltaPhi(phi1,phi2);
          double deltaRtest = TMath::Sqrt(delta_eta*delta_eta + delta_phi*delta_phi);
	  fTH1DMap["deltaRphotons"]->Fill(deltaRtest,Weight);

          fTH1DMap["nvtx"]->Fill(nvtx,Weight);
          fTH1DMap["pt1"]->Fill(pt1,Weight);
          fTH1DMap["pt2"]->Fill(pt2,Weight);
	  fTH1DMap["t1pfmetSumEt"]->Fill(t1pfmetSumEt,Weight);
	  fTH1DMap["t1pfmetSumEtCorr"]->Fill(t1pfmetSumEtCorr,Weight);
          fTH1DMap["t1pfmetphi"]->Fill(t1pfmetPhi,Weight);
          fTH1DMap["t1pfmetphiCorr"]->Fill(t1pfmetPhiCorr,Weight);
          fTH1DMap["pfmetphi"]->Fill(pfmetphi,Weight);
          fTH1DMap["calometphi"]->Fill(calometphi,Weight);
          fTH1DMap["phi1"]->Fill(phi1,Weight);
          fTH1DMap["phi2"]->Fill(phi2,Weight);
          fTH1DMap["eta1"]->Fill(eta1,Weight);
          fTH1DMap["eta2"]->Fill(eta2,Weight);
          fTH1DMap["chiso1"]->Fill(chiso1,Weight);
          fTH1DMap["chiso2"]->Fill(chiso2,Weight);
          fTH1DMap["neuiso1"]->Fill(neuiso1,Weight);
          fTH1DMap["neuiso2"]->Fill(neuiso2,Weight);
          fTH1DMap["phoiso1"]->Fill(phoiso1,Weight);
          fTH1DMap["phoiso2"]->Fill(phoiso2,Weight);
          fTH1DMap["sieie1"]->Fill(sieie1,Weight);
          fTH1DMap["sieie2"]->Fill(sieie2,Weight);
          if (inEB) fTH1DMap["sieie1EB"]->Fill(sieie1,Weight);
          if (inEB) fTH1DMap["sieie2EB"]->Fill(sieie2,Weight);
          if (inEE) fTH1DMap["sieie1EE"]->Fill(sieie1,Weight);
          if (inEE) fTH1DMap["sieie2EE"]->Fill(sieie2,Weight);
          fTH1DMap["hoe1"]->Fill(hoe1,Weight);
          fTH1DMap["hoe2"]->Fill(hoe2,Weight);
          fTH1DMap["r91"]->Fill(r91,Weight);
          fTH1DMap["r92"]->Fill(r92,Weight);
          fTH1DMap["eleveto1"]->Fill(eleveto1,Weight);
          fTH1DMap["eleveto2"]->Fill(eleveto2,Weight);
	  fTH1DMap["ptJet1"]->Fill(ptJetLead,Weight);  
	  fTH1DMap["ptJet2"]->Fill(ptJetSubLead,Weight);   
	  fTH1DMap["phiJet1"]->Fill(phiJetLead,Weight);  
	  fTH1DMap["phiJet2"]->Fill(phiJetSubLead,Weight);  
	  fTH1DMap["etaJet1"]->Fill(etaJetLead,Weight);       
	  fTH1DMap["etaJet2"]->Fill(etaJetSubLead,Weight);    
	  fTH1DMap["dphiJet1MET"]->Fill(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetPhiCorr),Weight);
	  fTH1DMap["dphiJet2MET"]->Fill(deltaPhi(fLorenzVecJet2.Phi(),t1pfmetPhiCorr),Weight);
	  fTH1DMap["absdphiJet1MET"]->Fill(TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetPhiCorr)),Weight);
	  fTH1DMap["absdphiJet2MET"]->Fill(TMath::Abs(deltaPhi(fLorenzVecJet2.Phi(),t1pfmetPhiCorr)),Weight);
	  fTH1DMap["absdphi_ggJet1"]->Fill(TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),fLorenzVecgg.Phi())),Weight);
	  fTH1DMap["absdphi_g1MET"]->Fill(TMath::Abs(deltaPhi(fLorenzVec1.Phi(),t1pfmetPhiCorr)),Weight);
	  if (t1pfmetCorr > METcut) fTH1DMap["absdphi_g1MET_metCUT"]->Fill(TMath::Abs(deltaPhi(fLorenzVec1.Phi(),t1pfmetPhiCorr)),Weight);

          
	  // Study the effect of Jets on the MET distribution
	  // DeltaPhi between each Jet and the MET

	  // set these values to true for events w/o jets
	  //Bool_t dphiJet1METpass = true;	// dphi Jet1-MET < 2.7
	  //Bool_t dphiJet2METpass = true;	// dphi Jet2-MET < 2.7

	  //if(run==260538&& lumi==245 && event==413101759)	std::cout <<lumi<<" *** Event njets = "<< nJets <<std::endl;
	  //if(run==260426&& lumi==49  && event==81336845) 	std::cout <<lumi<<" *** Event njets = "<< nJets <<std::endl;
	  //if(run==260424&& lumi==593 && event==1100585340)	std::cout <<lumi<<" *** Event njets = "<< nJets <<std::endl;
	  //if(run==260424&& lumi==166 && event==308985087)	std::cout <<lumi<<" *** Event njets = "<< nJets <<std::endl;
	  //if(run==260627&& lumi==756 && event==1386828994)	std::cout <<lumi<<" *** Event njets = "<< nJets <<std::endl;
	  //if(run==259685&& lumi==160 && event==289342590)	std::cout <<lumi<<" *** Event njets = "<< nJets <<std::endl;
	  //if(run==256843&& lumi==1033&& event==1427294382)	std::cout <<lumi<<" *** Event njets = "<< nJets <<std::endl;
	  //if(run==258706&& lumi==18  && event==32214611)	std::cout <<lumi<<" *** Event njets = "<< nJets <<std::endl;
	  //if(run==258703&& lumi==377 && event==653522584)	std::cout <<lumi<<" *** Event njets = "<< nJets <<std::endl;
	  //if(run==258703&& lumi==302 && event==512333471)	std::cout <<lumi<<" *** Event njets = "<< nJets <<std::endl;
	  //if(run==258656&& lumi==75  && event==125514897)	std::cout <<lumi<<" *** Event njets = "<< nJets <<std::endl;
	  //if(run==256843&& lumi==38  && event==55470023)	std::cout <<lumi<<" *** Event njets = "<< nJets <<std::endl;

	  // look at dphi of jet and MET of events w/ jets
	  // if only one high pt jet, min & max dphi are dphi of single jet
	  //if ( nJets == 1 ){
	  //  if ( ptJetLead > 50 ){
	  //    Double_t dphi_JetMET = TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetPhi));
	  //    min_dphi_JetMET = dphi_JetMET;
	  //    max_dphi_JetMET = dphi_JetMET;
	  //  }
	  //}
	  // if more than one high pt jet, find min & max dphi of the top 4 jets


	  if (nJets50 > 0){
	    fTH1DMap["absdphi_maxJetMET"]->Fill(TMath::Abs(max_dphi_JetMET),Weight);
	    fTH1DMap["absdphi_minJetMET"]->Fill(TMath::Abs(min_dphi_JetMET),Weight);
	    fTH1DMap["absdphiUncorr_maxJetMET"]->Fill(TMath::Abs(max_dphi_JetMETUncorr),Weight);
	    fTH1DMap["absdphiUncorr_minJetMET"]->Fill(TMath::Abs(min_dphi_JetMETUncorr),Weight);
	    if (t1pfmetCorr > METcut){
	      fTH1DMap["absdphi_maxJetMET_metCUT"]->Fill(TMath::Abs(max_dphi_JetMET),Weight);
	      fTH1DMap["absdphi_minJetMET_metCUT"]->Fill(TMath::Abs(min_dphi_JetMET),Weight);
	    }
	    fTH1DMap["jetInfo_NEMfrac1"]->Fill(NEMfracJet1,Weight);
	    fTH1DMap["jetInfo_CEMfrac1"]->Fill(CEMfracJet1,Weight);
	    fTH1DMap["jetInfo_CHfrac1"]->Fill(CHfracJet1,Weight);
	    fTH1DMap["jetInfo_NHfrac1"]->Fill(NHfracJet1,Weight);
	    fTH1DMap["jetInfo_PHfrac1"]->Fill(PHfracJet1,Weight);
	    fTH1DMap["jetInfo_ELfrac1"]->Fill(ELfracJet1,Weight);
	    fTH1DMap["jetInfo_MUfrac1"]->Fill(MUfracJet1,Weight);
	    fTH1DMap["jetInfo_CHmult1"]->Fill(CHmultJet1,Weight);
	    fTH1DMap["jetInfo_NEmult1"]->Fill(NEmultJet1,Weight);
	    fTH1DMap["jetInfo_pt1"]->Fill(ptJetLead,Weight);
	    fTH1DMap["jetInfo_eta1"]->Fill(etaJetLead,Weight);
	    fTH1DMap["jetInfo_phi1"]->Fill(phiJetLead,Weight);
	    fTH1DMap["jetInfo_mass1"]->Fill(massJetLead,Weight);
	  }


          if (min_dphiJETMETpass && dphiggMETpass && t1pfmetCorr <= METcut){
            if (!isData || (isData && outsideMgg)) fTH1DMap["test_mgg_lowmet"]->Fill(mgg,Weight);
          }
 
          if (outsideMgg && t1pfmetCorr > METcut){
            fTH1DMap["clean_dphi_min_jMET_metCUT"]->Fill(min_dphi_JetMET,Weight);
            fTH1DMap["clean_dphi_hMET_metCUT"]->Fill(dphiggMET,Weight);
          } 
          if (outsideMgg && min_dphiJETMETpass && dphiggMETpass){
            fTH1DMap["clean_dphi_min_gMET"]->Fill(mindphigMET,Weight); 
            if (t1pfmetCorr > METcut) fTH1DMap["clean_dphi_min_gMET_metCUT"]->Fill(mindphigMET,Weight); 
          }
          if (min_dphiJETMETpass && dphiggMETpass && t1pfmetCorr > METcut && outsideMgg){
	    fTH1DMap["metphi_metCUT"]->Fill(t1pfmetPhiCorr,Weight);
            fTH1DMap["ggphi_metCUT"]->Fill(fLorenzVecgg.Phi(),Weight);
	    fTH1DMap["ptgg_afterMETcut"]->Fill(ptgg,Weight);
            fTH1DMap["pt1_afterMETcut"]->Fill(pt1,Weight);
            fTH1DMap["pt2_afterMETcut"]->Fill(pt2,Weight);
          }
          if (outsideMgg){
            fTH1DMap["test_mgg_nocuts"]->Fill(mgg,Weight);
	    if (min_dphiJETMETpass) fTH1DMap["test_mgg_dphiJmet"]->Fill(mgg,Weight);
            if (dphiggMETpass)      fTH1DMap["test_mgg_dphiGGmet"]->Fill(mgg,Weight);
          }
 
          if (outsideMgg && t1pfmetCorr < METcut){
            fTH1DMap["studies_dphi_minjMET_lowMET"]->Fill(min_dphi_JetMET,Weight);
            fTH1DMap["studies_dphi_hMET_lowMET"]->Fill(dphiggMET,Weight);
            if (t1pfmetCorr < 10){
              fTH1DMap["studies_dphi_minjMET_lowMET10"]->Fill(min_dphi_JetMET,Weight);
              fTH1DMap["studies_dphi_hMET_lowMET10"]->Fill(dphiggMET,Weight);
            }
            if (t1pfmetCorr < 20){
              fTH1DMap["studies_dphi_minjMET_lowMET20"]->Fill(min_dphi_JetMET,Weight);
              fTH1DMap["studies_dphi_hMET_lowMET20"]->Fill(dphiggMET,Weight);
            }
            if (t1pfmetCorr < 30){
              fTH1DMap["studies_dphi_minjMET_lowMET30"]->Fill(min_dphi_JetMET,Weight);
              fTH1DMap["studies_dphi_hMET_lowMET30"]->Fill(dphiggMET,Weight);
            }
            if (t1pfmetCorr < 40){
              fTH1DMap["studies_dphi_minjMET_lowMET40"]->Fill(min_dphi_JetMET,Weight);
              fTH1DMap["studies_dphi_hMET_lowMET40"]->Fill(dphiggMET,Weight);
            }
          }
 

	  Bool_t dphigMETpass = false; // dphi g1-JET && g2-JET < 2.7

	  if ( dphig1MET < 2.7) dphigMETpass = true;
	  fTH1DMap["absdphi_maxgMET"]->Fill(TMath::Abs(deltaPhi(maxdphigMET,t1pfmetPhiCorr)),Weight);
	  if (t1pfmetCorr > METcut) fTH1DMap["absdphi_maxgMET_metCUT"]->Fill(TMath::Abs(deltaPhi(maxdphigMET,t1pfmetPhiCorr)),Weight);



	  // plots
	  if ( !isData && dphigMETpass ) fTH1DMap["metCor_Sig"]->Fill(t1pfmetCorr,Weight);

	  if ( !isData ){
	    if ( dphiggMETpass /*&& max_dphiJETMETpass*/ && min_dphiJETMETpass ){
	      //numMargaret_METCorr++;
	      numMargaret_METCorr+=Weight;
	      //if ( t1pfmetCorr > METcut ) numMargaret_METCorrMETcut++;
	      if ( t1pfmetCorr > METcut ) numMargaret_METCorrMETcut+=Weight;
	    }
	    if ( dphiggMETpassUncorr && max_dphiJETMETpassUncorr && min_dphiJETMETpassUncorr ){ 
	      //numMargaret_METUncorr++;
	      numMargaret_METUncorr+=Weight;
	      //if ( t1pfmet > METcut ) numMargaret_METUncorrMETcut++;
	      if ( t1pfmet > METcut ) numMargaret_METUncorrMETcut+=Weight;
	    }
          }

	  if ( (doBlind && outsideMgg) || !doBlind){
	    if ( t1pfmetCorr > METcut ) nDataOrig++;
	    if ( dphigMETpass ){
	      fTH1DMap["met_aftergMETCut"]->Fill(t1pfmet,Weight);
	      fTH1DMap["metCor_aftergMETCut"]->Fill(t1pfmetCorr,Weight);
	    }
	    if ( dphiJet1ggpass ){ 
		    fTH1DMap["met_afterJetCut"]->Fill(t1pfmet,Weight); 
		    fTH1DMap["metCor_afterJetCut"]->Fill(t1pfmetCorr,Weight); 
	    }
	    if ( t1pfmetCorr > METcut ){
	      if ( max_dphiJETMETpass ){
	        nDatadphi1++;
	        if ( min_dphiJETMETpass ){
	          nDatadphi2++;
	          if ( dphiggMETpass ) nDatadphi3++;
	        }
	      }
	    }
	    if ( dphiggMETpass){
	      fTH1DMap["met_afterggMETCut"]->Fill(t1pfmet,Weight); 
	      fTH1DMap["metCor_afterggMETCut"]->Fill(t1pfmetCorr,Weight); 
	      fTH1DMap["nvtx_afterggMETCut"]->Fill(nvtx,Weight);
	      fTH1DMap["ptgg_afterggMETCut"]->Fill(ptgg,Weight);
	      fTH1DMap["pt1_afterggMETCut"]->Fill(pt1,Weight);
	      fTH1DMap["pt2_afterggMETCut"]->Fill(pt2,Weight);
	      if ( dphiJet1ggpass ){ 
	        fTH1DMap["met_Isolategg"]->Fill(t1pfmet,Weight); 
	        fTH1DMap["metCor_Isolategg"]->Fill(t1pfmetCorr,Weight); 
	      } 
	    } 
	    fTH1DMap["met_womaxJetMET"]->Fill(t1pfmet,Weight); 
	    fTH1DMap["metCor_womaxJetMET"]->Fill(t1pfmetCorr,Weight); 
	    if ( max_dphiJETMETpass ){
	      fTH1DMap["met_maxJetMET"]->Fill(t1pfmet,Weight); 
	      fTH1DMap["metCor_maxJetMET"]->Fill(t1pfmetCorr,Weight); 
	    } 
	    if ( min_dphiJETMETpass ){
	      fTH1DMap["met_minJetMET"]->Fill(t1pfmet,Weight); 
	      fTH1DMap["metCor_minJetMET"]->Fill(t1pfmetCorr,Weight); 
	    }
	    if ( /*max_dphiJETMETpass &&*/ min_dphiJETMETpass ){ 
	      fTH1DMap["met_afterJetMETPhiCut"]->Fill(t1pfmet,Weight); 
	      fTH1DMap["metCor_afterJetMETPhiCut"]->Fill(t1pfmetCorr,Weight); 
	      if ( dphiggMETpass /*&& dphigMETpass*/ ){
	        //fTH1DMap["met_IsolateALL"]->Fill(t1pfmet,Weight); 
	        fTH1DMap["metCor_IsolateALL_Varbin"]->Fill(t1pfmetCorr,Weight);
	        fTH1DMap["metCor_IsolateALL"]->Fill(t1pfmetCorr,Weight);
                if (mgg >= 105 && mgg <= 180) fTH1DMap["metCor_IsolateALL_selmgg"]->Fill(t1pfmetCorr,Weight);
                if (mgg >= 120 && mgg <= 130) fTH1DMap["t1pfmetCorr_selmgg_Varbin"]->Fill(t1pfmetCorr,Weight); 
                if (mgg >= 120 && mgg <= 130) fTH1DMap["t1pfmetCorr_selmgg"]->Fill(t1pfmetCorr,Weight); 
	        if (t1pfmet >= METcut) fTH1DMap["mgg_selt1pfmet"]->Fill(mgg,Weight);  
	        if (t1pfmet < METcut) fTH1DMap["mgg_inverseselt1pfmet"]->Fill(mgg,Weight);  
		//if ( isData && t1pfmetCorr > 80 ) std::cout << run << ":" << lumi << ":" << event << std::endl;
		//if ( isData && t1pfmetCorr > METcut ) std::cout << "MET Tail: mgg = " << mgg << " MET = " << t1pfmetCorr << " ptgg = " << ptgg << " pt1/mgg = " << pt1/mgg << " pt2/mgg = " << pt2/mgg  << " Run = " << run << " Lumi = " << lumi << " Event " << event << std::endl;
	      } 
	    }
	    if ( dphiggMETpassUncorr /*&& max_dphiJETMETpassUncorr*/ && min_dphiJETMETpassUncorr ){ 
	      fTH1DMap["met_IsolateALL_Varbin"]->Fill(t1pfmet,Weight);
	      fTH1DMap["met_IsolateALL"]->Fill(t1pfmet,Weight);
              if (mgg >= 120 && mgg <= 130) fTH1DMap["t1pfmet_selmgg"]->Fill(t1pfmet,Weight); 
	    }
	  }

	  if ( dphiggMETpassUncorr ){
            //numMargaretmaxggMET++;
            numMargaretmaxggMET+=Weight;
	    if ( max_dphiJETMETpassUncorr ){
	      //numMargaretmaxjMET++;
	      numMargaretmaxjMET+=Weight;
              if ( min_dphiJETMETpassUncorr ){
                //numMargaretminjMET++;
                numMargaretminjMET+=Weight;
	        if ( !isData && mgg > 120 && mgg < 130 ){
	          //numMargaretmgg++;
	          numMargaretmgg+=Weight;
		  if ( t1pfmet > METcut ){
		    //numMargaretMETcut++;
		    numMargaretMETcut+=Weight;
		  }
		}
              }
            }
          }



	  if ( !isData && dphiggMETpass /*&& max_dphiJETMETpass*/ && min_dphiJETMETpass  ){
	    //-------------- 
	    // ABCD plots
	    //-------------- 
	    fTH2DMap["t1pfmet_mgg"]->Fill(mgg,t1pfmetCorr,Weight);
	    fTH2DMap["t1pfmet_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);
	    //-------------- for OptSel1 and OptSel2 ABCD 
	    if (fWhichSel==0){ 
	      if (pt1/mgg > 0.50 && pt2/mgg > 0.25 && ptgg > 90)  fTH2DMap["Sel1_M1_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M600
	      if (pt1/mgg > 0.50 && pt2/mgg > 0.25 && ptgg > 90)  fTH2DMap["Sel1_M1_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M600
	      if (pt1/mgg > 0.80 && pt2/mgg > 0.25 && ptgg > 135) fTH2DMap["Sel1_M2_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M800
	      if (pt1/mgg > 0.80 && pt2/mgg > 0.25 && ptgg > 135) fTH2DMap["Sel1_M2_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M800
	      if (pt1/mgg > 0.85 && pt2/mgg > 0.25 && ptgg > 170) fTH2DMap["Sel1_M3_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1000
	      if (pt1/mgg > 0.85 && pt2/mgg > 0.25 && ptgg > 170) fTH2DMap["Sel1_M3_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1000
	      if (pt1/mgg > 1.20 && pt2/mgg > 0.25 && ptgg > 250) fTH2DMap["Sel1_M4_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1200
	      if (pt1/mgg > 1.20 && pt2/mgg > 0.25 && ptgg > 250) fTH2DMap["Sel1_M4_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1200
	      if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg > 260) fTH2DMap["Sel1_M5_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1400
	      if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg > 260) fTH2DMap["Sel1_M5_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1400
	      if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg > 315) fTH2DMap["Sel1_M6_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1700
	      if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg > 315) fTH2DMap["Sel1_M6_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1700
	      if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg > 315) fTH2DMap["Sel1_M7_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M2500
	      if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg > 315) fTH2DMap["Sel1_M7_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M2500

	      if (pt1/mgg > 0.55 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.5) fTH2DMap["Sel2_M1_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M600
	      if (pt1/mgg > 0.55 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.5) fTH2DMap["Sel2_M1_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M600
	      if (pt1/mgg > 0.80 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.2) fTH2DMap["Sel2_M2_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M800
	      if (pt1/mgg > 0.80 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.2) fTH2DMap["Sel2_M2_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M800
	      if (pt1/mgg > 0.80 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.2) fTH2DMap["Sel2_M3_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1000
	      if (pt1/mgg > 0.80 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.2) fTH2DMap["Sel2_M3_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1000
	      if (pt1/mgg > 1.30 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.2) fTH2DMap["Sel2_M4_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1200
	      if (pt1/mgg > 1.30 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.2) fTH2DMap["Sel2_M4_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1200
	      if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.3) fTH2DMap["Sel2_M5_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1400
	      if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.3) fTH2DMap["Sel2_M5_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1400
	      if (pt1/mgg > 1.55 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.3) fTH2DMap["Sel2_M6_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1700
	      if (pt1/mgg > 1.55 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.3) fTH2DMap["Sel2_M6_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1700
	      if (pt1/mgg > 1.50 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.3) fTH2DMap["Sel2_M7_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M2500
	      if (pt1/mgg > 1.50 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.3) fTH2DMap["Sel2_M7_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M2500
            }
	    //-------------- 

	    fTH1DMap["metCorr_forShape"]->Fill(t1pfmetCorr,Weight);
	    fTH1DMap["mgg_forShape"]->Fill(mgg,Weight);
	    fTH1DMap["mgg_IsolateALL"]->Fill(mgg,Weight);
            if (pt1 > 0.50*mgg) fTH1DMap["mgg_IsolateALL_pt1"]->Fill(mgg,Weight);
            if (pt2 > 0.25*mgg) fTH1DMap["mgg_IsolateALL_pt2"]->Fill(mgg,Weight);
            if (ptgg > 90) fTH1DMap["mgg_IsolateALL_ptgg"]->Fill(mgg,Weight);
            if (pt1 > 0.50*mgg && pt2 > 0.25*mgg) fTH1DMap["mgg_IsolateALL_woPtgg"]->Fill(mgg,Weight);
            if (pt1 > 0.50*mgg && pt2 > 0.25*mgg && ptgg > 90) fTH1DMap["mgg_IsolateALL_wPtgg"]->Fill(mgg,Weight);
	    fTH1DMap["ptgg_IsolateALL"]->Fill(ptgg,Weight);
	    fTH1DMap["nvtx_IsolateALL"]->Fill(nvtx,Weight);
	    fTH1DMap["BDTindex"]->Fill(BDTindex,Weight);
	    if (t1pfmetCorr > METcut){
	        fTH1DMap["ptgg_IsolateALLmetCUT"]->Fill(ptgg,Weight);
	        fTH1DMap["nvtx_IsolateALLmetCUT"]->Fill(nvtx,Weight);
		fTH1DMap["mgg_IsolateALLmetCUT"]->Fill(mgg,Weight);
	    	fTH1DMap["mgg_metCUT_forShape"]->Fill(mgg,Weight);
	    }
	    else fTH1DMap["mgg_IsolateALLlowCUT"]->Fill(mgg,Weight);
	    if (t1pfmet > METcut) fTH1DMap["mgg_IsolateALLUncorrmetCUT"]->Fill(mgg,Weight);
	  }
          if ( isData && dphiggMETpass /*&& max_dphiJETMETpass*/ && min_dphiJETMETpass ){
	    if (doBlind){
	      //if (outsideMgg){
	        fTH2DMap["t1pfmet_mgg"]->Fill(mgg,t1pfmetCorr,Weight);
	        fTH2DMap["t1pfmet_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);
	        //-------------- for OptSel1 and OptSel2 ABCD  
		if (fWhichSel==0){
	          if (pt1/mgg > 0.50 && pt2/mgg > 0.25 && ptgg > 90)  fTH2DMap["Sel1_M1_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M600
	          if (pt1/mgg > 0.50 && pt2/mgg > 0.25 && ptgg > 90)  fTH2DMap["Sel1_M1_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M600
	          if (pt1/mgg > 0.80 && pt2/mgg > 0.25 && ptgg > 135) fTH2DMap["Sel1_M2_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M800
	          if (pt1/mgg > 0.80 && pt2/mgg > 0.25 && ptgg > 135) fTH2DMap["Sel1_M2_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M800
	          if (pt1/mgg > 0.85 && pt2/mgg > 0.25 && ptgg > 170) fTH2DMap["Sel1_M3_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1000
	          if (pt1/mgg > 0.85 && pt2/mgg > 0.25 && ptgg > 170) fTH2DMap["Sel1_M3_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1000
	          if (pt1/mgg > 1.20 && pt2/mgg > 0.25 && ptgg > 250) fTH2DMap["Sel1_M4_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1200
	          if (pt1/mgg > 1.20 && pt2/mgg > 0.25 && ptgg > 250) fTH2DMap["Sel1_M4_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1200
	          if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg > 260) fTH2DMap["Sel1_M5_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1400
	          if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg > 260) fTH2DMap["Sel1_M5_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1400
	          if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg > 315) fTH2DMap["Sel1_M6_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1700
	          if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg > 315) fTH2DMap["Sel1_M6_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1700
	          if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg > 315) fTH2DMap["Sel1_M7_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M2500
	          if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg > 315) fTH2DMap["Sel1_M7_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M2500

	          if (pt1/mgg > 0.55 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.5) fTH2DMap["Sel2_M1_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M600
	          if (pt1/mgg > 0.55 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.5) fTH2DMap["Sel2_M1_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M600
	          if (pt1/mgg > 0.80 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.2) fTH2DMap["Sel2_M2_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M800
	          if (pt1/mgg > 0.80 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.2) fTH2DMap["Sel2_M2_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M800
	          if (pt1/mgg > 0.80 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.2) fTH2DMap["Sel2_M3_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1000
	          if (pt1/mgg > 0.80 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.2) fTH2DMap["Sel2_M3_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1000
	          if (pt1/mgg > 1.30 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.2) fTH2DMap["Sel2_M4_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1200
	          if (pt1/mgg > 1.30 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.2) fTH2DMap["Sel2_M4_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1200
	          if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.3) fTH2DMap["Sel2_M5_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1400
	          if (pt1/mgg > 1.40 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.3) fTH2DMap["Sel2_M5_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1400
	          if (pt1/mgg > 1.55 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.3) fTH2DMap["Sel2_M6_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M1700
	          if (pt1/mgg > 1.55 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.3) fTH2DMap["Sel2_M6_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M1700
	          if (pt1/mgg > 1.50 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.3) fTH2DMap["Sel2_M7_met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); // M2500
	          if (pt1/mgg > 1.50 && pt2/mgg > 0.25 && ptgg/t1pfmetCorr > 0.3) fTH2DMap["Sel2_M7_met_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);  // M2500
		}
	        else{// whichSelection !=0
	          fTH2DMap["t1pfmet_mgg"]->Fill(mgg,t1pfmetCorr,Weight);
	          fTH2DMap["t1pfmet_mgg_unwgt"]->Fill(mgg,t1pfmetCorr);
		}
	        if (outsideMgg){
		  fTH1DMap["mgg_IsolateALL"]->Fill(mgg,Weight);
                  if (pt1 > 0.50*mgg) fTH1DMap["mgg_IsolateALL_pt1"]->Fill(mgg,Weight);
                  if (pt2 > 0.25*mgg) fTH1DMap["mgg_IsolateALL_pt2"]->Fill(mgg,Weight);
                  if (ptgg > 90) fTH1DMap["mgg_IsolateALL_ptgg"]->Fill(mgg,Weight);
                  if (pt1 > 0.50*mgg && pt2 > 0.25*mgg) fTH1DMap["mgg_IsolateALL_woPtgg"]->Fill(mgg,Weight);
                  if (pt1 > 0.50*mgg && pt2 > 0.25*mgg && ptgg > 90) fTH1DMap["mgg_IsolateALL_wPtgg"]->Fill(mgg,Weight);
		  if (t1pfmetCorr > METcut) fTH1DMap["mgg_IsolateALLmetCUT"]->Fill(mgg,Weight);
		  if (t1pfmetCorr > METcut && mgg >= 105 && mgg <= 180){
		    float deltaR12 = reco::deltaR(eta1,phi1,eta2,phi2);
		    std::cout << " Lumi = " << lumi << " run " << run << " event " << event << ": ---- DR: " << deltaR12 << std::endl;
		  }


		  else fTH1DMap["mgg_IsolateALLlowCUT"]->Fill(mgg,Weight);
		  if (t1pfmet > METcut) fTH1DMap["mgg_IsolateALLUncorrmetCUT"]->Fill(mgg,Weight);
		}
		//}// end outside mgg
	      }// end doBlind
              else{
		fTH2DMap["met_mgg"]->Fill(mgg,t1pfmetCorr,Weight); 
		fTH1DMap["mgg_IsolateALL"]->Fill(mgg,Weight);
                if (pt1 > 0.50*mgg) fTH1DMap["mgg_IsolateALL_pt1"]->Fill(mgg,Weight);
                if (pt2 > 0.25*mgg) fTH1DMap["mgg_IsolateALL_pt2"]->Fill(mgg,Weight);
                if (ptgg > 90) fTH1DMap["mgg_IsolateALL_ptgg"]->Fill(mgg,Weight);
                if (pt1 > 0.50*mgg && pt2 > 0.25*mgg) fTH1DMap["mgg_IsolateALL_woPtgg"]->Fill(mgg,Weight);
                if (pt1 > 0.50*mgg && pt2 > 0.25*mgg && ptgg > 90) fTH1DMap["mgg_IsolateALL_wPtgg"]->Fill(mgg,Weight);
		if (t1pfmetCorr > METcut) fTH1DMap["mgg_IsolateALLmetCUT"]->Fill(mgg,Weight);
		else fTH1DMap["mgg_IsolateALLlowCUT"]->Fill(mgg,Weight);
		if (t1pfmet > METcut) fTH1DMap["mgg_IsolateALLUncorrmetCUT"]->Fill(mgg,Weight);
	      }
	      fTH1DMap["ptgg_IsolateALL"]->Fill(ptgg,Weight);
	      fTH1DMap["nvtx_IsolateALL"]->Fill(nvtx,Weight);
	      if (t1pfmetCorr > METcut){
		fTH1DMap["ptgg_IsolateALLmetCUT"]->Fill(ptgg,Weight);
	        fTH1DMap["nvtx_IsolateALLmetCUT"]->Fill(nvtx,Weight);
	      }
	  } 


	  if ( dphiJet1ggpass ){ 
            fTH1DMap["nvtx_afterJetCut"]->Fill(nvtx,Weight); 
            fTH1DMap["ptgg_afterJetCut"]->Fill(ptgg,Weight); 
            if (isData && doBlind && outsideMgg) fTH1DMap["mgg_afterJetCut"]->Fill(mgg,Weight);  
            else if (!isData || !doBlind) fTH1DMap["mgg_afterJetCut"]->Fill(mgg,Weight); 
	  }

          fTH1DMap["phigg"]->Fill(fLorenzVecgg.Phi(),Weight); 
          fTH1DMap["dphi_ggmet"]->Fill(deltaPhi(fLorenzVecgg.Phi(),t1pfmetPhiCorr),Weight);
          fTH1DMap["absdphi_ggmet"]->Fill(TMath::Abs(deltaPhi(fLorenzVecgg.Phi(),t1pfmetPhiCorr)),Weight);
          fTH1DMap["absdphiUncorr_ggmet"]->Fill(TMath::Abs(deltaPhi(fLorenzVecgg.Phi(),t1pfmetPhi)),Weight);
          if (t1pfmetCorr > METcut ) fTH1DMap["absdphi_ggmet_metCUT"]->Fill(TMath::Abs(deltaPhi(fLorenzVecgg.Phi(),t1pfmetPhiCorr)),Weight);
          fTH1DMap["deta_gg"]->Fill((eta1-eta2),Weight);
          fTH1DMap["absdeta_gg"]->Fill(TMath::Abs(eta1-eta2),Weight);

          //std::cout << passCH1 <<" "<< passNH1 <<" "<< passPH1 <<" "<< passHE1 <<" "<< passS1 << std::endl; 
          //std::cout << passCH2 <<" "<< passNH2 <<" "<< passPH2 <<" "<< passHE2 <<" "<< passS2 << std::endl; 
          //std::cout << passAll1 <<" "<< passAll2 <<" "<< passBoth << std::endl;

          //fill n-1 plots for the photon ID selection variables
          //if (passCH1 && passNH1 && passPH1 && passS1)  fTH1DMap["hoe1_n-1"]->Fill(hoe1,Weight); 
          //if (passCH1 && passNH1 && passPH1 && passHE1) fTH1DMap["sieie1_n-1"]->Fill(sieie1,Weight);
          //if (passCH1 && passNH1 && passHE1 && passS1)  fTH1DMap["phoiso1_n-1"]->Fill(phoiso1,Weight);
          //if (passCH1 && passPH1 && passHE1 && passS1)  fTH1DMap["neuiso1_n-1"]->Fill(neuiso1,Weight);
          //if (passPH1 && passNH1 && passHE1 && passS1)  fTH1DMap["chiso1_n-1"]->Fill(chiso1,Weight);

          //if (passCH2 && passNH2 && passPH2 && passS2)  fTH1DMap["hoe2_n-1"]->Fill(hoe2,Weight); 
          //if (passCH2 && passNH2 && passPH2 && passHE2) fTH1DMap["sieie2_n-1"]->Fill(sieie2,Weight);
          //if (passCH2 && passNH2 && passHE2 && passS2)  fTH1DMap["phoiso2_n-1"]->Fill(phoiso2,Weight);
          //if (passCH2 && passPH2 && passHE2 && passS2)  fTH1DMap["neuiso2_n-1"]->Fill(neuiso2,Weight);
          //if (passPH2 && passNH2 && passHE2 && passS2)  fTH1DMap["chiso2_n-1"]->Fill(chiso2,Weight);

          //if (passAll1){// fill pho1 plots if these photons pass phoID
          //  fTH1DMap["pt1_n-1"]->Fill(pt1,Weight);
          //  fTH1DMap["r91_n-1"]->Fill(r91,Weight);
          //  fTH1DMap["phi1_n-1"]->Fill(phi1,Weight);
          //  fTH1DMap["eta1_n-1"]->Fill(eta1,Weight);
          //}
          //if (passAll2){// fill pho2 plots if these photons pass phoID
          //  fTH1DMap["pt2_n-1"]->Fill(pt2,Weight);
          //  fTH1DMap["r92_n-1"]->Fill(r92,Weight);
          //  fTH1DMap["phi2_n-1"]->Fill(phi2,Weight);
          //  fTH1DMap["eta2_n-1"]->Fill(eta2,Weight);
          //} 
          //if (passBoth){
          //  fTH1DMap["nvtx_n-1"]->Fill(nvtx,Weight);
          //  fTH1DMap["t1pfmetphi_n-1"]->Fill(t1pfmetPhi,Weight);  
          //  fTH1DMap["pfmetphi_n-1"]->Fill(pfmetphi,Weight);
          //  fTH1DMap["calometphi_n-1"]->Fill(calometphi,Weight);
          //  if (isData && doBlind){// BLIND THE DATA
          //    if (mgg < 115 || mgg > 135){
          //      fTH1DMap["mgg_n-1"]->Fill(mgg,Weight);  
          //      if (t1pfmet < 100) fTH2DMap["t1pfmet_mgg"]->Fill(mgg,t1pfmet,Weight);
          //    }
          //    if (t1pfmet < 100) fTH1DMap["t1pfmet_n-1"]->Fill(t1pfmet,Weight);  
          //    if (pfmet < 100)   fTH1DMap["pfmet_n-1"]->Fill(pfmet,Weight);
          //    if (calomet < 100) fTH1DMap["calomet_n-1"]->Fill(calomet,Weight);
          //    /*if (ptgg<0)*/ fTH1DMap["ptgg_n-1"]->Fill(ptgg,Weight);  
          //    //if (mgg >= 110 && mgg <= 130) fTH1DMap["t1pfmet_selmgg"]->Fill(t1pfmet,Weight); 
          //    if (t1pfmet >= 50 && ( mgg < 115 || mgg > 135)) fTH1DMap["mgg_selt1pfmet"]->Fill(mgg,Weight);  
          //  }
          //  else{
          //    fTH1DMap["mgg_n-1"]->Fill(mgg,Weight);  
          //    fTH2DMap["t1pfmet_mgg"]->Fill(mgg,t1pfmet,Weight);
          //    fTH1DMap["t1pfmet_n-1"]->Fill(t1pfmet,Weight);  
          //    fTH1DMap["pfmet_n-1"]->Fill(pfmet,Weight);
          //    fTH1DMap["calomet_n-1"]->Fill(calomet,Weight);
          //    fTH1DMap["ptgg_n-1"]->Fill(ptgg,Weight);  
	  
          if (passCH1 && passCH2){
            fTH1DMap["eff_sel"]->Fill(3.5,Weight);
            if (passNH1 && passNH2){
              fTH1DMap["eff_sel"]->Fill(4.5,Weight);
              if (passPH1 && passPH2){
                fTH1DMap["eff_sel"]->Fill(5.5,Weight);
                if (passS1 && passS2){ 
                  fTH1DMap["eff_sel"]->Fill(6.5,Weight);
           	if (passHE1 && passHE2){
                    fTH1DMap["eff_sel"]->Fill(7.5,Weight);
          	  if (!isData || !doBlind){// BLIND THE DATA
                      if (mgg >= 115 && mgg <= 135){
              	      fTH1DMap["eff_sel"]->Fill(8.5,Weight);
              	      if (t1pfmet >= 100){
              	        fTH1DMap["eff_sel"]->Fill(9.5,Weight);
              	      }
                      }
          	  }
                  }
                }
              }
            }
          }

	  if (dphiggMETpass && max_dphiJETMETpass && min_dphiJETMETpass ){
	    if (doBlind){
	      if (outsideMgg){
                fTH1DMap["ptggOverMET"]->Fill(ptgg/t1pfmetCorr,Weight);
                fTH2DMap["t1pfmet_ptgg"]->Fill(ptgg,t1pfmetCorr,Weight);
	      }
	    }
	    else{
	      fTH1DMap["ptggOverMET"]->Fill(ptgg/t1pfmetCorr,Weight);
              fTH2DMap["t1pfmet_ptgg"]->Fill(ptgg,t1pfmetCorr,Weight);
	    }
	  }

	  if (!isData && dphiggMETpass && max_dphiJETMETpass && min_dphiJETMETpass ){
	    nEvents_allVtx++;
	    fTH2DMap["ptzp_njets"]->Fill(nJets20,fLorenzVecZp.Pt(),Weight);
	    fTH1DMap["vtx_eff_ptzp_d"]->Fill(fLorenzVecZp.Pt());
	    fTH1DMap["vtx_eff_nvtx_d"]->Fill(nvtx);
	    fTH1DMap["vtx_eff_met_d"]->Fill(t1pfmetCorr);
	    fTH1DMap["vtx_eff_njet_d"]->Fill(nJets20);
	    Double_t vtxZdiff = TMath::Abs(genVtxZ-vtxZ); 
	    Double_t vtx0Zdiff = TMath::Abs(genVtxZ-vtx0Z); 
	    Bool_t goodVtx = true;
	    Bool_t goodVtx0 = true;
	    if ( vtxZdiff > 1.0 ) goodVtx = false;
	    if ( vtx0Zdiff > 1.0 ) goodVtx0 = false;
	    if ( goodVtx ){
	      nEvents_goodVtx++;
	      fTH1DMap["vtx_eff_ptzp_n"]->Fill(fLorenzVecZp.Pt());
	      fTH1DMap["vtx_eff_nvtx_n"]->Fill(nvtx);
	      fTH1DMap["vtx_eff_met_n"]->Fill(t1pfmetCorr);
	      fTH1DMap["vtx_eff_njet_n"]->Fill(nJets20);
	    }
	    if ( goodVtx0 ) nEvents_goodVtx0++;
	  }

	  // compute the numerator and denomerator for efficiency plots
          for (UInt_t i = 0; i < 60; i++){
            if (nvtx == i){
              effPUd[i]++;
              if (passBoth) effPUn[i]++;
            }
            if (ptgg >= 10*i && ptgg < 10*(i+1)){
              effptd[i]++;
              if (passBoth) effptn[i]++;
            }
          }
 
        }// end if passes trigger
      }// end if passes pt cuts 
      
      if (hltDiphoton30Mass95==1){ //passes trigger
        if(passAll2 && pt2 > mgg/4) fTH1DMap["phi1_pho2pass"]->Fill(phi1,Weight);
        if(passAll1 && pt1 > mgg/3) fTH1DMap["phi2_pho1pass"]->Fill(phi2,Weight);
      }
    }// end if passes MET filter
   
  }// end loop over entries in tree


  if ( isData ){
    TH1D * nDataPassingFilters = Plotter::MakeTH1DPlot("nDataPassingFilters","",8,0.,8.,"","Events");
    nDataPassingFilters->Fill(0.5,nData); 
    nDataPassingFilters->Fill(1.5,nDataMETfil1); 
    nDataPassingFilters->Fill(2.5,nDataMETfil2); 
    nDataPassingFilters->Fill(3.5,nDataMETfil3); 
    nDataPassingFilters->Fill(4.5,nDataMETfil4); 
    nDataPassingFilters->Fill(5.5,nDataMETfil5); 
    nDataPassingFilters->Fill(6.5,nDataMETfil6); 
    nDataPassingFilters->Fill(7.5,nDataMETfil7); 
   
    nDataPassingFilters->GetXaxis()->SetBinLabel(1,"Orig");
    nDataPassingFilters->GetXaxis()->SetBinLabel(2,"GV");
    nDataPassingFilters->GetXaxis()->SetBinLabel(3,"eeBadSC");
    nDataPassingFilters->GetXaxis()->SetBinLabel(4,"HBHENoiseIso");
    nDataPassingFilters->GetXaxis()->SetBinLabel(5,"HBHENoise");
    nDataPassingFilters->GetXaxis()->SetBinLabel(6,"MuonBadTrack");
    nDataPassingFilters->GetXaxis()->SetBinLabel(7,"HadronTrackRes");
    nDataPassingFilters->GetXaxis()->SetBinLabel(8,"CSC");
 
    gStyle->SetOptStat(0);
    TCanvas *c1 = new TCanvas();
    c1->cd();
    nDataPassingFilters->Draw("HIST");
    CMSLumi(c1,11,fLumi);
    c1->SaveAs(Form("%s%s/data_metFilters.%s",fName.Data(),species.Data(),fType.Data()));

    TH1D * nDataPassingdphi = Plotter::MakeTH1DPlot("nDataPassingdphi","",4,0.,4.,"","Events");
    nDataPassingdphi->Fill(0.5,nDataOrig); 
    nDataPassingdphi->Fill(1.5,nDatadphi1); 
    nDataPassingdphi->Fill(2.5,nDatadphi2); 
    nDataPassingdphi->Fill(3.5,nDatadphi3); 

    nDataPassingdphi->GetXaxis()->SetBinLabel(1,"Orig");
    nDataPassingdphi->GetXaxis()->SetBinLabel(2,"max #Delta#phi(j,MET)"); 
    nDataPassingdphi->GetXaxis()->SetBinLabel(3,"min #Delta#phi(j,MET)"); 
    nDataPassingdphi->GetXaxis()->SetBinLabel(4,"#Delta#phi(gg,MET)"); 

    //TLegend * leg = new TLegend(0.6,0.8,0.9,0.934);// (x1,y1,x2,y2)
    //leg->SetBorderSize(4);
    //leg->SetTextSize(0.2);
    //leg->AddEntry(nDataPassingdphi,"l","Data");

    gStyle->SetOptStat(0);
    TCanvas *c2 = new TCanvas();
    c2->cd();
    nDataPassingdphi->Draw("HIST");
    //leg->Draw("SAME");
    CMSLumi(c2,12,fLumi);
    c2->SaveAs(Form("%s%s/data_dphicuts_metCUT.%s",fName.Data(),species.Data(),fType.Data()));
  }

  std::cout << "Number Events that have passed Analyzer: " << nentries << " events. " << std::endl;
  std::cout << "Number Events rejected by MET filters:   " << numFailingMETfil    << " out of " << nentries << " events. " << std::endl;

  //std::cout << "nvtx   Int " << fTH1DMap["nvtx"]->Integral() << std::endl;
  //std::cout << "ptJet1 Int " << fTH1DMap["ptJet1"]->Integral() << std::endl;

  std::cout << "======================================================" << std::endl;
  std::cout << "mgg integral = " << fTH1DMap["mgg_IsolateALL"]->Integral() << std::endl;
  //std::cout << "======================================================" << std::endl;
  UInt_t binMETze = fTH1DMap["t1pfmetCorr_partblind"]->GetXaxis()->FindBin(0.);
  UInt_t binMETlo = fTH1DMap["t1pfmetCorr_partblind"]->GetXaxis()->FindBin(METcut);
  UInt_t binMEThi = fTH1DMap["t1pfmetCorr_partblind"]->GetXaxis()->FindBin(299.);
  std::cout << "MET CUT IS " << METcut << std::endl; 
  //std::cout << "Events in MET tail of CorrMET			= " << fTH1DMap["t1pfmetCorr_partblind"]->Integral(binMETlo,binMEThi) << std::endl;
  ////std::cout << "======================================================" << std::endl;
  ////std::cout << "Events in MET tail of CorrMET + JetCut		= " << fTH1DMap["metCor_afterJetCut"]->Integral(binMETlo,binMEThi) << std::endl;
  //std::cout << "Events in MET tail of CorrMET + ggMETCut	= " << fTH1DMap["metCor_afterggMETCut"]->Integral(binMETlo,binMEThi) << std::endl;
  //std::cout << "Events in MET tail of CorrMET + gMETCut		= " << fTH1DMap["metCor_aftergMETCut"]->Integral(binMETlo,binMEThi) << std::endl;
  //std::cout << "Events in MET tail of CorrMET + maxJetMETCut	= " << fTH1DMap["metCor_maxJetMET"]->Integral(binMETlo,binMEThi) << std::endl;
  //std::cout << "Events in MET tail of CorrMET + minJetMETCut	= " << fTH1DMap["metCor_minJetMET"]->Integral(binMETlo,binMEThi) << std::endl;
  //std::cout << "Events in MET tail of CorrMET + JetMETPhiCut	= " << fTH1DMap["metCor_afterJetMETPhiCut"]->Integral(binMETlo,binMEThi) << std::endl;
  ////std::cout << "======================================================" << std::endl;
  ////std::cout << "Events in MET tail of CorrMET + gg Iso		= " << fTH1DMap["metCor_Isolategg"]->Integral(binMETlo,binMEThi) << std::endl;
  ////std::cout << "======================================================" << std::endl;
  //std::cout << "Events in MET tail of MET + ALL Iso		= " << fTH1DMap["met_IsolateALL"]->Integral(binMETlo,binMEThi) << std::endl;
  std::cout << "Events in MET tail of CorrMET + ALL Iso		= " << fTH1DMap["metCor_IsolateALL"]->Integral(binMETlo,binMEThi) << std::endl;
  std::cout << "Events in all MET  of CorrMET + ALL Iso		= " << fTH1DMap["metCor_IsolateALL"]->Integral(binMETze,binMEThi) << std::endl;
  std::cout << "Events in MET tail of MET + ALL Iso		= " << fTH1DMap["met_IsolateALL"]->Integral(binMETlo,binMEThi) << std::endl;
  std::cout << "Events in all MET  of MET + ALL Iso		= " << fTH1DMap["met_IsolateALL"]->Integral(binMETze,binMEThi) << std::endl;
  //std::cout << "Efficiency in         CorrMET + ALL Iso		= " << fTH1DMap["metCor_IsolateALL"]->Integral(binMETlo,binMEThi)/fTH1DMap["metCor_IsolateALL"]->Integral(binMETze,binMEThi) << std::endl; 
  //std::cout << "======================================================" << std::endl;
  std::cout << "======================================================" << std::endl;
  if (!isData) std::cout << "Total Vtx(BDT) Efficiency = " << Form("%1.3f",(Double_t)nEvents_goodVtx/(Double_t)nEvents_allVtx) << std::endl; 
  if (!isData) std::cout << "Total Vtx0     Efficiency = " << Form("%1.3f",(Double_t)nEvents_goodVtx0/(Double_t)nEvents_allVtx) << std::endl; 

  std::cout << " Starting number of events         = " << nentries << std::endl;
  std::cout << " Num of events after MET fil       = " << numMargaretMETfil   << std::endl;
  std::cout << " Num of events after Selection     = " << numMargaretSel      << std::endl;
  std::cout << " Num of events after Trigger       = " << numMargaretTrig     << std::endl;
  std::cout << " Num of events after Duplicate Rem = " << numMargaretDupRem   << std::endl;
  std::cout << " Num of events after Lepton Vetoes = " << numMargaretLepVeto  << std::endl;
  std::cout << " Num of events after maxDphiggMET  = " << numMargaretmaxggMET << std::endl;
  std::cout << " Num of events after maxDphiJetMET = " << numMargaretmaxjMET  << std::endl;
  std::cout << " Num of events after minDphiJetMET = " << numMargaretminjMET  << std::endl;
  std::cout << " Num of events in mgg [120,130]    = " << numMargaretmgg      << std::endl;
  std::cout << " Num of events after MET cut       = " << numMargaretMETcut   << std::endl;

  std::cout << "======================================================" << std::endl;
if (!isData){
  std::cout << " Num of events CorrMET before       = " << numMargaret_METCorr		<< std::endl;
  std::cout << " Num of events CorrMET after        = " << numMargaret_METCorrMETcut	<< std::endl;
  std::cout << " Num of events UncorrMET before     = " << numMargaret_METUncorr  	<< std::endl;
  std::cout << " Num of events UncorrMET after      = " << numMargaret_METUncorrMETcut	<< std::endl;

  Double_t eff_Corr = (Double_t)numMargaret_METCorrMETcut/(Double_t)numMargaret_METCorr; 
  Double_t err_Corr = TMath::Sqrt(eff_Corr*(1.0-eff_Corr)/(Double_t)numMargaret_METCorr);
  Double_t eff_Uncorr = (Double_t)numMargaret_METUncorrMETcut/(Double_t)numMargaret_METUncorr; 
  Double_t err_Uncorr = TMath::Sqrt(eff_Uncorr*(1.0-eff_Uncorr)/(Double_t)numMargaret_METUncorr);

  std::cout << " CorrMET   eff = " << eff_Corr   << " \\pm " << err_Corr   << std::endl; 
  std::cout << " UncorrMET eff = " << eff_Uncorr << " \\pm " << err_Uncorr << std::endl; 
  std::cout << "======================================================" << std::endl;
}

  Double_t effPU = 0;
  Double_t effpt = 0;
  Double_t bin = 0;
  for (UInt_t i=0; i<60; i++){
    bin = (Double_t)i;
    if (effPUd[i] > 0) effPU = (Double_t)effPUn[i]/(Double_t)effPUd[i];
    if (effptd[i] > 0) effpt = (Double_t)effptn[i]/(Double_t)effptd[i];
    fTH1DMap["eff_PU"]->Fill(bin,effPU); 
    fTH1DMap["eff_pt"]->Fill(bin*10,effpt); 
  }

  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(1,"nentries");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(2,"passPt");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(3,"passTrigger");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(4,"passCHiso");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(5,"passNHiso");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(6,"passPHiso");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(7,"passSieie");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(8,"passHoe");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(9,"passMgg");
  fTH1DMap["eff_sel"]->GetXaxis()->SetBinLabel(10,"passMet");  

  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(1,"nentries");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(2,"Pho26Pho16M60");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(3,"Pho36Pho22M15");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(4,"Pho42Pho25M15");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(5,"Dipho30M95");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(6,"Dipho30M70");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(7,"Dipho30M55");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(8,"Dipho30M55PV");
  fTH1DMap["hlt"]->GetXaxis()->SetBinLabel(9,"Dipho30M55EB");

  Plotter::SavePlots();

}// end Plotter::DoPlots


void Plotter::SetUpPlots(){
  // fill all plots from tree
  fTH1DMap["nvtx"]			= Plotter::MakeTH1DPlot("nvtx","",40,0.,40.,"nvtx","");
  fTH1DMap["mgg"]			= Plotter::MakeTH1DPlot("mgg","",26,99.,151.,"m_{#gamma#gamma} [GeV]",""); 
  fTH1DMap["ptgg"]			= Plotter::MakeTH1DPlot("ptgg","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");
  fTH1DMap["t1pfmetphi"]		= Plotter::MakeTH1DPlot("t1pfmetphi","",20,-4.,4.,"E_{T}^{miss} #phi","");
  fTH1DMap["t1pfmetCorr"]		= Plotter::MakeTH1DPlot("t1pfmetCorr","",75,0.,900,"p_{T}^{miss} [GeV]",""); 
  fTH1DMap["t1pfmetphiCorr"]		= Plotter::MakeTH1DPlot("t1pfmetphiCorr","",20,-4.,4.,"E_{T}^{miss} #phi","");
  fTH1DMap["metphi_metCUT"]		= Plotter::MakeTH1DPlot("metphi_metCUT","",20,-4.,4.,"E_{T}^{miss} #phi","");
  fTH1DMap["ggphi_metCUT"]		= Plotter::MakeTH1DPlot("ggphi_metCUT","",20,-4.,4.,"#phi(#gamma#gamma)","");
  fTH1DMap["ptggOverMET"]		= Plotter::MakeTH1DPlot("ptggOverMET","",100,0.,10.,"p_{T,#gamma#gamma}/E_{T}^{miss}","");
  fTH1DMap["pfmet"]			= Plotter::MakeTH1DPlot("pfmet","",100,0.,1000,"PF MET [GeV]","");
  fTH1DMap["pfmetphi"]			= Plotter::MakeTH1DPlot("pfmetphi","",80,-4.,4.,"PF MET #phi","");
  fTH1DMap["calomet"]			= Plotter::MakeTH1DPlot("calomet","",100,0.,1000,"calo MET [GeV]","");
  fTH1DMap["calometphi"]		= Plotter::MakeTH1DPlot("calometphi","",80,-4.,4.,"calo MET #phi","");
  fTH1DMap["phi1"]			= Plotter::MakeTH1DPlot("phi1","",20,-4.,4.,"#phi(#gamma1)","");
  fTH1DMap["phi2"]			= Plotter::MakeTH1DPlot("phi2","",20,-4.,4.,"#phi(#gamma2)","");
  fTH1DMap["eta1"]			= Plotter::MakeTH1DPlot("eta1","",20,-3.,3.,"#eta(#gamma1)","");
  fTH1DMap["eta1_beforeIDloose"]	= Plotter::MakeTH1DPlot("eta1_beforeIDloose","",20,-3.,3.,"#eta(#gamma1)","");
  fTH1DMap["eta1_afterIDloose"]		= Plotter::MakeTH1DPlot("eta1_afterIDloose","",20,-3.,3.,"#eta(#gamma1)","");
  fTH1DMap["eta2"]			= Plotter::MakeTH1DPlot("eta2","",20,-3.,3.,"#eta(#gamma2)","");
  fTH1DMap["pt1"]			= Plotter::MakeTH1DPlot("pt1","",60,0.,600.,"p_{T,#gamma1} [GeV]","");
  fTH1DMap["pt1_beforeIDloose"]		= Plotter::MakeTH1DPlot("pt1_beforeIDloose","",60,0.,600.,"p_{T,#gamma1} [GeV]","");
  fTH1DMap["pt1_afterIDloose"]		= Plotter::MakeTH1DPlot("pt1_afterIDloose","",60,0.,600.,"p_{T,#gamma1} [GeV]","");
  fTH1DMap["pt2"]			= Plotter::MakeTH1DPlot("pt2","",40,0.,400.,"p_{T,#gamma2} [GeV]","");
  fTH1DMap["chiso1"]			= Plotter::MakeTH1DPlot("chiso1","",75,-5.,15.,"CHiso(#gamma1)","");
  fTH1DMap["chiso2"]			= Plotter::MakeTH1DPlot("chiso2","",75,-5.,15.,"CHiso(#gamma2)","");
  fTH1DMap["neuiso1"]			= Plotter::MakeTH1DPlot("neuiso1","",75,-5.,15.,"NHiso(#gamma1)","");
  fTH1DMap["neuiso2"]			= Plotter::MakeTH1DPlot("neuiso2","",75,-5.,15.,"NHiso(#gamma2)","");
  fTH1DMap["phoiso1"]			= Plotter::MakeTH1DPlot("phoiso1","",75,-5.,15.,"PHiso(#gamma1)",""); 
  fTH1DMap["phoiso2"]			= Plotter::MakeTH1DPlot("phoiso2","",75,-5.,15.,"PHiso(#gamma2)",""); 
  fTH1DMap["sieie1"]			= Plotter::MakeTH1DPlot("sieie1","",75,0.,0.03,"#sigma_{i#eta i#eta}(#gamma1)",""); 
  fTH1DMap["sieie2"]			= Plotter::MakeTH1DPlot("sieie2","",75,0.,0.03,"#sigma_{i#eta i#eta}(#gamma2)",""); 
  fTH1DMap["sieie1EB"]			= Plotter::MakeTH1DPlot("sieie1EB","",75,0.,0.03,"#sigma_{i#eta i#eta}(#gamma1)",""); 
  fTH1DMap["sieie2EB"]			= Plotter::MakeTH1DPlot("sieie2EB","",75,0.,0.03,"#sigma_{i#eta i#eta}(#gamma2)",""); 
  fTH1DMap["sieie1EE"]			= Plotter::MakeTH1DPlot("sieie1EE","",75,0.,0.03,"#sigma_{i#eta i#eta}(#gamma1)",""); 
  fTH1DMap["sieie2EE"]			= Plotter::MakeTH1DPlot("sieie2EE","",75,0.,0.03,"#sigma_{i#eta i#eta}(#gamma2)",""); 
  fTH1DMap["hoe1"]			= Plotter::MakeTH1DPlot("hoe1","",25,0.,0.025,"H/E(#gamma1)","");
  fTH1DMap["hoe2"]			= Plotter::MakeTH1DPlot("hoe2","",25,0.,0.025,"H/E(#gamma2)","");
  fTH1DMap["r91"]			= Plotter::MakeTH1DPlot("r91","",50,0.,1.1,"R9(#gamma1)","");
  fTH1DMap["r92"]			= Plotter::MakeTH1DPlot("r92","",50,0.,1.1,"R9(#gamma2)","");
  fTH1DMap["eleveto1"]			= Plotter::MakeTH1DPlot("eleveto1","",2,0,2.0,"Electron Veto(#gamma1)","");
  fTH1DMap["eleveto2"]			= Plotter::MakeTH1DPlot("eleveto2","",2,0,2.0,"Electron Veto(#gamma2)","");
  fTH1DMap["ptJet1"]			= Plotter::MakeTH1DPlot("ptJet1","",60,0.,600.,"p_{T,Jet1} [GeV]","");		
  fTH1DMap["ptJet2"]			= Plotter::MakeTH1DPlot("ptJet2","",60,0.,600.,"p_{T,Jet2} [GeV]","");		
  fTH1DMap["phiJet1"]			= Plotter::MakeTH1DPlot("phiJet1","",20,-4.,4.,"#phi(Jet1)","");
  fTH1DMap["phiJet2"]			= Plotter::MakeTH1DPlot("phiJet2","",20,-4.,4.,"#phi(Jet2)","");
  fTH1DMap["etaJet1"]			= Plotter::MakeTH1DPlot("etaJet1","",20,-3.,3.,"#eta(Jet1)","");
  fTH1DMap["etaJet2"]			= Plotter::MakeTH1DPlot("etaJet2","",20,-3.,3.,"#eta(Jet2)","");
  fTH1DMap["dphiJet1MET"]		= Plotter::MakeTH1DPlot("dphiJet1MET","",20,-4.,4.,"#Delta#phi(Jet1,E_{T}^{miss})","");
  fTH1DMap["dphiJet2MET"]		= Plotter::MakeTH1DPlot("dphiJet2MET","",20,-4.,4.,"#Delta#phi(Jet2,E_{T}^{miss})","");
  fTH1DMap["absdphiJet1MET"]		= Plotter::MakeTH1DPlot("absdphiJet1MET","",20,-4.,4.,"|#Delta#phi(Jet1,E_{T}^{miss})|","");
  fTH1DMap["absdphiJet2MET"]		= Plotter::MakeTH1DPlot("absdphiJet2MET","",20,-4.,4.,"|#Delta#phi(Jet2,E_{T}^{miss})|","");
  fTH1DMap["absdphi_ggJet1"]		= Plotter::MakeTH1DPlot("absdphi_ggJet1","",20,-4,4.,"|#Delta#phi(Jet1,E_{T}^{miss})|","");
  fTH1DMap["deltaRphotons"]		= Plotter::MakeTH1DPlot("deltaRphotons","",20,0,4.,"#Delta R(#gamma1,#gamma2)","");

  fTH1DMap["pt1_afterMETcut"]		= Plotter::MakeTH1DPlot("pt1_afterMETcut","",40,0.,400.,"p_{T,#gamma1} [GeV]","");
  fTH1DMap["pt2_afterMETcut"]		= Plotter::MakeTH1DPlot("pt2_afterMETcut","",40,0.,400.,"p_{T,#gamma2} [GeV]","");
  fTH1DMap["ptgg_afterMETcut"]		= Plotter::MakeTH1DPlot("ptgg_afterMETcut","",40,0.,400.,"p_{T,#gamma#gamma} [GeV]","");

  fTH1DMap["clean_dphi_min_gMET"]        = Plotter::MakeTH1DPlot("clean_dphi_min_gMET","",40,0.,4.,"min|#Delta#phi(#gamma,E_{T}^{miss})|","");
  fTH1DMap["clean_dphi_min_gMET_metCUT"] = Plotter::MakeTH1DPlot("clean_dphi_min_gMET_metCUT","",40,0.,4.,"min|#Delta#phi(#gamma,E_{T}^{miss})|","");
  fTH1DMap["clean_dphi_min_jMET_metCUT"] = Plotter::MakeTH1DPlot("clean_dphi_min_jMET_metCUT","",40,0.,4.,"min|#Delta#phi(jet,E_{T}^{miss})|","");
  fTH1DMap["clean_dphi_hMET_metCUT"]     = Plotter::MakeTH1DPlot("clean_dphi_hMET_metCUT","",40,0.,4.,"|#Delta#phi(#gamma#gamma,E_{T}^{miss})|","");
  fTH1DMap["test_mgg_lowmet"]		 = Plotter::MakeTH1DPlot("mgg_lowmet","",41,99.,181.,"m_{#gamma#gamma} [GeV]","Events");  
  fTH1DMap["test_mgg_nocuts"]		 = Plotter::MakeTH1DPlot("mgg_nocuts","",41,99.,181.,"m_{#gamma#gamma} [GeV]","Events");  
  fTH1DMap["test_mgg_dphiJmet"]		 = Plotter::MakeTH1DPlot("mgg_dphiJmet","",41,99.,181.,"m_{#gamma#gamma} [GeV]","Events");  
  fTH1DMap["test_mgg_dphiGGmet"]	 = Plotter::MakeTH1DPlot("mgg_dphiGGmet","",41,99.,181.,"m_{#gamma#gamma} [GeV]","Events");  

  fTH1DMap["newstudies_MET_lowmgg"]	   = Plotter::MakeTH1DPlot("newstudies_MET_lowmgg","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["newstudies_MET_highmgg"]	   = Plotter::MakeTH1DPlot("newstudies_MET_highmgg","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["newstudies_MET_outmgg"]	   = Plotter::MakeTH1DPlot("newstudies_MET_outmgg","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["newstudies_MET_selmgg"]	   = Plotter::MakeTH1DPlot("newstudies_MET_selmgg","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["newstudies_ptgg_lowMET"]       = Plotter::MakeTH1DPlot("newstudies_ptgg_lowMET","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");  
  fTH1DMap["newstudies_ptgg_highMET"]      = Plotter::MakeTH1DPlot("newstudies_ptgg_highMET","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");  
  fTH1DMap["newstudies_njet_lowMET"]       = Plotter::MakeTH1DPlot("newstudies_njet_lowMET","",10,0.,10.,"Num Jets","");
  fTH1DMap["newstudies_njet_highMET"]      = Plotter::MakeTH1DPlot("newstudies_njet_highMET","",10,0.,10.,"Num Jets","");
  fTH1DMap["newstudies_mgg_njet"]          = Plotter::MakeTH1DPlot("newstudies_mgg_njet","",38,105.,181.,"m_{#gamma#gamma} [GeV]","Events");  
  fTH1DMap["newstudies_mgg_ptgg"]          = Plotter::MakeTH1DPlot("newstudies_mgg_ptgg","",38,105.,181.,"m_{#gamma#gamma} [GeV]","Events");  
  fTH1DMap["newstudies_mgg_dphihMET"]      = Plotter::MakeTH1DPlot("newstudies_mgg_dphihMET","",38,105.,181.,"m_{#gamma#gamma} [GeV]","Events");  
  fTH1DMap["newstudies_mgg_dphijMET"]      = Plotter::MakeTH1DPlot("newstudies_mgg_dphijMET","",38,105.,181.,"m_{#gamma#gamma} [GeV]","Events");  
  fTH1DMap["newstudies_dphihMET_lowMET"]   = Plotter::MakeTH1DPlot("newstudies_dphihMET_lowMET","",20,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");
  fTH1DMap["newstudies_dphihMET_highMET"]  = Plotter::MakeTH1DPlot("newstudies_dphihMET_highMET","",20,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");
  fTH1DMap["newstudies_dphijMET_lowMET"]   = Plotter::MakeTH1DPlot("newstudies_dphijMET_lowMET","",10,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["newstudies_dphijMET_highMET"]  = Plotter::MakeTH1DPlot("newstudies_dphijMET_highMET","",10,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["newstudies_dphihMET_lowMET_selmgg"]   = Plotter::MakeTH1DPlot("newstudies_dphihMET_lowMET_selmgg","",20,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");
  fTH1DMap["newstudies_dphihMET_highMET_selmgg"]  = Plotter::MakeTH1DPlot("newstudies_dphihMET_highMET_selmgg","",20,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");
  fTH1DMap["newstudies_dphijMET_lowMET_selmgg"]   = Plotter::MakeTH1DPlot("newstudies_dphijMET_lowMET_selmgg","",10,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["newstudies_dphijMET_highMET_selmgg"]  = Plotter::MakeTH1DPlot("newstudies_dphijMET_highMET_selmgg","",10,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["newstudies_dphihMET_lowMET_outmgg"]   = Plotter::MakeTH1DPlot("newstudies_dphihMET_lowMET_outmgg","",20,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");
  fTH1DMap["newstudies_dphihMET_highMET_outmgg"]  = Plotter::MakeTH1DPlot("newstudies_dphihMET_highMET_outmgg","",20,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");
  fTH1DMap["newstudies_dphijMET_lowMET_outmgg"]   = Plotter::MakeTH1DPlot("newstudies_dphijMET_lowMET_outmgg","",10,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["newstudies_dphijMET_highMET_outmgg"]  = Plotter::MakeTH1DPlot("newstudies_dphijMET_highMET_outmgg","",10,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["newstudies_mgg_addptgg"]       = Plotter::MakeTH1DPlot("newstudies_mgg_addptgg","",38,105.,181.,"m_{#gamma#gamma} [GeV]","Events");  
  fTH1DMap["newstudies_mgg_addnjet"]       = Plotter::MakeTH1DPlot("newstudies_mgg_addnjet","",38,105.,181.,"m_{#gamma#gamma} [GeV]","Events");  
  fTH1DMap["newstudies_mgg_adddphijMET"]   = Plotter::MakeTH1DPlot("newstudies_mgg_adddphijMET","",38,105.,181.,"m_{#gamma#gamma} [GeV]","Events");  
  fTH1DMap["newstudies_mgg_adddphihMET"]   = Plotter::MakeTH1DPlot("newstudies_mgg_adddphihMET","",38,105.,181.,"m_{#gamma#gamma} [GeV]","Events"); 
  fTH1DMap["newstudies_mgg_barrel"]        = Plotter::MakeTH1DPlot("newstudies_mgg_barrel","",38,105.,181.,"m_{#gamma#gamma} [GeV]","Events");  
  fTH1DMap["newstudies_mgg_else"]          = Plotter::MakeTH1DPlot("newstudies_mgg_else","",38,105.,181.,"m_{#gamma#gamma} [GeV]","Events");  


  fTH1DMap["studies_dphi_minjMET_lowMET"]	= Plotter::MakeTH1DPlot("studies_dphi_minjMET_lowMET","",10,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["studies_dphi_hMET_lowMET"]	 	= Plotter::MakeTH1DPlot("studies_dphi_hMET_lowMET","",20,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");
  fTH1DMap["studies_dphi_minjMET_lowMET10"]	= Plotter::MakeTH1DPlot("studies_dphi_minjMET_lowMET10","",10,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["studies_dphi_hMET_lowMET10"]	= Plotter::MakeTH1DPlot("studies_dphi_hMET_lowMET10","",20,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");
  fTH1DMap["studies_dphi_minjMET_lowMET20"]	= Plotter::MakeTH1DPlot("studies_dphi_minjMET_lowMET20","",10,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["studies_dphi_hMET_lowMET20"]	= Plotter::MakeTH1DPlot("studies_dphi_hMET_lowMET20","",20,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");
  fTH1DMap["studies_dphi_minjMET_lowMET30"]	= Plotter::MakeTH1DPlot("studies_dphi_minjMET_lowMET30","",10,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["studies_dphi_hMET_lowMET30"]	= Plotter::MakeTH1DPlot("studies_dphi_hMET_lowMET30","",20,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");
  fTH1DMap["studies_dphi_minjMET_lowMET40"]	= Plotter::MakeTH1DPlot("studies_dphi_minjMET_lowMET40","",10,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["studies_dphi_hMET_lowMET40"]	= Plotter::MakeTH1DPlot("studies_dphi_hMET_lowMET40","",20,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");

  fTH1DMap["absdphi_g1MET"]		= Plotter::MakeTH1DPlot("absdphi_g1MET","",10,0.,4.,"|#Delta#phi(Pho1,E_{T}^{miss})|","");
  fTH1DMap["absdphi_maxgMET"]		= Plotter::MakeTH1DPlot("absdphi_maxgMET","",10,0.,4.,"Max|#Delta#phi(Pho,E_{T}^{miss})|","");
  fTH1DMap["absdphi_maxgMET_metCUT"]	= Plotter::MakeTH1DPlot("absdphi_maxgMET_metCUT","",10,0.,4.,"Max|#Delta#phi(Pho,E_{T}^{miss})|","");
  fTH1DMap["absdphi_maxJetMET"]		= Plotter::MakeTH1DPlot("absdphi_maxJetMET","",10,0.,4.,"Max|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["absdphi_minJetMET"]		= Plotter::MakeTH1DPlot("absdphi_minJetMET","",10,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["absdphi_ggmet"]		= Plotter::MakeTH1DPlot("absdphi_ggmet","",20,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");
  fTH1DMap["dphi_ggmet"]		= Plotter::MakeTH1DPlot("dphi_ggmet","",20,-4.,4.,"#Delta#phi(#gamma#gamma,MET)","");
  fTH1DMap["absdphi_g1MET_metCUT"]	= Plotter::MakeTH1DPlot("absdphi_g1MET_metCUT","",40,0.,4.,"|#Delta#phi(Pho1,E_{T}^{miss})|","");
  fTH1DMap["absdphi_maxJetMET_metCUT"]	= Plotter::MakeTH1DPlot("absdphi_maxJetMET_metCUT","",40,0.,4.,"Max|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["absdphi_minJetMET_metCUT"]	= Plotter::MakeTH1DPlot("absdphi_minJetMET_metCUT","",40,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["absdphi_ggmet_metCUT"]	= Plotter::MakeTH1DPlot("absdphi_ggmet_metCUT","",40,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");
  fTH1DMap["absdphiUncorr_maxJetMET"]	= Plotter::MakeTH1DPlot("absdphiUncorr_maxJetMET","",10,0.,4.,"Max|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["absdphiUncorr_minJetMET"]	= Plotter::MakeTH1DPlot("absdphiUncorr_minJetMET","",10,0.,4.,"Min|#Delta#phi(Jet,E_{T}^{miss})|","");
  fTH1DMap["absdphiUncorr_ggmet"]	= Plotter::MakeTH1DPlot("absdphiUncorr_ggmet","",20,0.,4.,"|#Delta#phi(#gamma#gamma,MET)|","");

  fTH1DMap["met_afterJetMETPhiCut"]	= Plotter::MakeTH1DPlot("met_afterJetMETPhiCut","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["metCor_afterJetMETPhiCut"]	= Plotter::MakeTH1DPlot("metCorr_afterJetMETPhiCut","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["met_afterggMETCut"]		= Plotter::MakeTH1DPlot("met_afterggMETCut","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["metCor_afterggMETCut"]	= Plotter::MakeTH1DPlot("metCorr_afterggMETCut","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["met_aftergMETCut"]		= Plotter::MakeTH1DPlot("met_aftergMETCut","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["metCor_aftergMETCut"]	= Plotter::MakeTH1DPlot("metCorr_aftergMETCut","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["metCor_Sig"] 		= Plotter::MakeTH1DPlot("metCorr_Sig","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["met_afterJetCut"]		= Plotter::MakeTH1DPlot("met_afterJetCut","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["metCor_afterJetCut"]	= Plotter::MakeTH1DPlot("metCorr_afterJetCut","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["met_womaxJetMET"]		= Plotter::MakeTH1DPlot("met_womaxJetMET","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["metCor_womaxJetMET"]	= Plotter::MakeTH1DPlot("metCorr_womaxJetMET","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["met_maxJetMET"]		= Plotter::MakeTH1DPlot("met_maxJetMET","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["metCor_maxJetMET"]		= Plotter::MakeTH1DPlot("metCorr_maxJetMET","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["met_minJetMET"]		= Plotter::MakeTH1DPlot("met_minJetMET","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["metCor_minJetMET"]		= Plotter::MakeTH1DPlot("metCorr_minJetMET","",60,0.,300.,"p_{T}^{miss} [GeV]","");

  Float_t METbins[] = {0,10,20,30,40,50,60,70,80,90,100,150,200};
  Int_t numbins = sizeof(METbins)/sizeof(Float_t) -1;
  fTH1DMap["met_IsolateALL_Varbin"]	= Plotter::MakeVariableTH1DPlot("met_IsolateALL_Varbin","",numbins,METbins,"p_{T}^{miss} [GeV]","");
  fTH1DMap["metCor_IsolateALL_Varbin"]	= Plotter::MakeVariableTH1DPlot("metCorr_IsolateALL_Varbin","",numbins,METbins,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmet_selmgg"]		= Plotter::MakeVariableTH1DPlot("t1pfmet_selmgg","",numbins,METbins,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetCorr_selmgg_Varbin"]	= Plotter::MakeVariableTH1DPlot("t1pfmetCorr_selmgg_Varbin","",numbins,METbins,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetCorr_selmgg"]	= Plotter::MakeTH1DPlot("t1pfmetCorr_selmgg","",70,0.,350.,"E_{T}^{miss} [GeV]","Events");
 
  fTH1DMap["met_IsolateALL"]		= Plotter::MakeTH1DPlot("met_IsolateALL","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["metCor_IsolateALL"]		= Plotter::MakeTH1DPlot("metCorr_IsolateALL","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["metCor_IsolateALL_selmgg"]	= Plotter::MakeTH1DPlot("metCorr_IsolateALL_selmgg","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["met_Isolategg"]		= Plotter::MakeTH1DPlot("met_Isolategg","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["metCor_Isolategg"]		= Plotter::MakeTH1DPlot("metCorr_Isolategg","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["mgg_IsolateALL"]		= Plotter::MakeTH1DPlot("mgg_IsolateALL","",38,105.,181.,"m_{#gamma#gamma} [GeV]","Events");  
  fTH1DMap["mgg_IsolateALL_woPtgg"]	= Plotter::MakeTH1DPlot("mgg_IsolateALL_woPtgg","",38,105.,181.,"m_{#gamma#gamma} [GeV]","");  
  fTH1DMap["mgg_IsolateALL_wPtgg"]	= Plotter::MakeTH1DPlot("mgg_IsolateALL_wPtgg","",38,105.,181.,"m_{#gamma#gamma} [GeV]","");  
  fTH1DMap["mgg_IsolateALL_pt1"]	= Plotter::MakeTH1DPlot("mgg_IsolateALL_pt1","",38,105.,181.,"m_{#gamma#gamma} [GeV]","");  
  fTH1DMap["mgg_IsolateALL_pt2"]	= Plotter::MakeTH1DPlot("mgg_IsolateALL_pt2","",38,105.,181.,"m_{#gamma#gamma} [GeV]","");  
  fTH1DMap["mgg_IsolateALL_ptgg"]	= Plotter::MakeTH1DPlot("mgg_IsolateALL_ptgg","",38,105.,181.,"m_{#gamma#gamma} [GeV]","");  
  //fTH1DMap["mgg_IsolateALLmetCUT"]	= Plotter::MakeTH1DPlot("mgg_IsolateALLmetCUT","",41,99.,181.,"m_{#gamma#gamma} [GeV]","");  
  fTH1DMap["mgg_IsolateALLmetCUT"]	= Plotter::MakeTH1DPlot("mgg_IsolateALLmetCUT","",38,105.,181.,"m_{#gamma#gamma} [GeV]","");  
  fTH1DMap["mgg_IsolateALLlowCUT"]	= Plotter::MakeTH1DPlot("mgg_IsolateALLlowCUT","",38,105.,181.,"m_{#gamma#gamma} [GeV]","");  
  fTH1DMap["mgg_IsolateALLUncorrmetCUT"]= Plotter::MakeTH1DPlot("mgg_IsolateALLUncorrmetCUT","",41,99.,181.,"m_{#gamma#gamma} [GeV]","");  
  fTH1DMap["ptgg_IsolateALL"]		= Plotter::MakeTH1DPlot("ptgg_IsolateALL","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");  
  fTH1DMap["ptgg_IsolateALLmetCUT"]	= Plotter::MakeTH1DPlot("ptgg_IsolateALLmetCUT","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");  
  fTH1DMap["nvtx_IsolateALL"]		= Plotter::MakeTH1DPlot("nvtx_IsolateALL","",60,0.,60.,"nvtx","");  
  fTH1DMap["nvtx_IsolateALLmetCUT"]	= Plotter::MakeTH1DPlot("nvtx_IsolateALLmetCUT","",60,0.,60.,"nvtx","");  

  fTH1DMap["nvtx_afterJetCut"]		= Plotter::MakeTH1DPlot("nvtx_afterJetCut","",40,0.,40.,"nvtx","");
  fTH1DMap["mgg_afterJetCut"]		= Plotter::MakeTH1DPlot("mgg_afterJetCut","",26,99.,151.,"m_{#gamma#gamma} [GeV]","");  
  fTH1DMap["ptgg_afterJetCut"]		= Plotter::MakeTH1DPlot("ptgg_afterJetCut","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");
  fTH1DMap["nvtx_afterggMETCut"]	= Plotter::MakeTH1DPlot("nvtx_afterggMETCut","",40,0.,40.,"nvtx","");
  fTH1DMap["ptgg_afterggMETCut"]	= Plotter::MakeTH1DPlot("ptgg_afterggMETCut","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");
  fTH1DMap["pt1_afterggMETCut"]		= Plotter::MakeTH1DPlot("pt1_afterggMETCut","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");
  fTH1DMap["pt2_afterggMETCut"]		= Plotter::MakeTH1DPlot("pt2_afterggMETCut","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");

  fTH1DMap["jetInfo_CHfrac1"]		= Plotter::MakeTH1DPlot("jetInfo_CHfrac1","",20,0,1.,"CH frac","");
  fTH1DMap["jetInfo_NHfrac1"]		= Plotter::MakeTH1DPlot("jetInfo_NHfrac1","",20,0,1.,"NH frac","");
  fTH1DMap["jetInfo_NEMfrac1"]		= Plotter::MakeTH1DPlot("jetInfo_NEMfrac1","",20,0,1.,"NEM frac","");
  fTH1DMap["jetInfo_CEMfrac1"]		= Plotter::MakeTH1DPlot("jetInfo_CEMfrac1","",20,0,1.,"CEM frac","");
  fTH1DMap["jetInfo_PHfrac1"]		= Plotter::MakeTH1DPlot("jetInfo_PHfrac1","",20,0,1.,"Pho frac","");
  fTH1DMap["jetInfo_ELfrac1"]		= Plotter::MakeTH1DPlot("jetInfo_ELfrac1","",20,0,1.,"El frac","");
  fTH1DMap["jetInfo_MUfrac1"]		= Plotter::MakeTH1DPlot("jetInfo_MUfrac1","",20,0,1.,"Mu frac","");
  fTH1DMap["jetInfo_CHmult1"]		= Plotter::MakeTH1DPlot("jetInfo_CHmult1","",2,0,2.,"CH mult","");
  fTH1DMap["jetInfo_NEmult1"]		= Plotter::MakeTH1DPlot("jetInfo_NEmult1","",2,0,2.,"NE mult","");
  fTH1DMap["jetInfo_pt1"]		= Plotter::MakeTH1DPlot("jetInfo_pt1","",40,0,200,"p_{T}","");
  fTH1DMap["jetInfo_eta1"]		= Plotter::MakeTH1DPlot("jetInfo_eta1","",40,-5.,5.,"#eta","");
  fTH1DMap["jetInfo_phi1"]		= Plotter::MakeTH1DPlot("jetInfo_phi1","",20,-4.,4.,"#phi","");
  fTH1DMap["jetInfo_mass1"]		= Plotter::MakeTH1DPlot("jetInfo_mass1","",50,0,100,"mass [GeV]","");

  fTH1DMap["nJets"]			= Plotter::MakeTH1DPlot("nJets","",10,0.,10.,"Num Jets","");
  fTH1DMap["nElec"]			= Plotter::MakeTH1DPlot("nElec","",10,0.,10.,"Num Electrons","");
  fTH1DMap["nMuon"]			= Plotter::MakeTH1DPlot("nMuon","",10,0.,10.,"Num Muons","");
  fTH1DMap["nJets_metCUT"]		= Plotter::MakeTH1DPlot("nJets_metCUT","",10,0.,10.,"Num Jets","");
  fTH1DMap["nElec_metCUT"]		= Plotter::MakeTH1DPlot("nElec_metCUT","",10,0.,10.,"Num Electrons","");
  fTH1DMap["nMuon_metCUT"]		= Plotter::MakeTH1DPlot("nMuon_metCUT","",10,0.,10.,"Num Muons","");
  fTH1DMap["BDTindex"]			= Plotter::MakeTH1DPlot("BDTindex","",10,0.,10.,"BDT Vtx Index","");

  fTH1DMap["t1pfmet_partblind"]		= Plotter::MakeTH1DPlot("t1pfmet_partblind","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetCorr_partblind"]	= Plotter::MakeTH1DPlot("t1pfmetCorr_partblind","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetSumEt"]		= Plotter::MakeTH1DPlot("t1pfmetSumEt","",60,0.,300.,"Sum E_{T} [GeV]","");
  fTH1DMap["t1pfmetSumEtCorr"]		= Plotter::MakeTH1DPlot("t1pfmetSumEtCorr","",60,0.,300.,"Sum E_{T} [GeV]","");
  fTH1DMap["t1pfmet_scaledipho"]	= Plotter::MakeTH1DPlot("t1pfmet_scaledipho","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmet"]			= Plotter::MakeTH1DPlot("t1pfmet","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetJetEnUp"]		= Plotter::MakeTH1DPlot("JetEnUp","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetJetEnDown"]		= Plotter::MakeTH1DPlot("JetEnDown","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetJetResUp"]		= Plotter::MakeTH1DPlot("JetResUp","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetJetResDown"] 	= Plotter::MakeTH1DPlot("JetResDown","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetMuonEnUp"]		= Plotter::MakeTH1DPlot("MuonEnUp","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetMuonEnDown"]		= Plotter::MakeTH1DPlot("MuonEnDown","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetEleEnUp"]		= Plotter::MakeTH1DPlot("EleEnUp","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetEleEnDown"]		= Plotter::MakeTH1DPlot("EleEnDown","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetTauEnUp"]		= Plotter::MakeTH1DPlot("TauEnUp","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetTauEnDown"]		= Plotter::MakeTH1DPlot("TauEnDown","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetPhoEnUp"]		= Plotter::MakeTH1DPlot("PhoEnUp","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetPhoEnDown"]		= Plotter::MakeTH1DPlot("PhoEnDown","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetUnclEnUp"]		= Plotter::MakeTH1DPlot("UnclEnUp","",75,0.,900.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetUnclEnDown"]		= Plotter::MakeTH1DPlot("UnclEnDown","",75,0.,900.,"p_{T}^{miss} [GeV]","");

  fTH1DMap["vtx_eff_met_n"]		= Plotter::MakeTH1DPlot("vtx_eff_met_n","",200,0.,1000.,"p_{T}^{miss} [GeV]","Vtx Efficiency");	
  fTH1DMap["vtx_eff_met_d"]		= Plotter::MakeTH1DPlot("vtx_eff_met_d","",200,0.,1000.,"p_{T}^{miss} [GeV]","Vtx Efficiency");	
  fTH1DMap["vtx_eff_nvtx_n"]		= Plotter::MakeTH1DPlot("vtx_eff_nvtx_n","",40,0.,40.,"nvtx","Vtx Efficinecy");	
  fTH1DMap["vtx_eff_nvtx_d"]		= Plotter::MakeTH1DPlot("vtx_eff_nvtx_d","",40,0.,40.,"nvtx","Vtx Efficinecy");	
  fTH1DMap["vtx_eff_ptzp_n"]		= Plotter::MakeTH1DPlot("vtx_eff_ptzp_n","",200,0.,1000.,"Z' p_{T} [GeV]","Vtx Efficiency");	
  fTH1DMap["vtx_eff_ptzp_d"]		= Plotter::MakeTH1DPlot("vtx_eff_ptzp_d","",200,0.,1000.,"Z' p_{T} [GeV]","Vtx Efficiency");	
  fTH1DMap["vtx_eff_njet_n"]		= Plotter::MakeTH1DPlot("vtx_eff_njet_n","",10,0.,10.,"Num jets","Vtx Efficinecy");	
  fTH1DMap["vtx_eff_njet_d"]		= Plotter::MakeTH1DPlot("vtx_eff_njet_d","",10,0.,10.,"Num jets","Vtx Efficinecy");	

  // n minus 1 plots
  //fTH1DMap["nvtx_n-1"]		= Plotter::MakeTH1DPlot("nvtx_n-1","",40,0.,40.,"nvtx","");
  //fTH1DMap["mgg_n-1"]			= Plotter::MakeTH1DPlot("mgg_n-1","",26,99.,151.,"m_{#gamma#gamma} [GeV]","");  
  //fTH1DMap["ptgg_n-1"]		= Plotter::MakeTH1DPlot("ptgg_n-1","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");
  //fTH1DMap["t1pfmet_n-1"]		= Plotter::MakeTH1DPlot("t1pfmet_n-1","",25,0.,200.,"p_{T}^{miss} [GeV]","");
  //fTH1DMap["t1pfmetphi_n-1"]		= Plotter::MakeTH1DPlot("t1pfmetphi_n-1","",20,-4.,4.,"E_{T}^{miss} #phi","");
  //fTH1DMap["pfmet_n-1"]		= Plotter::MakeTH1DPlot("pfmet_n-1","",100,0.,1000,"PF MET [GeV]","");
  //fTH1DMap["pfmetphi_n-1"]		= Plotter::MakeTH1DPlot("pfmetphi_n-1","",80,-4.,4.,"PF MET #phi","");
  //fTH1DMap["calomet_n-1"]		= Plotter::MakeTH1DPlot("calomet_n-1","",100,0.,1000,"calo MET [GeV]","");
  //fTH1DMap["calometphi_n-1"]		= Plotter::MakeTH1DPlot("calometphi_n-1","",80,-4.,4.,"calo MET #phi","");
  //fTH1DMap["phi1_n-1"]		= Plotter::MakeTH1DPlot("phi1_n-1","",20,-4.,4.,"#phi(#gamma1)","");
  //fTH1DMap["phi2_n-1"]		= Plotter::MakeTH1DPlot("phi2_n-1","",20,-4.,4.,"#phi(#gamma2)","");
  //fTH1DMap["eta1_n-1"]		= Plotter::MakeTH1DPlot("eta1_n-1","",20,-3.,3.,"#eta(#gamma1)","");
  //fTH1DMap["eta2_n-1"]		= Plotter::MakeTH1DPlot("eta2_n-1","",20,-3.,3.,"#eta(#gamma2)","");
  //fTH1DMap["pt1_n-1"]			= Plotter::MakeTH1DPlot("pt1_n-1","",60,0.,600.,"p_{T,#gamma1} [GeV]","");
  //fTH1DMap["pt2_n-1"]			= Plotter::MakeTH1DPlot("pt2_n-1","",60,0.,600.,"p_{T,#gamma2} [GeV]","");
  //fTH1DMap["chiso1_n-1"]		= Plotter::MakeTH1DPlot("chiso1_n-1","",75,-5.,15.,"CHiso(#gamma1)","");
  //fTH1DMap["chiso2_n-1"]		= Plotter::MakeTH1DPlot("chiso2_n-1","",75,-5.,15.,"CHiso(#gamma2)","");
  //fTH1DMap["neuiso1_n-1"]		= Plotter::MakeTH1DPlot("neuiso1_n-1","",75,-5.,15.,"NHiso(#gamma1)","");
  //fTH1DMap["neuiso2_n-1"]		= Plotter::MakeTH1DPlot("neuiso2_n-1","",75,-5.,15.,"NHiso(#gamma2)","");
  //fTH1DMap["phoiso1_n-1"]		= Plotter::MakeTH1DPlot("phoiso1_n-1","",75,-5.,15.,"PHiso(#gamma1)",""); 
  //fTH1DMap["phoiso2_n-1"]		= Plotter::MakeTH1DPlot("phoiso2_n-1","",75,-5.,15.,"PHiso(#gamma2)",""); 
  //fTH1DMap["sieie1_n-1"]		= Plotter::MakeTH1DPlot("sieie1_n-1","",75,0.,0.03,"#sigma_{i#eta i#eta}(#gamma1)",""); 
  //fTH1DMap["sieie2_n-1"]		= Plotter::MakeTH1DPlot("sieie2_n-1","",75,0.,0.03,"#sigma_{i#eta i#eta}(#gamma2)",""); 
  //fTH1DMap["hoe1_n-1"]		= Plotter::MakeTH1DPlot("hoe1_n-1","",25,0.,0.025,"H/E(#gamma1)","");
  //fTH1DMap["hoe2_n-1"]		= Plotter::MakeTH1DPlot("hoe2_n-1","",25,0.,0.025,"H/E(#gamma2)","");
  //fTH1DMap["r91_n-1"]			= Plotter::MakeTH1DPlot("r91_n-1","",50,0.,1.1,"R9(#gamma1)","");
  //fTH1DMap["r92_n-1"]			= Plotter::MakeTH1DPlot("r92_n-1","",50,0.,1.1,"R9(#gamma2)","");

  // special plots
  fTH1DMap["phigg"]			= Plotter::MakeTH1DPlot("phigg","",20,-4.,4.,"#phi(#gamma#gamma)","");

  fTH1DMap["mgg_inverseselt1pfmet"]	= Plotter::MakeTH1DPlot("mgg_inverseselt1pfmet","",75,105.,180.,"m_{#gamma#gamma} [GeV]","");
  fTH1DMap["mgg_selt1pfmet"]		= Plotter::MakeTH1DPlot("mgg_selt1pfmet","",26,99.,151.,"m_{#gamma#gamma} [GeV]","");
  fTH1DMap["phi1_pho2pass"]     	= Plotter::MakeTH1DPlot("phi1_pho2pass","",80,-4.,4.,"","");
  fTH1DMap["phi2_pho1pass"]     	= Plotter::MakeTH1DPlot("phi2_pho1pass","",80,-4.,4.,"","");
  fTH1DMap["t1pfmet_zoom"]		= Plotter::MakeTH1DPlot("t1pfmet_zoom","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["t1pfmetCorr_zoom"]		= Plotter::MakeTH1DPlot("t1pfmetCorr_zoom","",60,0.,300.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["deta_gg"]			= Plotter::MakeTH1DPlot("deta_gg","",20,-3.,3.,"#Delta#eta(#gamma#gamma)","");
  fTH1DMap["absdeta_gg"]		= Plotter::MakeTH1DPlot("absdeta_gg","",20,0.,3.,"|#Delta#eta(#gamma#gamma)|","");
  fTH1DMap["ptgg_selt1pfmet"]		= Plotter::MakeTH1DPlot("ptgg_selt1pfmet","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");
  fTH1DMap["t1pfmet_selptgg"]		= Plotter::MakeTH1DPlot("t1pfmet_selptgg","",100,0.,1000.,"p_{T}^{miss} [GeV]","");

  fTH1DMap["metCorr_forShape"]		= Plotter::MakeTH1DPlot("metCorr_forShape","",30,0.,200.,"p_{T}^{miss} [GeV]","");
  fTH1DMap["mgg_forShape"]		= Plotter::MakeTH1DPlot("mgg_forShape","",26,99.,151.,"m_{#gamma#gamma} [GeV]","");  
  fTH1DMap["mgg_metCUT_forShape"]	= Plotter::MakeTH1DPlot("mgg_metCUT_forShape","",26,99.,151.,"m_{#gamma#gamma} [GeV]","");  

  //// pho cat plots
  //fTH1DMap["EBHighR9_mgg"]		= Plotter::MakeTH1DPlot("EBHighR9_mgg","",26,99.,151.,"m_{#gamma#gamma} [GeV]","");  
  //fTH1DMap["EBHighR9_ptgg"]		= Plotter::MakeTH1DPlot("EBHighR9_ptgg","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");
  //fTH1DMap["EBHighR9_t1pfmet"]	= Plotter::MakeTH1DPlot("EBHighR9_t1pfmet","",75,0.,900,"t1PF MET [GeV]","");
  //fTH1DMap["EBLowR9_mgg"]		= Plotter::MakeTH1DPlot("EBLowR9_mgg","",26,99.,151.,"m_{#gamma#gamma} [GeV]","");  
  //fTH1DMap["EBLowR9_ptgg"]		= Plotter::MakeTH1DPlot("EBLowR9_ptgg","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");
  //fTH1DMap["EBLowR9_t1pfmet"]		= Plotter::MakeTH1DPlot("EBLowR9_t1pfmet","",75,0.,900,"t1PF MET [GeV]","");
  //fTH1DMap["EEHighR9_mgg"]		= Plotter::MakeTH1DPlot("EEHighR9_mgg","",26,99.,151.,"m_{#gamma#gamma} [GeV]","");  
  //fTH1DMap["EEHighR9_ptgg"]		= Plotter::MakeTH1DPlot("EEHighR9_ptgg","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");
  //fTH1DMap["EEHighR9_t1pfmet"]	= Plotter::MakeTH1DPlot("EEHighR9_t1pfmet","",75,0.,900,"t1PF MET [GeV]","");
  //fTH1DMap["EELowR9_mgg"]		= Plotter::MakeTH1DPlot("EELowR9_mgg","",26,99.,151.,"m_{#gamma#gamma} [GeV]","");  
  //fTH1DMap["EELowR9_ptgg"]		= Plotter::MakeTH1DPlot("EELowR9_ptgg","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");
  //fTH1DMap["EELowR9_t1pfmet"]		= Plotter::MakeTH1DPlot("EELowR9_t1pfmet","",75,0.,900,"t1PF MET [GeV]","");

  // efficiency plots
  fTH1DMap["eff_sel"]			= Plotter::MakeTH1DPlot("eff_sel","",10,0.,10.,"","");
  fTH1DMap["selection"]			= Plotter::MakeTH1DPlot("selection","",6,-0.5,5.5,"","");
  fTH1DMap["selection_unwgt"]		= Plotter::MakeTH1DPlot("selection_unwgt","",8,-0.5,7.5,"","");
  fTH1DMap["eff_PU"]			= Plotter::MakeTH1DPlot("eff_PU","",60,0.,60.,"","");
  fTH1DMap["eff_pt"]			= Plotter::MakeTH1DPlot("eff_pt","",60,0.,600.,"","");
  fTH1DMap["hlt"]			= Plotter::MakeTH1DPlot("hlt","",10,0.,10,"","");
  fTH1DMap["sel_ptgg"]			= Plotter::MakeTH1DPlot("sel_ptgg","",200,0,200,"","");
  fTH1DMap["sel_dphi"]			= Plotter::MakeTH1DPlot("sel_dphi","",20,0,20,"","");
  fTH1DMap["sel_deta"]			= Plotter::MakeTH1DPlot("sel_deta","",20,0,20,"","");

  // 2D plots
  fTH2DMap["mgg_PU"]			= Plotter::MakeTH2DPlot("mgg_PU","",60,0.,60.,40,100.,300.,"nvtx","m_{#gamma#gamma} [GeV]");
  fTH2DMap["mgg_ptgg"] 			= Plotter::MakeTH2DPlot("mgg_ptgg","",50,0.,500.,40,100.,300.,"p_{T,#gamma#gamma} [GeV]","m_{#gamma#gamma}");
  fTH2DMap["t1pfmet_PU"]		= Plotter::MakeTH2DPlot("t1pfmet_PU","",60,50.,300.,100,0.,1000.,"nvtx","p_{T}^{miss} [GeV]");
  fTH2DMap["t1pfmet_ptgg"]		= Plotter::MakeTH2DPlot("t1pfmet_ptgg","",90,0.,900.,90,0.,900.,"p_{T,#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
  fTH2DMap["t1pfmet_dphi"]		= Plotter::MakeTH2DPlot("t1pfmet_dphi","",20,-4.,4.,25,0.,250.,"#Delta#phi(#gamma#gamma,E_{T}^{miss})","p_{T}^{miss} [GeV]");
  fTH2DMap["ptzp_njets"]		= Plotter::MakeTH2DPlot("ptzp_njets","",10,0.,10.,100,0.,500.,"Num Jets","Z' p_{T} [GeV]");

  // 2D plot for ABCD method
  fTH2DMap["met_mgg"]			= Plotter::MakeTH2DPlot("met_mgg","",50,100.,180.,50.,0.,600,"m_{#gamma#gamma} [GeV]","MET (T1pfmet) [GeV]");
  fTH2DMap["t1pfmet_mgg"]		= Plotter::MakeTH2DPlot("t1pfmet_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
  fTH2DMap["t1pfmet_mgg_unwgt"]		= Plotter::MakeTH2DPlot("t1pfmet_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
  // for ABCD with cuts for each mass point
  if (fWhichSel==0){
    // OptSel1
    fTH2DMap["Sel1_M1_met_mgg"]		= Plotter::MakeTH2DPlot("Sel1_M1_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel1_M1_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel1_M1_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel1_M2_met_mgg"]		= Plotter::MakeTH2DPlot("Sel1_M2_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel1_M2_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel1_M2_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel1_M3_met_mgg"]		= Plotter::MakeTH2DPlot("Sel1_M3_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel1_M3_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel1_M3_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel1_M4_met_mgg"]		= Plotter::MakeTH2DPlot("Sel1_M4_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel1_M4_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel1_M4_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel1_M5_met_mgg"]		= Plotter::MakeTH2DPlot("Sel1_M5_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel1_M5_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel1_M5_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel1_M6_met_mgg"]		= Plotter::MakeTH2DPlot("Sel1_M6_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel1_M6_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel1_M6_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel1_M7_met_mgg"]		= Plotter::MakeTH2DPlot("Sel1_M7_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel1_M7_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel1_M7_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    // OptSel2
    fTH2DMap["Sel2_M1_met_mgg"]		= Plotter::MakeTH2DPlot("Sel2_M1_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel2_M1_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel2_M1_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel2_M2_met_mgg"]		= Plotter::MakeTH2DPlot("Sel2_M2_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel2_M2_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel2_M2_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel2_M3_met_mgg"]		= Plotter::MakeTH2DPlot("Sel2_M3_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel2_M3_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel2_M3_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel2_M4_met_mgg"]		= Plotter::MakeTH2DPlot("Sel2_M4_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel2_M4_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel2_M4_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel2_M5_met_mgg"]		= Plotter::MakeTH2DPlot("Sel2_M5_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel2_M5_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel2_M5_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel2_M6_met_mgg"]		= Plotter::MakeTH2DPlot("Sel2_M6_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel2_M6_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel2_M6_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel2_M7_met_mgg"]		= Plotter::MakeTH2DPlot("Sel2_M7_met_mgg","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
    fTH2DMap["Sel2_M7_met_mgg_unwgt"]	= Plotter::MakeTH2DPlot("Sel2_M7_met_mgg_unwgt","",800,100.,300.,4000,0.,1000,"m_{#gamma#gamma} [GeV]","p_{T}^{miss} [GeV]");
  }

}// end Plotter::SetUpPlots

TH1D * Plotter::MakeVariableTH1DPlot(const TString hname, const TString htitle, const Int_t numbins, Float_t bins[], const TString xtitle, const TString ytitle){
  TString ytitleNew;
  if (ytitle=="") ytitleNew = "Events / GeV";
  else ytitleNew = ytitle;
  
  TH1D * hist = new TH1D(hname.Data(),htitle.Data(),numbins,bins);
  hist->GetXaxis()->SetTitle(xtitle.Data());
  hist->GetYaxis()->SetTitle(ytitleNew.Data());
  hist->Sumw2();
  gStyle->SetOptStat(1111111);
  return hist;
}// end Plotter::MakeVariableTH1DPlot

TH1D * Plotter::MakeTH1DPlot(const TString hname, const TString htitle, const Int_t nbins, const Double_t xlow, const Double_t xhigh, const TString xtitle, const TString ytitle){
  TString ytitleNew;
  Float_t binwidth = (xhigh-xlow)/nbins;
  //std::cout << "binwidth " <<  binwidth << std::endl;
  if (ytitle=="") ytitleNew = Form("Events / %2.1f GeV",binwidth);
  else ytitleNew = ytitle;
  //std::cout << "yTitle = " << ytitleNew.Data() << std::endl;
 
  TH1D * hist = new TH1D(hname.Data(),htitle.Data(),nbins,xlow,xhigh);
  hist->GetXaxis()->SetTitle(xtitle.Data());
  hist->GetYaxis()->SetTitle(ytitleNew.Data());
  hist->Sumw2();
  gStyle->SetOptStat(1111111);
  return hist;
}// end Plotter::MakeTH1DPlot

TH2D * Plotter::MakeTH2DPlot(const TString hname, const TString htitle, const Int_t xnbins, const Double_t xlow, const Double_t xhigh, const Int_t ynbins, const Double_t ylow, const Double_t yhigh, const TString xtitle, const TString ytitle){
  TH2D * hist = new TH2D(hname.Data(),htitle.Data(),xnbins,xlow,xhigh,ynbins,ylow,yhigh);
  hist->GetXaxis()->SetTitle(xtitle.Data());
  hist->GetYaxis()->SetTitle(ytitle.Data());
  gStyle->SetOptStat(0);
  return hist;
}// end Plotter::MakeTH2DPlot

TH1D * DrawOverflowBin(const TH1D * h){
    Int_t nbin = h->GetNbinsX()+1;
    Double_t overflow = h->GetBinContent(nbin); 

}


void Plotter::SavePlots(){
  outFile->cd();

  TCanvas * canv = new TCanvas();

  for (TH1DMapIter mapiter = fTH1DMap.begin(); mapiter != fTH1DMap.end(); mapiter++){
    canv->Clear();

    if ((*mapiter).second == (TH1D*) NULL)	{std::cout << "TH1D Null" << std::endl;}
    if (outFile == (TFile*) NULL)		{std::cout << "OutFile Null" << std::endl;}
    if (canv == (TCanvas*) NULL)		{std::cout << "Canvas Null" << std::endl;}

    //fTH1DNewMap[(*mapiter).first].second = DrawOverflowBin( (*mapiter).second );   
 
    if ((*mapiter).first=="metCor_IsolateALL_Varbin" || (*mapiter).first=="t1pfmet_selmgg" || (*mapiter).first=="met_IsolateALL_Varbin" || (*mapiter).first=="t1pfmetCorr_selmgg_Varbin"){
      for (UInt_t bin = 1; bin < (*mapiter).second->GetSize()-1; bin++){
	Float_t bincontent = (*mapiter).second->GetBinContent(bin);
        Float_t binwidth   = (*mapiter).second->GetBinWidth(bin);
	Float_t binerr 	   = (*mapiter).second->GetBinError(bin);
        Float_t newbinerr  = binerr/binwidth; 
        (*mapiter).second->SetBinContent(bin,bincontent/binwidth);
	(*mapiter).second->SetBinError(bin,newbinerr);
      }
    }
 

    (*mapiter).second->Write(); // save histos to root file 
    canv->cd();
    (*mapiter).second->Draw("HIST");

    CMSLumi(canv,0,fLumi);

    //// UNCOMMENT THESE LINES IF WANT TO MAKE OUTPUT FILES OF ALL PLOTS
    //canv->SetLogy(0);
    //canv->SaveAs(Form("%s%s/%s.%s",fName.Data(),species.Data(),(*mapiter).first.Data(),fType.Data()));

    //canv->SetLogy(1);
    //canv->SaveAs(Form("%s%s/%s_log.%s",fName.Data(),species.Data(),(*mapiter).first.Data(),fType.Data())); 

  }// end of loop over mapiter for 1d plots
  delete canv;

  TCanvas * canv2d = new TCanvas();

  for (TH2DMapIter mapiter = fTH2DMap.begin(); mapiter != fTH2DMap.end(); mapiter++){
    //canv->Clear();

    if ((*mapiter).second == (TH2D*) NULL)	{std::cout << "TH2D Null" << std::endl;}
    if (outFile == (TFile*) NULL)		{std::cout << "OutFile Null" << std::endl;}
    if (canv == (TCanvas*) NULL)		{std::cout << "Canvas Null" << std::endl;}

    (*mapiter).second->Write(); // save histos to root file 
    canv2d->cd();
    (*mapiter).second->Draw("colz");

    CMSLumi(canv2d,0,fLumi);

    canv2d->SetLogy(0);
    //canv2d->SaveAs(Form("%s%s/%s.%s",fName.Data(),species.Data(),(*mapiter).first.Data(),fType.Data()));
  }// end of loop over mapiter for 2d plots
  delete canv2d;

}// end Plotter::SavePlots

void Plotter::DeleteHists(){
  for (TH1DMapIter mapiter = fTH1DMap.begin(); mapiter != fTH1DMap.end(); mapiter++){
    delete ((*mapiter).second);
  }
  fTH1DMap.clear();

  for (TH2DMapIter mapiter = fTH2DMap.begin(); mapiter != fTH2DMap.end(); mapiter++){
    delete ((*mapiter).second);
  }
  fTH2DMap.clear();

}// end Plotter::DeleteHists

void Plotter::SetBranchAddresses(){
  tpho->SetBranchAddress("run", &run, &b_run);
  tpho->SetBranchAddress("event", &event, &b_event);
  tpho->SetBranchAddress("lumi", &lumi, &b_lumi);
  tpho->SetBranchAddress("rho", &rho, &b_rho);
  tpho->SetBranchAddress("sampleID", &sampleID, &b_sampleID);
  tpho->SetBranchAddress("weight", &weight,  &b_weight);
  tpho->SetBranchAddress("nvtx",   &nvtx,    &b_nvtx);
  tpho->SetBranchAddress("mgg",    &mgg,     &b_mgg);
  tpho->SetBranchAddress("ptgg",   &ptgg,    &b_ptgg);
  tpho->SetBranchAddress("seedEnergy1", &seedEnergy1, &b_seedEnergy1);
  tpho->SetBranchAddress("seedEnergy2", &seedEnergy2, &b_seedEnergy2);
  tpho->SetBranchAddress("t1pfmet", &t1pfmet, &b_t1pfmet);   
  tpho->SetBranchAddress("t1p2pfmet", &t1p2pfmet, &b_t1p2pfmet);
  tpho->SetBranchAddress("t1pfmetJetEnUp", &t1pfmetJetEnUp, &b_t1pfmetJetEnUp);
  tpho->SetBranchAddress("t1pfmetJetEnDown", &t1pfmetJetEnDown, &b_t1pfmetJetEnDown);
  tpho->SetBranchAddress("t1pfmetJetResUp", &t1pfmetJetResUp, &b_t1pfmetJetResUp);
  tpho->SetBranchAddress("t1pfmetJetResDown", &t1pfmetJetResDown, &b_t1pfmetJetResDown);
  tpho->SetBranchAddress("t1pfmetMuonEnUp", &t1pfmetMuonEnUp, &b_t1pfmetMuonEnUp);
  tpho->SetBranchAddress("t1pfmetMuonEnDown", &t1pfmetMuonEnDown, &b_t1pfmetMuonEnDown);
  tpho->SetBranchAddress("t1pfmetElectronEnUp", &t1pfmetElectronEnUp, &b_t1pfmetElectronEnUp);
  tpho->SetBranchAddress("t1pfmetElectronEnDown", &t1pfmetElectronEnDown, &b_t1pfmetElectronEnDown);
  tpho->SetBranchAddress("t1pfmetTauEnUp", &t1pfmetTauEnUp, &b_t1pfmetTauEnUp);
  tpho->SetBranchAddress("t1pfmetTauEnDown", &t1pfmetTauEnDown, &b_t1pfmetTauEnDown);
  tpho->SetBranchAddress("t1pfmetPhotonEnUp", &t1pfmetPhotonEnUp, &b_t1pfmetPhotonEnUp);
  tpho->SetBranchAddress("t1pfmetPhotonEnDown", &t1pfmetPhotonEnDown, &b_t1pfmetPhotonEnDown);
  tpho->SetBranchAddress("t1pfmetUnclusteredEnUp", &t1pfmetUnclusteredEnUp, &b_t1pfmetUnclusteredEnUp);
  tpho->SetBranchAddress("t1pfmetUnclusteredEnDown", &t1pfmetUnclusteredEnDown, &b_t1pfmetUnclusteredEnDown);
  tpho->SetBranchAddress("t1pfmetPhi", &t1pfmetPhi, &b_t1pfmetPhi);
  tpho->SetBranchAddress("t1pfmetSumEt", &t1pfmetSumEt, &b_t1pfmetSumEt);   
  tpho->SetBranchAddress("pfmet", &pfmet, &b_pfmet);   
  tpho->SetBranchAddress("pfmetPhi", &pfmetphi, &b_pfmetPhi);   
  tpho->SetBranchAddress("pfmetSumEt", &pfmetSumEt, &b_pfmetSumEt);   
  tpho->SetBranchAddress("calomet", &calomet, &b_calomet);   
  tpho->SetBranchAddress("calometPhi", &calometphi, &b_calometPhi);   
  tpho->SetBranchAddress("calometSumEt", &calometSumEt, &b_calometSumEt);   
  tpho->SetBranchAddress("genmatch1", &genmatch1, &b_genmatch1);  
  tpho->SetBranchAddress("genmatch2", &genmatch2, &b_genmatch2);   
  tpho->SetBranchAddress("pt1", &pt1, &b_pt1);   
  tpho->SetBranchAddress("pt2", &pt2, &b_pt2);   
  tpho->SetBranchAddress("chiso1", &chiso1, &b_chiso1);   
  tpho->SetBranchAddress("chiso2", &chiso2, &b_chiso2);   
  tpho->SetBranchAddress("neuiso1", &neuiso1, &b_neuiso1);   
  tpho->SetBranchAddress("neuiso2", &neuiso2, &b_neuiso2);   
  tpho->SetBranchAddress("phoiso1", &phoiso1, &b_phoiso1);   
  tpho->SetBranchAddress("phoiso2", &phoiso2, &b_phoiso2);   
  tpho->SetBranchAddress("sieie1", &sieie1, &b_sieie1);   
  tpho->SetBranchAddress("sieie2", &sieie2, &b_sieie2);   
  tpho->SetBranchAddress("hoe1", &hoe1, &b_hoe1);   
  tpho->SetBranchAddress("hoe2", &hoe2, &b_hoe2);   
  tpho->SetBranchAddress("r91", &r91, &b_r91);   
  tpho->SetBranchAddress("r92", &r92, &b_r92);   
  tpho->SetBranchAddress("phi1", &phi1, &b_phi1);   
  tpho->SetBranchAddress("phi2", &phi2, &b_phi2);   
  tpho->SetBranchAddress("eta1", &eta1, &b_eta1);   
  tpho->SetBranchAddress("eta2", &eta2, &b_eta2);   
  tpho->SetBranchAddress("eleveto1", &eleveto1, &b_eleveto1);   
  tpho->SetBranchAddress("eleveto2", &eleveto2, &b_eleveto2);  
  tpho->SetBranchAddress("presel1", &presel1, &b_presel1); 
  tpho->SetBranchAddress("presel2", &presel2, &b_presel2); 
  tpho->SetBranchAddress("sel1", &sel1, &b_sel1); 
  tpho->SetBranchAddress("sel2", &sel2, &b_sel2); 
  tpho->SetBranchAddress("passCHiso1", &passCHiso1, &b_passCHiso1);   
  tpho->SetBranchAddress("passCHiso2", &passCHiso2, &b_passCHiso2);   
  tpho->SetBranchAddress("passNHiso1", &passNHiso1, &b_passNHiso1);   
  tpho->SetBranchAddress("passNHiso2", &passNHiso2, &b_passNHiso2);   
  tpho->SetBranchAddress("passPHiso1", &passPHiso1, &b_passNHiso1);   
  tpho->SetBranchAddress("passPHiso2", &passPHiso2, &b_passNHiso2);   
  tpho->SetBranchAddress("passSieie1", &passSieie1, &b_passSieie1);
  tpho->SetBranchAddress("passSieie2", &passSieie2, &b_passSieie2);
  tpho->SetBranchAddress("passHoe1", &passHoe1, &b_passHoe1);
  tpho->SetBranchAddress("passHoe2", &passHoe2, &b_passHoe2);
  tpho->SetBranchAddress("passLooseCHiso1", &passLooseCHiso1, &b_passLooseCHiso1);
  tpho->SetBranchAddress("passLooseCHiso2", &passLooseCHiso2, &b_passLooseCHiso2);
  tpho->SetBranchAddress("passLooseNHiso1", &passLooseNHiso1, &b_passLooseNHiso1);
  tpho->SetBranchAddress("passLooseNHiso2", &passLooseNHiso2, &b_passLooseNHiso2);
  tpho->SetBranchAddress("passLoosePHiso1", &passLoosePHiso1, &b_passLoosePHiso1);
  tpho->SetBranchAddress("passLoosePHiso2", &passLoosePHiso2, &b_passLoosePHiso2);
  tpho->SetBranchAddress("passLooseSieie1", &passLooseSieie1, &b_passLooseSieie1);
  tpho->SetBranchAddress("passLooseSieie2", &passLooseSieie2, &b_passLooseSieie2);
  tpho->SetBranchAddress("passLooseHoe1", &passLooseHoe1, &b_passLooseHoe1);
  tpho->SetBranchAddress("passLooseHoe2", &passLooseHoe2, &b_passLooseHoe2);
  tpho->SetBranchAddress("passTightCHiso1", &passTightCHiso1, &b_passTightCHiso1);
  tpho->SetBranchAddress("passTightCHiso2", &passTightCHiso2, &b_passTightCHiso2);
  tpho->SetBranchAddress("passTightNHiso1", &passTightNHiso1, &b_passTightNHiso1);
  tpho->SetBranchAddress("passTightNHiso2", &passTightNHiso2, &b_passTightNHiso2);
  tpho->SetBranchAddress("passTightPHiso1", &passTightPHiso1, &b_passTightPHiso1);
  tpho->SetBranchAddress("passTightPHiso2", &passTightPHiso2, &b_passTightPHiso2);
  tpho->SetBranchAddress("passTightSieie1", &passTightSieie1, &b_passTightSieie1);
  tpho->SetBranchAddress("passTightSieie2", &passTightSieie2, &b_passTightSieie2);
  tpho->SetBranchAddress("passTightHoe1", &passTightHoe1, &b_passTightHoe1);
  tpho->SetBranchAddress("passTightHoe2", &passTightHoe2, &b_passTightHoe2);
  tpho->SetBranchAddress("hltPhoton26Photon16Mass60", &hltPhoton26Photon16Mass60, &b_hltPhoton26Photon16Mass60);
  tpho->SetBranchAddress("hltPhoton36Photon22Mass15", &hltPhoton36Photon22Mass15, &b_hltPhoton36Photon22Mass15);
  tpho->SetBranchAddress("hltPhoton42Photon25Mass15", &hltPhoton42Photon25Mass15, &b_hltPhoton42Photon25Mass15);
  tpho->SetBranchAddress("hltDiphoton30Mass95", &hltDiphoton30Mass95, &b_hltDiphoton30Mass95);
  tpho->SetBranchAddress("hltDiphoton30Mass70", &hltDiphoton30Mass70, &b_hltDiphoton30Mass70);
  tpho->SetBranchAddress("hltDiphoton30Mass55", &hltDiphoton30Mass55, &b_hltDiphoton30Mass55);
  tpho->SetBranchAddress("hltDiphoton30Mass55PV", &hltDiphoton30Mass55PV, &b_hltDiphoton30Mass55PV);
  tpho->SetBranchAddress("hltDiphoton30Mass55EB", &hltDiphoton30Mass55EB, &b_hltDiphoton30Mass55EB);
  tpho->SetBranchAddress("nEle", &nEle, &b_nEle);
  tpho->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
  tpho->SetBranchAddress("nJets20", &nJets20, &b_nJets20);
  tpho->SetBranchAddress("nJets30", &nJets30, &b_nJets30);
  tpho->SetBranchAddress("nJets40", &nJets40, &b_nJets40);
  tpho->SetBranchAddress("nJets50", &nJets50, &b_nJets50);
  tpho->SetBranchAddress("nLooseBjets", &nLooseBjets, &b_nLooseBjets);
  tpho->SetBranchAddress("nMediumBjets", &nMediumBjets, &b_nMediumBjets);
  tpho->SetBranchAddress("ptJetLead", &ptJetLead, &b_ptJetLead);
  tpho->SetBranchAddress("etaJetLead", &etaJetLead, &b_etaJetLead);
  tpho->SetBranchAddress("phiJetLead", &phiJetLead, &b_phiJetLead);
  tpho->SetBranchAddress("massJetLead", &massJetLead, &b_massJetLead);
  tpho->SetBranchAddress("indexJetLead", &indexJetLead, &b_indexJetLead);
  tpho->SetBranchAddress("NEMfracJet1", &NEMfracJet1, &b_NEMfracJet1);
  tpho->SetBranchAddress("CEMfracJet1", &CEMfracJet1, &b_CEMfracJet1);
  tpho->SetBranchAddress("ELfracJet1", &ELfracJet1, &b_ELfracJet1);
  tpho->SetBranchAddress("CHfracJet1", &CHfracJet1, &b_CHfracJet1);
  tpho->SetBranchAddress("NHfracJet1", &NHfracJet1, &b_NHfracJet1);
  tpho->SetBranchAddress("PHfracJet1", &PHfracJet1, &b_PHfracJet1);
  tpho->SetBranchAddress("MUfracJet1", &MUfracJet1, &b_MUfracJet1);
  tpho->SetBranchAddress("CHmultJet1", &CHmultJet1, &b_CHmultJet1);
  tpho->SetBranchAddress("NEmultJet1", &NEmultJet1, &b_NEmultJet1);
  tpho->SetBranchAddress("ptJetSubLead", &ptJetSubLead, &b_ptJetSubLead);
  tpho->SetBranchAddress("etaJetSubLead", &etaJetSubLead, &b_etaJetSubLead);
  tpho->SetBranchAddress("phiJetSubLead", &phiJetSubLead, &b_phiJetSubLead);
  tpho->SetBranchAddress("massJetSubLead", &massJetSubLead, &b_massJetSubLead);
  tpho->SetBranchAddress("indexJetSubLead", &indexJetSubLead, &b_indexJetSubLead);
  tpho->SetBranchAddress("NEMfracJet2", &NEMfracJet2, &b_NEMfracJet2);
  tpho->SetBranchAddress("CEMfracJet2", &CEMfracJet2, &b_CEMfracJet2);
  tpho->SetBranchAddress("ELfracJet2", &ELfracJet2, &b_ELfracJet2);
  tpho->SetBranchAddress("CHfracJet2", &CHfracJet2, &b_CHfracJet2);
  tpho->SetBranchAddress("NHfracJet2", &NHfracJet2, &b_NHfracJet2);
  tpho->SetBranchAddress("PHfracJet2", &PHfracJet2, &b_PHfracJet2);
  tpho->SetBranchAddress("MUfracJet2", &MUfracJet2, &b_MUfracJet2);
  tpho->SetBranchAddress("CHmultJet2", &CHmultJet2, &b_CHmultJet2);
  tpho->SetBranchAddress("NEmultJet2", &NEmultJet2, &b_NEmultJet2);
  tpho->SetBranchAddress("ptJet3", &ptJet3, &b_ptJet3);
  tpho->SetBranchAddress("etaJet3", &etaJet3, &b_etaJet3);
  tpho->SetBranchAddress("phiJet3", &phiJet3, &b_phiJet3);
  tpho->SetBranchAddress("massJet3", &massJet3, &b_massJet3);
  tpho->SetBranchAddress("indexJet3", &indexJet3, &b_indexJet3);
  tpho->SetBranchAddress("NEMfracJet3", &NEMfracJet3, &b_NEMfracJet3);
  tpho->SetBranchAddress("CEMfracJet3", &CEMfracJet3, &b_CEMfracJet3);
  tpho->SetBranchAddress("ELfracJet3", &ELfracJet3, &b_ELfracJet3);
  tpho->SetBranchAddress("CHfracJet3", &CHfracJet3, &b_CHfracJet3);
  tpho->SetBranchAddress("NHfracJet3", &NHfracJet3, &b_NHfracJet3);
  tpho->SetBranchAddress("PHfracJet3", &PHfracJet3, &b_PHfracJet3);
  tpho->SetBranchAddress("MUfracJet3", &MUfracJet3, &b_MUfracJet3);
  tpho->SetBranchAddress("CHmultJet3", &CHmultJet3, &b_CHmultJet3);
  tpho->SetBranchAddress("NEmultJet3", &NEmultJet3, &b_NEmultJet3);
  tpho->SetBranchAddress("ptJet4", &ptJet4, &b_ptJet4);
  tpho->SetBranchAddress("etaJet4", &etaJet4, &b_etaJet4);
  tpho->SetBranchAddress("phiJet4", &phiJet4, &b_phiJet4);
  tpho->SetBranchAddress("massJet4", &massJet4, &b_massJet4);
  tpho->SetBranchAddress("indexJet4", &indexJet4, &b_indexJet4);
  tpho->SetBranchAddress("NEMfracJet4", &NEMfracJet4, &b_NEMfracJet4);
  tpho->SetBranchAddress("CEMfracJet4", &CEMfracJet4, &b_CEMfracJet4);
  tpho->SetBranchAddress("ELfracJet4", &ELfracJet4, &b_ELfracJet4);
  tpho->SetBranchAddress("CHfracJet4", &CHfracJet4, &b_CHfracJet4);
  tpho->SetBranchAddress("NHfracJet4", &NHfracJet4, &b_NHfracJet4);
  tpho->SetBranchAddress("PHfracJet4", &PHfracJet4, &b_PHfracJet4);
  tpho->SetBranchAddress("MUfracJet4", &MUfracJet4, &b_MUfracJet4);
  tpho->SetBranchAddress("CHmultJet4", &CHmultJet4, &b_CHmultJet4);
  tpho->SetBranchAddress("NEmultJet4", &NEmultJet4, &b_NEmultJet4);
  tpho->SetBranchAddress("vhtruth", &vhtruth, &b_vhtruth);

  // met filters
  tpho->SetBranchAddress("metF_GV",&metF_GV, &b_metF_GV);
  tpho->SetBranchAddress("metF_globalTightHalo",&metF_globalTightHalo, &b_metF_globalTightHalo);
  tpho->SetBranchAddress("metF_HBHENoise", &metF_HBHENoise, &b_metF_HBHENoise);
  tpho->SetBranchAddress("metF_HBHENoiseIso", &metF_HBHENoiseIso, &b_metF_HBHENoiseIso);
  tpho->SetBranchAddress("metF_EcalDeadCell",&metF_EcalDeadCell, &b_metF_EcalDeadCell);
  tpho->SetBranchAddress("metF_badMuon",&metF_badMuon, &b_metF_badMuon);
  tpho->SetBranchAddress("metF_badChargedHadron",&metF_badChargedHadron, &b_metF_badChargedHadron); 
  tpho->SetBranchAddress("metF_CSC", &metF_CSC, &b_metF_CSC);
  tpho->SetBranchAddress("metF_eeBadSC", &metF_eeBadSC, &b_metF_eeBadSC);
  tpho->SetBranchAddress("metF_MuonBadTrack", &metF_MuonBadTrack, &b_metF_MuonBadTrack);
  tpho->SetBranchAddress("metF_HadronTrackRes", &metF_HadronTrackRes, &b_metF_HadronTrackRes);

  tpho->SetBranchAddress("higgsVtxX", &higgsVtxX, &b_higgsVtxX);
  tpho->SetBranchAddress("higgsVtxY", &higgsVtxY, &b_higgsVtxY);
  tpho->SetBranchAddress("higgsVtxZ", &higgsVtxZ, &b_higgsVtxZ);
  tpho->SetBranchAddress("vtxIndex", &vtxIndex, &b_vtxIndex);
  tpho->SetBranchAddress("vtxX", &vtxX, &b_vtxX);
  tpho->SetBranchAddress("vtxY", &vtxY, &b_vtxY);
  tpho->SetBranchAddress("vtxZ", &vtxZ, &b_vtxZ);
  tpho->SetBranchAddress("vtx0Z", &vtx0Z, &b_vtx0Z);
  tpho->SetBranchAddress("genVtxX", &genVtxX, &b_genVtxX);
  tpho->SetBranchAddress("genVtxY", &genVtxY, &b_genVtxY);
  tpho->SetBranchAddress("genVtxZ", &genVtxZ, &b_genVtxZ);
  tpho->SetBranchAddress("massCorrSmear", &massCorrSmear, &b_massCorrSmear);
  tpho->SetBranchAddress("massCorrSmearUp", &massCorrSmearUp, &b_massCorrSmearUp);
  tpho->SetBranchAddress("massCorrSmearDown", &massCorrSmearDown, &b_massCorrSmearDown);
  tpho->SetBranchAddress("massCorrScale", &massCorrScale, &b_massCorrScale);
  tpho->SetBranchAddress("massCorrScaleUp", &massCorrScaleUp, &b_massCorrScaleUp);
  tpho->SetBranchAddress("massCorrScaleDown", &massCorrScaleDown, &b_massCorrScaleDown);
  tpho->SetBranchAddress("massRaw", &massRaw, &b_massRaw);
  tpho->SetBranchAddress("mva1", &mva1, &b_mva1);
  tpho->SetBranchAddress("mva2", &mva2, &b_mva2);
  tpho->SetBranchAddress("genZ", &genZ, &b_genZ);
  tpho->SetBranchAddress("ptZ",  &ptZ,  &b_ptZ);
  tpho->SetBranchAddress("etaZ", &etaZ, &b_etaZ);
  tpho->SetBranchAddress("phiZ", &phiZ, &b_phiZ);
  tpho->SetBranchAddress("BDTptgg", &BDTptgg, &b_BDTptgg);
  tpho->SetBranchAddress("BDTmassRaw", &BDTmassRaw, &b_BDTmassRaw);
  tpho->SetBranchAddress("BDTr91", &BDTr91, &b_BDTr91);
  tpho->SetBranchAddress("BDTr92", &BDTr92, &b_BDTr92);
  tpho->SetBranchAddress("BDTvtxZ", &BDTvtxZ, &b_BDTvtxZ);
  tpho->SetBranchAddress("BDTindex", &BDTindex, &b_BDTindex);
  tpho->SetBranchAddress("massOrig", &massOrig, &b_massOrig);
  tpho->SetBranchAddress("ptggOrig", &ptggOrig, &b_ptggOrig);
  tpho->SetBranchAddress("t1pfmetCorr", &t1pfmetCorr, &b_t1pfmetCorr);   
  tpho->SetBranchAddress("t1pfmetCorrPx", &t1pfmetCorrPx, &b_t1pfmetCorrPx);   
  tpho->SetBranchAddress("t1pfmetCorrPy", &t1pfmetCorrPy, &b_t1pfmetCorrPy);   
  tpho->SetBranchAddress("t1pfmetCorrPhi", &t1pfmetCorrPhi, &b_t1pfmetCorrPhi);   

  //tpho->SetBranchAddress("", &, &b_);
  
}// end Plotter::SetBranchAddresses


void Plotter::DeleteBranches(){
  delete b_weight;
  delete b_nvtx;
  delete b_mgg;
  delete b_ptgg;
  delete b_pt1;
  delete b_pt2;
}// end Plotter::DeleteBranches


void Plotter::FindMinAndMax(TH1F *& h, int plotLog){
  Float_t max = h->GetMaximum();
  if (plotLog==1) h->SetMaximum(10*max);
  if (plotLog==0) h->SetMaximum(2*max);

  Float_t min = 1000;
  Bool_t newmin = false;

  for (Int_t bin=1; bin <= h->GetNbinsX(); bin++){
    Float_t tmpmin = h->GetBinContent(bin);
    if ((tmpmin < min) && (tmpmin > 0)){
      min = tmpmin;
      newmin = true;
    }
  }

  if (newmin){
    h->SetMinimum(0.90*min);
  }
}// end Plotter::FindMinAndMax



