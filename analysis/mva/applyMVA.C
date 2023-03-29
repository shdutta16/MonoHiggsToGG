#include "TROOT.h"

void applyMVA(TString path, TString mva, TString addname, Int_t opt, TString sample)
{

  // --- input file
  TString filepath = "/afs/cern.ch/work/m/mzientek/public/25ns_v80X_MVA/";
  TString name = TString::Format("%s_skim_woVetos",sample.Data());
  TString model = "BARY";

  TFile* data = new TFile(TString::Format("%s%s.root",filepath.Data(),name.Data()));
  TTree* tree = (TTree*)data->Get("DiPhotonTree");

  // --- output file
  std::cout << "Running option: " << opt << std::endl;
  TFile* fout = new TFile(TString::Format("%sOutputMVA_%s_opt%d%s_%s.root",filepath.Data(),mva.Data(),opt,addname.Data(),name.Data()),"RECREATE");
  TTree* tout = new TTree("DiPhotonTree","");

  // --- copy histograms
  TH1F * h_entries         = (TH1F*)data->Get("h_entries");
  TH1F * h_sumW            = (TH1F*)data->Get("h_sumW");
  TH1F * h_selection       = (TH1F*)data->Get("h_selection");
  TH1F * h_selection_unwgt = (TH1F*)data->Get("h_selection_unwgt");
  TH1F * h_numbers         = (TH1F*)data->Get("h_numbers");

  // --- get the Reader to handle application
  TMVA::Tools::Instance();
  TMVA::Reader *reader = new TMVA::Reader( "!:Color:!Silent" );

  // --- setup input variables (names must match weight file names & order)
  int nvar = 8;
  Float_t var[nvar-1];
  // MVA variables
  reader->AddVariable( "dphiggmet",   &var[0]);
  reader->AddVariable( "ptgg",        &var[1]);
  if (opt > 1) reader->AddVariable( "t1pfmetCorr", &var[2]);
  if (opt > 2) reader->AddVariable( "nJets30",     &var[3]);
  if (opt > 2) reader->AddVariable( "nEle",        &var[4]);
  if (opt > 2) reader->AddVariable( "nMuons",      &var[5]);
  if (opt > 3) reader->AddVariable( "phigg",       &var[6]);
  if (opt > 4) reader->AddVariable( "mgg",         &var[7]);

  TString MVAname = TString::Format("%s_%s_opt%d%s/weights/TMVAClassification_%s.weights.xml",mva.Data(),model.Data(),opt,addname.Data(),mva.Data());
  reader->BookMVA(TString::Format("%s method",mva.Data()),MVAname);

  // --- pick input variables in test sample
  Float_t testVar0;
  Float_t testVar1;
  Float_t testVar2;
  Float_t testVar3;
  Float_t testVar4;
  Int_t  itestVar0;
  Int_t  itestVar1;
  Int_t  itestVar2;

  // variables 
  tree->SetBranchAddress( "dphiggmet",   &testVar0);
  tree->SetBranchAddress( "ptgg",        &testVar1);
  if (opt > 1) tree->SetBranchAddress( "t1pfmetCorr", &testVar2);
  if (opt > 2) tree->SetBranchAddress( "nJets30",     &itestVar0);
  if (opt > 2) tree->SetBranchAddress( "nEle",        &itestVar1);
  if (opt > 2) tree->SetBranchAddress( "nMuons",      &itestVar2);
  if (opt > 3) tree->SetBranchAddress( "phigg",       &testVar3);
  tree->SetBranchAddress( "mgg",         &testVar4);

  Float_t	mgg;
  Float_t 	eta1;
  Float_t 	eta2;
  Float_t	chiso1;
  Float_t	chiso2;
  Int_t 	lumi; 
  Int_t 	run;
  UInt_t	event;
  Float_t	weight;
  tree->SetBranchAddress( "eta1",	&eta1);
  tree->SetBranchAddress( "eta2",	&eta2);
  tree->SetBranchAddress( "chiso1",	&chiso1);
  tree->SetBranchAddress( "chiso2",	&chiso2);
  tree->SetBranchAddress( "lumi",	&lumi);
  tree->SetBranchAddress( "run",	&run);
  tree->SetBranchAddress( "event",	&event);
  tree->SetBranchAddress( "weight",	&weight);

  // --- add more variables to view later
  tout->Branch( "mgg",          &testVar4);
  tout->Branch( "eta1",		&eta1);
  tout->Branch( "eta2",		&eta2);
  tout->Branch( "chiso1",	&chiso1);
  tout->Branch( "chiso2",	&chiso2);
  tout->Branch( "lumi",		&lumi);
  tout->Branch( "run",		&run);
  tout->Branch( "event",	&event);
  tout->Branch( "weight",	&weight);
  if (opt > 0) tout->Branch( "dphiggmet",   &testVar0);
  if (opt > 0) tout->Branch( "ptgg",        &testVar1);
  if (opt > 1) tout->Branch( "t1pfmetCorr", &testVar2);
  if (opt > 2) tout->Branch( "nJets30",     &itestVar0);
  if (opt > 2) tout->Branch( "nEle",        &itestVar1);
  if (opt > 2) tout->Branch( "nMuons",      &itestVar2);
  if (opt > 3) tout->Branch( "phigg",       &testVar3);
  
  // --- add branch to store response 
  Float_t MVA_response = 0.;
  tout->Branch("MVA_response", &MVA_response);

  // --- Loop over all events in tree
  for (Long64_t event=0; event < tree->GetEntries(); event++){
    if (event%1000==0) std::cout << "...Processing event: " << event << std::endl;
   
    tree->GetEntry(event);

    var[0] = testVar0;
    var[1] = testVar1;
    if (opt > 1) var[2] = testVar2;
    if (opt > 2) var[3] = float(itestVar0);
    if (opt > 2) var[4] = float(itestVar1);
    if (opt > 2) var[5] = float(itestVar2);
    if (opt > 3) var[6] = testVar3;
    if (opt > 4) var[7] = testVar4;
    mgg = testVar4;

    MVA_response = reader->EvaluateMVA( TString::Format("%s method",mva.Data()) );
    //std::cout << "MVA response: " << MVA_response << event << std::endl;
    //if (event%1000==0) std::cout << "MVA response: " << MVA_response << event << std::endl;
 
    tout->Fill();
 
  }

  // --- write out histos
  h_entries->Write();
  h_sumW->Write();
  h_selection->Write();
  h_selection_unwgt->Write();
  h_numbers->Write();  

  // --- save tree
  tout->Write();
}
