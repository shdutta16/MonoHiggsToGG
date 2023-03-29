#ifndef _stackplots_
#define _stackplots_

#include "CommonTypes.hh"
#include "Common.hh"
#include "Config.hh"

#include "TCanvas.h"
#include "TPad.h"
#include "THStack.h"
#include "TLegend.h"
#include "TLine.h"
#include "TLatex.h"
#include "TMath.h"

typedef std::vector<TFile*>	TFileVec;
typedef std::vector<TH1F*>	TH1FVec;
typedef std::vector<TH1FVec>	TH1FVecVec;
typedef std::vector<THStack*>	THStackVec;
typedef std::vector<TLegend*>	TLegVec;
typedef std::vector<TLine*>	TLineVec;
typedef std::vector<TCanvas*>	TCanvVec;
typedef std::vector<TPad*>	TPadVec;

class StackPlots{
public:
  StackPlots();
  void OpenInputFiles();
  void InitTH1FNamesAndSubDNames();
  void InitInputPlots();
  void InitOutputPlots();
  void InitOutputLegends();
  void InitRatioPlots();
  void InitRatioLines();
  void InitOutputCanvPads();

  void DoStack(std::ofstream & yields);
  void MakeStackPlots(std::ofstream & yields);
  void MakeRatioPlots();
  void MakeOutputCanvas();

  void FixPlotStyling(const Int_t th1f); 
  void DrawUpperPad(const Int_t th1f, const Bool_t isLogY);
  void DrawLowerPad(const Int_t th1f);
  void SetLines(const Int_t th1f);
  void SaveCanvas(const Int_t th1f, const Bool_t isLogY);
  Float_t GetMaximum(const Int_t th1f);
  Float_t GetMinimum(const Int_t th1f);

  ~StackPlots();

private:
  TString	fOutDir;
  TStrMap	fTH1FSubDMap;
  TFile*	fOutFile;
  TStrVec	fTH1FNames;
  UInt_t	fNTH1F;

  TFileVec	fDataFiles;
  TFileVec	fBkgFiles;
  TFileVec	fSigFiles;
  TStrVec	fDataNames;
  TStrVec	fBkgNames;
  TStrVec	fSigNames;
  Int_t		fNData;
  Int_t		fNBkg;
  Int_t		fNSig;
  TH1FVecVec	fInDataTH1FHists;
  TH1FVecVec 	fInBkgTH1FHists;
  TH1FVecVec 	fInSigTH1FHists;

  TH1FVec	fOutDataTH1FHists;
  TH1FVec	fOutBkgTH1FHists;
  TH1FVec	fOutSigTH1FHists;
  THStackVec	fOutMCTH1FStacks;
  THStackVec	fOutMCUncStacks;

  TH1FVec	fOutDYBkgHists;
  TH1FVec	fOutEWK1BkgHists;
  TH1FVec	fOutEWK2BkgHists;
  TH1FVec	fOutJetsBkgHists;
  TH1FVec	fOutHiggsBkgHists;
  TH1FVec	fGJetsClone;
  Double_t	qcdint;
  Double_t	gjetint;
 
  TStrIntMap	fBkgIndicesMap;
  TLegVec	fTH1FLegends;
  TLegVec	fSigLegends;
  TLegVec	fRatioLegends;

  TCanvVec	fOutTH1FCanvases;
  TPadVec	fOutTH1FStackPads;
  TPadVec	fOutTH1FRatioPads;
  TLineVec	fOutTH1FRatioLines;
  TH1FVec	fOutRatioTH1FHists;
  TH1FVec	fOutRatioErrors;
  TH1FVec	fOutRatioMCErrs;

};

#endif 
