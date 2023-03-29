#ifndef _config_
#define _config_

#include "CommonTypes.hh"

namespace Config{
 
  //------------------------------------------------------------------------
  // General configuration
  //------------------------------------------------------------------------

  constexpr    Float_t lumi = 35.9; // fb^-1
  static const TString extraText = "Preliminary";

  //------------------------------------------------------------------------
  // TDRStyle Config --> needed by stacker
  //------------------------------------------------------------------------
  constexpr    Float_t  TitleSize    = 0.035;
  constexpr    Float_t  TitleXOffset = 1.1;
  constexpr    Float_t  TitleYOffset = 1.1;

  constexpr    Float_t  LabelOffset = 0.007;
  constexpr    Float_t  LabelSize   = 0.03;

  constexpr    Float_t  TickLength  = 0.03;

  constexpr    Float_t  TitleFF     = 1.4; // offset fudge factor   

  //------------------------------------------------------------------------
  // METcorr configuration
  //------------------------------------------------------------------------

  

  //------------------------------------------------------------------------
  // Objects set at command line and in main 
  //------------------------------------------------------------------------

  extern TString      outdir;
  extern Bool_t       doBlind;
  extern Bool_t       doAnalysis;
  extern Bool_t       doStack;
  extern Bool_t       doComb;
  extern Bool_t       makeCards;
  extern TString      plotnames;
  extern Bool_t       mergeBkgs;
  extern Bool_t       doQCDrewgt;
  extern Bool_t       scaleToData;
  extern TString      yieldsPlot;
  extern Int_t        whichSel;
  extern Bool_t       doEffPlots;
  extern Bool_t       doStandard;
  extern Bool_t       doNminus1;
  extern Bool_t       useData;
  extern Bool_t       doMETcor;
  extern TString      outtype;

  extern TStrBoolMap  SampleMap;
  extern ColorMap     colorMap;
  extern TStrMap      nameMap;
  extern TStrMap      SampleLatexMap;
 
};

#endif

