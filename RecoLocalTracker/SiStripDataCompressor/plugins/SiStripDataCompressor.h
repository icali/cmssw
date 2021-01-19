#ifndef RecoLocalTracker_SiStripDataCompressor_h
#define RecoLocalTracker_SiStripDataCompressor_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/SiStripDetSetCompressedCluster/interface/SiStripDetSetCompressedCluster.h"

#include "RecoLocalTracker/SiStripDataCompressor/interface/SiStripCompressionAlgorithm.h"

#include <vector>
#include <memory>


class SiStripDataCompressor : public edm::stream::EDProducer<> {
public:
  explicit SiStripDataCompressor(const edm::ParameterSet& conf);
  void produce(edm::Event&, const edm::EventSetup&) override;

private:

  std::unique_ptr<SiStripCompressionAlgorithm> algorithm;
    
  edm::InputTag inputTagClusters;
  edm::EDGetTokenT<vclusters_t> clusterToken;

  
};

#endif
