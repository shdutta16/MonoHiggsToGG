#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TLegend.h>
#include <TMath.h>
#include "../../../DataFormats/Math/interface/deltaPhi.h"
#include "Style.hh"
#include "Style.cpp"
#include <iostream>


using namespace std;

void doEff( TString inDir, TString outDir, TString fType, Double_t lumi, vector<Double_t> fMETCorr ){

  cout << " Computing Efficiency for Samples in " << inDir << endl;

  vector< TString > sigNames;
  //sigNames.push_back("Bkg_MC");
  //sigNames.push_back("BaryonicZp_mZP10_mChi1");
  //sigNames.push_back("BaryonicZp_mZP10_mChi10");
  //sigNames.push_back("BaryonicZp_mZP10_mChi50");
  //sigNames.push_back("BaryonicZp_mZP10_mChi1000");
  //sigNames.push_back("BaryonicZp_mZP10000_mChi1");
  //sigNames.push_back("BaryonicZp_mZP10000_mChi10");
  //sigNames.push_back("BaryonicZp_mZP10000_mChi50");
  //sigNames.push_back("BaryonicZp_mZP10000_mChi150");
  //sigNames.push_back("BaryonicZp_mZP10000_mChi500");
  //sigNames.push_back("BaryonicZp_mZP10000_mChi1000");
  sigNames.push_back("2HDM_mZP600_mA0300");
  sigNames.push_back("2HDM_mZP800_mA0300");
  sigNames.push_back("2HDM_mZP1000_mA0300");
  sigNames.push_back("2HDM_mZP1200_mA0300");
  sigNames.push_back("2HDM_mZP1400_mA0300");
  sigNames.push_back("2HDM_mZP1700_mA0300");
  sigNames.push_back("2HDM_mZP2500_mA0300");
  UInt_t fNSig = sigNames.size();

  //vector<Double_t> mass = {1,10,50,1000};
  //vector<Double_t> mass = {1,10,50,150,500,1000};
   vector<Double_t> mass = {600,800,1000,1200,1400,1700,2500};//{1,10,100,1000}; 
  //vector<Double_t> mass = {100};

  vector< TFile *> inFile;
  inFile.resize(fNSig);

  vector< Double_t > original;
  original.resize(fNSig);

  // setup eff vectors
  vector< vector< Double_t > > eff1;
  vector< vector< Double_t > > eff2;
  vector< vector< Double_t > > eff1err;
  vector< vector< Double_t > > eff2err;
  eff1.resize(fNSig);
  eff2.resize(fNSig);
  eff1err.resize(fNSig);
  eff2err.resize(fNSig);
  for (UInt_t mc=0; mc<fNSig; mc++){
    eff1[mc].resize(4);
    eff2[mc].resize(4);
    eff1err[mc].resize(4);
    eff2err[mc].resize(4);
  }


  for (UInt_t mc=0; mc<fNSig; mc++){
    inFile[mc] = TFile::Open(inDir+sigNames[mc]+".root");
    if ( inFile[mc] ) inFile[mc]->cd();
    else{
      cout << "File " << inDir << sigNames[mc] << ".root does not exist!" << endl;
      return;
    }

    cout << " Working on " << sigNames[mc] << endl; 

    // 1=trigger, 2=presel, 3=selection, 4=pt1>30,pt2>20, 5=pt1>mgg/3,pt2>mgg/4, 6=goodVtx, 7=mgg, 8=met
    TH1D * fSel_unwgt = (TH1D*)inFile[mc]->Get("h_selection_unwgt");
    original[mc] = fSel_unwgt->GetBinContent(1);
    //original[mc] = 432325000000;
    cout << "orig = " << original[mc] << endl;

    TTree * tpho = (TTree*)inFile[mc]->Get("DiPhotonTree");

    // variables from tree
    Float_t	mgg;
    Float_t	ptgg;
    Float_t	pt1; 
    Float_t	pt2; 
    Float_t	phi1;
    Float_t	phi2;
    Float_t	eta1;
    Float_t	eta2;
    Float_t	t1pfmetCorr;
    Float_t	t1pfmetCorrPhi;
    Float_t	t1pfmet;
    Float_t	t1pfmetPhi;
    Float_t	t1pfmetSumEt;
    Int_t	nJets;
    Int_t	nMuons;
    Int_t	nEle;
    Float_t	ptJetLead;
    Float_t	etaJetLead;
    Float_t	phiJetLead;
    Float_t	massJetLead;
    Float_t	ptJetSubLead;
    Float_t	etaJetSubLead;
    Float_t	phiJetSubLead;
    Float_t	massJetSubLead;
    Float_t	ptJet3;
    Float_t	etaJet3;
    Float_t	phiJet3;
    Float_t	massJet3;
    Float_t	ptJet4;
    Float_t	etaJet4;
    Float_t	phiJet4;
    Float_t	massJet4;
 
    // branches from tree
    TBranch	*b_mgg;
    TBranch	*b_ptgg;
    TBranch	*b_pt1;
    TBranch	*b_pt2;
    TBranch	*b_phi1;
    TBranch	*b_phi2;
    TBranch	*b_eta1;
    TBranch	*b_eta2;
    TBranch	*b_t1pfmetCorr;
    TBranch	*b_t1pfmetCorrPhi;
    TBranch	*b_t1pfmet;
    TBranch	*b_t1pfmetPhi;
    TBranch	*b_t1pfmetSumEt;
    TBranch	*b_nJets;
    TBranch	*b_nMuons;
    TBranch	*b_nEle;
    TBranch	*b_ptJetLead;   //!
    TBranch	*b_etaJetLead;   //!
    TBranch	*b_phiJetLead;   //!
    TBranch	*b_massJetLead;   //!
    TBranch	*b_ptJetSubLead;   //!
    TBranch	*b_etaJetSubLead;   //!
    TBranch	*b_phiJetSubLead;   //!
    TBranch	*b_massJetSubLead;   //!
    TBranch	*b_ptJet3;   //!
    TBranch	*b_etaJet3;   //!
    TBranch	*b_phiJet3;   //!
    TBranch	*b_massJet3;   //!
    TBranch	*b_ptJet4;   //!
    TBranch	*b_etaJet4;   //!
    TBranch	*b_phiJet4;   //!
    TBranch	*b_massJet4;   //!

    // set branch addresses from tree
    tpho->SetBranchAddress("mgg", &mgg, &b_mgg);
    tpho->SetBranchAddress("ptgg", &ptgg, &b_ptgg);
    tpho->SetBranchAddress("pt1", &pt1, &b_pt1);   
    tpho->SetBranchAddress("pt2", &pt2, &b_pt2);   
    tpho->SetBranchAddress("eta1", &eta1, &b_eta1);   
    tpho->SetBranchAddress("eta2", &eta2, &b_eta2);   
    tpho->SetBranchAddress("phi1", &phi1, &b_phi1);   
    tpho->SetBranchAddress("phi2", &phi2, &b_phi2);   
    tpho->SetBranchAddress("t1pfmetCorr", &t1pfmetCorr, &b_t1pfmetCorr);   
    tpho->SetBranchAddress("t1pfmetCorrPhi", &t1pfmetCorrPhi, &b_t1pfmetCorrPhi);
    tpho->SetBranchAddress("t1pfmet", &t1pfmet, &b_t1pfmet);   
    tpho->SetBranchAddress("t1pfmetPhi", &t1pfmetPhi, &b_t1pfmetPhi);
    tpho->SetBranchAddress("t1pfmetSumEt", &t1pfmetSumEt, &b_t1pfmetSumEt);   
    tpho->SetBranchAddress("nEle", &nEle, &b_nEle);
    tpho->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
    tpho->SetBranchAddress("nJets", &nJets, &b_nJets);
    tpho->SetBranchAddress("ptJetLead", &ptJetLead, &b_ptJetLead);
    tpho->SetBranchAddress("etaJetLead", &etaJetLead, &b_etaJetLead);
    tpho->SetBranchAddress("phiJetLead", &phiJetLead, &b_phiJetLead);
    tpho->SetBranchAddress("massJetLead", &massJetLead, &b_massJetLead);
    tpho->SetBranchAddress("ptJetSubLead", &ptJetSubLead, &b_ptJetSubLead);
    tpho->SetBranchAddress("etaJetSubLead", &etaJetSubLead, &b_etaJetSubLead);
    tpho->SetBranchAddress("phiJetSubLead", &phiJetSubLead, &b_phiJetSubLead);
    tpho->SetBranchAddress("massJetSubLead", &massJetSubLead, &b_massJetSubLead);
    tpho->SetBranchAddress("ptJet3", &ptJet3, &b_ptJet3);
    tpho->SetBranchAddress("etaJet3", &etaJet3, &b_etaJet3);
    tpho->SetBranchAddress("phiJet3", &phiJet3, &b_phiJet3);
    tpho->SetBranchAddress("massJet3", &massJet3, &b_massJet3);
    tpho->SetBranchAddress("ptJet4", &ptJet4, &b_ptJet4);
    tpho->SetBranchAddress("etaJet4", &etaJet4, &b_etaJet4);
    tpho->SetBranchAddress("phiJet4", &phiJet4, &b_phiJet4);
    tpho->SetBranchAddress("massJet4", &massJet4, &b_massJet4);

    // Make TLorentzVector for objects
    TLorentzVector fLorenzVecg1;
    TLorentzVector fLorenzVecg2;
    TLorentzVector fLorenzVecgg;
    TLorentzVector fLorenzVecCorrMET; 
    TLorentzVector fLorenzVecJet1;
    TLorentzVector fLorenzVecJet2;
    TLorentzVector fLorenzVecJet3;
    TLorentzVector fLorenzVecJet4;

    // counters for efficiency
    vector< Int_t > count1a;
    vector< Int_t > count1b;
    vector< Int_t > count1c;
    vector< Int_t > count1d;
    vector< Int_t > count1e;
    vector< Int_t > count2a;
    vector< Int_t > count2b;
    vector< Int_t > count2c;
    vector< Int_t > count2d;
    count1a.resize(fNSig);
    count1b.resize(fNSig);
    count1c.resize(fNSig);
    count1d.resize(fNSig);
    count1e.resize(fNSig);
    count2a.resize(fNSig);
    count2b.resize(fNSig);
    count2c.resize(fNSig);
    count2d.resize(fNSig);


    // START loop over entries in tree
    UInt_t nentries = tpho->GetEntries();
    for (UInt_t entry = 0; entry < nentries; entry++){
      tpho->GetEntry(entry);

      fLorenzVecg1.SetPtEtaPhiM(pt1,eta1,phi1,0.);
      fLorenzVecg2.SetPtEtaPhiM(pt2,eta2,phi2,0.);
      fLorenzVecgg = fLorenzVecg1 + fLorenzVecg2;
      fLorenzVecJet1.SetPtEtaPhiM(ptJetLead,etaJetLead,phiJetLead,massJetLead);
      fLorenzVecJet2.SetPtEtaPhiM(ptJetSubLead,etaJetSubLead,phiJetSubLead,massJetSubLead);
      fLorenzVecJet3.SetPtEtaPhiM(ptJet3,etaJet3,phiJet3,massJet3);
      fLorenzVecJet4.SetPtEtaPhiM(ptJet4,etaJet4,phiJet4,massJet4);

      //// t1pfmet phi Correction
      //Double_t t1pfmetCorrX = t1pfmet*cos(t1pfmetPhi) - (fMETCorr[0] + fMETCorr[1]*t1pfmetSumEt);
      //Double_t t1pfmetCorrY = t1pfmet*sin(t1pfmetPhi) - (fMETCorr[2] + fMETCorr[3]*t1pfmetSumEt);
      //Double_t t1pfmetCorrE = sqrt(t1pfmetCorrX*t1pfmetCorrX + t1pfmetCorrY*t1pfmetCorrY);
      ////std::cout << "px = t1pfmet*cos(t1pfmetPhi) - (" << fMETCorr[0] << " + " << fMETCorr[1] << "*t1pfmetSumEt)" << std::endl;
      ////std::cout << "py = t1pfmet*sin(t1pfmetPhi) - (" << fMETCorr[2] << " + " << fMETCorr[3] << "*t1pfmetSumEt)" << std::endl;
      //fLorenzVecCorrMET.SetPxPyPzE(t1pfmetCorrX,t1pfmetCorrY,0,t1pfmetCorrE);
      //Double_t t1pfmetPhiCorr = fLorenzVecCorrMET.Phi(); 
      //Double_t t1pfmetCorr = fLorenzVecCorrMET.Pt();
      Double_t t1pfmetPhiCorr = t1pfmetCorrPhi; 
      //Double_t t1pfmetCorr    = t1pfmetCorr; 
      
      // DeltaPhi between each Jet and the MET
      // set these values to true for events w/o jets
      Bool_t max_dphiJETMETpass = true;	// max dphi Jet-MET < 2.7 
      Bool_t min_dphiJETMETpass = true;	// min dphi Jet-MET > 0.5 
      
      Double_t max_dphi_JetMET = 0.;
      Double_t min_dphi_JetMET = 10.;
      
      if ( nJets > 0 ){
        Double_t dphiJet1METmin = 10;
        Double_t dphiJet2METmin = 10;
        Double_t dphiJet3METmin = 10;
        Double_t dphiJet4METmin = 10;
        Double_t dphiJet1METmax = 0;
        Double_t dphiJet2METmax = 0;
        Double_t dphiJet3METmax = 0;
        Double_t dphiJet4METmax = 0;
        if ( ptJetLead > 50 ){
          dphiJet1METmin = TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetPhiCorr));
          dphiJet1METmax = TMath::Abs(deltaPhi(fLorenzVecJet1.Phi(),t1pfmetPhiCorr));
        }
        if ( ptJetSubLead > 50 ){
          dphiJet2METmin = TMath::Abs(deltaPhi(fLorenzVecJet2.Phi(),t1pfmetPhiCorr));
          dphiJet2METmax = TMath::Abs(deltaPhi(fLorenzVecJet2.Phi(),t1pfmetPhiCorr));
        }
        if ( ptJet3 > 50 ){
          dphiJet3METmin = TMath::Abs(deltaPhi(fLorenzVecJet3.Phi(),t1pfmetPhiCorr));
          dphiJet3METmax = TMath::Abs(deltaPhi(fLorenzVecJet3.Phi(),t1pfmetPhiCorr));
        }
        if ( ptJet4 > 50 ){
          dphiJet4METmin = TMath::Abs(deltaPhi(fLorenzVecJet4.Phi(),t1pfmetPhiCorr));
          dphiJet4METmax = TMath::Abs(deltaPhi(fLorenzVecJet4.Phi(),t1pfmetPhiCorr));
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
      
      if (max_dphi_JetMET > 2.7) max_dphiJETMETpass = false;// max dphi Jet-MET < 2.7 
      if (min_dphi_JetMET < 0.5) min_dphiJETMETpass = false;// min dphi Jet-MET > 0.5 
      
      // DeltaPhi between gg and MET
      Double_t dphiggMET = TMath::Abs(deltaPhi(fLorenzVecgg.Phi(),t1pfmetPhiCorr));
      Bool_t dphiggMETpass = false; // dphi gg-MET > 2.1
      if ( dphiggMET > 2.1 ) dphiggMETpass = true;


      // START applying cuts
      //if ( !dphiggMETpass || !max_dphiJETMETpass /*|| !min_dphiJETMETpass*/) continue;

      // for selection option 1 
      if ( pt1 > 0.5*mgg && pt2 > 0.25*mgg && t1pfmetCorr > 130 & ptgg > 90){
        count1a[mc]++;
        if ( dphiggMETpass ){
          count1b[mc]++;
          if ( max_dphiJETMETpass){
            count1c[mc]++;
            if ( nEle < 2 && nMuons == 0){
	      count1d[mc]++;
	      if ( nJets <= 2) count1e[mc]++;
	    }
          }
        }
      }  

      // for selection option 2 
      if ( pt1 > 0.65*mgg && pt2 > 0.25*mgg && t1pfmetCorr > 80){
        count2a[mc]++;
        if ( nEle < 2){
          count2b[mc]++;
          if ( nMuons == 0){
            count2c[mc]++;
            if ( ptgg > 50) count2d[mc]++;
          }
        }
      }  

    }// end loop over tree entries  

    cout << count1a[mc] << " " << count1b[mc] << " " << count1c[mc] << " " << count1d[mc] << endl;
    //cout << count2a[mc] << " " << count2b[mc] << " " << count2c[mc] << " " << count2d[mc] << endl;

    eff1[mc][0] = (Double_t)count1a[mc]/(Double_t)original[mc];
    eff1[mc][1] = (Double_t)count1b[mc]/(Double_t)original[mc];
    eff1[mc][2] = (Double_t)count1c[mc]/(Double_t)original[mc];
    eff1[mc][3] = (Double_t)count1d[mc]/(Double_t)original[mc];
    eff1[mc][4] = (Double_t)count1e[mc]/(Double_t)original[mc];

    eff2[mc][0] = (Double_t)count2a[mc]/(Double_t)original[mc];
    eff2[mc][1] = (Double_t)count2b[mc]/(Double_t)original[mc];
    eff2[mc][2] = (Double_t)count2c[mc]/(Double_t)original[mc];
    eff2[mc][3] = (Double_t)count2d[mc]/(Double_t)original[mc];
  
    eff1err[mc][0] = TMath::Sqrt(eff1[mc][0]*(1.0-eff1[mc][0])/(Double_t)original[mc]);
    eff1err[mc][1] = TMath::Sqrt(eff1[mc][1]*(1.0-eff1[mc][1])/(Double_t)original[mc]);
    eff1err[mc][2] = TMath::Sqrt(eff1[mc][2]*(1.0-eff1[mc][2])/(Double_t)original[mc]);
    eff1err[mc][3] = TMath::Sqrt(eff1[mc][3]*(1.0-eff1[mc][3])/(Double_t)original[mc]);
    eff1err[mc][4] = TMath::Sqrt(eff1[mc][4]*(1.0-eff1[mc][4])/(Double_t)original[mc]);

    eff2err[mc][0] = TMath::Sqrt(eff2[mc][0]*(1.0-eff2[mc][0])/(Double_t)original[mc]);
    eff2err[mc][1] = TMath::Sqrt(eff2[mc][1]*(1.0-eff2[mc][1])/(Double_t)original[mc]);
    eff2err[mc][2] = TMath::Sqrt(eff2[mc][2]*(1.0-eff2[mc][2])/(Double_t)original[mc]);
    eff2err[mc][3] = TMath::Sqrt(eff2[mc][3]*(1.0-eff2[mc][3])/(Double_t)original[mc]);
   
    //cout << "Eff1 = " << eff1[mc][0] << endl;
    //cout << "Eff2 = " << eff2[mc][0] << endl;

  }// end loop over sig files




  vector< TH1D * > eff_mDM1;  
  eff_mDM1.resize(5);
  vector< TH1D * > eff_mDM2;  
  eff_mDM2.resize(5);

  Int_t binForMass;
  for (UInt_t h = 0; h < 5; h++){ 
    //eff_mDM1[h] = new TH1D(Form("eff_mDM1_%i",h),"",3000,0.8,2000);
    //eff_mDM2[h] = new TH1D(Form("eff_mDM2_%i",h),"",300,0.8,2000);
    eff_mDM1[h] = new TH1D(Form("eff_mDM1_%i",h),"",75,500,3000);
    eff_mDM2[h] = new TH1D(Form("eff_mDM2_%i",h),"",75,500,3000);
    for (UInt_t mc = 0; mc < fNSig; mc++){
      binForMass = eff_mDM1[h]->FindBin(mass[mc]);
      eff_mDM1[h]->SetBinContent(binForMass,eff1[mc][h]);
      eff_mDM1[h]->SetBinError(binForMass,eff1err[mc][h]);
      cout << "After cut #" << h << " efficiency = " << eff1[mc][h] << " pm " << eff1err[mc][h] << endl; 

      binForMass = eff_mDM2[h]->FindBin(mass[mc]);
      eff_mDM2[h]->SetBinContent(binForMass,eff2[mc][h]);
      eff_mDM2[h]->SetBinError(binForMass,eff2err[mc][h]);
    }
    eff_mDM1[h]->SetMaximum(0.6);
    //eff_mDM1[h]->SetMaximum(0.45);
    eff_mDM1[h]->SetMinimum(0.2);
    eff_mDM2[h]->SetMaximum(0.7);
    eff_mDM2[h]->SetMinimum(0.2);
  }

  TString latexCMS = "CMS";
  TString latexHgg = "Z' #rightarrow DM + h(#gamma#gamma)";
  TString latexlumi = Form("%1.1f fb^{-1}",lumi);
  TString latexener = " (13 TeV)";
  TString latexname = latexlumi+latexener; 

  TLatex *l1 = new TLatex(0.14,0.86,latexCMS);
  l1->SetTextSize(0.036);
  l1->SetTextAlign(12);
  l1->SetNDC(kTRUE);
  l1->SetTextFont(62);
  TLatex *l2 = new TLatex(0.73,0.92,latexname);
  l2->SetTextSize(0.034);
  l2->SetTextAlign(12);
  l2->SetNDC(kTRUE);
  l2->SetTextFont(42);

  TCanvas *c = new TCanvas();
  c->cd();
  gStyle->SetOptStat(0);

  eff_mDM1[0]->GetXaxis()->SetTitle("m_{Z'} [GeV]");
  //eff_mDM1[0]->GetXaxis()->SetTitle("m_{#Chi} [GeV]");
  eff_mDM1[0]->GetYaxis()->SetTitle("Efficiency");
  eff_mDM1[0]->Draw("PE1");

  ColorMap effCol;
  effCol["0"] = kBlack;
  effCol["1"] = kBlue;
  effCol["2"] = kTeal+10;
  effCol["3"] = kMagenta;
  effCol["4"] = kRed-7;

  TLegend* ftempLegends = new TLegend(0.32,0.2,0.7,0.4); // (x1,y1,x2,y2)

  ftempLegends->AddEntry(eff_mDM1[0],"pt1,pt2,MET,ptgg","l");
  ftempLegends->AddEntry(eff_mDM1[1],"add dphi(h,MET) cut","l");
  ftempLegends->AddEntry(eff_mDM1[2],"add dphi(j,MET) cut","l");
  ftempLegends->AddEntry(eff_mDM1[3],"add #el/mu cut","l");
  ftempLegends->AddEntry(eff_mDM1[4],"add #jet cut","l");

  ftempLegends->SetBorderSize(4);
  ftempLegends->SetLineColor(kBlack);
  ftempLegends->SetTextSize(0.03);//0.035
  ftempLegends->SetLineWidth(2);
  ftempLegends->Draw("SAME");

  l1->Draw("same");
  l2->Draw("same");

  for (UInt_t h = 0; h < 5; h++){
    eff_mDM1[h]->SetLineColor(effCol[Form("%i",h)]);
    eff_mDM1[h]->Draw("PE1 SAME");
  }
  
  //CMSLumi(c,11,lumi);
  c->SetLogx();
  c->SaveAs(Form("%seff_mDM1.%s",outDir.Data(),fType.Data()));
  delete c; 


  TCanvas *c2 = new TCanvas();
  c2->cd();
  gStyle->SetOptStat(0);

  eff_mDM2[0]->GetXaxis()->SetTitle("m_{Z'} [GeV]");
  eff_mDM2[0]->GetYaxis()->SetTitle("Efficiency");

  eff_mDM2[0]->Draw("PE1");

  TLegend* ftempLegends2 = new TLegend(0.32,0.2,0.9,0.4); // (x1,y1,x2,y2)

  ftempLegends2->AddEntry(eff_mDM2[0],"pt1,pt2 + dphi","l");
  ftempLegends2->AddEntry(eff_mDM2[1],"add el veto","l");
  ftempLegends2->AddEntry(eff_mDM2[2],"add muon veto ","l");
  ftempLegends2->AddEntry(eff_mDM2[3],"add ptgg cut","l");

  ftempLegends2->SetBorderSize(4);
  ftempLegends2->SetLineColor(kBlack);
  ftempLegends2->SetTextSize(0.03);//0.035
  ftempLegends2->SetLineWidth(2);
  ftempLegends2->Draw("SAME");

  for (UInt_t h = 0; h < 4; h++){
    eff_mDM2[h]->SetLineColor(effCol[Form("%i",h)]);
    eff_mDM2[h]->Draw("PE1 SAME");
  }
  
  //CMSLumi(c2,11,lumi);
  c2->SetLogx();
  c2->SaveAs(Form("%seff_mDM2.%s",outDir.Data(),fType.Data()));
  delete c2; 
  

 
}//end doEff
