#include "RecoLocalTracker/SiStripDigiCompressor/plugins/SiStripDigiCompressor.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/transform.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripRawProcessingFactory.h"
#include "FWCore/Utilities/interface/transform.h"
#include <memory>

SiStripDigiCompressor::SiStripDigiCompressor(const edm::ParameterSet& conf)
    : inputTags(conf.getParameter<std::vector<edm::InputTag>>("DigiProducersList")){

  produces<edmNew::DetSetVector<SiStripDigi>>();
  inputTokens = edm::vector_transform(
      inputTags, [this](edm::InputTag const& tag) { return consumes<edm::DetSetVector<SiStripDigi>>(tag); });
}

void SiStripDigiCompressor::produce(edm::Event& event, const edm::EventSetup& es) {
  auto output = std::make_unique<edmNew::DetSetVector<SiStripDigi>>();
  output->reserve(10000, 4 * 10000);

  edm::Handle<edm::DetSetVector<SiStripDigi>> input;

  for (auto const& token : inputTokens) {
    if (findInput(token, input, event)){
      //algorithm->clusterize(*inputOld, *output);
    } else
      edm::LogError("Input Not Found") << "[SiStripDigiCompressor::produce] ";  // << tag;
  }

  LogDebug("Output") << output->dataSize() << " compressed Digis from " << output->size() << " modules";
  output->shrink_to_fit();
  event.put(std::move(output));
}

template <class T>
inline bool SiStripDigiCompressor::findInput(const edm::EDGetTokenT<T>& tag, edm::Handle<T>& handle, const edm::Event& e) {
  e.getByToken(tag, handle);
  return handle.isValid();
}

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SiStripDigiCompressor);