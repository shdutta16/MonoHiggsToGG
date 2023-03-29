#include "../interface/OverlayPlots.hh"

OverlayPlots::OverlayPlots()
{

  //------------------------------------------------------------------------
  // Setup samples
  //------------------------------------------------------------------------
  for (TStrBoolMapIter iter = Config::SampleMap.begin(); iter != Config::SampleMap.end(); ++iter) {
    if ((*iter).second) { // Data
      fDataNames.push_back((*iter).first);
    }
    else { // MC
      if (((*iter).first).Contains("mZP",TString::kExact)) fSigNames.push_back((*iter).first);
      else fBkgNames.push_back((*iter).first);
    }
  }
  fNData = fDataNames.size();
  fNBkg  = fBkgNames.size();
  fNSig  = fSigNames.size();
  std::cout << "DataSize = " << fNData << " BkgSize = " << fNBkg << " SigSize = " << fNSig << std::endl;

  //------------------------------------------------------------------------
  // Make map to store indices for bkg samples 
  //------------------------------------------------------------------------
  for (Int_t mc = 0; mc < fNBkg; mc++){
    fBkgIndicesMap[fBkgNames[mc]] = mc;
  } 

  //------------------------------------------------------------------------
  // Setup output
  //------------------------------------------------------------------------
  fOutDir = Form("%s/comb",Config::outdir.Data()); // where to put output stack plots 
  MakeOutDir(fOutDir); // make output directory 
  fOutFile = new TFile(Form("%s/stackplots_canv.root",fOutDir.Data()),"RECREATE"); // ouput tfile to store canvases here for quick editting

  //------------------------------------------------------------------------
  // Initialize objects
  //------------------------------------------------------------------------
  OverlayPlots::OpenInputFiles();            // open the input files
  OverlayPlots::InitTH1FNamesAndSubDNames(); // get histo names from plotnames file
  MakeSubDirs(fTH1FSubDMap,fOutDir);         // setup output subdir
  OverlayPlots::InitInputPlots();            // initialize input histos
  OverlayPlots::InitOutputCanvPads();        // initialize output canv
  OverlayPlots::InitOutputPlots();           // initialize output histos
  OverlayPlots::InitOutputLegends();         // initialize output legends

}

void OverlayPlots::DoComb()
{

  //------------------------------------------------------------------------
  // Start running
  //------------------------------------------------------------------------
  gStyle->SetOptStat(0);
  gStyle->SetFrameLineWidth(3);
  OverlayPlots::MakeOverlayPlots();
  OverlayPlots::MakeOutputCanvas();

}

void OverlayPlots::FixPlotStyling(const Int_t th1f)
{

  //------------------------------------------------------------------------
  // Make axis labels pretty 
  //------------------------------------------------------------------------
  if (Config::useData){
    fOutDataTH1FHists[th1f]->GetYaxis()->SetLabelSize  (0.05); 
    fOutDataTH1FHists[th1f]->GetYaxis()->SetTitleSize  (0.07);
    fOutDataTH1FHists[th1f]->GetYaxis()->SetTitleOffset(0.8);
    fOutDataTH1FHists[th1f]->GetXaxis()->SetTitleOffset(999);
    fOutDataTH1FHists[th1f]->GetXaxis()->SetLabelSize  (0);
  }

  //------------------------------------------------------------------------
  // Make the bkgs pretty 
  //------------------------------------------------------------------------
  fOutBkgTH1FHists[th1f]->SetMarkerSize(0);
  fOutBkgTH1FHists[th1f]->SetFillColor(kGray+3);
  fOutBkgTH1FHists[th1f]->SetFillStyle(3013);
  if (Config::mergeBkgs){
    fOutHiggsBkgHists[th1f]->SetLineColor(Config::colorMap["SMHiggs"]);
    fOutEWK1BkgHists[th1f]->SetLineColor(Config::colorMap["EWK1pho"]);
    fOutEWK2BkgHists[th1f]->SetLineColor(Config::colorMap["EWK2pho"]);
    fOutJetsBkgHists[th1f]->SetLineColor(Config::colorMap["Jetspho"]);
    fOutDYBkgHists[th1f]->SetLineColor(Config::colorMap["DYJetsToLL"]);
  }

  //------------------------------------------------------------------------
  // Make the signals pretty 
  //------------------------------------------------------------------------
  for ( Int_t mc = 0; mc < fNSig; mc++ ){
    fInSigTH1FHists[th1f][mc]->SetFillStyle(0);
    fInSigTH1FHists[th1f][mc]->SetLineWidth(3);
    fInSigTH1FHists[th1f][mc]->SetLineStyle(8);
    fInSigTH1FHists[th1f][mc]->SetLineColor(Config::colorMap[fSigNames[mc]]);
  }


}

void OverlayPlots::MakeOutputCanvas()
{

  //------------------------------------------------------------------------
  // Setup canvases 
  //------------------------------------------------------------------------
  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){
    OverlayPlots::FixPlotStyling(th1f);      // make histos pretty 

    // draw first with  log scale
    Bool_t isLogY = true;
    OverlayPlots::DrawUpperPad(th1f,isLogY); // upper pad is overlay
    OverlayPlots::SaveCanvas(th1f,isLogY);   // now save the canvas, w/ logy

    // draw second with lin scale
    isLogY = false;
    OverlayPlots::DrawUpperPad(th1f,isLogY); // upper pad is overlay
    OverlayPlots::SaveCanvas(th1f,isLogY);   // now save the canvas, w/o logy
  }// end th1f loop

}

void OverlayPlots::DrawUpperPad(const Int_t th1f, const Bool_t isLogY) 
{    

  //------------------------------------------------------------------------
  // Setup the pad for the stack plots 
  //------------------------------------------------------------------------
  fOutTH1FCanvases[th1f]->cd();
  fOutTH1FStackPads[th1f]->Draw(); // draw upper pad   
  fOutTH1FStackPads[th1f]->cd();   // upper pad is current pad
  
  //------------------------------------------------------------------------
  // Set maximum by comparing added mc vs added data
  //------------------------------------------------------------------------
  if (isLogY) fInSigTH1FHists[th1f][0]->SetMaximum(1E2);
  else        fInSigTH1FHists[th1f][0]->SetMaximum(1.5); 
  if (isLogY) fInSigTH1FHists[th1f][0]->SetMinimum(0.001); 
  else        fInSigTH1FHists[th1f][0]->SetMinimum(0); 
  
  //------------------------------------------------------------------------
  // Draw the data, stack, sig histos, legend
  //------------------------------------------------------------------------
  fInSigTH1FHists[th1f][0]->Draw("HIST"); // draw signal first
  if (Config::useData) fOutDataTH1FHists[th1f]->Draw("PE SAME");
  if (Config::mergeBkgs){
    fInBkgTH1FHists[th1f][fBkgIndicesMap["VHToGG"]]->Draw("HIST SAME");
    fOutHiggsBkgHists[th1f]->Draw("HIST SAME");
    fOutEWK2BkgHists[th1f]->Draw("HIST SAME");
    fOutDYBkgHists[th1f]->Draw("HIST SAME");
    fOutEWK1BkgHists[th1f]->Draw("HIST SAME");
    fOutJetsBkgHists[th1f]->Draw("HIST SAME");
    fInBkgTH1FHists[th1f][fBkgIndicesMap["DiPhoton"]]->Draw("HIST SAME");
  }
  else{
    for (Int_t mc = 0; mc < fNBkg; mc++){
      fInBkgTH1FHists[th1f][mc]->Draw("HIST SAME"); // overlay each bkg
    }
  }
  for ( Int_t mc = 0; mc < fNSig; mc++){
    fInSigTH1FHists[th1f][mc]->Draw("HIST SAME"); // overlay signals
  }
  if (Config::useData) fOutDataTH1FHists[th1f]->Draw("PE SAME"); // redraw data 
  fTH1FLegends[th1f]->Draw("SAME");               // draw the legend
  fSigLegends[th1f]->Draw("SAME");                // draw sig legend
}

void OverlayPlots::SaveCanvas(const Int_t th1f, const Bool_t isLogY)
{

  //------------------------------------------------------------------------
  // Write out the canvases 
  //------------------------------------------------------------------------
  TString suffix;
  if   (isLogY) {suffix = "log";}
  else          {suffix = "lin";}

  Int_t log = (isLogY)? 1:0;

  fOutTH1FStackPads[th1f]->cd();         // upper pad is current pad
  fOutTH1FStackPads[th1f]->SetLogy(log); // set logy on this pad

  fOutTH1FCanvases[th1f]->cd();          // go back to the main canvas before saving
  CMSLumi(fOutTH1FCanvases[th1f]);       // write out Lumi info
 
  //------------------------------------------------------------------------
  // SAVE 
  //------------------------------------------------------------------------
  TString writePlots = Form("%s/%s/%s_%s.%s",fOutDir.Data(),fTH1FSubDMap[fTH1FNames[th1f]].Data(),fTH1FNames[th1f].Data(),suffix.Data(),Config::outtype.Data());
  fOutTH1FCanvases[th1f]->SaveAs(writePlots);
  fOutFile->cd();
  if (!isLogY) fOutTH1FCanvases[th1f]->Write(Form("%s",fTH1FNames[th1f].Data()));
}


void OverlayPlots::MakeOverlayPlots()
{

  //------------------------------------------------------------------------
  // Copy plots into output hists/comb
  //------------------------------------------------------------------------
  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){

    //------------------------------------------------------------------------
    // Scale to integral -- for all samples 
    //------------------------------------------------------------------------
    Float_t tmp_int;
    for (Int_t data = 0; data < fNData; data++){
      tmp_int = fInDataTH1FHists[th1f][data]->Integral();
      if (tmp_int > 0) fInDataTH1FHists[th1f][data]->Scale(1/tmp_int);
    }
    for (Int_t mc = 0; mc < fNBkg; mc++){
      tmp_int = fInBkgTH1FHists[th1f][mc]->Integral();
      if (tmp_int > 0) fInBkgTH1FHists[th1f][mc]->Scale(1/tmp_int);
    } 
    for ( Int_t mc = 0; mc < fNSig; mc++){
      tmp_int = fInSigTH1FHists[th1f][mc]->Integral();
      if (tmp_int > 0) fInSigTH1FHists[th1f][mc]->Scale(1/tmp_int);
    }

    //------------------------------------------------------------------------
    // Copy data
    //------------------------------------------------------------------------
    for (Int_t data = 0; data < fNData; data++){
      if (data==0) fOutDataTH1FHists[th1f] = (TH1F*)fInDataTH1FHists[th1f][data]->Clone();
      else         fOutDataTH1FHists[th1f]->Add(fInDataTH1FHists[th1f][data]);
    }
    if (Config::useData) fTH1FLegends[th1f]->AddEntry(fOutDataTH1FHists[th1f],"Data","EPL");
 
    //------------------------------------------------------------------------
    // Copy sig 
    //------------------------------------------------------------------------
    for (Int_t mc = 0; mc < fNSig; mc++){
      fSigLegends[th1f]->AddEntry(fInSigTH1FHists[th1f][mc],Config::nameMap[fSigNames[mc]],"L");
    }

    //------------------------------------------------------------------------
    // Use Gjets weighted to QCD as QCD
    //------------------------------------------------------------------------
    if (Config::doQCDrewgt){
      Double_t qcdint  = fInBkgTH1FHists[th1f][fBkgIndicesMap["QCD"]]->Integral();
      Double_t gjetint = fInBkgTH1FHists[th1f][fBkgIndicesMap["GJets"]]->Integral();
      fGJetsClone[th1f] = (TH1F*)fInBkgTH1FHists[th1f][fBkgIndicesMap["GJets"]]->Clone();
      fGJetsClone[th1f]->Scale(qcdint/gjetint);
    }  

    //------------------------------------------------------------------------
    // Copy bkg -- to use as unc. 
    //------------------------------------------------------------------------
    for (Int_t mc = 0; mc < fNBkg; mc++){
      if (mc==0) fOutBkgTH1FHists[th1f] = (TH1F*)fInBkgTH1FHists[th1f][mc]->Clone();
      else if (fBkgNames[mc]=="QCD" && Config::doQCDrewgt) fOutBkgTH1FHists[th1f]->Add(fGJetsClone[th1f]);
      else       fOutBkgTH1FHists[th1f]->Add(fInBkgTH1FHists[th1f][mc]);
    }

    //------------------------------------------------------------------------
    // Merge backgrounds
    //------------------------------------------------------------------------
    if (Config::mergeBkgs){

      //------------------------------------------------------------------------
      // merge higgs bkg
      //------------------------------------------------------------------------
      fOutHiggsBkgHists[th1f] = (TH1F*)fInBkgTH1FHists[th1f][fBkgIndicesMap["ttHJetToGG"]]->Clone();
      fOutHiggsBkgHists[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["GluGluHToGG"]]);
      fOutHiggsBkgHists[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["VBFHToGG"]]);

      //------------------------------------------------------------------------
      // merge DY bkg 
      //------------------------------------------------------------------------
      fOutDYBkgHists[th1f] = (TH1F*)fInBkgTH1FHists[th1f][fBkgIndicesMap["DYJetsToLL"]]->Clone();

      //------------------------------------------------------------------------
      // merge EWK+1pho bkg 
      //------------------------------------------------------------------------
      fOutEWK1BkgHists[th1f] = (TH1F*)fInBkgTH1FHists[th1f][fBkgIndicesMap["TGJets"]]->Clone();
      fOutEWK1BkgHists[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["TTGJets"]]);
      fOutEWK1BkgHists[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["TTJets"]]);
      fOutEWK1BkgHists[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["WGToLNuG"]]);
      fOutEWK1BkgHists[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["ZGTo2LG"]]);
      fOutEWK1BkgHists[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["ZGTo2NuG"]]);
      fOutEWK1BkgHists[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["WJetsToLNu"]]);

      //------------------------------------------------------------------------
      // merge EWK+2pho bkg 
      //------------------------------------------------------------------------
      fOutEWK2BkgHists[th1f] = (TH1F*)fInBkgTH1FHists[th1f][fBkgIndicesMap["TTGG_0Jets"]]->Clone();
      //fOutEWK2BkgHists[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["ZZTo2L2Nu"]]);
      fOutEWK2BkgHists[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["ZZTo2L2Q"]]);
      fOutEWK2BkgHists[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["WZTo2L2Q"]]);

      //------------------------------------------------------------------------
      // merge QCD and Gjets 
      //------------------------------------------------------------------------
      fOutJetsBkgHists[th1f] = (TH1F*)fInBkgTH1FHists[th1f][fBkgIndicesMap["GJets"]]->Clone();
      if (Config::doQCDrewgt) fOutJetsBkgHists[th1f]->Add(fGJetsClone[th1f]);
      else fOutJetsBkgHists[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["QCD"]]);
    }

    //------------------------------------------------------------------------
    // Add bkgs to ordered legend entries  
    //------------------------------------------------------------------------
    if (Config::mergeBkgs){ // add merged bkgs -- ordered
      fTH1FLegends[th1f]->AddEntry(fOutDYBkgHists[th1f],Config::nameMap["DYJetsToLL"],"l"); 
      fTH1FLegends[th1f]->AddEntry(fInBkgTH1FHists[th1f][fBkgIndicesMap["VHToGG"]],Config::nameMap["VHToGG"],"l");
      fTH1FLegends[th1f]->AddEntry(fOutEWK1BkgHists[th1f],Config::nameMap["EWK1pho"],"l");
      fTH1FLegends[th1f]->AddEntry(fOutHiggsBkgHists[th1f],Config::nameMap["SMHiggs"],"l");
      fTH1FLegends[th1f]->AddEntry(fOutJetsBkgHists[th1f],Config::nameMap["Jetspho"],"l");
      fTH1FLegends[th1f]->AddEntry(fOutEWK2BkgHists[th1f],Config::nameMap["EWK2pho"],"l");
      fTH1FLegends[th1f]->AddEntry(fInBkgTH1FHists[th1f][fBkgIndicesMap["DiPhoton"]],Config::nameMap["DiPhoton"],"l");
    }
    else { // if not merging, just add all bkgs
      for (Int_t mc = 0; mc < fNBkg; mc++){
        fTH1FLegends[th1f]->AddEntry(fInBkgTH1FHists[th1f][mc],Config::nameMap[fBkgNames[mc]],"l");
      }
    }

  }// end th1f loop

}

void OverlayPlots::InitOutputCanvPads()
{

  //------------------------------------------------------------------------
  // Initialize canvas and pads 
  //------------------------------------------------------------------------
  fOutTH1FCanvases.resize(fNTH1F);
  fOutTH1FStackPads.resize(fNTH1F);
  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){
    fOutTH1FCanvases[th1f] = new TCanvas(Form("%s_canv",fTH1FNames[th1f].Data()),"",550,550);
    fOutTH1FCanvases[th1f]->cd();
    
    //fOutTH1FStackPads[th1f] = new TPad(Form("%s_upad",fTH1FNames[th1f].Data()),"",0.0,0.0,1.0,1.0);
    fOutTH1FStackPads[th1f] = new TPad(Form("%s_upad",fTH1FNames[th1f].Data()),"",0.0,0.1,1.0,1.0);
    fOutTH1FStackPads[th1f]->SetBottomMargin(0.03);
    fOutTH1FStackPads[th1f]->SetTopMargin(0.06);
    fOutTH1FStackPads[th1f]->SetRightMargin(0.05040323); 
    fOutTH1FStackPads[th1f]->SetLeftMargin(0.1290323);    
 
  }
}



void OverlayPlots::InitOutputLegends()
{

  //------------------------------------------------------------------------
  // Initialize legends 
  //------------------------------------------------------------------------
  fTH1FLegends.resize(fNTH1F);
  fSigLegends.resize(fNTH1F);

  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){
    fTH1FLegends[th1f] = new TLegend(0.51,0.75,0.91,0.92,NULL,"brNDC"); // (x1,y1,x2,y2)
    fTH1FLegends[th1f]->SetTextSize(0.036);
    fTH1FLegends[th1f]->SetNColumns(2);
    fTH1FLegends[th1f]->SetBorderSize(0);
    fTH1FLegends[th1f]->SetLineColor(1);
    fTH1FLegends[th1f]->SetLineStyle(1);
    fTH1FLegends[th1f]->SetLineWidth(1);
    fTH1FLegends[th1f]->SetFillColor(0);
    fTH1FLegends[th1f]->SetFillStyle(0);
    fTH1FLegends[th1f]->SetTextFont(42);

    fSigLegends[th1f] = new TLegend(0.51,0.68,0.87,0.75); // (x1,y1,x2,y2)
    //fSigLegends[th1f]->SetHeader("m_{A} = 300 GeV");
    fSigLegends[th1f]->SetTextSize(0.036);
    fSigLegends[th1f]->SetNColumns(1);
    fSigLegends[th1f]->SetBorderSize(0);
    fSigLegends[th1f]->SetLineColor(1);
    fSigLegends[th1f]->SetLineStyle(1);
    fSigLegends[th1f]->SetLineWidth(1);
    fSigLegends[th1f]->SetFillColor(0);
    fSigLegends[th1f]->SetFillStyle(0);
    fSigLegends[th1f]->SetTextFont(42);

  }
}

void OverlayPlots::InitOutputPlots()
{

  //------------------------------------------------------------------------
  // Init output th1f hists 
  //------------------------------------------------------------------------
  fOutDataTH1FHists.resize(fNTH1F); // make enough space for data double hists
  fOutBkgTH1FHists.resize(fNTH1F);  // make enough space for bkg double hists
  fOutSigTH1FHists.resize(fNTH1F);  // make enough space for sig double hists

  if (Config::doQCDrewgt) fGJetsClone.resize(fNTH1F);

  if (Config::mergeBkgs){
    fOutDYBkgHists.resize(fNTH1F);
    fOutEWK1BkgHists.resize(fNTH1F);
    fOutEWK2BkgHists.resize(fNTH1F);
    fOutJetsBkgHists.resize(fNTH1F);
    fOutHiggsBkgHists.resize(fNTH1F);
  }
 
}

void OverlayPlots::InitInputPlots()
{

  //------------------------------------------------------------------------
  // Init input th1f hists 
  //------------------------------------------------------------------------
  fInDataTH1FHists.resize(fNTH1F);
  fInBkgTH1FHists.resize(fNTH1F);
  fInSigTH1FHists.resize(fNTH1F);
  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){ // loop over double hists
    //------------------------------------------------------------------------
    // Init data hists 
    //------------------------------------------------------------------------
    fInDataTH1FHists[th1f].resize(fNData); 
    for (Int_t data = 0; data < fNData; data++) { // init data double hists
	fInDataTH1FHists[th1f][data] = (TH1F*)fDataFiles[data]->Get(Form("%s",fTH1FNames[th1f].Data()));	
	CheckValidTH1F(fInDataTH1FHists[th1f][data],fTH1FNames[th1f],fDataFiles[data]->GetName());
    }

    //------------------------------------------------------------------------
    // Init bkg hists 
    //------------------------------------------------------------------------
    fInBkgTH1FHists[th1f].resize(fNBkg); 
    for (Int_t mc = 0; mc < fNBkg; mc++) { // init mc double hists
      fInBkgTH1FHists[th1f][mc] = (TH1F*)fBkgFiles[mc]->Get(Form("%s",fTH1FNames[th1f].Data()));
      CheckValidTH1F(fInBkgTH1FHists[th1f][mc],fTH1FNames[th1f],fBkgFiles[mc]->GetName());
      fInBkgTH1FHists[th1f][mc]->SetLineColor(Config::colorMap[fBkgNames[mc]]);
    }

    //------------------------------------------------------------------------
    // Init sig hists 
    //------------------------------------------------------------------------
    fInSigTH1FHists[th1f].resize(fNSig); 
    for (Int_t mc = 0; mc < fNSig; mc++) { // init mc double hists
      fInSigTH1FHists[th1f][mc] = (TH1F*)fSigFiles[mc]->Get(Form("%s",fTH1FNames[th1f].Data()));
      CheckValidTH1F(fInSigTH1FHists[th1f][mc],fTH1FNames[th1f],fSigFiles[mc]->GetName());
      fInSigTH1FHists[th1f][mc]->SetLineColor(Config::colorMap[fSigNames[mc]]);
    }
  }
}

void OverlayPlots::InitTH1FNamesAndSubDNames()
{

  //------------------------------------------------------------------------
  // Choose the histograms to run 
  //------------------------------------------------------------------------
  std::ifstream plots_to_run;
  plots_to_run.open(Form("%s/%s",Config::outdir.Data(),Config::plotnames.Data()),std::ios::in);

  TString plotname;
  TString subdir;

  while (plots_to_run >> plotname >> subdir) {
    fTH1FNames.push_back(plotname);
    fTH1FSubDMap[plotname] = subdir;
  }
  plots_to_run.close();

  // store the size of the number of plots
  fNTH1F = fTH1FNames.size();

  if (fTH1FNames.size() == 0) {
    std::cerr << "Somehow, no plots were read in for the stacker ...exiting..." << std::endl;
    exit(1);
  }
}

void OverlayPlots::OpenInputFiles()
{

  //------------------------------------------------------------------------
  // Get input files 
  //------------------------------------------------------------------------
  fDataFiles.resize(fNData);
  for (Int_t data = 0; data < fNData; data++) {
    TString datafile = Form("%s/%s/plots_%s.root",Config::outdir.Data(),fDataNames[data].Data(),fDataNames[data].Data());
    fDataFiles[data] = TFile::Open(datafile.Data());
    CheckValidFile(fDataFiles[data],datafile);
  }
  fBkgFiles.resize(fNBkg);
  for (Int_t mc = 0; mc < fNBkg; mc++) {
    TString mcfile = Form("%s/%s/plots_%s.root",Config::outdir.Data(),fBkgNames[mc].Data(),fBkgNames[mc].Data());
    fBkgFiles[mc] = TFile::Open(mcfile.Data());
    CheckValidFile(fBkgFiles[mc],mcfile);
  }
  fSigFiles.resize(fNSig);
  for (Int_t mc = 0; mc < fNSig; mc++) {
    TString mcfile = Form("%s/%s/plots_%s.root",Config::outdir.Data(),fSigNames[mc].Data(),fSigNames[mc].Data());
    fSigFiles[mc] = TFile::Open(mcfile.Data());
    CheckValidFile(fSigFiles[mc],mcfile);
  }
}


OverlayPlots::~OverlayPlots()
{

  //------------------------------------------------------------------------
  // Finish by deleting everything
  //------------------------------------------------------------------------
  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){
    delete fOutDataTH1FHists[th1f];
    delete fOutBkgTH1FHists[th1f];
    delete fOutSigTH1FHists[th1f];
 
    delete fTH1FLegends[th1f];
    delete fSigLegends[th1f]; 

    if (Config::doQCDrewgt) delete fGJetsClone[th1f];
 
    if (Config::mergeBkgs){
      delete fOutDYBkgHists[th1f];
      delete fOutEWK1BkgHists[th1f];
      delete fOutEWK2BkgHists[th1f];
      delete fOutJetsBkgHists[th1f];
      delete fOutHiggsBkgHists[th1f];
    }

    for (Int_t data = 0; data < fNData; data++) { delete fInDataTH1FHists[th1f][data]; } 
    for (Int_t mc = 0; mc < fNBkg; mc++)        { delete fInBkgTH1FHists[th1f][mc]; } 
    for (Int_t mc = 0; mc < fNSig; mc++)        { delete fInSigTH1FHists[th1f][mc]; } 
  }

  for (Int_t data = 0; data < fNData; data++) { delete fDataFiles[data]; }
  for (Int_t mc = 0; mc < fNBkg; mc++)        { delete fBkgFiles[mc]; }
  for (Int_t mc = 0; mc < fNSig; mc++)        { delete fSigFiles[mc]; }

  delete fOutFile;
}


