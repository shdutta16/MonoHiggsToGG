#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TH1.h>

#include <vector>
#include <iostream>

Float_t Bwgt;		
Float_t Bmgg;		
Float_t Bpt1;		
Float_t Bpt2;		
Float_t Bptgg;	
Float_t Bmet;		
Float_t Bdphigg;	
Float_t Bdphij;	
Int_t   Bnjet;	
Int_t   Bnmu;		
Int_t   Bnel;		
                      
Float_t Swgt;		
Float_t Smgg;		
Float_t Spt1;		
Float_t Spt2;		
Float_t Sptgg;	
Float_t Smet;		
Float_t Sdphigg;	
Float_t Sdphij;	
Int_t   Snjet;	
Int_t   Snmu;		
Int_t   Snel;		

void    RunSample(TString, TString, TString, std::vector<Float_t>);
Float_t OptimizeSigma(TTree*, TTree*, TString, int, double);
Float_t Optimize2DScan(TString, TString, TTree*, TTree*, TString, TString, int, int, double, double, TString, double, TString, double);

void optimization(){
  TString path = "/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/";
  TString outPath = "~/www/Plots/13TeV_v80X_moriond17/Optimization/";

  std::vector< TString > samples;
  samples.push_back("ScalarZp_mZP10_mChi1");
  samples.push_back("BaryonicZp_mZP10_mChi1");
  samples.push_back("2HDM_mZP600_mA0300");
  UInt_t nsamples = samples.size();

  std::vector< std::vector<Float_t> > cutvec;
  cutvec.resize(nsamples); 
 
  for (UInt_t mc = 0; mc < nsamples; mc++){
    RunSample(path,outPath,samples[mc],cutvec[mc]);
  }

}

void RunSample(TString path, TString outPath, TString signal, std::vector<Float_t> cutvec){
 
  // ----------------------------------------------------------------
  // get input files 
  // ----------------------------------------------------------------
  TFile *fsig = TFile::Open(Form("%s%s_rawskim.root",path.Data(),signal.Data()),"READ");
  if (fsig == NULL){ cout << "Signal file not found, exiting..." << endl; }

  std::vector<TString> BkgNames;
  BkgNames.push_back("DiPhoton");
  BkgNames.push_back("GJets");
  BkgNames.push_back("QCD");
  BkgNames.push_back("DYJetsToLL"); 
  UInt_t fNBkg = BkgNames.size();

  // ----------------------------------------------------------------
  // get input trees 
  // ----------------------------------------------------------------
  TTree *sig = (TTree*)fsig->Get("DiPhotonTree");
  if (sig == NULL){ cout << "Signal tree not found, exiting..." << endl; }
  
  TChain *bkg = new TChain();
  for (UInt_t mc = 0; mc < fNBkg; mc++){
    bkg->Add(Form("%s%s_rawskim.root/DiPhotonTree",path.Data(),BkgNames[mc].Data()));
  }

  // ----------------------------------------------------------------
  // set up branches 
  // ----------------------------------------------------------------
  bkg->SetBranchAddress("weight",&Bwgt);
  bkg->SetBranchAddress("mgg",&Bmgg);
  bkg->SetBranchAddress("pt1",&Bpt1);
  bkg->SetBranchAddress("pt2",&Bpt2);
  bkg->SetBranchAddress("ptgg",&Bptgg);
  bkg->SetBranchAddress("t1pfmetCorr",&Bmet);
  bkg->SetBranchAddress("dphiggmet",&Bdphigg);
  bkg->SetBranchAddress("mindphijmet",&Bdphij); 
  bkg->SetBranchAddress("nJets30",&Bnjet);
  bkg->SetBranchAddress("nMuons",&Bnmu);
  bkg->SetBranchAddress("nEle",&Bnel);

  sig->SetBranchAddress("weight",&Swgt);
  sig->SetBranchAddress("mgg",&Smgg);
  sig->SetBranchAddress("pt1",&Spt1);
  sig->SetBranchAddress("pt2",&Spt2);
  sig->SetBranchAddress("ptgg",&Sptgg);
  sig->SetBranchAddress("t1pfmetCorr",&Smet);
  sig->SetBranchAddress("dphiggmet",&Sdphigg);
  sig->SetBranchAddress("mindphijmet",&Sdphij); 
  sig->SetBranchAddress("nJets30",&Snjet);
  sig->SetBranchAddress("nMuons",&Snmu);
  sig->SetBranchAddress("nEle",&Snel);

  // ----------------------------------------------------------------
  // call 2D optimization scan for each variable 
  // OptimizeScan(sig, bkg, var1, var2, ncuts1, ncuts2, cutstart1, cutstart2)
  // ----------------------------------------------------------------
  Optimize2DScan(outPath, signal, sig, bkg, "pt1",  "pt2", 15, 15, 0.3, 0.25, "met",  40, "ptgg",   70);
  Optimize2DScan(outPath, signal, sig, bkg, "ptgg", "pt1", 25, 10,   0, 0.30, "met",  40, "pt2",  0.25);
  Optimize2DScan(outPath, signal, sig, bkg, "met",  "pt1", 25, 10,   0, 0.30, "ptgg", 70, "pt2",  0.25);
  Optimize2DScan(outPath, signal, sig, bkg, "ptgg", "met", 25, 25,   0,    0, "pt1", 0.3, "pt2",  0.25);
 
  // ----------------------------------------------------------------
  // call optimization for each variable 
  // ----------------------------------------------------------------
  Float_t ptggcut = 0;
  Float_t pt1cut  = 0;
  Float_t pt2cut  = 0;
  Float_t metcut  = 0;
  Float_t jetcut  = 0;
  Float_t nelcut  = 0;
  Float_t nmucut  = 0;
  Float_t phigcut = 0;
  Float_t phijcut = 0;

  //ptggcut = OptimizeSigma(sig, bkg, "ptgg", 10, 30.0); 
  //pt1cut  = OptimizeSigma(sig, bkg, "pt1",  10, 0.30);
  //pt2cut  = OptimizeSigma(sig, bkg, "pt2",  10, 0.25);
  //metcut  = OptimizeSigma(sig, bkg, "met",  10, 30.0);
  //jetcut  = OptimizeSigma(sig, bkg, "njet",  4,  0.0);
  //nelcut  = OptimizeSigma(sig, bkg, "nel",   3,  0.0);
  //nmucut  = OptimizeSigma(sig, bkg, "nmu",   3,  0.0);
  //phigcut = OptimizeSigma(sig, bkg, "phig", 10,  1.8);
  //phijcut = OptimizeSigma(sig, bkg, "phij", 10,  0.0);

  //std::cout << "--------- Optimal cuts ------------" << std::endl;
  //std::cout << "   ptgg : " << ptggcut << std::endl; 
  //std::cout << "   pt1  : " << pt1cut  << std::endl; 
  //std::cout << "   pt2  : " << pt2cut  << std::endl;
  //std::cout << "   met  : " << metcut  << std::endl;
  //std::cout << "   njet : " << jetcut  << std::endl;
  //std::cout << "   nel  : " << nelcut  << std::endl;
  //std::cout << "   mmu  : " << nmucut  << std::endl;
  //std::cout << "   phig : " << phigcut << std::endl;
  //std::cout << "   phij : " << phijcut << std::endl; 
  //std::cout << "-----------------------------------" << std::endl;

  cutvec.push_back(pt1cut);
  cutvec.push_back(pt2cut);
  cutvec.push_back(ptggcut);
  cutvec.push_back(metcut);

  delete sig;
  delete bkg;
  delete fsig; 

}

Float_t Optimize2DScan(TString outPath, TString signal, TTree* sig, TTree* bkg, TString var1, TString var2, int nstep1, int nstep2, double cut1, double cut2, TString var3, double cut3, TString var4, double cut4){

  std::cout << "Comparing " << var1 << " with " << var2 << std::endl;

  int v1 = 0;
  int v2 = 0;
  int v3 = 0;
  int v4 = 0;
  if (var1=="ptgg") v1 = 0;
  if (var1=="pt1")  v1 = 1;
  if (var1=="pt2")  v1 = 2;
  if (var1=="met")  v1 = 3;
  if (var2=="ptgg") v2 = 0;
  if (var2=="pt1")  v2 = 1;
  if (var2=="pt2")  v2 = 2;
  if (var2=="met")  v2 = 3;
  if (var3=="ptgg") v3 = 0;
  if (var3=="pt1")  v3 = 1;
  if (var3=="pt2")  v3 = 2;
  if (var3=="met")  v3 = 3;
  if (var4=="ptgg") v4 = 0;
  if (var4=="pt1")  v4 = 1;
  if (var4=="pt2")  v4 = 2;
  if (var4=="met")  v4 = 3;

  if (v1==v2) std::cout << "Variables are same! This shouldn't happen..." << std::endl;

  Float_t sigma = 0;
  Float_t width1 = 5;
  Float_t width2 = 5;
  if (v1==1 || v1==2) width1 = 0.05; 
  if (v2==1 || v2==2) width2 = 0.05;
  std::vector< Double_t > cutval1;
  std::vector< Double_t > cutval2;
  for (UInt_t i=0; i<nstep1; i++){ cutval1.push_back(cut1+i*width1); }
  for (UInt_t j=0; j<nstep2; j++){ cutval2.push_back(cut2+j*width2); }
  
  Double_t max1 = cut1+(Double_t)nstep1*width1;
  Double_t max2 = cut2+(Double_t)nstep2*width2;

  TH2F* outputhist = new TH2F("outputhist","",nstep2,cut2,max2,nstep1,cut1,max1);

  Float_t variable1 = 0; 
  Float_t variable2 = 0;
  Float_t variable3 = 0; 
  Float_t variable4 = 0; 

  Double_t startsig = 0; 
  std::vector< std::vector< Double_t > > totalbkg;
  std::vector< std::vector< Double_t > > totalsig;
  std::vector< std::vector< Double_t > > totaleff;
  std::vector< std::vector< Double_t > > totalsigma;
  totalbkg.resize(nstep1);
  totalsig.resize(nstep1);
  totaleff.resize(nstep1);
  totalsigma.resize(nstep1);
  for (UInt_t i = 0; i < nstep1; i++){
    totalbkg[i].resize(nstep2);
    totalsig[i].resize(nstep2);
    totaleff[i].resize(nstep2);
    totalsigma[i].resize(nstep2);
  }

  // ----------------------------------------------------------------
  // run over bkgs 
  // ----------------------------------------------------------------
  for (UInt_t entry = 0; entry < bkg->GetEntries(); entry++){
    bkg->GetEntry(entry);
    if (Bmet  > 130) continue;
    if (Bnjet >   2) continue;  
    if (Bnel  >   0) continue;
    if (Bnmu  >   0) continue;  
    if (Bdphigg < 2.1) continue;
    if (Bdphij  < 0.5) continue;
    
    if (Bmgg < 115 || Bmgg > 135) continue;

    if (v1==0) variable1 = Bptgg; 
    if (v1==1) variable1 = Bpt1/Bmgg;
    if (v1==2) variable1 = Bpt2/Bmgg;
    if (v1==3) variable1 = Bmet;
    if (v2==0) variable2 = Bptgg; 
    if (v2==1) variable2 = Bpt1/Bmgg;
    if (v2==2) variable2 = Bpt2/Bmgg;
    if (v2==3) variable2 = Bmet;
    if (v3==0) variable3 = Bptgg; 
    if (v3==1) variable3 = Bpt1/Bmgg;
    if (v3==2) variable3 = Bpt2/Bmgg;
    if (v3==3) variable3 = Bmet;
    if (v4==0) variable4 = Bptgg; 
    if (v4==1) variable4 = Bpt1/Bmgg;
    if (v4==2) variable4 = Bpt2/Bmgg;
    if (v4==3) variable4 = Bmet;

    if (variable1 < cut1) continue;
    if (variable2 < cut2) continue;
    if (variable3 < cut3) continue;
    if (variable4 < cut4) continue;

    for (UInt_t n = 0; n < nstep1; n++){
      for (UInt_t m = 0; m < nstep2; m++){
        if (variable1 < cutval1[n]) continue;
        if (variable2 < cutval2[m]) continue;
        totalbkg[n][m] += Bwgt;
      }
    }
  }
   
  // ----------------------------------------------------------------
  // run over sigs 
  // ----------------------------------------------------------------
  for (UInt_t entry = 0; entry < sig->GetEntries(); entry++){
    sig->GetEntry(entry);
    startsig += Swgt;
 
    if (Smet  > 130) continue;
    if (Snjet >   2) continue;
    if (Snel  >   0) continue;
    if (Snmu  >   0) continue; 
    if (Sdphigg < 2.1) continue;
    if (Sdphij  < 0.5) continue;  
 
    if (Smgg < 115 || Smgg > 135) continue;
 
    if (v1==0) variable1 = Sptgg; 
    if (v1==1) variable1 = Spt1/Smgg;
    if (v1==2) variable1 = Spt2/Smgg;
    if (v1==3) variable1 = Smet;
    if (v2==0) variable2 = Sptgg; 
    if (v2==1) variable2 = Spt1/Smgg;
    if (v2==2) variable2 = Spt2/Smgg;
    if (v2==3) variable2 = Smet;
    if (v3==0) variable3 = Sptgg; 
    if (v3==1) variable3 = Spt1/Smgg;
    if (v3==2) variable3 = Spt2/Smgg;
    if (v3==3) variable3 = Smet;
    if (v4==0) variable4 = Sptgg; 
    if (v4==1) variable4 = Spt1/Smgg;
    if (v4==2) variable4 = Spt2/Smgg;
    if (v4==3) variable4 = Smet;
 
    if (variable1 < cut1) continue;
    if (variable2 < cut2) continue;
    if (variable3 < cut3) continue;
    if (variable4 < cut4) continue;
 
    for (UInt_t n = 0; n < nstep1; n++){
      for (UInt_t m = 0; m < nstep2; m++){
        if (variable1 < cutval1[n]) continue;
        if (variable2 < cutval2[m]) continue;
        totalsig[n][m] += Swgt;
      }
    }
  }

  // ----------------------------------------------------------------
  // calculate sigma & fill histo 
  // ----------------------------------------------------------------
  for (UInt_t i=0; i < nstep1; i++){
    for (UInt_t j=0; j < nstep2; j++){
      totaleff[i][j]=totalsig[i][j]/startsig; 
      totalsigma[i][j]=totaleff[i][j]/(2.5+sqrt(totalbkg[i][j]));
      //std::cout << "cutval1: " << cutval1[i] << " cutval2: " << cutval2[j] << " --- sigma: " << totalsigma[i][j] << std::endl;
      outputhist->Fill(cutval2[j]+width2/2,cutval1[i]+width1/2,totalsigma[i][j]); 
    }
  }   

  TCanvas *c = new TCanvas("c","",600,600);
  c->cd();
  gStyle->SetOptStat(0);

  outputhist->GetXaxis()->SetTitle(Form("%s",var2.Data()));
  outputhist->GetYaxis()->SetTitle(Form("%s",var1.Data()));
  outputhist->Draw("colz");
  c->SetRightMargin(0.15);

  c->SaveAs(Form("%s%s_Opt2DScan_%s_vs_%s.png",outPath.Data(),signal.Data(),var1.Data(),var2.Data()));
  c->SaveAs(Form("%s%s_Opt2DScan_%s_vs_%s.pdf",outPath.Data(),signal.Data(),var1.Data(),var2.Data()));
 
  delete c;  
  delete outputhist;

  return sigma;
}


Float_t OptimizeSigma(TTree* sig, TTree* bkg, TString var, int nstp, double cutstart){
  Float_t cut = 0; 
  Int_t nsteps = nstp; 

  TH1F* sighiststart = new TH1F("sigstart","",38,105,181);
  std::vector< TH1F* > sighisto; 
  std::vector< TH1F* > bkghisto; 
  sighisto.resize(nsteps);
  bkghisto.resize(nsteps);
  for (UInt_t n = 0; n < nsteps; n++){
    sighisto[n] = new TH1F(Form("sig%d",n),"",38,105,181);
    bkghisto[n] = new TH1F(Form("bkg%d",n),"",38,105,181);
  }

  // ----------------------------------------------------------------
  // starting points for cuts 
  // ----------------------------------------------------------------
  Float_t cut_pt1   = 0.30;
  Float_t cut_pt2   = 0.25;
  Float_t cut_ptgg  = 30;
  Float_t cut_met   = 30;
  Float_t cut_dphi1 = 2.1;
  Float_t cut_dphi2 = 0.5;
  Int_t   cut_njet  = 2;
  Int_t   cut_nel   = 0;
  Int_t   cut_nmu   = 0;

  Float_t variable = 0;
  std::vector<Float_t> cutval;
  cutval.resize(nsteps);
  for (UInt_t n = 0; n < nsteps; n++){
    if (var=="pt1" || var=="pt2" || var=="phig" || var=="phij") cutval[n] = cutstart + 0.05*n;
    else if (var=="njet" || var=="nel" || var=="nmu")           cutval[n] = cutstart + n;
    else                                                        cutval[n] = cutstart + 5*n;  
  }

  // ----------------------------------------------------------------
  // run over bkgs 
  // ----------------------------------------------------------------
  for (UInt_t entry = 0; entry < bkg->GetEntries(); entry++){
    bkg->GetEntry(entry);

    if (var=="pt1")  variable = Bpt1/Bmgg;
    if (var=="pt2")  variable = Bpt2/Bmgg;
    if (var=="ptgg") variable = Bptgg; 
    if (var=="met")  variable = Bmet;
    if (var=="phig") variable = Bdphigg;
    if (var=="phij") variable = Bdphij;
    if (var=="njet") variable = Bnjet;
    if (var=="nel")  variable = Bnel;
    if (var=="nmu")  variable = Bnmu;
 
    if (Bpt1  < cut_pt1*Bmgg) continue;
    if (Bpt2  < cut_pt2*Bmgg) continue;
    if (Bptgg < cut_ptgg)     continue;
    if (Bmet  < cut_met)      continue;
    if (Bnel  > cut_nel)      continue;
    if (Bnmu  > cut_nmu)      continue;  
    if (Bnjet > cut_njet)     continue;  

    for (UInt_t n = 0; n < nsteps; n++){
      if (var=="njet" || var=="nel" || var=="nmu"){
        if (variable > cutval[n]) continue;
      }
      else{ 
        if (variable < cutval[n]) continue;
      }
      bkghisto[n]->Fill(Bmgg,Bwgt);
    }

  }
   
  // ----------------------------------------------------------------
  // run over sigs 
  // ----------------------------------------------------------------
  for (UInt_t entry = 0; entry < sig->GetEntries(); entry++){
    sig->GetEntry(entry);

    if (var=="pt1")  variable = Spt1/Smgg;
    if (var=="pt2")  variable = Spt2/Smgg;
    if (var=="ptgg") variable = Sptgg; 
    if (var=="met")  variable = Smet;
    if (var=="phig") variable = Sdphigg;
    if (var=="phij") variable = Sdphij;
    if (var=="njet") variable = Snjet;
    if (var=="nel")  variable = Snel;
    if (var=="nmu")  variable = Snmu;
 
    sighiststart->Fill(Smgg,Swgt);
    
    if (Spt1  < cut_pt1*Smgg) continue;
    if (Spt2  < cut_pt2*Smgg) continue;
    if (Sptgg < cut_ptgg)     continue;
    if (Smet  < cut_met)      continue;
    if (Snel  > cut_nel)      continue;
    if (Snmu  > cut_nmu)      continue; 
    if (Snjet > cut_njet)     continue; 

    for (UInt_t n = 0; n < nsteps; n++){
      if (var=="njet" || var=="nel" || var=="nmu"){
        if (variable > cutval[n]) continue;
      }
      else{ 
        if (variable < cutval[n]) continue;
      }
      sighisto[n]->Fill(Smgg,Swgt);
    }

  }

  Float_t Byield = 0;
  Float_t Berror = 0;
  Float_t Syield = 0;
  Float_t Sstart = 0;
  Float_t Seff   = 0;
  Float_t efferr = 0; 
  std::vector<Float_t> Sigma;
  Sigma.resize(nsteps);
  std::vector<Float_t> Err;
  Err.resize(nsteps);
  Float_t tmpSig = 0;

  for (UInt_t n = 0; n < nsteps; n++){
    Sigma[n] = 0;
    Err[n] = 0;
    Sstart = sighiststart->Integral(); 
    Byield = bkghisto[n]->Integral();
    Berror = sqrt(Byield); 
    if (Byield < 0) Byield = 0.0;
    Syield = sighisto[n]->Integral();
    Seff   = Syield/Sstart;
    efferr = Seff*(1.0-Seff)/Sstart;
    Sigma[n] = Seff/(2.5+sqrt(Byield)); 
    Err[n]   = efferr/(2.5+sqrt(Byield)) - Seff*Berror/(sqrt(Byield)*(2.5+sqrt(Byield))*(2.5+sqrt(Byield))); 
 
    std::cout << var << " cut at " << cutval[n] << " : sigma = " << Sigma[n]  << "+/-" << Err[n] << endl;
   
    tmpSig = 0;
    if (Sigma[n] > tmpSig){ 
      tmpSig = Sigma[n];
      cut = cutval[n];
    }

  }

  delete sighiststart;
  for (UInt_t n = 0; n < nsteps; n++){
    delete bkghisto[n];
    delete sighisto[n];
  }

  return cut; 
} 




