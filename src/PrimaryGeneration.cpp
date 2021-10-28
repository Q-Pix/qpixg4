// -----------------------------------------------------------------------------
//  G4_QPIX | PrimaryGeneration.cpp
//
//  Class for the definition of the primary generation action.
//   * Author: Everybody is an author!
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

// ROOT includes
#include "Math/SVector.h"
#include "Math/SMatrix.h"

// C++ includes
#include <chrono>
#include <math.h>
#include <stdlib.h>

PrimaryGeneration::PrimaryGeneration()
  : G4VUserPrimaryGeneratorAction(),
    decay_at_time_zero_(false),
    isotropic_(false),
    override_vertex_position_(false),
    vertex_x_(2.3/2),
    vertex_y_(6.0/2),
    vertex_z_(3.6/2),
    particle_gun_(0)
{
  msg_ = new G4GenericMessenger(this, "/Inputs/", "Control commands of the ion primary generator.");
  msg_->DeclareProperty("Particle_Type", Particle_Type_,  "which particle?");
  msg_->DeclareProperty("decay_at_time_zero", decay_at_time_zero_,
                        "Set to true to make unstable isotopes decay at t=0.");
  msg_->DeclareProperty("isotropic", isotropic_, "isotropic");
  msg_->DeclareProperty("override_vertex_position",
                        override_vertex_position_,
                        "override vertex position");
  msg_->DeclareProperty("vertex_x", vertex_x_, "vertex x").SetUnit("mm");;
  msg_->DeclareProperty("vertex_y", vertex_y_, "vertex y").SetUnit("mm");;
  msg_->DeclareProperty("vertex_z", vertex_z_, "vertex z").SetUnit("mm");;

  particle_gun_ = new G4GeneralParticleSource();

  // get dictionary of particles
  particle_table_ = G4ParticleTable::GetParticleTable();

  // get supernova timing class
  supernova_timing_ = new SupernovaTiming();

  super = new Supernova();

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  // std::default_random_engine generator(seed);
  generator_ = std::default_random_engine(seed);
  distribution_ = std::normal_distribution< double >(0, 1);
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

  if (Particle_Type_ ==  "SUPERNOVA")
  {
    super->Get_Detector_Dimensions(detector_length_x_, detector_length_y_, detector_length_z_);
    super->Gen_Supernova_Background(event);
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

  if (override_vertex_position_)
  {
    offset = G4ThreeVector(vertex_x_, vertex_y_, vertex_z_);
  }

  // G4PrimaryVertex* vertex = new G4PrimaryVertex(offset, 0.);

  // Generate a new MARLEY event using the owned marley::Generator object
  marley::Event ev = marley_generator.create_event();

  // // print MARLEY event information
  // ev.print_human_readable(G4cout);

  double p_array[3] = { 0, 0, 0 };

  // get neutrino energy and momentum
  double neutrino_energy = 0.;  // MeV
  for (const auto& p : ev.get_initial_particles())
  {
    if (std::abs(p->pdg_code()) == 12 or
        std::abs(p->pdg_code()) == 14 or
        std::abs(p->pdg_code()) == 16 or
        std::abs(p->pdg_code()) == 18)
    {
      neutrino_energy = p->total_energy();  // MeV
      p_array[0] = p->px();                 // MeV
      p_array[1] = p->py();                 // MeV
      p_array[2] = p->pz();                 // MeV
    }
  }

  //---------------------------------------------------------------------------
  // Isotropic sample
  // ================
  // To generate an isotropic sample of MARLEY events, we must generate some
  // random vector b and compute the rotation matrix R that would rotate the
  // incident neutrino three-momentum a to align with b.  We then use R to
  // rotate the 3-momenta of all the particles in the event.
  //---------------------------------------------------------------------------

  double identity_array[9] = { 1, 0, 0,
                               0, 1, 0,
                               0, 0, 1  };

  ROOT::Math::SMatrix< double, 3 > I(identity_array, 9);
  ROOT::Math::SMatrix< double, 3 > R = I;

  if (isotropic_)
  {
    //-------------------------------------------------------------------------
    // sphere point picking
    // https://mathworld.wolfram.com/SpherePointPicking.html
    //-------------------------------------------------------------------------

    // get random point on the surface of the unit sphere
    double b_array[3] = { distribution_(generator_),
                          distribution_(generator_),
                          distribution_(generator_)  };
    ROOT::Math::SVector< double, 3 > b(b_array, 3);
    b = b.Unit(); // normalize

    // ROOT::Math::SVector< double, 3 > b(b_array, 3);
    // std::cout << "b: " << b(0) << ", " << b(1) << ", " << b(2) << std::endl;
    // std::cout << "ROOT::Math::Mag(b): " << ROOT::Math::Mag(b) << std::endl;
    // b = b.Unit();
    // std::cout << "b: " << b(0) << ", " << b(1) << ", " << b(2)
    //           << std::endl;
    // std::cout << "ROOT::Math::Mag(b): " << ROOT::Math::Mag(b) << std::endl;

    //-------------------------------------------------------------------------
    // rotation matrix
    // https://math.stackexchange.com/a/476311
    //-------------------------------------------------------------------------

    // incident neutrino momentum direction
    ROOT::Math::SVector< double, 3 > a(p_array, 3);
    a = a.Unit();  // normalize

    // cosine between a and b
    double cosine = ROOT::Math::Dot(a, b);

    // cross product between a and b
    ROOT::Math::SVector< double, 3 > v = ROOT::Math::Cross(a, b);

    // sine between a and b
    double sine = ROOT::Math::Mag(v);

    // skew-symmetric cross-product matrix of v
    double V_array[9] = {     0, -v(2),  v(1),
                           v(2),     0, -v(0),
                          -v(1),  v(0),     0  };
    ROOT::Math::SMatrix< double, 3 > V(V_array, 9);

    // rotation matrix
    R = I + V + V*V*(1-cosine)/sine/sine;

    //-------------------------------------------------------------------------
  }

  //---------------------------------------------------------------------------

  // timing
  double time = 0.;  // ns
  if (!supernova_timing_->Initialized())
  {
    supernova_timing_->Initialize();
  }
  if (supernova_timing_->Status())
  {
    time = supernova_timing_->Sample(neutrino_energy);  // s
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
    // generatorParticle->SetPx      (ip->px());
    // generatorParticle->SetPy      (ip->py());
    // generatorParticle->SetPz      (ip->pz());
    generatorParticle->SetEnergy  (ip->total_energy());

    ROOT::Math::SVector< double, 3 > a(ip->px(), ip->py(), ip->pz());
    a = R*a;

    generatorParticle->SetPx      (a(0));
    generatorParticle->SetPy      (a(1));
    generatorParticle->SetPz      (a(2));

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
    // generatorParticle->SetPx      (fp->px());
    // generatorParticle->SetPy      (fp->py());
    // generatorParticle->SetPz      (fp->pz());
    generatorParticle->SetEnergy  (fp->total_energy());

    ROOT::Math::SVector< double, 3 > a(fp->px(), fp->py(), fp->pz());
    a = R*a;

    generatorParticle->SetPx      (a(0));
    generatorParticle->SetPy      (a(1));
    generatorParticle->SetPz      (a(2));

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

    // G4PrimaryParticle* particle = new G4PrimaryParticle(pdef,
    //                                                     fp->px(),
    //                                                     fp->py(),
    //                                                     fp->pz());

    G4PrimaryParticle* particle = new G4PrimaryParticle(pdef, a(0), a(1), a(2));

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
