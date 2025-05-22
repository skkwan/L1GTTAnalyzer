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

std::string formatFloat(float f, int decimalPoints);

/*********************************************************************/

/*
 * Make efficiency plot for one set of settings.
 */

void makeEfficienciesPlotOneScheme(void)  {
  gROOT->ProcessLine(".L calculateEfficiency.cpp");


  TString treePath = "L1TrackNtuple/eventTree";

  TString timestamp = "250516_221442";
  TString rootFileDirectory = "/eos/user/s/skkwan/globalTrackTrigger/analyzer_TT_" + timestamp + ".root";
  TString outputDirectory = "/eos/user/s/skkwan/www/globalTrackTrigger/";
 

  float xMin, xMax;
  TString genCut, l1Cut;
  bool useVariableBinning;

  std::vector<TGraphAsymmErrors*> vGraphs;
  std::vector<TString> vLabels;
  std::vector<int> vColors;
  std::string threshold_str;

  TString outputPlotName;

  outputPlotName = "efficiency_TTbar_" + timestamp;

  /****************************************/
  /* (Plot #1) overlay trkMHT and trkMET  */
  /****************************************/
  vGraphs.clear();  vLabels.clear();  vColors.clear();
  xMin = 0;
  xMax = 510;
  genCut  = "";
  useVariableBinning = false;

  // Get 30 kHz rate thresholds from text files in other directory
  float threshold_trkMHT = 0.0;
  float threshold_trkMET = 0.0;

  std::string str_threshold_trkMHT, str_threshold_trkMET; 
  ifstream f1("../ratesPlots/trkMHT_threshold_30kHz.txt");
  while (getline(f1, str_threshold_trkMHT)) {
    cout << str_threshold_trkMHT;
  }
  threshold_trkMHT = std::stof(str_threshold_trkMHT);
  f1.close();

  ifstream f2("../ratesPlots/trkMET_threshold_30kHz.txt");
  while (getline(f2, str_threshold_trkMET)) {
    cout << str_threshold_trkMET;
  }
  threshold_trkMET = std::stof(str_threshold_trkMET);
  f2.close();

  TGraphAsymmErrors *eff_trkMHT_for_overlay = calculateEfficiency("trueMET", treePath, rootFileDirectory,
              "(trkMHT > " + str_threshold_trkMHT + ")",
              genCut, xMin, xMax, useVariableBinning);          
  vGraphs.push_back(eff_trkMHT_for_overlay);
  vLabels.push_back("trkMHT: reaches 30 kHz at " + str_threshold_trkMHT + " GeV");
  vColors.push_back(kRed);

  TGraphAsymmErrors *eff_trkMET_for_overlay = calculateEfficiency("trueMET", treePath, rootFileDirectory,
    "(trkMET > " + str_threshold_trkMET + ")",
    genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(eff_trkMET_for_overlay);
  vLabels.push_back("trkMET: reaches 30 kHz at " + str_threshold_trkMET + " GeV");
  vColors.push_back(kBlack);

  plotNEfficiencies(vGraphs, vLabels, vColors,
                    "TrueMET / GeV",
                    "Phase-2 GTT",   
                    outputPlotName,                                                             
                    outputDirectory, "L1 threshold determined from 30 kHz rate"); // L1 p_{T} > 25 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02, "Gen p_{T} > 30 GeV");  

}



void makeEfficienciesPlots(void) {

  makeEfficienciesPlotOneScheme();
}

/*
 * Format a float with the given number of decimals after and return it as a string.
 */
std::string formatFloat(float f, int decimalPoints) {
  // Format the threshold with one point after the decimal
  std::ostringstream oss;
  oss << std::setprecision(decimalPoints) << fixed << f;
  std::string formatted_str = oss.str();
  return formatted_str;
}
/*********************************************************************/
