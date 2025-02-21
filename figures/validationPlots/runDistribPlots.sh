# runDistribPlot.sh
# Usage: source runDistribPlot.sh

root -l -b -q 'makeDistributionPlots.C("SF_1.00_cPt-gt-15_genPt-gt-15",                                             \
                                       "TTbar (200 PU)",
                                       "/afs/cern.ch/work/s/skkwan/public/globalTrackTrigger/CMSSW_14_2_0_pre2/src/L1Trigger/L1TGTTAnalyzer/test/events_1000_GTTObjects_ttbar200PU_Phase2Spring24.root", \
                                       "/afs/cern.ch/work/s/skkwan/public/globalTrackTrigger/CMSSW_14_2_0_pre2/src/L1Trigger/L1TGTTAnalyzer/test/", \
                                       "(trkMHTEmu > 5) && (trkMHT > 5)", "",     \
                                       "Trk MHT Emu > 5, Trk MHT > 5")'

