/*******************************************************************/
/* makeEfficienciesPlotDPnote.cpp                                  */
/* ROOT macro                                                      */
/* Usage: root -l -b -q makeEfficienciesPlotDPNote.cpp             */
/*        Uncomment code blocks labeled "Plot #1", #2,...          */
/*        at a time                                                */
/*******************************************************************/

#include "efficiencyHist.cpp"
#include "calculateEfficiency.cpp"

#include "../parametricCurve/cutoffValues.h"
#include "../parametricCurve/fillTH2D.h"
#include "../parametricCurve/parametricFit.h"

#include <string>
/*********************************************************************/

/*
 * Make efficiency plot for one set of settings.
 */

void makeEfficienciesPlotOneScheme(void)  {
  gROOT->ProcessLine(".L calculateEfficiency.cpp");


  TString treePath = "L1TrackNtuple/eventTree";

  TString rootFileDirectory = "/eos/user/s/skkwan/globalTrackTrigger/analyzer_T2tt_Phase2Spring24_mvaCut0p6.root";
  TString outputDirectory = ""; // /eos/user/s/skkwan/phase2RCTDevel/figures/efficiencies/";
 

  float xMin, xMax;
  TString genCut, l1Cut;
  bool useVariableBinning;

  std::vector<TGraphAsymmErrors*> vGraphs;
  std::vector<TString> vLabels;
  std::vector<int> vColors;

  TString outputPlotName;

  outputPlotName = "efficiency";

  /****************************************/
  /* (Plot #1) overlay trkMHT and trkMET  */
  /****************************************/
  vGraphs.clear();  vLabels.clear();  vColors.clear();
  xMin = 0;
  xMax = 200;
  genCut  = "";
  useVariableBinning = false;

  TGraphAsymmErrors *eff_trkMHT_for_overlay = calculateEfficiency("trueMET", treePath, rootFileDirectory,
              "(trkMHT > 60)",
              genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(eff_trkMHT_for_overlay);
  vLabels.push_back("trkMHT (cut on trkMHT > 60 GeV)");
  vColors.push_back(kBlack);

  TGraphAsymmErrors *eff_trkMET_for_overlay = calculateEfficiency("trueMET", treePath, rootFileDirectory,
    "(trkMET > 60)",
    genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(eff_trkMET_for_overlay);
  vLabels.push_back("trkMET (cut on trkMET > 60 GeV)");
  vColors.push_back(kRed);

  plotNEfficiencies(vGraphs, vLabels, vColors,
                    "TrueMET / GeV",
                    "Phase-2 GTT",   
                    outputPlotName + "_T2tt_trkMHT_trkMET_overlay",                                                             
                    outputDirectory, "L1 quantity > 60 GeV"); // L1 p_{T} > 25 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02, "Gen p_{T} > 30 GeV");  

}



void makeEfficienciesPlots(void) {

  makeEfficienciesPlotOneScheme();
}
/*********************************************************************/
