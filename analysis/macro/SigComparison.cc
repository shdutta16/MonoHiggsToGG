#include <TFile.h>
#include <TLine.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMath.h>
#include "Style.hh"
#include "Style.cpp"
#include "../../../DataFormats/Math/interface/deltaPhi.h"
#include "mkPlotsLivia/CMS_lumi.C"
#include <iostream>

using namespace std;

void makePlots(std::string, int, double, double, TString, TString, TString, TString, bool);

void SigComparison(){
 
  cout << "Signal Comparison" << endl;

  //TString inDir = "data/25ns_v76X_v2/";
  TString inDir = "";
  TString nameFileSig1 = "mA0300"; 
  TString nameFileSig2 = "mA0800"; 
  TString outDir = "~/www/Plots/25ns_Limits_76X_2DResults/";
  // SPECIFY LUMI in mkPlotsLivia/CMS_lumi.C

  cout << "Comparing Sample " << nameFileSig1 << " and " << nameFileSig2 << endl;

  //makePlots("mgg",    26, 99., 151, inDir, outDir, nameFileSig1, nameFileSig2);
  //makePlots("t1pfmet",75, 0.,  900, inDir, outDir, nameFileSig1, nameFileSig2, 0);
  //makePlots("pt1",    30, 0.,  300, inDir, outDir, nameFileSig1, nameFileSig2, 0);
  //makePlots("pt2",    30, 0.,  300, inDir, outDir, nameFileSig1, nameFileSig2, 0);
  //makePlots("ptgg",   60, 0.,  600, inDir, outDir, nameFileSig1, nameFileSig2, 0);
  makePlots("deltaR", 20, 0.,    4, inDir, outDir, nameFileSig1, nameFileSig2, 1);
  //makePlots("nvtx",   40, 0.,  40,  inDir, outDir, nameFileSig1, nameFileSig2);

}


void makePlots(std::string var, int BINS, double MIN, double MAX, TString inDir, TString outDir, TString nameFileSig1, TString nameFileSig2, bool special){
  
  TString mainCut = "(mgg>=100 && mgg<180)";

  TFile* fileSig1;
  TFile* fileSig2;
  fileSig1 = new TFile(inDir+"diPhotons_"+nameFileSig1+"_mZp2000_woPhoID.root"); 
  fileSig2 = new TFile(inDir+"diPhotons_"+nameFileSig2+"_mZp2000_woPhoID.root"); 
  
  TTree* treeSig1 = (TTree*)fileSig1->Get("DiPhotonTree");
  TTree* treeSig2 = (TTree*)fileSig2->Get("DiPhotonTree");

  TH1F* h_Sig1 = new TH1F("h_Sig1","",BINS,MIN,MAX);
  TH1F* h_Sig2  = new TH1F("h_Sig2","",BINS,MIN,MAX);

  gStyle->SetOptStat(0);

  TCanvas* ctmp = new TCanvas("ctmp",""); 
  ctmp->cd();

  TPad * pad1 = new TPad("","",0.01,0.001,0.99,1.);//x1,y1,x2,y2
  pad1->SetBottomMargin(0.1);
  pad1->SetTopMargin(0.1);
  pad1->SetRightMargin(0.06);
  pad1->SetLeftMargin(0.12);
  pad1->SetLogy(1);
  pad1->Draw();
  pad1->cd();
 
  if (!special){  
    treeSig1->Draw((var+">>h_Sig1").c_str(), "("+mainCut+")*1");
    treeSig2->Draw((var+">>h_Sig2").c_str(), "("+mainCut+")*1");
  }

  if (special){
    Float_t eta1;
    Float_t eta2;
    Float_t phi1;
    Float_t phi2;
    TBranch *b_eta1;
    TBranch *b_eta2;
    TBranch *b_phi1;
    TBranch *b_phi2;
    treeSig1->SetBranchAddress("eta1", &eta1, &b_eta1);
    treeSig1->SetBranchAddress("eta2", &eta2, &b_eta2);
    treeSig1->SetBranchAddress("phi1", &phi1, &b_phi1);
    treeSig1->SetBranchAddress("phi2", &phi2, &b_phi2);
    UInt_t nentries1 = treeSig1->GetEntries();
    for (UInt_t entry = 0; entry < nentries1; entry++){
      treeSig1->GetEntry(entry);
      double delta_eta = eta1-eta2;
      double delta_phi = deltaPhi(phi1,phi2);
      double deltaR = TMath::Sqrt(delta_eta*delta_eta + delta_phi*delta_phi);
      h_Sig1->Fill(deltaR);
    }

    TBranch *b_eta2_1;
    TBranch *b_eta2_2;
    TBranch *b_phi2_1;
    TBranch *b_phi2_2;
    treeSig2->SetBranchAddress("eta1", &eta1, &b_eta2_1);
    treeSig2->SetBranchAddress("eta2", &eta2, &b_eta2_2);
    treeSig2->SetBranchAddress("phi1", &phi1, &b_phi2_1);
    treeSig2->SetBranchAddress("phi2", &phi2, &b_phi2_2);
    UInt_t nentries2 = treeSig2->GetEntries();
    for (UInt_t entry = 0; entry < nentries1; entry++){
      treeSig2->GetEntry(entry);
      double delta_eta = eta1-eta2;
      double delta_phi = deltaPhi(phi1,phi2);
      double deltaR = TMath::Sqrt(delta_eta*delta_eta + delta_phi*delta_phi);
      h_Sig2->Fill(deltaR);
    }
    
  }

  h_Sig1->SetLineColor(kRed);
  h_Sig1->SetFillColor(kWhite);
  h_Sig1->SetMarkerSize(0.6);
  h_Sig1->Sumw2();

  h_Sig2->SetLineColor(kBlue);
  h_Sig2->SetFillColor(kWhite);
  h_Sig2->SetMarkerSize(0.6);
  h_Sig2->Sumw2();

  double IntSig1 = h_Sig1->Integral();
  double IntSig2  = h_Sig2->Integral();
  if (IntSig1 > 0) h_Sig1->Scale(1/IntSig1);
  if (IntSig2  > 0) h_Sig2->Scale(1/IntSig2);

  h_Sig1->Draw("HE");
  h_Sig1->GetYaxis()->SetTitle(TString::Format("Evens /%.2f",double((MAX-MIN))/BINS));
  h_Sig1->GetXaxis()->SetTitle((var).c_str());
  h_Sig1->SetMaximum(h_Sig1->GetMaximum()*10);

  h_Sig1->Draw("HE");
  h_Sig2->Draw("HE SAME");

  TLegend *leg1;
  leg1 = new TLegend(0.58,0.80,0.82,0.85,"", "brNDC");// x1,y1,x2,y2
  leg1->AddEntry(h_Sig1,nameFileSig1,"L");
  leg1->AddEntry(h_Sig2,nameFileSig2,"L");

  leg1->SetTextSize(0.035);
  leg1->SetTextFont(42);
  leg1->SetBorderSize(0);
  leg1->SetFillStyle(0);
  leg1->Draw("same");

  pad1->SetLogy(1);
  pad1->RedrawAxis();

  //TH1F *h_ratio;
  //h_ratio = (TH1F*)h_Sig1->Clone();
  //h_ratio->SetTitle("");
  //h_ratio->Divide(h_Sig2); 
  //h_ratio->SetLineColor(kBlack);
  //h_ratio->SetMinimum(-0.1);// define Y
  //h_ratio->SetMaximum(2.1); // .. range
  //h_ratio->SetStats(0);
  //h_ratio->GetYaxis()->SetTitle("Sig1/Sig2");

  //TLine *h_line;
  //h_line->SetY1(1.0);
  //h_line->SetY2(1.0); 
  //h_line->SetX1(h_ratio->GetXaxis()->GetXmin());
  //h_line->SetX2(h_ratio->GetXaxis()->GetXmax()); 
  //h_line->SetLineWidth(2);
  //h_line->SetLineColor(kRed);

  ctmp->cd();

  //TPad * pad2 = new TPad("","",0.01,0.001,0.99,1.);//x1,y1,x2,y2
  //pad2->SetBottomMargin(0.4);
  //pad2->SetRightMargin(0.06);
  //pad2->SetLeftMargin(0.12);
  //pad2->SetLogy(1);
  //pad2->Draw();
  //pad2->cd();

  //h_ratio->Draw("EP");
  //h_line->Draw("SAME");

  //h_ratio->GetXaxis()->SetLabelSize(0.11);
  //h_ratio->GetXaxis()->SetTitleSize(0.15);
  //h_ratio->GetXaxis()->SetTickSize(0.11);
  //h_ratio->GetYaxis()->SetNdivisions(505);
  //h_ratio->GetYaxis()->SetLabelSize(0.11);
  //h_ratio->GetYaxis()->SetTitleSize(0.15);
  //h_ratio->GetYaxis()->SetTitleOffset(0.35);

  //h_ratio->Draw("EP SAME");

  
  CMS_lumi(ctmp,false,11);
  ctmp->cd();
  ctmp->SaveAs(Form("%s/sig_%s_%s_%s_log.png",outDir.Data(),var.c_str(),nameFileSig1.Data(),nameFileSig2.Data()));
  ctmp->SaveAs(Form("%s/sig_%s_%s_%s_log.pdf",outDir.Data(),var.c_str(),nameFileSig1.Data(),nameFileSig2.Data()));
 
  delete ctmp;
  //delete h_line;
  //delete h_ratio;
  //delete h_Sig1;
  //delete h_Sig2;
  //delete pad1;
  //delete pad2;

}// makePlots



