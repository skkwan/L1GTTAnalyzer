/*******************************************************************/
/* plotRates.cpp                                                   */
/* Author: Stephanie Kwan                                          */
/*******************************************************************/

#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TH1F.h"

#include "TAxis.h"
#include "TChain.h"

#include "THStack.h"
#include "TLegend.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TTree.h"

#include <assert.h>
#include <cmath>
#include <iostream>
#include <math.h>
#include <string>
#include <TMath.h>
#include <vector>

#include "../baseCodeForPlots/tdrstyle.C"
#include "../baseCodeForPlots/comparisonPlots.C"

#ifndef PLOTRATES_CPP_INCL
#define PLOTRATES_CPP_INCL

/*******************************************************************/

void plotNRates(std::vector<TH1F*> hists, 
                std::vector<TString> labels,
                std::vector<int> colors,
                float xMin, float xMax,
                float yMin, float yMax,
                TString xAxisLabel,
                TString outputName,
                TString outputDir,
                bool useLogy
                ) {
  assert((hists.size() == labels.size()) && (hists.size() == colors.size()));

  setTDRStyle();
  TCanvas* Tcan = new TCanvas("Tcan","", 100, 20, 1000, 1000);
//  TLegend* leg = new TLegend(0.55,0.15,0.90,0.45); // bottom right corner
  TLegend* leg = new TLegend(0.45,0.65,0.90,0.95);
  applySmallerLegStyle(leg);

  Tcan->SetGrid();

  TLatex *latex = new TLatex(); 
  latex->SetNDC();
  latex->SetTextFont(42);
  latex->SetTextColor(kBlack);

  Tcan->cd();     /* Set current canvas */
  Tcan->SetFillColor(0);

  if (useLogy) gPad->SetLogy();


  // Make canvas larger so y-axis label won't run off the side
  Tcan->SetLeftMargin(0.18);

  std::vector<TH1F*>::iterator itHist;
  std::vector<TString>::iterator itLabel;
  std::vector<int>::iterator itColor;
  
  TH1F* histDummy;
  for (itHist = hists.begin(), itLabel = labels.begin(), itColor = colors.begin();
       itHist != hists.end();
       itHist++, itLabel++, itColor++)
    {
      if (itHist == hists.begin()) // only do this once 
        {
          histDummy = new TH1F(**itHist);
        }
      
      // De-reference the iterator to get the TH1F*
      (*itHist)->SetLineWidth(2);
      (*itHist)->SetLineColor(*itColor);
      (*itHist)->SetMarkerSize(0.7);
      (*itHist)->SetMarkerStyle(20);
      (*itHist)->SetMarkerColor(*itColor);
    }

  gStyle->SetEndErrorSize(3);

//  histDummy->Draw("E1");

  for (itHist = hists.begin(); itHist != hists.end(); itHist++)
    {
      (*itHist)->GetXaxis()->SetRangeUser(xMin, xMax);
      (*itHist)->GetYaxis()->SetRangeUser(yMin, yMax);
      (*itHist)->GetXaxis()->SetTitle(xAxisLabel);
      (*itHist)->GetXaxis()->SetNdivisions(520);  // 20 primary divisions, 5 secondary divisions: https://root.cern.ch/doc/master/classTH1.html#a5d3b67a639d33ecb34a64fdb539000bb 
      (*itHist)->GetYaxis()->SetTitle("Rate (kHz)");
      (*itHist)->GetXaxis()->SetTitleSize(0.06); // default is 0.03                                                                    
      (*itHist)->GetYaxis()->SetTitleSize(0.06); // default is 0.03 
      (*itHist)->GetYaxis()->SetMaxDigits(6); // Suppress scientific notation on y-axis because it clashes with the header
      (*itHist)->Draw("E1 SAME");
    }


  // draw horizontal line to represent 30 kHz
  TLine* line_rate = new TLine(0, 30, xMax, 30);
  line_rate->SetLineColor(kBlack);
  line_rate->SetLineWidth(1);
  line_rate->SetLineStyle(9);
  line_rate->Draw("SAME");

  // histDummy->GetYaxis()->SetNoExponent(kFALSE);
  /* Customize legend */
  //  leg->SetHeader(legendName); 
  for (itHist = hists.begin(), itLabel = labels.begin();
       itHist != hists.end();
       itHist++, itLabel++) {
    
    leg->AddEntry(*itHist, "#scale[0.6]{" + *itLabel + "}",  "l");
  }
  leg->Draw();


  // Default to RCT label, use GCT if not
  TString emuLabel = "#scale[1.0]{#bf{CMS}} #scale[0.6]{#it{GTT Emulator}}";  
  //TString emuLabel = "#scale[1.0]{#bf{CMS}} #scale[0.6]{#it{TDR Emulator}}";  

  latex->DrawLatex(0.17, 0.960, emuLabel); 
  latex->DrawLatex(0.75, 0.960, "#scale[0.6]{200 PU, MinBias}"); 

  float commentaryXpos = 0.48;
  latex->DrawLatex(commentaryXpos, 0.9, "#scale[0.7]{Phase-2 L1 Global Track Trigger}");
  //  latex->DrawLatex(commentaryXpos, 0.790, "#scale[0.8]{Phase 2 HLT TDR Winter20}");

  Tcan->Update();


  Tcan->cd();
  Tcan->SaveAs(outputDir+outputName+".pdf");
  Tcan->SaveAs(outputDir+outputName+".png");

}
             

/*******************************************************************/

#endif
