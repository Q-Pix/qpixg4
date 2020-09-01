// -----------------------------------------------------------------------------
//  AnalysisManager.h
//
//  Class definition of the analysis manager
//   * Author: Everybody is an author!
//   * Creation date: 4 August 2020
// -----------------------------------------------------------------------------

#ifndef AnalysisManager_h
#define AnalysisManager_h 1

// Q-Pix includes
#include "MCParticle.h"

// GEANT4 includes
#include "globals.hh"

// MARLEY includes
#include "marley/Event.hh"
#include "marley/Particle.hh"

// ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

// C++ includes
#include <map>
#include <set>

class AnalysisManager {

    public:

        AnalysisManager();
        ~AnalysisManager();

        void Book(std::string const);
        void Save();
        void EventFill();
        void EventReset();

        void SetRun(int const value);
        void SetEvent(int const value);

        void FillMetadata(double const &, double const &, double const &);

        void AddInitialMARLEYParticle(marley::Particle const &);
        void AddFinalMARLEYParticle(marley::Particle const &);

        void AddMCParticle(MCParticle const *);

        int ProcessToKey(std::string const &);

        inline void AddProcess(std::string const & process) { process_names_.insert(process); }
        inline std::set< std::string > GetProcessNames() const { return process_names_; }

        static AnalysisManager* Instance();

    private:

        static AnalysisManager * instance_;

        std::set< std::string > process_names_;

        // ROOT objects
        TFile * tfile_;
        TTree * metadata_;
        TTree * event_tree_;
        TTree * marley_event_tree_;

        // variables that will go into the metadata tree
        double detector_length_x_;
        double detector_length_y_;
        double detector_length_z_;

        // variables that will go into the event trees
        int run_;
        int event_;

        int number_particles_;
        int number_hits_;

        double energy_deposit_;

        std::vector< int >    particle_track_id_;
        std::vector< int >    particle_parent_track_id_;
        std::vector< int >    particle_pdg_code_;
        std::vector< double > particle_mass_;
        std::vector< double > particle_charge_;
        std::vector< int >    particle_process_key_;
        std::vector< int >    particle_total_occupancy_;

        std::vector< double > particle_initial_x_;
        std::vector< double > particle_initial_y_;
        std::vector< double > particle_initial_z_;
        std::vector< double > particle_initial_t_;

        std::vector< double > particle_initial_px_;
        std::vector< double > particle_initial_py_;
        std::vector< double > particle_initial_pz_;
        std::vector< double > particle_initial_energy_;

        std::vector< int >    hit_track_id_;
        std::vector< double > hit_start_x_;
        std::vector< double > hit_start_y_;
        std::vector< double > hit_start_z_;
        std::vector< double > hit_start_t_;
        std::vector< double > hit_end_x_;
        std::vector< double > hit_end_y_;
        std::vector< double > hit_end_z_;
        std::vector< double > hit_end_t_;
        std::vector< double > hit_length_;
        std::vector< double > hit_energy_deposit_;
        std::vector< int >    hit_process_key_;

        // number of MARLEY particles
        int marley_initial_number_particles_;
        int marley_final_number_particles_;

        // vectors for initial MARLEY particles
        std::vector< double > marley_initial_particle_px_;
        std::vector< double > marley_initial_particle_py_;
        std::vector< double > marley_initial_particle_pz_;
        std::vector< double > marley_initial_particle_energy_;
        std::vector< int >    marley_initial_particle_pdg_code_;
        std::vector< double > marley_initial_particle_mass_;
        std::vector< double > marley_initial_particle_charge_;

        // vectors for final MARLEY particles
        std::vector< double > marley_final_particle_px_;
        std::vector< double > marley_final_particle_py_;
        std::vector< double > marley_final_particle_pz_;
        std::vector< double > marley_final_particle_energy_;
        std::vector< int >    marley_final_particle_pdg_code_;
        std::vector< double > marley_final_particle_mass_;
        std::vector< double > marley_final_particle_charge_;

};

#endif
