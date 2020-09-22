// -----------------------------------------------------------------------------
//  MARLEYManager.h
//
//  Class definition of the MARLEY manager
//   * Author: Everybody is an author!
//   * Creation date: 7 August 2020
// -----------------------------------------------------------------------------

#ifndef MARLEYManager_h
#define MARLEYManager_h 1

// MARLEY includes
#include "marley/Generator.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"

class MARLEYManager {

    public:

        MARLEYManager();
        ~MARLEYManager();

        static MARLEYManager * Instance();

        void Initialize(std::string);
        void Get_Detector_Dimensions(double detector_x_, double detector_y_, double detector_z_);
        void Light_Em_Up(G4Event*);
        marley::Generator & Generator();

    protected:
        // GEANT4 dictionary of particles
        G4ParticleTable* particle_table_;

    private:

        static MARLEYManager * instance_;

        // MARLEY event generator object
        marley::Generator marley_generator_;

        double detector_length_x_;
        double detector_length_y_;
        double detector_length_z_;

};

#endif

