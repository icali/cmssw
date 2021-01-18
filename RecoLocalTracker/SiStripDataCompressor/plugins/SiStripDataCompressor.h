#ifndef RecoLocalTracker_SiStripDataCompressor_h
#define RecoLocalTracker_SiStripDataCompressor_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"


#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"


#include "RecoLocalTracker/SiStripDataCompressor/interface/SiStripCompressionAlgorithm.h"

#include <limits>

#include <vector>
#include <memory>


class SiStripDataCompressor : public edm::stream::EDProducer<> {
public:
  explicit SiStripDataCompressor(const edm::ParameterSet& conf);
  void produce(edm::Event&, const edm::EventSetup&) override;

private:

  typedef edm::EDGetTokenT<vclusters_t> token_t;
  typedef std::vector<token_t> token_v;
  
  template <class T> bool findInput(const edm::EDGetTokenT<T>&, edm::Handle<T>&, const edm::Event&);
  const std::vector<edm::InputTag> inputTags;
  std::unique_ptr<SiStripCompressionAlgorithm> algorithm;
  token_v inputTokens;
};

#endif
