import FWCore.ParameterSet.Config as cms

SiStripDataCompressor = cms.EDProducer("SiStripDataCompressor",
   clustersToBeCompressed = cms.InputTag("siStripClusters")
)


