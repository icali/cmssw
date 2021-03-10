#include "RecoLocalTracker/SiStripClusterizer/test/SiStripApproximatedClustersDump.h"


SiStripApproximatedClustersDump::SiStripApproximatedClustersDump(const edm::ParameterSet& conf) {
  inputTagClusters = conf.getParameter< edm::InputTag >("approximatedClustersTag");
  clusterToken = consumes< edmNew::DetSetVector<SiStripApproximateClusterv2>>(inputTagClusters);
  
  usesResource("TFileService");

  outNtuple = fs->make<TNtuple>("ApproxClusters", "ApproxClusters", "detId:barycenter:charge");
  
}

SiStripApproximatedClustersDump::~SiStripApproximatedClustersDump() {}

void SiStripApproximatedClustersDump::analyze(const edm::Event& event, const edm::EventSetup& es) {
  
  edm::Handle<edmNew::DetSetVector<SiStripApproximateClusterv2>> inClusters;
  event.getByToken(clusterToken, inClusters);

  for (edmNew::DetSetVector<SiStripApproximateClusterv2>::const_iterator itApprox = inClusters->begin(); itApprox!= inClusters->end(); itApprox++) {
    uint32_t detId = itApprox->detId();
    
    for (edmNew::DetSet<SiStripApproximateClusterv2>::const_iterator itClusters = itApprox->begin(); itClusters!= itApprox->end(); itClusters++){
      uint16_t barywidth = itClusters->baryWith();
      uint8_t avCharge=itClusters->avgCharge();
      outNtuple->Fill(detId, barywidth, avCharge);
    }
  }
}


