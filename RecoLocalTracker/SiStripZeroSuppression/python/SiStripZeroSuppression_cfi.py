import FWCore.ParameterSet.Config as cms

from RecoLocalTracker.SiStripZeroSuppression.DefaultAlgorithms_cff import *

siStripZeroSuppression = cms.EDProducer("SiStripZeroSuppression",

    Algorithms = DefaultAlgorithms,
    RawDigiProducersList = cms.VInputTag( cms.InputTag('siStripDigis','VirginRaw'), 
                                          cms.InputTag('siStripDigis','ProcessedRaw'),
                                          cms.InputTag('siStripDigis','ScopeMode')),
                                       #   cms.InputTag('siStripDigis','ZeroSuppressed')),


    DigisToMergeZS = cms.InputTag('siStripDigis','ZeroSuppressed'),
    DigisToMergeVR = cms.InputTag('siStripVRDigis','VirginRaw'),
                                    

    storeCM = cms.bool(True), 
    fixCM= cms.bool(False),                # put -999 into CM collection for "inspected" APV

    produceRawDigis = cms.bool(True),     # if mergeCollection is True, produceRawDigi is not considered
    produceCalculatedBaseline = cms.bool(False),
    produceBaselinePoints = cms.bool(False),
    storeInZScollBadAPV = cms.bool(True), # it selects if in the ZS collection the bad APVs are written. To be kept for ZS
    mergeCollections = cms.bool(False),
    produceHybridFormat = cms.bool(False)
    
    
)
