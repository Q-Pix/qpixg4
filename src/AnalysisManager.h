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
#include "GeneratorParticle.h"
#include "MCParticle.h"

// GEANT4 includes
#include "globals.hh"

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
        void Cd();
        void EventFill();
        void EventReset();

        void SetRun(int const);
        void SetEvent(int const);
        void SetParticleID(int const id){m_particle_id = id;};

        void FillMetadata(double const &, double const &, double const &);
        void FillROOTMeta(Float_t, Float_t, Float_t, Float_t, Float_t, Float_t,
                          Int_t, Int_t, Float_t, Int_t, Int_t, Int_t);

        void AddInitialGeneratorParticle(GeneratorParticle const *);
        void AddFinalGeneratorParticle(GeneratorParticle const *);

        void AddMCParticle(MCParticle const *);
        // adding to fill entries in tree by hits instead of events
        void FillMCParticle(MCParticle const *);

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

        // variables that will go into the metadata tree
        double detector_length_x_;
        double detector_length_y_;
        double detector_length_z_;

        // extra meta variables from the Odyssey neutrino interactions, defined
        // in ROOTManager.h
        Float_t axis_x_ = kMaxLong64;
        Float_t axis_y_ = kMaxLong64;
        Float_t axis_z_ = kMaxLong64;
        Float_t xpos_ = -1;
        Float_t ypos_ = -1;
        Float_t zpos_ = -1;
        Int_t fsPdg_ = 0;
        Float_t fsEnergy_ = -1;
        Int_t fsEvt_ = -1;
        Int_t fsFHC_ = -1;
        Int_t fsRun_ = -1;

        // variables that will go into the event trees
        int run_;
        int event_;

        int number_particles_;
        int number_hits_;

        double energy_deposit_;

        // for FillMCParticle to fill by event
        int    m_hit_track_id_;
        double m_hit_start_x_;
        double m_hit_start_y_;
        double m_hit_start_z_;
        double m_hit_start_t_;
        double m_hit_end_x_;
        double m_hit_end_y_;
        double m_hit_end_z_;
        double m_hit_end_t_;
        double m_hit_length_;
        double m_hit_energy_deposit_;
        int m_hit_process_key_;
        int m_particle_id=-1; // helper from run action

        // std::vector< int >    particle_track_id_;
        // std::vector< int >    particle_parent_track_id_;
        // std::vector< int >    particle_pdg_code_;
        // std::vector< double > particle_mass_;
        // std::vector< double > particle_charge_;
        // std::vector< int >    particle_process_key_;
        // std::vector< int >    particle_total_occupancy_;

        // std::vector< int >                particle_number_daughters_;
        // std::vector< std::vector< int > > particle_daughter_track_ids_;

        // std::vector< double > particle_initial_x_;
        // std::vector< double > particle_initial_y_;
        // std::vector< double > particle_initial_z_;
        // std::vector< double > particle_initial_t_;

        // std::vector< double > particle_initial_px_;
        // std::vector< double > particle_initial_py_;
        // std::vector< double > particle_initial_pz_;
        // std::vector< double > particle_initial_energy_;

        // std::vector< int >    hit_track_id_;
        // std::vector< double > hit_start_x_;
        // std::vector< double > hit_start_y_;
        // std::vector< double > hit_start_z_;
        // std::vector< double > hit_start_t_;
        // std::vector< double > hit_end_x_;
        // std::vector< double > hit_end_y_;
        // std::vector< double > hit_end_z_;
        // std::vector< double > hit_end_t_;
        // std::vector< double > hit_length_;
        // std::vector< double > hit_energy_deposit_;
        // std::vector< int >    hit_process_key_;

        // number of generator particles
        // int generator_initial_number_particles_;
        // int generator_final_number_particles_;

        // vectors for initial generator particles
        // std::vector< double > generator_initial_particle_x_;
        // std::vector< double > generator_initial_particle_y_;
        // std::vector< double > generator_initial_particle_z_;
        // std::vector< double > generator_initial_particle_t_;
        // std::vector< double > generator_initial_particle_px_;
        // std::vector< double > generator_initial_particle_py_;
        // std::vector< double > generator_initial_particle_pz_;
        // std::vector< double > generator_initial_particle_energy_;
        // std::vector< int >    generator_initial_particle_pdg_code_;
        // std::vector< double > generator_initial_particle_mass_;
        // std::vector< double > generator_initial_particle_charge_;

        // vectors for final generator particles
        // std::vector< double > generator_final_particle_x_;
        // std::vector< double > generator_final_particle_y_;
        // std::vector< double > generator_final_particle_z_;
        // std::vector< double > generator_final_particle_t_;
        // std::vector< double > generator_final_particle_px_;
        // std::vector< double > generator_final_particle_py_;
        // std::vector< double > generator_final_particle_pz_;
        // std::vector< double > generator_final_particle_energy_;
        // std::vector< int >    generator_final_particle_pdg_code_;
        // std::vector< double > generator_final_particle_mass_;
        // std::vector< double > generator_final_particle_charge_;

};

#endif
