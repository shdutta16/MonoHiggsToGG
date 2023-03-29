#include "../interface/Config.hh"

namespace Config{

  //------------------------------------------------------------------------
  // Default Config parameters
  //------------------------------------------------------------------------
  TString outdir      = "diPhoPlots";
  Bool_t  doBlind     = true; 
  Bool_t  doAnalysis  = false;
  Bool_t  doStack     = false;
  Bool_t  doComb      = false;
  Bool_t  makeCards   = false;
  TString plotnames   = "plotnames.txt";
  Bool_t  mergeBkgs   = false;
  Bool_t  doQCDrewgt  = false;
  Bool_t  scaleToData = false;
  TString yieldsPlot  = "mgg_hiMET";
  Int_t   whichSel    = 0;
  Bool_t  doEffPlots  = false;
  Bool_t  doStandard  = false;
  Bool_t  doNminus1   = false;
  Bool_t  useData     = false;
  Bool_t  doMETcor    = false;
  TString outtype     = "png";

  TStrBoolMap SampleMap;      // set in main.cc
  ColorMap    colorMap;       // set in main.cc
  TStrMap     nameMap;        // set in main.cc
  TStrMap     SampleLatexMap; // set in main.cc

}
