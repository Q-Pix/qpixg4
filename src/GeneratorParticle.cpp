// -----------------------------------------------------------------------------
//  GeneratorParticle.cpp
//
//  Class definition of GeneratorParticle
//   * Author: Everybody is an author!
//   * Creation date: 4 September 2020
// -----------------------------------------------------------------------------

#include "globals.hh"
#include "GeneratorParticle.h"

#include "G4SystemOfUnits.hh"

//-----------------------------------------------------------------------------
GeneratorParticle::GeneratorParticle()
{}

//-----------------------------------------------------------------------------
GeneratorParticle::~GeneratorParticle()
{}

void GeneratorParticle::Print() const
{
  G4cout << "PDG:      " << pdg_code_ << G4endl
         << "Mass:     " << mass_/CLHEP::GeV << "GeV" << G4endl
         << "Charge:   " << charge_/CLHEP::eplus << G4endl
         << "X:        " << x_/CLHEP::mm << "mm" << G4endl
         << "Y:        " << y_/CLHEP::mm << "mm" << G4endl
         << "Z:        " << z_/CLHEP::mm << "mm" << G4endl;
         << "T:        " << t_/CLHEP::ns << "ns" << G4endl
         << "Px:       " << px_/CLHEP::GeV << "GeV" << G4endl
         << "Py:       " << py_/CLHEP::GeV << "GeV" << G4endl
         << "Pz:       " << pz_/CLHEP::GeV << "GeV" << G4endl
         << "E:       " << energy_/CLHEP::GeV << "GeV" << G4endl;

}
