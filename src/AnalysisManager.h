// -----------------------------------------------------------------------------
//  AnalysisManager.h
//
//  Class definition of the analysis manager
//   * Author: Everybody is an author!
//   * Creation date: 4 August 2020
// -----------------------------------------------------------------------------

#ifndef AnalysisManager_h
#define AnalysisManager_h 1

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

class AnalysisManager {

    public:

        AnalysisManager();
        ~AnalysisManager();

        void Book(const std::string);
        void Save();
        void EventFill();
        void EventReset();

        void SetRun(const int value);
        void SetEvent(const int value);

        void AddInitialMARLEYParticle(const marley::Particle &);
        void AddFinalMARLEYParticle(const marley::Particle &);

        static AnalysisManager* Instance();

    private:

        static AnalysisManager* instance_;

        // ROOT objects
        TFile * tfile_;
        TTree * ttree_;
        TTree * marley_event_tree_;

        // variables that will go into the event trees
        int run_;
        int event_;

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
