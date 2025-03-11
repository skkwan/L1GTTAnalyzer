//To find the rate from a (very very large) min bias sample,
//Runs off of the track object ntuplizer

#include "TROOT.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TMath.h"
#include <TError.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;
TH1D* GetCumulative(TH1D* plot);

void makeRatePlot(TString variable_name = "trkMHT") {
  TChain* tree = new TChain("L1TrackNtuple/eventTree");
  tree->Add("/afs/cern.ch/work/s/skkwan/public/globalTrackTrigger/CMSSW_14_2_0_pre2/src/L1Trigger/L1TGTTAnalyzer/test/onefile_MinBias_pathological.root");

  if (tree->GetEntries() == 0) {
    cout << "File doesn't exist or is empty, returning..." << endl;
    return;
  }

  // define leafs & branches
  float this_value = 0;
  TBranch* b_this_variable;

  tree->SetBranchAddress(variable_name, &this_value, &b_this_variable);

  float numBins = 100.0;

  TH1D* histReco = new TH1D("reco", "reco; " + variable_name + " (GeV); L1 Rate (kHz)", numBins, 0, numBins);  //using tracks

  int nevt = tree->GetEntries();
  cout << "number of events = " << nevt << endl;

  // event loop
  for (int i = 0; i < nevt; i++) {
    tree->GetEntry(i);
    if (i % 10000 == 0)
      std::cout << i << "/" << nevt << std::endl;

      histReco->Fill(this_value);
  }  // end event loop

  // -------------------------------------------------------------------------------------------
  // Get rate histogram
  histReco->SetStats(0);
  histReco->SetLineColor(kRed);
  histReco->SetTitle("");
  TH1D* fCumulative = GetCumulative(histReco);
  fCumulative->Scale(4.0e3 / fCumulative->GetBinContent(1));

  //  fCumulative->SetTitle("MinBias Events PU 200; " + variable_name + " [GeV]; L1 Rate [kHz]");
  fCumulative->SetMarkerSize(0.7);
  fCumulative->SetMarkerStyle(20);
  fCumulative->SetMarkerColor(fCumulative->GetLineColor());
  fCumulative->GetYaxis()->SetRangeUser(1E-01, 1E05);

  // Initialize legend 
  TLegend* leg = new TLegend(0.48, 0.68, 0.88, 0.88);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.04);
  leg->SetTextFont(42);
  leg->AddEntry(histReco, variable_name, "l");



  // Create the canvas 
  TCanvas* can = new TCanvas("can", "can", 100, 20, 1000, 1000);
  can->SetGridx();
  can->SetGridy();
  can->SetLogy();
  can->SetLeftMargin(0.18);

  // Draw the objects
  fCumulative->Draw();
  leg->Draw("SAME");

  // draw horizontal line to represent 35 kHz
  TLine* line_rate = new TLine(0, 35, numBins, 35);
  line_rate->SetLineColor(kBlack);
  line_rate->SetLineWidth(2);
  line_rate->Draw("SAME");

  std::cout << variable_name << "threshold at 35kHz "
            << fCumulative->GetBinLowEdge(fCumulative->FindLastBinAbove(35)) << std::endl;

  // add decorator text
  TLatex *latex = new TLatex(); 
  latex->SetNDC();
  latex->SetTextFont(42);
  latex->SetTextColor(kBlack);
  TString emuLabel = "#scale[1.0]{#bf{CMS}} #scale[0.6]{#it{Phase-2 GTT}}";  
  latex->DrawLatex(0.10, 0.920, emuLabel);  
  latex->DrawLatex(0.50, 0.920, "#scale[0.6]{200 PU MinBias (Phase2Spring24)}"); 

    
  can->SaveAs(variable_name + "_minBias_RatePlot.root");
  can->SaveAs(variable_name + "_minBias_RatePlot.png");
  can->SaveAs(variable_name + "_minBias_RatePlot.pdf");
}

TH1D* GetCumulative(TH1D* plot) {
  std::string newName = Form("cumulative_%s", plot->GetName());
  TH1D* temp = (TH1D*)plot->Clone(newName.c_str());
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
