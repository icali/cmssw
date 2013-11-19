# Auto generated configuration file
# using: 
# Revision: 1.381.2.13 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step2 --process reL1 --condition auto:hltonline_8E33v1 --data --output [{"e":"RAW","t":"RAW","o":["drop FEDRawDataCollection_rawDataCollector__LHC"]}] -s L1REPACK
import FWCore.ParameterSet.Config as cms

process = cms.Process('reL1')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
#process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.L1Emulator_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
                            #fileNames = cms.untracked.vstring('/store/group/phys_heavyions/icali/HIHighPt/RAW/181530/A8D6061E-030D-E111-A482-BCAEC532971A.root') #PbPb
                            fileNames = cms.untracked.vstring('/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_PAZeroBiasPixel_SingleTrack_CMSSW528_RAW/5f752ae4ea383967d47381bbbc967a29/SD_PAZeroBiasPixel_SingleTrack_9_1_1oM.root') #pA
                            #fileNames = cms.untracked.vstring('/store/group/phys_heavyions/icali/PAPhysics/pAPilotRun_Run202792GoodLumis_PAZeroBiasPixel_SingleTrack_CMSSW528_RAW/5f752ae4ea383967d47381bbbc967a29/SD_PAZeroBiasPixel_SingleTrack_9_1_1oM.root')
)

process.options = cms.untracked.PSet(

)


# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.381.2.13 $'),
    annotation = cms.untracked.string('step2 nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)



# Output definition

process.RAWoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
   # outputCommands = cms.untracked.vstring(process.RAWEventContent.outputCommands),
    fileName = cms.untracked.string('step2_L1REPACK.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RAW')
    )
)




# customize the L1 emulator to run customiseL1EmulatorFromRaw with HLT to switchToSimGmtGctGtDigis
import L1Trigger.Configuration.L1Trigger_custom
process = L1Trigger.Configuration.L1Trigger_custom.customiseL1GtEmulatorFromRaw( process )
#process = L1Trigger.Configuration.L1Trigger_custom.customiseResetPrescalesAndMasks( process )

# customize the HLT to use the emulated results
#import HLTrigger.Configuration.customizeHLTforL1Emulator
#process = HLTrigger.Configuration.customizeHLTforL1Emulator.switchToL1Emulator( process )
#process = HLTrigger.Configuration.customizeHLTforL1Emulator.switchToSimGtDigis( process )

# Automatic addition of the customisation function from L1Trigger.Configuration.customise_overwriteL1Menu
#from L1Trigger.Configuration.customise_overwriteL1Menu import customise 

#call to customisation function customise imported from L1Trigger.Configuration.customise_overwriteL1Menu
#process = customise(process) 

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:hltonline_PIon', '')
#process.RAWoutput.outputCommands.extend(['drop FEDRawDataCollection_rawDataCollector__LHC'])

# RCT
#from L1Trigger.RegionalCaloTrigger.rctDigis_cfi import *
# GCT
#from L1Trigger.GlobalCaloTrigger.gctDigis_cfi import *
#process=rctDigis(process)
#process=gctDigis(process)
#sequence = cms.Sequence(rctDigis*gctDigis)
process.L1em_step = cms.Path(process.L1Emulator)

# Path and EndPath definitions
#process.L1simulation_step = cms.Path(process.SimL1Emulator)
#process.digi_step = cms.Path(process.RawToDigi)
#process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWoutput_step = cms.EndPath(process.RAWoutput)

# Schedule definition
#process.schedule = cms.Schedule(process.L1simulation_step,process.digi_step, process.filterActiveBitsNoZB_step,process.endjob_step,process.RAWoutput_step)
#process.schedule = cms.Schedule(process.digi_step,process.endjob_step,process.RAWoutput_step)

process.schedule = cms.Schedule(process.L1em_step, process.RAWoutput_step)
