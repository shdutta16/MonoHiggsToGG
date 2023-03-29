#include "mkPlotsLivia/CMS_lumi.C"
using namespace RooFit;
using namespace RooStats;

std::string filePOSTfix="";

void MakeComparisonPlots(TString, TString, int, double, double, TString);


void compareSlewRate(){
  MakeComparisonPlots("massOrig",   "massRaw",        26, 99., 151., "m_{#gamma#gamma} (GeV)");
 
  return;
}

void MakeComparisonPlots(TString var1, TString var2, int BINS, double MIN, double MAX, TString xaxis){
  TString path = "/afs/cern.ch/work/m/mzientek/public/25ns_v80X_v4/";
  TString mainCut = "1";  
  gStyle->SetOptStat(0);

  // read file
  TString filename = "2HDM_mZP600_mA0300.root";
  TString name = "2HDM_mZP600_mA0300_SlewRate_seedEn";
  TFile file(TString::Format("%s%s",path.Data(),filename.Data()));
  TTree* tree = (TTree*) file.Get("DiPhotonTree");
  bool isMC = (filename=="DoubleEG.root" || filename=="DoubleEG_skimmedtree.root" || filename=="DoubleEG_skimmedtree_origSel.root")? false:true;

  // get histos to compare
  TH1F *h_1; 
  TH1F *h_2;
  TH1F *h_1_e1;
  TH1F *h_2_e1;
  TH1F *h_1_e2;
  TH1F *h_2_e2;
  TH1F *h_1_e3;
  TH1F *h_2_e3;

  TCanvas* c = new TCanvas("c","",1);
  c->cd();
  c->Draw();
  TPad* pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 1.0, 1.0);
  pad1->Draw();
  pad1->cd();

  h_1 = new TH1F("h_1","h_1", BINS, MIN, MAX);  
  h_2 = new TH1F("h_2","h_2", BINS, MIN, MAX);  
  h_1_e1 = new TH1F("h_1_e1","h_1_e1", BINS, MIN, MAX);  
  h_2_e1 = new TH1F("h_2_e1","h_2_e1", BINS, MIN, MAX);  
  h_1_e2 = new TH1F("h_1_e2","h_1_e2", BINS, MIN, MAX);  
  h_2_e2 = new TH1F("h_2_e2","h_2_e2", BINS, MIN, MAX);  
  h_1_e3 = new TH1F("h_1_e3","h_1_e3", BINS, MIN, MAX);  
  h_2_e3 = new TH1F("h_2_e3","h_2_e3", BINS, MIN, MAX);  

  Float_t weight;	tree->SetBranchAddress("weight",&weight);
  Float_t seedEnergy1;	tree->SetBranchAddress("seedEnergy1",&seedEnergy1);
  Float_t seedEnergy2;	tree->SetBranchAddress("seedEnergy2",&seedEnergy2);
  Float_t massOrig;	tree->SetBranchAddress("massOrig",&massOrig);
  Float_t massRaw;	tree->SetBranchAddress("massRaw",&massRaw);

  Float_t numEvents = 0;
  Float_t numCorr = 0;

  for (UInt_t entry=0; entry<tree->GetEntries(); entry++){
    tree->GetEntry(entry);
 
    numEvents += weight;
    if (seedEnergy1 > 200 || seedEnergy2 > 200) numCorr += weight;

    h_1->Fill(massOrig,weight);
    h_2->Fill(massRaw,weight);

    float seedEn = TMath::Max(seedEnergy1,seedEnergy2);
    bool seedEn1 = (seedEn <= 150);
    bool seedEn2 = (seedEn > 150 && seedEn <= 300);
    bool seedEn3 = (seedEn > 300);

    if (seedEn1) h_1_e1->Fill(massOrig,weight);
    if (seedEn1) h_2_e1->Fill(massRaw,weight);

    if (seedEn2) h_1_e2->Fill(massOrig,weight);
    if (seedEn2) h_2_e2->Fill(massRaw,weight);

    if (seedEn3) h_1_e3->Fill(massOrig,weight);
    if (seedEn3) h_2_e3->Fill(massRaw,weight);

  }

  Float_t deltaM0 = (h_2->GetMean()-h_1->GetMean());
  Float_t deltaM1 = (h_2_e1->GetMean()-h_1_e1->GetMean());
  Float_t deltaM2 = (h_2_e2->GetMean()-h_1_e2->GetMean());
  Float_t deltaM3 = (h_2_e3->GetMean()-h_1_e3->GetMean());


  Float_t fracCorr = numCorr/numEvents;
  std::cout << "Fraction of events corrected = " << fracCorr << std::endl;
  std::cout << "deltaM overall               = " << deltaM0  << std::endl;
  std::cout << "deltaM mid                   = " << deltaM2  << std::endl;
  std::cout << "deltaM high                  = " << deltaM3  << std::endl;

  h_1->SetLineColor(kBlack);
  h_1->SetFillColor(kBlack);
  h_1->SetFillStyle(3013);
  h_1->SetTitle("");
  h_1->GetYaxis()->SetTitle("Events");
  h_1->GetXaxis()->SetTitle(xaxis);
  h_2->SetLineColor(kRed);
  h_2->SetLineWidth(2);
  h_2->SetTitle("");

  h_1_e1->SetLineColor(kBlack);
  h_1_e1->SetFillColor(kBlack);
  h_1_e1->SetFillStyle(3013);
  h_1_e1->SetTitle("");
  h_1_e1->GetYaxis()->SetTitle("Events");
  h_1_e1->GetXaxis()->SetTitle(xaxis);
  h_2_e1->SetLineColor(kRed);
  h_2_e1->SetLineWidth(2);
  h_2_e1->SetTitle("");

  h_1_e2->SetLineColor(kBlack);
  h_1_e2->SetFillColor(kBlack);
  h_1_e2->SetFillStyle(3013);
  h_1_e2->SetTitle("");
  h_1_e2->GetYaxis()->SetTitle("Events");
  h_1_e2->GetXaxis()->SetTitle(xaxis);
  h_2_e2->SetLineColor(kRed);
  h_2_e2->SetLineWidth(2);
  h_2_e2->SetTitle("");

  h_1_e3->SetLineColor(kBlack);
  h_1_e3->SetFillColor(kBlack);
  h_1_e3->SetFillStyle(3013);
  h_1_e3->SetTitle("");
  h_1_e3->GetYaxis()->SetTitle("Events");
  h_1_e3->GetXaxis()->SetTitle(xaxis);
  h_2_e3->SetLineColor(kRed);
  h_2_e3->SetLineWidth(2);
  h_2_e3->SetTitle("");

  //// normalize the histos
  //h_1->Scale(1.0/h_1->Integral());
  //h_2->Scale(1.0/h_2->Integral());
  //h_1_e1->Scale(1.0/h_1_e1->Integral());
  //h_2_e1->Scale(1.0/h_2_e1->Integral());
  //h_1_e2->Scale(1.0/h_1_e2->Integral());
  //h_2_e2->Scale(1.0/h_2_e2->Integral());
  //h_1_e3->Scale(1.0/h_1_e3->Integral());
  //h_2_e3->Scale(1.0/h_2_e3->Integral());
  
  // setup the max
  Double_t max_all = TMath::Max(h_1->GetMaximum(), h_2->GetMaximum()); 
  Double_t max_e1 = TMath::Max(h_1_e1->GetMaximum(), h_2_e1->GetMaximum()); 
  Double_t max_e2 = TMath::Max(h_1_e2->GetMaximum(), h_2_e2->GetMaximum());
  Double_t max_e3 = TMath::Max(h_1_e3->GetMaximum(), h_2_e3->GetMaximum());
  h_1->SetMaximum(max_all*1.5);
  h_1_e1->SetMaximum(max_e1*1.5);
  h_1_e2->SetMaximum(max_e2*1.5);
  h_1_e3->SetMaximum(max_e3*1.5);

  // setup legend 
  TLegend *leg1 = new TLegend(0.6,0.7,0.8,0.85); // (x1,y1,x2,y2)
  leg1->SetBorderSize(4);
  leg1->SetTextSize(0.03);
  leg1->SetLineWidth(2);
  leg1->AddEntry(h_1,"Uncorrected mgg","F");
  leg1->AddEntry(h_2,"Slew-rate Corr. mgg","L");

  int iPos=11;
  CMS_lumi(pad1, true, iPos);
  pad1->RedrawAxis();
  c->cd();

  h_1->Draw("HIST");
  h_2->Draw("HIST SAME");
  leg1->Draw("SAME");

  c->SetLogy(0);
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s0_lin.png",name.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s0_lin.pdf",name.Data()));
  c->SetLogy(1);
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s0_log.png",name.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s0_log.pdf",name.Data()));

  h_1_e1->Draw("HIST");
  h_2_e1->Draw("HIST SAME");
  leg1->Draw("SAME");

  c->SetLogy(0);
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s1_lin.png",name.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s1_lin.pdf",name.Data()));
  c->SetLogy(1);
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s1_log.png",name.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s1_log.pdf",name.Data()));

  c->cd();
  h_1_e2->Draw("HIST");
  h_2_e2->Draw("HIST SAME");
  leg1->Draw("SAME");

  c->SetLogy(0);
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s2_lin.png",name.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s2_lin.pdf",name.Data()));
  c->SetLogy(1);
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s2_log.png",name.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s2_log.pdf",name.Data()));

  c->cd();
  h_1_e3->Draw("HIST");
  h_2_e3->Draw("HIST SAME");
  leg1->Draw("SAME");

  c->SetLogy(0);
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s3_lin.png",name.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s3_lin.pdf",name.Data()));
  c->SetLogy(1);
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s3_log.png",name.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV_v80X_v4/slewrate/plots_80X_%s3_log.pdf",name.Data()));

}



