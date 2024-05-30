// -----------------------------------------------------------------------------
//  G4_QPIX | NeutronBackground.H
//
//  Redefining spectrum of photons from neutron capture and distributing them 
//  into the detector.
//   * Author: Gabriela Vitti Stenico
//   * Creation date: 08 Dec 2023
//   * Update date: 23 April 2024
// -----------------------------------------------------------------------------
#ifndef NEUTRON_BACKGROUND_H
#define NEUTRON_BACKGROUND_H

#include "G4VPrimaryGenerator.hh"
#include "globals.hh"

#include <fstream>


class NeutronBackground : public G4VPrimaryGenerator
{
  public:
    explicit NeutronBackground();
    ~NeutronBackground() override = default; 

    void GeneratePrimaryVertex(G4Event*) override;

  private:
  G4int NBverbose;
  std::ifstream inputfile;
  const char* evfile;

};

#endif