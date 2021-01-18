#ifndef RecoLocalTracker_SiStripCompressionAlgorithm_h
#define RecoLocalTracker_SiStripCompressionAlgorithm_h

//namespace edm {
//  class EventSetup;
//}
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/SiStripDetSetCompressedCluster/interface/SiStripDetSetCompressedCluster.h"

#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

typedef edm::DetSetVector<SiStripDigi> vdigis_t;
typedef edm::DetSet<SiStripDigi> digis_t;
  
typedef edmNew::DetSetVector<SiStripCluster> vclusters_t;
typedef edmNew::DetSet<SiStripCluster> clusters_t;

typedef edmNew::DetSetVector<SiStripDetSetCompressedCluster> v_comp_clusters_t;
typedef edmNew::DetSet<SiStripDetSetCompressedCluster> comp_clusters_t;


//class FedChannelConnection;

class SiStripCompressionAlgorithm {
public:
  explicit SiStripCompressionAlgorithm();
  virtual ~SiStripCompressionAlgorithm() {}
  
    
  void compress(const vclusters_t&, v_comp_clusters_t&);
  

  

private:
    void LoadRealModelDataFromFile();
    void commpressDetModule(const clusters_t& , v_comp_clusters_t::TSFastFiller& );
    //void compressDataContent();
       

};
#endif