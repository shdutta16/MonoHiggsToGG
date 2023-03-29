#include <TFile.h>
#include <TH1.h>
#include <TLorentzVector.h>
#include "../../../DataFormats/Math/interface/deltaPhi.h"

void runVar(TString, TString, TString, bool, string, int, float, float);
TH1F* prepTree(TString, string, int, float, float, TString);
TH1F* newVar(TString, string, int, float, float, TString); 

void compareVar(){

  TString indir  = "/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/";
  TString outdir = "~/www/Plots/MonoHgg2017/VariableShapes/"; 
  TString skim  = "_rawskim"; 

  //runVar(indir,outdir,skim,0,"mgg",		26, 99, 151);
  //runVar(indir,outdir,skim,0,"t1pfmetCorr",	90, 0,  900);
  //runVar(indir,outdir,skim,0,"ptgg",		60, 0,  600);
  //runVar(indir,outdir,skim,0,"pt1",		30, 0,  300);
  //runVar(indir,outdir,skim,0,"pt2",		30, 0,  300);
  //runVar(indir,outdir,skim,0,"eta1",		20, -4, 4);
  //runVar(indir,outdir,skim,0,"eta2",		20, -4, 4);
  //runVar(indir,outdir,skim,0,"nJets30",		10, 0,  10);
  //runVar(indir,outdir,skim,0,"nEle",		5,  0,  5);
  //runVar(indir,outdir,skim,0,"nMuons",		5,  0,  5);
  //runVar(indir,outdir,skim,0,"dphiggmet",	20, 0,  4);
  //runVar(indir,outdir,skim,0,"mindphijmet",	20, 0,  4);
  runVar(indir,outdir,skim,1,"dphig1met",	20, 0,  4);
  runVar(indir,outdir,skim,1,"dphig2met",	20, 0,  4);
  runVar(indir,outdir,skim,1,"detag1g2",	20, 0,  4);
 
}

void runVar(TString indir, TString outdir, TString skim, bool NewVar, string VAR, int BIN, float MIN, float MAX){
 
  std::cout << "Comparing variable " << VAR << std::endl;

  TFile* fout = new TFile(outdir+"OutputVarComparison.root","UPDATE");
  TCanvas* c = new TCanvas("","",800,800);
  TLegend* l = new TLegend(0.5,0.70,0.85,0.88,"","brNDC");// x1,y1,x2,y2
  TPad* p = new TPad("","",0.01,0.01,0.99,0.99); 
  l->SetBorderSize(0); 

  TH1F* sig1; 
  TH1F* sig2; 
  TH1F* bkg1; 
  TH1F* bkg2; 
  TH1F* bkg3; 

  if (NewVar){
   sig1 = newVar(indir+"2HDM_mZP600_mA0300"+skim,     VAR,BIN,MIN,MAX,"");
   sig2 = newVar(indir+"BaryonicZp_mZP10_mChi1"+skim, VAR,BIN,MIN,MAX,"");
   bkg1 = newVar(indir+"DiPhoton"+skim,               VAR,BIN,MIN,MAX,"");
   bkg2 = newVar(indir+"VHToGG"+skim,                 VAR,BIN,MIN,MAX,"");
   bkg3 = newVar(indir+"WGToLNuG"+skim,               VAR,BIN,MIN,MAX,"");
  }
  else {
   sig1 = prepTree(indir+"2HDM_mZP600_mA0300"+skim,     VAR,BIN,MIN,MAX,"");
   sig2 = prepTree(indir+"BaryonicZp_mZP10_mChi1"+skim, VAR,BIN,MIN,MAX,"");
   bkg1 = prepTree(indir+"DiPhoton"+skim,               VAR,BIN,MIN,MAX,"");
   bkg2 = prepTree(indir+"VHToGG"+skim,                 VAR,BIN,MIN,MAX,"");
   bkg3 = prepTree(indir+"WGToLNuG"+skim,               VAR,BIN,MIN,MAX,"");
  }

  float max1 = sig1->GetMaximum();
  float max2 = (max1 > sig2->GetMaximum())? max1:sig2->GetMaximum();
  float max3 = (max2 > bkg1->GetMaximum())? max2:bkg1->GetMaximum();
  float max4 = (max3 > bkg2->GetMaximum())? max3:bkg2->GetMaximum();
  float max  = (max4 > bkg3->GetMaximum())? max4:bkg3->GetMaximum();
   
  sig1->SetMaximum(max*2);
  sig1->SetMinimum(0.0001); 
  sig1->GetYaxis()->SetTitle("A.U.");
  TString varname = "";
  if (VAR=="mgg") 		varname = "m_{#gamma#gamma} [GeV]";
  else if (VAR=="t1pfmetCorr")	varname = "p_{T}^{miss} [GeV]";
  else if (VAR=="ptgg")		varname = "p_{T,#gamma#gamma} [GeV]";
  else if (VAR=="pt1") 		varname = "p_{T,1} [GeV]";
  else if (VAR=="pt2") 		varname = "p_{T,2} [GeV]";
  else if (VAR=="eta1")		varname = "#eta_{1}";
  else if (VAR=="eta2")		varname = "#eta_{2}";
  else if (VAR=="dphiggmet")	varname = "|#Delta#phi(#gamma#gamma,p_{T}^{miss})|";
  else if (VAR=="mindphijmet")  varname = "|#Delta#phi(j,p_{T}^{miss})|";
  else if (VAR=="nJets30")	varname = "Num. jets";
  else if (VAR=="nEle")		varname = "Num. electrons";
  else if (VAR=="nMuons")	varname = "Num. muons";
  else if (VAR=="dphig1met")	varname = "|#Delta#phi(#gamma_{1},p_{T}^{miss})|";
  else if (VAR=="dphig2met")	varname = "|#Delta#phi(#gamma_{2},p_{T}^{miss})|";
  else if (VAR=="detag1g2")	varname = "|#Delta#eta(#gamma_{1},#gamma_{2})|";
  else varname = VAR;
  std::cout << "Variable name: " << varname << std::endl;
  sig1->GetXaxis()->SetTitle(varname);


  c->cd(); p->Draw(); p->cd();
  gStyle->SetOptStat(0);
  sig1->SetLineWidth(2);
  sig2->SetLineWidth(2);
  sig1->SetLineColor(kRed);
  sig2->SetLineColor(kMagenta);
  bkg1->SetLineColor(kBlack);
  bkg1->SetFillColor(kBlack);
  bkg1->SetFillStyle(3003);
  bkg2->SetLineColor(kGreen);
  bkg3->SetLineColor(kTeal);
  
  l->AddEntry(sig1,"2HDM: m_{Z'} = 600 GeV, m_{A} = 300 GeV","L");
  l->AddEntry(sig2,"BARY: m_{Z'} = 10 GeV, m_{#chi} = 1 GeV","L");
  l->AddEntry(bkg1,"#gamma#gamma","LF");
  l->AddEntry(bkg2,"V+H","L");
  l->AddEntry(bkg3,"W(l#nu)+#gamma","L");
   
  sig1->Draw("HIST");
  bkg1->Draw("HIST SAME");
  bkg2->Draw("HIST SAME");
  bkg3->Draw("HIST SAME");
  sig1->Draw("HIST SAME");
  sig2->Draw("HIST SAME");

  l->Draw("SAME");
  p->SetLogy(0);
  c->SaveAs(Form("%scompare_%s_lin.png",outdir.Data(),VAR.c_str())); 
  c->SaveAs(Form("%scompare_%s_lin.pdf",outdir.Data(),VAR.c_str()));
  p->SetLogy(1); 
  c->SaveAs(Form("%scompare_%s_log.png",outdir.Data(),VAR.c_str())); 
  c->SaveAs(Form("%scompare_%s_log.pdf",outdir.Data(),VAR.c_str())); 

  delete c; 
}

TH1F* prepTree(TString fin, string VAR, int BIN, float MIN, float MAX, TString cut=""){
  TFile* f = new TFile(fin+".root");
  TTree* t = (TTree*)f->Get("DiPhotonTree");
  TH1F*  h = new TH1F("h","",BIN,MIN,MAX); 
  TString cutstr = ""; 
  if (cut!="") cutstr = "("+cut+")*";
  cutstr += "weight"; 
  t->Draw((VAR+">>h").c_str(),cutstr);
  float totint = h->Integral();
  h->Scale(1/totint);
  return h; 
}

TH1F* newVar(TString fin, string VAR, int BIN, float MIN, float MAX, TString cut=""){
  std::cout << "Running " << fin << std::endl;
  TFile* f = new TFile(fin+".root");
  TTree* t = (TTree*)f->Get("DiPhotonTree");
  TH1F*  h = new TH1F("h","",BIN,MIN,MAX);
 
  // variables needed
  Float_t weight;	t->SetBranchAddress("weight",&weight);
  Float_t eta1;		t->SetBranchAddress("eta1",&eta1);
  Float_t eta2;		t->SetBranchAddress("eta2",&eta2);
  Float_t phi1;		t->SetBranchAddress("phi1",&phi1);
  Float_t phi2;		t->SetBranchAddress("phi2",&phi2);
  Float_t pt1;		t->SetBranchAddress("pt1",&pt1);
  Float_t pt2;		t->SetBranchAddress("pt2",&pt2);
  Float_t met;		t->SetBranchAddress("t1pfmetCorr",&met);
  Float_t metphi;	t->SetBranchAddress("t1pfmetCorrPhi",&metphi);
  
  // new variables made
  Float_t dphig1met;
  Float_t dphig2met;
  Float_t detag1g2;

  TLorentzVector lorVecPho1;
  TLorentzVector lorVecPho2;

  // loop over entries
  for (UInt_t entry = 0; entry < t->GetEntries(); entry++)
  {
    t->GetEntry(entry);
    lorVecPho1.SetPtEtaPhiM(pt1,eta1,phi1,0.);
    lorVecPho2.SetPtEtaPhiM(pt2,eta2,phi2,0.);
    
    dphig1met = TMath::Abs(deltaPhi(lorVecPho1.Phi(),metphi));
    dphig2met = TMath::Abs(deltaPhi(lorVecPho2.Phi(),metphi));
    detag1g2  = TMath::Abs(lorVecPho1.Eta()-lorVecPho2.Eta());
  
    if (VAR=="dphig1met") h->Fill(dphig1met,weight);
    if (VAR=="dphig2met") h->Fill(dphig2met,weight);
    if (VAR=="detag1g2")  h->Fill(detag1g2,weight);  
  }
 
  float totint = h->Integral();
  h->Scale(1/totint);
  return h; 
}
