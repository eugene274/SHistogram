//
// Created by eugene on 11/14/18.
//

#ifndef SHISTOGRAM_TESTSHISTOBUILDER_H
#define SHISTOGRAM_TESTSHISTOBUILDER_H

#include <gtest/gtest.h>
#include <SHistoBuilder.h>
#include <TMath.h>
#include <TRandom.h>
#include <TDirectory.h>
#include <TFile.h>

namespace {


TEST(TestSHistoBuilder, FillRandom) {

  SHistoBuilder histoBuilder("test");


  Double_t var_phi = 0;
  Double_t var_pt = 0;
  Double_t var_y = 0;


  auto PHI_AX = histoBuilder.AddAxis("Phi", 100, 0, 2*TMath::Pi(), &var_phi, "\\phi [rad]");
  auto PT_AX = histoBuilder.AddAxis("Pt", 100, 0, 5, &var_pt, "p_{T} [GeV/c]");
  auto Y_AX = histoBuilder.AddAxis("Y", 100, -1, 5, &var_y, "Y");

  TFile ff("Test_SHistoBuilder.root", "recreate");
  ff.cd();

  histoBuilder.AddHistogram(PT_AX);
  auto correlation = histoBuilder.AddCorrelations({{PHI_AX, PT_AX}, {Y_AX, PT_AX}});

  for (int i = 0; i < 1000; ++i) {
    var_phi = 2*TMath::Pi()*gRandom->Rndm();
    var_pt = 5*gRandom->Rndm();
    var_y = 1;
    histoBuilder.Fill();
  }

  auto &hPtVsPhi = histoBuilder[correlation[0]];
  EXPECT_EQ(hPtVsPhi.GetEntries(), 1000.0);

  auto &hPt = histoBuilder[PT_AX];
  EXPECT_EQ(hPt.GetEntries(), 1000.0);

  gDirectory->ls();
  ff.Write();



}



}

#endif //SHISTOGRAM_TESTSHISTOBUILDER_H
