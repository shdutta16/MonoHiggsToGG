#include "TROOT.h"
#include "TFile.h"
#include "TString.h"
#include "TGraph.h"

void getLimitGraphs(TString, TGraphAsymmErrors* &, TGraphAsymmErrors* &, TGraphAsymmErrors* &, TGraphAsymmErrors* &, TGraphAsymmErrors* &, string &);
void drawGraphs(TString, std::vector<std::pair<TString,TString> >, string, TString, TString, std::vector<TGraphAsymmErrors* >);
void drawGraphsWithBands(TString, std::vector<std::pair<TString,TString> >, string, TString, TString, std::vector<TGraphAsymmErrors* >, std::vector<TGraphAsymmErrors* >);

void compareLimits(){

  TString indir  = "/afs/cern.ch/work/m/mzientek/private/FitDirectory/EXO_7_4_15/src/HiggsAnalysis/CombinedLimit/";
  TString outdir = "~/www/Plots/13TeV_v80X_moriond17/TestLimits/";
  TString model  = "2HDM";

  int type = 5;
  TString nametype = "exp";
  if (type==0) nametype = "theo";
  if (type==1) nametype = "exp";
  if (type==2) nametype = "exp68";
  if (type==3) nametype = "exp95";
  if (type==4) nametype = "obs";
  if (type==5) nametype = "exp_wbands";

  //--------------------------------------------------------------------
  // choose files and the titles 
  //--------------------------------------------------------------------
  std::vector< std::pair<TString,TString> > fileNames;
  if (model.Contains("BARY",TString::kExact)){
    fileNames.push_back(std::make_pair(TString::Format("%sntuples4fit_orig/graphs_ZpBaryonic_Asymptotic_1.root",indir.Data()),"Original Limits")); 
    fileNames.push_back(std::make_pair(TString::Format("%sntuples4fit_mDM1/graphs_ZpBaryonic_Asymptotic_1.root",indir.Data()),"New Method Limits")); 
  }
  if (model.Contains("2HDM",TString::kExact)){
    fileNames.push_back(std::make_pair(TString::Format("%sgraphs_2HDM300_Asymptotic_001.root",indir.Data()),"Original Limits"));                 // Livia 2HDM
    fileNames.push_back(std::make_pair(TString::Format("%sntuples4fit_orig/graphs_2HDM_Asymptotic_300.root",indir.Data()),"Original Limits (New)")); // Rerun 2HDM
    fileNames.push_back(std::make_pair(TString::Format("%sntuples4fit_2HDM/graphs_2HDM_Asymptotic_300.root",indir.Data()),"New Method Limits")); // New 2HDM
  }
  int num  = fileNames.size();
  std::cout << "Comparing " << num << " limit curves... " << std::endl;

  //--------------------------------------------------------------------
  // pickup the graphs
  //--------------------------------------------------------------------
  std::vector< TGraphAsymmErrors* > theo;	theo.resize(num);
  std::vector< TGraphAsymmErrors* > exp;	exp.resize(num);
  std::vector< TGraphAsymmErrors* > exp68;	exp68.resize(num);
  std::vector< TGraphAsymmErrors* > exp95;	exp95.resize(num);
  std::vector< TGraphAsymmErrors* > obs;	obs.resize(num);
  string mass = "";
  for ( int i = 0; i < num; ++i){ 
    getLimitGraphs( fileNames[i].first, theo[i], exp[i], exp68[i], exp95[i], obs[i], mass);
  }

  //--------------------------------------------------------------------
  // redraw together
  //--------------------------------------------------------------------
  if (type==0) drawGraphs(outdir,fileNames,mass,model,nametype,theo); 
  if (type==1) drawGraphs(outdir,fileNames,mass,model,nametype,exp);
  if (type==2) drawGraphs(outdir,fileNames,mass,model,nametype,exp68);
  if (type==3) drawGraphs(outdir,fileNames,mass,model,nametype,exp95);
  if (type==4) drawGraphs(outdir,fileNames,mass,model,nametype,obs);
  if (type==5) drawGraphsWithBands(outdir,fileNames,mass,model,nametype,exp,exp68);

}

void drawGraphs(TString outdir, std::vector<std::pair<TString,TString>> names, string mass, TString model, TString type, std::vector<TGraphAsymmErrors* > g){

  TCanvas *c = new TCanvas("c");
  c->cd();
  c->SetGridx();
  c->SetGridy();
  c->SetLogx();
  c->SetLogy();

  TPad *p = new TPad();
  c->SetLeftMargin(0.15);
  c->SetRightMargin(0.10);
  c->SetTopMargin(0.10); 
  c->SetBottomMargin(0.15);

  TLegend *l = new TLegend(0.55,0.60,0.85,0.85);
  l->SetBorderSize(0);

  if (model.Contains("BARY",TString::kExact)) g[0]->GetXaxis()->SetRangeUser(10,10000);
  if (model.Contains("2HDM",TString::kExact)) g[0]->GetXaxis()->SetRangeUser(600,4000);
  g[0]->GetYaxis()->SetRangeUser(0.01,500);
  g[0]->Draw("ACP");

  int color = 1;
  for(int i = 0; i < g.size(); i++){
    g[i]->SetLineStyle(1);
    g[i]->SetMarkerStyle(33);
    g[i]->SetMarkerSize(2);
    if (i!=0) color = i*50;
    g[i]->SetMarkerColor(color);
    g[i]->SetLineColor(color);
    g[i]->Draw("CP same");
    l->AddEntry(g[i],names[i].second,"PL");
  }
  l->Draw("same"); 
 
  c->SaveAs(TString::Format("%sLimitComparison_%s_%s_m%s_%i.png",outdir.Data(),model.Data(),type.Data(),mass.c_str(),(int)g.size()));
  c->SaveAs(TString::Format("%sLimitComparison_%s_%s_m%s_%i.pdf",outdir.Data(),model.Data(),type.Data(),mass.c_str(),(int)g.size()));

}

void getLimitGraphs(TString name, TGraphAsymmErrors* & theory, TGraphAsymmErrors* & exp, TGraphAsymmErrors* & exp68, TGraphAsymmErrors* & exp95, TGraphAsymmErrors* & obs, string & mass){

  string sname = name.Data();
  string delim1     = "Asymptotic_";
  string delim2     = ".root";
  size_t posdelim1  = sname.find(delim1);
  size_t posdelim2  = sname.find(delim2);
  mass = sname.substr(posdelim1+delim1.length(),posdelim2-posdelim1-delim1.length());
  std::cout << "Mass: " << mass << std::endl;

  TFile *infile = new TFile(name);
  theory = (TGraphAsymmErrors*)infile->Get("theo");
  exp    = (TGraphAsymmErrors*)infile->Get(TString::Format("expected_%s",mass.c_str())); 
  exp68  = (TGraphAsymmErrors*)infile->Get(TString::Format("expected68_%s",mass.c_str()));
  exp95  = (TGraphAsymmErrors*)infile->Get(TString::Format("expected95_%s",mass.c_str()));
  obs    = (TGraphAsymmErrors*)infile->Get(TString::Format("observed_%s",mass.c_str()));

  if(exp==(TGraphAsymmErrors*)NULL){
    std::cout << "Graphs not found -- Check the mass point" << std::endl;
    exit(1); 
  }
  
  delete infile;

}

void drawGraphsWithBands(TString outdir, std::vector<std::pair<TString,TString>> names, string mass, TString model, TString type, std::vector<TGraphAsymmErrors* > g, std::vector<TGraphAsymmErrors* > b){

  TCanvas *c = new TCanvas("c");
  c->cd();
  c->SetGridx();
  c->SetGridy();
  c->SetLogx();
  c->SetLogy();

  TPad *p = new TPad();
  c->SetLeftMargin(0.15);
  c->SetRightMargin(0.10);
  c->SetTopMargin(0.10); 
  c->SetBottomMargin(0.15);

  TLegend *l = new TLegend(0.55,0.60,0.85,0.85);
  l->SetBorderSize(0);

  if (model.Contains("BARY",TString::kExact)) g[0]->GetXaxis()->SetRangeUser(10,10000);
  if (model.Contains("2HDM",TString::kExact)) g[0]->GetXaxis()->SetRangeUser(600,4000);
  g[0]->GetYaxis()->SetRangeUser(0.01,500);
  g[0]->Draw("ACP");
  b[0]->SetFillColor(kYellow);
  b[0]->SetLineColor(kYellow);
  b[0]->SetMarkerStyle(0); 
  b[0]->Draw("FLE3 SAME");

  int color = 1;
  for(int i = 0; i < g.size(); i++){
    g[i]->SetLineStyle(1);
    g[i]->SetMarkerStyle(33);
    g[i]->SetMarkerSize(2);
    if (i!=0) color = i*50;
    g[i]->SetMarkerColor(color);
    g[i]->SetLineColor(color);
    g[i]->Draw("CP same");
    l->AddEntry(g[i],names[i].second,"PL");
    if (i==0) l->AddEntry(b[i],"1 std. dev.","F");
  }
  l->Draw("same"); 
 
  c->SaveAs(TString::Format("%sLimitComparison_%s_%s_m%s_%i.png",outdir.Data(),model.Data(),type.Data(),mass.c_str(),(int)g.size()));
  c->SaveAs(TString::Format("%sLimitComparison_%s_%s_m%s_%i.pdf",outdir.Data(),model.Data(),type.Data(),mass.c_str(),(int)g.size()));

}

