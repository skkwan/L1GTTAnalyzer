/*******************************************************************/
/* makeOverlayRatesPlots.cpp                                       */
/* ROOT macro                                                      */
/* Usage: root -l -b -q makeOverlayRatesPlots.cpp                  */
/*******************************************************************/

#include "calculateRates.cpp"
#include "plotNRates.cpp"

TH1F* GetCumulative(TH1F* plot);

/*********************************************************************/

TH1F* getRateHistogram(TString variable_name, float xMax) {
    TChain* tree = new TChain("L1TrackNtuple/eventTree");
    tree->Add("/eos/user/s/skkwan/globalTrackTrigger/analyzer_MinBias_0p6MVAcut_250421_232041_partial.root");

    if (tree->GetEntries() == 0) {
        cout << "File doesn't exist or is empty, returning..." << endl;
        return NULL;
    }

    // define leafs & branches
    float this_value = 0;
    TBranch* b_this_variable;

    tree->SetBranchAddress(variable_name, &this_value, &b_this_variable);

    float numBins = 40;

    TH1F* histReco = new TH1F("reco", "reco; " + variable_name + " (GeV); L1 Rate (kHz)", numBins, 0, xMax);  //using tracks

    int nevt = tree->GetEntries();
    cout << "number of events = " << nevt << endl;

    // event loop
    for (int i = 0; i < nevt; i++) {
        tree->GetEntry(i);
        if (i % 10000 == 0)
        std::cout << i << "/" << nevt << std::endl;

        histReco->Fill(this_value);
    }  // end event loop

    TH1F* fCumulative = GetCumulative(histReco);
    fCumulative->Scale(30e3 / fCumulative->GetBinContent(1));
    return fCumulative;
}

/*********************************************************************/

/* 
 * Create rates plots.
 */

void makeOverlayRatePlots(void)
{
  gROOT->ProcessLine(".L calculateRates.cpp");

  TString outputDirectory = "";
  // Input file and output directory

  float xMin, xMax;
  float yMin, yMax;
  std::vector<TH1F*> vHists;
  std::vector<TString> vLabels;
  std::vector<int> vColors;
  bool useLogy;

  vHists.clear();  vLabels.clear();  vColors.clear();

  // Rates as a function of l1Pt    
  xMin = 0;
  xMax = 100.0;
  yMin = 1.0;
  yMax = 10e4;
  useLogy = true;


  TH1F* rate_trkMHT = getRateHistogram("trkMHT", xMax + 20);
  TH1F* rate_trkMET = getRateHistogram("trkMET", xMax + 20);


  vHists.push_back(rate_trkMHT); vLabels.push_back("TrkMHT, MVA > 0.6"); vColors.push_back(kBlack);
  vHists.push_back(rate_trkMET); vLabels.push_back("TrkMET"); vColors.push_back(kRed);

  // one more color if necessary: kAzure-9
  plotNRates(vHists, vLabels, vColors,
             xMin, xMax, yMin, yMax,
             "L1 Threshold (GeV)",
             "rates_overlay_mva_0p6",
             outputDirectory,
             useLogy);

  vHists.clear();  vLabels.clear();  vColors.clear();

}

TH1F* GetCumulative(TH1F* plot) {
    std::string newName = Form("cumulative_%s", plot->GetName());
    TH1F* temp = (TH1F*)plot->Clone(newName.c_str());
    temp->SetDirectory(0);
    for (int i = 0; i < plot->GetNbinsX() + 1; i++) {
      double content(0.0), error2(0.0);
      for (int j = i; j < plot->GetNbinsX() + 1; j++) {
        content += plot->GetBinContent(j);
        error2 += plot->GetBinError(j) * plot->GetBinError(j);
      }
      temp->SetBinContent(i, content);
      temp->SetBinError(i, TMath::Sqrt(error2));
    }
    return temp;
  }
  