#ifndef _combinertools_
#define _combinertools_

#include "Style.hh"

#include "TFile.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TString.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TROOT.h"
#include "TMath.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "THStack.h"
#include "TLine.h"
#include "TEfficiency.h"

#include <iostream>
#include <cmath>

typedef std::vector<TFile*>   		TFileVec;
typedef std::vector<TH1D*>    		TH1DVec;
typedef std::vector<TH1DVec>  		TH1DVecVec;
typedef std::vector<THStack*> 		THStackVec;
typedef std::vector<TPad*>    		TPadVec;
typedef std::vector<TLegend*> 		TLegVec;
typedef std::vector<TCanvas*> 		TCanvVec;
typedef std::vector<TLine*>   		TLineVec;
typedef std::vector<TEfficiency*> 	TEffVec;

class Combiner{
public:
  Combiner(const SamplePairVec Samples, const Double_t inLumi, const ColorMap colorMap, const TString outname, const Bool_t doNmin1, const Bool_t do_stack, const TString type, const Bool_t doQCDrescale, const Int_t whichSelection, const Bool_t mergeBkgs);
  void InitCanvAndHists();
  void InitTH1DNames();
  void DoComb();
  void VtxEfficiencies();
  void PhotonIDEfficiencies(const UInt_t mc, const UInt_t isType);
  void FindMETEfficiencies();
  void MakeMETEffPlots();
  void MakeEffPlots();
  void GetEfficiency(TH1D *h_num, TH1D *h_den, const TString name, const TString sample);
  void StackPlots();
  void MakeOutputCanvas();
  void DrawCanvasStack(const UInt_t th1d, const Bool_t isLogY);
  void DrawCanvasOverlay(const UInt_t th1d, const Bool_t isLogY);
  void MakeRatioPlots();
  void MakeRatioLine(const UInt_t th1d);
  Double_t GetMaximum(const UInt_t th1d, const Bool_t stack);
  Double_t GetMinimum(const UInt_t th1d, const Bool_t stack);
  ~Combiner();

private:
  TString	addText;
  Double_t 	lumi;
  Bool_t	doStack;
  Bool_t	doQCDscale;
  Bool_t	doMergeBkgs;
  TString	fType;
  Int_t		fWhichSel;
  Double_t	METcut;

  UInt_t	fNData;
  UInt_t	fNBkg;
  UInt_t	fNSig;

  UInt_t	fIndexEff;
  UInt_t	fIndexMET;
  UInt_t	fNMETPlots;
  UInt_t	fIndexMgg;
  UInt_t 	fIndexPtgg;
  UInt_t	fIndexNvtx;
  UInt_t	fIndexDphi;
  UInt_t	fIndexDeta;

  UInt_t	fNTH1D;
  TStrVec	fTH1DNames;

  TStrVec 	fDataNames; 
  TStrVec	fBkgNames;
  TStrVec	fSigNames;

  TPadVec	fOutTH1DStackPads;
  TPadVec	fOutTH1DRatioPads;

  TFileVec	fDataFiles;
  TFileVec	fBkgFiles;
  TFileVec	fSigFiles;

  ColorMap	fColorMap;
  TStrMap	fSampleTitleMap;

  TH1DVecVec	fInDataTH1DHists;
  TH1DVecVec	fInBkgTH1DHists;
  TH1DVecVec	fInSigTH1DHists;

  TStrVec	SystMET;
  TStrMap	fSystMETTitleMap;
  ColorMap	fColorMapMETEff;
  TH1DVecVec	fOutBkgMETEffTH1DHists;
  TH1DVecVec	fOutSigMETEffTH1DHists;

  TH1DVec	GJetsClone;

  TH1DVec	fOutBkgTH1DHists;
  TH1DVec	fOutSigTH1DHists;
  TH1DVec	fOutDataTH1DHists;
  TH1DVec	fOutHiggsBkgTH1DHists;
  TH1DVec	fOutEWK1phoBkgTH1DHists;
  TH1DVec	fOutEWK2phoBkgTH1DHists;
  TH1DVec	fOutJetsphoBkgTH1DHists;
  TH1DVec	fOutDYJetsBkgTH1DHists;

  TH1DVec	fOutRatioTH1DHists;
  TH1DVec	fOutRatioTH1DHistsCopy;
  TLineVec	fOutTH1DRatioLines;

  TH1DVec	fOutTH1DComb;
  THStackVec    fOutBkgTH1DStacks;
  THStackVec	fOutBkgTH1DStacksForUncer;
  TLegVec	fTH1DLegends;
  TLegVec	fTH1DLegendsSig;
  TCanvVec	fOutTH1DCanvases;

  TString	fOutDir;
  TString	fOutName;
  TFile *	fOutFile;


};
#endif
