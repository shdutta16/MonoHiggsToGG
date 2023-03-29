#include <TFile.h>
#include <TH1.h>
#include <TH2.h>

using namespace std;

void makePrettyPlot(TFile*, TString, TString, TString); 

void plot2Defficiency()
{

  // ----------------------------------------------------------------
  // Get input and setup output root file 
  // ----------------------------------------------------------------
  TString indir  = "EffandYieldsTables/";
  TString outdir = "~/www/Plots/13TeV_v80X_moriond17/SignalInterpretation/";
  TFile *infile  = TFile::Open(Form("%sOutput_Eff.root",indir.Data()));

  // ----------------------------------------------------------------
  // Make the 2D plots pretty 
  // ----------------------------------------------------------------
  makePrettyPlot(infile,outdir,"2HDM","HighMET");
  makePrettyPlot(infile,outdir,"2HDM","LowMET");
  makePrettyPlot(infile,outdir,"BARY","HighMET");
  makePrettyPlot(infile,outdir,"BARY","LowMET");

}

void makePrettyPlot(TFile* infile, TString outdir, TString sig, TString met)
{

  // ----------------------------------------------------------------
  // Masses 
  // ----------------------------------------------------------------
  std::vector< Double_t > mass_x;
  std::vector< Double_t > mass_y;
  if (sig=="2HDM") mass_x = {600,800,1000,1200,1400,1700,2000,2500};
  if (sig=="2HDM") mass_y = {300,400,500,600,700,800};
  if (sig=="BARY") mass_x = {10,50,100,200,500,1000,2000,10000};
  if (sig=="BARY") mass_y = {1,10,50,150,500,1000};
  Int_t num_x = mass_x.size();
  Int_t num_y = mass_y.size();
 
  // ----------------------------------------------------------------
  // Set up histo 
  // ----------------------------------------------------------------
  TString histoname = Form("h_%s_Eff_%s",sig.Data(),met.Data());
  TH2D *h_eff_old = (TH2D*)infile->Get(Form("%s",histoname.Data())); 
  TH2D *h_eff = new TH2D(Form("h_eff_%s_%s",sig.Data(),met.Data()),"",num_x,0,num_x,num_y,0,num_y);

  // ----------------------------------------------------------------
  // Fill new histo 
  // ----------------------------------------------------------------
  Double_t bin_x;
  Double_t bin_y;
  Double_t eff;
  for (UInt_t mx = 0; mx < num_x; mx++){
    for (UInt_t my = 0; my < num_y; my++){
      bin_x = h_eff_old->GetXaxis()->FindBin(mass_x[mx]); 
      bin_y = h_eff_old->GetYaxis()->FindBin(mass_y[my]);
      eff   = h_eff_old->GetBinContent(bin_x,bin_y);
      h_eff->Fill(mx,my,eff);
      h_eff->GetXaxis()->SetBinLabel(mx+1,Form("%1.f",mass_x[mx]));
      h_eff->GetYaxis()->SetBinLabel(my+1,Form("%1.f",mass_y[my]));
    } 
  } 

  // ----------------------------------------------------------------
  // Styling 
  // ----------------------------------------------------------------
  h_eff->SetTitle("");
  h_eff->GetXaxis()->SetTitle("m_{Z'} [GeV]");
  if (sig=="2HDM") h_eff->GetYaxis()->SetTitle("m_{A} [GeV]"); 
  else             h_eff->GetYaxis()->SetTitle("m_{#chi} [GeV]"); 

  // ----------------------------------------------------------------
  // Save plot 
  // ----------------------------------------------------------------
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("1.1f");
  TCanvas *c = new TCanvas(); 
  h_eff->SetMarkerSize(2);
  h_eff->Draw("COLZ TEXT"); 
  c->SaveAs(Form("%sh_2Deff_%s_%s.png",outdir.Data(),sig.Data(),met.Data()));
  c->SaveAs(Form("%sh_2Deff_%s_%s.pdf",outdir.Data(),sig.Data(),met.Data()));
  
  delete c;
  delete h_eff;

}


