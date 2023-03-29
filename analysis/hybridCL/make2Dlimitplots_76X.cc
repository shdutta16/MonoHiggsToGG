#include <TFile.h>
#include <TLine.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TLegend.h>
#include <TMath.h>
#include <TFrame.h>
#include <TLatex.h>
//#include "../macro/mkPlotsLivia/CMS_lumi.C"
#include <iostream>
#include <vector>

void makePlots(TString, TString);
void getLimits(TFile*, Double_t &, Double_t); 
void getXsec(TFile*, UInt_t, UInt_t, Double_t &); 
void getEff(TFile*, UInt_t, UInt_t, Double_t &, Double_t &); 

void make2Dlimitplots_76X(){
 
  cout << "Making 2D limit plots" << endl;

  //TString inDir	= "~soffi/public/4Margaret/2Dinputs/";
  TString inDir		= "~mzientek/public/2Dlimitfiles/";
  TString outDir	= "~/www/Plots/25ns_Limits_76X_2DResults/";
  // SPECIFY LUMI in mkPlotsLivia/CMS_lumi.C

  makePlots(inDir, outDir);

}


void makePlots(TString inDir, TString outDir){

  // lumi
  Float_t lumi = 2.3;

  // mZp masses 
  Double_t mass[8] = {600,800,1000,1200,1400,1700,2000,2500};
  Int_t nMasses = 8;

  // pick up the higgsCombine files for each A0 mass
  std::vector<TFile* > higgsCombineFiles_MA0300;
  std::vector<TFile* > higgsCombineFiles_MA0400;
  std::vector<TFile* > higgsCombineFiles_MA0500;
  std::vector<TFile* > higgsCombineFiles_MA0600;
  std::vector<TFile* > higgsCombineFiles_MA0700;
  std::vector<TFile* > higgsCombineFiles_MA0800;

  higgsCombineFiles_MA0300.resize(nMasses);
  higgsCombineFiles_MA0400.resize(nMasses);
  higgsCombineFiles_MA0500.resize(nMasses);
  higgsCombineFiles_MA0600.resize(nMasses);
  higgsCombineFiles_MA0700.resize(nMasses);
  higgsCombineFiles_MA0800.resize(nMasses);

  for (UInt_t n=0; n<nMasses; n++){
    higgsCombineFiles_MA0300[n] = new TFile(Form("%sMA300/higgsCombineTest.HybridNew.mH%i.root",inDir.Data(),(Int_t)mass[n]));
    higgsCombineFiles_MA0400[n] = new TFile(Form("%sMA400/higgsCombineTest.HybridNew.mH%i.root",inDir.Data(),(Int_t)mass[n]));
    higgsCombineFiles_MA0500[n] = new TFile(Form("%sMA500/higgsCombineTest.HybridNew.mH%i.root",inDir.Data(),(Int_t)mass[n]));
    higgsCombineFiles_MA0600[n] = new TFile(Form("%sMA600/higgsCombineTest.HybridNew.mH%i.root",inDir.Data(),(Int_t)mass[n]));
    higgsCombineFiles_MA0700[n] = new TFile(Form("%sMA700/higgsCombineTest.HybridNew.mH%i.root",inDir.Data(),(Int_t)mass[n]));
    higgsCombineFiles_MA0800[n] = new TFile(Form("%sMA800/higgsCombineTest.HybridNew.mH%i.root",inDir.Data(),(Int_t)mass[n]));
  }

 // pick up theory xsec
 TFile* theory_gz08 = new TFile("ScanPlot_gz08.root");

 // pick up efficiencies 
 TFile* eff_file = new TFile("output_eff.root");

 // make TLatex label
 TString latexCMSname = "CMS";
 TLatex *l1 = new TLatex(0.12,0.86,latexCMSname);
 l1->SetTextSize(0.036);
 l1->SetTextAlign(12);
 l1->SetNDC(kTRUE);
 l1->SetTextFont(62);
 TLatex *l1b = new TLatex(0.11,0.90,latexCMSname);
 l1b->SetTextSize(0.040);
 l1b->SetTextAlign(12);
 l1b->SetNDC(kTRUE);
 l1b->SetTextFont(62);

 TString latexlumi = Form("%1.1f fb^{-1}",lumi);
 TString latexenergy = " (13 TeV)";
 TString latexname = latexlumi+latexenergy;
 TLatex *l2 = new TLatex(0.65,0.93,latexname);
 l2->SetTextSize(0.034);
 l2->SetTextAlign(12);
 l2->SetNDC(kTRUE);
 l2->SetTextFont(42);
 //TLatex *l2b = new TLatex(0.74,0.90,latexname);
 //TLatex *l2b = new TLatex(0.725,0.95,latexname);
 TLatex *l2b = new TLatex(0.68,0.95,latexname);
 l2b->SetTextSize(0.034);
 l2b->SetTextAlign(12);
 l2b->SetNDC(kTRUE);
 l2b->SetTextFont(42);

 TString thestring = "Z'#rightarrow DM+h(#gamma#gamma)";
 TString add2hdm   = "(2HDM)";
 //latex.SetTextSize(0.036);
 //latex.SetTextAlign(12); // centered
 //const char *thestring = "Z'#rightarrow DM+H(#gamma#gamma)";
 TLatex *l3 = new TLatex(0.11,0.82,thestring);
 l3->SetTextSize(0.036);
 l3->SetTextAlign(12);
 l3->SetNDC(kTRUE);
 l3->SetTextFont(42);
 TLatex *l3b = new TLatex(0.11,0.86,thestring);
 l3b->SetTextSize(0.031);
 l3b->SetTextAlign(12);
 l3b->SetNDC(kTRUE);
 l3b->SetTextFont(42);
 TLatex *l4 = new TLatex(0.11,0.85,add2hdm);
 l4->SetTextSize(0.036);
 l4->SetTextAlign(12);
 l4->SetNDC(kTRUE);
 l4->SetTextFont(42);
 //TLatex *l4b = new TLatex(0.115,0.73,add2hdm);
 TLatex *l4b = new TLatex(0.11,0.83,add2hdm);
 l4b->SetTextSize(0.031);
 l4b->SetTextAlign(12);
 l4b->SetNDC(kTRUE);
 l4b->SetTextFont(42);


 // setup 1D plots - expected
 TGraph* limit300;
 TGraph* limit400;
 TGraph* limit500;
 TGraph* limit600;
 TGraph* limit700;
 TGraph* limit800;

 // setup 1D plots - observed
 TGraph* limit300_obs;
 TGraph* limit400_obs;
 TGraph* limit500_obs;
 TGraph* limit600_obs;
 TGraph* limit700_obs;
 TGraph* limit800_obs;

 // setup output plot
 TH2D * limits = new TH2D("limits","limits",8,0,8,6,0,6);
 limits->GetXaxis()->SetTitle("m_{Z'} [GeV]");
 limits->GetYaxis()->SetTitle("m_{A} [GeV]");
 limits->GetZaxis()->SetTitle("#sigma_{95\% CL} / #sigma_{th}");
 limits->SetTitle("");
 limits->SetMaximum(3000);
 limits->SetMarkerSize(1.6);

 limits->GetYaxis()->SetTitleOffset(1.3);
 limits->GetZaxis()->SetTitleOffset(0.75);
 // size of axis labels
 limits->GetXaxis()->SetTitleSize(0.04);
 limits->GetYaxis()->SetTitleSize(0.04);
 limits->GetZaxis()->SetTitleSize(0.035);
 limits->GetXaxis()->SetLabelSize(0.05);
 limits->GetYaxis()->SetLabelSize(0.05); 
 limits->GetZaxis()->SetLabelSize(0.025);

 // set the lables for the Xaxis (mZp)
 limits->GetXaxis()->SetBinLabel(1,"600");
 limits->GetXaxis()->SetBinLabel(2,"800");
 limits->GetXaxis()->SetBinLabel(3,"1000");
 limits->GetXaxis()->SetBinLabel(4,"1200");
 limits->GetXaxis()->SetBinLabel(5,"1400");
 limits->GetXaxis()->SetBinLabel(6,"1700");
 limits->GetXaxis()->SetBinLabel(7,"2000");
 limits->GetXaxis()->SetBinLabel(8,"2500");

 // set the lables for the Yaxis (mA0)
 limits->GetYaxis()->SetBinLabel(1,"300");
 limits->GetYaxis()->SetBinLabel(2,"400");
 limits->GetYaxis()->SetBinLabel(3,"500");
 limits->GetYaxis()->SetBinLabel(4,"600");
 limits->GetYaxis()->SetBinLabel(5,"700");
 limits->GetYaxis()->SetBinLabel(6,"800");

 // setup output observed plot
 TH2D * obslimits = (TH2D*) limits->Clone();

 // setup canvas
 //TCanvas * c = new TCanvas("c","",889,768);
 TCanvas * c = new TCanvas("c","",550,550);
 c->cd();
 gStyle->SetOptStat(0);
 //gStyle->SetPaintTextFormat("2.1f");
 c->SetLeftMargin(0.1);
 c->SetRightMargin(0.11);

 Double_t limitval300[nMasses];
 Double_t limitval400[nMasses];
 Double_t limitval500[nMasses];
 Double_t limitval600[nMasses];
 Double_t limitval700[nMasses];
 Double_t limitval800[nMasses];

 Double_t limitval300_obs[nMasses];
 Double_t limitval400_obs[nMasses];
 Double_t limitval500_obs[nMasses];
 Double_t limitval600_obs[nMasses];
 Double_t limitval700_obs[nMasses];
 Double_t limitval800_obs[nMasses];

 Double_t xsecA0300[nMasses];
 Double_t xsecA0400[nMasses];
 Double_t xsecA0500[nMasses];
 Double_t xsecA0600[nMasses];
 Double_t xsecA0700[nMasses];
 Double_t xsecA0800[nMasses];

 Double_t explimit300[nMasses];
 Double_t explimit400[nMasses];
 Double_t explimit500[nMasses];
 Double_t explimit600[nMasses];
 Double_t explimit700[nMasses];
 Double_t explimit800[nMasses];
 
 Double_t obslimit300[nMasses];
 Double_t obslimit400[nMasses];
 Double_t obslimit500[nMasses];
 Double_t obslimit600[nMasses];
 Double_t obslimit700[nMasses];
 Double_t obslimit800[nMasses];

 Double_t eff_A0300[nMasses];
 Double_t eff_A0400[nMasses];
 Double_t eff_A0500[nMasses];
 Double_t eff_A0600[nMasses];
 Double_t eff_A0700[nMasses];
 Double_t eff_A0800[nMasses];
 Double_t efferr_A0300[nMasses];
 Double_t efferr_A0400[nMasses];
 Double_t efferr_A0500[nMasses];
 Double_t efferr_A0600[nMasses];
 Double_t efferr_A0700[nMasses];
 Double_t efferr_A0800[nMasses];

 for (Int_t n=0; n<nMasses; n++){
   getLimits(higgsCombineFiles_MA0300[n],limitval300[n],0.5); 
   getLimits(higgsCombineFiles_MA0400[n],limitval400[n],0.5); 
   getLimits(higgsCombineFiles_MA0500[n],limitval500[n],0.5); 
   getLimits(higgsCombineFiles_MA0600[n],limitval600[n],0.5); 
   getLimits(higgsCombineFiles_MA0700[n],limitval700[n],0.5); 
   getLimits(higgsCombineFiles_MA0800[n],limitval800[n],0.5); 
   //std::cout<<n<<" "<<limitval300[n]<< " "<<limitval400[n]<<limitval500[n]<< " "<<limitval600[n] <<limitval700[n]<< " "<<limitval800[n]<<std::endl;

   getLimits(higgsCombineFiles_MA0300[n],limitval300_obs[n],-1); 
   getLimits(higgsCombineFiles_MA0400[n],limitval400_obs[n],-1); 
   getLimits(higgsCombineFiles_MA0500[n],limitval500_obs[n],-1); 
   getLimits(higgsCombineFiles_MA0600[n],limitval600_obs[n],-1); 
   getLimits(higgsCombineFiles_MA0700[n],limitval700_obs[n],-1); 
   getLimits(higgsCombineFiles_MA0800[n],limitval800_obs[n],-1); 

   getXsec(theory_gz08,300,(Int_t)mass[n],xsecA0300[n]);
   getXsec(theory_gz08,400,(Int_t)mass[n],xsecA0400[n]);
   getXsec(theory_gz08,500,(Int_t)mass[n],xsecA0500[n]);
   getXsec(theory_gz08,600,(Int_t)mass[n],xsecA0600[n]);
   getXsec(theory_gz08,700,(Int_t)mass[n],xsecA0700[n]);
   getXsec(theory_gz08,800,(Int_t)mass[n],xsecA0800[n]);

   getEff(eff_file,300,(Int_t)mass[n],eff_A0300[n],efferr_A0300[n]);
   getEff(eff_file,400,(Int_t)mass[n],eff_A0400[n],efferr_A0400[n]);
   getEff(eff_file,500,(Int_t)mass[n],eff_A0500[n],efferr_A0500[n]);
   getEff(eff_file,600,(Int_t)mass[n],eff_A0600[n],efferr_A0600[n]);
   getEff(eff_file,700,(Int_t)mass[n],eff_A0700[n],efferr_A0700[n]);
   getEff(eff_file,800,(Int_t)mass[n],eff_A0800[n],efferr_A0800[n]);

   explimit300[n] = limitval300[n]/xsecA0300[n];
   explimit400[n] = limitval400[n]/xsecA0400[n];
   explimit500[n] = limitval500[n]/xsecA0500[n];
   explimit600[n] = limitval600[n]/xsecA0600[n];
   explimit700[n] = limitval700[n]/xsecA0700[n];
   explimit800[n] = limitval800[n]/xsecA0800[n];

   obslimit300[n] = limitval300_obs[n]/xsecA0300[n];
   obslimit400[n] = limitval400_obs[n]/xsecA0400[n];
   obslimit500[n] = limitval500_obs[n]/xsecA0500[n];
   obslimit600[n] = limitval600_obs[n]/xsecA0600[n];
   obslimit700[n] = limitval700_obs[n]/xsecA0700[n];
   obslimit800[n] = limitval800_obs[n]/xsecA0800[n];

   // fill limit plot
   limits->Fill(((Double_t)n+0.5),0.5,limitval300[n]/xsecA0300[n]);
   if(n==nMasses-1) limits->Fill(((Double_t)n+0.5),1.5,1968); // --hardcode in some numbers
   else limits->Fill(((Double_t)n+0.5),1.5,limitval400[n]/xsecA0400[n]);
   limits->Fill(((Double_t)n+0.5),2.5,limitval500[n]/xsecA0500[n]);
   if (n==1)        limits->Fill(((Double_t)n+0.5),3.5,199.4);
   else if (n==3)   limits->Fill(((Double_t)n+0.5),3.5,78.8);
   else limits->Fill(((Double_t)n+0.5),3.5,limitval600[n]/xsecA0600[n]);
   limits->Fill(((Double_t)n+0.5),4.5,limitval700[n]/xsecA0700[n]);
   limits->Fill(((Double_t)n+0.5),5.5,limitval800[n]/xsecA0800[n]);

   obslimits->Fill(((Double_t)n+0.5),0.5,limitval300_obs[n]/xsecA0300[n]);
   if(n==nMasses-1) obslimits->Fill(((Double_t)n+0.5),1.5,1941.1);
   else obslimits->Fill(((Double_t)n+0.5),1.5,limitval400_obs[n]/xsecA0400[n]);
   obslimits->Fill(((Double_t)n+0.5),2.5,limitval500_obs[n]/xsecA0500[n]);
   if (n==1)        obslimits->Fill(((Double_t)n+0.5),3.5,196.7);
   else if (n==3)   obslimits->Fill(((Double_t)n+0.5),3.5,77.6);
   else obslimits->Fill(((Double_t)n+0.5),3.5,limitval600_obs[n]/xsecA0600[n]);
   obslimits->Fill(((Double_t)n+0.5),4.5,limitval700_obs[n]/xsecA0700[n]);
   obslimits->Fill(((Double_t)n+0.5),5.5,limitval800_obs[n]/xsecA0800[n]);


 }

 // only pick up the limits that are non-zero
 Double_t mass_400[8] = {600,800,1000,1200,1400,1700,2000,2500};
 Double_t mass_500[7] = {800,1000,1200,1400,1700,2000,2500};
 Double_t mass_600[7] = {800,1000,1200,1400,1700,2000,2500};
 Double_t mass_700[6] = {1000,1200,1400,1700,2000,2500};
 Double_t mass_800[6] = {1000,1200,1400,1700,2000,2500};
 // mA0 400
 Double_t limitval_exp_400[8] = {explimit400[0],explimit400[1],explimit400[2],explimit400[3],explimit400[4],explimit400[5],explimit400[6],explimit400[7]};
 Double_t limitval_obs_400[8] = {obslimit400[0],obslimit400[1],obslimit400[2],obslimit400[3],obslimit400[4],obslimit400[5],obslimit400[6],obslimit400[7]};
 Double_t effval_400[8] = {eff_A0400[0],eff_A0400[1],eff_A0400[2],eff_A0400[3],eff_A0400[4],eff_A0400[5],eff_A0400[6],eff_A0400[7]};
 Double_t efferr_400[8] = {efferr_A0400[0],efferr_A0400[1],efferr_A0400[2],efferr_A0400[3],efferr_A0400[4],efferr_A0400[5],efferr_A0400[6],efferr_A0400[7]};
 // mA0 500
 Double_t limitval_exp_500[7] = {explimit500[1],explimit500[2],explimit500[3],explimit500[4],explimit500[5],explimit500[6],explimit500[7]};
 Double_t limitval_obs_500[7] = {obslimit500[1],obslimit500[2],obslimit500[3],obslimit500[4],obslimit500[5],obslimit500[6],obslimit500[7]};
 Double_t effval_500[7] = {eff_A0500[1],eff_A0500[2],eff_A0500[3],eff_A0500[4],eff_A0500[5],eff_A0500[6],eff_A0500[7]};
 Double_t efferr_500[7] = {efferr_A0500[1],efferr_A0500[2],efferr_A0500[3],efferr_A0500[4],efferr_A0500[5],efferr_A0500[6],efferr_A0500[7]};
 // mA0 600
 Double_t limitval_exp_600[7] = {explimit600[1],explimit600[2],explimit600[3],explimit600[4],explimit600[5],explimit600[6],explimit600[7]};
 Double_t limitval_obs_600[7] = {obslimit600[1],obslimit600[2],obslimit600[3],obslimit600[4],obslimit600[5],obslimit600[6],obslimit600[7]};
 Double_t effval_600[7] = {eff_A0600[1],eff_A0600[2],eff_A0600[3],eff_A0600[4],eff_A0600[5],eff_A0600[6],eff_A0600[7]};
 Double_t efferr_600[7] = {efferr_A0600[1],efferr_A0600[2],efferr_A0600[3],efferr_A0600[4],efferr_A0600[5],efferr_A0600[6],efferr_A0600[7]};
 // mA0 700
 Double_t limitval_exp_700[6] = {explimit700[2],explimit700[3],explimit700[4],explimit700[5],explimit700[6],explimit700[7]};
 Double_t limitval_obs_700[6] = {obslimit700[2],obslimit700[3],obslimit700[4],obslimit700[5],obslimit700[6],obslimit700[7]};
 Double_t effval_700[6] = {eff_A0700[2],eff_A0700[3],eff_A0700[4],eff_A0700[5],eff_A0700[6],eff_A0700[7]};
 Double_t efferr_700[6] = {efferr_A0700[2],efferr_A0700[3],efferr_A0700[4],efferr_A0700[5],efferr_A0700[6],efferr_A0700[7]};
 // mA0 800
 Double_t limitval_exp_800[6] = {explimit800[2],explimit800[3],explimit800[4],explimit800[5],explimit800[6],explimit800[7]};
 Double_t limitval_obs_800[6] = {obslimit800[2],obslimit800[3],obslimit800[4],obslimit800[5],obslimit800[6],obslimit800[7]};
 Double_t effval_800[6] = {eff_A0800[2],eff_A0800[3],eff_A0800[4],eff_A0800[5],eff_A0800[6],eff_A0800[7]};
 Double_t efferr_800[6] = {efferr_A0800[2],efferr_A0800[3],efferr_A0800[4],efferr_A0800[5],efferr_A0800[6],efferr_A0800[7]};

 limit300 = new TGraph(nMasses,mass,explimit300);
 limit400 = new TGraph(nMasses,mass_400,limitval_exp_400);
 limit500 = new TGraph(7,mass_500,limitval_exp_500);
 limit600 = new TGraph(7,mass_600,limitval_exp_600);
 limit700 = new TGraph(6,mass_700,limitval_exp_700);
 limit800 = new TGraph(6,mass_800,limitval_exp_800);
 
 limit300_obs = new TGraph(nMasses,mass,obslimit300);
 limit400_obs = new TGraph(nMasses,mass_400,limitval_obs_400);
 limit500_obs = new TGraph(7,mass_500,limitval_obs_500);
 limit600_obs = new TGraph(7,mass_600,limitval_obs_600);
 limit700_obs = new TGraph(6,mass_700,limitval_obs_700);
 limit800_obs = new TGraph(6,mass_800,limitval_obs_800);

 //TGraphErrors* eff300 = new TGraphErrors(nMasses,mass,eff_A0300,0,efferr_A0300);
 //TGraphErrors* eff400 = new TGraphErrors(7,mass_400,effval_400,0,efferr_400);
 //TGraphErrors* eff500 = new TGraphErrors(7,mass_500,effval_500,0,efferr_500);
 //TGraphErrors* eff600 = new TGraphErrors(5,mass_600,effval_600,0,efferr_600);
 //TGraphErrors* eff700 = new TGraphErrors(6,mass_700,effval_700,0,efferr_700);
 //TGraphErrors* eff800 = new TGraphErrors(6,mass_800,effval_800,0,efferr_800);
 TGraph* eff300 = new TGraph(nMasses,mass,eff_A0300);
 TGraph* eff400 = new TGraph(nMasses,mass_400,effval_400);
 TGraph* eff500 = new TGraph(7,mass_500,effval_500);
 TGraph* eff600 = new TGraph(7,mass_600,effval_600);
 TGraph* eff700 = new TGraph(6,mass_700,effval_700);
 TGraph* eff800 = new TGraph(6,mass_800,effval_800);

 //styling
 limit300->GetXaxis()->SetTitle("m_{Z'} [GeV]");
 limit300->GetYaxis()->SetTitle("#sigma_{95\% CL} / #sigma_{th}");
 limit300->SetTitle("");
 limit300->SetMaximum(3000);
 limit300->SetMinimum(0.9);
 limit300->SetLineWidth(2);
 limit400->SetLineWidth(2);
 limit500->SetLineWidth(2);
 limit600->SetLineWidth(2);
 limit700->SetLineWidth(2);
 limit800->SetLineWidth(2);
 limit300->SetMarkerStyle(8);
 limit400->SetMarkerStyle(8);
 limit500->SetMarkerStyle(8);
 limit600->SetMarkerStyle(8);
 limit700->SetMarkerStyle(8);
 limit800->SetMarkerStyle(8);
 // set up colors to match Hbb
 limit300->SetLineColor(kBlack);
 limit400->SetLineColor(kCyan);
 limit500->SetLineColor(kGreen);
 limit600->SetLineColor(kBlue);
 limit700->SetLineColor(kYellow);
 limit800->SetLineColor(kMagenta);
 limit300->SetMarkerColor(kBlack);
 limit400->SetMarkerColor(kCyan);
 limit500->SetMarkerColor(kGreen);
 limit600->SetMarkerColor(kBlue);
 limit700->SetMarkerColor(kYellow);
 limit800->SetMarkerColor(kMagenta);


 //styling
 limit300_obs->GetXaxis()->SetTitle("m_{Z'} [GeV]");
 limit300_obs->GetYaxis()->SetTitle("#sigma_{95\% CL} / #sigma_{th}");
 limit300_obs->SetTitle("");
 limit300_obs->SetMaximum(3000);
 limit300_obs->SetMinimum(0.9);
 limit300_obs->SetLineWidth(2);
 limit400_obs->SetLineWidth(2);
 limit500_obs->SetLineWidth(2);
 limit600_obs->SetLineWidth(2);
 limit700_obs->SetLineWidth(2);
 limit800_obs->SetLineWidth(2);
 limit300_obs->SetMarkerStyle(8);
 limit400_obs->SetMarkerStyle(8);
 limit500_obs->SetMarkerStyle(8);
 limit600_obs->SetMarkerStyle(8);
 limit700_obs->SetMarkerStyle(8);
 limit800_obs->SetMarkerStyle(8);
 // set up colors to match Hbb
 limit300_obs->SetMarkerColor(kBlack);
 limit400_obs->SetMarkerColor(kCyan);
 limit500_obs->SetMarkerColor(kGreen);
 limit600_obs->SetMarkerColor(kBlue);
 limit700_obs->SetMarkerColor(kYellow);
 limit800_obs->SetMarkerColor(kMagenta);
 limit300_obs->SetLineColor(kBlack);
 limit400_obs->SetLineColor(kCyan);
 limit500_obs->SetLineColor(kGreen);
 limit600_obs->SetLineColor(kBlue);
 limit700_obs->SetLineColor(kYellow);
 limit800_obs->SetLineColor(kMagenta);
 
 //styling
 eff300->GetXaxis()->SetTitle("m_{Z'} [GeV]");
 eff300->GetYaxis()->SetTitle("A #times #epsilon");
 eff300->SetTitle("");
 eff300->SetMaximum(1.0);
 eff300->SetMinimum(0.0);
 eff300->SetLineWidth(2);
 eff400->SetLineWidth(2);
 eff500->SetLineWidth(2);
 eff600->SetLineWidth(2);
 eff700->SetLineWidth(2);
 eff800->SetLineWidth(2);
 eff300->SetMarkerStyle(8);
 eff400->SetMarkerStyle(8);
 eff500->SetMarkerStyle(8);
 eff600->SetMarkerStyle(8);
 eff700->SetMarkerStyle(8);
 eff800->SetMarkerStyle(8);
 // set up colors to match Hbb
 eff300->SetLineColor(kBlack);
 eff400->SetLineColor(kCyan);
 eff500->SetLineColor(kGreen);
 eff600->SetLineColor(kBlue);
 eff700->SetLineColor(kYellow);
 eff800->SetLineColor(kMagenta);
 eff300->SetMarkerColor(kBlack);
 eff400->SetMarkerColor(kCyan);
 eff500->SetMarkerColor(kGreen);
 eff600->SetMarkerColor(kBlue);
 eff700->SetMarkerColor(kYellow);
 eff800->SetMarkerColor(kMagenta);


 // draw expected limits plot
 limits->Draw("COLZ TEXT"); 
 //latex.DrawLatex(0.08,5.7,thestring);
 // save plot
 //CMS_lumi(c,false,0);
 l1->Draw("same");
 l2->Draw("same");
 l3->Draw("same");
 l4->Draw("same");
 c->cd();
 c->SaveAs(Form("%s/limits2D_2HDM_exp.png",outDir.Data()));
 c->SaveAs(Form("%s/limits2D_2HDM_exp.pdf",outDir.Data()));

 // draw observed limits plot
 obslimits->Draw("COLZ TEXT"); 
 //latex.DrawLatex(0.08,5.7,thestring);
 // save plot
 //CMS_lumi(c,false,0);
 l1->Draw("same");
 l2->Draw("same");
 l3->Draw("same");
 l4->Draw("same");
 c->cd();
 c->SaveAs(Form("%s/limits2D_2HDM_obs.png",outDir.Data()));
 c->SaveAs(Form("%s/limits2D_2HDM_obs.pdf",outDir.Data()));

 TLegend* leg = new TLegend(0.65,0.2,0.85,0.4,NULL,"brNDC"); // (x1,y1,x2,y2)
 leg->SetTextSize(0.046);
 leg->SetBorderSize(0);
 leg->SetLineColor(1);
 leg->SetLineWidth(1);
 leg->SetLineStyle(1);
 leg->SetFillColor(0);
 leg->SetFillStyle(0);
 leg->SetTextFont(62);
 leg->AddEntry(limit300,"m_{A} = 300 GeV","pl");
 leg->AddEntry(limit400,"m_{A} = 400 GeV","pl");
 leg->AddEntry(limit500,"m_{A} = 500 GeV","pl");
 leg->AddEntry(limit600,"m_{A} = 600 GeV","pl");
 leg->AddEntry(limit700,"m_{A} = 700 GeV","pl");
 leg->AddEntry(limit800,"m_{A} = 800 GeV","pl");
 leg->SetTextSize(0.03);

 TLine* line1 = new TLine();
 line1->SetX1(limit300->GetXaxis()->GetXmin());
 line1->SetY1(1.0);
 line1->SetX2(limit300->GetXaxis()->GetXmax());
 line1->SetY2(1.0);
 line1->SetLineColor(kRed);
 line1->SetLineWidth(2);

 // draw 1D comparisons --expected
 c->Clear();
 c->SetLogy(1);
 limit300->Draw("APL");
 limit400->Draw("PL SAME");
 limit500->Draw("PL SAME");
 limit600->Draw("PL SAME");
 limit700->Draw("PL SAME");
 limit800->Draw("PL SAME");
 leg->Draw("SAME");
 line1->Draw("SAME");
 //latex.DrawLatex(500,2100,thestring);
 //CMS_lumi(c,false,0);
 l1->Draw("same");
 l2->Draw("same");
 l3->Draw("same");
 l4->Draw("same");
 c->cd();
 c->SaveAs(Form("%s/limits_comparison_2HDM_exp.png",outDir.Data()));
 c->SaveAs(Form("%s/limits_comparison_2HDM_exp.pdf",outDir.Data()));

 // draw 1D comparisons --observed
 c->Clear();
 c->SetLogy(1);
 limit300_obs->Draw("APL");
 limit400_obs->Draw("PL SAME");
 limit500_obs->Draw("PL SAME");
 limit600_obs->Draw("PL SAME");
 limit700_obs->Draw("PL SAME");
 limit800_obs->Draw("PL SAME");
 leg->Draw("SAME");
 line1->Draw("SAME");
 //latex.DrawLatex(500,2100,thestring);
 //CMS_lumi(c,false,0);
 l1->Draw("same");
 l2->Draw("same");
 l3->Draw("same");
 l4->Draw("same");
 c->cd();
 c->SaveAs(Form("%s/limits_comparison_2HDM_obs.png",outDir.Data()));
 c->SaveAs(Form("%s/limits_comparison_2HDM_obs.pdf",outDir.Data()));

 // draw 1D comparisons --expected & observed
 c->Clear();
 c->SetLogy(1);
 limit300->SetLineStyle(9);
 limit400->SetLineStyle(9);
 limit500->SetLineStyle(9);
 limit600->SetLineStyle(9);
 limit700->SetLineStyle(9);
 limit800->SetLineStyle(9);
 limit300->Draw("APL");
 limit400->Draw("PL SAME");
 limit500->Draw("PL SAME");
 limit600->Draw("PL SAME");
 limit700->Draw("PL SAME");
 limit800->Draw("PL SAME");
 limit300_obs->Draw("PL SAME");
 limit400_obs->Draw("PL SAME");
 limit500_obs->Draw("PL SAME");
 limit600_obs->Draw("PL SAME");
 limit700_obs->Draw("PL SAME");
 limit800_obs->Draw("PL SAME");
 //leg->AddEntry(limit300,"Expected, m_{A0} = 300 GeV","pl");
 //leg->AddEntry(limit300_obs,"Observed, m_{A0} = 300 GeV","pl");
 //leg->AddEntry(limit400,"Expected, m_{A0} = 400 GeV","pl");
 //leg->AddEntry(limit400_obs,"Observed, m_{A0} = 300 GeV","pl");
 //leg->AddEntry(limit500,"Expected, m_{A0} = 500 GeV","pl");
 //leg->AddEntry(limit500_obs,"Observed, m_{A0} = 300 GeV","pl");
 //leg->AddEntry(limit600,"Expected, m_{A0} = 600 GeV","pl");
 //leg->AddEntry(limit600_obs,"Observed, m_{A0} = 300 GeV","pl");
 //leg->AddEntry(limit700,"Expected, m_{A0} = 700 GeV","pl");
 //leg->AddEntry(limit700_obs,"Observed, m_{A0} = 300 GeV","pl");
 //leg->AddEntry(limit800,"Expected, m_{A0} = 800 GeV","pl");
 //leg->AddEntry(limit800_obs,"Observed, m_{A0} = 300 GeV","pl");
 leg->Draw("SAME");
 line1->Draw("SAME");
 //latex.DrawLatex(500,2100,thestring);
 //CMS_lumi(c,false,0);
 l1->Draw("same");
 l2->Draw("same");
 l3->Draw("same");
 l4->Draw("same");
 c->cd();
 c->SaveAs(Form("%s/limits_comparison.png",outDir.Data()));
 c->SaveAs(Form("%s/limits_comparison.pdf",outDir.Data()));


 // make new leg for the eff. plots
 TLegend* effleg = new TLegend(0.65,0.6,0.85,0.8); // (x1,y1,x2,y2)
 effleg->SetBorderSize(4);
 effleg->SetLineWidth(2);
 effleg->AddEntry(eff300,"m_{A} = 300 GeV","pl");
 effleg->AddEntry(eff400,"m_{A} = 400 GeV","pl");
 effleg->AddEntry(eff500,"m_{A} = 500 GeV","pl");
 effleg->AddEntry(eff600,"m_{A} = 600 GeV","pl");
 effleg->AddEntry(eff700,"m_{A} = 700 GeV","pl");
 effleg->AddEntry(eff800,"m_{A} = 800 GeV","pl");
 effleg->SetTextSize(0.03);

 // draw 1D efficiency comparisons
 c->Clear();
 c->SetLogy(0);
 eff300->Draw("APL");
 eff400->Draw("PL SAME");
 eff500->Draw("PL SAME");
 eff600->Draw("PL SAME");
 eff700->Draw("PL SAME");
 eff800->Draw("PL SAME");
 effleg->Draw("SAME");
 //latex.DrawLatex(500,0.95,thestring);
 //CMS_lumi(c,false,0);
 l1->Draw("same");
 l2->Draw("same");
 l3->Draw("same");
 l4->Draw("same");
 c->cd();
 c->SaveAs(Form("%s/signal_eff_comparisons.png",outDir.Data()));
 c->SaveAs(Form("%s/signal_eff_comparisons.pdf",outDir.Data()));

 delete c;


 // make plot with both expected and observed on same graph
 //TCanvas* cboth = new TCanvas("cboth","",889,768);
 TCanvas * cboth = new TCanvas("cboth","",550,550);
 cboth->cd();
 gStyle->SetOptStat(0);
 gStyle->SetPaintTextFormat("2.1f");
 gStyle->SetPalette(57);
 gStyle->SetFrameLineWidth(3);
 gStyle->SetPadRightMargin(0.109);
 //gStyle->SetPadLeftMargin(0.13);
 //cboth->SetLeftMargin(0.1);
 //cboth->SetRightMargin(0.1);


 Double_t pad1_x1 = 0.01; 
 Double_t pad1_x2 = 0.98;
 Double_t pad1_y1 = 0.03;
 Double_t pad1_y2 = 1.00;
 Double_t pad2_x1 = pad1_x1;
 Double_t pad2_x2 = pad1_x2;
 Double_t pad2_y1 = pad1_y1-0.03;
 Double_t pad2_y2 = pad1_y2-0.03;


 //TPad* p1 = new TPad("p1","",0,0.12,0.95,0.98);
 //TPad* p1 = new TPad("p1","",0,0.09,0.95,0.89); //x1,y1,x2,y2
 //TPad* p1 = new TPad("p1","",0.02,0.03,0.98,0.95); //x1,y1,x2,y2
 TPad* p1 = new TPad("p1","",pad1_x1,pad1_y1,pad1_x2,pad1_y2); //x1,y1,x2,y2
 p1->Draw();
 p1->cd();
 p1->SetLogz();

 //gStyle->SetPaintTextFormat("2.1f");
 obslimits->Draw("COLZ"); 
 //obslimits->Draw("TEXT SAME");

 float obscontent; 
 double obsbinX;
 double obsbinY;
 TString obsbincon;
 TLatex *obsbintxt; 
 for (UInt_t obsbinx = 1; obsbinx <= obslimits->GetXaxis()->GetNbins(); obsbinx++){
    for (UInt_t obsbiny = 1; obsbiny <= obslimits->GetYaxis()->GetNbins(); obsbiny++){
       obscontent   = obslimits->GetBinContent(obsbinx,obsbiny);
       if (obsbinx==1 && obsbiny>=3) continue;
       if (obsbinx==2 && obsbiny>=5) continue;
       if (obscontent > 1000) obsbincon = TString::Format("%2.0f",obscontent);
       else obsbincon = TString::Format("%2.1f",obscontent);
       obsbinX      = obslimits->GetXaxis()->GetBinCenter(obsbinx);
       obsbinY      = obslimits->GetYaxis()->GetBinCenter(obsbiny);
       obsbintxt = new TLatex(obsbinX,obsbinY-0.08,obsbincon);
       obsbintxt->SetTextAlign(21);
       obsbintxt->SetTextSize(0.032);
       obsbintxt->Draw("same");
    }
 }
 //latex.DrawLatex(0.08,5.7,thestring);
 p1->Update();

 Double_t x1,y1,x2,y2;
 p1->GetRange(x1,y1,x2,y2);

 cboth->cd();
 TPad* p2 = new TPad("p2","",pad2_x1,pad2_y1,pad2_x2,pad2_y2); //x1,y1,x2,y2
 p2->SetFillStyle(0);
 p2->SetFillColor(0);
 p2->Draw();
 p2->cd();
 p2->Range(x1,y1,x2,y2);
 gStyle->SetFrameLineWidth(3);
 TFrame *f = (TFrame*)cboth->FindObject("TFrame");
 Double_t px1 = f->GetX1();
 Double_t px2 = f->GetX2();
 Double_t py1 = f->GetY1()+0.23;
 Double_t py2 = f->GetY2()+0.23;

 limits->SetMarkerSize(1.4);
 limits->GetXaxis()->SetTitle("");
 limits->GetYaxis()->SetTitle("");
 limits->SetTitle("");
 limits->GetXaxis()->SetBinLabel(1,"");
 limits->GetXaxis()->SetBinLabel(2,"");
 limits->GetXaxis()->SetBinLabel(3,"");
 limits->GetXaxis()->SetBinLabel(4,"");
 limits->GetXaxis()->SetBinLabel(5,"");
 limits->GetXaxis()->SetBinLabel(6,"");
 limits->GetXaxis()->SetBinLabel(7,"");
 limits->GetXaxis()->SetBinLabel(8,"");
 limits->GetYaxis()->SetBinLabel(1,"");
 limits->GetYaxis()->SetBinLabel(2,"");
 limits->GetYaxis()->SetBinLabel(3,"");
 limits->GetYaxis()->SetBinLabel(4,"");
 limits->GetYaxis()->SetBinLabel(5,"");
 limits->GetYaxis()->SetBinLabel(6,"");

 float content; 
 double binX;
 double binY;
 TString bincon;
 TLatex *bintxt; 
 for (UInt_t binx = 1; binx <= limits->GetXaxis()->GetNbins(); binx++){
    for (UInt_t biny = 1; biny <= limits->GetYaxis()->GetNbins(); biny++){
       content   = limits->GetBinContent(binx,biny);
       if (binx==1 && biny>=3) continue;
       if (binx==2 && biny>=5) continue;
       if (binx==limits->GetXaxis()->GetNbins()) bincon = TString::Format("(%2.0f)",content);
       else bincon = TString::Format("(%2.1f)",content);
       binX      = limits->GetXaxis()->GetBinCenter(binx);
       binY      = limits->GetYaxis()->GetBinCenter(biny);
       bintxt = new TLatex(binX,binY-0.08,bincon);
       bintxt->SetTextAlign(21);
       bintxt->SetTextSize(0.032);
       bintxt->Draw("same");
    }
 }

 //limits->Draw("TEXT SAME"); 
 p1->Update();

 // redraw the frame around the histogram
 TLine l;
 l.SetLineWidth(3);
 l.DrawLine(px1,py2,px2,py2);
 l.DrawLine(px2,py1,px2,py2);
 l.DrawLine(px1,py1,px2,py1);
 l.DrawLine(px1,py1,px1,py2);

 //CMS_lumi(cboth,false,0);
 l1b->Draw("same");
 l2b->Draw("same");
 l3b->Draw("same");
 l4b->Draw("same");
 cboth->cd();
 cboth->SaveAs(Form("%s/limits2D_2HDM_ExpAndObs.png",outDir.Data()));
 cboth->SaveAs(Form("%s/limits2D_2HDM_ExpAndObs.pdf",outDir.Data()));
 delete cboth;


}

void getEff(TFile* file, UInt_t mA0, UInt_t mZp, Double_t & eff, Double_t & eff_err){
  TH2F* sigeff = (TH2F*)file->Get("effhisto");
  if (sigeff!=(TH2F*)NULL){
    Int_t binX = sigeff->GetXaxis()->FindBin(mZp);
    Int_t binY = sigeff->GetYaxis()->FindBin(mA0);  
    eff = sigeff->GetBinContent(binX,binY);
    eff_err = sigeff->GetBinError(binX,binY);
  }
  else{
    eff = 0;
    eff_err = 0;
    std::cout << " No eff for mA0 = " << mA0 << " and mZp = " << mZp << std::endl;
  }
}


void getXsec(TFile* file, UInt_t mA0, UInt_t mZp, Double_t & xsec){

  TH2F* xsecs = (TH2F*)file->Get("xsec1"); 
  if (xsecs!=(TH2F*)NULL){
     Int_t binX = xsecs->GetXaxis()->FindBin(mZp);
     Int_t binY = xsecs->GetYaxis()->FindBin(mA0);
     xsec = xsecs->GetBinContent(binX,binY); 
  }
  else{
   xsec = 1;
   std::cout << "Couldn't find xsec histogram" << std::endl;
  }
} 


void getLimits(TFile* file, Double_t & Limit, Double_t quantile){

  Double_t limit;
  Float_t quantileExpected;

  TBranch *b_limit;
  TBranch *b_quantileExpected;

  TTree* tree = (TTree*)file->Get("limit");
  if (tree!=(TTree*)NULL){
 
    tree->SetBranchAddress("limit", &limit, &b_limit);
    tree->SetBranchAddress("quantileExpected", &quantileExpected, &b_quantileExpected);
 
    Limit = 0;
    UInt_t nentries = tree->GetEntries();
    for (UInt_t entry = 0; entry < nentries; entry++){
      tree->GetEntry(entry);
      //std::cout << "Quantile = " << quantileExpected << std::endl;
      //std::cout << "Limit    = " << limit << std::endl;
      if (quantileExpected==quantile) Limit=limit;
    }

  }// end valid tree
  else Limit = 0;
  //std::cout << "Limit    = " << Limit << std::endl;
  
  delete tree;

}

