#include "RecoLocalTracker/SiStripDataCompressor/interface/SiStripCompressionAlgorithm.h"
#include "RecoLocalTracker/SiStripDataCompressor/include/anlz4cmssw.h"
#include <iostream>

SiStripCompressionAlgorithm::SiStripCompressionAlgorithm(){
    this->LoadRealModelDataFromFile();
}

void SiStripCompressionAlgorithm::compress(vclusters_t const & ncColl, vcomp_clusters_t& compColl){
    
    for (vclusters_t::const_iterator itInColl = ncColl.begin(); itInColl!= ncColl.end(); itInColl++) {
      vcomp_clusters_t::TSFastFiller ff(compColl, itInColl->detId());
      commpressDetModule(*itInColl, ff);
      if (ff.empty())
        ff.abort(); 
    }
    
}

void SiStripCompressionAlgorithm::commpressDetModule(const clusters_t& ncClusters, vcomp_clusters_t::TSFastFiller& compressedClusters) {

  std::vector<std::vector<uint8_t>> toBeCompressed;
  std::vector<std::uint8_t> compAmplitudes;
  SiStripDetSetCompressedCluster compCluster;

  for (clusters_t::const_iterator itNcClusters = ncClusters.begin(); itNcClusters!= ncClusters.end(); itNcClusters++){
    compCluster.push_back_supportInfo(itNcClusters->firstStrip(), itNcClusters->isMerged(), itNcClusters->getSplitClusterError());
    toBeCompressed.push_back(itNcClusters->amplitudes());        
  }
  
  std::size_t evtSz = anlz4cmssw_compress(toBeCompressed, compAmplitudes);  
  std::cout << "To be compressed zise: " << toBeCompressed.size() << " Compressed Amplitude Size: " << compAmplitudes.size() << " Lib size: " << evtSz << std::endl;
  compCluster.loadCompressedAmplitudes(compAmplitudes);
  compressedClusters.push_back(compCluster);
    
}
     

/*

  uint16_t nAPVFlagged = 0;
  auto beginAPV = ncDigi.begin();
  const auto indigis_end = ncDigi.end();
  auto iAPV = firstAPV;
  while (beginAPV != indigis_end) {
    const auto endAPV = std::lower_bound(beginAPV, indigis_end, SiStripDigi((iAPV + 1) * 128, 0));
    const auto nDigisInAPV = std::distance(beginAPV, endAPV);
    
      digivector_t workDigis(128, -1024);
      for (auto it = beginAPV; it != endAPV; ++it) {
        workDigis[it->strip() - 128 * iAPV] = it->adc() * 2 - 1024;
      }
      
      const auto apvFlagged = restorer->inspectAndRestore(
          ncDigi.id, iAPV, workDigisPedSubtracted, workDigis, subtractorCMN->getAPVsCM());
      nAPVFlagged += apvFlagged;
      
       // return SiStripDigi(inDigi.strip(), suppressor->truncate(inDigi.adc()));
      

   
*/  

void SiStripCompressionAlgorithm::LoadRealModelDataFromFile(){
    
    std::uint8_t modelBuf[65536];
    std::vector<std::uint8_t> model;
    FILE *fTrained = std::fopen("/afs/cern.ch/user/i/icali/waCompStudies_11_1_6/RecoLocalTracker/SiStripDataCompressor/data/model.dat", "rb");
        while (true) {
            std::size_t rdSz = std::fread(modelBuf, 1, sizeof modelBuf, fTrained);
            if (rdSz <= 0)
                break;
            model.insert(model.end(), modelBuf, &modelBuf[rdSz]);
        }
        std::fclose(fTrained);

        anlz4cmssw_load_trained_model(model);
    
}

//void SiStripCompressionAlgorithm::compressDataContent() {
//}

/*
inline void SiStripZeroSuppression::processHybrid(const edm::DetSetVector<SiStripDigi>& input) {
  for (const auto& inDigis : input) {
    edm::DetSet<SiStripDigi> suppressedDigis(inDigis.id);

    uint16_t nAPVflagged = 0;
    nAPVflagged = algorithms->suppressHybridData(inDigis, suppressedDigis);

    storeExtraOutput(inDigis.id, nAPVflagged);
    if (!suppressedDigis.empty())
      output_base.push_back(std::move(suppressedDigis));
  }
}



template <class T>
inline void ThreeThresholdAlgorithm::endCandidate(State& state, T& out) const {
  if (candidateAccepted(state)) {
    applyGains(state);
    appendBadNeighbors(state);
    if (siStripClusterTools::chargePerCM(state.det().detId, state.ADCs.begin(), state.ADCs.end()) > minGoodCharge)
      out.push_back(SiStripCluster(firstStrip(state), state.ADCs.begin(), state.ADCs.end()));
  }
  clearCandidate(state);
}
*/






