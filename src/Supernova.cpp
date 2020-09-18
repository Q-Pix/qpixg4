// -----------------------------------------------------------------------------
//  Supernova.cpp
//
//  Class definition of Supernova
//   * Author: Everybody is an author!
//   * Creation date: 17 September 2020
// -----------------------------------------------------------------------------

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "Randomize.hh"
#include "G4LogicalVolumeStore.hh"

#include "Supernova.h"

//-----------------------------------------------------------------------------
Supernova::Supernova()
{}

//-----------------------------------------------------------------------------
Supernova::~Supernova()
{}



void Supernova::Gen_test(G4Event* event)
{

    // get detector dimensions
    if (!detector_solid_vol_)
    {
        G4LogicalVolume* detector_logic_vol
        = G4LogicalVolumeStore::GetInstance()->GetVolume("detector.logical");
        if (detector_logic_vol) detector_solid_vol_ = dynamic_cast<G4Box*>(detector_logic_vol->GetSolid());
    }
    G4ParticleDefinition* pdef = G4IonTable::GetIonTable()->GetIon(18, 39, 0.); // Ar39
    if (!pdef)G4Exception("SetParticleDefinition()", "[IonGun]",FatalException, " can not create ion ");

    G4PrimaryParticle* particle = new G4PrimaryParticle(pdef);
    particle->SetMomentumDirection(G4ThreeVector(0.,1.,0.));
    particle->SetKineticEnergy(1.*eV); // just an ion sitting

    double Ran_X = G4UniformRand() * detector_solid_vol_->GetXHalfLength();
    double Ran_Y = G4UniformRand() * detector_solid_vol_->GetYHalfLength();
    double Ran_Z = G4UniformRand() * detector_solid_vol_->GetZHalfLength();
    // if (G4UniformRand()>0.5){ Ran_X *= -1; }
    // if (G4UniformRand()>0.5){ Ran_Y *= -1; }
    // if (G4UniformRand()>0.5){ Ran_Z *= -1; }
    G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(Ran_X,Ran_Y,Ran_Z), 0.);
    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);    
}


