// -----------------------------------------------------------------------------
//  MARLEYManager.cpp
//
//
//   * Author: Everybody is an author!
//   * Creation date: 7 August 2020
// -----------------------------------------------------------------------------

#include "MARLEYManager.h"

// MARLEY includes
#include "marley/JSONConfig.hh"

// Geant includes
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4Event.hh"

// Q-Pix includes
#include "GeneratorParticle.h"
#include "MCTruthManager.h"




MARLEYManager * MARLEYManager::instance_ = 0;

//-----------------------------------------------------------------------------
MARLEYManager::MARLEYManager()
{
}

//-----------------------------------------------------------------------------
MARLEYManager::~MARLEYManager()
{
}

//-----------------------------------------------------------------------------
MARLEYManager * MARLEYManager::Instance()
{
    if (instance_ == 0) instance_ = new MARLEYManager();
    return instance_;
}

//-----------------------------------------------------------------------------
marley::Generator & MARLEYManager::Generator()
{
    return marley_generator_;
}

//-----------------------------------------------------------------------------
void MARLEYManager::Initialize(std::string marley_json)
{
    if (!marley_json.empty())
    {
        std::cout << "Configuring MARLEY..." << std::endl;
        marley::JSONConfig marley_config(marley_json);
        marley_generator_ = marley_config.create_generator();
    }
    else
    {
        std::cout << "MARLEY configuration file not found!  Continuing..."
                  << std::endl;
    }
}



//-----------------------------------------------------------------------------
void MARLEYManager::Get_Detector_Dimensions(double detector_x_, double detector_y_, double detector_z_)
{
    detector_length_x_ = detector_x_;
    detector_length_y_ = detector_y_;
    detector_length_z_ = detector_z_;
}

void MARLEYManager::Light_Em_Up(G4Event* event)
{
    // get MC truth manager
    MCTruthManager * mc_truth_manager = MCTruthManager::Instance();
    
    // get MARLEY manager and generator
    MARLEYManager * marley_manager = MARLEYManager::Instance();
    marley::Generator & marley_generator = marley_manager->Generator();

    // G4PrimaryVertex* vertex = new G4PrimaryVertex(G4ThreeVector(0., 0., 0.), 0.);

    G4ThreeVector offset(detector_length_x_/2.,
                         detector_length_y_/2.,
                         detector_length_z_/2.);

    G4PrimaryVertex* vertex = new G4PrimaryVertex(offset, 0.);

    // Generate a new MARLEY event using the owned marley::Generator object
    marley::Event ev = marley_generator.create_event();

    // // print MARLEY event information
    // ev.print_human_readable(G4cout);

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



