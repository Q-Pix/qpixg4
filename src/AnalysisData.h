// -----------------------------------------------------------------------------
//  AnalysisData.h
//
//  Class definition of analysisdata (second half of AnalysisManager)
//   * Author: Everybody is an author!
//   * Creation date: 20 December 2022
// -----------------------------------------------------------------------------

#ifndef AnalysisData_h
#define AnalysisData_h 1

// GEANT4 includes
#include "globals.hh"

// ROOT includes
#include "Rtypes.h"

// C++ includes
#include <map>
#include <set>
#include <string>
#include <vector>


class GeneratorParticle;
class MCParticle;

class AnalysisData {

  public:
    AnalysisData();
    ~AnalysisData();

    void EventReset();

    void SetRun(int const);
    void SetEvent(int const);

    void AddInitialGeneratorParticle(GeneratorParticle const *);
    void AddIntermediateGeneratorParticle(GeneratorParticle const *);
    void AddFinalGeneratorParticle(GeneratorParticle const *);

    void AddMCParticle(MCParticle const *);

    int ProcessToKey(const G4String &);

    inline void AddProcess(std::string const & process) { process_names_.insert(process); }
    inline std::set< std::string > GetProcessNames() const { return process_names_; }

    // declarations to use automatically generated copy constructor and assignment operators
    AnalysisData(const AnalysisData& rhs) = default;
    AnalysisData& operator=(const AnalysisData& rhs) = default;   


    std::set< std::string > process_names_;

    // variables that will go into the event trees
    int run_;
    int event_;

    int number_particles_;
    Long64_t number_hits_;

    double energy_deposit_;

    std::vector< int >    particle_track_id_;
    std::vector< int >    particle_parent_track_id_;
    std::vector< int >    particle_pdg_code_;
    std::vector< double > particle_mass_;
    std::vector< double > particle_charge_;
    std::vector< int >    particle_process_key_;
    std::vector< int >    particle_total_occupancy_;

    std::vector< int >                particle_number_daughters_;
    std::vector< std::vector< int > > particle_daughter_track_ids_;

    std::vector< double > particle_initial_x_;
    std::vector< double > particle_initial_y_;
    std::vector< double > particle_initial_z_;
    std::vector< double > particle_initial_t_;

    std::vector< double > particle_initial_px_;
    std::vector< double > particle_initial_py_;
    std::vector< double > particle_initial_pz_;
    std::vector< double > particle_initial_energy_;

    std::vector< int >    hit_track_id_;
    std::vector< int >    hit_pdg_code_;
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

    // number of generator particles
    int generator_initial_number_particles_;
    int generator_intermediate_number_particles_;
    int generator_final_number_particles_;

    // vectors for initial generator particles
    std::vector< double > generator_initial_particle_x_;
    std::vector< double > generator_initial_particle_y_;
    std::vector< double > generator_initial_particle_z_;
    std::vector< double > generator_initial_particle_t_;
    std::vector< double > generator_initial_particle_px_;
    std::vector< double > generator_initial_particle_py_;
    std::vector< double > generator_initial_particle_pz_;
    std::vector< double > generator_initial_particle_energy_;
    std::vector< int >    generator_initial_particle_pdg_code_;
    std::vector< double > generator_initial_particle_mass_;
    std::vector< double > generator_initial_particle_charge_;

    // vectors for initial generator particles
    std::vector< double > generator_intermediate_particle_x_;
    std::vector< double > generator_intermediate_particle_y_;
    std::vector< double > generator_intermediate_particle_z_;
    std::vector< double > generator_intermediate_particle_t_;
    std::vector< double > generator_intermediate_particle_px_;
    std::vector< double > generator_intermediate_particle_py_;
    std::vector< double > generator_intermediate_particle_pz_;
    std::vector< double > generator_intermediate_particle_energy_;
    std::vector< int >    generator_intermediate_particle_pdg_code_;
    std::vector< double > generator_intermediate_particle_mass_;
    std::vector< double > generator_intermediate_particle_charge_;

    // vectors for final generator particles
    std::vector< double > generator_final_particle_x_;
    std::vector< double > generator_final_particle_y_;
    std::vector< double > generator_final_particle_z_;
    std::vector< double > generator_final_particle_t_;
    std::vector< double > generator_final_particle_px_;
    std::vector< double > generator_final_particle_py_;
    std::vector< double > generator_final_particle_pz_;
    std::vector< double > generator_final_particle_energy_;
    std::vector< int >    generator_final_particle_pdg_code_;
    std::vector< double > generator_final_particle_mass_;
    std::vector< double > generator_final_particle_charge_;

};

#endif
