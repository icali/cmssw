#include "RecoLocalTracker/SiStripDigiCompressor/plugins/SiStripDigiCompressor.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/transform.h"

SiStripDigiCompressor::SiStripDigiCompressor(const edm::ParameterSet& conf)
    : inputTags(conf.getParameter<std::vector<edm::InputTag>>("DigiProducersList")) {
  produces<edmNew::DetSetVector<SiStripDigi>>();
  inputTokens = edm::vector_transform(
      inputTags, [this](edm::InputTag const& tag) { return consumes<edm::DetSetVector<SiStripDigi>>(tag); });
}

void SiStripDigiCompressor::produce(edm::Event& event, const edm::EventSetup& es) {
  auto output = std::make_unique<edmNew::DetSetVector<SiStripCluster>>();
  output->reserve(10000, 4 * 10000);

  edm::Handle<edm::DetSetVector<SiStripDigi>> inputOld;
  //   edm::Handle< edmNew::DetSetVector<SiStripDigi> >  inputNew;

  //algorithm->initialize(es);

  for (auto const& token : inputTokens) {
    if (findInput(token, inputOld, event)){
    //  algorithm->clusterize(*inputOld, *output);
    //     else if( findInput( tag, inputNew, event) ) algorithm->clusterize(*inputNew, *output);
    }else
      edm::LogError("Input Not Found") << "[SiStripDigiCompressor::produce] ";  // << tag;
  }

  LogDebug("Output") << output->dataSize() << " clusters from " << output->size() << " modules";
  output->shrink_to_fit();
  event.put(std::move(output));
}

template <class T>
inline bool SiStripDigiCompressor::findInput(const edm::EDGetTokenT<T>& tag, edm::Handle<T>& handle, const edm::Event& e) {
  e.getByToken(tag, handle);
  return handle.isValid();
}
