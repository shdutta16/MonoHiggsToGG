#include "../interface/CardMaker.hh"

CardMaker::CardMaker(TString inDir)
{

  //------------------------------------------------------------------------
  // Setup samples
  //------------------------------------------------------------------------
  for (TStrBoolMapIter iter = Config::SampleMap.begin(); iter != Config::SampleMap.end(); ++iter){
    if ((*iter).second) { fDataNames.push_back((*iter).first); } // Data 
    else{
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
  fOutDir = Form("%s/cards",Config::outdir.Data()); // where to put output stack plots 
  MakeOutDir(fOutDir); // make output directory 

  //------------------------------------------------------------------------
  // Initialize objects
  //------------------------------------------------------------------------
  CardMaker::OpenInputFiles(inDir);            // open the input files and trees 

}

void CardMaker::MakeCards()
{

  //------------------------------------------------------------------------
  // Apply additional cuts 
  //------------------------------------------------------------------------
 
  

}



void CardMaker::OpenInputFiles(TString indir)
{

  //------------------------------------------------------------------------
  // Get input files and skimmed trees
  //------------------------------------------------------------------------
  TString treename = "DiPhotonTree";
  fDataFiles.resize(fNData);
  fDataTrees.resize(fNData);
  for (Int_t data = 0; data < fNData; data++) {
    TString datafile = Form("%s/%s_skimmedtree.root",indir.Data(),fDataNames[data].Data());
    fDataFiles[data] = TFile::Open(datafile.Data());
    CheckValidFile(fDataFiles[data],datafile);
    fDataTrees[data] = (TTree*)fDataFiles[data]->Get(treename);
    CheckValidTree(fDataTrees[data],treename,datafile); 
  }
  fBkgFiles.resize(fNBkg);
  fBkgTrees.resize(fNBkg);
  for (Int_t mc = 0; mc < fNBkg; mc++) {
    TString mcfile = Form("%s/%s_skimmedtree.root",indir.Data(),fBkgNames[mc].Data());
    fBkgFiles[mc] = TFile::Open(mcfile.Data());
    CheckValidFile(fBkgFiles[mc],mcfile);
    fBkgTrees[mc] = (TTree*)fBkgFiles[mc]->Get(treename);
    CheckValidTree(fBkgTrees[mc],treename,mcfile); 
  }
  fSigFiles.resize(fNSig);
  fSigTrees.resize(fNSig);
  for (Int_t mc = 0; mc < fNSig; mc++) {
    TString mcfile = Form("%s/%s_skimmedtree.root",indir.Data(),fSigNames[mc].Data());
    fSigFiles[mc] = TFile::Open(mcfile.Data());
    CheckValidFile(fSigFiles[mc],mcfile);
    fSigTrees[mc] = (TTree*)fSigFiles[mc]->Get(treename);
    CheckValidTree(fSigTrees[mc],treename,mcfile);
  }
}

CardMaker::~CardMaker()
{

  //------------------------------------------------------------------------
  // Finish by deleting everything
  //------------------------------------------------------------------------
  for (Int_t data = 0; data < fNData; data++) { delete fDataFiles[data]; }
  for (Int_t mc = 0; mc < fNBkg; mc++)        { delete fBkgFiles[mc];    }
  for (Int_t mc = 0; mc < fNSig; mc++)        { delete fSigFiles[mc];    }
  
}
