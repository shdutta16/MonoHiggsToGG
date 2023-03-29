#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TGraph2DErrors.h>
#include <TH2.h>
#include <iostream>

using namespace std;

void    GetEfficiencies(TString, TString, TFile*, TString, Float_t, Bool_t, Bool_t);
void    GetEfficiency(TString, TString, TString, Float_t, Bool_t, Bool_t, Bool_t, Float_t &, Float_t &);
void    GetXandY(TString, Double_t &, Double_t &);
void    CalculateNumbers(TTree*, TH1D*, Int_t &, Int_t &, Float_t &, Float_t &);
Float_t CalculateEff(Int_t, Float_t, Int_t, Float_t, Float_t &);

void makeEfficiencyList()
{

  TString indir   = "/afs/cern.ch/work/m/mzientek/public/25ns_v80X_moriond17_v3/";
  TString outdir  = "EffandYieldsTables/";
  Float_t lumi    = 35921.4; //in pb^-1 (for weighting)

  // ----------------------------------------------------------------
  // Which skim to use 
  // ----------------------------------------------------------------
  TString addname="_optB";
  TString skim="skimmedtree"; //default
  if (addname.Contains("optA",TString::kExact)) skim="newskim";
  //if (addname.Contains("optB",TString::kExact)) skim="newskimv2"; // has lep & jet vetoes
  if (addname.Contains("optB",TString::kExact)) skim="skim_woVetos";// no lep or jet vetoes

  // ----------------------------------------------------------------
  // Make root file to store efficiencies 
  // ----------------------------------------------------------------
  TFile *outroot = new TFile(Form("%sOutput_Eff.root",outdir.Data()),"RECREATE");
  TString outfile;

  // ----------------------------------------------------------------
  // Call function for each category 
  // ----------------------------------------------------------------
  cout << "Computing Efficiency highMET region" << endl;
  outfile = Form("%sPlainEff%s_highMET.tex",outdir.Data(),addname.Data());
  GetEfficiencies(indir,outfile,outroot,skim,lumi,true,false);

  cout << "Computing Efficiency lowMET region" << endl;
  outfile = Form("%sPlainEff%s_lowMET.tex",outdir.Data(),addname.Data());
  GetEfficiencies(indir,outfile,outroot,skim,lumi,false,false); 

  cout << "Computing Yields highMET region" << endl;
  outfile = Form("%sYields%s_highMET.tex",outdir.Data(),addname.Data());
  GetEfficiencies(indir,outfile,outroot,skim,lumi,true,true);

  cout << "Computing Yields lowMET region" << endl;
  outfile = Form("%sYields%s_lowMET.tex",outdir.Data(),addname.Data());
  GetEfficiencies(indir,outfile,outroot,skim,lumi,false,true);

}

void GetEfficiencies(TString indir, TString outfile, TFile* outroot, TString skim, Float_t lumi, Bool_t hiMETeff, Bool_t yields)
{
  // ----------------------------------------------------------------
  // Setup histos for output root file
  // ----------------------------------------------------------------
  TString type = (yields)? "Yield":"Eff";
  TString cat  = (hiMETeff)? "HighMET":"LowMET";
  TString name = Form("%s_%s",type.Data(),cat.Data());
  TH2D* dat_2HDM = new TH2D(Form("h_2HDM_%s",name.Data()),Form("h_2HDM_%s",name.Data()),2500,500,3000,700,200,900);
  TH2D* err_2HDM = new TH2D(Form("h_2HDM_%s_err",name.Data()),Form("h_2HDM_%s_err",name.Data()),2500,500,3000,700,200,900);
  TH2D* dat_BARY = new TH2D(Form("h_BARY_%s",name.Data()),Form("h_BARY_%s",name.Data()),10005,0,10005,1500,0,1500); 
  TH2D* err_BARY = new TH2D(Form("h_BARY_%s_err",name.Data()),Form("h_BARY_%s_err",name.Data()),10005,0,10005,1500,0,1500); 
  TGraph2DErrors* eff_2HDM = new TGraph2DErrors();
  TGraph2DErrors* eff_BARY = new TGraph2DErrors(); 
  if (!yields){
    eff_2HDM->SetNameTitle(Form("graph_2HDM_%s",name.Data()),Form("graph_2HDM_%s",name.Data()));
    eff_BARY->SetNameTitle(Form("graph_BARY_%s",name.Data()),Form("graph_BARY_%s",name.Data()));
  }
  Double_t mZp[11]={0.1,15,35,75,150,250,400,750,1500,6000,16000};
  Double_t mDM[9] ={0.1,5,15,25,75,125,325,750,1250};
  TH2F* var_BARY = new TH2F(Form("var_BARY_%s",name.Data()),Form("var_BARY_Eff_%s",name.Data()),10,mZp,8,mDM);

  // ----------------------------------------------------------------
  // samples to use 
  // ----------------------------------------------------------------
  vector< TString > Samples;

  Samples.push_back("2HDM_mZP600_mA0300" );
  Samples.push_back("2HDM_mZP800_mA0300" );
  Samples.push_back("2HDM_mZP1000_mA0300"); 
  Samples.push_back("2HDM_mZP1200_mA0300"); 
  Samples.push_back("2HDM_mZP1400_mA0300"); 
  Samples.push_back("2HDM_mZP1700_mA0300"); 
  Samples.push_back("2HDM_mZP2000_mA0300"); 
  Samples.push_back("2HDM_mZP2500_mA0300"); 
  Samples.push_back("2HDM_mZP600_mA0400" );
  Samples.push_back("2HDM_mZP800_mA0400" );
  Samples.push_back("2HDM_mZP1000_mA0400"); 
  Samples.push_back("2HDM_mZP1200_mA0400"); 
  Samples.push_back("2HDM_mZP1400_mA0400"); 
  Samples.push_back("2HDM_mZP1700_mA0400"); 
  Samples.push_back("2HDM_mZP2000_mA0400"); 
  Samples.push_back("2HDM_mZP2500_mA0400"); 
  Samples.push_back("2HDM_mZP800_mA0500" );
  Samples.push_back("2HDM_mZP1000_mA0500"); 
  Samples.push_back("2HDM_mZP1200_mA0500"); 
  Samples.push_back("2HDM_mZP1400_mA0500"); 
  Samples.push_back("2HDM_mZP1700_mA0500"); 
  Samples.push_back("2HDM_mZP2000_mA0500"); 
  Samples.push_back("2HDM_mZP2500_mA0500"); 
  Samples.push_back("2HDM_mZP800_mA0600" );
  Samples.push_back("2HDM_mZP1000_mA0600"); 
  Samples.push_back("2HDM_mZP1200_mA0600"); 
  Samples.push_back("2HDM_mZP1400_mA0600"); 
  Samples.push_back("2HDM_mZP1700_mA0600"); 
  Samples.push_back("2HDM_mZP2000_mA0600"); 
  Samples.push_back("2HDM_mZP2500_mA0600"); 
  Samples.push_back("2HDM_mZP1000_mA0700"); 
  Samples.push_back("2HDM_mZP1200_mA0700"); 
  Samples.push_back("2HDM_mZP1400_mA0700"); 
  Samples.push_back("2HDM_mZP1700_mA0700"); 
  Samples.push_back("2HDM_mZP2000_mA0700"); 
  Samples.push_back("2HDM_mZP2500_mA0700"); 
  Samples.push_back("2HDM_mZP1000_mA0800"); 
  Samples.push_back("2HDM_mZP1200_mA0800"); 
  Samples.push_back("2HDM_mZP1400_mA0800"); 
  Samples.push_back("2HDM_mZP1700_mA0800"); 
  Samples.push_back("2HDM_mZP2000_mA0800"); 
  Samples.push_back("2HDM_mZP2500_mA0800"); 

  Samples.push_back("BaryonicZp_mZP10000_mChi1000");
  Samples.push_back("BaryonicZp_mZP10000_mChi10"  );
  Samples.push_back("BaryonicZp_mZP10000_mChi150" );
  Samples.push_back("BaryonicZp_mZP10000_mChi1"   );
  Samples.push_back("BaryonicZp_mZP10000_mChi500" );
  Samples.push_back("BaryonicZp_mZP10000_mChi50"  );
  Samples.push_back("BaryonicZp_mZP1000_mChi1000" );
  Samples.push_back("BaryonicZp_mZP1000_mChi150"  );
  Samples.push_back("BaryonicZp_mZP1000_mChi1"    );
  Samples.push_back("BaryonicZp_mZP100_mChi10"    );
  Samples.push_back("BaryonicZp_mZP100_mChi1"     );
  Samples.push_back("BaryonicZp_mZP10_mChi1000"   );
  Samples.push_back("BaryonicZp_mZP10_mChi10"     );
  Samples.push_back("BaryonicZp_mZP10_mChi1"      );
  Samples.push_back("BaryonicZp_mZP10_mChi50"     );
  Samples.push_back("BaryonicZp_mZP10_mChi150"    );
  Samples.push_back("BaryonicZp_mZP10_mChi500"    );
  //Samples.push_back("BaryonicZp_mZP15_mChi10"   );
  Samples.push_back("BaryonicZp_mZP1995_mChi1000" );
  Samples.push_back("BaryonicZp_mZP2000_mChi1"    );
  Samples.push_back("BaryonicZp_mZP2000_mChi500"  );
  Samples.push_back("BaryonicZp_mZP200_mChi150"   );
  Samples.push_back("BaryonicZp_mZP200_mChi1"     );
  Samples.push_back("BaryonicZp_mZP200_mChi50"    );
  Samples.push_back("BaryonicZp_mZP20_mChi1"      );
  Samples.push_back("BaryonicZp_mZP295_mChi150"   );
  Samples.push_back("BaryonicZp_mZP300_mChi50"    );
  Samples.push_back("BaryonicZp_mZP300_mChi1"     );
  Samples.push_back("BaryonicZp_mZP500_mChi150"   );
  Samples.push_back("BaryonicZp_mZP500_mChi1"     );
  Samples.push_back("BaryonicZp_mZP500_mChi500"   );
  Samples.push_back("BaryonicZp_mZP50_mChi50"     );
  Samples.push_back("BaryonicZp_mZP50_mChi10"     );
  Samples.push_back("BaryonicZp_mZP50_mChi1"      );
  Samples.push_back("BaryonicZp_mZP95_mChi50"     );
  Samples.push_back("BaryonicZp_mZP995_mChi500"   );

  //Samples.push_back("ScalarZp_mZP10000_mChi1"   );
  //Samples.push_back("ScalarZp_mZP10000_mChi10"  );
  //Samples.push_back("ScalarZp_mZP10000_mChi1000");
  //Samples.push_back("ScalarZp_mZP10000_mChi150" );
  //Samples.push_back("ScalarZp_mZP10000_mChi50"  );
  //Samples.push_back("ScalarZp_mZP10000_mChi500" );
  //Samples.push_back("ScalarZp_mZP1000_mChi1000" );
  //Samples.push_back("ScalarZp_mZP1000_mChi1"    );
  //Samples.push_back("ScalarZp_mZP1000_mChi150"  );
  //Samples.push_back("ScalarZp_mZP100_mChi1"     );
  //Samples.push_back("ScalarZp_mZP100_mChi10"    );
  //Samples.push_back("ScalarZp_mZP10_mChi1"      );
  //Samples.push_back("ScalarZp_mZP10_mChi10"     );
  //Samples.push_back("ScalarZp_mZP10_mChi150"    );
  //Samples.push_back("ScalarZp_mZP10_mChi50"     );
  //Samples.push_back("ScalarZp_mZP10_mChi1000"   );
  //Samples.push_back("ScalarZp_mZP10_mChi500"    );
  //Samples.push_back("ScalarZp_mZP200_mChi1"     );
  //Samples.push_back("ScalarZp_mZP200_mChi150"   );
  //Samples.push_back("ScalarZp_mZP200_mChi50"    );
  //Samples.push_back("ScalarZp_mZP20_mChi1"      );
  //Samples.push_back("ScalarZp_mZP295_mChi150"   );
  //Samples.push_back("ScalarZp_mZP300_mChi1"     );
  //Samples.push_back("ScalarZp_mZP300_mChi50"    );
  //Samples.push_back("ScalarZp_mZP500_mChi1"     );
  //Samples.push_back("ScalarZp_mZP500_mChi150"   );
  //Samples.push_back("ScalarZp_mZP500_mChi500"   );
  //Samples.push_back("ScalarZp_mZP50_mChi1"      );
  //Samples.push_back("ScalarZp_mZP50_mChi10"     );
  //Samples.push_back("ScalarZp_mZP50_mChi50"     );
  //Samples.push_back("ScalarZp_mZP95_mChi50"     );
  //Samples.push_back("ScalarZp_mZP995_mChi500"   );
  //Samples.push_back("ScalarZp_mZP15_mChi10"     );

  UInt_t nSamples = Samples.size();
  vector< Float_t > Eff;  Eff.resize(nSamples);
  vector< Float_t > Err;  Err.resize(nSamples);

  // ----------------------------------------------------------------
  // setup latex doc
  // ----------------------------------------------------------------
  if (yields) cout << "Writing yields to: " << outfile << endl;
  else        cout << "Writing efficiencies to: " << outfile << endl;
  std::ofstream outResults;
  outResults.open(outfile);
  if (outResults.is_open()){
    outResults << "\\documentclass[a4paper,landscape]{article}" << std::endl;
    outResults << "\\usepackage[paperheight=15.0in,paperwidth=6.0in,margin=1.0in,headheight=0.0in,footskip=0.5in,includehead,includefoot]{geometry}" << std::endl;
    outResults << "\\begin{document}" << std::endl;
    if (yields) outResults << "\% Yields listed for each signal" << std::endl; 
    else        outResults << "\% Efficiencies listed for each signal" << std::endl;
    outResults << "\\begin{table}[bthp]" <<std::endl;
    outResults << "\\begin{tabular}{|l|r|}" <<std::endl;
    outResults << "\\hline " <<std::endl;

    // ----------------------------------------------------------------
    // get efficiency for each sample & write to table 
    // ----------------------------------------------------------------
    Double_t mX = 0;
    Double_t mY = 0;
    Int_t n2HDM = 0;
    Int_t nBARY = 0;
    for (UInt_t n=0; n < nSamples; n++){
      GetXandY(Samples[n],mX,mY);
      GetEfficiency(indir,Samples[n],skim,lumi,true,hiMETeff,yields,Eff[n],Err[n]);
      if (!yields){ // convert to percentage
        Eff[n]*=100;
        Err[n]*=100;
      }
      if (yields && hiMETeff) outResults << Samples[n] << " & " << Form("%1.1f",Eff[n]) << " $ \\pm $ " << Form("%1.1f",Err[n]) << " \\\\" << endl;
      else if (yields)        outResults << Samples[n] << " & " << Form("%1.2f",Eff[n]) << " $ \\pm $ " << Form("%1.2f",Err[n]) << " \\\\" << endl;
      else                    outResults << Samples[n] << " & " << Form("%1.1f",Eff[n]) << " $ \\pm $ " << Form("%1.1f",Err[n]) << " \\\\" << endl;

      // ----------------------------------------------------------------
      // fill the histograms 
      // ----------------------------------------------------------------
      if (Samples[n].Contains("2HDM",TString::kExact)){
        dat_2HDM->Fill(mX,mY,Eff[n]);
        err_2HDM->Fill(mX,mY,Err[n]);
        if (!yields){
          eff_2HDM->SetPoint(n2HDM,mX,mY,Eff[n]);
          eff_2HDM->SetPointError(n2HDM,0,0,Err[n]);
        }
      }
      if (Samples[n].Contains("Baryonic",TString::kExact)){
        dat_BARY->Fill(mX,mY,Eff[n]);
        err_BARY->Fill(mX,mY,Err[n]);
        if (!yields){
          var_BARY->Fill(mX,mY,Eff[n]);
          Double_t binX = var_BARY->GetXaxis()->FindBin(mX);
          Double_t binY = var_BARY->GetYaxis()->FindBin(mY);
          var_BARY->SetBinError(binX,binY,Err[n]);
          eff_BARY->SetPoint(nBARY,mX,mY,Eff[n]);
          eff_BARY->SetPointError(nBARY,0,0,Err[n]);
        }
      }
      if (Samples[n].Contains("2HDM",TString::kExact)) n2HDM++;
      if (Samples[n].Contains("Bary",TString::kExact)) nBARY++;

    }

    // ----------------------------------------------------------------
    // finish latex doc 
    // ----------------------------------------------------------------
    outResults << "\\hline" <<std::endl;
    outResults << "\\end{tabular}" <<std::endl;
    outResults << "\\end{table}" <<std::endl;
    outResults << "\\end{document}" <<std::endl;
  }
  else cout << "Error with output file..." << endl; 

  // ----------------------------------------------------------------
  // save the histograms then delete
  // ----------------------------------------------------------------
  outroot->cd();
  if (!yields) var_BARY->Write();
  if (!yields) eff_2HDM->Write();
  if (!yields) eff_BARY->Write();
  dat_2HDM->Write();
  err_2HDM->Write();
  dat_BARY->Write();
  err_BARY->Write();
  if (!yields) delete var_BARY;
  if (!yields) delete eff_2HDM;
  if (!yields) delete eff_BARY;
  delete dat_2HDM;
  delete err_2HDM;
  delete dat_BARY;
  delete err_BARY;


}

void GetXandY(TString name, Double_t & mX, Double_t & mY)
{
  TString strX = "_mZP";
  TString strY = (name.Contains("2HDM",TString::kExact))? "_mA0":"_mChi";
  Ssiz_t lenX = strX.Length();
  Ssiz_t lenY = strY.Length();
  Ssiz_t indX = name.Index(strX);
  Ssiz_t indY = name.Index(strY);
  TString tmpX(name((indX+lenX),indY-indX-lenX));
  TString tmpY(name(indY+lenY,name.Length()-indY-lenY));
  mX = (Double_t) tmpX.Atoi();
  mY = (Double_t) tmpY.Atoi(); 
}

Float_t CalculateEff(Int_t iden, Float_t fden, Int_t inum, Float_t fnum, Float_t & err)
{
   Float_t eff  = fnum/fden;
   if (eff <= 0) eff = 0;
   //err = eff*sqrt(fnum*(1.0-eff))/fden;
   err = sqrt(eff*(1.0-eff)/fden);

   Float_t ieff = (Float_t)inum/(Float_t)iden;
   //Float_t ierr = ieff*sqrt((Float_t)inum*(1.0-ieff)/(Double_t)iden);
   Float_t ierr = sqrt(ieff*(1.0-ieff)/(Double_t)iden);

   //cout << "den = " << iden << " OR " << fden << endl;
   //cout << "num = " << inum << " OR " << fnum << endl;
   //cout << "Eff = " << ieff << " OR " << eff << endl; 
   //cout << "Err = " << ierr << " OR " << err << endl;
 
   return eff;
}

void GetEfficiency(TString indir, TString name, TString skim, Float_t lumi, Bool_t totEff, Bool_t hiMETeff, Bool_t yields, Float_t & Eff, Float_t & Err)
{

  cout << " Working on Sample: " << name << endl;
  // ----------------------------------------------------------------
  // get file, tree, h_selection histos 
  // ----------------------------------------------------------------
  TFile *file = TFile::Open(Form("%s%s_%s.root",indir.Data(),name.Data(),skim.Data()));
  if (!file){ cout << "Sample does not exist! Exiting..." << endl; return;} 
  TTree *tree = (TTree*)file->Get("DiPhotonTree");
  if (!tree){ cout << "Tree does not exist! Exiting..." << endl; return;} 
  TH1D *fSel_wgt   = (TH1D*)file->Get("h_selection");
  TH1D *fSel_unwgt = (TH1D*)file->Get("h_selection_unwgt");  
  TH1D *fNum       = (TH1D*)file->Get("h_numbers"); 
  if (!fSel_wgt || !fSel_unwgt || !fNum){ cout << "Histo does not exist! Exiting..." << endl; return;}
 
  // ----------------------------------------------------------------
  // Calculate all numbers needed for efficiency 
  // ----------------------------------------------------------------
  Int_t   i_anlyz = fSel_unwgt->GetBinContent(1);
  Float_t f_anlyz = fSel_wgt->GetBinContent(1);//*lumi; // NOT WEIGHTED BY PU or SF
  Int_t   i_begin = fNum->GetBinContent(1);
  Float_t f_begin = fNum->GetBinContent(2);
  Int_t   i_loMET = 0;
  Int_t   i_hiMET = 0;
  Float_t f_loMET = 0;
  Float_t f_hiMET = 0;
  CalculateNumbers(tree,fNum,i_loMET,i_hiMET,f_loMET,f_hiMET);

  // ----------------------------------------------------------------
  // Calculate yields or efficiency 
  // ----------------------------------------------------------------
  if ( yields ){ // yields
    if (hiMETeff) Eff = f_hiMET;
    else          Eff = f_loMET;
    Err = TMath::Sqrt(Eff);
  }
  else{ // calculate efficiencies 
    if ( totEff  && hiMETeff  ) Eff = CalculateEff(i_anlyz,f_anlyz,i_hiMET,f_hiMET,Err);
    if ( totEff  && !hiMETeff ) Eff = CalculateEff(i_anlyz,f_anlyz,i_loMET,f_loMET,Err);
    if ( !totEff && hiMETeff  ) Eff = CalculateEff(i_begin,f_begin,i_hiMET,f_hiMET,Err);
    if ( !totEff && !hiMETeff ) Eff = CalculateEff(i_begin,f_begin,i_loMET,f_loMET,Err);
  }
 
}


void CalculateNumbers(TTree* tree, TH1D* hNum, Int_t & numi_loMET, Int_t & numi_hiMET, Float_t & numf_loMET, Float_t & numf_hiMET)
{

  // ----------------------------------------------------------------
  // set up branches - weight & those needed to apply the selection 
  // ----------------------------------------------------------------
  Float_t weight;		tree->SetBranchAddress("weight",&weight);
  Float_t mgg;			tree->SetBranchAddress("mgg",&mgg);
  Float_t t1pfmetCorr;		tree->SetBranchAddress("t1pfmetCorr",&t1pfmetCorr);
  Float_t dphiggmet;		tree->SetBranchAddress("dphiggmet",&dphiggmet);
  Float_t mindphijmet;		tree->SetBranchAddress("mindphijmet",&mindphijmet);
  Int_t nJets30;		tree->SetBranchAddress("nJets30",&nJets30);

  // ----------------------------------------------------------------
  // loop over input tree
  // ----------------------------------------------------------------
  for (UInt_t entry = 0; entry < tree->GetEntries(); entry++)
  {
    tree->GetEntry(entry);

    // ----------------------------------------------------------------
    // apply the cuts 
    // ----------------------------------------------------------------
    if (dphiggmet < 2.1)        continue;
    if (mindphijmet < 0.5)      continue;
    if (mgg < 115 || mgg > 135) continue;
    if (nJets30 > 2)            continue; 

    // ----------------------------------------------------------------
    // store the final number of events 
    // ----------------------------------------------------------------
    if (t1pfmetCorr > 50 && t1pfmetCorr <= 130){ numi_loMET++; numf_loMET+=weight; }
    if (t1pfmetCorr >  130){                     numi_hiMET++; numf_hiMET+=weight; }
  }
}


