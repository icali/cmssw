#include "RecoLocalTracker/SiStripDataCompressor/plugins/SiStripDataCompressor.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/transform.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


SiStripDataCompressor::SiStripDataCompressor(const edm::ParameterSet& conf)
    : inputTags(conf.getParameter<std::vector<edm::InputTag>>("DigiProducersList")) {
  produces<edmNew::DetSetVector<SiStripDigi>>();
  inputTokens = edm::vector_transform(
      inputTags, [this](edm::InputTag const& tag) { return consumes<vdigis_t>(tag); });
}

void SiStripDataCompressor::produce(edm::Event& event, const edm::EventSetup& es) {
  auto output = std::make_unique<vdigis_t>();

  edm::Handle<vdigis_t> digis;


  for (auto const& token : inputTokens) {
    if (findInput(token, digis, event)){
      //algorithm->compress(*digis, *output);
    }else
      edm::LogError("Input Not Found") << "[SiStripDataCompressor::produce] ";  // << tag;
  }

  //LogDebug("Output") << output->dataSize() << " clusters from " << output->size() << " modules";
  //output->shrink_to_fit();
  event.put(std::move(output));
}

template <class T>
inline bool SiStripDataCompressor::findInput(const edm::EDGetTokenT<T>& tag, edm::Handle<T>& handle, const edm::Event& e) {
  e.getByToken(tag, handle);
  return handle.isValid();
}
