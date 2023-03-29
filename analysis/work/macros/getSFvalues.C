#include <TFile.h>
#include <TH2.h>

#include <iostream>

void   makeTxtFile(TString, TString);
double getValue(TH2F*, double, double);

void getSFvalues(){

  cout << "Get SF values from root file" << endl;
  TString fin  = "~/public/egammaEffi.txt_EGM2D.root";
  TString fout = "SFfile.txt";
  makeTxtFile(fin,fout);

}

void makeTxtFile(TString file, TString fileout){

  TFile* infile = new TFile(file);
  TH2F*  inhist = (TH2F*)infile->Get("EGamma_SF2D");
  std::ofstream outfile;
  outfile.open(fileout);

  std::vector<double> etas;
  etas = {-2.2,-1.8,-1.0,-0.5,0.5,1.0,1.8,2.2};
 
  if (outfile.is_open()){
    for (i=0; i < etas.size(); i++){
      if (i==0) outfile << "if (-2.5   <= eta && eta < -2.0  ){" << std::endl;
      if (i==1) outfile << "if (-2.0   <= eta && eta < -1.566){" << std::endl;
      if (i==2) outfile << "if (-1.444 <= eta && eta < -0.8  ){" << std::endl;
      if (i==3) outfile << "if (-0.8   <= eta && eta < 0.0   ){" << std::endl;
      if (i==4) outfile << "if (0.0    <= eta && eta < 0.8   ){" << std::endl;
      if (i==5) outfile << "if (0.8    <= eta && eta < 1.444 ){" << std::endl;
      if (i==6) outfile << "if (1.566  <= eta && eta < 2.0   ){" << std::endl;
      if (i==7) outfile << "if (2.0    <= eta && eta < 2.5   ){" << std::endl;
      outfile << "  if (pt <= 35) SF = "       << getValue(inhist, etas[i],  30) << ";" << std::endl;
      outfile << "  if (35 < pt && pt <= 50)  SF = " << getValue(inhist, etas[i],  40) << ";" << std::endl;
      outfile << "  if (50 < pt && pt <= 90)  SF = " << getValue(inhist, etas[i],  80) << ";" << std::endl;
      outfile << "  if (90 < pt && pt <= 150) SF = " << getValue(inhist, etas[i], 100) << ";" << std::endl;
      outfile << "  if (pt > 150) SF = "       << getValue(inhist, etas[i], 160) << ";" << std::endl;
      outfile << "}" << std::endl;
    }
  }
  else std::cout << "File did not open" << std::endl;  

}

double getValue(TH2F* hist, double eta, double pt){
  double SF = 1.0;

  if (hist!=(TH2F*)NULL){
    int binX = hist->GetXaxis()->FindBin(eta);
    int binY = hist->GetYaxis()->FindBin(pt);
    SF = hist->GetBinContent(binX,binY);
  }
  else std::cout << "Could not find histo" << std::endl;
  
  return SF; 
} 
