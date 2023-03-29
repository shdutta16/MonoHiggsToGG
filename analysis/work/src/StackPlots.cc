#include "../interface/StackPlots.hh"

StackPlots::StackPlots()
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
  fOutDir = Form("%s/stacks",Config::outdir.Data()); // where to put output stack plots 
  MakeOutDir(fOutDir); // make output directory 
  fOutFile = new TFile(Form("%s/stackplots_canv.root",fOutDir.Data()),"RECREATE"); // ouput tfile to store canvases here for quick editting

  //------------------------------------------------------------------------
  // Initialize objects
  //------------------------------------------------------------------------
  StackPlots::OpenInputFiles();            // open the input files
  StackPlots::InitTH1FNamesAndSubDNames(); // get histo names from plotnames file
  MakeSubDirs(fTH1FSubDMap,fOutDir);       // setup output subdir
  StackPlots::InitInputPlots();            // initialize input histos
  StackPlots::InitOutputCanvPads();        // initialize output canv
  StackPlots::InitOutputPlots();           // initialize output histos
  StackPlots::InitOutputLegends();         // initialize output legends
  StackPlots::InitRatioPlots();            // initialize output ratio plots
  StackPlots::InitRatioLines();            // initialize output ratio lines

}

void StackPlots::DoStack(std::ofstream & yields)
{

  //------------------------------------------------------------------------
  // Start running
  //------------------------------------------------------------------------
  gStyle->SetOptStat(0);
  gStyle->SetFrameLineWidth(3);
  StackPlots::MakeStackPlots(yields);
  StackPlots::MakeRatioPlots();
  StackPlots::MakeOutputCanvas();

}

void StackPlots::FixPlotStyling(const Int_t th1f)
{

  //------------------------------------------------------------------------
  // Make ratio plot pretty 
  //------------------------------------------------------------------------
  fOutTH1FRatioPads[th1f]->SetTicky(1);
  fOutRatioTH1FHists[th1f]->SetNdivisions(505);
  fOutRatioTH1FHists[th1f]->GetXaxis()->SetTickLength(0.07);
  fOutRatioTH1FHists[th1f]->GetXaxis()->SetTickSize(0.11);
  
  //------------------------------------------------------------------------
  // Make axis labels pretty 
  //------------------------------------------------------------------------
  fOutDataTH1FHists[th1f]->GetYaxis()->SetLabelSize  (0.05); 
  fOutDataTH1FHists[th1f]->GetYaxis()->SetTitleSize  (0.07);
  fOutDataTH1FHists[th1f]->GetYaxis()->SetTitleOffset(0.8);
  fOutDataTH1FHists[th1f]->GetXaxis()->SetTitleOffset(999);
  fOutDataTH1FHists[th1f]->GetXaxis()->SetLabelSize  (0);

  //------------------------------------------------------------------------
  // Make the bkgs pretty 
  //------------------------------------------------------------------------
  fOutBkgTH1FHists[th1f]->SetMarkerSize(0);
  fOutBkgTH1FHists[th1f]->SetFillColor(kGray+3);
  fOutBkgTH1FHists[th1f]->SetFillStyle(3013);
  if (Config::mergeBkgs){
    fOutHiggsBkgHists[th1f]->SetFillColor(Config::colorMap["SMHiggs"]);
    fOutEWK1BkgHists[th1f]->SetFillColor(Config::colorMap["EWK1pho"]);
    fOutEWK2BkgHists[th1f]->SetFillColor(Config::colorMap["EWK2pho"]);
    fOutJetsBkgHists[th1f]->SetFillColor(Config::colorMap["Jetspho"]);
    fOutDYBkgHists[th1f]->SetFillColor(Config::colorMap["DYJetsToLL"]);
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

void StackPlots::MakeOutputCanvas()
{

  //------------------------------------------------------------------------
  // Setup canvases 
  //------------------------------------------------------------------------
  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){
    StackPlots::FixPlotStyling(th1f);      // make histos pretty 

    // draw first with  log scale
    Bool_t isLogY = true;
    StackPlots::DrawUpperPad(th1f,isLogY); // upper pad is stack
    StackPlots::DrawLowerPad(th1f);        // lower pad is ratio
    StackPlots::SaveCanvas(th1f,isLogY);   // now save the canvas, w/ logy

    // draw second with lin scale
    isLogY = false;
    StackPlots::DrawUpperPad(th1f,isLogY); // upper pad is stack
    StackPlots::DrawLowerPad(th1f);        // lower pad is ratio
    StackPlots::SaveCanvas(th1f,isLogY);   // now save the canvas, w/o logy
  }// end th1f loop

}

void StackPlots::DrawUpperPad(const Int_t th1f, const Bool_t isLogY) 
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
  Float_t min = StackPlots::GetMinimum(th1f);
  Float_t max = StackPlots::GetMaximum(th1f);
  if (isLogY) fOutDataTH1FHists[th1f]->SetMaximum(max*1E3);
  else        fOutDataTH1FHists[th1f]->SetMaximum(max*1.5); 
  if (isLogY) fOutDataTH1FHists[th1f]->SetMinimum(0.05); 
  else        fOutDataTH1FHists[th1f]->SetMinimum(0); 
  //------------------------------------------------------------------------
  // Customization for specific histos
  //------------------------------------------------------------------------
  if (isLogY && fTH1FNames[th1f].Contains("t1pfmetCorr",TString::kExact)) fOutDataTH1FHists[th1f]->SetMinimum(0.001);  
  if (isLogY && fTH1FNames[th1f].Contains("t1pfmetCorr",TString::kExact)) fOutDataTH1FHists[th1f]->SetMaximum(max*0.5E1);  
 
  //------------------------------------------------------------------------
  // Draw the data, stack, sig histos, legend
  //------------------------------------------------------------------------
  fOutDataTH1FHists[th1f]->Draw("PE"); // draw first so labels appear

  fOutMCTH1FStacks[th1f]->Draw("HIST SAME");
  fOutTH1FStackPads[th1f]->RedrawAxis("SAME");    // redraw axis (stack kills it)
  fOutBkgTH1FHists[th1f]->Draw("E2 SAME");        // draw MC error
  for ( Int_t mc = 0; mc < fNSig; mc++){
    fInSigTH1FHists[th1f][mc]->Draw("HIST SAME"); // overlay signals
  }
  fOutDataTH1FHists[th1f]->Draw("PE SAME");       // redraw data 
  fTH1FLegends[th1f]->Draw("SAME");               // draw the legend
  fSigLegends[th1f]->Draw("SAME");                // draw sig legend
}

Float_t StackPlots::GetMaximum(const Int_t th1f) 
{

  //------------------------------------------------------------------------
  // Calculate max 
  //------------------------------------------------------------------------
  Float_t max = -1e9;
  if (fOutDataTH1FHists[th1f]->GetBinContent(fOutDataTH1FHists[th1f]->GetMaximumBin()) > fOutBkgTH1FHists[th1f]->GetBinContent(fOutBkgTH1FHists[th1f]->GetMaximumBin())) {
    max = fOutDataTH1FHists[th1f]->GetBinContent(fOutDataTH1FHists[th1f]->GetMaximumBin());
  }
  else {
    max = fOutBkgTH1FHists[th1f]->GetBinContent(fOutBkgTH1FHists[th1f]->GetMaximumBin());
  }
  return max;
}

Float_t StackPlots::GetMinimum(const Int_t th1f) 
{
  //------------------------------------------------------------------------
  // Calculate min 
  //------------------------------------------------------------------------
  // need to loop through to check bin != 0
  Float_t datamin  = 1e9;
  Bool_t newdatamin = false;

  for (Int_t bin = 1; bin <= fOutDataTH1FHists[th1f]->GetNbinsX(); bin++){
    Float_t tmpmin = fOutDataTH1FHists[th1f]->GetBinContent(bin);
    if ((tmpmin < datamin) && (tmpmin > 0)) {
      datamin    = tmpmin;
      newdatamin = true;
    }
    else if ((tmpmin < datamin)) {
      datamin    = tmpmin;
      newdatamin = true;
    }
  }

  Float_t mcmin  = 1e9;
  Bool_t newmcmin = false;
  for (Int_t bin = 1; bin <= fOutBkgTH1FHists[th1f]->GetNbinsX(); bin++){
    Float_t tmpmin = fOutBkgTH1FHists[th1f]->GetBinContent(bin);
    if ((tmpmin < mcmin) && (tmpmin > 0)) {
      mcmin    = tmpmin;
      newmcmin = true;
    }
    else if ((tmpmin < mcmin)) {
      mcmin    = tmpmin;
      newmcmin = true;
    }
  }
  
  // now set return variable min
  Float_t min = 1; // to not royally mess up logy plots in case where plots have no fill and no new min is set for data or mc
  if (newdatamin || newmcmin) {
    if (datamin < mcmin) {
      min = datamin;
    }
    else {
      min = mcmin;
    }
  }
  return min;
}

void StackPlots::DrawLowerPad(const Int_t th1f) 
{    

  //------------------------------------------------------------------------
  // Draw the ratio plot 
  //------------------------------------------------------------------------
  fOutTH1FCanvases[th1f]->cd();          // Go back to the main canvas before defining pad2
  fOutTH1FRatioPads[th1f]->Draw();       // draw lower pad
  fOutTH1FRatioPads[th1f]->cd();         // lower pad is current pad

  StackPlots::SetLines(th1f);            // make line at 1 
  fOutRatioTH1FHists[th1f]->Draw("EP");  // draw first so line can appear
  fOutTH1FRatioLines[th1f]->Draw("SAME");

  //------------------------------------------------------------------------
  // Style for the ratio plot 
  //------------------------------------------------------------------------
  fOutRatioTH1FHists[th1f]->GetYaxis()->SetNdivisions(505);
  // X
  fOutRatioTH1FHists[th1f]->GetXaxis()->SetLabelSize  (0.157); 
  fOutRatioTH1FHists[th1f]->GetXaxis()->SetTitleSize  (0.16);
  fOutRatioTH1FHists[th1f]->GetXaxis()->SetTickLength (0.07);
  fOutRatioTH1FHists[th1f]->GetXaxis()->SetTickSize   (0.11);
  fOutRatioTH1FHists[th1f]->GetXaxis()->SetTitleOffset(1.02);
  // Y
  fOutRatioTH1FHists[th1f]->GetYaxis()->SetLabelSize  (0.13); 
  fOutRatioTH1FHists[th1f]->GetYaxis()->SetTitleSize  (0.14);
  fOutRatioTH1FHists[th1f]->GetYaxis()->SetTitleOffset(0.38);
  fOutRatioTH1FHists[th1f]->GetYaxis()->CenterTitle();

  fOutRatioTH1FHists[th1f]->Draw("EP SAME"); // redraw to go over line
  fOutRatioErrors[th1f]->Draw("E SAME");     // draw errors when ratio bigger than max
  fOutRatioMCErrs[th1f]->Draw("E2 SAME");    // plots MC error copy
  fRatioLegends[th1f]->Draw("SAME");         // draw legend
}

void StackPlots::SetLines(const Int_t th1f)
{

  //------------------------------------------------------------------------
  // Make line at ratio of 1.0 over whole x range
  //------------------------------------------------------------------------
  fOutTH1FRatioLines[th1f]->SetX1(fOutRatioTH1FHists[th1f]->GetXaxis()->GetXmin());
  fOutTH1FRatioLines[th1f]->SetX2(fOutRatioTH1FHists[th1f]->GetXaxis()->GetXmax());
  fOutTH1FRatioLines[th1f]->SetY1(1.0);
  fOutTH1FRatioLines[th1f]->SetY2(1.0);

  //------------------------------------------------------------------------
  // Customize appearance of line
  //------------------------------------------------------------------------
  fOutTH1FRatioLines[th1f]->SetLineColor(kBlack);
  fOutTH1FRatioLines[th1f]->SetLineWidth(1);
}

void StackPlots::SaveCanvas(const Int_t th1f, const Bool_t isLogY)
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


void StackPlots::MakeStackPlots(std::ofstream & yields)
{

  //------------------------------------------------------------------------
  // Copy plots into output hists/stacks
  //------------------------------------------------------------------------
  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){

    //------------------------------------------------------------------------
    // Copy data
    //------------------------------------------------------------------------
    for (Int_t data = 0; data < fNData; data++){
      if (data==0) fOutDataTH1FHists[th1f] = (TH1F*)fInDataTH1FHists[th1f][data]->Clone();
      else         fOutDataTH1FHists[th1f]->Add(fInDataTH1FHists[th1f][data]);
    }
    fTH1FLegends[th1f]->AddEntry(fOutDataTH1FHists[th1f],"Data","EPL");
 
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
      qcdint  = fInBkgTH1FHists[th1f][fBkgIndicesMap["QCD"]]->Integral();
      gjetint = fInBkgTH1FHists[th1f][fBkgIndicesMap["GJets"]]->Integral(); 
      fGJetsClone[th1f] = (TH1F*)fInBkgTH1FHists[th1f][fBkgIndicesMap["GJets"]]->Clone();
      if (qcdint > 0 && gjetint > 0) fGJetsClone[th1f]->Scale(qcdint/gjetint);
    }  

    //------------------------------------------------------------------------
    // Copy bkg -- to use as unc. 
    //------------------------------------------------------------------------
    Double_t index_non0int = 0;
    for (Int_t mc = 0; mc < fNBkg; mc++){
      if (fInBkgTH1FHists[th1f][mc]->Integral() <= 0) continue;
      else{ index_non0int = mc; break; }
    }
    fOutBkgTH1FHists[th1f] = (TH1F*)fInBkgTH1FHists[th1f][index_non0int]->Clone();
    for (Int_t mc = index_non0int+1; mc < fNBkg; mc++){
      if (fBkgNames[mc]=="QCD" && qcdint > 0){
        if (Config::doQCDrewgt) fOutBkgTH1FHists[th1f]->Add(fGJetsClone[th1f]);
        else                    fOutBkgTH1FHists[th1f]->Add(fInBkgTH1FHists[th1f][mc]); }
      else    fOutBkgTH1FHists[th1f]->Add(fInBkgTH1FHists[th1f][mc]);
    }

    //------------------------------------------------------------------------
    // Scale to data if called 
    //------------------------------------------------------------------------
    if (Config::useData && Config::scaleToData){
      Double_t dataint = fOutDataTH1FHists[th1f]->Integral();
      Double_t bkgint  = fOutBkgTH1FHists[th1f]->Integral();
      //------------------------------------------------------------------------
      // Add the bkg int. in the blinded region of mgg plots 
      //------------------------------------------------------------------------
      if (fTH1FNames[th1f].Contains("mgg",TString::kExact) && Config::doBlind){
        Int_t binLo = fOutBkgTH1FHists[th1f]->FindBin(115);
        Int_t binHi = fOutBkgTH1FHists[th1f]->FindBin(135);
        Float_t mggint = fOutBkgTH1FHists[th1f]->Integral(binLo,binHi);
        dataint += mggint;
      }
      //Double_t scale = dataint/bkgint;
      Double_t scale = 1.082;
      fOutBkgTH1FHists[th1f]->Scale(scale);
      if (Config::doQCDrewgt && qcdint > 0) fGJetsClone[th1f]->Scale(scale);
      for (Int_t mc = 0; mc < fNBkg; mc++ ){
        if (fBkgNames[mc]!="GluGluHToGG" && fBkgNames[mc]!="VHToGG" && fBkgNames[mc]!="VBFHToGG" && fBkgNames[mc]!="ttHJetToGG"){// dont rewgt SM hgg
          fInBkgTH1FHists[th1f][mc]->Scale(scale);
        } 
      }
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
    // Add bkgs to stack after scaling & ordered legend entries  
    //------------------------------------------------------------------------
    if (Config::mergeBkgs){ // add merged bkgs -- ordered
      fOutMCTH1FStacks[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["VHToGG"]]);
      fOutMCTH1FStacks[th1f]->Add(fOutHiggsBkgHists[th1f]);
      fOutMCTH1FStacks[th1f]->Add(fOutEWK2BkgHists[th1f]);
      fOutMCTH1FStacks[th1f]->Add(fOutDYBkgHists[th1f]);
      fOutMCTH1FStacks[th1f]->Add(fOutEWK1BkgHists[th1f]);
      fOutMCTH1FStacks[th1f]->Add(fOutJetsBkgHists[th1f]);
      fOutMCTH1FStacks[th1f]->Add(fInBkgTH1FHists[th1f][fBkgIndicesMap["DiPhoton"]]);

      fTH1FLegends[th1f]->AddEntry(fOutDYBkgHists[th1f],Config::nameMap["DYJetsToLL"],"f"); 
      fTH1FLegends[th1f]->AddEntry(fInBkgTH1FHists[th1f][fBkgIndicesMap["VHToGG"]],Config::nameMap["VHToGG"],"f");
      fTH1FLegends[th1f]->AddEntry(fOutEWK1BkgHists[th1f],Config::nameMap["EWK1pho"],"f");
      fTH1FLegends[th1f]->AddEntry(fOutHiggsBkgHists[th1f],Config::nameMap["SMHiggs"],"f");
      fTH1FLegends[th1f]->AddEntry(fOutJetsBkgHists[th1f],Config::nameMap["Jetspho"],"f");
      fTH1FLegends[th1f]->AddEntry(fOutEWK2BkgHists[th1f],Config::nameMap["EWK2pho"],"f");
      fTH1FLegends[th1f]->AddEntry(fInBkgTH1FHists[th1f][fBkgIndicesMap["DiPhoton"]],Config::nameMap["DiPhoton"],"f");
    }
    else { // if not merging, just add all bkgs
      for (Int_t mc = 0; mc < fNBkg; mc++){
        if (fBkgNames[mc]=="QCD"){
          if (Config::doQCDrewgt) fOutMCTH1FStacks[th1f]->Add(fGJetsClone[th1f]);
          else fOutMCTH1FStacks[th1f]->Add(fInBkgTH1FHists[th1f][mc]);
        } 
        else fOutMCTH1FStacks[th1f]->Add(fInBkgTH1FHists[th1f][mc]);
        fTH1FLegends[th1f]->AddEntry(fInBkgTH1FHists[th1f][mc],Config::nameMap[fBkgNames[mc]],"f");
      }
    }
    fTH1FLegends[th1f]->AddEntry(fOutBkgTH1FHists[th1f],"Stat. Unc.","F");

    //------------------------------------------------------------------------
    // Make yields table  
    //------------------------------------------------------------------------
    if (fTH1FNames[th1f] == Config::yieldsPlot){
      //------------------------------------------------------------------------
      // Calculate yields
      //------------------------------------------------------------------------
      float m0 = 105; float m1 = 115; float m2 = 135; float m3 = 180; // region to perform integrals
      //------------------------------------------------------------------------
      // Data 
      //------------------------------------------------------------------------
      double dataint1, dataerr1, dataint2, dataerr2, dataint, dataerr; 
      if (Config::useData){
        dataint1 = GetIntegralAndError(fOutDataTH1FHists[th1f],m0,m1,dataerr1);
        dataint2 = GetIntegralAndError(fOutDataTH1FHists[th1f],m2,m3,dataerr2);
        if (Config::doBlind){ dataint = dataint1 + dataint2; dataerr = TMath::Sqrt(dataint); }
        else{ dataint = GetIntegralAndError(fOutDataTH1FHists[th1f],m0,m3,dataerr); }
      }

      //------------------------------------------------------------------------
      // Bkg MC 
      //------------------------------------------------------------------------
      DblVec bkgints, bkgerrs; bkgints.resize(fNBkg); bkgerrs.resize(fNBkg);
      for (UInt_t mc = 0; mc < fNBkg; mc++){
        bkgints[mc] = GetIntegralAndError(fInBkgTH1FHists[th1f][mc],m0,m3,bkgerrs[mc]);
      }

      //------------------------------------------------------------------------
      // Total Bkg 
      //------------------------------------------------------------------------
      double bkgint1, bkgerr1, bkgint2, bkgerr2, bkgintSB, bkgerrSB, bkgint, bkgerr;
      bkgint1 = GetIntegralAndError(fOutBkgTH1FHists[th1f],m0,m1,bkgerr1);
      bkgint2 = GetIntegralAndError(fOutBkgTH1FHists[th1f],m2,m3,bkgerr2);
      if (Config::doBlind){ bkgintSB = bkgint1 + bkgint2; bkgerrSB = TMath::Sqrt(bkgerr1*bkgerr1+bkgerr2*bkgerr2); }
      bkgint = GetIntegralAndError(fOutBkgTH1FHists[th1f],m0,m3,bkgerr); 
 
      //------------------------------------------------------------------------
      // Write out the table
      //------------------------------------------------------------------------
      yields << "\\documentclass[a4paper,landscape]{article}" << std::endl;
      yields << "\\usepackage[paperheight=15.0in,paperwidth=6.0in,margin=1.0in,headheight=0.0in,footskip=0.5in,includehead,includefoot]{geometry}" << std::endl;
      yields << "\\begin{document}" << std::endl;
      yields << "\% Integrals " << std::endl; // start summary of results table 
      yields << "" << std::endl;
      yields << "\\begin{table}[bthp]" <<std::endl;
      yields << "\\begin{tabular}{|l|r|}" <<std::endl;
      yields << "\\hline \\hline" <<std::endl;
      yields << "\\multicolumn{2}{|c|}{" << fTH1FNames[th1f] << "} \\\\" << std::endl;
      yields << "\\multicolumn{2}{|c|}{" << Form("$\\sqrt{s}$ = 13 TeV; L = %1.1f $fb^{-1}$",Config::lumi) <<"} \\\\" <<std::endl;
      yields << "\\hline \\hline" << std::endl;
      yields << "Sample & Integral \\\\" << std::endl;
      yields << "\\hline" <<std::endl;
    
      for (UInt_t mc = 0; mc < fNBkg; mc++){
        yields << fBkgNames[mc] << " & " << TString::Format("%2.1f",bkgints[mc]) << " $\\pm$ " << TString::Format("%2.1f",bkgerrs[mc]) << " \\\\ " << std::endl;
      } 
      yields << "\\hline" <<std::endl;
      if (Config::doBlind) yields << " Total Bkg (sideband) & " << bkgintSB << " $\\pm$ " << TString::Format("%2.1f",bkgerrSB) << " \\\\ " << std::endl;
                           yields << " Total Bkg & "            << bkgint   << " $\\pm$ " << TString::Format("%2.1f",bkgerr)   << " \\\\ " << std::endl;
      yields << "\\hline" <<std::endl;
      if (Config::doBlind) yields << " Data (sideband) & "      << dataint  << " $\\pm$ " << TString::Format("%2.1f",dataerr)  << " \\\\" << std::endl; 
      else                 yields << " Data & "                 << dataint  << " $\\pm$ " << TString::Format("%2.1f",dataerr)  << " \\\\" << std::endl; 

      yields << "\\hline" << std::endl;
      yields << "\\end{tabular}" <<std::endl;
      yields << "\\end{table}" <<std::endl; // end summary of results table
      yields << "" << std::endl;
      yields << "\\end{document}" <<std::endl;
    }

  }// end th1f loop

}

void StackPlots::MakeRatioPlots()
{

  //------------------------------------------------------------------------
  // Setup ratio plots 
  //------------------------------------------------------------------------
  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){
    if( fNData > 0 ) fOutRatioTH1FHists[th1f] = (TH1F*)fOutDataTH1FHists[th1f]->Clone();
    else             fOutRatioTH1FHists[th1f] = (TH1F*)fOutBkgTH1FHists[th1f]->Clone();

    //fOutRatioTH1FHists[th1f]->Add(fOutBkgTH1FHists[th1f],-1.0);  
    fOutRatioTH1FHists[th1f]->Divide(fOutBkgTH1FHists[th1f]); 
    fOutRatioTH1FHists[th1f]->SetTitle("");
    fOutRatioTH1FHists[th1f]->GetYaxis()->SetTitle("Data/MC");
    TString xtitle  = fOutBkgTH1FHists[th1f]->GetXaxis()->GetTitle();
    fOutRatioTH1FHists[th1f]->GetXaxis()->SetTitle(xtitle);
    fOutRatioTH1FHists[th1f]->SetLineColor(kGray+3);
    fOutRatioTH1FHists[th1f]->SetMinimum(-0.1); // Define Y ..
    fOutRatioTH1FHists[th1f]->SetMaximum(2.2);  // .. range
    fOutRatioTH1FHists[th1f]->SetStats(0);      // No statistics on lower plot

    //------------------------------------------------------------------------
    // error for when ratio is above max 
    //------------------------------------------------------------------------
    fOutRatioErrors[th1f] = (TH1F*)fOutRatioTH1FHists[th1f]->Clone();
    fOutRatioErrors[th1f]->SetMarkerStyle(0);
    for (UInt_t bin=0; bin<=fOutRatioTH1FHists[th1f]->GetNbinsX(); bin++){
      float bincontent = fOutRatioTH1FHists[th1f]->GetBinContent(bin);
      float binerror   = fOutRatioTH1FHists[th1f]->GetBinError(bin);
      float histomax   = fOutRatioTH1FHists[th1f]->GetMaximum();
      if (bincontent > histomax){ 
        float difference = bincontent - histomax;
        fOutRatioErrors[th1f]->SetBinContent(bin,histomax-0.000001);
        fOutRatioErrors[th1f]->SetBinError(bin,binerror-difference);
      }
    }

    //------------------------------------------------------------------------
    // ratio MC error plot
    //------------------------------------------------------------------------
    fOutRatioMCErrs[th1f] = (TH1F*)fOutBkgTH1FHists[th1f]->Clone();
    fOutRatioMCErrs[th1f]->Divide(fOutBkgTH1FHists[th1f]);
    fOutRatioMCErrs[th1f]->SetFillColor(kGray+3);
    fOutRatioMCErrs[th1f]->SetFillStyle(3013);
    fOutRatioMCErrs[th1f]->SetMarkerSize(0);
    fRatioLegends[th1f]->AddEntry(fOutRatioMCErrs[th1f], "Stat. Unc.","f"); //add this to legend

  }// end th1f loop
}

void StackPlots::InitRatioPlots()
{

  //------------------------------------------------------------------------
  // Initialize ratio plots 
  //------------------------------------------------------------------------
  fOutRatioTH1FHists.resize(fNTH1F); // th1f hists
  fOutRatioMCErrs.resize(fNTH1F);    // mc err hists
  fOutRatioErrors.resize(fNTH1F);    // err for ratios above max 
}

void StackPlots::InitRatioLines()
{

  //------------------------------------------------------------------------
  // Initialize lines for ratio plots 
  //------------------------------------------------------------------------
  fOutTH1FRatioLines.resize(fNTH1F);
  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){
    fOutTH1FRatioLines[th1f] = new TLine();
  }
}

void StackPlots::InitOutputCanvPads()
{

  //------------------------------------------------------------------------
  // Initialize canvas and pads 
  //------------------------------------------------------------------------
  fOutTH1FCanvases.resize(fNTH1F);
  fOutTH1FStackPads.resize(fNTH1F);
  fOutTH1FRatioPads.resize(fNTH1F);
  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){
    fOutTH1FCanvases[th1f] = new TCanvas(Form("%s_canv",fTH1FNames[th1f].Data()),"",550,550);
    fOutTH1FCanvases[th1f]->cd();
    
    fOutTH1FStackPads[th1f] = new TPad(Form("%s_upad",fTH1FNames[th1f].Data()),"",0.0,0.3,1.0,1.0);
    fOutTH1FStackPads[th1f]->SetBottomMargin(0.03);
    fOutTH1FStackPads[th1f]->SetTopMargin(0.06);
    fOutTH1FStackPads[th1f]->SetRightMargin(0.05040323); 
    fOutTH1FStackPads[th1f]->SetLeftMargin(0.1290323);    
 
    fOutTH1FRatioPads[th1f] = new TPad(Form("%s_lpad",fTH1FNames[th1f].Data()),"",0.0,0.0,1.0,0.3);
    fOutTH1FRatioPads[th1f]->SetTopMargin(0.0);
    fOutTH1FRatioPads[th1f]->SetRightMargin(0.05040323);
    fOutTH1FRatioPads[th1f]->SetLeftMargin(0.1290323);
    fOutTH1FRatioPads[th1f]->SetBottomMargin(0.366666678814);
  }
}



void StackPlots::InitOutputLegends()
{

  //------------------------------------------------------------------------
  // Initialize legends 
  //------------------------------------------------------------------------
  fTH1FLegends.resize(fNTH1F);
  fSigLegends.resize(fNTH1F);
  fRatioLegends.resize(fNTH1F);

  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){
    fTH1FLegends[th1f] = new TLegend(0.45,0.69,0.92,0.92,NULL,"brNDC"); // (x1,y1,x2,y2)
    fTH1FLegends[th1f]->SetTextSize(0.036);
    fTH1FLegends[th1f]->SetNColumns(2);
    fTH1FLegends[th1f]->SetBorderSize(0);
    fTH1FLegends[th1f]->SetLineColor(1);
    fTH1FLegends[th1f]->SetLineStyle(1);
    fTH1FLegends[th1f]->SetLineWidth(1);
    fTH1FLegends[th1f]->SetFillColor(0);
    fTH1FLegends[th1f]->SetFillStyle(0);
    fTH1FLegends[th1f]->SetTextFont(42);

    fSigLegends[th1f] = new TLegend(0.45,0.55,0.74,0.68); // (x1,y1,x2,y2)
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

    fRatioLegends[th1f] = new TLegend(0.65,0.85,0.94,0.94);// (x1,y1,x2,y2)
    fRatioLegends[th1f]->SetFillColor(0);
    fRatioLegends[th1f]->SetLineColor(0);
    fRatioLegends[th1f]->SetShadowColor(0);
    fRatioLegends[th1f]->SetTextFont(42);
    fRatioLegends[th1f]->SetTextSize(0.09);
    fRatioLegends[th1f]->SetBorderSize(1);
    //fRatioLegends[th1f]->SetNColumns(2);

  }
}

void StackPlots::InitOutputPlots()
{

  //------------------------------------------------------------------------
  // Init output th1f hists 
  //------------------------------------------------------------------------
  fOutDataTH1FHists.resize(fNTH1F); // make enough space for data double hists
  fOutBkgTH1FHists.resize(fNTH1F);  // make enough space for bkg double hists
  fOutSigTH1FHists.resize(fNTH1F);  // make enough space for sig double hists
  fOutMCTH1FStacks.resize(fNTH1F);  // same with stack
  fOutMCUncStacks.resize(fNTH1F);   // same with stack for uncer.

  if (Config::doQCDrewgt) fGJetsClone.resize(fNTH1F);

  if (Config::mergeBkgs){
    fOutDYBkgHists.resize(fNTH1F);
    fOutEWK1BkgHists.resize(fNTH1F);
    fOutEWK2BkgHists.resize(fNTH1F);
    fOutJetsBkgHists.resize(fNTH1F);
    fOutHiggsBkgHists.resize(fNTH1F);
  }
 
  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){
    fOutMCTH1FStacks[th1f] = new THStack(Form("%s_stack",fTH1FNames[th1f].Data()),"");
    fOutMCUncStacks[th1f]  = new THStack(Form("%s_uncer",fTH1FNames[th1f].Data()),"");
  }
}

void StackPlots::InitInputPlots()
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
      fInBkgTH1FHists[th1f][mc]->SetFillColor(Config::colorMap[fBkgNames[mc]]);
    }

    //------------------------------------------------------------------------
    // Init sig hists 
    //------------------------------------------------------------------------
    fInSigTH1FHists[th1f].resize(fNSig); 
    for (Int_t mc = 0; mc < fNSig; mc++) { // init mc double hists
      fInSigTH1FHists[th1f][mc] = (TH1F*)fSigFiles[mc]->Get(Form("%s",fTH1FNames[th1f].Data()));
      CheckValidTH1F(fInSigTH1FHists[th1f][mc],fTH1FNames[th1f],fSigFiles[mc]->GetName());
      fInSigTH1FHists[th1f][mc]->SetFillColor(Config::colorMap[fSigNames[mc]]);
    }
  }
}

void StackPlots::InitTH1FNamesAndSubDNames()
{

  //------------------------------------------------------------------------
  // Choose the histograms to run 
  //------------------------------------------------------------------------
  std::ifstream plots_to_run;
  TString plotpath = Form("%s/%s",Config::outdir.Data(),Config::plotnames.Data());
  std::cout << "Looking for plots in : " << plotpath << std::endl;
  plots_to_run.open(plotpath,std::ios::in);

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

void StackPlots::OpenInputFiles()
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


StackPlots::~StackPlots()
{

  //------------------------------------------------------------------------
  // Finish by deleting everything
  //------------------------------------------------------------------------
  for (Int_t th1f = 0; th1f < fNTH1F; th1f++){
    delete fOutDataTH1FHists[th1f];
    delete fOutBkgTH1FHists[th1f];
    delete fOutSigTH1FHists[th1f];
    delete fOutMCTH1FStacks[th1f];
    delete fOutMCUncStacks[th1f];
 
    delete fTH1FLegends[th1f];
    delete fSigLegends[th1f]; 
    delete fRatioLegends[th1f];

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


