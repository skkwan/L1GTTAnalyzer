# runRatesPlots.sh
# Usage: source runRatesPlots.sh

root -l -b -q 'makeRatePlot.cpp("trkMET")'

root -l -b -q 'makeRatePlot.cpp("trkMHT")'