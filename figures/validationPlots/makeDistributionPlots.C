// Usage: root -l -b -q runDistributionPlots.C
// Create plots of variables from a TTree

#include "../baseCodeForPlots/singleDistribution.cpp"
#include "../baseCodeForPlots/scatterPlot.cpp"

// sampleName: name of folder                      (e.g. "RelValElectronGunPt2To100_NoPU")
// legend: description at top of plot, LaTeX OK    (e.g. "RelVal Electron Gun Pt 2 to 100, No PU"")
// inputDirectory: path to input root file         (e.g. "analyzer.root")
// cut: cut applied to all plots                   (e.g. "pt > 0")
// descriptor: first descriptin in plot, LaTeX OK  (e.g. "L1 p_{T} > 0" )
// descriptor, bonusDescriptor: second line of description in plot, LaTeX OK 
void makeDistributionPlots(TString sampleName, TString legend, TString inputDirectory,
                           TString outputDirectory,
                           TString cut = "", 
                           TString descriptor = "",
                           TString bonusDescriptor = "")
{
  // Load the macro
  //  gROOT->ProcessLine(".L ../baseCodeForPlots/comparisonPlots.C");
 
  std::cout << sampleName << " " << legend << " " << inputDirectory << std::endl;
  TString treePath = "L1TrackNtuple/eventTree";
  gSystem->Exec("mkdir -p " + outputDirectory);

  int nBins = 51;

  float ymaxDummyValue = -99;
  float ymaxPtDiff = 0.35;
  float ymaxDeltaR = 0.1;
  float ymaxPt = 0.06;
  float ymaxPhi = 0.05;

  descriptor = ("" + descriptor);  

  // L1 GTT quantities
  singleDistributionPlots("trkMHT",  "trkMHT",  cut, legend, treePath, inputDirectory, outputDirectory, "TrkMHT", descriptor, bonusDescriptor, nBins, 0, 200, ymaxDummyValue);
  singleDistributionPlots("trkMHTEmu",  "trkMHTEmu",  cut, legend, treePath, inputDirectory, outputDirectory, "TrkMHTEmu", descriptor, bonusDescriptor, nBins, 0, 200, ymaxDummyValue);

  // singleDistributionPlots("gct_deltaR", "gct_deltaR", cut + "", legend, treePath, inputDirectory, outputDirectory, "GCT #DeltaR (L1, Gen)", descriptor, bonusDescriptor, nBins, 0, 0.05, ymaxDeltaR);
  // singleDistributionPlots("gct_pT_resolution", "((gct_cPt/" + SF + ") - genPt)/(genPt)", cut + "", legend, treePath, inputDirectory, outputDirectory,
  //                        "GCT (p_{T}^{reco} - p_{T}^{gen})/p_{T}^{gen}", descriptor, bonusDescriptor, nBins, -0.5, 0.5, ymaxPtDiff);


}
