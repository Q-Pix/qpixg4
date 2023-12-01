// -----------------------------------------------------------------------------
//  MCTruthManager.h
//
//  Class definition of the MC truth manager
//   * Author: Everybody is an author!
//   * Creation date: 10 August 2020
// -----------------------------------------------------------------------------

#ifndef MCTruthManager_h
#define MCTruthManager_h 1

// Q-Pix includes
#include "GeneratorParticle.h"
#include "MCParticle.h"

// GEANT4 includes
#include "globals.hh"

// MARLEY includes
#include "marley/Particle.hh"

// ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

// C++ includes
#include <map>
#include <set>

class MCTruthManager {

    public:

        MCTruthManager();
        ~MCTruthManager();

        void EventReset();

        void SetRun(int const);
        void SetEvent(int const);

        static MCTruthManager* Instance();

        void AddInitialGeneratorParticle(GeneratorParticle *);
        void AddIntermediateGeneratorParticle(GeneratorParticle *);
        void AddFinalGeneratorParticle(GeneratorParticle *);

        void AddInitialMARLEYParticle(marley::Particle const &);
        void AddFinalMARLEYParticle(marley::Particle const &);

        void AddMCParticle(MCParticle *);
        MCParticle * GetMCParticle(int const);

        inline std::vector< GeneratorParticle * > GetInitialGeneratorParticles() const { return initial_generator_particles_; }
        inline std::vector< GeneratorParticle * > GetIntermediateGeneratorParticles() const { return intermediate_generator_particles_; }
        inline std::vector< GeneratorParticle * > GetFinalGeneratorParticles()   const { return final_generator_particles_;   }

        inline std::vector< marley::Particle > GetInitialMARLEYParticles() const { return initial_marley_particles_; }
        inline std::vector< marley::Particle > GetFinalMARLEYParticles()   const { return final_marley_particles_;   }

        inline std::map< int, MCParticle * > GetMCParticleMap() const { return mc_particle_map_; }

        //all particles of the event
        std::vector<int>    *GetParticlesID()                    {return _particle_track_id;}
        std::vector<int>    *GetParticlesParentID()              {return _particle_parent_track_id;}
        std::vector<int>    *GetParticlesPDG()                   {return _particle_pdg_code;}
        std::vector<double> *GetParticlesEnergy()                {return _particle_initial_energy;}
        std::vector<double> *GetParticlesMass()                  {return _particle_mass;}
        std::vector<double> *GetParticlesCharge()                {return _particle_charge;}
        std::vector<int>    *GetParticlesProcess()               {return _particle_process_key;}
        std::vector<double> *GetParticlesX()                     {return _particle_initial_x;}
        std::vector<double> *GetParticlesY()                     {return _particle_initial_y;}
        std::vector<double> *GetParticlesZ()                     {return _particle_initial_z;}
        std::vector<double> *GetParticlesT()                     {return _particle_initial_t;}
        std::vector<double> *GetParticlesPx()                    {return _particle_initial_px;}
        std::vector<double> *GetParticlesPy()                    {return _particle_initial_py;}
        std::vector<double> *GetParticlesPz()                    {return _particle_initial_pz;}
        std::vector<int>    *GetParticlesNDaughters()            {return _particle_number_daughters;}
        std::vector<std::vector<int>> *GetParticlesDaughtersIDs(){return _particle_daughter_track_ids;}

        typedef std::map<int,MCParticle*> MCParticleMap;

    private:

        static G4ThreadLocal MCTruthManager * instance_;

        int run_;
        int event_;

        // containers of generator particles
        std::vector< GeneratorParticle * > initial_generator_particles_;
        std::vector< GeneratorParticle * > intermediate_generator_particles_;
        std::vector< GeneratorParticle * > final_generator_particles_;

        // containers of MARLEY particles
        std::vector< marley::Particle > initial_marley_particles_;
        std::vector< marley::Particle > final_marley_particles_;

        // MC particle map
        // std::map< int, MCParticle > mc_particle_map_;
        std::map< int, MCParticle * > mc_particle_map_;

        //all particles of the event
        std::vector<int>    *_particle_track_id;
        std::vector<int>    *_particle_parent_track_id;
        std::vector<int>    *_particle_pdg_code;
        std::vector<double> *_particle_initial_energy;
        std::vector<double> *_particle_mass;
        std::vector<double> *_particle_charge;
        std::vector<int>    *_particle_process_key;
        std::vector<double> *_particle_initial_x;
        std::vector<double> *_particle_initial_y;
        std::vector<double> *_particle_initial_z;
        std::vector<double> *_particle_initial_t;
        std::vector<double> *_particle_initial_px;
        std::vector<double> *_particle_initial_py;
        std::vector<double> *_particle_initial_pz;
        std::vector<int>    *_particle_number_daughters;
        std::vector<std::vector<int>> *_particle_daughter_track_ids;

};

#endif
