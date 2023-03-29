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

void makePlots(std::string, int, double, double, TString, TString, std::vector<TString>, UInt_t, TString);

void AllSignalComparisons(){
 
  cout << "Signal Comparison" << endl;

  TString inDir = "data/25ns_v80X_v0/";
  TString mZp = "1000";
  TString nameFileSig1 = Form("2HDM_mZP%s_mA0300",mZp.Data()); 
  TString nameFileSig2 = Form("2HDM_mZP%s_mA0400",mZp.Data()); 
  TString nameFileSig3 = Form("2HDM_mZP%s_mA0500",mZp.Data()); 
  TString nameFileSig4 = Form("2HDM_mZP%s_mA0600",mZp.Data()); 
  TString nameFileSig5 = Form("2HDM_mZP%s_mA0700",mZp.Data()); 
  TString nameFileSig6 = Form("2HDM_mZP%s_mA0800",mZp.Data()); 
  std::vector< TString > nameFiles;
  nameFiles.push_back(nameFileSig1);
  nameFiles.push_back(nameFileSig2);
  nameFiles.push_back(nameFileSig3);
  nameFiles.push_back(nameFileSig4);
  nameFiles.push_back(nameFileSig5);
  nameFiles.push_back(nameFileSig6);
  UInt_t nFiles = nameFiles.size();
  
  TString outDir = "~/www/Plots/25ns_Limits_80X_Results";
  // SPECIFY LUMI in mkPlotsLivia/CMS_lumi.C

  cout << "Comparing Sample with mZp = " << mZp << endl;

  makePlots("mgg",    26, 99., 151,  inDir, outDir, nameFiles, nFiles, mZp);
  makePlots("t1pfmet",200, 0., 2000, inDir, outDir, nameFiles, nFiles, mZp);
  //makePlots("pt1",    30, 0.,  300,  inDir, outDir, nameFiles, nFiles, mZp);
  //makePlots("pt2",    30, 0.,  300,  inDir, outDir, nameFiles, nFiles, mZp);
  //makePlots("ptgg",   60, 0.,  600,  inDir, outDir, nameFiles, nFiles, mZp);
  //makePlots("nvtx",   40, 0.,  40,   inDir, outDir, nameFiles, nFiles, mZp);

}

void makePlots(std::string var, int BINS, double MIN, double MAX, TString inDir, TString outDir, std::vector<TString> nameFiles, UInt_t nFiles, TString mZp){
  
  TString mainCut = "(mgg>=100 && mgg<180)";

  std::vector< TFile* > fileSig;
  fileSig.resize(nFiles);
  for (UInt_t n=0; n<nFiles; n++){
    fileSig[n] = new TFile(inDir+nameFiles[n]+".root"); 
  } 

  std::vector< TTree* > treeSig;
  treeSig.resize(nFiles);
  for (UInt_t n=0; n<nFiles; n++){
    treeSig[n] = (TTree*)fileSig[n]->Get("DiPhotonTree");
  }
  
  std::vector< TH1F* > histoSig;
  histoSig.resize(nFiles);
  //for (UInt_t n=0; n<nFiles; n++){
  //  histoSig[n] = new TH1F("histoSig","",BINS,MIN,MAX);
  //}
 
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
 
  for (UInt_t n=0; n<nFiles; n++){
    TH1F* histoTmp = new TH1F("histoTmp","",BINS,MIN,MAX);
    treeSig[n]->Draw((var+">>histoTmp").c_str(), "("+mainCut+")*1");
    histoSig[n] = (TH1F*)histoTmp->Clone(); 
    delete histoTmp;
  }

  for (UInt_t n=0; n<nFiles; n++){
    histoSig[n]->SetFillColor(kWhite);
    histoSig[n]->SetLineWidth(2);
    histoSig[n]->SetMarkerSize(0.6);
    histoSig[n]->Sumw2();
  }

  if (nFiles >= 1) histoSig[0]->SetLineColor(kBlack);
  if (nFiles >= 2) histoSig[1]->SetLineColor(kCyan);
  if (nFiles >= 3) histoSig[2]->SetLineColor(kGreen);
  if (nFiles >= 4) histoSig[3]->SetLineColor(kBlue);
  if (nFiles >= 5) histoSig[4]->SetLineColor(kYellow);
  if (nFiles >= 6) histoSig[5]->SetLineColor(kMagenta);

  double integral = 0;
  for (UInt_t n=0; n<nFiles; n++){
    integral = histoSig[n]->Integral();
    if (integral > 0) histoSig[n]->Scale(1/integral);
  }

  for (UInt_t n=0; n<nFiles; n++){
    if (n==0){
      histoSig[n]->Draw("HE");
      histoSig[n]->GetYaxis()->SetTitle(TString::Format("Events/%.2f",double((MAX-MIN))/BINS));
      histoSig[n]->GetXaxis()->SetTitle((var).c_str());
      if (var=="t1pfmet") histoSig[n]->SetMaximum(histoSig[n]->GetMaximum()*5);
      else histoSig[n]->SetMaximum(histoSig[n]->GetMaximum()*100);
      histoSig[n]->Draw("HE");
    }
    else histoSig[n]->Draw("HE SAME");  

  }

  TLegend *leg1;
  leg1 = new TLegend(0.58,0.65,0.82,0.9,"", "brNDC");// x1,y1,x2,y2
  for (UInt_t n=0; n<nFiles; n++){
     if (n==0) leg1->AddEntry(histoSig[n],nameFiles[n]+"_mA0300","L");
     else leg1->AddEntry(histoSig[n],nameFiles[n],"L");
  }

  leg1->SetTextSize(0.035);
  leg1->SetTextFont(42);
  leg1->SetBorderSize(0);
  leg1->SetFillStyle(0);
  leg1->Draw("same");

  pad1->SetLogy(1);
  pad1->RedrawAxis();

  ctmp->cd();
  
  CMS_lumi(ctmp,false,0);
  ctmp->cd();
  ctmp->SaveAs(Form("%s/sig_mZp%s_%s_log.png",outDir.Data(),mZp.Data(),var.c_str()));
  ctmp->SaveAs(Form("%s/sig_mZp%s_%s_log.pdf",outDir.Data(),mZp.Data(),var.c_str()));
 
  delete ctmp;

}// makePlots



