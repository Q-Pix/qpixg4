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

        void AddInitialMARLEYParticle(marley::Particle const &);
        void AddFinalMARLEYParticle(marley::Particle const &);

        void AddMCParticle(MCParticle const *);

        static AnalysisManager* Instance();

    private:

        static AnalysisManager * instance_;

        // ROOT objects
        TFile * tfile_;
        TTree * ttree_;
        TTree * marley_event_tree_;

        // variables that will go into the event trees
        int run_;
        int event_;

        std::vector< int >    particle_track_id_;
        std::vector< int >    particle_parent_track_id_;
        std::vector< int >    particle_pdg_code_;
        std::vector< double > particle_mass_;
        std::vector< double > particle_charge_;
        std::vector< int >    particle_end_process_id_;
        std::vector< int >    particle_total_occupancy_;

        std::vector< double > particle_initial_x_;
        std::vector< double > particle_initial_y_;
        std::vector< double > particle_initial_z_;
        std::vector< double > particle_initial_t_;

        std::vector< double > particle_initial_px_;
        std::vector< double > particle_initial_py_;
        std::vector< double > particle_initial_pz_;
        std::vector< double > particle_initial_energy_;

        std::vector< std::vector< double > > hit_start_x_;
        std::vector< std::vector< double > > hit_start_y_;
        std::vector< std::vector< double > > hit_start_z_;
        std::vector< std::vector< double > > hit_start_t_;
        std::vector< std::vector< double > > hit_end_x_;
        std::vector< std::vector< double > > hit_end_y_;
        std::vector< std::vector< double > > hit_end_z_;
        std::vector< std::vector< double > > hit_end_t_;
        std::vector< std::vector< double > > hit_length_;
        std::vector< std::vector< double > > hit_energy_deposit_;

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
