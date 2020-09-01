// -----------------------------------------------------------------------------
//  root_macro.c
//
//  Example of a macro for reading the ROOT files produced from the
//  Q_PIX_GEANT4 program.
//   * Author: Everybody is an author!
//   * Creation date: 12 August 2020
// -----------------------------------------------------------------------------

// C++ includes
#include <iostream>
#include <string>
#include <vector>

// ROOT includes
#include "TBranch.h"
#include "TChain.h"
#include "TFile.h"
#include "TROOT.h"

//------------------------------------------------------------
// forward declarations
//------------------------------------------------------------

// progress bar
void update_progress(double progress, size_t precision);
double last_progress_percentage_ = -1.0;

// set branch addresses
void set_branch_addresses();

//------------------------------------------------------------
// input files
//------------------------------------------------------------

std::vector< std::string > file_list_ = {
    "path/to/output/file_0.root",
    "path/to/output/file_1.root",
    "path/to/output/file_2.root",
};

TChain * chain_ = new TChain("event_tree");

//------------------------------------------------------------
// set up branch variables
//------------------------------------------------------------

int run_;
int event_;
int number_particles_;
int number_hits_;
double energy_deposit_;

std::vector< int >    * particle_track_id_ = 0;
std::vector< int >    * particle_parent_track_id_ = 0;
std::vector< int >    * particle_pdg_code_ = 0;
std::vector< double > * particle_mass_ = 0;
std::vector< double > * particle_charge_ = 0;
std::vector< int >    * particle_process_key_ = 0;
std::vector< int >    * particle_total_occupancy_ = 0;

std::vector< double > * particle_initial_x_ = 0;
std::vector< double > * particle_initial_y_ = 0;
std::vector< double > * particle_initial_z_ = 0;
std::vector< double > * particle_initial_t_ = 0;

std::vector< double > * particle_initial_px_ = 0;
std::vector< double > * particle_initial_py_ = 0;
std::vector< double > * particle_initial_pz_ = 0;
std::vector< double > * particle_initial_energy_ = 0;

std::vector< int >    * hit_track_id_ = 0;
std::vector< double > * hit_start_x_ = 0;
std::vector< double > * hit_start_y_ = 0;
std::vector< double > * hit_start_z_ = 0;
std::vector< double > * hit_start_t_ = 0;
std::vector< double > * hit_end_x_ = 0;
std::vector< double > * hit_end_y_ = 0;
std::vector< double > * hit_end_z_ = 0;
std::vector< double > * hit_end_t_ = 0;
std::vector< double > * hit_length_ = 0;
std::vector< double > * hit_energy_deposit_ = 0;
std::vector< int >    * hit_process_key_ = 0;

// set branch addresses
void set_branch_addresses(TChain * chain)
{
  chain->SetBranchAddress("run", &run_);
  chain->SetBranchAddress("event", &event_);
  chain->SetBranchAddress("number_particles", &number_particles_);
  chain->SetBranchAddress("number_hits", &number_hits_);
  chain->SetBranchAddress("energy_deposit", &energy_deposit_);

  chain->SetBranchAddress("particle_track_id", &particle_track_id_);
  chain->SetBranchAddress("particle_parent_track_id", &particle_parent_track_id_);
  chain->SetBranchAddress("particle_pdg_code", &particle_pdg_code_);
  chain->SetBranchAddress("particle_mass", &particle_mass_);
  chain->SetBranchAddress("particle_charge", &particle_charge_);
  chain->SetBranchAddress("particle_process_key", &particle_process_key_);
  chain->SetBranchAddress("particle_total_occupancy", &particle_total_occupancy_);

  chain->SetBranchAddress("particle_initial_x", &particle_initial_x_);
  chain->SetBranchAddress("particle_initial_y", &particle_initial_y_);
  chain->SetBranchAddress("particle_initial_z", &particle_initial_z_);
  chain->SetBranchAddress("particle_initial_t", &particle_initial_t_);

  chain->SetBranchAddress("particle_initial_px", &particle_initial_px_);
  chain->SetBranchAddress("particle_initial_py", &particle_initial_py_);
  chain->SetBranchAddress("particle_initial_pz", &particle_initial_pz_);
  chain->SetBranchAddress("particle_initial_energy", &particle_initial_energy_);

  chain->SetBranchAddress("hit_track_id", &hit_track_id_);

  chain->SetBranchAddress("hit_start_x", &hit_start_x_);
  chain->SetBranchAddress("hit_start_y", &hit_start_y_);
  chain->SetBranchAddress("hit_start_z", &hit_start_z_);
  chain->SetBranchAddress("hit_start_t", &hit_start_t_);

  chain->SetBranchAddress("hit_end_x", &hit_end_x_);
  chain->SetBranchAddress("hit_end_y", &hit_end_y_);
  chain->SetBranchAddress("hit_end_z", &hit_end_z_);
  chain->SetBranchAddress("hit_end_t", &hit_end_t_);

  chain->SetBranchAddress("hit_energy_deposit", &hit_energy_deposit_);
  chain->SetBranchAddress("hit_length", &hit_length_);
  chain->SetBranchAddress("hit_process_key", &hit_process_key_);
} // set_branch_addresses()

//----------------------------------------------------------------------
// main function
//----------------------------------------------------------------------
int main()
{
  //----------------------------------------------------------
  // add files to chain
  //----------------------------------------------------------
  for (auto const& file_name : file_list_)
  {
    chain_->Add(file_name.c_str());
  }

  //----------------------------------------------------------
  // set branch addresses
  //----------------------------------------------------------
  set_branch_addresses(chain_);

  // get number of entries
  size_t const number_entries = chain_->GetEntries();

  std::cout << "There are a total of "
            << number_entries
            << " events in the sample.\n"
            << "Looping over events..."
            << std::endl;

  // loop over events
  for (size_t idx = 0; idx < number_entries; ++idx)
  {
    update_progress(double((idx+1.0) / number_entries), 1);
    chain_->GetEntry(idx);

    // loop over all particles in the event
    for (int p_idx = 0; p_idx < number_particles_; ++p_idx)
    {
      int const track_id = particle_track_id_->at(p_idx);
      int const parent_track_id = particle_parent_track_id_->at(p_idx);
      int const pdg_code = particle_pdg_code_->at(p_idx);
      double const mass = particle_mass_->at(p_idx);  // MeV
      int const process_key = particle_process_key_->at(p_idx);
      double const initial_x = particle_initial_x_->at(p_idx);  // cm
      double const initial_y = particle_initial_y_->at(p_idx);  // cm
      double const initial_z = particle_initial_z_->at(p_idx);  // cm
      double const initial_t = particle_initial_t_->at(p_idx);  // ns
      double const initial_px = particle_initial_px_->at(p_idx);  // MeV
      double const initial_py = particle_initial_py_->at(p_idx);  // MeV
      double const initial_pz = particle_initial_pz_->at(p_idx);  // MeV
      double const initial_energy = particle_initial_energy_->at(p_idx);  // MeV
    }

    // loop over all hits in the event
    for (int h_idx = 0; h_idx < number_hits_; ++h_idx)
    {
      // track ID of particle responsible for this hit
      int const track_id = hit_track_id_->at(h_idx);

      // from PreStepPoint
      double const start_x = hit_start_x_->at(h_idx);  // cm
      double const start_y = hit_start_y_->at(h_idx);  // cm
      double const start_z = hit_start_z_->at(h_idx);  // cm
      double const start_t = hit_start_t_->at(h_idx);  // ns

      // from PostStepPoint
      double const end_x = hit_end_x_->at(h_idx);  // cm
      double const end_y = hit_end_y_->at(h_idx);  // cm
      double const end_z = hit_end_z_->at(h_idx);  // cm
      double const end_t = hit_end_t_->at(h_idx);  // ns

      // energy deposit
      double const energy_deposit = hit_energy_deposit_->at(h_idx);  // MeV

      // PreStepPoint -> PostStepPoint
      double const length = hit_length_->at(h_idx);  // cm

      // process key
      double const process_key = hit_process_key_->at(h_idx);
    }
  }
} // main()

//----------------------------------------------------------------------
// progress bar
//----------------------------------------------------------------------
void update_progress(double progress, size_t precision=1)
{
  size_t const bar_length = 50;
  std::string status = "";

  if (progress < 0)
  {
    progress = 0;
    status = "Halt...";
  }

  if (progress >= 1)
  {
    progress = 1;
    status = "Done!";
  }

  //double const progress_percentage = std::round(progress * 1e6) / 1e4;
  double const progress_percentage =
      std::round(progress * std::pow(10, 2+precision)) / std::pow(10, precision);

  //if (std::fmod(progress_percentage, precision) == 0)
  if (std::fmod(progress_percentage, 1) == 0
      and last_progress_percentage_ != progress_percentage)
  {

    std::cout << "[";
    int pos = bar_length * progress;
    for (size_t i = 0; i < bar_length; ++i)
    {
      if (i < pos) std::cout << "=";
      else if (i == pos) std::cout << ">";
      else std::cout << " ";
    }

    //std::cout << "] " << int(progress_percentage) << "%\r ";
    std::cout << "] " << int(progress_percentage) << "% " << status << "\r";
    std::cout.flush();

    last_progress_percentage_ = progress_percentage;

  }

  if (progress >= 1) std::cout << std::endl;
} // update_progress()

//----------------------------------------------------------------------
// run, forrest, run!
//----------------------------------------------------------------------
void root_macro()
{
  main();
} // root_macro()

