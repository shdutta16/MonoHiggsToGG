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

void makePlots(std::string, int, double, double, TString, TString, TString, TString);
void getEff(TH1F *&, double&, double&);


void PhoIDEff(){
 
  cout << "PhoID Eff Comparison" << endl;

  TString inDir = "TestingPhoID/";
  TString nameFileSig1 = "diPhotons_"; 
  TString nameFileSig2 = "diPhotons_"; 
  TString outDir = "~/www/Plots/PhoIDEffStudies/";
  // SPECIFY LUMI in mkPlotsLivia/CMS_lumi.C

  cout << "Comparing Sample " << nameFileSig1 << " and " << nameFileSig2 << endl;

  makePlots("h_selection", 10, 0., 10, inDir, outDir, nameFileSig1, nameFileSig2);

}


void makePlots(std::string var, int BINS, double MIN, double MAX, TString inDir, TString outDir, TString nameFileSig1, TString nameFileSig2){

  TFile* fileSig600_1;
  TFile* fileSig600_2;
  fileSig600_1 = new TFile(inDir+nameFileSig1+"600_"+"oldID.root"); 
  fileSig600_2 = new TFile(inDir+nameFileSig2+"600_"+"newID.root"); 
  TFile* fileSig2000_1;
  TFile* fileSig2000_2;
  fileSig2000_1 = new TFile(inDir+nameFileSig1+"2000_"+"oldID.root"); 
  fileSig2000_2 = new TFile(inDir+nameFileSig2+"2000_"+"newID.root"); 
  TFile* fileSig2500_1;
  TFile* fileSig2500_2;
  fileSig2500_1 = new TFile(inDir+nameFileSig1+"2500_"+"oldID.root"); 
  fileSig2500_2 = new TFile(inDir+nameFileSig2+"2500_"+"newID.root"); 

  if (fileSig600_1 == (TFile*) NULL) std::cout << "file bad" << std::endl;

  TH1F* h_Sig600_1  = (TH1F*)fileSig600_1->Get("h_selection");
  TH1F* h_Sig600_2  = (TH1F*)fileSig600_2->Get("h_selection");
  TH1F* h_Sig2000_1 = (TH1F*)fileSig2000_1->Get("h_selection");
  TH1F* h_Sig2000_2 = (TH1F*)fileSig2000_2->Get("h_selection");
  TH1F* h_Sig2500_1 = (TH1F*)fileSig2500_1->Get("h_selection");
  TH1F* h_Sig2500_2 = (TH1F*)fileSig2500_2->Get("h_selection");

  std::cout << "picked up histos" << std::endl;

  gStyle->SetOptStat(0);

  TCanvas* ctmp = new TCanvas("ctmp",""); 
  ctmp->cd();

  TPad * pad1 = new TPad("","",0.01,0.001,0.99,1.);//x1,y1,x2,y2
  pad1->SetBottomMargin(0.1);
  pad1->SetTopMargin(0.1);
  pad1->SetRightMargin(0.06);
  pad1->SetLeftMargin(0.12);
  pad1->SetLogy(0);
  pad1->SetLogx(1);
  pad1->Draw();
  pad1->cd();
  
  if (h_Sig600_1 == (TH1F*) NULL) std::cout << "TH1F 1 bad" << std::endl;
  if (h_Sig600_2 == (TH1F*) NULL) std::cout << "TH1F 2 bad" << std::endl;
 
  //double phoIDnum600_1 = h_Sig600_1->GetBinContent(3);//passes phoID
  //double phoIDden600_1 = h_Sig600_1->GetBinContent(2);//passes presel
  //double phoIDnum600_2 = h_Sig600_2->GetBinContent(3);//passes phoID
  //double phoIDden600_2 = h_Sig600_2->GetBinContent(2);//passes presel

  //std::cout << phoIDnum600_1 << " / " << phoIDden600_1 << std::endl;
  //std::cout << phoIDnum600_2 << " / " << phoIDden600_2 << std::endl;

  //double phoIDeff600_1 = phoIDnum600_1/phoIDden600_1;
  //double phoIDeff600_2 = phoIDnum600_2/phoIDden600_2;
  //double phoIDefferr600_1 = TMath::Sqrt(phoIDeff600_1*(1.0-phoIDeff600_1)/phoIDnum600_1); 
  //double phoIDefferr600_2 = TMath::Sqrt(phoIDeff600_2*(1.0-phoIDeff600_2)/phoIDnum600_2); 

  double eff600_old = 0;
  double err600_old = 0;
  double eff600_new = 0;
  double err600_new = 0;
  double eff2000_old = 0;
  double err2000_old = 0;
  double eff2000_new = 0;
  double err2000_new = 0;
  double eff2500_old = 0;
  double err2500_old = 0;
  double eff2500_new = 0;
  double err2500_new = 0;
  getEff(h_Sig600_1, eff600_old, err600_old);
  getEff(h_Sig2000_1, eff2000_old, err2000_old);
  getEff(h_Sig2500_1, eff2500_old, err2500_old);
  getEff(h_Sig600_2, eff600_new, err600_new);
  getEff(h_Sig2000_2, eff2000_new, err2000_new);
  getEff(h_Sig2500_2, eff2500_new, err2500_new);

  double m_point[3] = {600,2000,2500};
  double eff_old[3] = {eff600_old, eff2000_old, eff2500_old};
  double eff_new[3] = {eff600_new, eff2000_new, eff2500_new};
  double err_old[3] = {err600_old, err2000_old, err2500_old};
  double err_new[3] = {err600_new, err2000_new, err2500_new};

  TGraphErrors* h_Sig1 = new TGraphErrors(3,m_point,eff_old,0,err_old);
  TGraphErrors* h_Sig2 = new TGraphErrors(3,m_point,eff_new,0,err_new);

  h_Sig1->SetMarkerColor(kRed);
  h_Sig1->SetLineColor(kRed);
  h_Sig1->SetMarkerStyle(8);
  h_Sig1->SetLineWidth(2);
  //h_Sig1->Sumw2();

  h_Sig2->SetMarkerColor(kBlue);
  h_Sig2->SetLineColor(kBlue);
  h_Sig2->SetMarkerStyle(8);
  h_Sig2->SetLineWidth(2);
  //h_Sig2->Sumw2();

  h_Sig1->Draw("ALP");

  h_Sig1->SetTitle("");
  h_Sig1->GetYaxis()->SetTitle("Efficiency");
  h_Sig1->GetXaxis()->SetTitle("m_{Z'}");
  h_Sig1->SetMaximum(1.0);

  h_Sig2->Draw("PL SAME");

  TLegend *leg1;
  leg1 = new TLegend(0.58,0.80,0.82,0.85,"", "brNDC");// x1,y1,x2,y2
  leg1->AddEntry(h_Sig1,"Old PhoID","L");
  leg1->AddEntry(h_Sig2,"New PhoID","L");

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

  
  CMS_lumi(ctmp,false,0);
  ctmp->cd();
  ctmp->SaveAs(Form("%s/sig_phoID_log.png",outDir.Data()));
  ctmp->SaveAs(Form("%s/sig_phoID_log.pdf",outDir.Data()));
 
  delete ctmp;
  //delete h_line;
  //delete h_ratio;
  //delete h_Sig1;
  //delete h_Sig2;
  //delete pad1;
  //delete pad2;

}// makePlots


void getEff(TH1F *& histo, double & eff, double & efferr){

  double num = histo->GetBinContent(3); // passes phoID
  double den = histo->GetBinContent(2); // passes presel
  eff = num/den;
  efferr = TMath::Sqrt(eff*(1-eff)/num); 

}// getEff
