#ifndef RecoLocalTracker_SiStripDigiCompressor_h
#define RecoLocalTracker_SiStripDigiCompressor_h

#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripDigi/interface/SiStripProcessedRawDigi.h"

#include <vector>
#include <memory>
#include <limits>

//namespace edm {
//  class EventSetup;
//}
class SiStripDigi;

class SiStripDigiCompressor : public edm::stream::EDProducer<> {
public:
  explicit SiStripDigiCompressor(const edm::ParameterSet& conf);
  void produce(edm::Event&, const edm::EventSetup&) override;

private:
  template <class T>
  bool findInput(const edm::EDGetTokenT<T>&, edm::Handle<T>&, const edm::Event&);
  const std::vector<edm::InputTag> inputTags;
  typedef edm::EDGetTokenT<edm::DetSetVector<SiStripDigi> > token_t;
  typedef std::vector<token_t> token_v;
  token_v inputTokens;
};

#endif
