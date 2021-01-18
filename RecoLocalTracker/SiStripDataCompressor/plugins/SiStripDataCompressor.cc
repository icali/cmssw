#include "RecoLocalTracker/SiStripDataCompressor/plugins/SiStripDataCompressor.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/transform.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


SiStripDataCompressor::SiStripDataCompressor(const edm::ParameterSet& conf)
    : inputTags(conf.getParameter<std::vector<edm::InputTag>>("DigiProducersList")) {
  produces<edmNew::DetSetVector<SiStripDigi>>();
  inputTokens = edm::vector_transform(
      inputTags, [this](edm::InputTag const& tag) { return consumes<vclusters_t>(tag); });
}

void SiStripDataCompressor::produce(edm::Event& event, const edm::EventSetup& es) {
  auto outClusters = std::make_unique<vclusters_t>();

  edm::Handle<vclusters_t> inClusters;


  for (auto const& token : inputTokens) {
    if (findInput(token, inClusters, event)){
      //algorithm->compress(*digis, *outClusters);
    }else
      edm::LogError("Input Not Found") << "[SiStripDataCompressor::produce] ";  // << tag;
  }

  //LogDebug("Output") << outClusters->dataSize() << " clusters from " << outClusters->size() << " modules";
  //outClusters->shrink_to_fit();
  event.put(std::move(outClusters));
}

template <class T>
inline bool SiStripDataCompressor::findInput(const edm::EDGetTokenT<T>& tag, edm::Handle<T>& handle, const edm::Event& e) {
  e.getByToken(tag, handle);
  return handle.isValid();
}
