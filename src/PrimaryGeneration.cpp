// -----------------------------------------------------------------------------
//  G4Basic | PrimaryGeneration.cpp
//
//  Class for the definition of the primary generation action.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "PrimaryGeneration.h"

#include <G4ParticleDefinition.hh>
#include <G4SystemOfUnits.hh>
#include <G4IonTable.hh>
#include <G4PrimaryParticle.hh>
#include <G4PrimaryVertex.hh>
#include <G4Event.hh>
#include <G4Electron.hh>
#include <G4MuonPlus.hh>
#include <G4Proton.hh>

#include <G4GenericMessenger.hh>
#include <Randomize.hh>

#include <stdlib.h>
#include <math.h>

PrimaryGeneration::PrimaryGeneration():
  G4VUserPrimaryGeneratorAction()
{
  msg_ = new G4GenericMessenger(this, "/Inputs/", "Control commands of the ion primary generator.");
  msg_->DeclareProperty("Particle_Type", Particle_Type_,  "which particle?");
  //msg_->DeclareProperty("Particle_energy", Particle_Energy_,  "Energy of the particle.");
}


PrimaryGeneration::~PrimaryGeneration()
{
  delete msg_;
}


void PrimaryGeneration::GeneratePrimaries(G4Event* event)
{
  if (Particle_Type_ ==  "Ar39")
  {
    G4ParticleDefinition* pdef = G4IonTable::GetIonTable()->GetIon(18, 39, 0.); // Ar39
    if (!pdef)G4Exception("SetParticleDefinition()", "[IonGun]",FatalException, " can not create ion ");

    G4PrimaryParticle* particle = new G4PrimaryParticle(pdef);
    particle->SetMomentumDirection(G4ThreeVector(0.,1.,0.));
    particle->SetKineticEnergy(1.*eV); // just an ion sitting

    double Ran_X = G4UniformRand()*(1.0/2)*m;
    double Ran_Y = G4UniformRand()*(1.0/2)*m;
    double Ran_Z = G4UniformRand()*(5.0/2)*m;
    if (G4UniformRand()>0.5){ Ran_X *= -1; }
    if (G4UniformRand()>0.5){ Ran_Y *= -1; }
    if (G4UniformRand()>0.5){ Ran_Z *= -1; }
    G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(Ran_X,Ran_Y,Ran_Z), 0.);
    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);
  }
  else if (Particle_Type_ ==  "Muon")
  {
    double Ran_X = G4UniformRand()*(1.0/2)*m;
    double Ran_Y = G4UniformRand()*(1.0/2)*m;
    double Ran_Z = G4UniformRand()*(5.0/2)*m;
    if (G4UniformRand()>0.5){ Ran_X *= -1; }
    if (G4UniformRand()>0.5){ Ran_Y *= -1; }
    if (G4UniformRand()>0.5){ Ran_Z *= -1; }
    G4ParticleDefinition* pdef = G4MuonPlus::Definition();

    G4PrimaryParticle* particle = new G4PrimaryParticle(pdef);
    particle->SetMomentumDirection(G4ThreeVector(0.,1.,0.));
    particle->SetKineticEnergy(10.*GeV); 

    
    G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(0, -(1.0/2)*m ,Ran_Z), 0.);
    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);
  }
  else if (Particle_Type_ ==  "Proton")
  {

    double Ran_X = G4UniformRand()*(1.0/2)*m;
    double Ran_Y = G4UniformRand()*(1.0/2)*m;
    double Ran_Z = G4UniformRand()*(5.0/2)*m;
    if (G4UniformRand()>0.5){ Ran_X *= -1; }
    if (G4UniformRand()>0.5){ Ran_Y *= -1; }
    if (G4UniformRand()>0.5){ Ran_Z *= -1; }
    double NorXY = sqrt(Ran_X*Ran_X+Ran_Y*Ran_Y) ;

    G4ParticleDefinition* pdef = G4Proton::Definition();

    G4PrimaryParticle* particle = new G4PrimaryParticle(pdef);
    particle->SetMomentumDirection(G4ThreeVector(Ran_X/NorXY , Ran_Y/NorXY, 0.));
    particle->SetKineticEnergy(100.*MeV); // just an ion sitting

    G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(Ran_X,Ran_Y,Ran_Z), 0.);
    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);
  }
  else if (Particle_Type_ ==  "Electron")
  {
    G4ParticleDefinition* pdef = G4Electron::Definition();

    G4PrimaryParticle* particle = new G4PrimaryParticle(pdef);
    particle->SetMomentumDirection(G4ThreeVector(0.,1.,0.));
    particle->SetKineticEnergy(3.*MeV); 

    
    G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(0, 0 ,0.5), 0.);
    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);
  }
  else
  {
    exit (EXIT_FAILURE);
    //G4Exception(FatalException, " Pick a defined particle... ");
  }
  

}
