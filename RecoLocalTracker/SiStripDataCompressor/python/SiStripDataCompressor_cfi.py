import FWCore.ParameterSet.Config as cms

SiStripDataCompressor = cms.EDProducer("SiStripDataCompressor",
                               DigiProducersList = cms.VInputTag(
                                  cms.InputTag('siStripDigis','ZeroSuppressed')
                               )
)


