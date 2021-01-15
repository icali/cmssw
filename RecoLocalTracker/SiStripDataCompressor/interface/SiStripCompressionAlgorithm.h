#ifndef RecoLocalTracker_SiStripCompressionAlgorithm_h
#define RecoLocalTracker_SiStripCompressionAlgorithm_h

//namespace edm {
//  class EventSetup;
//}
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

typedef edm::DetSetVector<SiStripDigi> vdigis_t;
typedef edm::DetSet<SiStripDigi> digis_t;
  
typedef edmNew::DetSetVector<SiStripCluster> vcluster_t;
typedef edmNew::DetSet<SiStripCluster> clusters_t;


//class FedChannelConnection;

class SiStripCompressionAlgorithm {
public:
    typedef edm::DetSetVector<SiStripDigi> vdigi_t;
    typedef edm::DetSet<SiStripDigi> digi_t;

    virtual ~SiStripCompressionAlgorithm() {}
    SiStripCompressionAlgorithm();
    
    void compress(const vdigi_t&, vdigi_t&);
  

  

private:
    void LoadRealModelDataFromFile();
    virtual void initialize();
    void commpressDetModule(const digi_t& , digi_t& );
    void compressDataContent();
       

};
#endif