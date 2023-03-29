#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>

using namespace std;

void	Get1DEfficiency(TFile*, TString, TFile*, TString, Bool_t, Bool_t, TString);

void makeEfficiencyFit()
{

  // ----------------------------------------------------------------
  // Get input and setup output root file 
  // ----------------------------------------------------------------
  TString indir  = "EffandYieldsTables/";
  TString outdir = "~/www/Plots/13TeV_v80X_moriond17/SignalInterpretation/";
  TFile *infile  = TFile::Open(Form("%sOutput_Eff.root",indir.Data()));
  TFile *outfile = TFile::Open(Form("%sOutput_1DEff.root",outdir.Data()),"RECREATE");

  // ----------------------------------------------------------------
  // Fit w/ deg1 polynomial 
  // ----------------------------------------------------------------
  ////// BARYONIC
  Get1DEfficiency(infile,outdir,outfile,"BARY",true, true, "deg1"); // fit mDM, highMET
  Get1DEfficiency(infile,outdir,outfile,"BARY",false,true, "deg1"); // fit mZp, highMET
  Get1DEfficiency(infile,outdir,outfile,"BARY",true, false,"deg1"); // fit mDM, lowMET
  Get1DEfficiency(infile,outdir,outfile,"BARY",false,false,"deg1"); // fit mZp, lowMET
  // 2HDM
  Get1DEfficiency(infile,outdir,outfile,"2HDM",true, true, "deg1"); // fit mA,  highMET
  Get1DEfficiency(infile,outdir,outfile,"2HDM",false,true, "deg1"); // fit mZp, highMET 
  Get1DEfficiency(infile,outdir,outfile,"2HDM",true, false,"deg1"); // fit mA,  lowMET  
  Get1DEfficiency(infile,outdir,outfile,"2HDM",false,false,"deg1"); // fit mZp, lowMET 
  //// ----------------------------------------------------------------
  //// Fit w/ deg2 polynomial 
  //// ----------------------------------------------------------------
  //// BARYONIC
  //Get1DEfficiency(infile,outdir,outfile,"BARY",true, true, "deg2"); // fit mDM, highMET  
  //Get1DEfficiency(infile,outdir,outfile,"BARY",false,true, "deg2"); // fit mZp, highMET
  //Get1DEfficiency(infile,outdir,outfile,"BARY",true, false,"deg2"); // fit mDM, lowMET
  //Get1DEfficiency(infile,outdir,outfile,"BARY",false,false,"deg2"); // fit mZp, lowMET 
  //// 2HDM                                                                               
  //Get1DEfficiency(infile,outdir,outfile,"2HDM",true, true, "deg2"); // fit mA,  highMET
  //Get1DEfficiency(infile,outdir,outfile,"2HDM",false,true, "deg2"); // fit mZp, highMET  
  //Get1DEfficiency(infile,outdir,outfile,"2HDM",true, false,"deg2"); // fit mA,  lowMET   
  //Get1DEfficiency(infile,outdir,outfile,"2HDM",false,false,"deg2"); // fit mZp, lowMET 
 
  // ----------------------------------------------------------------
  // Close the file 
  // ----------------------------------------------------------------
  outfile->Close();
  delete outfile;
}

void Get1DEfficiency(TFile* infile, TString outdir, TFile* outfile, TString sig, Bool_t useY, Bool_t highMET, TString deg)
{

  // ----------------------------------------------------------------
  // Set up variables 
  // ----------------------------------------------------------------
  TString met = (highMET)? "HighMET":"LowMET";
  Int_t model = 0;                                    // BARY
  if (sig.EqualTo("2HDM",TString::kExact)) model = 1; // 2HDM
  std::vector< Double_t > mY;
  if (model==0 && useY)  mY = {1, 10, 50, 150, 500, 1000};                         // Plot for each of these BARY DM masses
  if (model==1 && useY)  mY = {300, 400, 500, 600, 700, 800};                      // Plot for each of these 2HDM A  masses
  if (model==0 && !useY) mY = {10, 20, 50, 100, 200, 300, 500, 1000, 2000, 10000}; // Plot for each of these BARY Zp masses
  if (model==1 && !useY) mY = {600, 800, 1000, 1200, 1400, 1700, 2000, 2500};      // Plot for each of these 2HDM Zp masses
  TString xname = "DM";
  if (model==1 && useY) xname = "A";
  if (!useY)            xname = "Zp";
  Double_t minX = 0;
  Double_t maxX = 0;
  if (model==0 && useY){  minX = 0;   maxX = 2500; }
  if (model==1 && useY){  minX = 500; maxX = 3000; }
  if (model==0 && !useY){ minX = 0;   maxX = 1050; }
  if (model==1 && !useY){ minX = 250; maxX = 850;  }

  // ----------------------------------------------------------------
  // Get the 2D efficiencies 
  // ----------------------------------------------------------------
  TString histoname = Form("h_%s_Eff_%s",sig.Data(),met.Data());
  TH2D* h_eff = (TH2D*)infile->Get(Form("%s",histoname.Data()));
  TH2D* h_err = (TH2D*)infile->Get(Form("%s_err",histoname.Data()));

  // ----------------------------------------------------------------
  // Set up the fit 
  // ----------------------------------------------------------------
  TString eqn = "[0] + [1]*x";
  if (deg.EqualTo("deg2",TString::kExact)) eqn = "[0] + [1]*x + [2]*x^2";
  TF1 *polN = new TF1("polN",Form("%s",eqn.Data()),-20,20);
  TString pname = Form("h_prob_%s_%s_%s_m%s",deg.Data(),sig.Data(),met.Data(),xname.Data());
  TH1D* h_prob = new TH1D(pname,pname,mY.size(),0,mY.size()); // histo w/ prob for each fit

  outfile->cd();
  TCanvas *c = new TCanvas();

  for (UInt_t m=0; m < mY.size(); m++){
    Double_t mbin = 0;
    if (useY) mbin = h_eff->GetYaxis()->FindBin(mY[m]);
    else      mbin = h_eff->GetXaxis()->FindBin(mY[m]);
    Double_t minbin = mbin - 1; 
    Double_t maxbin = mbin + 1;
    // ----------------------------------------------------------------
    // Get Efficiencies (projection from 2D plot) 
    // ----------------------------------------------------------------
    TString hname = Form("h_eff_1D_%s_%s_%s_m%s%1.f",deg.Data(),sig.Data(),met.Data(),xname.Data(),mY[m]);
    TH1D* h_eff_1D = new TH1D(hname,hname,(maxX-minX),minX,maxX);
    if (useY) h_eff_1D = h_eff->ProjectionX(hname,minbin,maxbin); 
    else      h_eff_1D = h_eff->ProjectionY(hname,minbin,maxbin);
    if (model==0) h_eff_1D->GetXaxis()->SetRange(minX,maxX);

    // ----------------------------------------------------------------
    // Get errors on efficiencies 
    // ----------------------------------------------------------------
    if (useY){ 
      Double_t binY = h_err->GetYaxis()->FindBin(mY[m]);
      for (UInt_t binX=0; binX < h_eff_1D->GetSize(); binX++){
        Double_t errX = h_err->GetBinContent(binX,binY);
        h_eff_1D->SetBinError(binX,errX);
      } 
    }
    else{
      Double_t binX = h_err->GetXaxis()->FindBin(mY[m]);
      for (UInt_t binY=0; binY < h_eff_1D->GetSize(); binY++){
        Double_t errY = h_err->GetBinContent(binX,binY);
        h_eff_1D->SetBinError(binY,errY);
      }
    }

    // ----------------------------------------------------------------
    // Fit efficiencies 
    // ----------------------------------------------------------------
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);
    h_eff_1D->Fit("polN"); 
    h_prob->Fill(m,polN->GetProb());
    h_prob->GetXaxis()->SetBinLabel(m+1,Form("%1.f",mY[m]));

    // ----------------------------------------------------------------
    // Save & delete histos 
    // ----------------------------------------------------------------
    c->cd();
    h_eff_1D->SetMarkerStyle(20);
    h_eff_1D->SetMarkerSize(0.5);
    h_eff_1D->Draw("PE");
    h_eff_1D->Write();
    gStyle->SetStatX(0.89);
    gStyle->SetStatY(0.89);
    c->SetLogx(0);
    c->SaveAs(Form("%sfit_%s/%s_m%s/h_1Deff_%s_%s_%s_m%1.f_lin.png",outdir.Data(),deg.Data(),sig.Data(),xname.Data(),deg.Data(),sig.Data(),met.Data(),mY[m]));
    c->SaveAs(Form("%sfit_%s/%s_m%s/h_1Deff_%s_%s_%s_m%1.f_lin.pdf",outdir.Data(),deg.Data(),sig.Data(),xname.Data(),deg.Data(),sig.Data(),met.Data(),mY[m]));
    c->SetLogx(1);
    //TPaveStats* stat = (TPaveStats*)h_eff_1D->GetListOfFunctions()->FindObject("stats");
    //stat->SetX1NDC(0.15);
    //stat->SetX2NDC(0.40);
    //stat->SetY1NDC(0.80);
    //c->Modified();
    //c->Update(); 
    c->SaveAs(Form("%sfit_%s/%s_m%s/h_1Deff_%s_%s_%s_m%1.f_log.png",outdir.Data(),deg.Data(),sig.Data(),xname.Data(),deg.Data(),sig.Data(),met.Data(),mY[m]));
    c->SaveAs(Form("%sfit_%s/%s_m%s/h_1Deff_%s_%s_%s_m%1.f_log.pdf",outdir.Data(),deg.Data(),sig.Data(),xname.Data(),deg.Data(),sig.Data(),met.Data(),mY[m]));
    delete h_eff_1D; 
  } 
  
  // ----------------------------------------------------------------
  // Save histo w/ Prob(chi2) for the fit 
  // ----------------------------------------------------------------
  c->cd();
  c->SetLogx(0);
  gStyle->SetOptStat(0);
  h_prob->GetYaxis()->SetTitle("Prob(#chi^{2})");
  if (model==0 && useY)  h_prob->GetXaxis()->SetTitle("m_{#chi}");
  if (model==1 && useY)  h_prob->GetXaxis()->SetTitle("m_{A}");
  if (model==0 && !useY) h_prob->GetXaxis()->SetTitle("m_{Z'}");
  if (model==1 && !useY) h_prob->GetXaxis()->SetTitle("m_{Z'}");
  h_prob->Draw("HIST");
  h_prob->Write();
  c->SaveAs(Form("%sfit_%s/%s_m%s/h_fitprob_%s_%s_%s.png",outdir.Data(),deg.Data(),sig.Data(),xname.Data(),deg.Data(),sig.Data(),met.Data()));
  c->SaveAs(Form("%sfit_%s/%s_m%s/h_fitprob_%s_%s_%s.pdf",outdir.Data(),deg.Data(),sig.Data(),xname.Data(),deg.Data(),sig.Data(),met.Data()));
   
  delete h_prob; 
  delete c;

}

