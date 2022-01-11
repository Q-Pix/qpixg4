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

    G4GenericMessenger* msg_; // Messenger for configuration parameters
    G4String Particle_Type_;
    bool PrintParticleInfo_;
    //double Particle_Energy_;

    bool decay_at_time_zero_;
    bool isotropic_;
    bool override_vertex_position_;
    double vertex_x_;
    double vertex_y_;
    double vertex_z_;

    G4GeneralParticleSource * particle_gun_;

    SupernovaTiming * supernova_timing_;

    Supernova * super;

    double detector_length_x_;
    double detector_length_y_;
    double detector_length_z_;

    G4Box* detector_solid_vol_;
    std::string Particle_infoRoot_Path;

    void MARLEYGeneratePrimaries(G4Event*);
    void GENIEGeneratePrimaries(G4Event*);


    std::default_random_engine generator_;
    std::normal_distribution< double > distribution_;

};

#endif
