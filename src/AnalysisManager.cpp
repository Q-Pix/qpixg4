// -----------------------------------------------------------------------------
//  AnalysisManager.cpp
//
//  Class definition of the analysis manager
//   * Author: Everybody is an author!
//   * Creation date: 4 August 2020
// -----------------------------------------------------------------------------

#include "AnalysisManager.h"

AnalysisManager * AnalysisManager::instance_ = 0;

//-----------------------------------------------------------------------------
AnalysisManager::AnalysisManager()
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
    if (instance_ == 0) instance_ = new AnalysisManager();
    return instance_;
}

//-----------------------------------------------------------------------------
void AnalysisManager::Book(std::string const file_path)
{
    // ROOT output file
    tfile_ = new TFile(file_path.data(), "recreate", "qpix");

    // metadata tree
    metadata_ = new TTree("metadata", "metadata");

    metadata_->Branch("detector_length_x", &detector_length_x_, "detector_length_x/D");
    metadata_->Branch("detector_length_y", &detector_length_y_, "detector_length_y/D");
    metadata_->Branch("detector_length_z", &detector_length_z_, "detector_length_z/D");

    // event tree
    event_tree_ = new TTree("event_tree", "event tree");

    event_tree_->Branch("run",   &run_,   "run/I");
    event_tree_->Branch("event", &event_, "event/I");

    // toggled from RunActionInput to know what kind of particle is generating these hits
    if(m_particle_id > 0)
        event_tree_->Branch("particle_id", &m_particle_id, "RadioParticleID/I");

    // event_tree_->Branch("generator_initial_number_particles",  &generator_initial_number_particles_, "generator_initial_number_particles/I");
    // event_tree_->Branch("generator_initial_particle_x",        &generator_initial_particle_x_);
    // event_tree_->Branch("generator_initial_particle_y",        &generator_initial_particle_y_);
    // event_tree_->Branch("generator_initial_particle_z",        &generator_initial_particle_z_);
    // event_tree_->Branch("generator_initial_particle_t",        &generator_initial_particle_t_);
    // event_tree_->Branch("generator_initial_particle_px",       &generator_initial_particle_px_);
    // event_tree_->Branch("generator_initial_particle_py",       &generator_initial_particle_py_);
    // event_tree_->Branch("generator_initial_particle_pz",       &generator_initial_particle_pz_);
    // event_tree_->Branch("generator_initial_particle_energy",   &generator_initial_particle_energy_);
    // event_tree_->Branch("generator_initial_particle_pdg_code", &generator_initial_particle_pdg_code_);
    // event_tree_->Branch("generator_initial_particle_mass",     &generator_initial_particle_mass_);
    // event_tree_->Branch("generator_initial_particle_charge",   &generator_initial_particle_charge_);

    // event_tree_->Branch("generator_final_number_particles",    &generator_final_number_particles_,   "generator_final_number_particles/I");
    // event_tree_->Branch("generator_final_particle_x",          &generator_final_particle_x_);
    // event_tree_->Branch("generator_final_particle_y",          &generator_final_particle_y_);
    // event_tree_->Branch("generator_final_particle_z",          &generator_final_particle_z_);
    // event_tree_->Branch("generator_final_particle_t",          &generator_final_particle_t_);
    // event_tree_->Branch("generator_final_particle_px",         &generator_final_particle_px_);
    // event_tree_->Branch("generator_final_particle_py",         &generator_final_particle_py_);
    // event_tree_->Branch("generator_final_particle_pz",         &generator_final_particle_pz_);
    // event_tree_->Branch("generator_final_particle_energy",     &generator_final_particle_energy_);
    // event_tree_->Branch("generator_final_particle_pdg_code",   &generator_final_particle_pdg_code_);
    // event_tree_->Branch("generator_final_particle_mass",       &generator_final_particle_mass_);
    // event_tree_->Branch("generator_final_particle_charge",     &generator_final_particle_charge_);

    event_tree_->Branch("number_particles", &number_particles_, "number_particles/I");
    event_tree_->Branch("number_hits",      &number_hits_,      "number_hits/I");

    event_tree_->Branch("energy_deposit",   &energy_deposit_,   "energy_deposit/D");

    // event_tree_->Branch("particle_track_id",        &particle_track_id_);
    // event_tree_->Branch("particle_parent_track_id", &particle_parent_track_id_);
    // event_tree_->Branch("particle_pdg_code",        &particle_pdg_code_);
    // event_tree_->Branch("particle_mass",            &particle_mass_);
    // event_tree_->Branch("particle_charge",          &particle_charge_);
    // event_tree_->Branch("particle_process_key",     &particle_process_key_);
    // event_tree_->Branch("particle_total_occupancy", &particle_total_occupancy_);
    // event_tree_->Branch("particle_initial_x",       &particle_initial_x_);
    // event_tree_->Branch("particle_initial_y",       &particle_initial_y_);
    // event_tree_->Branch("particle_initial_z",       &particle_initial_z_);
    // event_tree_->Branch("particle_initial_t",       &particle_initial_t_);
    // event_tree_->Branch("particle_initial_px",      &particle_initial_px_);
    // event_tree_->Branch("particle_initial_py",      &particle_initial_py_);
    // event_tree_->Branch("particle_initial_pz",      &particle_initial_pz_);
    // event_tree_->Branch("particle_initial_energy",  &particle_initial_energy_);

    // event_tree_->Branch("particle_number_daughters",  &particle_number_daughters_);
    // event_tree_->Branch("particle_daughter_track_id", &particle_daughter_track_ids_);

    // event_tree_->Branch("hit_track_id",       &hit_track_id_);
    // event_tree_->Branch("hit_start_x",        &hit_start_x_);
    // event_tree_->Branch("hit_start_y",        &hit_start_y_);
    // event_tree_->Branch("hit_start_z",        &hit_start_z_);
    // event_tree_->Branch("hit_start_t",        &hit_start_t_);
    // event_tree_->Branch("hit_end_x",          &hit_end_x_);
    // event_tree_->Branch("hit_end_y",          &hit_end_y_);
    // event_tree_->Branch("hit_end_z",          &hit_end_z_);
    // event_tree_->Branch("hit_end_t",          &hit_end_t_);
    // event_tree_->Branch("hit_energy_deposit", &hit_energy_deposit_);
    // event_tree_->Branch("hit_length",         &hit_length_);
    // event_tree_->Branch("hit_process_key",    &hit_process_key_);

    // if using fill mcparticle use these methods instead
    event_tree_->Branch("hit_track_id",       &m_hit_track_id_);
    event_tree_->Branch("hit_start_x",        &m_hit_start_x_);
    event_tree_->Branch("hit_start_y",        &m_hit_start_y_);
    event_tree_->Branch("hit_start_z",        &m_hit_start_z_);
    event_tree_->Branch("hit_start_t",        &m_hit_start_t_);
    event_tree_->Branch("hit_end_x",          &m_hit_end_x_);
    event_tree_->Branch("hit_end_y",          &m_hit_end_y_);
    event_tree_->Branch("hit_end_z",          &m_hit_end_z_);
    event_tree_->Branch("hit_end_t",          &m_hit_end_t_);
    event_tree_->Branch("hit_energy_deposit", &m_hit_energy_deposit_);
    event_tree_->Branch("hit_length",         &m_hit_length_);
    event_tree_->Branch("hit_process_key",    &m_hit_process_key_);
}

//-----------------------------------------------------------------------------
void AnalysisManager::Save()
{
    // write TTree objects to file and close file
    tfile_->cd();
    metadata_->Write();
    event_tree_->Write();
    tfile_->Close();
}

//-----------------------------------------------------------------------------
void AnalysisManager::EventReset()
{
    // reset event variables after filling TTree objects per event
    event_ = -1;
    number_particles_ = 0;

    // generator_initial_number_particles_ = 0;
    // generator_initial_particle_x_.clear();
    // generator_initial_particle_y_.clear();
    // generator_initial_particle_z_.clear();
    // generator_initial_particle_t_.clear();
    // generator_initial_particle_px_.clear();
    // generator_initial_particle_py_.clear();
    // generator_initial_particle_pz_.clear();
    // generator_initial_particle_energy_.clear();
    // generator_initial_particle_pdg_code_.clear();
    // generator_initial_particle_mass_.clear();
    // generator_initial_particle_charge_.clear();

    // generator_final_number_particles_ = 0;
    // generator_final_particle_x_.clear();
    // generator_final_particle_y_.clear();
    // generator_final_particle_z_.clear();
    // generator_final_particle_t_.clear();
    // generator_final_particle_px_.clear();
    // generator_final_particle_py_.clear();
    // generator_final_particle_pz_.clear();
    // generator_final_particle_energy_.clear();
    // generator_final_particle_pdg_code_.clear();
    // generator_final_particle_mass_.clear();
    // generator_final_particle_charge_.clear();

    number_hits_ = 0;
    energy_deposit_ = 0;

    // particle_track_id_.clear();
    // particle_parent_track_id_.clear();
    // particle_pdg_code_.clear();
    // particle_mass_.clear();
    // particle_charge_.clear();
    // particle_process_key_.clear();
    // particle_total_occupancy_.clear();

    // particle_number_daughters_.clear();
    // particle_daughter_track_ids_.clear();

    // particle_initial_x_.clear();
    // particle_initial_y_.clear();
    // particle_initial_z_.clear();
    // particle_initial_t_.clear();

    // particle_initial_px_.clear();
    // particle_initial_py_.clear();
    // particle_initial_pz_.clear();
    // particle_initial_energy_.clear();

    // hit_track_id_.clear();
    // hit_start_x_.clear();
    // hit_start_y_.clear();
    // hit_start_z_.clear();
    // hit_start_t_.clear();
    // hit_end_x_.clear();
    // hit_end_y_.clear();
    // hit_end_z_.clear();
    // hit_end_t_.clear();
    // hit_energy_deposit_.clear();
    // hit_length_.clear();
    // hit_process_key_.clear();
}

//-----------------------------------------------------------------------------
void AnalysisManager::EventFill()
{
    // fill TTree objects per event
    event_tree_->Fill();
}

//-----------------------------------------------------------------------------
void AnalysisManager::SetRun(int const value)
{
    run_ = value;
}

//-----------------------------------------------------------------------------
void AnalysisManager::SetEvent(int const value)
{
    event_ = value;
}

//-----------------------------------------------------------------------------
void AnalysisManager::FillMetadata(double const & detector_length_x,
                                   double const & detector_length_y,
                                   double const & detector_length_z)
{
    detector_length_x_ = detector_length_x;
    detector_length_y_ = detector_length_y;
    detector_length_z_ = detector_length_z;
    metadata_->Fill();
}

//-----------------------------------------------------------------------------
void AnalysisManager::AddInitialGeneratorParticle(GeneratorParticle const * particle)
{
    // generator_initial_number_particles_ += 1;
    // generator_initial_particle_pdg_code_.push_back(particle->PDGCode());
    // generator_initial_particle_mass_.push_back(particle->Mass());
    // generator_initial_particle_charge_.push_back(particle->Charge());
    // generator_initial_particle_x_.push_back(particle->X());
    // generator_initial_particle_y_.push_back(particle->Y());
    // generator_initial_particle_z_.push_back(particle->Z());
    // generator_initial_particle_t_.push_back(particle->T());
    // generator_initial_particle_px_.push_back(particle->Px());
    // generator_initial_particle_py_.push_back(particle->Py());
    // generator_initial_particle_pz_.push_back(particle->Pz());
    // generator_initial_particle_energy_.push_back(particle->Energy());
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

    number_particles_ += 1;

    std::vector< TrajectoryHit > const hits = particle->Hits();

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
    number_particles_ += 1;
    std::vector< TrajectoryHit > const hits = particle->Hits();
    number_hits_ = hits.size();

    for (auto const & hit : hits)
    {
        energy_deposit_ += hit.Energy();

        m_hit_track_id_ = hit.TrackID();

        m_hit_start_x_ = hit.StartPoint().X();
        m_hit_start_y_ = hit.StartPoint().Y();
        m_hit_start_z_ = hit.StartPoint().Z();
        m_hit_start_t_ = hit.StartTime();

        m_hit_end_x_ = hit.EndPoint().X();
        m_hit_end_y_ = hit.EndPoint().Y();
        m_hit_end_z_ = hit.EndPoint().Z();
        m_hit_end_t_ = hit.EndTime();

        m_hit_length_ = hit.Length();
        m_hit_energy_deposit_ = hit.Energy();

        m_hit_process_key_ = this->ProcessToKey(hit.Process());

        // fill on every hit
        event_tree_->Fill();
    }
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
    else { std::cout << "Uncategorized process: " << process << std::endl; }

    return key;
}

void AnalysisManager::Cd(){
    tfile_->cd();
}

