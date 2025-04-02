############################################################
# define basic process
############################################################

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import os

############################################################
# edit options here
############################################################

from Configuration.Eras.Era_Phase2C17I13M9_cff import Phase2C17I13M9

L1TRK_INST ="MyL1TrackJets" ### if not in input DIGRAW then we make them in the above step
process = cms.Process(L1TRK_INST, Phase2C17I13M9)

#L1TRKALGO = 'HYBRID'  #baseline, 4par fit
#L1TRKALGO = 'HYBRID_DISPLACED'  #extended, 5par fit
L1TRKALGO = 'HYBRID_PROMPTANDDISP'

DISPLACED = ''


runVtxNN = True
############################################################
# import standard configurations
############################################################


# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2026D110Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.L1TrackTrigger_cff')
process.load('Configuration.StandardSequences.SimPhase2L1GlobalTriggerEmulator_cff')
process.load('L1Trigger.Configuration.Phase2GTMenus.SeedDefinitions.prototypeSeeds')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic_T33', '')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.INFO.limit = cms.untracked.int32(0) # default: 0

############################################################
# input and output
############################################################

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(5))

readFiles = cms.untracked.vstring(
    # # TTbar
    # '/store/mc/Phase2Spring24DIGIRECOMiniAOD/TT_TuneCP5_14TeV-powheg-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU200_AllTP_140X_mcRun4_realistic_v4-v1/2560000/11d1f6f0-5f03-421e-90c7-b5815197fc85.root'

    # # mStop 1000 mLSP 775. Phase2Spring24 dataset is still on tape
    # /SMS-T2tt_mStop-1000_mLSP-775_TuneCP5_14TeV-madgraphMLM-pythia8/Phase2Spring24DIGIRECOMiniAOD-PU200_Trk1GeV_140X_mcRun4_realistic_v4-v1/GEN-SIM-DIGI-RAW-MINIAOD
    # for now use Phase2Spring23 # 'root://cms-xrd-global.cern.ch//store/mc/Phase2Spring24DIGIRECOMiniAOD/SMS-T2tt_mStop-1000_mLSP-775_TuneCP5_14TeV-madgraphMLM-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU200_Trk1GeV_140X_mcRun4_realistic_v4-v1/2520000/01b76f7e-ce03-4192-90d7-2a2c0f83b10b.root'
    # '/store/mc/Phase2Spring23DIGIRECOMiniAOD/SMS-T2tt_mStop-1000_mLSP-775_TuneCP5_14TeV-madgraphMLM-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU200_L1TFix_131X_mcRun4_realistic_v9-v1/2820000/118886f3-2732-4bbc-a33c-bddaadb0bc89.root'

    # MinBias: /MinBias_TuneCP5_14TeV-pythia8/Phase2Spring24DIGIRECOMiniAOD-PU200ALCA_140X_mcRun4_realistic_v4-v2/GEN-SIM-DIGI-RAW-MINIAOD 
    '/store/mc/Phase2Spring24DIGIRECOMiniAOD/MinBias_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU200ALCA_140X_mcRun4_realistic_v4-v2/120000/004f52f5-5cac-4a87-ab4d-ac7fcea30858.root'
)
secFiles = cms.untracked.vstring()

process.source = cms.Source ("PoolSource",
                            fileNames = readFiles,
                            secondaryFileNames = secFiles,
                            duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
                            )

process.source.inputCommands = cms.untracked.vstring("keep *","drop l1tTkPrimaryVertexs_L1TkPrimaryVertex__*")
process.Timing = cms.Service("Timing",
  summaryOnly = cms.untracked.bool(True),
  useJobReport = cms.untracked.bool(False)
)

# process.TFileService = cms.Service("TFileService", fileName = cms.string('events_100_GTTObjects_ttbar200PU_Phase2Spring24.root'), closeFileFast = cms.untracked.bool(True))
process.TFileService = cms.Service("TFileService", fileName = cms.string('onefile_T2tt_mStop-1000_mLSP-775.root'), closeFileFast = cms.untracked.bool(True))


############################################################
# L1 tracking: remake stubs?
############################################################

process.load('L1Trigger.TrackTrigger.TrackTrigger_cff')
from L1Trigger.TrackTrigger.TTStubAlgorithmRegister_cfi import *
process.load("SimTracker.TrackTriggerAssociation.TrackTriggerAssociator_cff")

from SimTracker.TrackTriggerAssociation.TTClusterAssociation_cfi import *
TTClusterAssociatorFromPixelDigis.digiSimLinks = cms.InputTag("simSiPixelDigis","Tracker")

process.TTClusterStub = cms.Path(process.TrackTriggerClustersStubs)
process.TTClusterStubTruth = cms.Path(process.TrackTriggerAssociatorClustersStubs)


# DTC emulation
process.load('L1Trigger.TrackerDTC.ProducerED_cff')
process.dtc = cms.Path(process.TrackerDTCProducer)

process.load("L1Trigger.TrackFindingTracklet.L1HybridEmulationTracks_cff")
process.load("L1Trigger.L1TTrackMatch.l1tTrackSelectionProducer_cfi")
process.load("L1Trigger.L1TTrackMatch.l1tTrackVertexAssociationProducer_cfi")
process.load("L1Trigger.L1TTrackMatch.l1tTrackJets_cfi")
process.load("L1Trigger.L1TTrackMatch.l1tGTTInputProducer_cfi")
process.load("L1Trigger.L1TTrackMatch.l1tTrackJetsEmulation_cfi")
process.load("L1Trigger.L1TTrackMatch.l1tTrackFastJets_cfi")
process.load("L1Trigger.L1TTrackMatch.l1tTrackerEtMiss_cfi")
process.load("L1Trigger.L1TTrackMatch.l1tTrackerEmuEtMiss_cfi")
process.load("L1Trigger.L1TTrackMatch.l1tTrackerHTMiss_cfi")
process.load("L1Trigger.L1TTrackMatch.l1tTrackerEmuHTMiss_cfi")
process.load("L1Trigger.L1TTrackMatch.l1tTrackTripletEmulation_cfi")
process.load('L1Trigger.VertexFinder.l1tVertexProducer_cfi')
 


############################################################
# Primary vertex
############################################################
process.pPV = cms.Path(process.l1tVertexFinder)


if runVtxNN:
    process.l1tVertexFinderEmulator = process.l1tVertexProducer.clone()
    process.l1tVertexFinderEmulator.VertexReconstruction.Algorithm = "NNEmulation"

    process.l1tTrackSelectionProducer.cutSet = cms.PSet(ptMin = cms.double(2.0), # pt must be greater than this value, [GeV]
                                                        absEtaMax = cms.double(2.4), # absolute value of eta must be less than this value
                                                        absZ0Max = cms.double(15.0), # z0 must be less than this value, [cm]
                                                        nStubsMin = cms.int32(4), # number of stubs must be greater than or equal to this value
                                                        nPSStubsMin = cms.int32(0), # the number of stubs in the PS Modules must be greater than or equal to this value
                                                        promptMVAMin = cms.double(-1.0), # MVA must be greater than this value
                                                        reducedBendChi2Max = cms.double(999.0), # bend chi2 must be less than this value
                                                        reducedChi2RZMax = cms.double(999.0), # chi2rz/dof must be less than this value
                                                        reducedChi2RPhiMax = cms.double(999.0), # chi2rphi/dof must be less than this value
                                                        reducedChi2MaxNstub4 = cms.double(999.9), # chi2/dof with nstub==4 must be less than this value
                                                        reducedChi2MaxNstub5 = cms.double(999.9), # chi2/dof with nstub>4 must be less than this value
                                                        reducedBendChi2MaxNstub4 = cms.double(999.9), # bend chi2 with nstub==4 must be less than this value
                                                        reducedBendChi2MaxNstub5 = cms.double(999.9), # bend chi2 with nstub>4 must be less than this value
                                                        ),
    VertexAssociator = process.l1tTrackVertexNNAssociationProducer
    AssociationName = "l1tTrackVertexNNAssociationProducer"
else:
    VertexAssociator = process.l1tTrackVertexAssociationProducer
    AssociationName = "l1tTrackVertexAssociationProducer"
    
process.pPVemu = cms.Path(process.l1tVertexFinderEmulator)

# HYBRID: prompt tracking
if (L1TRKALGO == 'HYBRID'):
    process.TTTracksEmu = cms.Path(process.L1THybridTracks)
    process.TTTracksEmuWithTruth = cms.Path(process.L1THybridTracksWithAssociators)
    process.pL1TrackSelection = cms.Path(process.l1tTrackSelectionProducer *
                                         process.l1tTrackSelectionProducerForJets *
                                         process.l1tTrackSelectionProducerForEtMiss)
    process.pL1TrackVertexAssociation = cms.Path(VertexAssociator *
                                                 process.l1tTrackVertexAssociationProducerForJets*
                                                 process.l1tTrackVertexAssociationProducerForEtMiss)
    process.pL1TrackJets = cms.Path(process.l1tTrackJets)
    process.pL1TrackFastJets=cms.Path(process.l1tTrackFastJets)
    process.pL1GTTInput = cms.Path(process.l1tGTTInputProducer)
    process.pL1TrackJetsEmu = cms.Path(process.l1tTrackJetsEmulation)
    process.pTkMET = cms.Path(process.l1tTrackerEtMiss)
    process.pTkMETEmu = cms.Path(process.l1tTrackerEmuEtMiss)
    process.pTkMHT = cms.Path(process.l1tTrackerHTMiss)
    process.pTkMHTEmulator = cms.Path(process.l1tTrackerEmuHTMiss)
    process.pL1TrackTripletEmulator = cms.Path(process.l1tTrackTripletEmulation)
    DISPLACED = 'Prompt'

# HYBRID: extended tracking
elif (L1TRKALGO == 'HYBRID_DISPLACED'):
    process.TTTracksEmu = cms.Path(process.L1TExtendedHybridTracks)
    process.TTTracksEmuWithTruth = cms.Path(process.L1TExtendedHybridTracksWithAssociators)
    process.pL1TrackSelection = cms.Path(process.l1tTrackSelectionProducer *
                                         process.l1tTrackSelectionProducerExtended *
                                         process.l1tTrackSelectionProducerExtendedForJets *
                                         process.l1tTrackSelectionProducerExtendedForEtMiss)
    process.pL1TrackVertexAssociation = cms.Path(process.l1tTrackVertexAssociationProducerExtended *
                                                 process.l1tTrackVertexAssociationProducerExtendedForJets *
                                                 process.l1tTrackVertexAssociationProducerExtendedForEtMiss)
    process.pL1TrackJets = cms.Path(process.l1tTrackJetsExtended)
    process.pL1TrackFastJets = cms.Path(process.l1tTrackFastJetsExtended)
    process.pL1GTTInput = cms.Path(process.l1tGTTInputProducerExtended)
    process.pL1TrackJetsEmu = cms.Path(process.l1tTrackJetsExtendedEmulation)
    process.pTkMET = cms.Path(process.l1tTrackerEtMissExtended)
    process.pTkMHT = cms.Path(process.l1tTrackerHTMissExtended)
    process.pTkMHTEmulator = cms.Path(process.l1tTrackerEmuHTMissExtended)
    DISPLACED = 'Displaced'#

# HYBRID: extended tracking
elif (L1TRKALGO == 'HYBRID_PROMPTANDDISP'):
    process.TTTracksEmu = cms.Path(process.L1TPromptExtendedHybridTracks)
    process.TTTracksEmuWithTruth = cms.Path(process.L1TPromptExtendedHybridTracksWithAssociators)
    process.pL1TrackSelection = cms.Path(process.l1tTrackSelectionProducer * process.l1tTrackSelectionProducerExtended *
                                         process.l1tTrackSelectionProducerForJets * process.l1tTrackSelectionProducerExtendedForJets *
                                         process.l1tTrackSelectionProducerForEtMiss * process.l1tTrackSelectionProducerExtendedForEtMiss)
    process.pL1TrackVertexAssociation = cms.Path(VertexAssociator * process.l1tTrackVertexAssociationProducerExtended *
                                                 process.l1tTrackVertexAssociationProducerForJets * process.l1tTrackVertexAssociationProducerExtendedForJets *
                                                 process.l1tTrackVertexAssociationProducerForEtMiss * process.l1tTrackVertexAssociationProducerExtendedForEtMiss)
    process.pL1TrackJets = cms.Path(process.l1tTrackJets*process.l1tTrackJetsExtended)
    process.pL1TrackFastJets = cms.Path(process.l1tTrackFastJets*process.l1tTrackFastJetsExtended)
    process.pL1GTTInput = cms.Path(process.l1tGTTInputProducer*process.l1tGTTInputProducerExtended)
    process.pL1TrackJetsEmu = cms.Path(process.l1tTrackJetsEmulation*process.l1tTrackJetsExtendedEmulation)
    process.pTkMET = cms.Path(process.l1tTrackerEtMiss*process.l1tTrackerEtMissExtended)
    process.pTkMETEmu = cms.Path(process.l1tTrackerEmuEtMiss)
    process.pTkMHT = cms.Path(process.l1tTrackerHTMiss*process.l1tTrackerHTMissExtended)
    process.pTkMHTEmulator = cms.Path(process.l1tTrackerEmuHTMiss*process.l1tTrackerEmuHTMissExtended)
    process.pL1TrackTripletEmulator = cms.Path(process.l1tTrackTripletEmulation)
    DISPLACED = 'Both'




############################################################
# Define the track ntuple process, MyProcess is the (unsigned) PDGID corresponding to the process which is run
# e.g. single electron/positron = 11
#      single pion+/pion- = 211
#      single muon+/muon- = 13
#      pions in jets = 6
#      taus = 15
#      all TPs = 1
############################################################

process.L1TrackNtuple = cms.EDAnalyzer('L1TrackObjectNtupleMaker',
        MyProcess = cms.int32(1),
        DebugMode = cms.bool(False),      # printout lots of debug statements
        SaveAllTracks = cms.bool(True),  # save *all* L1 tracks, not just truth matched to primary particle
        SaveStubs = cms.bool(False),      # save some info for *all* stubs
        Displaced = cms.string(DISPLACED),# "Prompt", "Displaced", "Both"
        L1Tk_minNStub = cms.int32(4),     # L1 tracks with >= 4 stubs
        TP_minNStub = cms.int32(4),       # require TP to have >= X number of stubs associated with it
        TP_minNStubLayer = cms.int32(4),  # require TP to have stubs in >= X layers/disks
        TP_minPt = cms.double(2.0),       # only save TPs with pt > X GeV
        TP_maxEta = cms.double(2.5),      # only save TPs with |eta| < X
        TP_maxZ0 = cms.double(15.0),      # only save TPs with |z0| < X cm
        L1TrackInputTag = cms.InputTag("l1tTTTracksFromTrackletEmulation", "Level1TTTracks"),                                                      # TTTracks, prompt
        L1TrackExtendedInputTag = cms.InputTag("l1tTTTracksFromExtendedTrackletEmulation", "Level1TTTracks"),                                      # TTTracks, extended
        MCTruthTrackInputTag = cms.InputTag("TTTrackAssociatorFromPixelDigis", "Level1TTTracks"),                                               # MCTruth track, prompt
        MCTruthTrackExtendedInputTag = cms.InputTag("TTTrackAssociatorFromPixelDigisExtended", "Level1TTTracks"),                               # MCTruth track, extended
        L1TrackGTTInputTag = cms.InputTag("l1tGTTInputProducer","Level1TTTracksConverted"),                                                      # TTTracks, prompt, GTT converted
        L1TrackExtendedGTTInputTag = cms.InputTag("l1tGTTInputProducerExtended","Level1TTTracksExtendedConverted"),                              # TTTracks, extended, GTT converted
        L1TrackSelectedInputTag = cms.InputTag("l1tTrackSelectionProducer", "Level1TTTracksSelected"),                                           # TTTracks, prompt, selected
        L1TrackSelectedEmulationInputTag = cms.InputTag("l1tTrackSelectionProducer", "Level1TTTracksSelectedEmulation"),                         # TTTracks, prompt, emulation, selected
        L1TrackSelectedAssociatedInputTag = cms.InputTag(AssociationName, "Level1TTTracksSelectedAssociated"),                                           # TTTracks, prompt, selected, associated
        L1TrackSelectedAssociatedEmulationInputTag = cms.InputTag(AssociationName, "Level1TTTracksSelectedAssociatedEmulation"),                         # TTTracks, prompt, emulation, selected, associated

        L1TrackSelectedForJetsInputTag = cms.InputTag("l1tTrackSelectionProducerForJets", "Level1TTTracksSelected"),                                           # TTTracks, prompt, selected
        L1TrackSelectedEmulationForJetsInputTag = cms.InputTag("l1tTrackSelectionProducerForJets", "Level1TTTracksSelectedEmulation"),                         # TTTracks, prompt, emulation, selected
        L1TrackSelectedAssociatedForJetsInputTag = cms.InputTag("l1tTrackVertexAssociationProducerForJets", "Level1TTTracksSelectedAssociated"),                                           # TTTracks, prompt, selected, associated
        L1TrackSelectedAssociatedEmulationForJetsInputTag = cms.InputTag("l1tTrackVertexAssociationProducerForJets", "Level1TTTracksSelectedAssociatedEmulation"),                         # TTTracks, prompt, emulation, selected, associated
                                       
        L1TrackSelectedForEtMissInputTag = cms.InputTag("l1tTrackSelectionProducerForEtMiss", "Level1TTTracksSelected"),                                           # TTTracks, prompt, selected
        L1TrackSelectedEmulationForEtMissInputTag = cms.InputTag("l1tTrackSelectionProducerForEtMiss", "Level1TTTracksSelectedEmulation"),                         # TTTracks, prompt, emulation, selected
        L1TrackSelectedAssociatedForEtMissInputTag = cms.InputTag("l1tTrackVertexAssociationProducerForEtMiss", "Level1TTTracksSelectedAssociated"),                                           # TTTracks, prompt, selected, associated
        L1TrackSelectedAssociatedEmulationForEtMissInputTag = cms.InputTag("l1tTrackVertexAssociationProducerForEtMiss", "Level1TTTracksSelectedAssociatedEmulation"),                         # TTTracks, prompt, emulation, selected, associated

        L1TrackExtendedSelectedInputTag = cms.InputTag("l1tTrackSelectionProducerExtended", "Level1TTTracksExtendedSelected"),                                           # TTTracks, extended, selected
        L1TrackExtendedSelectedEmulationInputTag = cms.InputTag("l1tTrackSelectionProducerExtended", "Level1TTTracksExtendedSelectedEmulation"),                         # TTTracks, extended, emulation, selected
        L1TrackExtendedSelectedAssociatedInputTag = cms.InputTag("l1tTrackVertexAssociationProducerExtended", "Level1TTTracksExtendedSelectedAssociated"),                                           # TTTracks, extended, selected, associated
        L1TrackExtendedSelectedAssociatedEmulationInputTag = cms.InputTag("l1tTrackVertexAssociationProducerExtended", "Level1TTTracksExtendedSelectedAssociatedEmulation"),                         # TTTracks, extended, emulation, selected, associated
                                       
        L1TrackExtendedSelectedForJetsInputTag = cms.InputTag("l1tTrackSelectionProducerExtendedForJets", "Level1TTTracksExtendedSelected"),                                           # TTTracks, extended, selected
        L1TrackExtendedSelectedEmulationForJetsInputTag = cms.InputTag("l1tTrackSelectionProducerExtendedForJets", "Level1TTTracksExtendedSelectedEmulation"),                         # TTTracks, extended, emulation, selected
        L1TrackExtendedSelectedAssociatedForJetsInputTag = cms.InputTag("l1tTrackVertexAssociationProducerExtendedForJets", "Level1TTTracksExtendedSelectedAssociated"),                                           # TTTracks, extended, selected, associated
        L1TrackExtendedSelectedAssociatedEmulationForJetsInputTag = cms.InputTag("l1tTrackVertexAssociationProducerExtendedForJets", "Level1TTTracksExtendedSelectedAssociatedEmulation"),                         # TTTracks, extended, emulation, selected, associated
                                       
        L1TrackExtendedSelectedForEtMissInputTag = cms.InputTag("l1tTrackSelectionProducerExtendedForEtMiss", "Level1TTTracksExtendedSelected"),                                           # TTTracks, extended, selected
        L1TrackExtendedSelectedEmulationForEtMissInputTag = cms.InputTag("l1tTrackSelectionProducerExtendedForEtMiss", "Level1TTTracksExtendedSelectedEmulation"),                         # TTTracks, extended, emulation, selected
        L1TrackExtendedSelectedAssociatedForEtMissInputTag = cms.InputTag("l1tTrackVertexAssociationProducerExtendedForEtMiss", "Level1TTTracksExtendedSelectedAssociated"),                                           # TTTracks, extended, selected, associated
        L1TrackExtendedSelectedAssociatedEmulationForEtMissInputTag = cms.InputTag("l1tTrackVertexAssociationProducerExtendedForEtMiss", "Level1TTTracksExtendedSelectedAssociatedEmulation"),                         # TTTracks, extended, emulation, selected, associated
                                                                              
        L1StubInputTag = cms.InputTag("TTStubsFromPhase2TrackerDigis","StubAccepted"),
        MCTruthClusterInputTag = cms.InputTag("TTClusterAssociatorFromPixelDigis", "ClusterAccepted"),
        MCTruthStubInputTag = cms.InputTag("TTStubAssociatorFromPixelDigis", "StubAccepted"),
        TrackingParticleInputTag = cms.InputTag("mix", "MergedTrackTruth"),
        TrackingVertexInputTag = cms.InputTag("mix", "MergedTrackTruth"),
        GenJetInputTag = cms.InputTag("ak4GenJets", ""),
        ##track jets and track MET
        SaveTrackJets = cms.bool(True), #includes emulated jets
        SaveTrackSums = cms.bool(True), #includes simulated/emulated track MET, MHT, and HT
        TrackFastJetsInputTag = cms.InputTag("l1tTrackFastJets","L1TrackFastJets"),
        TrackFastJetsExtendedInputTag = cms.InputTag("l1tTrackFastJetsExtended","L1TrackFastJetsExtended"),
        TrackJetsInputTag=cms.InputTag("l1tTrackJets", "L1TrackJets"),
        TrackTripletsInputTag = cms.InputTag("l1tTrackTripletEmulation", "L1TrackTriplet"),
        TrackJetsExtendedInputTag=cms.InputTag("l1tTrackJetsExtended", "L1TrackJetsExtended"),
        TrackJetsEmuInputTag = cms.InputTag("l1tTrackJetsEmulation","L1TrackJets"),
        TrackJetsExtendedEmuInputTag = cms.InputTag("l1tTrackJetsExtendedEmulation","L1TrackJetsExtended"),
        TrackMETInputTag = cms.InputTag("l1tTrackerEtMiss","L1TrackerEtMiss"),
        TrackMETExtendedInputTag = cms.InputTag("l1tTrackerEtMissExtended","L1TrackerExtendedEtMiss"),
        TrackMETEmuInputTag = cms.InputTag("l1tTrackerEmuEtMiss","L1TrackerEmuEtMiss"),
        TrackMHTInputTag = cms.InputTag("l1tTrackerHTMiss","L1TrackerHTMiss"), #includes HT
        TrackMHTExtendedInputTag = cms.InputTag("l1tTrackerHTMissExtended","L1TrackerHTMissExtended"),
        TrackMHTEmuInputTag = cms.InputTag("l1tTrackerEmuHTMiss",process.l1tTrackerEmuHTMiss.L1MHTCollectionName.value()),
        TrackMHTEmuExtendedInputTag = cms.InputTag("l1tTrackerEmuHTMissExtended",process.l1tTrackerEmuHTMissExtended.L1MHTCollectionName.value()),
        SimVertexInputTag = cms.InputTag("g4SimHits",""),
        GenParticleInputTag = cms.InputTag("genParticles",""),
        RecoVertexInputTag=cms.InputTag("l1tVertexFinder", "L1Vertices"),
        RecoVertexEmuInputTag=cms.InputTag("l1tVertexFinderEmulator", "L1VerticesEmulation"),
)

process.ntuple = cms.Path(process.L1TrackNtuple)

process.out = cms.OutputModule( "PoolOutputModule",
 #                               outputCommands = process.RAWSIMEventContent.outputCommands,
                                outputCommands = cms.untracked.vstring("keep *","drop *_*_*_HLT"),
                                fileName = cms.untracked.string("test.root" )
		               )
#process.out.outputCommands.append('keep  *_*_*_*')
#process.out.outputCommands.append('drop  l1tEMTFHits_*_*_*')
process.pOut = cms.EndPath(process.out)


# use this if you want to re-run the stub making
# process.schedule = cms.Schedule(process.TTClusterStub,process.TTClusterStubTruth,process.TTTracksEmuWithTruth,process.ntuple)

# use this if cluster/stub associators not available
# process.schedule = cms.Schedule(process.TTClusterStubTruth,process.TTTracksEmuWithTruth,process.ntuple)

process.schedule = cms.Schedule(process.TTClusterStub, process.TTClusterStubTruth, process.dtc, process.TTTracksEmuWithTruth, process.pL1GTTInput, process.pL1TrackSelection, process.pPV, process.pPVemu,process.pL1TrackVertexAssociation, process.pL1TrackJets, process.pL1TrackJetsEmu,process.pL1TrackFastJets, process.pTkMET, process.pTkMETEmu, process.pTkMHT, process.pTkMHTEmulator,process.pL1TrackTripletEmulator, process.ntuple)
