#define METcorr_cxx

#include "../interface/METcorr.hh"

DblVec METcorr::Loop(TString inDir, TString outFile)
{
 
  DblVec metcorr;
  metcorr.resize(4);
  for (UInt_t i=0; i<metcorr.size(); i++) metcorr[i]=0.0;
  
  //------------------------------------------------------------------------
  // Setup histograms
  //------------------------------------------------------------------------
  TFile *temp=new TFile(Form("%smetCorr_%s.root",inDir.Data(),outFile.Data()), "RECREATE");
  TProfile metx("metx","",100,0,2000);
  TProfile mety("mety","",100,0,2000);
  
  TProfile metxnew("metxnew","",100,0,2000);
  TProfile metynew("metynew","",100,0,2000);
  
  TH1D metphi("metphi","",100,-3.14,3.14);
  TH1D metphinew("metphinew","",100,-3.14,3.14);
 
  TH1D metCorr("metCorr","",4,0,4);
 
  Long64_t nentries = fChain->GetEntriesFast();
  std::cout << nentries << std::endl;

  //------------------------------------------------------------------------
  // Fill metx and mety bins 
  //------------------------------------------------------------------------
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry%100000 == 0) std::cout << "Events " << jentry << "/" << nentries << " processed 1' round" << std::endl;
    
    // loose presel already applied to samples
    if(pt1<33) continue;
    if(pt2<25) continue;
    
    metphi.Fill(t1pfmetPhi);
    metx.Fill(t1pfmetSumEt,t1pfmet*cos(t1pfmetPhi));
    mety.Fill(t1pfmetSumEt,t1pfmet*sin(t1pfmetPhi));
  }
  
  //------------------------------------------------------------------------
  // Fit metx and mety with a line 
  //------------------------------------------------------------------------
  double px0(0), px1(0), py0(0), py1(0); 
  TF1 line("line","pol1",100,2500);
  
  metx.Fit("line"); 
  px0 = line.GetParameter(0);
  px1 = line.GetParameter(1);
  
  mety.Fit("line"); 
  py0 = line.GetParameter(0);
  py1 = line.GetParameter(1);
 
  metCorr.Fill(0.5,px0);
  metCorr.Fill(1.5,px1);
  metCorr.Fill(2.5,py0);
  metCorr.Fill(3.5,py1);
 
  nentries = fChain->GetEntriesFast();
  
  //------------------------------------------------------------------------
  // Get the corrections 
  //------------------------------------------------------------------------
  nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry%100000 == 0) std::cout << "Events " << jentry << "/" << nentries << " processed 2' round" << std::endl;
    
    if(pt1<33) continue;
    if(pt2<25) continue;
    
    // correction for METx, METy bias
    double px(0), py(0), e(0);
    px = t1pfmet*cos(t1pfmetPhi) -(px0+px1*t1pfmetSumEt);
    py = t1pfmet*sin(t1pfmetPhi) -(py0+py1*t1pfmetSumEt);
    
    e = sqrt(px*px+py*py);
    
    TLorentzVector correctedMet;
    correctedMet.SetPxPyPzE(px,py,0,e);
    
    metphinew.Fill(correctedMet.Phi());
    metxnew.Fill(t1pfmetSumEt,correctedMet.Px());
    metynew.Fill(t1pfmetSumEt,correctedMet.Py());
    
  }
  
  //------------------------------------------------------------------------
  // Store the corrections 
  //------------------------------------------------------------------------

  metx.Write();
  mety.Write();
  
  metxnew.Write();
  metynew.Write();
  
  metphi.Write();
  metphinew.Write();
 
  metCorr.Write();
 
  temp->Write() ;
  temp->Close() ;
  temp->Delete();
  
  std::cout << "px = t1pfmet*cos(t1pfmetPhi) -(" << px0 << " + " << px1 << " * t1pfmetSumEt)" << std::endl;
  std::cout << "py = t1pfmet*sin(t1pfmetPhi) -(" << py0 << " + " << py1 << " * t1pfmetSumEt)" << std::endl;
  
  metcorr[0]=px0;
  metcorr[1]=px1;
  metcorr[2]=py0;
  metcorr[3]=py1;
  return metcorr;
}
