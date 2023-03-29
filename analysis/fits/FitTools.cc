using namespace RooFit;
using namespace RooStats;

static const Int_t nMetCat=4;
static const Int_t nPhoCat=5;
Int_t MinMass = 100;
Int_t MaxMass = 200;

std::vector<TString> defineMet(Int_t metCat){
  std::vector<TString> MetCat;
  MetCat.resize(metCat);
  for (UInt_t met=0; met<nMetCat; met++){
    MetCat[met]=TString::Format("metCat%d",met);
  }
  
  return MetCat;
}

std::vector<Color_t> SetColorMet(Int_t metCat){
   std::vector<Color_t> ColorMet;
   ColorMet.resize(metCat);
   ColorMet[0]=kBlack;
   ColorMet[1]=kGreen;
   ColorMet[2]=kTeal-1;
   ColorMet[3]=kMagenta;

   return ColorMet;
}


std::vector<TString> definePho(Int_t phoCat, TString mass, UInt_t sample){
  TString sampleName = "";
  if (sample==1) sampleName=TString::Format("sig_2HDM_mZP%s_mA0300",mass.Data());
  else sampleName=TString::Format("bkg_%s",mass.Data());

  std::vector<TString> PhoCat;
  PhoCat.resize(phoCat);
  PhoCat[0]=TString::Format("%s_all",sampleName.Data());
  PhoCat[1]=TString::Format("%s_EBHighR9",sampleName.Data());
  PhoCat[2]=TString::Format("%s_EBLowR9",sampleName.Data());
  PhoCat[3]=TString::Format("%s_EEHighR9",sampleName.Data());
  PhoCat[4]=TString::Format("%s_EELowR9",sampleName.Data());
  
  return PhoCat;
}

void AddSigData(RooWorkspace*, Float_t, UInt_t);
void sigModelFit(RooWorkspace*, TString, Int_t);
void drawPlots(RooWorkspace*, TString, Int_t, Float_t, Float_t, TString, Int_t);
RooArgSet* defineVariables();


RooArgSet* defineVariables(){

  RooRealVar* mass		= new RooRealVar("mass","m(gg)",100,200,"GeV");
  RooRealVar* leadEta		= new RooRealVar("leadEta","eta(g1)",-10,10,"");
  RooRealVar* subleadEta	= new RooRealVar("subleadEta","eta(g2)",-10,10,"");
  RooRealVar* leadR9		= new RooRealVar("leadR9","r9(g1)",-10,10,"");
  RooRealVar* subleadR9		= new RooRealVar("subleadR9","r9(g2)",-10,10,"");
  RooRealVar* nvtx		= new RooRealVar("nvtx","nvtx",0,60,"");
  RooRealVar* weight		= new RooRealVar("weight","weight",-5,5,"");
  RooRealVar* t1pfmet		= new RooRealVar("t1pfmet","t1pfmet",0,1200,""); 
  RooRealVar* passHlt		= new RooRealVar("passHlt","passHlt",-0.5,1.5,"");


  RooArgSet* ntplVars = new RooArgSet(*mass,*leadEta,*subleadEta,*leadR9,*subleadR9,*nvtx,*t1pfmet,*weight,*passHlt);

  return ntplVars;
}

void AddSigData(RooWorkspace* w, TString Mass, UInt_t sample){
  TString name = "";
  if (sample==1) name=TString::Format("2HDM_mZP%s",Mass.Data());
  else if (sample==0) name=Mass; 

  // Variables
  RooArgSet* ntplVars = defineVariables();
  std::vector<TString> MetCat = defineMet(nMetCat);
  std::vector<TString> PhoCat = definePho(nPhoCat,Mass,sample);

  TString mainCut = "mass>= 100 && mass <= 200 && passHlt==1";

  RooDataSet* signal[nMetCat][nPhoCat];
  RooAddPdf* signalPdf[nMetCat][nPhoCat];

  TFile* inFile = TFile::Open(Form("newNtuples/%s_new.root",name.Data()));
  if (inFile == (TFile*) NULL) std::cout<< Form("newNtuples/%s_new.root",name.Data()) << " NOT A VALID FILE " << std::endl;
  
  TTree* sigTree1 = new TTree();

  for (UInt_t met=0; met<nMetCat; met++){
    for (UInt_t pho=0; pho<nPhoCat; pho++){

      sigTree1 = (TTree*)inFile->Get(Form("%s/%s",MetCat[met].Data(),PhoCat[pho].Data()));
      if (sigTree1 == (TTree*) NULL) std::cout << Form("%s/%s",MetCat[met].Data(),PhoCat[pho].Data()) << " NOT A VALID TREE " << std::endl;
      sigTree1->SetTitle(name);
      sigTree1->SetName(name);

      signal[met][pho] = new RooDataSet("signal","dataset",sigTree1,*ntplVars,mainCut);//,"weight");
      signal[met][pho]->Print("v");
 
      //signalPdf[met][pho] = new RooAddPdf("signalPdf","pdf",*ntplVars);
 
      w->import(*signal[met][pho],Rename(TString::Format("%s_%s",MetCat[met].Data(),PhoCat[pho].Data()))); 

    }// end loop over pho cat
  }// end loop over met cat
  //w->Print("v");
}

void sigModelFit(RooWorkspace* w, TString sampleOrMassName, Int_t sample){

  // Set up constants for fit
  Float_t HMass=125.;// SM Higgs mass
  Float_t width=5.;//   SM Higgs width
  Float_t GaussSig=1.;
  Float_t minMassFit=120.;// mass window min for fit
  Float_t maxMassFit=130.;// mass window max for fit
  Int_t BINS=(Int_t)maxMassFit-minMassFit;// nbins for mass window
  std::vector<Color_t> colorMetCat = SetColorMet(nMetCat);

  // Set up PDFs (Gauss,BW,BWxGauss) for fitting mass
  RooRealVar mass("mass","mass",minMassFit,maxMassFit);
  RooRealVar fitMass("fitMass","fitMass",minMassFit,maxMassFit);
  RooRealVar Hmass("Hmass","Hmass",HMass,"GeV");
  RooRealVar Hwidth("Hwidth","Hwidth",width,"GeV");
  RooRealVar sigma("sigma","sigma",GaussSig);
  RooGaussian *Hgauss = new RooGaussian("Hgauss","H sig PDF gauss",fitMass,Hmass,Hwidth);
  RooBreitWigner *Hbw = new RooBreitWigner("Hbw","H sig PDF BW",fitMass,Hmass,Hwidth);
  RooVoigtian *Hconv  = new RooVoigtian("Hconv","H sig PDF BW conv Gauss",fitMass,Hmass,Hwidth,sigma);
  RooVoigtian* H_BWxGauss [nMetCat][nPhoCat];
 
  // Draw the PDFs
  RooPlot* mFrame = fitMass.frame();
  Hgauss->plotOn(mFrame,LineColor(kBlue));
  Hbw->plotOn(mFrame,LineColor(kRed));
  Hconv->plotOn(mFrame,LineColor(kBlack));
  TLegend* leg = new TLegend(0.12,0.7,0.37,0.9,"Different Fits","brNDC");
  leg->AddEntry(mFrame->getObject(0),"Gauss","L");
  leg->AddEntry(mFrame->getObject(1),"BW","L");
  leg->AddEntry(mFrame->getObject(2),"BWxGauss","L");
  leg->SetTextSize(0.03);
  leg->SetTextFont(42);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  // Save plot
  TCanvas* c = new TCanvas();
  c->cd();
  mFrame->Draw();
  leg->Draw();
  c->SaveAs("plots/PDF_Distributions.png");
  c->Close();
 
  // Read in from Workspace the Signal Dataset
  RooDataSet* sigDataSet[nMetCat][nPhoCat];
  TString datasetName = "";
  std::vector<TString> MetCat = defineMet(nMetCat);
  std::vector<TString> PhoCat = definePho(nPhoCat,sampleOrMassName,sample);
 
  for (UInt_t met=0; met < nMetCat; met++){
    for (UInt_t pho=0; pho < nPhoCat; pho++){
      datasetName = TString::Format("%s_%s",MetCat[met].Data(),PhoCat[pho].Data());
      sigDataSet[met][pho] = (RooDataSet*) w->data(datasetName);
    }
  }

  // Fit PDF shapes to Signal Dataset
  for (UInt_t met=0; met < nMetCat; met++){
    for (UInt_t pho=0; pho < nPhoCat; pho++){ 
       //Hgauss->fitTo(*sigDataSet[met][pho]);
       //Hbw->fitTo(*sigDataSet[met][pho]);
       //Hconv->fitTo(*sigDataSet[met][pho]);
       H_BWxGauss[met][pho] = new RooVoigtian("H_BWxGauss","H sig PDF",mass,Hmass,Hwidth,Hwidth);
       H_BWxGauss[met][pho]->fitTo(*sigDataSet[met][pho],RooFit::Save(kTRUE));
       //H_BWxGauss[met][pho]->Print("V");
    }
  }

  // Make Output File & TCanvases
  TFile* f = new TFile("signalPlots_wFits.root","RECREATE");
  f->cd();
  TCanvas* c1[nMetCat][nPhoCat];
  for (UInt_t met=0; met<nMetCat; met++){
    for (UInt_t pho=0; pho<nPhoCat; pho++){
      c1[met][pho] = new TCanvas(Form("c1%s%s",MetCat[met].Data(),PhoCat[pho].Data()),"c",1);
    }
  }

  // Plot the PDF & Dataset Overlaid 
  RooPlot* shapeth1f[nMetCat][nPhoCat];
  for (UInt_t met=0; met<nMetCat; met++){
    for (UInt_t pho=0; pho<nPhoCat; pho++){
      datasetName = TString::Format("%s_%s",MetCat[met].Data(),PhoCat[pho].Data());
      shapeth1f[met][pho] = w->var("mass")->frame(Range(minMassFit-10,maxMassFit+10),Bins(BINS+20));
      if(shapeth1f[met][pho]== (RooPlot*) NULL) std::cout<<"VARIABLE NOT FOUND" << std::endl;
    }
  } 

  for (UInt_t pho=0; pho<nPhoCat; pho++){
     for (UInt_t met=0; met<nMetCat; met++){
       c1[met][pho]->cd();

       // plot DataSet & Fit
       sigDataSet[met][pho]->plotOn(shapeth1f[met][pho],LineColor(colorMetCat[met]),DrawOption("L"),LineStyle(1),MarkerStyle(0),XErrorSize(0),DataError(RooAbsData::None));
       H_BWxGauss[met][pho]->plotOn(shapeth1f[met][pho],LineColor(kBlue),LineStyle(kDashed));
       // add Stat (dataset) & Param (fit) boxes
       sigDataSet[met][pho]->statOn(shapeth1f[met][pho]);
       //H_BWxGauss[met][pho]->paramOn(shapeth1f[met][pho],sigDataSet[met][pho]);
       // draw Plots
       shapeth1f[met][pho]->Draw(); 

       // setup Legend
       TLegend* leg = new TLegend(0.12,0.7,0.37,0.9,(TString::Format("%s",PhoCat[pho].Data())),"brNDC");
       leg->AddEntry(shapeth1f[met][pho]->getObject(0),MetCat[met].Data(),"L");
       leg->AddEntry(shapeth1f[met][pho]->getObject(1),"Fit BWxGauss","L");
       leg->SetTextSize(0.03);
       leg->SetTextFont(42);
       leg->SetBorderSize(0);
       leg->SetFillStyle(0);
       leg->Draw();

       // save Plots
       TString sampleName="";
       if (sample==1) sampleName=TString::Format("sig_2HDM_mZP%s",sampleOrMassName.Data());
       else sampleName = TString::Format("bkg_%s",sampleOrMassName.Data());
       c1[met][pho]->SetLogy(0);
       c1[met][pho]->SaveAs(TString::Format("plots/%s/%s_%s_wFit.png",sampleName.Data(),PhoCat[pho].Data(),MetCat[met].Data()));
       c1[met][pho]->SetLogy(1);
       c1[met][pho]->SaveAs(TString::Format("plots/%s/%s_%s_wFit_log.png",sampleName.Data(),PhoCat[pho].Data(),MetCat[met].Data()));
    } 
  }


}


void drawPlots(RooWorkspace* w, TString variable, int BINS, float MIN, float MAX, TString mass, Int_t sample){
  TString inDir = "";
  TString name = "";
  TString datasetName = "";
  std::vector<TString> MetCat = defineMet(nMetCat);
  std::vector<TString> PhoCat = definePho(nPhoCat,mass,sample);
 
  TFile* f = new TFile("signalPlots.root","RECREATE");
  f->cd();   

  TCanvas* c[nPhoCat]; 
  for (UInt_t pho=0; pho<nPhoCat; pho++){
   c[pho] = new TCanvas(Form("c%s",PhoCat[pho].Data()),"c",1);
  }

  std::vector<Color_t> colorMetCat = SetColorMet(nMetCat);

  RooDataSet* sigDataSet[nMetCat][nPhoCat];
  RooAddPdf* sigPdf[nMetCat][nPhoCat];
  RooPlot* sigth1f[nPhoCat];

  RooArgSet* ntplVars = defineVariables();
  UInt_t nVars = sizeof(*ntplVars);
  RooDataHist* rooHistInter[nMetCat][nPhoCat][nVars];
  RooHistPdf* rooPdfInter[nMetCat][nPhoCat][nVars]; 

  RooDataHist* tmpRooHist; //= new RooDataHist("tmp","tmp",BINS,MIN,MAX);

  TH1F* h_sig = new TH1F("h_sig","h_sig",BINS,MIN,MAX);
  h_sig->Sumw2();

  for (UInt_t met=0; met<nMetCat; met++){
    for (UInt_t pho=0; pho<nPhoCat; pho++){

      datasetName = TString::Format("%s_%s",MetCat[met].Data(),PhoCat[pho].Data());
      sigth1f[pho] = w->var("mass")->frame(Range(MIN,MAX),Bins(BINS));
      if(sigth1f[pho]== (RooPlot*) NULL) std::cout<<"VARIABLE NOT FOUND" << std::endl;

      sigDataSet[met][pho] = (RooDataSet*) w->data(datasetName);
      sigPdf[met][pho] = new RooAddPdf(TString::Format("Pdf_%s",datasetName.Data()));
      //rooHistInter[met][pho] = new RooDataHist(TString::Format("rooHist_%s",datasetName.Data()),"rooHistInter",*ntplVars,h_sig,1.0);
    }
  } 

  for (UInt_t pho=0; pho<nPhoCat; pho++){
     c[pho]->cd();
     std::cout << "In " << PhoCat[pho] << std::endl;
     for (UInt_t met=0; met<nMetCat; met++){
       // plot all met bins in same pho plot
       sigDataSet[met][pho]->plotOn(sigth1f[pho],LineColor(colorMetCat[met]),DrawOption("L"),LineStyle(1),MarkerStyle(0),XErrorSize(0),DataError(RooAbsData::None));
       std::cout << "Number Entries in MET bin #" << met << " = " << /* tmpRooHist->GetEntries() <<*/ std::endl;
     }
     sigth1f[pho]->Draw(); 
     TLegend* leg = new TLegend(0.55,0.6,0.87,0.88,(TString::Format("%s",PhoCat[pho].Data())),"brNDC");
     for (UInt_t met=0; met<nMetCat; met++){
       leg->AddEntry(sigth1f[pho]->getObject(met),MetCat[met].Data(),"L");
     }
     leg->Draw();
     c[pho]->SetLogy(0);
     c[pho]->SaveAs(TString::Format("plots/%s.png",PhoCat[pho].Data()));
     c[pho]->SetLogy(1);
     c[pho]->SaveAs(TString::Format("plots/%s_log.png",PhoCat[pho].Data()));
  }
 
}

void runfits(){
  // Setup Workspace
  TString card_name("MonoHiggs.rs");
  HLFactory hlf("HLFactory", card_name, false);
  RooWorkspace* w = hlf.GetWs();

  Float_t Lum = 1263.0; 
  RooRealVar lumi("lumi","lumi",Lum);
  w->import(lumi);
  //w->Print();
 
  // Add the samples to the Workspace
  std::cout << "Adding Signal Samples" << std::endl;
  AddSigData(w,"600",1);
  //AddSigData(w,"800",1);
  //AddSigData(w,"1000",1);
  //AddSigData(w,"1200",1);
  //AddSigData(w,"1400",1);
  //AddSigData(w,"VH",0);
  //AddSigData(w,"GluGluHToGG",0);

  // Do the Fits for each met & pho category 
  std::cout << "Starting SigModelFit" << std::endl;
  sigModelFit(w,"600",1);
  //sigModelFit(w,"800",1);
  //sigModelFit(w,"100",1);
  //sigModelFit(w,"1200",1);
  //sigModelFit(w,"1400",1);
  //sigModelFit(w,"VH",0);
  //sigModelFit(w,"GluGluHToGG",0);

  // Plot invariant mass overlaid for the MET categories
  std::cout << "Making Plots" << std::endl;
  drawPlots(w,"mgg",30,110.,140.,"600",1);
  //drawPlots(w,"mgg",30,110.,140.,"800",1);
  //drawPlots(w,"mgg",30,110.,140.,"1000",1);
  //drawPlots(w,"mgg",30,110.,140.,"1200",1);
  //drawPlots(w,"mgg",30,110.,140.,"1400",1);
  //drawPlots(w,"mgg",30,110.,140.,"VH",0);
  //drawPlots(w,"mgg",30,110.,140.,"GluGluHToGG",0);

  TString wkspFile = "signals_wkspace";
  std::cout << "Write out Workspace to File " << wkspFile.Data() << ".root" << std::endl;
  w->writeToFile(Form("%s.root",wkspFile.Data()));
  
  std::cout << "Finished Running" << std::endl;
 
}






