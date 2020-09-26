// -----------------------------------------------------------------------------
//  sample_time.c
//
//
//   * Author: Everybody is an author!
//   * Creation date: 25 September 2020
// -----------------------------------------------------------------------------

// C++ includes
#include <iostream>
#include <string>
#include <vector>

// ROOT includes
#include "TFile.h"
#include "TROOT.h"

//------------------------------------------------------------
// user-defined variables
//------------------------------------------------------------
double const energy = -0.021;  // MeV
// double const energy = 20.02;  // MeV

//------------------------------------------------------------
// input file
//------------------------------------------------------------
std::string const input_file = "/n/holystore01/LABS/guenette_lab/Users/jh/files/supernova/nusperbin2d.root";

//----------------------------------------------------------------------
// main function
//----------------------------------------------------------------------
int main()
{

  //----------------------------------------------------------
  // open ROOT file for reading
  //----------------------------------------------------------
  TFile * tfile = new TFile(input_file.data(), "read");

  // get time vs. energy histogram
  TH2D * th2 = (TH2D*) tfile->Get("nusperbin2d_nue");

  // project histogram onto energy axis
  TH1D * th1_energy = th2->ProjectionY();

  // get energy bin
  int const bin_idx = th1_energy->FindBin(energy);

  // get time distribution of energy bin
  TH1D * th1_time = th2->ProjectionX("time", bin_idx, bin_idx+1);
  unsigned int const n = (unsigned int) th1_time->Integral();

  // sample time
  // double const time = th1_time->GetRandom();  // sec

  //----------------------------------------------------------
  // sample from time distribution
  //----------------------------------------------------------
  TH1F * h = new TH1F("h", "", 9020, -0.0205, 8.9995);
  h->SetXTitle("time [s]");
  h->SetYTitle("entries per ms");

  // set random seed
  gRandom->SetSeed(0);

  for (unsigned int idx = 0; idx < n; ++idx)
  {
    h->Fill(th1_time->GetRandom());  // sec
  }

  //----------------------------------------------------------
  // draw
  //----------------------------------------------------------
  h->Draw();
  th1_time->Draw("same");
  // th2->Draw("colz");

} // main()

//----------------------------------------------------------------------
// run, forrest, run!
//----------------------------------------------------------------------
void sample_time()
{
  main();
} // sample_time()

