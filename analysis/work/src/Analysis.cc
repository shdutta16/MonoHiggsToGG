#include "../interface/Analysis.hh"
#include "../interface/deltaPhi.hh"

Analysis::Analysis(TString inName, TString outName, TString inSpecies, Double_t lumi,
                   Bool_t Data, Bool_t Blind, TString type, DblVec metCorr, Int_t whichSel)
{

  //------------------------------------------------------------------------
  // Setup variables from inputs
  //------------------------------------------------------------------------
  fType = type;  
  isData = Data;
  doBlind = Blind;
  fMETCorr = metCorr;
  fWhichSel = whichSel;
  fLumi = lumi;

  //------------------------------------------------------------------------
  // Get input ROOT file
  //------------------------------------------------------------------------
  name = inName;
  species = inSpecies;
  inFile = TFile::Open(Form("%s%s.root",name.Data(),species.Data()));
  CheckValidFile(inFile,Form("%s%s.root",name.Data(),species.Data()));  
  // Open Tree from inFile
  tpho = (TTree*)inFile->Get("DiPhotonTree"); 
  CheckValidTree(tpho,"DiPhotonTree",Form("%s%s.root",name.Data(),species.Data()));
  Analysis::SetBranchAddresses();

  //------------------------------------------------------------------------
  // Pick up h_selection plot to get analyzer eff
  //------------------------------------------------------------------------
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
    //standardTH1Map["selection_unwgt"] = (TH1D*)fSel_unwgt->Clone();
  }
  std::cout << "Finished getting the h_selection & h_selection_unwgt" << std::endl;  

  //------------------------------------------------------------------------
  // Make TLorentzVectors for objects in event 
  //------------------------------------------------------------------------
  TLorentzVector *fLorenzVec1           = new TLorentzVector();
  TLorentzVector *fLorenzVec2           = new TLorentzVector();
  TLorentzVector *fLorenzVecgg          = new TLorentzVector();
  TLorentzVector *fLorenzVecJet1        = new TLorentzVector();
  TLorentzVector *fLorenzVecJet2        = new TLorentzVector();
  TLorentzVector *fLorenzVecJet3        = new TLorentzVector();
  TLorentzVector *fLorenzVecJet4        = new TLorentzVector();

  //------------------------------------------------------------------------
  // Set up output root file
  //------------------------------------------------------------------------
  TString fName = outName;
  fOutDir = fName.Data();
  fOutDir+="/";
  fOutDir+=species.Data();
  MakeOutDir(fOutDir.Data());
  // Make output ROOT file
  outFile = new TFile(Form("%s/plots_%s.root",fOutDir.Data(),species.Data()),"UPDATE");
  CheckValidFile(outFile,Form("%s/plots_%s.root",fOutDir.Data(),species.Data()));

}// end Analysis::Analysis

Analysis::~Analysis()
{
  //------------------------------------------------------------------------
  // Finish by deleting everything
  //------------------------------------------------------------------------
  std::cout << "Finished & Deleting" <<std::endl;
  std::cout << "Deleting inTree" <<std::endl;
  delete tpho;
  std::cout << "Deleting inFile" <<std::endl;
  delete inFile;
  std::cout << "Deleting outFile" <<std::endl;
  delete outFile;
  std::cout << "Finished Deleting" <<std::endl;
}// end Analysis::~Analysis

void Analysis::DoPlots(int prompt)
{
  //------------------------------------------------------------------------
  // Setup which plots to do
  //------------------------------------------------------------------------
  if (Config::doStandard) Analysis::SetupStandardPlots();
  if (Config::doNminus1)  Analysis::SetupNminus1Plots();
  if (Config::doEffPlots) Analysis::SetupEffPlots();

  //------------------------------------------------------------------------
  // Setup MET threshold  
  //------------------------------------------------------------------------
  Double_t metCut = 100;            // OrigSel
  if (fWhichSel==1) metCut = 130;   // OptSel1
  if (fWhichSel==2) metCut = 130;   // OptSel2
  Double_t lowMetCut = 0;
  if (fWhichSel==2) lowMetCut = 50; // OptSel2

  //------------------------------------------------------------------------
  // Setup counters  
  //------------------------------------------------------------------------
  Int_t numFail_metF = 0;	// store nEvents failing any MET filter

  //------------------------------------------------------------------------
  // Loop over all events 
  //------------------------------------------------------------------------
  nentries = tpho->GetEntries();
  for ( UInt_t entry = 0; entry < nentries; entry++){
    tpho->GetEntry(entry);

    //------------------------------------------------------------------------
    // Fill TLorentzVectors for the event
    //------------------------------------------------------------------------
    fLorenzVec1.SetPtEtaPhiM(pt1,eta1,phi1,0.);
    fLorenzVec2.SetPtEtaPhiM(pt2,eta2,phi2,0.);
    fLorenzVecgg = fLorenzVec1 + fLorenzVec2;
    fLorenzVecJet1.SetPtEtaPhiM(ptJetLead,etaJetLead,phiJetLead,massJetLead);
    fLorenzVecJet2.SetPtEtaPhiM(ptJetSubLead,etaJetSubLead,phiJetSubLead,massJetSubLead);
    fLorenzVecJet3.SetPtEtaPhiM(ptJet3,etaJet3,phiJet3,massJet3);
    fLorenzVecJet4.SetPtEtaPhiM(ptJet4,etaJet4,phiJet4,massJet4);

    //------------------------------------------------------------------------
    // MET phi correction, old method
    //------------------------------------------------------------------------
    //Double_t t1pmfetCorrX, t1pfmetCorrY;
    //t1pfmetCorrX = t1pfmet*cos(t1pfmetPhi) - (fMETCorr[0] + fMETCorr[1]*t1pfmetSumEt);
    //t1pfmetCorrY = t1pfmet*sin(t1pfmetPhi) - (fMETCorr[2] + fMETCorr[3]*t1pfmetSumEt);
    //TLorentzVector correctedMet;
    //correctedMet.SetPxPyPzE(t1pfmetCorrX,t1pfmetCorrY,0,t1pfmetSumEt);  
    //Double_t t1pfmetCorrPhi = correctedMet.Phi();
    //Double_t t1pfmetCorr = correctedMet.Pt();

    //------------------------------------------------------------------------
    // Calculate the weight (could put in a PU reweight here) 
    //------------------------------------------------------------------------
    Double_t wgt = weight; 

    //------------------------------------------------------------------------
    // Check that data passes trigger
    //------------------------------------------------------------------------
    Bool_t passTrigger      = (isData)? (hltDiphoton30Mass95):true;
    if (!passTrigger) continue;

    //------------------------------------------------------------------------
    // Check samples pass MET filters
    //------------------------------------------------------------------------
    Bool_t passMetFil_All   = (metF_GV && metF_globalTightHalo && metF_HBHENoise && metF_HBHENoiseIso 
                              && metF_EcalDeadCell && metF_badMuon && metF_badChargedHadron);
    Bool_t passMetFil_Data  = (isData)?(metF_eeBadSC):true; // filter for data only
    Bool_t passMETfil       = (passMetFil_All && passMetFil_Data);
    if (!passMETfil){ numFail_metF++; continue; }
 
    //------------------------------------------------------------------------
    // Remove PP photons in QCD and Gjets samples
    //------------------------------------------------------------------------
    if (prompt==1 /*GJet*/ && (genmatch1==1 && genmatch2==1)) continue;
    if (prompt==2 /*QCD */ && (genmatch1==1 && genmatch2==1)) continue; 

    //------------------------------------------------------------------------
    // DeltaPhi (higgs,MET) 
    //------------------------------------------------------------------------
    Double_t dphi_ggMET = TMath::Abs(deltaPhi(fLorenzVecgg.Phi(),t1pfmetCorrPhi));
    Bool_t pass_dphi_hMET = (dphi_ggMET > 2.1)? true:false;
    
    //------------------------------------------------------------------------
    // DeltaPhi(jet,MET)
    //------------------------------------------------------------------------
    Double_t min_dphi_JetMET = 10.;
    if (nJets50 > 0){
      Double_t dphiJet1METmin = 10;
      Double_t dphiJet2METmin = 10;
      Double_t dphiJet3METmin = 10;
      Double_t dphiJet4METmin = 10;
      if ( ptJetLead > 50 )    dphiJet1METmin = TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetCorrPhi));
      if ( ptJetSubLead > 50 ) dphiJet2METmin = TMath::Abs(deltaPhi(fLorenzVecJet2.Phi(),t1pfmetCorrPhi));
      if ( ptJet3 > 50 )       dphiJet3METmin = TMath::Abs(deltaPhi(fLorenzVecJet3.Phi(),t1pfmetCorrPhi));
      if ( ptJet4 > 50 )       dphiJet4METmin = TMath::Abs(deltaPhi(fLorenzVecJet4.Phi(),t1pfmetCorrPhi));

      // find the min_dphi_JetMET 
      if (dphiJet1METmin < min_dphi_JetMET) min_dphi_JetMET = dphiJet1METmin;	   
      if (dphiJet2METmin < min_dphi_JetMET) min_dphi_JetMET = dphiJet2METmin;	   
      if (dphiJet3METmin < min_dphi_JetMET) min_dphi_JetMET = dphiJet3METmin;	   
      if (dphiJet4METmin < min_dphi_JetMET) min_dphi_JetMET = dphiJet4METmin;	   
    }
   
    Bool_t pass_dphi_jMET = (min_dphi_JetMET >= 0.5)? true:false; 
    Bool_t pass_dphi = (pass_dphi_hMET && pass_dphi_jMET); 

    //------------------------------------------------------------------------
    // Deal with the blinding 
    //------------------------------------------------------------------------
    Bool_t mggSB   = (mgg < 115 || mgg > 135);                     // in mgg SB
    Bool_t mggOkay = (!doBlind || (doBlind && mggSB))? true:false; // not blind, or in mgg SB (use for plots other than mgg)
    Bool_t mggPlot = (isData)? (mggOkay):true;                     // check for data the mgg ok (use for mgg plots)

    //------------------------------------------------------------------------
    // Fill histograms -- before all cuts
    //------------------------------------------------------------------------
    //--------> Fill the n minus 1 plots
    if (Config::doNminus1){
       nminus1TH1Map["n1_pt1"]->Fill(pt1,wgt);
       nminus1TH1Map["n1_pt2"]->Fill(pt2,wgt);
       nminus1TH1Map["n1_ptgg"]->Fill(ptgg,wgt);
       nminus1TH1Map["n1_nvtx"]->Fill(nvtx,wgt);
       if (mggPlot) nminus1TH1Map["n1_mgg"]->Fill(mgg,wgt);
       if (mggOkay) nminus1TH1Map["n1_met"]->Fill(t1pfmetCorr,wgt);
    }

    //------------------------------------------------------------------------
    // Apply kinematic selection 
    //------------------------------------------------------------------------
    //if (mgg < 105 || mgg > 200) continue; // to match the inputs to the fit
    Bool_t passSel = false;
    if (fWhichSel==0) passSel = true; // original selection
    Bool_t passLowMetSel  = (t1pfmetCorr <  metCut && pt1 > 0.45*mgg && pt2 > 0.25*mgg && ptgg > 75)? true:false;
    Bool_t passHighMetSel = (t1pfmetCorr >= metCut && pt1 > 0.50*mgg && pt2 > 0.25*mgg && ptgg > 90)? true:false;    
    if (fWhichSel==1 && (pt1 > 0.50*mgg && pt2 > 0.25*mgg && ptgg > 90)) passSel = true; // OptSel1: high-MET cuts only 
    if (fWhichSel==2 && (passLowMetSel || passHighMetSel)) passSel = true;               // OptSel2: low- and high-MET cuts
    if (!passSel) continue;

    //------------------------------------------------------------------------
    // Fill the histograms -- after kin. cuts, before lep,jet,dphi cuts
    //------------------------------------------------------------------------
    //--------> Fill the standard plots
    if (Config::doStandard){
       if (t1pfmetCorr >= metCut) standardTH1Map["nElec"]->Fill(nEle,wgt);
       if (t1pfmetCorr >= metCut) standardTH1Map["nMuon"]->Fill(nMuons,wgt);
       if (t1pfmetCorr >= metCut) standardTH1Map["nJets"]->Fill(nJets30,wgt);
       standardTH1Map["jetInfo_CHfrac1"]->Fill(CHfracJet1,wgt);
       standardTH1Map["jetInfo_NHfrac1"]->Fill(NHfracJet1,wgt);
       standardTH1Map["jetInfo_pt1"]->Fill(ptJetLead,wgt);
       standardTH1Map["jetInfo_eta1"]->Fill(etaJetLead,wgt);
       standardTH1Map["jetInfo_phi1"]->Fill(phiJetLead,wgt);
       standardTH1Map["jetInfo_mass1"]->Fill(massJetLead,wgt);
    }

    //------------------------------------------------------------------------
    // Apply all other selection 
    //------------------------------------------------------------------------
    //if (nEle >= 1 || nMuons >= 1) continue; // lepton veto
    if (nJets30 > 2) continue;              // jet veto
    if (!pass_dphi) continue;               // dphi cuts
 
    //------------------------------------------------------------------------
    // Fill the histograms -- after final cuts 
    //------------------------------------------------------------------------
    //--------> Fill the standard plots
    if (Config::doStandard){
       if (mggPlot) standardTH1Map["mgg"]->Fill(mgg,wgt);
       if (mggPlot && t1pfmetCorr > lowMetCut && t1pfmetCorr < metCut)  standardTH1Map["mgg_loMET"]->Fill(mgg,wgt);
       if (mggPlot && t1pfmetCorr >= metCut) standardTH1Map["mgg_hiMET"]->Fill(mgg,wgt);
       if (mggOkay) standardTH1Map["t1pfmetCorr_var"]->Fill(t1pfmetCorr,wgt);
       if (mggOkay) standardTH1Map["t1pfmetCorr"]->Fill(t1pfmetCorr,wgt);
       if (mggOkay) standardTH1Map["t1pfmet"]->Fill(t1pfmet,wgt);
       standardTH1Map["nvtx"]->Fill(nvtx,wgt);
       standardTH1Map["ptgg"]->Fill(ptgg,wgt);
       standardTH1Map["pt1"]->Fill(pt1,wgt);
       standardTH1Map["pt2"]->Fill(pt2,wgt);
       standardTH1Map["phi1"]->Fill(phi1,wgt);
       standardTH1Map["phi2"]->Fill(phi2,wgt);
       standardTH1Map["eta1"]->Fill(eta1,wgt);
       standardTH1Map["eta2"]->Fill(eta2,wgt);
    }

    //--------> Fill the efficiency plots
    if (Config::doEffPlots){
       //--------> MET uncertainty plots
       effTH1Map["met_unc"]->Fill(t1pfmetCorr,wgt);
       effTH1Map["met_unc_JetEnUp"]->Fill(t1pfmetJetEnUp,wgt);
       effTH1Map["met_unc_JetEnDown"]->Fill(t1pfmetJetEnDown,wgt);
       effTH1Map["met_unc_JetResUp"]->Fill(t1pfmetJetResUp,wgt);
       effTH1Map["met_unc_JetResDown"]->Fill(t1pfmetJetResDown,wgt);
       effTH1Map["met_unc_MuonEnUp"]->Fill(t1pfmetMuonEnUp,wgt);
       effTH1Map["met_unc_MuonEnDown"]->Fill(t1pfmetMuonEnDown,wgt);
       effTH1Map["met_unc_ElEnUp"]->Fill(t1pfmetElectronEnUp,wgt);
       effTH1Map["met_unc_ElEnDown"]->Fill(t1pfmetElectronEnDown,wgt);
       effTH1Map["met_unc_TauEnUp"]->Fill(t1pfmetTauEnUp,wgt);
       effTH1Map["met_unc_TauEnDown"]->Fill(t1pfmetTauEnDown,wgt);
       effTH1Map["met_unc_PhoEnUp"]->Fill(t1pfmetPhotonEnUp,wgt);
       effTH1Map["met_unc_PhoEnDown"]->Fill(t1pfmetPhotonEnDown,wgt);
       effTH1Map["met_unc_UnclEnUp"]->Fill(t1pfmetUnclusteredEnUp,wgt);
       effTH1Map["met_unc_UnclEnDown"]->Fill(t1pfmetUnclusteredEnDown,wgt);

       //--------> Vertex studies plots
       effTH1Map["vtx_eff_met_d"]->Fill(t1pfmetCorr);
       Double_t vtxBDT_Zdiff = TMath::Abs(genVtxZ-vtxZ); // currently vtxZ=vtx0Z (FIXME analyzer)
       Double_t vtx0_Zdiff   = TMath::Abs(genVtxZ-vtx0Z);
       Bool_t goodVtxB = ( vtxBDT_Zdiff < 1.0 )? true:false;
       Bool_t goodVtx0 = ( vtx0_Zdiff < 1.0   )? true:false;
       if ( goodVtxB ) effTH1Map["vtxB_eff_met_n"]->Fill(t1pfmetCorr); 
       if ( goodVtx0 ) effTH1Map["vtx0_eff_met_n"]->Fill(t1pfmetCorr);
    }

  }// end loop over entries!
  
  //------------------------------------------------------------------------
  // Normalize variable bin histograms 
  //------------------------------------------------------------------------
  if (Config::doStandard){
    for (TH1MapIter iter = standardTH1Map.begin(); iter != standardTH1Map.end(); iter++){
      TString hname = (*iter).first;
      if (hname.Contains("_var",TString::kExact)){
        for (UInt_t bin = 1; bin < (*iter).second->GetSize()-1; bin++){
          Float_t bincontent = (*iter).second->GetBinContent(bin);
          Float_t binwidth   = (*iter).second->GetBinWidth(bin);
          Float_t binerr     = (*iter).second->GetBinError(bin);
          Float_t newbinerr  = binerr/binwidth;
          (*iter).second->SetBinContent(bin,bincontent/binwidth);
          (*iter).second->SetBinError(bin,newbinerr);
        }
      }
    }
  }

  //------------------------------------------------------------------------
  // Save the histograms 
  //------------------------------------------------------------------------
  if (Config::doStandard) Analysis::OutputStandardPlots();
  if (Config::doNminus1)  Analysis::OutputNminus1Plots();
  if (Config::doEffPlots) Analysis::OutputEffPlots();

}// end Analysis::DoPlots

//void Analysis::SetupEfficiencyPlots()
//{
//  effTH1Map["eff_sel"]                  = Analysis::MakeTH1Plot(standardSubDirMap,"eff","eff_sel","",10,0.,10,"","");
//
//}// end Analysis:SetupEfficienyPlots

void Analysis::SetupStandardPlots()
{

  //------------------------------------------------------------------------
  // Photon or general event variables 
  //------------------------------------------------------------------------
  Float_t METbins[] = {0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,200,250,300};
  Int_t numbins = sizeof(METbins)/sizeof(Float_t) -1;
  standardTH1Map["nvtx"]		= Analysis::MakeTH1Plot(standardSubDirMap,"norm","nvtx","",50,0.,50.,"nVertices","");
  standardTH1Map["mgg"]			= Analysis::MakeTH1Plot(standardSubDirMap,"norm","mgg","",38,105.,181.,"m_{#gamma#gamma} [GeV]","");
  standardTH1Map["mgg_loMET"]		= Analysis::MakeTH1Plot(standardSubDirMap,"norm","mgg_loMET","",38,105.,181.,"m_{#gamma#gamma} [GeV]","");
  standardTH1Map["mgg_hiMET"]		= Analysis::MakeTH1Plot(standardSubDirMap,"norm","mgg_hiMET","",38,105.,181.,"m_{#gamma#gamma} [GeV]","");
  standardTH1Map["ptgg"]		= Analysis::MakeTH1Plot(standardSubDirMap,"norm","ptgg","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");
  standardTH1Map["phi1"]		= Analysis::MakeTH1Plot(standardSubDirMap,"norm","phi1","",20,-4.,4.,"#phi(#gamma1)","");
  standardTH1Map["phi2"]		= Analysis::MakeTH1Plot(standardSubDirMap,"norm","phi2","",20,-4.,4.,"#phi(#gamma2)","");
  standardTH1Map["eta1"]		= Analysis::MakeTH1Plot(standardSubDirMap,"norm","eta1","",20,-3.,3.,"#eta(#gamma1)","");
  standardTH1Map["eta2"]		= Analysis::MakeTH1Plot(standardSubDirMap,"norm","eta2","",20,-3.,3.,"#eta(#gamma2)","");
  standardTH1Map["pt1"]			= Analysis::MakeTH1Plot(standardSubDirMap,"norm","pt1","",60,0.,600.,"p_{T,#gamma1} [GeV]","");
  standardTH1Map["pt2"]			= Analysis::MakeTH1Plot(standardSubDirMap,"norm","pt2","",60,0.,600.,"p_{T,#gamma1} [GeV]","");
  standardTH1Map["t1pfmet"]		= Analysis::MakeTH1Plot(standardSubDirMap,"norm","t1pfmet","",70,0.,350.,"p_{T}^{miss} [GeV]","");
  standardTH1Map["t1pfmetCorr"]		= Analysis::MakeTH1Plot(standardSubDirMap,"norm","t1pfmetCorr","",70,0.,350.,"p_{T}^{miss} [GeV]","");
  standardTH1Map["t1pfmetCorr_var"]	= Analysis::MakeVarPlot(standardSubDirMap,"norm","t1pfmetCorr_var","",numbins,METbins,"p_{T}^{miss} [GeV]","");
  standardTH1Map["nElec"]		= Analysis::MakeTH1Plot(standardSubDirMap,"norm","nElec","",10,0.,10.,"nElec","");
  standardTH1Map["nMuon"]		= Analysis::MakeTH1Plot(standardSubDirMap,"norm","nMuon","",10,0.,10.,"nMuon","");

  //------------------------------------------------------------------------
  // Jet info plots 
  //------------------------------------------------------------------------
  standardTH1Map["nJets"]		= Analysis::MakeTH1Plot(standardSubDirMap,"jet","nJets","",10,0.,10.,"nJets","");
  standardTH1Map["jetInfo_CHfrac1"]	= Analysis::MakeTH1Plot(standardSubDirMap,"jet","jetInfo_CHfrac1","",20,0,1.,"CH frac","");
  standardTH1Map["jetInfo_NHfrac1"]	= Analysis::MakeTH1Plot(standardSubDirMap,"jet","jetInfo_NHfrac1","",20,0,1.,"NH frac","");
  standardTH1Map["jetInfo_pt1"]		= Analysis::MakeTH1Plot(standardSubDirMap,"jet","jetInfo_pt1","",40,0,200,"p_{T}","");
  standardTH1Map["jetInfo_eta1"]	= Analysis::MakeTH1Plot(standardSubDirMap,"jet","jetInfo_eta1","",40,-5.,5.,"#eta","");
  standardTH1Map["jetInfo_phi1"]	= Analysis::MakeTH1Plot(standardSubDirMap,"jet","jetInfo_phi1","",20,-4.,4.,"#phi","");
  standardTH1Map["jetInfo_mass1"]	= Analysis::MakeTH1Plot(standardSubDirMap,"jet","jetInfo_mass1","",50,0,100,"mass [GeV]","");

}// end Analysis::SetupStandardPlots

void Analysis::SetupNminus1Plots()
{

  //------------------------------------------------------------------------
  // Plots before kinematic selection 
  //------------------------------------------------------------------------
  nminus1TH1Map["n1_nvtx"]		= Analysis::MakeTH1Plot(nminus1SubDirMap,"nmin1","n1_nvtx","",50,0.,50.,"nVertices","");
  nminus1TH1Map["n1_mgg"]		= Analysis::MakeTH1Plot(nminus1SubDirMap,"nmin1","n1_mgg","",41,99.,181.,"m_{#gamma#gamma} [GeV]","");
  nminus1TH1Map["n1_ptgg"]		= Analysis::MakeTH1Plot(nminus1SubDirMap,"nmin1","n1_ptgg","",60,0.,600.,"p_{T,#gamma#gamma} [GeV]","");
  nminus1TH1Map["n1_pt1"]		= Analysis::MakeTH1Plot(nminus1SubDirMap,"nmin1","n1_pt1","",60,0.,600.,"p_{T,#gamma1} [GeV]","");
  nminus1TH1Map["n1_pt2"]		= Analysis::MakeTH1Plot(nminus1SubDirMap,"nmin1","n1_pt2","",60,0.,600.,"p_{T,#gamma1} [GeV]","");
  nminus1TH1Map["n1_met"]		= Analysis::MakeTH1Plot(nminus1SubDirMap,"nmin1","n1_t1pfmet","",70,0.,350.,"p_{T}^{miss} [GeV]","");

}// end Analysis::SetupNminus1Plots

void Analysis::SetupEffPlots()
{

  //------------------------------------------------------------------------
  // Vtx efficiency plots 
  //------------------------------------------------------------------------
  effTH1Map["vtx0_eff_met_n"]		= Analysis::MakeTH1Plot(effSubDirMap,"eff","vtx0_eff_met_n","",200,0.,1000.,"p_{T}^{miss} [GeV]", "Vtx Efficiency");
  effTH1Map["vtxB_eff_met_n"]		= Analysis::MakeTH1Plot(effSubDirMap,"eff","vtxB_eff_met_n","",200,0.,1000.,"p_{T}^{miss} [GeV]", "Vtx Efficiency");
  effTH1Map["vtx_eff_met_d"]		= Analysis::MakeTH1Plot(effSubDirMap,"eff","vtx_eff_met_d","",200,0.,1000.,"p_{T}^{miss} [GeV]", "Vtx Efficiency");

  //------------------------------------------------------------------------
  // MET uncertainty plots 
  //------------------------------------------------------------------------
  effTH1Map["met_unc"]			= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc","",            70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_JetEnUp"]		= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_JetEnUp","",    70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_JetEnDown"]	= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_JetEnDown","",  70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_JetResUp"]		= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_JetResUp","",   70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_JetResDown"]	= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_JetResDown","", 70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_MuonEnUp"]		= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_MuonEnUp","",   70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_MuonEnDown"]	= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_MuonEnDown","", 70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_ElEnUp"]		= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_ElEnUp","",     70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_ElEnDown"]		= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_ElEnDown","",   70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_TauEnUp"]		= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_TauEnUp","",    70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_TauEnDown"]	= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_TauEnDown","",  70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_PhoEnUp"]		= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_PhoEnUp","",    70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_PhoEnDown"]	= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_PhoEnDown","",  70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_UnclEnUp"]		= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_UnclEnUp","",   70,0.,350.,"p_{T}^{miss} [GeV]","");
  effTH1Map["met_unc_UnclEnDown"]	= Analysis::MakeTH1Plot(effSubDirMap,"eff","met_unc_UnclEnDown","", 70,0.,350.,"p_{T}^{miss} [GeV]","");

}// end Analysis::SetupEffPlots

void Analysis::OutputStandardPlots()
{
  //------------------------------------------------------------------------
  // Save and delete plots 
  //------------------------------------------------------------------------
   MakeSubDirs(standardSubDirMap,fOutDir);
   Analysis::SaveTH1s(standardTH1Map,standardSubDirMap);
   Analysis::DeleteTH1s(standardTH1Map);

}// end Analysis::OutputStandardPlots

void Analysis::OutputNminus1Plots()
{
  //------------------------------------------------------------------------
  // Save and delete plots 
  //------------------------------------------------------------------------
   MakeSubDirs(nminus1SubDirMap,fOutDir);
   Analysis::SaveTH1s(nminus1TH1Map,nminus1SubDirMap);
   Analysis::DeleteTH1s(nminus1TH1Map);

}// end Analysis::OutputStandardPlots

void Analysis::OutputEffPlots()
{
  //------------------------------------------------------------------------
  // Save and delete plots 
  //------------------------------------------------------------------------
   MakeSubDirs(effSubDirMap,fOutDir);
   Analysis::SaveTH1s(effTH1Map,effSubDirMap);
   Analysis::DeleteTH1s(effTH1Map);

}// end Analysis::OutputEffPlots

void Analysis::SaveTH1s(TH1Map & th1map, TStrMap & subdirmap)
{
  //------------------------------------------------------------------------
  // Save plots 
  //------------------------------------------------------------------------
  outFile->cd();
  for (TH1MapIter mapiter = th1map.begin(); mapiter != th1map.end(); ++mapiter) 
  { 
    // save to output file
    mapiter->second->Write(mapiter->second->GetName(),TObject::kWriteDelete); // map is map["hist name",TH1F*]
 
    // draw to canvas to save as png 
    TCanvas *canv = new TCanvas("canv","canv");
    canv->cd();
    mapiter->second->Draw(isData? "PE" : "HIST");

    // first save as linear, then log
    canv->SetLogy(0);
    CMSLumi(canv);
    canv->SaveAs(Form("%s/%s/lin/%s.%s",fOutDir.Data(),subdirmap[mapiter->first].Data(),mapiter->first.Data(),Config::outtype.Data()));

    canv->SetLogy(1);
    CMSLumi(canv);
    canv->SaveAs(Form("%s/%s/log/%s.%s",fOutDir.Data(),subdirmap[mapiter->first].Data(),mapiter->first.Data(),Config::outtype.Data()));

    delete canv;
  } 
 
}// end Analysis::SaveTH1s

void Analysis::DeleteTH1s(TH1Map & th1map)
{
  //------------------------------------------------------------------------
  // Delete TH1s 
  //------------------------------------------------------------------------
  for (TH1MapIter mapiter = th1map.begin(); mapiter != th1map.end(); ++mapiter) 
  { 
    delete (mapiter->second);
  }
  th1map.clear();
}

void Analysis::DeleteTH2s(TH2Map & th2map) 
{
  //------------------------------------------------------------------------
  // Delete TH2s 
  //------------------------------------------------------------------------
  for (TH2MapIter mapiter = th2map.begin(); mapiter != th2map.end(); ++mapiter) 
  { 
    delete (mapiter->second);
  }
  th2map.clear();
}

TH1F* Analysis::MakeVarPlot(TStrMap& subdirmap, TString subdir, TString hname, TString htitle, Int_t nbins, Float_t bins[], TString xtitle, TString ytitle){
  //------------------------------------------------------------------------
  // Setup Variable binned TH1s  
  //------------------------------------------------------------------------
  TString ytitleNew;
  if (ytitle=="") ytitleNew = Form("Events / GeV");
  else ytitleNew = ytitle;

  TH1F * hist = new TH1F(hname.Data(),htitle.Data(),nbins,bins);
  hist->GetXaxis()->SetTitle(xtitle.Data());
  hist->GetYaxis()->SetTitle(ytitleNew.Data());
  hist->Sumw2();
  gStyle->SetOptStat(1111111); 

  //------------------------------------------------------------------------
  // Set the subdir  
  //------------------------------------------------------------------------
  subdirmap[hname] = subdir;

  return hist;
}

TH1F* Analysis::MakeTH1Plot(TStrMap& subdirmap, TString subdir, TString hname, TString htitle, Int_t nbins, Double_t xlow, Double_t xhigh, TString xtitle, TString ytitle){
  //------------------------------------------------------------------------
  // Setup TH1s  
  //------------------------------------------------------------------------
  TString ytitleNew;
  Float_t binwidth = (xhigh-xlow)/nbins;
  if (ytitle=="") ytitleNew = Form("Events / %2.1f GeV",binwidth);
  else ytitleNew = ytitle;

  TH1F * hist = new TH1F(hname.Data(),htitle.Data(),nbins,xlow,xhigh);
  hist->GetXaxis()->SetTitle(xtitle.Data());
  hist->GetYaxis()->SetTitle(ytitleNew.Data());
  hist->Sumw2();
  gStyle->SetOptStat(1111111);

  //------------------------------------------------------------------------
  // Set the subdir  
  //------------------------------------------------------------------------
  subdirmap[hname] = subdir;

  return hist;
}// end Analysis::MakeTH1Plot

void Analysis::SetBranchAddresses()
{
  //------------------------------------------------------------------------
  // Set branch addresses and branch pointers  
  //------------------------------------------------------------------------
  tpho->SetBranchAddress("hltPhoton26Photon16Mass60", &hltPhoton26Photon16Mass60, &b_hltPhoton26Photon16Mass60);
  tpho->SetBranchAddress("hltPhoton36Photon22Mass15", &hltPhoton36Photon22Mass15, &b_hltPhoton36Photon22Mass15);
  tpho->SetBranchAddress("hltPhoton42Photon25Mass15", &hltPhoton42Photon25Mass15, &b_hltPhoton42Photon25Mass15);
  tpho->SetBranchAddress("hltDiphoton30Mass95", &hltDiphoton30Mass95, &b_hltDiphoton30Mass95);
  tpho->SetBranchAddress("hltDiphoton30Mass70", &hltDiphoton30Mass70, &b_hltDiphoton30Mass70);
  tpho->SetBranchAddress("hltDiphoton30Mass55", &hltDiphoton30Mass55, &b_hltDiphoton30Mass55);
  tpho->SetBranchAddress("hltDiphoton30Mass55PV", &hltDiphoton30Mass55PV, &b_hltDiphoton30Mass55PV);
  tpho->SetBranchAddress("hltDiphoton30Mass55EB", &hltDiphoton30Mass55EB, &b_hltDiphoton30Mass55EB);
  tpho->SetBranchAddress("run", &run, &b_run);
  tpho->SetBranchAddress("event", &event, &b_event);
  tpho->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
  tpho->SetBranchAddress("rho", &rho, &b_rho);
  tpho->SetBranchAddress("sampleID", &sampleID, &b_sampleID);
  tpho->SetBranchAddress("totXsec", &totXsec, &b_totXsec);
  tpho->SetBranchAddress("pu_weight", &pu_weight, &b_pu_weight);
  tpho->SetBranchAddress("pu_n", &pu_n, &b_pu_n);
  tpho->SetBranchAddress("sumDataset", &sumDataset, &b_sumDataset);
  tpho->SetBranchAddress("perEveW", &perEveW, &b_perEveW);
  tpho->SetBranchAddress("pfmet", &pfmet, &b_pfmet);
  tpho->SetBranchAddress("pfmetPhi", &pfmetPhi, &b_pfmetPhi);
  tpho->SetBranchAddress("pfmetSumEt", &pfmetSumEt, &b_pfmetSumEt);
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
  tpho->SetBranchAddress("t1pfmetCorr", &t1pfmetCorr, &b_t1pfmetCorr);
  tpho->SetBranchAddress("t1pfmetCorrPhi", &t1pfmetCorrPhi, &b_t1pfmetCorrPhi);
  tpho->SetBranchAddress("t1pfmetCorrPx", &t1pfmetCorrPx, &b_t1pfmetCorrPx);
  tpho->SetBranchAddress("t1pfmetCorrPy", &t1pfmetCorrPy, &b_t1pfmetCorrPy);
  tpho->SetBranchAddress("t1pfmetPhi", &t1pfmetPhi, &b_t1pfmetPhi);
  tpho->SetBranchAddress("t1pfmetSumEt", &t1pfmetSumEt, &b_t1pfmetSumEt);
  tpho->SetBranchAddress("calomet", &calomet, &b_calomet);
  tpho->SetBranchAddress("calometPhi", &calometPhi, &b_calometPhi);
  tpho->SetBranchAddress("calometSumEt", &calometSumEt, &b_calometSumEt);
  tpho->SetBranchAddress("ptgg", &ptgg, &b_ptgg);
  tpho->SetBranchAddress("mgg", &mgg, &b_mgg);
  tpho->SetBranchAddress("seedEnergy1", &seedEnergy1, &b_seedEnergy1);
  tpho->SetBranchAddress("seedEnergy2", &seedEnergy2, &b_seedEnergy2);
  tpho->SetBranchAddress("eventClass", &eventClass, &b_eventClass);
  tpho->SetBranchAddress("pt1", &pt1, &b_pt1);
  tpho->SetBranchAddress("ptUncorr1", &ptUncorr1, &b_ptUncorr1);
  tpho->SetBranchAddress("ptOverM1", &ptOverM1, &b_ptOverM1);
  tpho->SetBranchAddress("eta1", &eta1, &b_eta1);
  tpho->SetBranchAddress("phi1", &phi1, &b_phi1);
  tpho->SetBranchAddress("sceta1", &sceta1, &b_sceta1);
  tpho->SetBranchAddress("r91", &r91, &b_r91);
  tpho->SetBranchAddress("sieie1", &sieie1, &b_sieie1);
  tpho->SetBranchAddress("hoe1", &hoe1, &b_hoe1);
  tpho->SetBranchAddress("scRawEne1", &scRawEne1, &b_scRawEne1);
  tpho->SetBranchAddress("chiso1", &chiso1, &b_chiso1);
  tpho->SetBranchAddress("phoiso1", &phoiso1, &b_phoiso1);
  tpho->SetBranchAddress("neuiso1", &neuiso1, &b_neuiso1);
  tpho->SetBranchAddress("eleveto1", &eleveto1, &b_eleveto1);
  tpho->SetBranchAddress("pt2", &pt2, &b_pt2);
  tpho->SetBranchAddress("ptUncorr2", &ptUncorr2, &b_ptUncorr2);
  tpho->SetBranchAddress("ptOverM2", &ptOverM2, &b_ptOverM2);
  tpho->SetBranchAddress("eta2", &eta2, &b_eta2);
  tpho->SetBranchAddress("phi2", &phi2, &b_phi2);
  tpho->SetBranchAddress("sceta2", &sceta2, &b_sceta2);
  tpho->SetBranchAddress("r92", &r92, &b_r92);
  tpho->SetBranchAddress("sieie2", &sieie2, &b_sieie2);
  tpho->SetBranchAddress("hoe2", &hoe2, &b_hoe2);
  tpho->SetBranchAddress("scRawEne2", &scRawEne2, &b_scRawEne2);
  tpho->SetBranchAddress("chiso2", &chiso2, &b_chiso2);
  tpho->SetBranchAddress("phoiso2", &phoiso2, &b_phoiso2);
  tpho->SetBranchAddress("neuiso2", &neuiso2, &b_neuiso2);
  tpho->SetBranchAddress("eleveto2", &eleveto2, &b_eleveto2);
  tpho->SetBranchAddress("presel1", &presel1, &b_presel1);
  tpho->SetBranchAddress("presel2", &presel2, &b_presel2);
  tpho->SetBranchAddress("sel1", &sel1, &b_sel1);
  tpho->SetBranchAddress("sel2", &sel2, &b_sel2);
  tpho->SetBranchAddress("tightsel1", &tightsel1, &b_tightsel1);
  tpho->SetBranchAddress("tightsel2", &tightsel2, &b_tightsel2);
  tpho->SetBranchAddress("loosesel1", &loosesel1, &b_loosesel1);
  tpho->SetBranchAddress("loosesel2", &loosesel2, &b_loosesel2);
  tpho->SetBranchAddress("genmatch1", &genmatch1, &b_genmatch1);
  tpho->SetBranchAddress("genmatch2", &genmatch2, &b_genmatch12);
  tpho->SetBranchAddress("genmgg", &genmgg, &b_genmgg);
  tpho->SetBranchAddress("geniso1", &geniso1, &b_geniso1);
  tpho->SetBranchAddress("geniso2", &geniso2, &b_geniso2);
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
  tpho->SetBranchAddress("vtxIndex", &vtxIndex, &b_vtxIndex);
  tpho->SetBranchAddress("vtxX", &vtxX, &b_vtxX);
  tpho->SetBranchAddress("vtxY", &vtxY, &b_vtxY);
  tpho->SetBranchAddress("vtxZ", &vtxZ, &b_vtxZ);
  tpho->SetBranchAddress("vtx0Z", &vtx0Z, &b_vtx0Z);
  tpho->SetBranchAddress("higgsVtxX", &higgsVtxX, &b_higgsVtxX);
  tpho->SetBranchAddress("higgsVtxY", &higgsVtxY, &b_higgsVtxY);
  tpho->SetBranchAddress("higgsVtxZ", &higgsVtxZ, &b_higgsVtxZ);
  tpho->SetBranchAddress("genVtxX", &genVtxX, &b_genVtxX);
  tpho->SetBranchAddress("genVtxY", &genVtxY, &b_genVtxY);
  tpho->SetBranchAddress("genVtxZ", &genVtxZ, &b_genVtxZ);
  tpho->SetBranchAddress("passCHiso1", &passCHiso1, &b_passCHiso1);
  tpho->SetBranchAddress("passCHiso2", &passCHiso2, &b_passCHiso2);
  tpho->SetBranchAddress("passNHiso1", &passNHiso1, &b_passNHiso1);
  tpho->SetBranchAddress("passNHiso2", &passNHiso2, &b_passNHiso2);
  tpho->SetBranchAddress("passPHiso1", &passPHiso1, &b_passPHiso1);
  tpho->SetBranchAddress("passPHiso2", &passPHiso2, &b_passPHiso2);
  tpho->SetBranchAddress("passSieie1", &passSieie1, &b_passSieie1);
  tpho->SetBranchAddress("passSieie2", &passSieie2, &b_passSieie2);
  tpho->SetBranchAddress("passHoe1", &passHoe1, &b_passHoe1);
  tpho->SetBranchAddress("passHoe2", &passHoe2, &b_passHoe2);
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
  tpho->SetBranchAddress("nEleMed", &nEleMed, &b_nEleMed);
  tpho->SetBranchAddress("nMuTight", &nMuTight, &b_nMuTight);
  tpho->SetBranchAddress("nEle", &nEle, &b_nEle);
  tpho->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
  tpho->SetBranchAddress("nJets", &nJets, &b_nJets);
  tpho->SetBranchAddress("nJets20", &nJets20, &b_nJets20);
  tpho->SetBranchAddress("nJets30", &nJets30, &b_nJets30);
  tpho->SetBranchAddress("nJets40", &nJets40, &b_nJets40);
  tpho->SetBranchAddress("nJets50", &nJets50, &b_nJets50);
  tpho->SetBranchAddress("nLooseBjets", &nLooseBjets, &b_nLooseBjets);
  tpho->SetBranchAddress("nMediumBjets", &nMediumBjets, &b_nMediumBjets);
  tpho->SetBranchAddress("vhtruth", &vhtruth, &b_vhtruth);
  tpho->SetBranchAddress("metF_GV", &metF_GV, &b_metF_GV);
  tpho->SetBranchAddress("metF_EcalDeadCell", &metF_EcalDeadCell, &b_metF_EcalDeadCell);
  tpho->SetBranchAddress("metF_HBHENoise", &metF_HBHENoise, &b_metF_HBHENoise);
  tpho->SetBranchAddress("metF_HBHENoiseIso", &metF_HBHENoiseIso, &b_metF_HBHENoiseIso);
  tpho->SetBranchAddress("metF_CSC", &metF_CSC, &b_metF_CSC);
  tpho->SetBranchAddress("metF_eeBadSC", &metF_eeBadSC, &b_metF_eeBadSC);
  tpho->SetBranchAddress("metF_HadronTrackRes", &metF_HadronTrackRes, &b_metF_HadronTrackRes);
  tpho->SetBranchAddress("metF_MuonBadTrack", &metF_MuonBadTrack, &b_metF_MuonBadTrack);
  tpho->SetBranchAddress("metF_badMuon", &metF_badMuon, &b_metF_badMuon);
  tpho->SetBranchAddress("metF_badChargedHadron", &metF_badChargedHadron, &b_metF_badChargedHadron);
  tpho->SetBranchAddress("metF_globalTightHalo", &metF_globalTightHalo, &b_metF_globalTightHalo);
  tpho->SetBranchAddress("massCorrSmear", &massCorrSmear, &b_massCorrSmear);
  tpho->SetBranchAddress("massCorrSmearUp", &massCorrSmearUp, &b_massCorrSmearUp);
  tpho->SetBranchAddress("massCorrSmearDown", &massCorrSmearDown, &b_massCorrSmearDown);
  tpho->SetBranchAddress("massCorrScale", &massCorrScale, &b_massCorrScale);
  tpho->SetBranchAddress("massCorrScaleUp", &massCorrScaleUp, &b_massCorrScaleUp);
  tpho->SetBranchAddress("massCorrScaleDown", &massCorrScaleDown, &b_massCorrScaleDown);
  tpho->SetBranchAddress("massRaw", &massRaw, &b_massRaw);
  tpho->SetBranchAddress("massOrig", &massOrig, &b_massOrig);
  tpho->SetBranchAddress("ptggOrig", &ptggOrig, &b_ptggOrig);
  tpho->SetBranchAddress("genZ", &genZ, &b_genZ);
  tpho->SetBranchAddress("ptZ", &ptZ, &b_ptZ);
  tpho->SetBranchAddress("etaZ", &etaZ, &b_etaZ);
  tpho->SetBranchAddress("phiZ", &phiZ, &b_phiZ);
  tpho->SetBranchAddress("mva1", &mva1, &b_mva1);
  tpho->SetBranchAddress("mva2", &mva2, &b_mva2);
  tpho->SetBranchAddress("BDTptgg", &BDTptgg, &b_BDTptgg);
  tpho->SetBranchAddress("BDTmassRaw", &BDTmassRaw, &b_BDTmassRaw);
  tpho->SetBranchAddress("BDTr91", &BDTr91, &b_BDTr91);
  tpho->SetBranchAddress("BDTr92", &BDTr92, &b_BDTr92);
  tpho->SetBranchAddress("BDTvtxZ", &BDTvtxZ, &b_BDTvtxZ);
  tpho->SetBranchAddress("BDTindex", &BDTindex, &b_BDTindex);
  tpho->SetBranchAddress("Vtx0index", &Vtx0index, &b_Vtx0index);
  tpho->SetBranchAddress("weight", &weight, &b_weight);
  tpho->SetBranchAddress("SF1", &SF1, &b_SF1);
  tpho->SetBranchAddress("SF2", &SF2, &b_SF2);

}

