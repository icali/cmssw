#ifndef RecoLocalTracker_SiStripDigiCompressor_h
#define RecoLocalTracker_SiStripDigiCompressor_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <vector>
#include <memory>

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
