#include "RecoLocalTracker/SiStripClusterizer/interface/SiStripClusters2ApproxClustersv2.h"
#include <iostream>


SiStripClusters2ApproxClustersv2::SiStripClusters2ApproxClustersv2(const edm::ParameterSet& conf){
   inputClusters = conf.getParameter< edm::InputTag >("inputClusters");
   clusterToken = consumes< edmNew::DetSetVector< SiStripCluster > >(inputClusters);

   produces< edmNew::DetSetVector< SiStripApproximateClusterv2 > >(); 

}

void SiStripClusters2ApproxClustersv2::produce(edm::Event& e, edm::EventSetup const&){
  std::unique_ptr<edmNew::DetSetVector< SiStripApproximateClusterv2 > > result(new edmNew::DetSetVector< SiStripApproximateClusterv2 > );

  edm::Handle<edmNew::DetSetVector< SiStripCluster >> clusterCollection;
  e.getByToken(clusterToken, clusterCollection);

  uint32_t minID = 470444276;
  int maxFirst = -1;

  for( edmNew::DetSetVector<SiStripCluster>::const_iterator i = clusterCollection->begin(); i!=clusterCollection->end(); i++){

    std::vector< SiStripApproximateClusterv2 > tempVec;    

    edmNew::DetSetVector<SiStripApproximateClusterv2>::FastFiller ff = edmNew::DetSetVector<SiStripApproximateClusterv2>::FastFiller(*result, i->id());

    for( edmNew::DetSet<SiStripCluster>::const_iterator j = i->begin(); j!=i->end(); j++){

      if(maxFirst<j->firstStrip()) maxFirst = j->firstStrip();
      if(minID>i->id()) minID = i->id();

      uint8_t width = j->amplitudes().size();
      uint16_t barycenter = (uint16_t)j->barycenter();

      int charge = 0;
      for (unsigned k = 0; k < j->amplitudes().size(); k++) {
        charge += (int)j->amplitudes()[k];
      }

      SiStripApproximateClusterv2 approxCluster = SiStripApproximateClusterv2( (uint8_t)(charge/width), barycenter, width );
      ff.push_back(approxCluster);
    }
    

  }

  e.put(std::move(result));
}


