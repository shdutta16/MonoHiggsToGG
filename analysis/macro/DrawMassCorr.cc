#include "mkPlotsLivia/CMS_lumi.C"
using namespace RooFit;
using namespace RooStats;

std::string filePOSTfix="";

void MakeComparisonPlots(TString, TString, int, double, double);


void run(){
  cout << "Plot mgg Raw vs Corrections" << endl;

  MakeComparisonPlots("massRaw", "massCorrScale", 50, 100., 150.);
  MakeComparisonPlots("massRaw", "massCorrSmear", 50, 100., 150.);

  return;
}

void MakeComparisonPlots(TString var1, TString var2, int BINS, double MIN, double MAX){
  TString inDir = "";
  TString mainCut = "1";  
  gStyle->SetOptStat(0);

  // read file
  TFile file("/afs/cern.ch/work/m/mzientek/public/25ns_v80X_v3/GluGluHToGG.root");
  TTree* tree = (TTree*) file.Get("DiPhotonTree");

  // get histos to compare
  TH1F *h_1;
  TH1F *h_2;

  TCanvas* c = new TCanvas("c","",1);
  c->cd();
  c->Draw();
  TPad* pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 1.0, 1.0);
  pad1->Draw();
  pad1->cd();

  h_1 = new TH1F(TString::Format("h_%s",var1.Data()),TString::Format("h_%s",var1.Data()), BINS, MIN, MAX);  
  h_2 = new TH1F(TString::Format("h_%s",var2.Data()),TString::Format("h_%s",var2.Data()), BINS, MIN, MAX);  
  
  // draw histos
  tree->Draw(var1+TString::Format(">>h_%s",var1.Data()),"("+mainCut+")*weight");
  tree->Draw(var2+TString::Format(">>h_%s",var2.Data()),"("+mainCut+")*weight");
  h_1->SetLineColor(kBlue);
  h_1->SetTitle("");
  h_1->GetYaxis()->SetTitle("arb. units");
  h_1->GetXaxis()->SetTitle("m(#gamma#gamma) [GeV]");
  h_2->SetLineColor(kRed);
  h_2->SetTitle("");

  // normalize the histos
  Double_t int1 = h_1->Integral();
  if (int1 > 0) h_1->Scale(1.0/int1);
  Double_t int2 = h_2->Integral();
  if (int2 > 0) h_2->Scale(1.0/int2);
  
  // setup the max 
  Double_t max1 = h_1->GetMaximum();
  Double_t max2 = h_2->GetMaximum();
  Double_t theMax = 0;
  if (max1 < max2) theMax = max2;
  else theMax = max1; 
  h_1->SetMaximum(theMax*1.1);


  // setup legend 
  TLegend *leg1 = new TLegend(0.6,0.7,0.9,0.78); // (x1,y1,x2,y2)
  leg1->SetBorderSize(4);
  leg1->SetTextSize(0.03);
  leg1->SetLineWidth(2);
  leg1->AddEntry(h_1,TString::Format("%s",var1.Data()),"L");
  leg1->AddEntry(h_2,TString::Format("%s",var2.Data()),"L");

  int iPos=11;
  CMS_lumi(pad1, true, iPos);
  pad1->RedrawAxis();
  c->cd();

  h_1->Draw("HIST");
  h_2->Draw("HIST SAME");
  leg1->Draw("SAME");

  c->SaveAs(TString::Format("~/www/Plots/13TeV/plots80X_%s_v_%s.png",var1.Data(),var2.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV/plots80X_%s_v_%s.pdf",var1.Data(),var2.Data()));

}



