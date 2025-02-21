# L1GTTAnalyzer

This code builds on the n-tuplizer in CMSSW: `L1Trigger/L1TTrackMatch/test/L1TrackObjectNtupleMaker_cfg.py` and produces plots from this n-tuplizer.

## Setup

The latest CMSSW version to use is taken from the [SWGuideL1TPhase2Instructions TWiki](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TPhase2Instructions#Running_With_Track_Trigger).
```bash
cmsrel CMSSW_14_2_0_pre2
cd CMSSW_14_2_0_pre2/src
git cms-init
git cms-addpkg L1Trigger/L1TTrackMatch
# then clone this repository
scram b -j 8
```

## Development notes
Use the `cmsDriver` command which includes the Global Track Trigger to generate a current config file, and then copy in the relevant parts from `${CMSSW_BASE}/src/L1Trigger/L1TTrackMatch/test/L1TrackObjectNtupleMaker_cfg.py`. The result is [test/test-analyzer.py](test/test-analyzer.py) which works in `CMSSW_14_2_0_pre2` for a file from the current MC campaign:

```
/store/mc/Phase2Spring24DIGIRECOMiniAOD/TT_TuneCP5_14TeV-powheg-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU200_AllTP_140X_mcRun4_realistic_v4-v1/2560000/11d1f6f0-5f03-421e-90c7-b5815197fc85.root
```

## Making plots

### Distributions 

```bash
cd ${CMSSW_BASE}/src/L1TGTTAnalyzer/figures/validationPlots
bash runDistribPlots.sh
```

### Efficiencies

```bash
cd ${CMSSW_BASE}/src/L1TGTTAnalyzer/figures/efficiencyPlots
bash runEfficienciesPlots.sh
```
