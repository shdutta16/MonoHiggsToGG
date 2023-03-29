#include "TROOT.h"
#include "TFile.h"
#include "TString.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH2.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include <iostream>

void run(TString, TString, int);
void getXsec2HDM(TFile*, int, int, Double_t &);
void getXsecBary(TFile*, int, int, Double_t &);
void getLimitVal(TFile*, Double_t, Double_t &);
void getMassPoints(int, std::vector<TString> &);
void getInterpolation(TGraph*, bool, Double_t &); // not used
void getIntMasses(TString, int, int &, int &);
void getHCombFile(TString, TString, int, TFile* &);
void getContours(int, int, TGraph2D*, TGraph* &);
void compare1DmuPlots(TString, std::vector<int>, std::vector<TString>, int, std::map<TString,Double_t>, std::map<TString,Double_t>);
void get1DmuPlots(TGraph* &, TGraph* &, int, std::vector<TString>, int, std::map<TString,Double_t>, std::map<TString,Double_t>);
void setStyle(TCanvas*, TPad*);
void setCMSLabels(TLatex* &, TLatex* &, TLatex* &, TLatex* &, int);

void makeLimitPlotNicer(){
  std::cout << "Making exclusion plots..." << std::endl;
 
  Int_t type = 1; // type 0 : BARY, type 1 : 2HDM
 
  TString inDir  = "";
  if (type==0) inDir = "/afs/cern.ch/work/m/mzientek/private/FitDirectory/EXO_7_4_15/src/HiggsAnalysis/CombinedLimit/ntuples4fit_BARY/";
  //if (type==1) inDir = "/afs/cern.ch/work/m/mzientek/private/FitDirectory/EXO_7_4_15/src/HiggsAnalysis/CombinedLimit//ntuples4fit_2HDM/";
  if (type==1) inDir = "/afs/cern.ch/work/m/mzientek/public/ntuples4fit/ntuples4fit_2HDM/";
  TString outDir = "~/www/Plots/13TeV_v80X_moriond17/ContourTest/";

  // --- Call
  run(inDir,outDir,type);

}

void run(TString inDir, TString outDir, int type){

  TString strtype = "";
  if (type==0) strtype = "BARY";
  if (type==1) strtype = "2HDM";

  // Theory xsec root file
  TFile* xsecFile;
  if (type==0) xsecFile = new TFile("theory_ZPBaryonic.root");
  //if (type==0) xsecFile = new TFile("Output_xsecZpBARY.root");
  //if (type==1) xsecFile = new TFile("Output_xsecZp2HDM.root");
  //if (type==1) xsecFile = new TFile("Output_xsecZp2HDM_fit.root");
  //if (type==1) xsecFile = new TFile("Output_xsecZp2HDM_interpolated.root");
  //if (type==1) xsecFile = new TFile("Output_xsecZp2HDM_var.root");
  if (type==1) xsecFile = new TFile("Output_xsecZp2HDM_1Dfit.root");

  // Histo binning
  double nXbins, minX, maxX;
  double nYbins, minY, maxY;
  if (type==0){
    nXbins = 990;
    minX   = 10;
    maxX   = 1000;
    nYbins = 499;
    minY   = 1;
    maxY   = 500;
  }
  if (type==1){
    minX   = 575;
    maxX   = 2025;
    minY   = 287;
    maxY   = 612;
    nXbins = (maxX-minX)/50; 
    nYbins = (maxY-minY)/25;
  }

  // Output root file
  TString outfilename = TString::Format("%sContourPlot_OutputFile_%s.root",outDir.Data(),strtype.Data());
  std::cout << "Write out to : " << outfilename << std::endl;
  TFile* outfile = new TFile(outfilename,"RECREATE");

  // Setup which samples to run
  std::vector<TString> sampleNames; 
  getMassPoints( type, sampleNames );
  UInt_t nSig = sampleNames.size();

  int m1;
  int m2;
  std::map<TString,Double_t> xsecMap;
  std::map<TString,TFile* >  hcombMap;
  std::map<TString,Double_t> expLimitMap;
  std::map<TString,Double_t> obsLimitMap;
  std::map<TString,Double_t> p1sLimitMap;
  std::map<TString,Double_t> n1sLimitMap;
  std::map<TString,Double_t> expMuMap;
  std::map<TString,Double_t> obsMuMap;
  std::map<TString,Double_t> p1sMuMap;
  std::map<TString,Double_t> n1sMuMap;

  // Setup graphs
  TGraph2D* g2d_exp = new TGraph2D();
  TGraph2D* g2d_obs = new TGraph2D();
  TGraph2D* g2d_p1s = new TGraph2D();
  TGraph2D* g2d_n1s = new TGraph2D();
  // Setup histos
  TH2D* h2_exp = new TH2D("h2_exp","h2_exp",nXbins,minX,maxX,nYbins,minY,maxY);
  TH2D* h2_obs = new TH2D("h2_obs","h2_obs",nXbins,minX,maxX,nYbins,minY,maxY);
  TH2D* h2_p1s = new TH2D("h2_p1s","h2_p1s",nXbins,minX,maxX,nYbins,minY,maxY);
  TH2D* h2_n1s = new TH2D("h2_n1s","h2_n1s",nXbins,minX,maxX,nYbins,minY,maxY);
  TH2D* h2d_exp = new TH2D("h2d_exp","h2d_exp",nXbins,minX,maxX,nYbins,minY,maxY);
  TH2D* h2d_obs = new TH2D("h2d_obs","h2d_obs",nXbins,minX,maxX,nYbins,minY,maxY);
  TH2D* h2d_p1s = new TH2D("h2d_p1s","h2d_p1s",nXbins,minX,maxX,nYbins,minY,maxY);
  TH2D* h2d_n1s = new TH2D("h2d_n1s","h2d_n1s",nXbins,minX,maxX,nYbins,minY,maxY);

  // -----------------------------------
  // Loop over all signals
  // -----------------------------------
  for (UInt_t i=0; i < nSig; i++){
    // prep everything
    getIntMasses(sampleNames[i],type,m1,m2);                          // get int values of masses
    if (type==0) getXsecBary(xsecFile,m1,m2,xsecMap[sampleNames[i]]); // get xsecs
    if (type==1) getXsec2HDM(xsecFile,m1,m2,xsecMap[sampleNames[i]]); // get xsecs
    getHCombFile(inDir,sampleNames[i],type,hcombMap[sampleNames[i]]); // open higgs combine files
    // get limit values
    getLimitVal(hcombMap[sampleNames[i]],0.50,expLimitMap[sampleNames[i]]); // exp limit 
    getLimitVal(hcombMap[sampleNames[i]],-1.0,obsLimitMap[sampleNames[i]]); // obs limit 
    getLimitVal(hcombMap[sampleNames[i]],0.84,p1sLimitMap[sampleNames[i]]); // p1sig limit 
    getLimitVal(hcombMap[sampleNames[i]],0.16,n1sLimitMap[sampleNames[i]]); // n1sig limit
    expMuMap[sampleNames[i]] = expLimitMap[sampleNames[i]]/xsecMap[sampleNames[i]]; // exp signal significance 
    obsMuMap[sampleNames[i]] = obsLimitMap[sampleNames[i]]/xsecMap[sampleNames[i]]; // obs signal significance 
    p1sMuMap[sampleNames[i]] = p1sLimitMap[sampleNames[i]]/xsecMap[sampleNames[i]]; // p1sig signal significance 
    n1sMuMap[sampleNames[i]] = n1sLimitMap[sampleNames[i]]/xsecMap[sampleNames[i]]; // n1sig signal significance
    // setup 2D graphs
    g2d_exp->SetPoint(i,m1,m2,expMuMap[sampleNames[i]]); 
    g2d_obs->SetPoint(i,m1,m2,obsMuMap[sampleNames[i]]);
    g2d_p1s->SetPoint(i,m1,m2,p1sMuMap[sampleNames[i]]);
    g2d_n1s->SetPoint(i,m1,m2,n1sMuMap[sampleNames[i]]);
    // setup 2D histos
    h2d_exp->Fill(m1,m2,expMuMap[sampleNames[i]]);
    h2d_obs->Fill(m1,m2,obsMuMap[sampleNames[i]]);
    h2d_p1s->Fill(m1,m2,p1sMuMap[sampleNames[i]]);
    h2d_n1s->Fill(m1,m2,n1sMuMap[sampleNames[i]]);
  }

  g2d_exp->SetHistogram(h2_exp);
  g2d_obs->SetHistogram(h2_obs);
  g2d_p1s->SetHistogram(h2_p1s);
  g2d_n1s->SetHistogram(h2_n1s);

  // 1D mu comparison plot
  std::vector<int> masses;
  if (type==0) masses = {10,100,1000};          // BARY
  if (type==1) masses = {300,325,350,375,400};  // 2HDM
  compare1DmuPlots(outDir,masses,sampleNames,type,expMuMap,obsMuMap); 

  // get contours for each
  TGraph* gc_exp = NULL;
  TGraph* gc_obs = NULL;
  TGraph* gc_p1s = NULL;
  TGraph* gc_n1s = NULL;
  getContours(type, 0, g2d_exp, gc_exp);
  getContours(type, 1, g2d_obs, gc_obs); 
  getContours(type, 2, g2d_p1s, gc_p1s); 
  getContours(type, 3, g2d_n1s, gc_n1s);

  // save contours to outfile
  outfile->cd();
  gc_exp->SetName("expected_curve");
  gc_obs->SetName("observed_curve");
  gc_exp->Write();
  gc_obs->Write();
 
  gStyle->SetPadRightMargin(0.109);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetOptFit(1);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetNumberContours(505);

  // setup canvas and pad
  TCanvas *c_copy = new TCanvas("c_copy","c_copy",525,1113,800,795);
  TPad *p         = new TPad("p","",0.01,0.03,0.98,1.00);
  c_copy->cd();
  p->cd();
  setStyle(c_copy,p);
  c_copy->Update();

  // draw histos
  if (type==0) h2d_obs->GetYaxis()->SetRangeUser(5,500);
  h2d_obs->Draw("COLZ");
  h2d_obs->SetMinimum(0.1);
  h2d_obs->SetMaximum(50);

  // setup axis labels
  TLatex *lxex = new TLatex(0.5363409,0.1963589,"m_{Z'} [GeV]"); 
  TLatex *lyex;
  if (type==0) lyex = new TLatex(0.03,0.77,"m_{#chi} [GeV]");
  if (type==1) lyex = new TLatex(0.03,0.77,"m_{A} [GeV]");
  TLatex *lzex = new TLatex(0.8859649,0.9440832,"Observed #mu 95% CL");
  lxex->SetTextSize(0.054);     
  lxex->SetTextAlign(12);
  lxex->SetNDC(kTRUE);  
  lxex->SetTextFont(42);
  lxex->Draw("same"); 
  lyex->SetTextSize(0.054);
  lyex->SetTextAlign(12);
  lyex->SetTextAngle(90);
  lyex->SetNDC(kTRUE);
  lyex->SetTextFont(42);
  lyex->Draw("same"); 
  lzex->SetTextSize(0.054);
  lzex->SetTextAlign(12);
  lzex->SetTextAngle(270);
  lzex->SetNDC(kTRUE);
  lzex->SetTextFont(42);
  lzex->Draw("same"); 

  // style for contours
  c_copy->RedrawAxis();
  gc_exp->SetLineColor(kBlack);
  gc_exp->SetLineStyle(1);
  gc_exp->SetLineWidth(2);
  gc_obs->SetLineColor(kRed);
  gc_obs->SetLineStyle(1);
  gc_obs->SetLineWidth(2);
  gc_p1s->SetLineStyle(0);
  gc_n1s->SetLineStyle(0);
  gc_p1s->SetFillStyle(0);
  gc_n1s->SetFillStyle(0);

  // draw contours
  gc_obs->Draw("Csame");
  gc_exp->Draw("Csame");
  gc_p1s->Draw("Csame");
  gc_n1s->Draw("Csame");

  // setup legend
  TLegend* cleg = new TLegend(0.19,0.80,0.45,0.88,NULL,"brNDC"); // (x1,y1,x2,y2)
  cleg->SetBorderSize(0);
  cleg->SetLineColor(1);
  cleg->SetLineWidth(1);
  cleg->SetLineStyle(1);
  cleg->SetTextFont(62);
  cleg->AddEntry(gc_obs,"Observed 95\% CL","L");
  cleg->AddEntry(gc_exp,"Median Expected 95\% CL","L");
  cleg->AddEntry(gc_p1s,"Expected #pm 1#sigma_{experiment}","L");
  cleg->Draw("same");

  // save plot
  c_copy->cd();
  c_copy->SetLogz();
  TLatex *l1; TLatex *l2; TLatex *l3; TLatex *l4;
  setCMSLabels(l1,l2,l3,l4,type); // add CMS labels
  l1->Draw("same");
  l2->Draw("same");
  l3->Draw("same");
  //l4->Draw("same");
  
  TString addname = "";
  if (type==0) addname = "ZpBARY";
  if (type==1) addname = "Zp2HDM";
  c_copy->SaveAs(TString::Format("%sTEST_%s_Contour.png",outDir.Data(),addname.Data())); 
  c_copy->SaveAs(TString::Format("%sTEST_%s_Contour.pdf",outDir.Data(),addname.Data())); 

}

void compare1DmuPlots(TString outDir, std::vector<int> masses, std::vector<TString> sampleNames, int type, std::map<TString,Double_t> expMuMap, std::map<TString,Double_t> obsMuMap){

  int nSig = masses.size();
  std::vector< TGraph* > muExp;
  std::vector< TGraph* > muObs;
  muExp.resize(nSig);
  muObs.resize(nSig);

  for (UInt_t i = 0; i < nSig; i++){
    // make the 1D plots for each sample
    muExp[i] = new TGraph();
    muObs[i] = new TGraph(); 
    get1DmuPlots(muExp[i],muObs[i],masses[i],sampleNames,type,expMuMap,obsMuMap);
  } 

  TCanvas * c1D = new TCanvas("c1D");
  c1D->cd();
  c1D->SetLogx();
  c1D->SetLogy();

  muExp[0]->GetXaxis()->SetTitle("m_{Z'} [GeV]");
  muExp[0]->GetYaxis()->SetTitle("#sigma_{95\% CL} / #sigma_{th}");
  muExp[0]->SetMaximum(10);
  muExp[0]->SetMinimum(0.1);
  muExp[0]->Draw(); 

  // mu = 1 line
  TLine* line = new TLine();
  line->SetX1(muExp[0]->GetXaxis()->GetXmin());
  line->SetX2(muExp[0]->GetXaxis()->GetXmax());
  line->SetY1(1.0);
  line->SetY2(1.0);
  line->SetLineColor(kRed);
  line->SetLineWidth(2);
  line->Draw("SAME");

  // legend
  TLegend * lty;
  if (type==0) lty = new TLegend(0.12,0.80,0.35,0.88,NULL,"brNDC"); // (x1,y1,x2,y2)
  if (type==1) lty = new TLegend(0.72,0.30,0.90,0.38,NULL,"brNDC"); // (x1,y1,x2,y2)
  lty->SetBorderSize(0);
  lty->SetLineColor(1);
  lty->SetLineWidth(1);
  lty->SetLineStyle(1);
  lty->SetTextSize(0.03);
  lty->SetFillColor(0);
  muExp[0]->SetLineColor(kBlack);
  muExp[0]->SetLineWidth(2);
  muExp[0]->SetLineStyle(2);
  muObs[0]->SetLineColor(kBlack);
  muObs[0]->SetLineWidth(2);
  muObs[0]->SetLineStyle(1);
  lty->AddEntry(muExp[0],"Expected","L");
  lty->AddEntry(muObs[0],"Observed","L");
  
  TLegend * l1D;
  if (type==0) l1D = new TLegend(0.12,0.65,0.35,0.78,NULL,"brNDC"); // (x1,y1,x2,y2)
  if (type==1) l1D = new TLegend(0.72,0.15,0.90,0.28,NULL,"brNDC"); // (x1,y1,x2,y2)
  l1D->SetBorderSize(0);
  l1D->SetLineColor(1);
  l1D->SetLineWidth(1);
  l1D->SetLineStyle(1);
  l1D->SetTextSize(0.03);
  l1D->SetFillColor(0);
 
  for (UInt_t i = 0; i < nSig; i++){
    // style plots
    muExp[i]->SetLineStyle(2);
    muObs[i]->SetLineStyle(1);
    muExp[i]->SetLineWidth(2);
    muObs[i]->SetLineWidth(2);
    if (i > 0){
      muExp[i]->SetLineColor(10*(i+1));
      muObs[i]->SetLineColor(10*(i+1));
    }
    // draw
    muExp[i]->Draw("L SAME");
    muObs[i]->Draw("L SAME");
    if (type==0) l1D->AddEntry(muObs[i],TString::Format("m_{#chi} = %i",masses[i]),"L"); 
    if (type==1) l1D->AddEntry(muObs[i],TString::Format("m_{A} = %i",masses[i]),"L"); 
  }
  
  TLatex *l1; TLatex *l2; TLatex *l3; TLatex *l4;
  setCMSLabels(l1,l2,l3,l4,type); // add CMS labels
  //l1->Draw("same");
  //l2->Draw("same");
  //l3->Draw("same");
  lty->Draw("same");
  l1D->Draw("same");
  TString addname = "";
  if (type==0) addname = "ZpBARY"; 
  if (type==1) addname = "Zp2HDM";
  c1D->SaveAs(TString::Format("%sTEST_%s_1DMu.png",outDir.Data(),addname.Data()));
  c1D->SaveAs(TString::Format("%sTEST_%s_1DMu.pdf",outDir.Data(),addname.Data()));

}

void get1DmuPlots(TGraph* & muExp, TGraph* & muObs, int m, std::vector<TString> samples, int type, std::map<TString,Double_t> exp, std::map<TString,Double_t> obs){

  // select the samples to graph
  int m1, m1use;
  int m2, m2use;
  std::vector<TString> useSamples;
  for (UInt_t i = 0; i < samples.size(); i++){
    getIntMasses(samples[i],type,m1,m2); // get int values of masses
    if (m==m2) useSamples.push_back(samples[i]);
  } 

  for (UInt_t i = 0; i < useSamples.size(); i++){
    getIntMasses(useSamples[i],type,m1use,m2use); // get int values of masses
    muExp->SetPoint(i,m1use,exp[useSamples[i]]);  // set Expected graph points
    muObs->SetPoint(i,m1use,obs[useSamples[i]]);  // set Observed graph points
  }   

}

void setCMSLabels(TLatex* & l1, TLatex* & l2, TLatex* & l3, TLatex* & l4, int type){

  TString latexCMSname = "#bf{CMS} #it{Preliminary}";
  l1 = new TLatex(0.16,0.97,latexCMSname);
  l1->SetTextSize(0.036);
  l1->SetTextAlign(12);
  l1->SetNDC(kTRUE);
  l1->SetTextFont(42);

  TString latexlumi = "35.9 fb^{-1}";
  TString latexenergy = " (13 TeV)";
  TString latexname = latexlumi+latexenergy;
  l2 = new TLatex(0.505,.97,latexname);
  l2->SetTextSize(0.033);
  l2->SetTextAlign(12);
  l2->SetNDC(kTRUE);
  l2->SetTextFont(42);
 
  TString thestring = "Z'#rightarrow DM+h(#gamma#gamma)";
  TString addmodel  = ""; 
  if (type==0) addmodel = "(Baryonic)";
  if (type==1) addmodel = "(2HDM)"; 

  l3 = new TLatex(0.20,0.90,thestring);
  l3->SetTextSize(0.033);
  l3->SetTextAlign(12);
  l3->SetNDC(kTRUE);
  l3->SetTextFont(42);
  l4 = new TLatex(0.20,0.87,addmodel);
  l4->SetTextSize(0.031);
  l4->SetTextAlign(12);
  l4->SetNDC(kTRUE);
  l4->SetTextFont(42);

}

void setStyle(TCanvas* c, TPad* p){

  c->Range(-261.1739,-215.1203,1456.261,537.1745);
  c->SetFillColor(0);
  c->SetBorderMode(0);
  c->SetBorderSize(2);
  c->SetTickx(1);
  c->SetTicky(1);
  c->SetLeftMargin(0.1578947);
  c->SetRightMargin(0.2656642);
  c->SetTopMargin(0.04941482);
  c->SetBottomMargin(0.2925878);
  c->SetFrameFillStyle(0);
  c->SetFrameLineWidth(3);
  c->SetFrameBorderMode(0);
  c->SetFrameFillStyle(0);
  c->SetFrameLineWidth(3);
  c->SetFrameBorderMode(0);

  p->SetLogz();
  c->cd();

}

void getInterpolation(TGraph* gc, bool isFirst, Double_t & xobs){

  double* xvals=gc->GetX();
  double* yvals=gc->GetY();
  double deltaylow=999999999;
  double deltayup=999999999;
  double yobs=1.;
  double yup=999.;
  double xup=999.;
  double ylow=999.;
  double xlow=999.; 

  int binmin=13;
  if(isFirst){
    for(int i=0;i<binmin; i++){
    //std::cout<<yvals[i]<<" "<<xvals[i]<<std::endl;
    double deltay=yvals[i]-yobs;
    if(deltay>=0 && deltay<deltayup){
      deltayup=deltay;
      yup=yvals[i];
      xup=xvals[i];
    }else if(deltay<0 && abs(deltay)<abs(deltaylow)){
      deltaylow=deltay;
      xlow=xvals[i];
      ylow=yvals[i];
     
    }    
  }
  }else{
    for(int i=binmin;i<gc->GetN(); i++){
    //std::cout<<yvals[i]<<" "<<xvals[i]<<std::endl;
    double deltay=yvals[i]-yobs;
    if(deltay>=0 && deltay<deltayup){
      deltayup=deltay;
      yup=yvals[i];
      xup=xvals[i];
    }else if(deltay<0 && abs(deltay)<abs(deltaylow)){
      deltaylow=deltay;
      xlow=xvals[i];
      ylow=yvals[i];
     
    }    
  }

  }

  double m=(yup-ylow)/(xup-xlow);
  xobs=(1-ylow+m*xlow)/m; //output
}

void getContours(int type, int j, TGraph2D* g, TGraph* & gc){
  TCanvas* c = new TCanvas(TString::Format("c%d",j),"",800,800); 
  c->cd();

  if (j==0) std::cout << " exp " << std::endl;
  if (j==1) std::cout << " obs " << std::endl;
  if (j==2) std::cout << " p1s " << std::endl;
  if (j==3) std::cout << " n1s " << std::endl;

  // setup output histogram
  double nbinx = g->GetXaxis()->GetNbins()+1;
  double min_x = g->GetXaxis()->GetBinLowEdge(1) - g->GetXaxis()->GetBinWidth(1);
  double max_x = g->GetXaxis()->GetBinUpEdge(nbinx-1);
  double nbiny = g->GetYaxis()->GetNbins()+1;
  double min_y = g->GetYaxis()->GetBinLowEdge(1) - g->GetYaxis()->GetBinWidth(nbiny);
  double max_y = g->GetYaxis()->GetBinUpEdge(nbiny-1);
  //std::cout << nbinx << " " << min_x << " " << max_x << std::endl;
  //std::cout << nbiny << " " << min_y << " " << max_y << std::endl;

  // fill histogram correctly
  TH2D* h = new TH2D(TString::Format("h%d",j),"",nbinx,min_x,max_x,nbiny,min_y,max_y);
  h = g->GetHistogram();
  // get tgraph points
  int npoints = g->GetN();
  double * xarray = g->GetX();
  double * yarray = g->GetY();
  double * zarray = g->GetZ();
  for (int n = 0; n < npoints; n++){ 
    if (yarray[n] == 300){
      h->Fill(xarray[n],yarray[n],zarray[n]);
    }
  }

  Double_t contours[1];
  contours[0] = 1.;
  
  h->SetContour(1,contours);
  h->Draw("CONT Z LIST");
  c->Update();

  // Get contours
  TObjArray *conts = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
  TList* contLevel = NULL;
  TGraph* curv     = NULL;

  Int_t nGraphs    = 0;
  Int_t TotalConts = 0;

  if (conts == NULL){
    printf("*** No Contours Were Extracted!\n");
    TotalConts = 0; 
    //return;
  } else {
    TotalConts = conts->GetSize();
  }

  printf("TotalConts = %d\n", TotalConts);

  for(int i = 0; i < TotalConts; i++){
    contLevel = (TList*)conts->At(i);
    printf("Contour %d has %d Graphs\n", i, contLevel->GetSize());
  }

  Double_t x0, y0, z0;
  TLatex l;
  l.SetTextSize(0.03);
  char val[20];
  TMultiGraph* multig = new TMultiGraph();
  for(int i = 0; i < TotalConts; i++){ // loop over each contour 
    contLevel = (TList*)conts->At(i);
    // Get first graph from list on curves on this level 
    curv = (TGraph*)contLevel->First();
    for(int j = 0; j < 1; j++){// for each, get each curve 
      curv->GetPoint(0, x0, y0);
      nGraphs ++;
      printf("\tGraph: %d  -- %d Elements\n", nGraphs,curv->GetN());
      // Draw clones of the graphs to avoid deletions in case the 1st pad is redrawn. 
      gc = (TGraph*)curv->Clone(); 
      multig->Add(gc);
      curv = (TGraph*)contLevel->After(curv); // get next graph    
    }
  }
  delete h;
  delete c;
}

void getLimitVal(TFile* file, Double_t quantile, Double_t & Limit){
  Double_t limit;
  Float_t quantileExpected;
  TBranch *b_limit;
  TBranch *b_quantileExpected;

  TTree* tree = (TTree*)file->Get("limit");
  if (tree!=(TTree*)NULL){
    tree->SetBranchAddress("limit", &limit, &b_limit);
    tree->SetBranchAddress("quantileExpected", &quantileExpected, &b_quantileExpected);
    Limit = 0; 
    int nentries = tree->GetEntries();
    for (int entry = 0; entry < nentries; entry++){
      tree->GetEntry(entry);
      //std::cout << "Quantile = " << quantileExpected << std::endl;
      //std::cout << "Limit    = " << limit << std::endl;
      if (quantileExpected==quantile) Limit=limit;
      if (quantileExpected>quantile-0.01 && quantileExpected<quantile+0.01) Limit=limit;
    }    
  }// end valid tree
  else Limit = 0;
  delete tree;

}

void getHCombFile(TString inDir, TString name, int type, TFile* & higgsCombineFile){
  int m1, m2;
  getIntMasses(name,type,m1,m2);
  higgsCombineFile = new TFile(TString::Format("%shiggsCombineMonoHgg_sig_%s.Asymptotic.mH%d.root",inDir.Data(),name.Data(),m1));
}

void getIntMasses(TString name, int type, int & m1, int & m2){
  //pick out mZP mass 
  string sname = name.Data(); 
  string delimiterM1 = "_mZP";
  string delimiterM2 = "_mChi";
  if (type==1) delimiterM2 = "_mA0";
  size_t posM1 = sname.find(delimiterM1);
  size_t posM2 = sname.find(delimiterM2);
  string smass1 = sname.substr(posM1+delimiterM1.length(),posM2-posM1-delimiterM1.length());
  string smass2 = sname.substr(posM2+delimiterM2.length(),sname.length());
  const char * mass1 = smass1.c_str();
  const char * mass2 = smass2.c_str(); 
  m1 = std::atoi(mass1);
  m2 = std::atoi(mass2); 
  //std::cout << " Mass 1 = " << mass1 << " , Mass 2 = " << mass2 << std::endl;
}

void getXsecBary(TFile* file, int mZp, int mchi, Double_t & xsec){
  TH2F* xsecs = (TH2F*)file->Get("h2_BARY_XSEC_pb"); 
  if (xsecs!=(TH2F*)NULL){
    xsec = xsecs->Interpolate(mZp, mchi);
    //xsec = xsecs->GetBinContent(xsecs->GetXaxis()->FindBin(mZp),xsecs->GetYaxis()->FindBin(mchi));
    //std::cout<<" mZp: "<<mZp<< " mCHI: "<<mchi<< " XSEC: "<<xsec<<std::endl;
  }
  else{
   xsec = 1;
   std::cout << "Couldn't find xsec histogram" << std::endl;
  }
} 

void getXsec2HDM(TFile* file, int mZp, int mA0, Double_t & xsec){
  TH2F* xsecs = (TH2F*)file->Get("h2_2HDM_XSEC_pb"); 
  if (xsecs!=(TH2F*)NULL){
     Int_t binX = xsecs->GetXaxis()->FindBin(mZp);
     Int_t binY = xsecs->GetYaxis()->FindBin(mA0);
     xsec = xsecs->GetBinContent(binX,binY);
     //if (xsec==0){
     //   xsec = xsecs->Interpolate(mZp,mA0);
     //   std::cout << "Bin Content (" << mZp << "," << mA0 << ") empty -- new xsec : " << xsec << std::endl;
     //   if (xsec==0){
     //     xsec = 1;
     //     std::cout << "Couldn't compute xsec" << std::endl;
     //   }
     //}
  }
  else{
   xsec = 1; 
   std::cout << "Couldn't find xsec histogram" << std::endl;
  }
}

void getMassPoints( Int_t type, std::vector<TString> & names ){
  std::vector< int > mZp;
  std::vector< int > mDM;

  if (type==0){  // Baryonic Zp points
    mZp = {10,20,30,50,75,100,150,200,250,300,400,500,600,700,900,1200,1500,2000,2500,3000,4000,5000,6000,8000,10000}; 
    mDM = {1,5,10,20,40,60,100,125,150,175,200,250,300,350,400,450,500,600,700,800,900,1000};
  }
  if (type==1){ // 2HDM points
    for (int m_A = 0; m_A < 29; m_A++){ mDM.push_back(300+(25*m_A)); }
    //for (int m_Z = 0; m_Z < 69; m_Z++){ mZp.push_back(600+(50*m_Z)); }
    for (int m_Z = 0; m_Z < 29; m_Z++){ mZp.push_back(600+(50*m_Z)); }
  }

  int nZp = mZp.size();
  int nDM = mDM.size();
  for (int m2 = 0; m2 < nDM; m2++){
    for (int m1 = 0; m1 < nZp; m1++){
      if (type==0) names.push_back(TString::Format("ZpBaryonic_mZP%i_mChi%i",mZp[m1],mDM[m2]));
      if (type==1) names.push_back(TString::Format("2HDM_mZP%i_mA0%i",mZp[m1],mDM[m2]));
    }
  } 

}
