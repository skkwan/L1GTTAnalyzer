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

  TString rootFileDirectory = "/afs/cern.ch/work/s/skkwan/public/globalTrackTrigger/CMSSW_14_2_0_pre2/src/L1Trigger/L1TGTTAnalyzer/test/events_1000_GTTObjects_ttbar200PU_Phase2Spring24.root";
  TString outputDirectory = ""; // /eos/user/s/skkwan/phase2RCTDevel/figures/efficiencies/";
 

  float xMin, xMax;
  TString genCut, l1Cut;
  bool useVariableBinning;

  std::vector<TGraphAsymmErrors*> vGraphs;
  std::vector<TString> vLabels;
  std::vector<int> vColors;

  TString outputPlotName;

  outputPlotName = "efficiency";

  /**********************************************************************************************************/
  /* (Plot #1) eff. as a function of gen pT, standalone WP, GCT cluster pT > 25 GeV, genPt > 30 GeV         */
  /**********************************************************************************************************/

  vGraphs.clear();  vLabels.clear();  vColors.clear();
  xMin = 0;
  xMax = 150;
  l1Cut   = "(trkMHTEmu > 40) && (trueMET > 40)";
  genCut  = "(trueMET > 40)";
  useVariableBinning = false;

  TGraphAsymmErrors *eff = calculateEfficiency("trueMET", treePath, rootFileDirectory,
              l1Cut,
              genCut, xMin, xMax, useVariableBinning);
  vGraphs.push_back(eff);
  vLabels.push_back("TTbar");
  vColors.push_back(kBlack);

  plotNEfficiencies(vGraphs, vLabels, vColors,
                    "TrueMET / GeV",
                    "Phase-2 GTT Emulators",   
                    outputPlotName + "_TrueMET",                                                             
                    outputDirectory, "TrkMHTEmu > 40 GeV && TrueMET > 40 GeV"); // L1 p_{T} > 25 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02, "Gen p_{T} > 30 GeV");  

  // // next
  // vGraphs.clear();  vLabels.clear();  vColors.clear();
  // xMin = 0;
  // xMax = 120;
  // genCut  = "(trkMHTEmu > 30)";
  // l1Cut   = "(trkMET > 40)";
  // useVariableBinning = true;

  // eff = calculateEfficiency("trueMET", treePath, rootFileDirectory,
  //             l1Cut,
  //             genCut, xMin, xMax, useVariableBinning);
  // vGraphs.push_back(eff);
  // vLabels.push_back("TTbar");
  // vColors.push_back(kBlack);

  // plotNEfficiencies(vGraphs, vLabels, vColors,
  //                   "TrkMET / GeV",
  //                   "Phase-2 GTT Emulators",   
  //                   outputPlotName + "_trkMET",                                                             
  //                   outputDirectory, "trkMET > 40 GeV && trkMHTEmu > 30 GeV"); // L1 p_{T} > 25 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02, "Gen p_{T} > 30 GeV");  
                  

//   /***********************************************************************************/
//   /* (Plot #2) efficiency as a function of genEta: GCT > 25 GeV pT, gen pT > 30 GeV  */
//   /***********************************************************************************/

//   vGraphs.clear();  vLabels.clear();  vColors.clear();
//   xMin = -1.5;
//   xMax = 1.5;

//   genCut  = "(abs(genEta) < 1.4841) && (genPt > 30)";
//   l1Cut   = "(abs(genEta) < 1.4841) && (gct_cPt > 25) && (genPt > 30)";
//   useVariableBinning = false;

//   TGraphAsymmErrors *allEta = calculateEfficiency("genEta", treePath, rootFileDirectory,
//               l1Cut,
//               genCut, xMin, xMax, useVariableBinning);
//   vGraphs.push_back(allEta);
//   vLabels.push_back("TDR emulator w/o WP");
//   vColors.push_back(kGray);

//   TGraphAsymmErrors *tightEta = calculateEfficiency("genEta", treePath, rootFileDirectory,
//                                                   l1Cut + redCutString,
//                                                   genCut, xMin, xMax, useVariableBinning);
//   vGraphs.push_back(tightEta);
//   vLabels.push_back("TDR emulator with standalone WP");
//   vColors.push_back(kBlue);

//   TGraphAsymmErrors *allEta2 = calculateEfficiency("genEta", treePath2, rootFileDirectory2,
//                                                    l1Cut,
//                                                    genCut, xMin, xMax, useVariableBinning);
//   vGraphs.push_back(allEta2);
//   vLabels.push_back("Phase 2 emulator w/o WP");
//   vColors.push_back(kBlack);

//   TGraphAsymmErrors *tightEta2 = calculateEfficiency("genEta", treePath2, rootFileDirectory2,
//                                                    l1Cut + redCutString,
//                                                    genCut, xMin, xMax, useVariableBinning);
//   vGraphs.push_back(tightEta2);
//   vLabels.push_back("Phase 2 emulator with standalone WP");
//   vColors.push_back(kRed); 

//   plotNEfficiencies(vGraphs, vLabels, vColors,
//                     "Gen Electron #eta",
//                     "Emulators",                                                                
//                     "efficiency_genEta_barrel_l1Ptgt25GeV_genPtgt30GeV",
//                     outputDirectory, "L1 p_{T} > 25 GeV, |#eta^{Gen}| < 1.4841", 0.0, 1.02, "Gen p_{T} > 30 GeV");

}



void makeEfficienciesPlots(void) {

  makeEfficienciesPlotOneScheme();
}
/*********************************************************************/
