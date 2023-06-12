# MonoHiggsToGG

## Setting up flashgg dev_legacy_runII with MonoHiggsToGG

1. Get flashgg (for more details [link](https://github.com/cms-analysis/flashgg/tree/dev_legacy_runII) )
   ```
   export SCRAM_ARCH=slc7_amd64_gcc700
   cmsrel CMSSW_10_6_29
   cd CMSSW_10_6_29/src
   cmsenv
   git cms-init
   cd $CMSSW_BASE/src 
   git clone -b dev_legacy_runII https://github.com/cms-analysis/flashgg 
   source flashgg/setup_flashgg.sh
   ```
   
2. Get MonoHiggsToGG (after following above steps) by cloning this repo: `git clone -b main git@github.com:shdutta16/MonoHiggsToGG.git`

3. Copy the `LeptonSelection` from MonoHiggsToGG setup to flashgg setup. This contains additional methods that are used by MonoHiggsToGG.
   ```
   cp MonoHiggsToGG/analysis/addfiles/LeptonSelection.h flashgg/Taggers/interface/LeptonSelection_MHgg.h
   cp MonoHiggsToGG/analysis/addfiles/LeptonSelection.cc flashgg/Taggers/src/LeptonSelection_MHgg.cc
   ```
   
   ```
   std::vector<edm::Ptr<flashgg::Muon> > selectLooseMuons( const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, Ptr<flashgg::DiPhotonCandidate> dipho,
            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double muonEtaThreshold, double muonPtThreshold, double muPFIsoSumRelThreshold,
            double dRPhoLeadMuonThreshold, double dRPhoSubLeadMuonThreshold );
   ```
   
4. Build: `scram b -j 10`

5. To run:
   ```
   cd $CMSSW_BASE/src
   cd MonoHiggsToGG/analysis/test/MC_sig/ggTomonoH_aa_sinp0p35_tanb1p0_MXd10_MH3_300_MH4_200/
   cmsenv
   cmsRun diPhoAna_2018.py
   ```
6. This will create the ntuple `diPhotons_legacy2018.root`.


