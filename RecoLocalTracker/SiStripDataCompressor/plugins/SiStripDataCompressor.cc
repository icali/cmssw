#include "RecoLocalTracker/SiStripDataCompressor/plugins/SiStripDataCompressor.h"
#include "FWCore/Utilities/interface/transform.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


SiStripDataCompressor::SiStripDataCompressor(const edm::ParameterSet& conf){
    inputTagClusters = conf.getParameter< edm::InputTag >("clustersToBeCompressed");
    clusterToken = consumes< edmNew::DetSetVector< SiStripCluster > >(inputTagClusters);
    
    produces<vcomp_clusters_t>();
}

void SiStripDataCompressor::produce(edm::Event& event, const edm::EventSetup& es) {
  auto outClusters = std::make_unique<vcomp_clusters_t>();

  edm::Handle<vclusters_t> inClusters;
  event.getByToken(clusterToken, inClusters);

    
  algorithm->compress(*inClusters, *outClusters);
    
  

  //LogDebug("Output") << outClusters->dataSize() << " clusters from " << outClusters->size() << " modules";
  //outClusters->shrink_to_fit();
  event.put(std::move(outClusters));
}
