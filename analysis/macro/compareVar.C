#include "mkPlotsLivia/CMS_lumi.C"
using namespace RooFit;
using namespace RooStats;

std::string filePOSTfix="";

void MakeComparisonPlots(TString, TString, int, double, double, TString);


void compareVar(){
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
  TString filename = "DoubleEG_skimmedtree_origSel.root";
  TFile file(TString::Format("%s%s",path.Data(),filename.Data()));
  TTree* tree = (TTree*) file.Get("DiPhotonTree");
  bool isMC = (filename=="DoubleEG.root" || filename=="DoubleEG_skimmedtree.root" || filename=="DoubleEG_skimmedtree_origSel.root")? false:true;

  // get histos to compare
  TH1F *h_1;
  TH1F *h_2;
  TH1F *h_3;

  TCanvas* c = new TCanvas("c","",1);
  c->cd();
  c->Draw();
  TPad* pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 1.0, 1.0);
  pad1->Draw();
  pad1->cd();

  h_1 = new TH1F(TString::Format("h_%s",var1.Data()),TString::Format("h_%s",var1.Data()), BINS, MIN, MAX);  
  h_2 = new TH1F(TString::Format("h_%s",var2.Data()),TString::Format("h_%s",var2.Data()), BINS, MIN, MAX);  
  h_3 = new TH1F("h_ourMetCorr","h_ourMetCorr", BINS, MIN, MAX);  

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
  Float_t t1pfmet;      tree->SetBranchAddress("t1pfmet",&t1pfmet);
  Float_t t1pfmetPhi;   tree->SetBranchAddress("t1pfmetPhi",&t1pfmetPhi);
  Float_t t1pfmetSumEt; tree->SetBranchAddress("t1pfmetSumEt",&t1pfmetSumEt);
  Float_t weight;	tree->SetBranchAddress("weight",&weight);
  Float_t variable1;	tree->SetBranchAddress(TString::Format("%s",var1.Data()),&variable1);
  Float_t variable2;	tree->SetBranchAddress(TString::Format("%s",var2.Data()),&variable2);

  for (UInt_t entry=0; entry<tree->GetEntries(); entry++){
    tree->GetEntry(entry);
  
    h_1->Fill(variable1,weight);
    h_2->Fill(variable2,weight);

    if (var1=="t1pfmet")    t1pfmet = variable1;
    if (var1=="t1pfmetPhi") t1pfmetPhi = variable1; 

    if (!isMC){ 
      t1pfmetCorrX = t1pfmet*TMath::Cos(t1pfmetPhi) - (fMETCorrData[0] + fMETCorrData[1]*t1pfmetSumEt);
      t1pfmetCorrY = t1pfmet*TMath::Sin(t1pfmetPhi) - (fMETCorrData[2] + fMETCorrData[3]*t1pfmetSumEt);
    } 
    else{
      t1pfmetCorrX = t1pfmet*TMath::Cos(t1pfmetPhi) - (fMETCorrMC[0] + fMETCorrMC[1]*t1pfmetSumEt);
      t1pfmetCorrY = t1pfmet*TMath::Sin(t1pfmetPhi) - (fMETCorrMC[2] + fMETCorrMC[3]*t1pfmetSumEt);
    } 
    t1pfmetOurCorrMET = TMath::Sqrt(t1pfmetCorrX*t1pfmetCorrX + t1pfmetCorrY*t1pfmetCorrY);
    TLorentzVector correctedMet;
    correctedMet.SetPxPyPzE(t1pfmetCorrX,t1pfmetCorrY,0,t1pfmetSumEt);
    t1pfmetOurCorrPhi = correctedMet.Phi();

    if (var1=="t1pfmet")    h_3->Fill(t1pfmetOurCorrMET,weight);
    if (var1=="t1pfmetPhi") h_3->Fill(t1pfmetOurCorrPhi,weight);
  }

  // draw histos
  //tree->Draw(var1+TString::Format(">>h_%s",var1.Data()),"("+mainCut+")*weight");
  //tree->Draw(var2+TString::Format(">>h_%s",var2.Data()),"("+mainCut+")*weight");
  h_1->SetLineColor(kBlack);
  h_1->SetTitle("");
  h_1->GetYaxis()->SetTitle("arb. units");
  h_1->GetXaxis()->SetTitle(xaxis);
  h_2->SetLineColor(kRed);
  h_2->SetTitle("");
  h_3->SetLineColor(kBlue);
  h_3->SetTitle("");

  // normalize the histos
  Double_t int1 = h_1->Integral();
  if (int1 > 0) h_1->Scale(1.0/int1);
  Double_t int2 = h_2->Integral();
  if (int2 > 0) h_2->Scale(1.0/int2);
  Double_t int3 = h_3->Integral();
  if (int3 > 0) h_3->Scale(1.0/int3);
  
  // setup the max 
  Double_t max1 = h_1->GetMaximum();
  Double_t max2 = h_2->GetMaximum();
  Double_t max3 = 0;
  if (var1=="t1pfmet" || var1=="t1pfmetPhi") max3 = h_3->GetMaximum();
  Double_t tempmax = (max1 > max2)?    max1:max2;
  Double_t theMax  = (max3 > tempmax)? max3:tempmax;

  h_1->SetMaximum(theMax*1.5);

  // setup legend 
  TLegend *leg1 = new TLegend(0.2,0.2,0.5,0.3); // (x1,y1,x2,y2)
  leg1->SetBorderSize(4);
  leg1->SetTextSize(0.03);
  leg1->SetLineWidth(2);
  leg1->AddEntry(h_1,TString::Format("%s",var1.Data()),"L");
  leg1->AddEntry(h_2,TString::Format("%s",var2.Data()),"L");
  //if (var1=="t1pfmet" || var1=="t1pfmetPhi") leg1->AddEntry(h_3,TString::Format("%s_ourCorr",var1.Data()),"L");

  int iPos=11;
  CMS_lumi(pad1, true, iPos);
  pad1->RedrawAxis();
  c->cd();

  h_1->Draw("HIST");
  h_2->Draw("HIST SAME");
  //if (var1=="t1pfmet" || var1=="t1pfmetPhi") h_3->Draw("HIST SAME");
  leg1->Draw("SAME");

  //c->SaveAs(TString::Format("~/www/Plots/13TeV/plots_80X_Data_%s_v_%s.png",var1.Data(),var2.Data()));
  //c->SaveAs(TString::Format("~/www/Plots/13TeV/plots_80X_Data_%s_v_%s.pdf",var1.Data(),var2.Data()));
  //c->SetLogy(1);
  //c->SaveAs(TString::Format("~/www/Plots/13TeV/plots_80X_Data_%s_v_%s_log.png",var1.Data(),var2.Data()));
  //c->SaveAs(TString::Format("~/www/Plots/13TeV/plots_80X_Data_%s_v_%s_log.pdf",var1.Data(),var2.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV/plots_80X_Clean_Data_%s_v_%s.png",var1.Data(),var2.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV/plots_80X_Clean_Data_%s_v_%s.pdf",var1.Data(),var2.Data()));
  c->SetLogy(1);
  c->SaveAs(TString::Format("~/www/Plots/13TeV/plots_80X_Clean_Data_%s_v_%s_log.png",var1.Data(),var2.Data()));
  c->SaveAs(TString::Format("~/www/Plots/13TeV/plots_80X_Clean_Data_%s_v_%s_log.pdf",var1.Data(),var2.Data()));

  delete c;

}



