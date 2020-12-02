// -----------------------------------------------------------------------------
//  G4Basic | PrimaryGeneration.cpp
//
//  Class for the definition of the primary generation action.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "PrimaryGeneration.h"

// Q-Pix includes
#include "MARLEYManager.h"
#include "MCTruthManager.h"
#include "GeneratorParticle.h"

// MARLEY includes
#include "marley/Event.hh"
#include "marley/Particle.hh"

// GEANT4 includes
#include "G4PhysicalConstants.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4Event.hh"

#include "G4Electron.hh"
#include "G4MuonPlus.hh"
#include "G4Proton.hh"


#include "G4GenericMessenger.hh"
#include "Randomize.hh"

// C++ includes
#include <stdlib.h>
#include <math.h>

PrimaryGeneration::PrimaryGeneration()
  : G4VUserPrimaryGeneratorAction(),
    decay_at_time_zero_(false),
    particle_gun_(0)
{
  msg_ = new G4GenericMessenger(this, "/Inputs/", "Control commands of the ion primary generator.");
  msg_->DeclareProperty("Particle_Type", Particle_Type_,  "which particle?");
  msg_->DeclareProperty("decay_at_time_zero", decay_at_time_zero_,
                        "Set to true to make unstable isotopes decay at t=0.");

  particle_gun_ = new G4GeneralParticleSource();

  // get dictionary of particles
  particle_table_ = G4ParticleTable::GetParticleTable();

  // get supernova timing class
  supernova_timing_ = new SupernovaTiming();

  super = new Supernova();
}


PrimaryGeneration::~PrimaryGeneration()
{
  delete msg_;
  delete particle_gun_;
  delete supernova_timing_;
  delete super;
}


void PrimaryGeneration::GeneratePrimaries(G4Event* event)
{
  // get MC truth manager
  MCTruthManager * mc_truth_manager = MCTruthManager::Instance();

  // get detector dimensions
  if (!detector_solid_vol_)
  {
    G4LogicalVolume* detector_logic_vol
      = G4LogicalVolumeStore::GetInstance()->GetVolume("detector.logical");
    if (detector_logic_vol) detector_solid_vol_ = dynamic_cast<G4Box*>(detector_logic_vol->GetSolid());
  }
  if (detector_solid_vol_)
  {
    detector_length_x_ = detector_solid_vol_->GetXHalfLength() * 2.;
    detector_length_y_ = detector_solid_vol_->GetYHalfLength() * 2.;
    detector_length_z_ = detector_solid_vol_->GetZHalfLength() * 2.;
  }

  if (Particle_Type_ ==  "soup")
  {
    super->Get_Detector_Dimensions(detector_length_x_, detector_length_y_, detector_length_z_);
    super->Gen_test(event);
  }

  else if (Particle_Type_ ==  "MARLEY")
  {
    this->MARLEYGeneratePrimaries(event);
  }

  else
  {
    particle_gun_->GeneratePrimaryVertex(event);

    // get generated particle
    G4ParticleDefinition * const particle_definition = particle_gun_->GetParticleDefinition();

    int const pdg_code = particle_definition->GetPDGEncoding();
    double const charge = particle_definition->GetPDGCharge();
    double const mass = particle_definition->GetPDGMass() * CLHEP::MeV;

    // if the particle is a nucleus
    if (pdg_code > 1000000000)
    {
      if (decay_at_time_zero_ && !(particle_definition->GetPDGStable()))
      {
          // make unstable isotopes decay at t=0
          particle_definition->SetPDGLifeTime(1.*CLHEP::ps);
      }
    } // if the particle is a nucleus

    G4ThreeVector const & position = particle_gun_->GetParticlePosition();
    G4ThreeVector const & direction = particle_gun_->GetParticleMomentumDirection();

    double const x = position.x() / CLHEP::cm;
    double const y = position.y() / CLHEP::cm;
    double const z = position.z() / CLHEP::cm;
    double const t = particle_gun_->GetParticleTime();  // ns

    double const dx = direction.x();
    double const dy = direction.y();
    double const dz = direction.z();

    double const kinetic_energy = particle_gun_->GetParticleEnergy() * CLHEP::MeV;
    double const energy = kinetic_energy + mass;

    double const momentum = std::sqrt(energy*energy - mass*mass);

    double const px = momentum * dx;
    double const py = momentum * dy;
    double const pz = momentum * dz;

    // create generator particle
    GeneratorParticle * particle = new GeneratorParticle();
    particle->SetPDGCode (pdg_code);
    particle->SetMass    (mass    );
    particle->SetCharge  (charge  );
    particle->SetX       (x       );
    particle->SetY       (y       );
    particle->SetZ       (z       );
    particle->SetT       (t       );
    particle->SetPx      (px      );
    particle->SetPy      (py      );
    particle->SetPz      (pz      );
    particle->SetEnergy  (energy  );

    // add to MC truth manager
    mc_truth_manager->AddInitialGeneratorParticle(particle);
    mc_truth_manager->AddFinalGeneratorParticle(particle);
  }

}


void PrimaryGeneration::MARLEYGeneratePrimaries(G4Event* event)
{
  // get MC truth manager
  MCTruthManager * mc_truth_manager = MCTruthManager::Instance();

  // get MARLEY manager and generator
  MARLEYManager * marley_manager = MARLEYManager::Instance();
  marley::Generator & marley_generator = marley_manager->Generator();

  // G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(0., 0., 0.), 0.);

  // // get detector dimensions
  // if (!detector_solid_vol_)
  // {
  //   G4LogicalVolume* detector_logic_vol
  //     = G4LogicalVolumeStore::GetInstance()->GetVolume("detector.logical");
  //   if (detector_logic_vol) detector_solid_vol_ = dynamic_cast<G4Box*>(detector_logic_vol->GetSolid());
  // }
  // if (detector_solid_vol_)
  // {
  //   detector_length_x_ = detector_solid_vol_->GetXHalfLength() * 2.;
  //   detector_length_y_ = detector_solid_vol_->GetYHalfLength() * 2.;
  //   detector_length_z_ = detector_solid_vol_->GetZHalfLength() * 2.;
  //   // G4cout << "det. dim.: " << detector_length_x_ << " m × "
  //   //                         << detector_length_y_ << " m × "
  //   //                         << detector_length_z_ << " m"
  //   //        << G4endl;
  // }

  // G4ThreeVector offset(detector_length_x_/2.,
  //                      detector_length_y_/2.,
  //                      detector_length_z_/2.);

  // add padding so that electron tracks don't get truncated near the
  // boundaries of an APA drift volume
  double const padding = 20 * CLHEP::cm;

  G4ThreeVector offset(G4UniformRand() * (detector_length_x_-2*padding) + padding,
                       G4UniformRand() * (detector_length_y_-2*padding) + padding,
                       G4UniformRand() * (detector_length_z_-2*padding) + padding);

  // G4PrimaryVertex* vertex = new G4PrimaryVertex(offset, 0.);

  // Generate a new MARLEY event using the owned marley::Generator object
  marley::Event ev = marley_generator.create_event();

  // // print MARLEY event information
  // ev.print_human_readable(G4cout);

  // get neutrino energy
  double neutrinoEnergy = 0.;  // MeV
  for (const auto& p : ev.get_initial_particles())
  {
    if (std::abs(p->pdg_code()) == 12 or
        std::abs(p->pdg_code()) == 14 or
        std::abs(p->pdg_code()) == 16 or
        std::abs(p->pdg_code()) == 18)
    {
      neutrinoEnergy = p->total_energy();  // MeV
    }
  }

  // timing
  double time = 0.;  // ns
  if (!supernova_timing_->Initialized())
  {
    supernova_timing_->Initialize();
  }
  if (supernova_timing_->Status())
  {
    time = supernova_timing_->Sample(neutrinoEnergy);  // s
    time *= 1e9; // ns
    // std::cout << "time [ns]: " << time << std::endl;
  }

  // initialize vertex
  G4PrimaryVertex* vertex = new G4PrimaryVertex(offset, time);

  // std::cout << "vertex->GetT0() [ns]: " << vertex->GetT0() << std::endl;

  // Loop over each of the initial particles in the MARLEY event
  for (const auto& ip : ev.get_initial_particles())
  {
    // create generator particle
    GeneratorParticle * generatorParticle = new GeneratorParticle();
    generatorParticle->SetPDGCode (ip->pdg_code());
    generatorParticle->SetMass    (ip->mass());
    generatorParticle->SetCharge  (ip->charge());
    generatorParticle->SetX       (vertex->GetX0() / CLHEP::cm);
    generatorParticle->SetY       (vertex->GetY0() / CLHEP::cm);
    generatorParticle->SetZ       (vertex->GetZ0() / CLHEP::cm);
    generatorParticle->SetT       (vertex->GetT0());
    generatorParticle->SetPx      (ip->px());
    generatorParticle->SetPy      (ip->py());
    generatorParticle->SetPz      (ip->pz());
    generatorParticle->SetEnergy  (ip->total_energy());

    // add initial MARLEY particle to the MC truth manager
    mc_truth_manager->AddInitialGeneratorParticle(generatorParticle);
  }

  // Loop over each of the final particles in the MARLEY event
  for (const auto& fp : ev.get_final_particles())
  {
    // create generator particle
    GeneratorParticle * generatorParticle = new GeneratorParticle();
    generatorParticle->SetPDGCode (fp->pdg_code());
    generatorParticle->SetMass    (fp->mass());
    generatorParticle->SetCharge  (fp->charge());
    generatorParticle->SetX       (vertex->GetX0() / CLHEP::cm);
    generatorParticle->SetY       (vertex->GetY0() / CLHEP::cm);
    generatorParticle->SetZ       (vertex->GetZ0() / CLHEP::cm);
    generatorParticle->SetT       (vertex->GetT0());
    generatorParticle->SetPx      (fp->px());
    generatorParticle->SetPy      (fp->py());
    generatorParticle->SetPz      (fp->pz());
    generatorParticle->SetEnergy  (fp->total_energy());

    // add final MARLEY particle to the MC truth manager
    mc_truth_manager->AddFinalGeneratorParticle(generatorParticle);

    // Convert each one from a marley::Particle into a G4PrimaryParticle.
    // Do this by first setting the PDG code and the 4-momentum components.

    // get dictionary of particles if necessary
    if (particle_table_ == 0)
    {
      particle_table_ = G4ParticleTable::GetParticleTable();
    }

    // initialize particle definition
    G4ParticleDefinition* pdef;

    // get PDG code of marley::Particle
    int const pdg_code = fp->pdg_code();

    if (pdg_code == 0)
    {
      pdef = particle_table_->FindParticle("opticalphoton");
    }
    else
    {
      pdef = particle_table_->FindParticle(pdg_code);
    }

    // if the particle is a nucleus
    if (pdg_code > 1000000000)
    {
      if (!pdef)
      {
        int const Z = (pdg_code % 10000000) / 10000; // atomic number
        int const A = (pdg_code % 10000) / 10; // mass number
        pdef = particle_table_->GetIonTable()->GetIon(Z, A, 0.);
      }
    } // if the particle is a nucleus

    if (pdef == 0)
    {
      std::string message = "\nLine "
                          + std::to_string(__LINE__)
                          + " of file "
                          + __FILE__
                          + "\n\nUnknown PDG code: "
                          + std::to_string(pdg_code)
                          + "\n";
      G4Exception("PrimaryGeneration::GeneratePrimaries", "Error",
                  FatalException, message.c_str());
    }

    G4PrimaryParticle* particle = new G4PrimaryParticle(pdef,
                                                        fp->px(),
                                                        fp->py(),
                                                        fp->pz());

    // Also set the charge of the G4PrimaryParticle appropriately
    particle->SetCharge( fp->charge() );

    // particle->SetPolarization(); ??

    // Add the fully-initialized G4PrimaryParticle to the primary vertex
    vertex->SetPrimary( particle );
  }

  // The primary vertex has been fully populated with all final-state particles
  // from the MARLEY event. Add it to the G4Event object so that Geant4 can
  // begin tracking the particles through the simulated geometry.
  event->AddPrimaryVertex( vertex );
}
