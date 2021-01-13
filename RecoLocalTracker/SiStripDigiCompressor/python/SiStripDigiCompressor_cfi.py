import FWCore.ParameterSet.Config as cms

siStripDigiCompressor = cms.EDProducer("SiStripDigiCompressor",
                               DigiProducersList = cms.VInputTag(
                                  cms.InputTag('siStripDigis','ZeroSuppressed')
                               )
)


