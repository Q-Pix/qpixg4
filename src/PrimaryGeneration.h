// -----------------------------------------------------------------------------
//  G4_QPIX | PrimaryGeneration.h
//
//  Class for the definition of the primary generation action.
//   * Author: Everybody is an author!
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#ifndef PRIMARY_GENERATION_H
#define PRIMARY_GENERATION_H

// GEANT4 includes
#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4Box.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4String.hh"

// Q-Pix includes
#include "Supernova.h"
#include "SupernovaTiming.h"
#include "NeutronBackground.h"

// ROOT includes
#include "Math/SVector.h"
#include "Math/SMatrix.h" 

// C++ includes
#include <random>

class G4ParticleDefinition;
class G4GenericMessenger;

class PrimaryGeneration : public G4VUserPrimaryGeneratorAction
{

  public:

    PrimaryGeneration();
    virtual ~PrimaryGeneration();
    virtual void GeneratePrimaries(G4Event*);

  protected:

    // GEANT4 dictionary of particles
    G4ParticleTable* particle_table_;

  private:

    G4GeneralParticleSource * particle_gun_;

    SupernovaTiming * supernova_timing_;

    Supernova * super;

    static G4VPrimaryGenerator* neutron;

    double detector_length_x_;
    double detector_length_y_;
    double detector_length_z_;

    G4Box* detector_solid_vol_;

    void MARLEYGeneratePrimaries(G4Event*);
    void GENIEGeneratePrimaries(G4Event*);

    std::default_random_engine generator_;
    std::normal_distribution< double > distribution_;

    ROOT::Math::SMatrix< double, 3 > Rotation_Matrix(G4ThreeVector, G4ThreeVector);
};

#endif
