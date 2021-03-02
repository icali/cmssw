#include "RecoLocalTracker/SiStripClusterizer/test/SiStripApproximatedClustersDump.h"


SiStripApproximatedClustersDump::SiStripApproximatedClustersDump(const edm::ParameterSet& conf) {
  inputTagClusters = conf.getParameter< edm::InputTag >("approximatedClustersTag");
  clusterToken = consumes< edmNew::DetSetVector<SiStripApproximateClusterv2>>(inputTagClusters);
  
  usesResource("TFileService");

  outNtuple = fs_->make<TNtuple>("ApproxClusters", "ApproxClusters", "detId:barycenter:charge");
  
}

SiStripApproximatedClustersDump::~SiStripApproximatedClustersDump() {}

void SiStripApproximatedClustersDump::analyze(const edm::Event& e, const edm::EventSetup& es) {
  
  edm::Handle<edmNew::DetSetVector<SiStripApproximateClusterv2>> inClusters;
  event.getByToken(clusterToken, inClusters);

  for (edmNew::DetSetVector<SiStripApproximateClusterv2>::const_iterator itApprox = inClusters.begin(); itApprox!= inClusters.end(); itApprox++) {
    uint32_t detId = itApprox->id();
    
  }
}

DEFINE_FWK_MODULE(SiStripApproximatedClustersDump);
