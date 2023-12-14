// -----------------------------------------------------------------------------
//  G4_QPIX | NeutronBackground.H
//
//  Redefining spectrum of photons from neutron capture and distributing them 
//  into the detector.
//   * Author: Gabriela Vitti Stenico
//   * Creation date: 08 Dec 2023
// -----------------------------------------------------------------------------

#include "G4VPrimaryGenerator.hh"
#include "globals.hh"

#include <iostream>
#include <fstream>

class G4Event;

class NeutronBackground : public G4VPrimaryGenerator
{
  public:
    NeutronBackground();
    ~NeutronBackground();
    std::ifstream myfile;

  public:
    virtual void GeneratePrimaryVertex(G4Event*);
};
