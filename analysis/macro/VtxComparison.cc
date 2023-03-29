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
#include "mkPlotsLivia/CMS_lumi.C"
#include <iostream>

using namespace std;

void makePlots(std::string, int, double, double, int);

void VtxComparison(){
 
  cout << "Vtx Comparison" << endl; 

  // i=0 data, i=1 sig, i=2 bkg 
  for (Int_t i = 0; i < 3; i++){ 
    makePlots("pt1",    30, 0.,  300, i);
    makePlots("pt2",    30, 0.,  300, i);
    makePlots("mgg",    26, 99., 151, i);
    makePlots("ptgg",   60, 0.,  600, i);
    makePlots("nvtx",   40, 0.,  40,  i);
    makePlots("t1pfmet",75, 0.,  900, i);
  }
   
}


void makePlots(std::string var, int BINS, double MIN, double MAX, int type){
  TString inDir = "vtxCompare/";
  
  // lumi
  Float_t lumi = 2.2; //fb^-1 

  TString mainCut = "(mgg>=100 && mgg<180) && hltDiphoton30Mass95";
  TString blindData = "(mgg < 115 || mgg > 135)";

  TFile* fileVtx0;
  TFile* fileBDT;
  if (type == 0){// data
    fileVtx0 = new TFile(inDir+"diPhotons_data_Vtx0.root"); 
    fileBDT  = new TFile(inDir+"diPhotons_data_BDT.root"); 
  }
  if (type == 1){// sig
    fileVtx0 = new TFile(inDir+"diPhotons_sig_Vtx0.root"); 
    fileBDT  = new TFile(inDir+"diPhotons_sig_BDT.root"); 
  }
  if (type == 2){// bkg
    fileVtx0 = new TFile(inDir+"diPhotons_bkg_Vtx0.root"); 
    fileBDT  = new TFile(inDir+"diPhotons_bkg_BDT.root"); 
  }
  
  TTree* treeVtx0 = (TTree*)fileVtx0->Get("diPhoAna/DiPhotonTree");
  TTree* treeBDT = (TTree*)fileBDT->Get("diPhoAna/DiPhotonTree");

  TH1F* h_Vtx0 = new TH1F("h_Vtx0","",BINS,MIN,MAX);
  TH1F* h_BDT  = new TH1F("h_BDT","",BINS,MIN,MAX);

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
  
  if ((var=="mgg" || var=="t1pfmet") && type==0){
    treeVtx0->Draw((var+">>h_Vtx0").c_str(), "("+mainCut+" && "+blindData+")*1");
    treeBDT->Draw((var+">>h_BDT").c_str(), "("+mainCut+" && "+blindData+")*1");
  }
  else{
    treeVtx0->Draw((var+">>h_Vtx0").c_str(), "("+mainCut+")*1");
    treeBDT->Draw((var+">>h_BDT").c_str(), "("+mainCut+")*1");
  }

  h_Vtx0->SetLineColor(kRed);
  h_Vtx0->SetFillColor(kWhite);
  h_Vtx0->SetMarkerSize(0.6);
  h_Vtx0->Sumw2();

  h_BDT->SetLineColor(kBlue);
  h_BDT->SetFillColor(kWhite);
  h_BDT->SetMarkerSize(0.6);
  h_BDT->Sumw2();

  double IntVtx0 = h_Vtx0->Integral();
  double IntBDT  = h_BDT->Integral();
  if (IntVtx0 > 0) h_Vtx0->Scale(1/IntVtx0);
  if (IntBDT  > 0) h_BDT->Scale(1/IntBDT);

  h_Vtx0->Draw("HE");
  h_Vtx0->GetYaxis()->SetTitle(TString::Format("Evens /%.2f",double((MAX-MIN))/BINS));
  h_Vtx0->GetXaxis()->SetTitle((var).c_str());
  h_Vtx0->SetMaximum(h_Vtx0->GetMaximum()*10);

  h_Vtx0->Draw("HE");
  h_BDT->Draw("HE SAME");

  TLegend *leg1;
  leg1 = new TLegend(0.58,0.80,0.82,0.85,"", "brNDC");// x1,y1,x2,y2
  leg1->AddEntry(h_Vtx0,"Vtx0","L");
  leg1->AddEntry(h_BDT,"BDT","L");

  leg1->SetTextSize(0.035);
  leg1->SetTextFont(42);
  leg1->SetBorderSize(0);
  leg1->SetFillStyle(0);
  leg1->Draw("same");

  pad1->SetLogy(1);
  pad1->RedrawAxis();

  //TH1F *h_ratio;
  //h_ratio = (TH1F*)h_Vtx0->Clone();
  //h_ratio->SetTitle("");
  //h_ratio->Divide(h_BDT); 
  //h_ratio->SetLineColor(kBlack);
  //h_ratio->SetMinimum(-0.1);// define Y
  //h_ratio->SetMaximum(2.1); // .. range
  //h_ratio->SetStats(0);
  //h_ratio->GetYaxis()->SetTitle("Vtx0/BDT");

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
  if (type==0){
    ctmp->SaveAs(("~/www/Plots/CompareVtx/data_"+var+"_log.png").c_str());
    ctmp->SaveAs(("~/www/Plots/CompareVtx/data_"+var+"_log.pdf").c_str());
  }
  if (type==1){
    ctmp->SaveAs(("~/www/Plots/CompareVtx/sig_"+var+"_log.png").c_str());
    ctmp->SaveAs(("~/www/Plots/CompareVtx/sig_"+var+"_log.pdf").c_str());
  }
  if (type==2){
    ctmp->SaveAs(("~/www/Plots/CompareVtx/bkg_"+var+"_log.png").c_str());
    ctmp->SaveAs(("~/www/Plots/CompareVtx/bkg_"+var+"_log.pdf").c_str());
  }
 
  delete ctmp;
  //delete h_line;
  //delete h_ratio;
  //delete h_Vtx0;
  //delete h_BDT;
  //delete pad1;
  //delete pad2;

}// makePlots



