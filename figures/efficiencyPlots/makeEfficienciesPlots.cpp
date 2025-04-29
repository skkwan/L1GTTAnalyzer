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

  TString rootFileDirectory = "/eos/user/s/skkwan/globalTrackTrigger/analyzer_T2tt_Phase2Spring24_default.root";
  TString outputDirectory = ""; // /eos/user/s/skkwan/phase2RCTDevel/figures/efficiencies/";
 

  float xMin, xMax;
  TString genCut, l1Cut;
  bool useVariableBinning;

  std::vector<TGraphAsymmErrors*> vGraphs;
  std::vector<TString> vLabels;
  std::vector<int> vColors;
  std::string threshold_str;

  TString outputPlotName;

  outputPlotName = "efficiency";

  /****************************************/
  /* (Plot #1) overlay trkMHT and trkMET  */
  /****************************************/
  vGraphs.clear();  vLabels.clear();  vColors.clear();
  xMin = 0;
  xMax = 400;
  genCut  = "";
  useVariableBinning = false;

  TGraphAsymmErrors *eff_trkMHT_for_overlay = calculateEfficiency("trueMET", treePath, rootFileDirectory,
              "(trkMHT > 60)",
              genCut, xMin, xMax, useVariableBinning);          
  std::ostringstream oss_mht;
  oss_mht << std::setprecision(0) << fixed << getThreshold(eff_trkMHT_for_overlay, 0.9);
  threshold_str = oss_mht.str();
  vGraphs.push_back(eff_trkMHT_for_overlay);
  vLabels.push_back("trkMHT: reaches 90% eff at " + threshold_str + " GeV");
  vColors.push_back(kBlack);

  TGraphAsymmErrors *eff_trkMET_for_overlay = calculateEfficiency("trueMET", treePath, rootFileDirectory,
    "(trkMET > 60)",
    genCut, xMin, xMax, useVariableBinning);

  std::ostringstream oss_met;
  oss_met << std::setprecision(0) << fixed << getThreshold(eff_trkMET_for_overlay, 0.9);
  threshold_str = oss_met.str();
  vGraphs.push_back(eff_trkMET_for_overlay);
  vLabels.push_back("trkMET: reaches 90% eff at " + threshold_str + " GeV");
  vColors.push_back(kRed);

  plotNEfficiencies(vGraphs, vLabels, vColors,
                    "TrueMET / GeV",
                    "Phase-2 GTT",   
                    outputPlotName + "_T2tt_trkMHT_trkMET_overlay",                                                             
                    outputDirectory, "MVA score > 0.1 for track jets, L1 quantity > 60 GeV"); // L1 p_{T} > 25 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02, "Gen p_{T} > 30 GeV");  

}



void makeEfficienciesPlots(void) {

  makeEfficienciesPlotOneScheme();
}
/*********************************************************************/
