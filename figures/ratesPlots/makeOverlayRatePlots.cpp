/*******************************************************************/
/* makeOverlayRatesPlots.cpp                                       */
/* ROOT macro                                                      */
/* Usage: root -l -b -q makeOverlayRatesPlots.cpp                  */
/*******************************************************************/

#include "plotNRates.cpp"

TH1F* GetCumulative(TH1F* plot);
float GetThresholdFromFirstBinPassing(TH1F* hRate, float targetRate);
float GetThresholdFromInterpolation(TH1F* hRate, float targetRate);
std::string formatFloat(float f, int decimalPoints);


/*********************************************************************/

TH1F* getRateHistogram(TString variable_name, float xMax, std::string timestamp) {
    TChain* tree = new TChain("L1TrackNtuple/eventTree");
    std::string target = "/eos/user/s/skkwan/globalTrackTrigger/analyzer_MinBias_" + timestamp + "_partial.root";
    tree->Add(target.c_str());

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
  std::string timestamp = "250516_221135";

  gROOT->ProcessLine(".L calculateRates.cpp");

  TString outputDirectory = "/eos/user/s/skkwan/www/globalTrackTrigger/";
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


  TH1F* rate_trkMHT = getRateHistogram("trkMHT", xMax + 20, timestamp);
  TH1F* rate_trkMET = getRateHistogram("trkMET", xMax + 20, timestamp);

  // Get the threshold at which the rate goes below 30 kHz
  float thresh_trkMHT = GetThresholdFromInterpolation(rate_trkMHT, 30);
  float thresh_trkMET = GetThresholdFromInterpolation(rate_trkMET, 30);
  std::cout << ">>> trkMHT has threshold at " << thresh_trkMHT << std::endl;
  std::cout << ">>> trkMET has threshold at " << thresh_trkMET << std::endl;

  // Also write out the thresholds to a text file
  ofstream f1;
  f1.open("trkMHT_threshold_30kHz.txt");
  f1 << thresh_trkMHT;
  f1.close();

  ofstream f2;
  f2.open("trkMET_threshold_30kHz.txt");
  f2 << thresh_trkMET;
  f2.close();
  
  vHists.push_back(rate_trkMHT); vLabels.push_back("TrkMHT, reaches 30 kHz at " + formatFloat(thresh_trkMHT, 1) + " GeV"); vColors.push_back(kRed);
  vHists.push_back(rate_trkMET); vLabels.push_back("TrkMET, reaches 30 kHz at " + formatFloat(thresh_trkMET, 1) + " GeV"); vColors.push_back(kBlack);

  // one more color if necessary: kAzure-9
  plotNRates(vHists, vLabels, vColors,
             xMin, xMax, yMin, yMax,
             "L1 Threshold (GeV)",
             "rates_" + timestamp,
             outputDirectory,
             useLogy);

  vHists.clear();  vLabels.clear();  vColors.clear();

}

/* 
 * Get the cumulative histogram from an input histogram.
 */
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
  
/* 
 * For a rate histogram hRate, return the center of the first bin where the rate drops below targetRate.
 */
float GetThresholdFromFirstBinPassing(TH1F* hRate, float targetRate) {
  float x = 0;
  // Loop over the points 
  for (int i = 0; i < hRate->GetNbinsX(); i++) {
    float yValue = hRate->GetBinContent(i); 
    std::cout << "... checking point " << hRate->GetBinCenter(i) << ", " << yValue << std::endl;
    if (yValue < targetRate) {
      x = hRate->GetBinCenter(i);
      return x;
    }
  }
  std::cout << "[WARNING:] makeOverlayRatePlots.cpp: getThreshold function: reached end without ever finding a point exceeding the threshold " << targetRate << std::endl;
  return x;
}

/*
 * For a rate histogram hRate, return the x-axis value from interpolation where the rate drops below targetRate.
 */
float GetThresholdFromInterpolation(TH1F* hRate, float targetRate) {
  float x_solution = 0;
  // Loop over the points 
  for (int i = 0; i < hRate->GetNbinsX(); i++) {
    float y1 = hRate->GetBinContent(i); 
    float x1 = hRate->GetBinCenter(i);
    // if this is not the last bin, get the next yValue
    if (i < (hRate->GetNbinsX() -1)) {
      float y2 = hRate->GetBinContent(i+1);
      float x2 = hRate->GetBinCenter(i+1);
      std::cout << "... checking bin from " << x1 << " to " << x2 << " with yValue " << y1 << ", " << y2 << std::endl;
    
      // Perform interpolation if targetRate falls in this bin. Note that the rate is always decreasing, so y1 > y2
      if ((targetRate > y2) && (targetRate < y1)) {
        // y = m*x + b
        // slope 
        float m = ((y2 - y1)/(x2 - x1));
        // y-intercept: substitute back into equation for the point (x1, y1)
        float b = (y1 - (m * x1));
        // solve for x where y = targetRate
        x_solution = ((targetRate - b)/m);
      }
    }
  }
  return x_solution;
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
