#include <TH1.h>
#include <THStack.h>
#include <TFile.h>
#include <TTree.h>
#include <TLegend.h>

#include <iostream>
#include <fstream>
#include <iomanip>




void OptScan(int zPMass,int mass,int type, double* cuts){

  TString path = "/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/";

  TFile *sig;
  if(type==0) sig = TFile::Open(Form("%s2HDM_mZP%d_mA0%d_rawskim.root",path.Data(),zPMass,mass),"READ");
  if(type==1) sig = TFile::Open(Form("%sBaryonicZp_mZP%d_mChi%d_rawskim.root",path.Data(),zPMass,mass),"READ");
  if(type==2) sig = TFile::Open(Form("%sScalarZp_mZP%d_mChi%d_rawskim.root",path.Data(),zPMass,mass),"READ");
  
  TTree *tree_sig = (TTree*) sig->Get("DiPhotonTree"); 
  TChain *tree_bkg1 = new TChain();

  tree_bkg1->Add(Form("%sTTGG_0Jets_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sTTGJets_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sTTJets_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sTGJets_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sZGTo2LG_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sZGTo2NuG_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sZZTo2L2Q_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sWGToLNuG_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sWJetsToLNu_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sWZTo2L2Q_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sVBFHToGG_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sttHJetToGG_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sVHToGG_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sGluGluHToGG_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sQCD_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sGJets_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sDYJetsToLL_rawskim.root/DiPhotonTree",path.Data()));
  tree_bkg1->Add(Form("%sDiPhoton_rawskim.root/DiPhotonTree",path.Data()));
   
  TCut Cut_pt1;
  TCut Cut_pt2;
  TCut Cut_met;
  TCut Cut_ptgg;
  TCut mggmax = "mgg<130"; //signal region
  TCut mggmin = "mgg>120";

  TCut metcut = "t1pfmetCorr > 30 && t1pfmetCorr < 130";
  TCut dphi1 = "dphiggmet >= 2.1";
  TCut dphi2 = "mindphijmet >= 0.5";
  TCut njet  = "nJets30 <= 2";
  TCut nel   = "nEle == 0";
  TCut nmu   = "nMuons == 0";
 
  //TCut eveto1 = "eleveto1==1";
  //TCut eveto2 = "eleveto2==1";
  //TCut eveto = eveto1 && eveto2;
  //TCut genmatch = "((genmatch1==1 && genmatch2==0)||(genmatch1==0 && genmatch2==1)||(genmatch1==0 && genmatch2==0))";  
  
  
  TCanvas *canvas = new TCanvas("canvas","",600,400);   
  int i = 0;
  int j = 0;
  int k = 0;
  int l = 0;
  
  double  cut_pt1 = 0;
  double  cut_pt2 = 0;
  double  cut_met = 0;
  double  cut_ptgg = 0;  
  
  double Stot = 0;
  
  double S = 0;
  double F1 = 0;
  double F2 = 0;
  double F3 = 0;
  double F4 = 0;
  double F5 = 0;
  double F6 = 0;
  double F7 = 0;
  double F8 = 0;
  double F9 = 0;
  double F10 = 0;
  double F11 = 0;
  double F12 = 0; 
  double F13 = 0; 
  double F14 = 0; 
  double B = 0;
  double Eff = 0;
  
 
  double Sig3 = 0;
  double Sig5 = 0;

  // number of steps of the scan for each variable
  int NstepsPt1  = 1; 
  int NstepsPt2  = 1; 
  int NstepsMET  = 10; 
  int NstepsPtgg = 10; 
  
  double signMax = 0;
  double OldSig5 = 0;
  double optCut_pt1 = 0;
  double optCut_pt2 = 0;
  double optCut_met = 0;
  double optCut_ptgg = 0;
  double temp = 0;
  
  double firstCut[4];
// value for the first step of the scan for each variable
  firstCut[0]=0.30; //pt1
  firstCut[1]=0.25; //pt2
  firstCut[2]=30;   //MET
  firstCut[3]=30;   //ptgg
  
  cout<<"### Signal mZP"<<zPMass<<" mA0"<<mass<<" ###"<<endl;
  cout<<"Signal region 120 < mgg < 130"<<endl;  
  ios_base::fmtflags oldflags = std::cout.flags();

  TH1F *mgg_1 =new TH1F("mgg_1", "mgg_1", 30,100,180);
  TH1F *mgg_bkg1 =new TH1F("mgg_bkg1", "mgg_bkg1", 30,100,180);
  
  
  
  for(i = 0; i<NstepsPt1;i++){
    cut_pt1 = firstCut[0] + 0.05*i;
    Cut_pt1 = Form("pt1/mgg>%lf",cut_pt1);
    for(j = 0; j<NstepsPt2;j++){
      cut_pt2 = firstCut[1] + 0.05*j;
      Cut_pt2 = Form("pt2/mgg>%lf",cut_pt2);
      for(k = 0; k<NstepsMET;k++){
	cut_met = firstCut[2] + 5*k;
	Cut_met = Form("t1pfmetCorr>%lf",cut_met);
	tree_sig->Draw("(mgg)>>mgg_tot(30,100,180)","weight"*(mggmin && mggmax && metcut && dphi1 && dphi2 /*&& njet && nmu && nel*/));
	TH1F *mgg_tot =(TH1F*)gPad->GetPrimitive("mgg_tot"); 
	
	Stot = mgg_tot->Integral();  
	//	cout<<"pt1 > "<<cut_pt1<<"; pt2 > "<<cut_pt2<<"; met > "<<cut_met<<"; Stot ="<<Stot<<endl;
	//cout<<"cut\tS\tDiPho\tDY\tGJets\tggH\tQCD\tVH\tB\tEff\tSig(3sigma)\tSig(5sigma)"<<endl;
	for(l = 0; l<NstepsPtgg;l++){
	  cut_ptgg = firstCut[3]+ 5*l;
	  Cut_ptgg = Form("ptgg>%lf",cut_ptgg);
	  
	  Sig5 = 0.;
	  
	  tree_sig->Draw("(mgg)>>mgg_1","weight"*(Cut_met && Cut_pt2 && Cut_pt1&& Cut_ptgg && mggmin && mggmax && metcut && dphi1 && dphi2 /*&& njet && nel && nmu*/));
	  tree_bkg1->Draw("(mgg)>>mgg_bkg1","weight"*(Cut_met && Cut_pt2 && Cut_pt1 && Cut_ptgg && mggmin && mggmax && metcut && dphi1 && dphi2 /*&& njet && nel && nmu*/));
	  
	  S = mgg_1->Integral();
	  F1 = mgg_bkg1->Integral();  
	  if(F1 < 0) F1 = 0;
	  B = F1;//+F2+F3+F4+F5+F6+F7+F8+F9+F10+F11+F12+F13+F14;
	  Eff = S/Stot;
	  Sig3 = Eff/(1.5+sqrt(B));
	  Sig5 = Eff/(2.5+sqrt(B));
	  //  if(l==1){// printing the results only for 1 ptgg cut for each cycle, just a flag to see that it's working
	  //cout<<fixed<<setprecision(2);
	  cout<<cut_met<<"\t"<<Sig3<<"\t"<<Sig5<<endl;
	  cout.flags(oldflags);
	  cout.precision(7);
	  //cout<<"\t"<<Sig3<<"\t"<<Sig5<<endl;
	  // }
	  
	  if(Sig5>OldSig5){
	    temp = Sig5;  
	  }      
	  if(temp > signMax){
	    signMax = Sig5;
	    optCut_pt1 = cut_pt1;
	    optCut_pt2 = cut_pt2;
	    optCut_met = cut_met; 
	    optCut_ptgg = cut_ptgg;
	  }
	  OldSig5 = Sig5;
	}    
	cout<<""<<endl;
      }
      cout<<""<<endl;
    }
    cout<<""<<endl;
  }
  
  
  canvas->cd();
  cout<<"##################"<<endl;
  
  cout<<"### Signal mZP"<<zPMass<<" mA0"<<mass<<" ###"<<endl;
  cout<<"Signal region 120 < mgg < 130"<<endl;  
  cout<<"Optimal cuts:"<<endl;
  cout<<"pt1/mgg > "<<optCut_pt1<<endl;
  cout<<"pt2/mgg > "<<optCut_pt2<<endl;
  cout<<"MET > "<<optCut_met<<" GeV"<<endl;
  cout<<"ptgg > "<<optCut_ptgg<<" GeV"<<endl;
  cuts[0]=optCut_pt1;
  cuts[1]=optCut_pt2;
  cuts[2]=optCut_met;
  cuts[3]=optCut_ptgg;


}



 
  


void allScan2HDM(){

  double mZp[8]={600,800,1000,1200,1400,1700,2000,2500};
  double mA0[1]={300};
  double cuts_mZp600[4];
  double cuts_mZp800[4];
  double cuts_mZp1000[4];
  double cuts_mZp1200[4];
  double cuts_mZp1400[4];
  double cuts_mZp1700[4];
  double cuts_mZp2000[4];
  double cuts_mZp2500[4];
  int type=0;
  for(int i=0;i<8;i++){
    for(int j=0;j<1;j++){
      if(i==0) OptScan(mZp[i],mA0[j],type,cuts_mZp600);
      if(i==1) OptScan(mZp[i],mA0[j],type,cuts_mZp800);
      if(i==2) OptScan(mZp[i],mA0[j],type,cuts_mZp1000);
      if(i==3) OptScan(mZp[i],mA0[j],type,cuts_mZp1200);
      if(i==4) OptScan(mZp[i],mA0[j],type,cuts_mZp1400);
      if(i==5) OptScan(mZp[i],mA0[j],type,cuts_mZp1700);
      if(i==6) OptScan(mZp[i],mA0[j],type,cuts_mZp2000);
      if(i==7) OptScan(mZp[i],mA0[j],type,cuts_mZp2500);
    }

  }

  double pt1cuts[8]= {cuts_mZp600[0],cuts_mZp800[0],cuts_mZp1000[0],cuts_mZp1200[0],cuts_mZp1400[0],cuts_mZp1700[0],cuts_mZp2000[0],cuts_mZp2500[0]};
  double pt2cuts[8]= {cuts_mZp600[1],cuts_mZp800[1],cuts_mZp1000[1],cuts_mZp1200[1],cuts_mZp1400[1],cuts_mZp1700[1],cuts_mZp2000[1],cuts_mZp2500[1]};
  double metcuts[8]= {cuts_mZp600[2],cuts_mZp800[2],cuts_mZp1000[2],cuts_mZp1200[2],cuts_mZp1400[2],cuts_mZp1700[2],cuts_mZp2000[2],cuts_mZp2500[2]};
  double ptggcuts[8]= {cuts_mZp600[3],cuts_mZp800[3],cuts_mZp1000[3],cuts_mZp1200[3],cuts_mZp1400[3],cuts_mZp1700[3],cuts_mZp2000[3],cuts_mZp2500[3]};

  TGraph* g_pt1 =new TGraph(8,mZp,pt1cuts);
  TGraph* g_pt2 =new TGraph(8,mZp,pt2cuts);
  TGraph* g_met =new TGraph(8,mZp,metcuts);
  TGraph* g_ptgg=new TGraph(8,mZp,ptggcuts);

  g_pt1->SetMarkerStyle(8);
  g_pt2->SetMarkerStyle(8);
  g_met->SetMarkerStyle(8);
  g_ptgg->SetMarkerStyle(8);

  TCanvas* c = new TCanvas("c","c",1);
  g_pt1->Draw("AP");
  c->SaveAs("~/www/plotsMonoH/Optimization-2HDM/pt1cuts.png");

  g_pt2->Draw("AP");
  c->SaveAs("~/www/plotsMonoH/Optimization-2HDM/pt2cuts.png");

  g_met->Draw("AP");
  c->SaveAs("~/www/plotsMonoH/Optimization-2HDM/metcuts.png");

  g_ptgg->Draw("AP");
  c->SaveAs("~/www/plotsMonoH/Optimization-2HDM/ptggcuts.png");

}


void allScanBaryo(){

  double mZp[4]={10,100,500,1000};
  double mA0[1]={1};
  double cuts_m1[4];
  double cuts_m2[4];
  double cuts_m3[4];
  double cuts_m4[4];

  int type=1;
  for(int i=0;i<4;i++){
    for(int j=0;j<1;j++){
      if(i==0) OptScan(mZp[i],mA0[j],type,cuts_m1);
      if(i==1) OptScan(mZp[i],mA0[j],type,cuts_m2);
      if(i==2) OptScan(mZp[i],mA0[j],type,cuts_m3);
      if(i==3) OptScan(mZp[i],mA0[j],type,cuts_m4);
    }
  }

  double pt1cuts[4] = {cuts_m1[0],cuts_m2[0],cuts_m3[0],cuts_m4[0]};
  double pt2cuts[4] = {cuts_m1[1],cuts_m2[1],cuts_m3[1],cuts_m4[1]};
  double metcuts[4] = {cuts_m1[2],cuts_m2[2],cuts_m3[2],cuts_m4[2]};
  double ptggcuts[4]= {cuts_m1[3],cuts_m2[3],cuts_m3[3],cuts_m4[3]};

  TGraph* g_pt1 =new TGraph(4,mZp,pt1cuts);
  TGraph* g_pt2 =new TGraph(4,mZp,pt2cuts);
  TGraph* g_met =new TGraph(4,mZp,metcuts);
  TGraph* g_ptgg=new TGraph(4,mZp,ptggcuts);

  g_pt1->SetMarkerStyle(8);
  g_pt2->SetMarkerStyle(8);
  g_met->SetMarkerStyle(8);
  g_ptgg->SetMarkerStyle(8);

  TCanvas* c = new TCanvas("c","c",1);
  g_pt1->Draw("AP");
  c->SaveAs("~/www/plotsMonoH/Optimization-Bary/pt1cuts.png");

  g_pt2->Draw("AP");
  c->SaveAs("~/www/plotsMonoH/Optimization-Bary/pt2cuts.png");

  g_met->Draw("AP");
  c->SaveAs("~/www/plotsMonoH/Optimization-Bary/metcuts.png");

  g_ptgg->Draw("AP");
  c->SaveAs("~/www/plotsMonoH/Optimization-Bary/ptggcuts.png");

}




void allScanScal(){

  double mZp[4]={10,100,500,1000};
  double mA0[1]={1};
  double cuts_m1[4];
  double cuts_m2[4];
  double cuts_m3[4];
  double cuts_m4[4];

  int type=2;
  for(int i=0;i<4;i++){
    for(int j=0;j<1;j++){
      if(i==0) OptScan(mZp[i],mA0[j],type,cuts_m1);
      if(i==1) OptScan(mZp[i],mA0[j],type,cuts_m2);
      if(i==2) OptScan(mZp[i],mA0[j],type,cuts_m3);
      if(i==3) OptScan(mZp[i],mA0[j],type,cuts_m4);

    }

  }

  double pt1cuts[4] = {cuts_m1[0],cuts_m2[0],cuts_m3[0],cuts_m4[0]};
  double pt2cuts[4] = {cuts_m1[1],cuts_m2[1],cuts_m3[1],cuts_m4[1]};
  double metcuts[4] = {cuts_m1[2],cuts_m2[2],cuts_m3[2],cuts_m4[2]};
  double ptggcuts[4]= {cuts_m1[3],cuts_m2[3],cuts_m3[3],cuts_m4[3]};

  TGraph* g_pt1 =new TGraph(4,mZp,pt1cuts);
  TGraph* g_pt2 =new TGraph(4,mZp,pt2cuts);
  TGraph* g_met =new TGraph(4,mZp,metcuts);
  TGraph* g_ptgg=new TGraph(4,mZp,ptggcuts);

  g_pt1->SetMarkerStyle(8);
  g_pt2->SetMarkerStyle(8);
  g_met->SetMarkerStyle(8);
  g_ptgg->SetMarkerStyle(8);

  TCanvas* c = new TCanvas("c","c",1);
  g_pt1->Draw("AP");
  c->SaveAs("~/www/plotsMonoH/Optimization-Scal/pt1cuts.png");

  g_pt2->Draw("AP");
  c->SaveAs("~/www/plotsMonoH/Optimization-Scal/pt2cuts.png");

  g_met->Draw("AP");
  c->SaveAs("~/www/plotsMonoH/Optimization-Scal/metcuts.png");

  g_ptgg->Draw("AP");
  c->SaveAs("~/www/plotsMonoH/Optimization-Scal/ptggcuts.png");

}

