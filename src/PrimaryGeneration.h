// -----------------------------------------------------------------------------
//  G4Basic | PrimaryGeneration.h
//
//  Class for the definition of the primary generation action.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#ifndef PRIMARY_GENERATION_H
#define PRIMARY_GENERATION_H

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4String.hh>
class G4ParticleDefinition;
class G4GenericMessenger;

// MARLEY includes
#include "marley/Generator.hh"

class PrimaryGeneration: public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneration();
  virtual ~PrimaryGeneration();
  virtual void GeneratePrimaries(G4Event*);
  
  protected:
    // MARLEY event generator object
    marley::Generator marley_generator_;

private:
  G4GenericMessenger* msg_; // Messenger for configuration parameters
  G4String Particle_Type_;
  G4String MARLEY_json_;
  //double Particle_Energy_;

};

#endif
