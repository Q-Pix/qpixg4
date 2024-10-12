// -----------------------------------------------------------------------------
//  AnalysisManager.cpp
//
//  Class definition of the analysis manager
//   * Author: Everybody is an author!
//   * Creation date: 4 August 2020
// -----------------------------------------------------------------------------

// Class includes
#include "AnalysisManager.h"

// Qpix includes
#include "AnalysisData.h"
#include "ConfigManager.h"
#include "MCParticle.h"
#include "GeneratorParticle.h"


// Geant4 includes
#include "G4AutoLock.hh"

// ROOT includes
#include "TFile.h"
#include "TTree.h"

namespace {
  G4Mutex bookMutex = G4MUTEX_INITIALIZER;
  G4Mutex saveMutex = G4MUTEX_INITIALIZER;
  G4Mutex fillMutex = G4MUTEX_INITIALIZER;
  G4Mutex instMutex = G4MUTEX_INITIALIZER;
  G4Mutex metaMutex = G4MUTEX_INITIALIZER;
}

AnalysisManager * AnalysisManager::instance_ = 0;

//-----------------------------------------------------------------------------
AnalysisManager::AnalysisManager()
  : tfile_(0), metadata_(0), event_tree_(0)
{
#ifdef G4ANALYSIS_USE
#endif
}

//-----------------------------------------------------------------------------
AnalysisManager::~AnalysisManager()
{
#ifdef G4ANALYSIS_USE
#endif
}

//-----------------------------------------------------------------------------
AnalysisManager * AnalysisManager::Instance()
{
  G4AutoLock instLock(&instMutex);
  if (instance_ == 0) instance_ = new AnalysisManager();
  return instance_;
}

//-----------------------------------------------------------------------------
void AnalysisManager::Book(const std::string& file_path)
{

  if (!G4Threading::IsMasterThread()) return; // only run Book() for the master thread

  // Check if tfile_ is a null pointer, if so, create ROOT output file
  if (tfile_ == 0) {
    //G4cout << "Testing file_path.data() for null pointer. file_path.data() = " << file_path.data() << G4endl;
    tfile_ = new TFile(file_path.data(), "recreate", "qpix");
  }

  // check if metadata_ is a null pointer, if so, create metadata tree
  if (metadata_ == 0)
  {
    metadata_ = new TTree("metadata", "metadata");

    metadata_->Branch("detector_length_x", &detector_length_x_, "detector_length_x/D");
    metadata_->Branch("detector_length_y", &detector_length_y_, "detector_length_y/D");
    metadata_->Branch("detector_length_z", &detector_length_z_, "detector_length_z/D");
    metadata_->Branch("use_HD_detector_configuration", &useHDDetectorConfiguration_);
  }

  // check if event_tree_ is a null pointer, if so, create event tree
  if (event_tree_ == 0) 
  {
    event_tree_ = new TTree("event_tree", "event tree");

    event_tree_->Branch("run",   &event.run_,   "run/I");
    event_tree_->Branch("event", &event.event_, "event/I");

    event_tree_->Branch("generator_initial_number_particles",  &event.generator_initial_number_particles_, "generator_initial_number_particles/I");
    event_tree_->Branch("generator_initial_particle_x",        &event.generator_initial_particle_x_);
    event_tree_->Branch("generator_initial_particle_y",        &event.generator_initial_particle_y_);
    event_tree_->Branch("generator_initial_particle_z",        &event.generator_initial_particle_z_);
    event_tree_->Branch("generator_initial_particle_t",        &event.generator_initial_particle_t_);
    event_tree_->Branch("generator_initial_particle_px",       &event.generator_initial_particle_px_);
    event_tree_->Branch("generator_initial_particle_py",       &event.generator_initial_particle_py_);
    event_tree_->Branch("generator_initial_particle_pz",       &event.generator_initial_particle_pz_);
    event_tree_->Branch("generator_initial_particle_energy",   &event.generator_initial_particle_energy_);
    event_tree_->Branch("generator_initial_particle_pdg_code", &event.generator_initial_particle_pdg_code_);
    event_tree_->Branch("generator_initial_particle_mass",     &event.generator_initial_particle_mass_);
    event_tree_->Branch("generator_initial_particle_charge",   &event.generator_initial_particle_charge_);

    event_tree_->Branch("generator_intermediate_number_particles",  &event.generator_intermediate_number_particles_, "generator_intermediate_number_particles/I");
    event_tree_->Branch("generator_intermediate_particle_x",        &event.generator_intermediate_particle_x_);
    event_tree_->Branch("generator_intermediate_particle_y",        &event.generator_intermediate_particle_y_);
    event_tree_->Branch("generator_intermediate_particle_z",        &event.generator_intermediate_particle_z_);
    event_tree_->Branch("generator_intermediate_particle_t",        &event.generator_intermediate_particle_t_);
    event_tree_->Branch("generator_intermediate_particle_px",       &event.generator_intermediate_particle_px_);
    event_tree_->Branch("generator_intermediate_particle_py",       &event.generator_intermediate_particle_py_);
    event_tree_->Branch("generator_intermediate_particle_pz",       &event.generator_intermediate_particle_pz_);
    event_tree_->Branch("generator_intermediate_particle_energy",   &event.generator_intermediate_particle_energy_);
    event_tree_->Branch("generator_intermediate_particle_pdg_code", &event.generator_intermediate_particle_pdg_code_);
    event_tree_->Branch("generator_intermediate_particle_mass",     &event.generator_intermediate_particle_mass_);
    event_tree_->Branch("generator_intermediate_particle_charge",   &event.generator_intermediate_particle_charge_);

    event_tree_->Branch("generator_final_number_particles",    &event.generator_final_number_particles_,   "generator_final_number_particles/I");
    event_tree_->Branch("generator_final_particle_x",          &event.generator_final_particle_x_);
    event_tree_->Branch("generator_final_particle_y",          &event.generator_final_particle_y_);
    event_tree_->Branch("generator_final_particle_z",          &event.generator_final_particle_z_);
    event_tree_->Branch("generator_final_particle_t",          &event.generator_final_particle_t_);
    event_tree_->Branch("generator_final_particle_px",         &event.generator_final_particle_px_);
    event_tree_->Branch("generator_final_particle_py",         &event.generator_final_particle_py_);
    event_tree_->Branch("generator_final_particle_pz",         &event.generator_final_particle_pz_);
    event_tree_->Branch("generator_final_particle_energy",     &event.generator_final_particle_energy_);
    event_tree_->Branch("generator_final_particle_pdg_code",   &event.generator_final_particle_pdg_code_);
    event_tree_->Branch("generator_final_particle_mass",       &event.generator_final_particle_mass_);
    event_tree_->Branch("generator_final_particle_charge",     &event.generator_final_particle_charge_);

    event_tree_->Branch("number_particles", &event.number_particles_, "number_particles/I");
    event_tree_->Branch("number_hits",      &event.number_hits_,      "number_hits/L");

    event_tree_->Branch("energy_deposit",   &event.energy_deposit_,   "energy_deposit/D");

    event_tree_->Branch("particle_track_id",        &event.particle_track_id_);
    event_tree_->Branch("particle_parent_track_id", &event.particle_parent_track_id_);
    event_tree_->Branch("particle_pdg_code",        &event.particle_pdg_code_);
    event_tree_->Branch("particle_mass",            &event.particle_mass_);
    event_tree_->Branch("particle_charge",          &event.particle_charge_);
    event_tree_->Branch("particle_process_key",     &event.particle_process_key_);
    event_tree_->Branch("particle_total_occupancy", &event.particle_total_occupancy_);
    event_tree_->Branch("particle_initial_x",       &event.particle_initial_x_);
    event_tree_->Branch("particle_initial_y",       &event.particle_initial_y_);
    event_tree_->Branch("particle_initial_z",       &event.particle_initial_z_);
    event_tree_->Branch("particle_initial_t",       &event.particle_initial_t_);
    event_tree_->Branch("particle_initial_px",      &event.particle_initial_px_);
    event_tree_->Branch("particle_initial_py",      &event.particle_initial_py_);
    event_tree_->Branch("particle_initial_pz",      &event.particle_initial_pz_);
    event_tree_->Branch("particle_initial_energy",  &event.particle_initial_energy_);

    event_tree_->Branch("particle_number_daughters",  &event.particle_number_daughters_);
    event_tree_->Branch("particle_daughter_track_id", &event.particle_daughter_track_ids_);

    event_tree_->Branch("hit_track_id",       &event.hit_track_id_);
    event_tree_->Branch("hit_pdg_code",       &event.hit_pdg_code_);
    event_tree_->Branch("hit_start_x",        &event.hit_start_x_);
    event_tree_->Branch("hit_start_y",        &event.hit_start_y_);
    event_tree_->Branch("hit_start_z",        &event.hit_start_z_);
    event_tree_->Branch("hit_start_t",        &event.hit_start_t_);
    event_tree_->Branch("hit_end_x",          &event.hit_end_x_);
    event_tree_->Branch("hit_end_y",          &event.hit_end_y_);
    event_tree_->Branch("hit_end_z",          &event.hit_end_z_);
    event_tree_->Branch("hit_end_t",          &event.hit_end_t_);
    event_tree_->Branch("hit_energy_deposit", &event.hit_energy_deposit_);
    event_tree_->Branch("hit_length",         &event.hit_length_);
    event_tree_->Branch("hit_process_key",    &event.hit_process_key_);
  }
}

//-----------------------------------------------------------------------------
void AnalysisManager::Save()
{
  if (!G4Threading::IsMasterThread()) {return;}  // only run Save() with the master thread

  //G4AutoLock saveLock(&saveMutex);

  // write TTree objects to file and close file
  tfile_->cd();
  metadata_->Write();
  event_tree_->Write();
  tfile_->Close();
}

//-----------------------------------------------------------------------------
void AnalysisManager::EventFill(const AnalysisData& rhs)
{
  G4AutoLock fillLock(&fillMutex);
  // fill TTree objects per event
  event = rhs;
  event_tree_->Fill();
}

//-----------------------------------------------------------------------------
void AnalysisManager::SetEvent(int const value)
{
    // event_ = value;
}

//-----------------------------------------------------------------------------
void AnalysisManager::FillROOTMeta(Float_t axis_x, Float_t axis_y, Float_t axis_z, Float_t xpos, Float_t ypos, Float_t zpos,
            Int_t nEvt, Int_t fsPdg, Float_t fsEnergy, Int_t fsEvt, Int_t fsFileNo, Int_t fsFHC, Int_t fsRun, Int_t nFS, Float_t lepKE,
            Float_t hadTot, Float_t hadPip, Float_t hadPim, Float_t hadPi0, Float_t hadP, Float_t hadN, Float_t hadOther, double energy_deposit)
{
    metadata_->Branch("axis_x", &axis_x_, "axis_x/F");
    metadata_->Branch("axis_y", &axis_y_, "axis_y/F");
    metadata_->Branch("axis_z", &axis_z_, "axis_z/F");
    metadata_->Branch("xpos", &xpos_, "xpos/F");
    metadata_->Branch("ypos", &ypos_, "ypos/F");
    metadata_->Branch("zpos", &zpos_, "zpos/F");
    metadata_->Branch("nEvt", &nEvt_, "nEvt/I");
    metadata_->Branch("fsPdg", &fsPdg_, "fsPdg/I");
    metadata_->Branch("fsEnergy", &fsEnergy_, "fsEnergy/F");
    metadata_->Branch("fsEvt", &fsEvt_, "fsEvt/I");
    metadata_->Branch("fsFileNo", &fsFileNo_, "fsFileNo/I");
    metadata_->Branch("fsFHC", &fsFHC_, "fsFHC/I");
    metadata_->Branch("fsRun", &fsRun_, "fsRun/I");
    metadata_->Branch("nFS", &nFS_, "nFS/I");
    metadata_->Branch("lepKE", &lepKE_, "lepKE/F");
    // add the hadron energy into the metadata_ branches
    metadata_->Branch("hadTot", &hadTot_, "hadTot/F");
    metadata_->Branch("hadPip", &hadPip_, "hadPip/F");
    metadata_->Branch("hadPim", &hadPim_, "hadPim/F");
    metadata_->Branch("hadPi0", &hadPi0_, "hadPi0/F");
    metadata_->Branch("hadP", &hadP_, "hadP/F");
    metadata_->Branch("hadN", &hadN_, "hadN/F");
    metadata_->Branch("hadOther", &hadOther_, "hadOther/F");
    metadata_->Branch("hadOther", &hadOther_, "hadOther/F");
    // keep track of the work that geant4 did
    metadata_->Branch("energy_deposit", &energy_deposit_, "energy_deposit/D");
    // assign values
    axis_x_ =  axis_x;
    axis_y_ =  axis_y;
    axis_z_ =  axis_z;
    xpos_ = xpos;
    ypos_ = ypos;
    zpos_ = zpos;
    nEvt_ = nEvt;
    fsPdg_ = fsPdg;
    fsEnergy_ = fsEnergy;
    fsEvt_ = fsEvt;
    fsFileNo_ = fsFileNo;
    fsFHC_ = fsFHC;
    fsRun_ = fsRun;
    nFS_ = nFS;
    lepKE_ = lepKE;
    hadTot_ = hadTot;
    hadPip_ = hadPip;
    hadPim_ = hadPim;
    hadPi0_ = hadPi0;
    hadP_ = hadP;
    hadN_ = hadN;
    hadOther_ = hadOther;
    energy_deposit_ = energy_deposit;
}


//-----------------------------------------------------------------------------
void AnalysisManager::FillMetadata()
{
  G4AutoLock metaLock(&metaMutex);
  detector_length_x_ = ConfigManager::GetDetectorWidth();
  detector_length_y_ = ConfigManager::GetDetectorHeight();
  detector_length_z_ = ConfigManager::GetDetectorLength();
  useHDDetectorConfiguration_ = ConfigManager::GetUseHDDetectorConfiguration();
  metadata_->Fill();
}

//-----------------------------------------------------------------------------
void AnalysisManager::AddInitialGeneratorParticle(GeneratorParticle const * particle)
{
    event.generator_initial_number_particles_ += 1;
    event.generator_initial_particle_pdg_code_.push_back(particle->PDGCode());
    event.generator_initial_particle_mass_.push_back(particle->Mass());
    event.generator_initial_particle_charge_.push_back(particle->Charge());
    event.generator_initial_particle_x_.push_back(particle->X());
    event.generator_initial_particle_y_.push_back(particle->Y());
    event.generator_initial_particle_z_.push_back(particle->Z());
    event.generator_initial_particle_t_.push_back(particle->T());
    event.generator_initial_particle_px_.push_back(particle->Px());
    event.generator_initial_particle_py_.push_back(particle->Py());
    event.generator_initial_particle_pz_.push_back(particle->Pz());
    event.generator_initial_particle_energy_.push_back(particle->Energy());
}

//-----------------------------------------------------------------------------
void AnalysisManager::AddFinalGeneratorParticle(GeneratorParticle const * particle)
{
    // generator_final_number_particles_ += 1;
    // generator_final_particle_pdg_code_.push_back(particle->PDGCode());
    // generator_final_particle_mass_.push_back(particle->Mass());
    // generator_final_particle_charge_.push_back(particle->Charge());
    // generator_final_particle_x_.push_back(particle->X());
    // generator_final_particle_y_.push_back(particle->Y());
    // generator_final_particle_z_.push_back(particle->Z());
    // generator_final_particle_t_.push_back(particle->T());
    // generator_final_particle_px_.push_back(particle->Px());
    // generator_final_particle_py_.push_back(particle->Py());
    // generator_final_particle_pz_.push_back(particle->Pz());
    // generator_final_particle_energy_.push_back(particle->Energy());
}

//-----------------------------------------------------------------------------
void AnalysisManager::AddMCParticle(MCParticle const * particle)
{
    // particle_track_id_.push_back(particle->TrackID());
    // particle_parent_track_id_.push_back(particle->ParentTrackID());
    // particle_pdg_code_.push_back(particle->PDGCode());
    // particle_mass_.push_back(particle->Mass());
    // particle_charge_.push_back(particle->Charge());
    // particle_process_key_.push_back(this->ProcessToKey(particle->Process()));
    // particle_total_occupancy_.push_back(particle->TotalOccupancy());

    // particle_initial_x_.push_back(particle->InitialPosition().X());
    // particle_initial_y_.push_back(particle->InitialPosition().Y());
    // particle_initial_z_.push_back(particle->InitialPosition().Z());
    // particle_initial_t_.push_back(particle->InitialPosition().T());

    // particle_initial_px_.push_back(particle->InitialMomentum().X());
    // particle_initial_py_.push_back(particle->InitialMomentum().Y());
    // particle_initial_pz_.push_back(particle->InitialMomentum().Z());
    // particle_initial_energy_.push_back(particle->InitialMomentum().E());

    // particle_number_daughters_.push_back(particle->NumberDaughters());
    // particle_daughter_track_ids_.push_back(particle->Daughters());

    // number_particles_ += 1;
    // std::vector< TrajectoryHit > const hits = particle->Hits();

// // not used for FillMCParticle
//     for (auto const & hit : hits)
//     {
//         energy_deposit_ += hit.Energy();

//         hit_track_id_.push_back(hit.TrackID());

//         hit_start_x_.push_back(hit.StartPoint().X());
//         hit_start_y_.push_back(hit.StartPoint().Y());
//         hit_start_z_.push_back(hit.StartPoint().Z());
//         hit_start_t_.push_back(hit.StartTime());

//         hit_end_x_.push_back(hit.EndPoint().X());
//         hit_end_y_.push_back(hit.EndPoint().Y());
//         hit_end_z_.push_back(hit.EndPoint().Z());
//         hit_end_t_.push_back(hit.EndTime());

//         hit_length_.push_back(hit.Length());
//         hit_energy_deposit_.push_back(hit.Energy());

//         hit_process_key_.push_back(this->ProcessToKey(hit.Process()));
//         number_hits_ += 1;
//     }
}

void AnalysisManager::FillMCParticle(MCParticle const * particle)
{
    // number_particles_ += 1;
    // std::vector< TrajectoryHit > const hits = particle->Hits();
    // number_hits_ = hits.size();

    // for (auto const & hit : hits)
    // {
    //     energy_deposit_ += hit.Energy();

    //     m_hit_track_id_ = hit.TrackID();

    //     m_hit_start_x_ = hit.StartPoint().X();
    //     m_hit_start_y_ = hit.StartPoint().Y();
    //     m_hit_start_z_ = hit.StartPoint().Z();
    //     m_hit_start_t_ = hit.StartTime();

    //     m_hit_end_x_ = hit.EndPoint().X();
    //     m_hit_end_y_ = hit.EndPoint().Y();
    //     m_hit_end_z_ = hit.EndPoint().Z();
    //     m_hit_end_t_ = hit.EndTime();

    //     m_hit_length_ = hit.Length();
    //     m_hit_energy_deposit_ = hit.Energy();

    //     m_hit_process_key_ = this->ProcessToKey(hit.Process());

    //     // fill on every hit
    //     event_tree_->Fill();
    // }
}


//-----------------------------------------------------------------------------
int AnalysisManager::ProcessToKey(std::string const & process)
{
    int key = -2;

    if (process.compare("NoProcess")                 == 0) key = -1;
    else if (process.compare("primary")              == 0) key =  0;
    else if (process.compare("eIoni")                == 0) key =  1;
    else if (process.compare("msc")                  == 0) key =  2;
    else if (process.compare("compt")                == 0) key =  3;
    else if (process.compare("phot")                 == 0) key =  4;
    else if (process.compare("eBrem")                == 0) key =  5;
    else if (process.compare("ionIoni")              == 0) key =  6;
    else if (process.compare("hIoni")                == 0) key =  7;
    else if (process.compare("RadioactiveDecayBase") == 0) key =  8;
    else if (process.compare("CoulombScat")          == 0) key =  9;
    else if (process.compare("Rayl")                 == 0) key = 10;
    else if (process.compare("Transportation")       == 0) key = 11;
    else if (process.compare("annihil")              == 0) key = 12;
    else if (process.compare("conv")                 == 0) key = 13;
    else if (process.compare("hadElastic")           == 0) key = 14;
    else if (process.compare("nCapture")             == 0) key = 15;
    else if (process.compare("neutronInelastic")     == 0) key = 16;
    else if (process.compare("photonNuclear")        == 0) key = 17;
    else if (process.compare("protonInelastic")      == 0) key = 18;
    else if (process.compare("dInelastic")           == 0) key = 19;
    else if (process.compare("Decay")                == 0) key = 20;
    else if (process.compare("RadioactiveDecay")     == 0) key = 21;
    else if (process.compare("alphaInelastic")       == 0) key = 22;
    else if (process.compare("muIoni")               == 0) key = 23;
    else if (process.compare("pi+Inelastic")         == 0) key = 24;
    else if (process.compare("pi-Inelastic")         == 0) key = 25;
    else if (process.compare("kaon+Inelastic")       == 0) key = 26;
    else if (process.compare("kaon-Inelastic")       == 0) key = 27;
    else { std::cout << "Uncategorized process: " << process << std::endl; }

    return key;
}

void AnalysisManager::Cd(){
    tfile_->cd();
}

