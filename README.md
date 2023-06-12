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
   
   ```
   std::vector<edm::Ptr<flashgg::Muon> > selectLooseMuons( const std::vector<edm::Ptr<flashgg::Muon> > &muonPointers, Ptr<flashgg::DiPhotonCandidate> dipho,
            const std::vector<edm::Ptr<reco::Vertex> > &vertexPointers, double muonEtaThreshold, double muonPtThreshold, double muPFIsoSumRelThreshold,
            double dRPhoLeadMuonThreshold, double dRPhoSubLeadMuonThreshold )
    {
        std::vector<edm::Ptr<flashgg::Muon> > goodMuons;
        for( unsigned int muonIndex = 0; muonIndex < muonPointers.size(); muonIndex++ ) {
            Ptr<flashgg::Muon> muon = muonPointers[muonIndex];

            /*                                                                                                                                                                                              
            std::cout << " Muon index " << muonIndex << " has pt eta weight: "                                                                                                                              
                      << muon->pt() << " " << muon->eta() << " "                                                                                                                                            
                      << muon->centralWeight() << std::endl;                                                                                                                                                
            auto weightList = muon->weightList();                                                                                                                                                           
            for( unsigned int i = 0 ; i < weightList.size() ; i++ ) {                                                                                                                                       
                std::cout << "    " << weightList[i] << " " << muon->weight( weightList[i] );                                                                                                               
            }                                                                                                                                                                                               
            std::cout << std::endl;                                                                                                                                                                         
            */

            if( fabs( muon->eta() ) > muonEtaThreshold ) continue;
            if( muon->pt() < muonPtThreshold ) continue;

            int vtxInd = 0;
            double dzmin = 9999;
            for( size_t ivtx = 0 ; ivtx < vertexPointers.size(); ivtx++ ) {
                Ptr<reco::Vertex> vtx = vertexPointers[ivtx];
                if( !muon->innerTrack() ) continue;
                if( fabs( muon->innerTrack()->vz() - vtx->position().z() ) < dzmin ) {
                    dzmin = fabs( muon->innerTrack()->vz() - vtx->position().z() );
                    vtxInd = ivtx;
                }
            }

            Ptr<reco::Vertex> best_vtx = vertexPointers[vtxInd];

            if( !muon::isLooseMuon( *muon ) ) continue;
            //if( !muon::isSoftMuon( *muon, *best_vtx ) ) continue;
            
            double muPFIsoSumRel = ( muon->pfIsolationR04().sumChargedHadronPt
                                     + max( 0.,muon->pfIsolationR04().sumNeutralHadronEt
                                            + muon->pfIsolationR04().sumPhotonEt - 0.5 * muon->pfIsolationR04().sumPUPt ) ) / ( muon->pt() );

            float dRPhoLeadMuon = deltaR( muon->eta(), muon->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
            float dRPhoSubLeadMuon = deltaR( muon->eta(), muon->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),      dipho->subLeadingPhoton()->superCluster()->phi() );

            if( dRPhoLeadMuon < dRPhoLeadMuonThreshold || dRPhoSubLeadMuon < dRPhoSubLeadMuonThreshold ) continue;
            if( muPFIsoSumRel > muPFIsoSumRelThreshold ) continue;

            goodMuons.push_back( muon );
        }
        return goodMuons;
    }
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


