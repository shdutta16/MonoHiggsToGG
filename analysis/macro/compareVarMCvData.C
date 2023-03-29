#include "mkPlotsLivia/CMS_lumi.C"
using namespace RooFit;
using namespace RooStats;

std::string filePOSTfix="";

void MakeComparisonPlots(TString, TString, int, double, double, TString);


void compareVarMCvData(){
  cout << "Plot met vs corr met" << endl;
  MakeComparisonPlots("t1pfmet",    "t1pfmetCorr",    60, 0.,  300., "p_{T}^{miss} (GeV)");
  MakeComparisonPlots("t1pfmetPhi", "t1pfmetCorrPhi", 35, -3.5, 3.5, "p_{T}^{miss} #phi");

  return;
}

void MakeComparisonPlots(TString var1, TString var2, int BINS, double MIN, double MAX, TString xaxis){
  TString path = "/afs/cern.ch/work/m/mzientek/public/25ns_v80X_v3/";
  TString mainCut = "1";  
  gStyle->SetOptStat(0);

  // read file
  TString MCfile   = "DiPhoton_skimmedtree.root";
  TString Datafile = "DoubleEG_skimmedtree.root";
  TFile fileMC(TString::Format("%s%s",path.Data(),MCfile.Data()));
  TFile fileDa(TString::Format("%s%s",path.Data(),Datafile.Data()));
  TTree* treeMC = (TTree*) fileMC.Get("DiPhotonTree");
  TTree* treeDa = (TTree*) fileDa.Get("DiPhotonTree");

  // get histos to compare
  TH1F *h_1_MC;
  TH1F *h_2_MC;
  TH1F *h_3_MC;
  TH1F *h_1_Da;
  TH1F *h_2_Da;
  TH1F *h_3_Da;

  TCanvas* c = new TCanvas("c","",1);
  c->cd();
  c->Draw();
  TPad* pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 1.0, 1.0);
  pad1->Draw();
  pad1->cd();

  h_1_MC = new TH1F(TString::Format("h_MC_%s",var1.Data()),TString::Format("h_MC_%s",var1.Data()), BINS, MIN, MAX);  
  h_2_MC = new TH1F(TString::Format("h_MC_%s",var2.Data()),TString::Format("h_MC_%s",var2.Data()), BINS, MIN, MAX);  
  h_3_MC = new TH1F("h_MC_ourMetCorr","h_MC_ourMetCorr", BINS, MIN, MAX);  
  h_1_Da = new TH1F(TString::Format("h_Da_%s",var1.Data()),TString::Format("h_Da_%s",var1.Data()), BINS, MIN, MAX);  
  h_2_Da = new TH1F(TString::Format("h_Da_%s",var2.Data()),TString::Format("h_Da_%s",var2.Data()), BINS, MIN, MAX);  
  h_3_Da = new TH1F("h_Da_ourMetCorr","h_Da_ourMetCorr", BINS, MIN, MAX);  

  // pick up our phi corr
  std::vector< Double_t > fMETCorrMC;
  std::vector< Double_t > fMETCorrData;
  // pick up MC metCorr
  TString metStudyMC = Form("%s/metCorr_MC.root",path.Data());
  TFile *fmetCorrMC = TFile::Open(metStudyMC.Data());
  TH1D *MCmet = (TH1D*)fmetCorrMC->Get("metCorr");
  // pick up Data metCorr
  TString metStudyData = Form("%s/metCorr_Data.root",path.Data());
  TFile *fmetCorrDATA = TFile::Open(metStudyData.Data());
  TH1D *DATAmet = (TH1D*)fmetCorrDATA->Get("metCorr");
  for (UInt_t i=0; i<4; i++){
    fMETCorrMC.push_back(MCmet->GetBinContent(i+1));
    fMETCorrData.push_back(DATAmet->GetBinContent(i+1));
  }

  Double_t t1pfmetCorrX = 0;
  Double_t t1pfmetCorrY = 0;
  Double_t t1pfmetOurCorrPhi = 0;
  Double_t t1pfmetOurCorrMET = 0;
  Float_t t1pfmet;      treeMC->SetBranchAddress("t1pfmet",&t1pfmet);
  Float_t t1pfmetPhi;   treeMC->SetBranchAddress("t1pfmetPhi",&t1pfmetPhi);
  Float_t t1pfmetSumEt; treeMC->SetBranchAddress("t1pfmetSumEt",&t1pfmetSumEt);
  Float_t weight;	treeMC->SetBranchAddress("weight",&weight);
  Float_t variable1;	treeMC->SetBranchAddress(TString::Format("%s",var1.Data()),&variable1);
  Float_t variable2;	treeMC->SetBranchAddress(TString::Format("%s",var2.Data()),&variable2);

  for (UInt_t entry=0; entry<treeMC->GetEntries(); entry++){
    treeMC->GetEntry(entry);
  
    h_1_MC->Fill(variable1,weight);
    h_2_MC->Fill(variable2,weight);

    if (var1=="t1pfmet")    t1pfmet = variable1;
    if (var1=="t1pfmetPhi") t1pfmetPhi = variable1; 

    // MC met-phi corr
    t1pfmetCorrX = t1pfmet*TMath::Cos(t1pfmetPhi) - (fMETCorrMC[0] + fMETCorrMC[1]*t1pfmetSumEt);
    t1pfmetCorrY = t1pfmet*TMath::Sin(t1pfmetPhi) - (fMETCorrMC[2] + fMETCorrMC[3]*t1pfmetSumEt);
    
    t1pfmetOurCorrMET = TMath::Sqrt(t1pfmetCorrX*t1pfmetCorrX + t1pfmetCorrY*t1pfmetCorrY);
    TLorentzVector correctedMet;
    correctedMet.SetPxPyPzE(t1pfmetCorrX,t1pfmetCorrY,0,t1pfmetSumEt);
    t1pfmetOurCorrPhi = correctedMet.Phi();

    if (var1=="t1pfmet")    h_3_MC->Fill(t1pfmetOurCorrMET,weight);
    if (var1=="t1pfmetPhi") h_3_MC->Fill(t1pfmetOurCorrPhi,weight);
  }

  treeDa->SetBranchAddress("t1pfmet",&t1pfmet);
  treeDa->SetBranchAddress("t1pfmetPhi",&t1pfmetPhi);
  treeDa->SetBranchAddress("t1pfmetSumEt",&t1pfmetSumEt);
  treeDa->SetBranchAddress("weight",&weight);
  treeDa->SetBranchAddress(TString::Format("%s",var1.Data()),&variable1);
  treeDa->SetBranchAddress(TString::Format("%s",var2.Data()),&variable2);

  for (UInt_t entry=0; entry<treeDa->GetEntries(); entry++){
    treeDa->GetEntry(entry);
  
    h_1_Da->Fill(variable1,weight);
    h_2_Da->Fill(variable2,weight);

    if (var1=="t1pfmet")    t1pfmet = variable1;
    if (var1=="t1pfmetPhi") t1pfmetPhi = variable1; 

    // Data met-phi corr
    t1pfmetCorrX = t1pfmet*TMath::Cos(t1pfmetPhi) - (fMETCorrData[0] + fMETCorrData[1]*t1pfmetSumEt);
    t1pfmetCorrY = t1pfmet*TMath::Sin(t1pfmetPhi) - (fMETCorrData[2] + fMETCorrData[3]*t1pfmetSumEt);
    
    t1pfmetOurCorrMET = TMath::Sqrt(t1pfmetCorrX*t1pfmetCorrX + t1pfmetCorrY*t1pfmetCorrY);
    TLorentzVector correctedMet;
    correctedMet.SetPxPyPzE(t1pfmetCorrX,t1pfmetCorrY,0,t1pfmetSumEt);
    t1pfmetOurCorrPhi = correctedMet.Phi();

    if (var1=="t1pfmet")    h_3_Da->Fill(t1pfmetOurCorrMET,weight);
    if (var1=="t1pfmetPhi") h_3_Da->Fill(t1pfmetOurCorrPhi,weight);
  }

  // draw histos
  //tree->Draw(var1+TString::Format(">>h_%s",var1.Data()),"("+mainCut+")*weight");
  //tree->Draw(var2+TString::Format(">>h_%s",var2.Data()),"("+mainCut+")*weight");
  h_1_MC->SetLineColor(kBlack);
  h_1_MC->SetTitle("");
  h_1_MC->GetYaxis()->SetTitle("arb. units");
  h_1_MC->GetXaxis()->SetTitle(xaxis);
  h_2_MC->SetLineColor(kRed);
  h_2_MC->SetTitle("");
  h_3_MC->SetLineColor(kBlue);
  h_3_MC->SetTitle("");

  h_1_Da->SetLineColor(kBlack);
  h_1_Da->SetLineStyle(7);
  h_1_Da->SetTitle("");
  h_2_Da->SetLineColor(kRed);
  h_2_Da->SetLineStyle(7);
  h_2_Da->SetTitle("");
  h_3_Da->SetLineColor(kBlue);
  h_3_Da->SetLineStyle(7);
  h_3_Da->SetTitle("");

  // normalize the histos
  Double_t int1MC = h_1_MC->Integral();
  if (int1MC > 0) h_1_MC->Scale(1.0/int1MC);
  Double_t int2MC = h_2_MC->Integral();
  if (int2MC > 0) h_2_MC->Scale(1.0/int2MC);
  Double_t int3MC = h_3_MC->Integral();
  if (int3MC > 0) h_3_MC->Scale(1.0/int3MC);

  Double_t int1Da = h_1_Da->Integral();
  if (int1Da > 0) h_1_Da->Scale(1.0/int1Da);
  Double_t int2Da = h_2_Da->Integral();
  if (int2Da > 0) h_2_Da->Scale(1.0/int2Da);
  Double_t int3Da = h_3_Da->Integral();
  if (int3Da > 0) h_3_Da->Scale(1.0/int3Da);
  
  // setup the max 
  Double_t max1MC = h_1_MC->GetMaximum();
  Double_t max2MC = h_2_MC->GetMaximum();
  Double_t max1Da = h_1_MC->GetMaximum();
  Double_t max2Da = h_2_MC->GetMaximum();
  Double_t max3MC = 0;
  Double_t max3Da = 0;
  if (var1=="t1pfmet" || var1=="t1pfmetPhi") max3MC = h_3_MC->GetMaximum();
  if (var1=="t1pfmet" || var1=="t1pfmetPhi") max3Da = h_3_Da->GetMaximum();

  Double_t max1 = (max1MC > max1Da)? max1MC:max1Da;
  Double_t max2 = (max2MC > max2Da)? max2MC:max2Da;
  Double_t max3 = (max3MC > max3Da)? max3MC:max3Da;

  Double_t tempmax = (max1 > max2)?    max1:max2;
  Double_t theMax  = (max3 > tempmax)? max3:tempmax;
  h_1_MC->SetMaximum(theMax*1.5);

  // setup legend 
  TLegend *leg1 = new TLegend(0.2,0.2,0.4,0.45); // (x1,y1,x2,y2)
  leg1->SetBorderSize(4);
  leg1->SetTextSize(0.03);
  leg1->SetLineWidth(2);
  leg1->AddEntry(h_1_MC,TString::Format("%s (MC)",var1.Data()),"L");
  leg1->AddEntry(h_1_Da,TString::Format("%s (Data)",var1.Data()),"L");
  leg1->AddEntry(h_2_MC,TString::Format("%s (MC)",var2.Data()),"L");
  leg1->AddEntry(h_2_Da,TString::Format("%s (Data)",var2.Data()),"L");
  if (var1=="t1pfmet" || var1=="t1pfmetPhi") leg1->AddEntry(h_3_MC,TString::Format("%s_ourCorr (MC)",var1.Data()),"L");
  if (var1=="t1pfmet" || var1=="t1pfmetPhi") leg1->AddEntry(h_3_Da,TString::Format("%s_ourCorr (Data)",var1.Data()),"L");

  int iPos=11;
  CMS_lumi(pad1, true, iPos);
  pad1->RedrawAxis();
  c->cd();

  h_1_MC->Draw("HIST");
  h_1_Da->Draw("HIST SAME");
  h_2_MC->Draw("HIST SAME");
  h_2_Da->Draw("HIST SAME");
  if (var1=="t1pfmet" || var1=="t1pfmetPhi") h_3_MC->Draw("HIST SAME");
  if (var1=="t1pfmet" || var1=="t1pfmetPhi") h_3_Da->Draw("HIST SAME");
  leg1->Draw("SAME");

  c->SaveAs(TString::Format("~/www/Plots/13TeV/plots_80X_MCvData_%s_v_%s.png",var1.Data(),var2.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV/plots_80X_MCvData_%s_v_%s.pdf",var1.Data(),var2.Data()));

  c->SetLogy(1);

  c->SaveAs(TString::Format("~/www/Plots/13TeV/plots_80X_MCvData_%s_v_%s_log.png",var1.Data(),var2.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV/plots_80X_MCvData_%s_v_%s_log.pdf",var1.Data(),var2.Data()));

  delete c;

}



