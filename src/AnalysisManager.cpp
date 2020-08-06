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
    if (instance_ == 0) instance_ = new AnalysisManager;
    return instance_;
}

//-----------------------------------------------------------------------------
void AnalysisManager::Book(const std::string file_path)
{
    // ROOT output file
    tfile_ = new TFile(file_path.data(), "recreate", "q_pix_geant");

    // GEANT4 event tree
    ttree_ = new TTree("q_pix_geant4", "GEANT4 event tree");

    ttree_->Branch("run", &run_, "run/I");
    ttree_->Branch("event", &event_, "event/I");

    // MARLEY event tree
    marley_event_tree_ = new TTree("marley_event_tree", "MARLEY event tree");

    marley_event_tree_->Branch("run", &run_, "run/I");
    marley_event_tree_->Branch("event", &event_, "event/I");

    marley_event_tree_->Branch("initial_particle_px", &marley_initial_particle_px_);
    marley_event_tree_->Branch("initial_particle_py", &marley_initial_particle_py_);
    marley_event_tree_->Branch("initial_particle_pz", &marley_initial_particle_pz_);
    marley_event_tree_->Branch("initial_particle_energy", &marley_initial_particle_energy_);
    marley_event_tree_->Branch("initial_particle_pdg_code", &marley_initial_particle_pdg_code_);
    marley_event_tree_->Branch("initial_particle_mass", &marley_initial_particle_mass_);
    marley_event_tree_->Branch("initial_particle_charge", &marley_initial_particle_charge_);

    marley_event_tree_->Branch("final_particle_px", &marley_final_particle_px_);
    marley_event_tree_->Branch("final_particle_py", &marley_final_particle_py_);
    marley_event_tree_->Branch("final_particle_pz", &marley_final_particle_pz_);
    marley_event_tree_->Branch("final_particle_energy", &marley_final_particle_energy_);
    marley_event_tree_->Branch("final_particle_pdg_code", &marley_final_particle_pdg_code_);
    marley_event_tree_->Branch("final_particle_mass", &marley_final_particle_mass_);
    marley_event_tree_->Branch("final_particle_charge", &marley_final_particle_charge_);
}

//-----------------------------------------------------------------------------
void AnalysisManager::Save()
{
    // write TTree objects to file and close file
    ttree_->Write();
    marley_event_tree_->Write();
    tfile_->Close();
}

//-----------------------------------------------------------------------------
void AnalysisManager::EventReset()
{
    // reset event variables after filling TTree objects per event

    event_ = -1;

    marley_initial_particle_px_.clear();
    marley_initial_particle_py_.clear();
    marley_initial_particle_pz_.clear();
    marley_initial_particle_energy_.clear();
    marley_initial_particle_pdg_code_.clear();
    marley_initial_particle_mass_.clear();
    marley_initial_particle_charge_.clear();

    marley_final_particle_px_.clear();
    marley_final_particle_py_.clear();
    marley_final_particle_pz_.clear();
    marley_final_particle_energy_.clear();
    marley_final_particle_pdg_code_.clear();
    marley_final_particle_mass_.clear();
    marley_final_particle_charge_.clear();
}

//-----------------------------------------------------------------------------
void AnalysisManager::EventFill()
{
    // fill TTree objects per event
    ttree_->Fill();
    marley_event_tree_->Fill();
}

//-----------------------------------------------------------------------------
void AnalysisManager::SetRun(const int value)
{
    run_ = value;
}

//-----------------------------------------------------------------------------
void AnalysisManager::SetEvent(const int value)
{
    event_ = value;
}

//-----------------------------------------------------------------------------
void AnalysisManager::AddInitialMARLEYParticle(const marley::Particle & particle)
{
    marley_initial_particle_px_.push_back(particle.px());
    marley_initial_particle_py_.push_back(particle.py());
    marley_initial_particle_pz_.push_back(particle.pz());
    marley_initial_particle_energy_.push_back(particle.total_energy());
    marley_initial_particle_pdg_code_.push_back(particle.pdg_code());
    marley_initial_particle_mass_.push_back(particle.mass());
    marley_initial_particle_charge_.push_back(particle.charge());
}

//-----------------------------------------------------------------------------
void AnalysisManager::AddFinalMARLEYParticle(const marley::Particle & particle)
{
    marley_final_particle_px_.push_back(particle.px());
    marley_final_particle_py_.push_back(particle.py());
    marley_final_particle_pz_.push_back(particle.pz());
    marley_final_particle_energy_.push_back(particle.total_energy());
    marley_final_particle_pdg_code_.push_back(particle.pdg_code());
    marley_final_particle_mass_.push_back(particle.mass());
    marley_final_particle_charge_.push_back(particle.charge());
}

