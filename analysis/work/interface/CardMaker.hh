#ifndef _cardmaker_
#define _cardmaker_

#include "CommonTypes.hh"
#include "Common.hh"
#include "Config.hh"

typedef std::vector<TFile*>	TFileVec;
typedef std::vector<TTree*>	TTreeVec;

class CardMaker{
public:
  CardMaker(TString indir);
  void MakeCards();
  void OpenInputFiles(TString indir);
  ~CardMaker();

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
  TTreeVec	fDataTrees;
  TTreeVec	fBkgTrees;
  TTreeVec	fSigTrees;
  TStrIntMap	fBkgIndicesMap;

};

#endif
